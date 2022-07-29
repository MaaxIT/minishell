/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:58:36 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:58:55 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	error_and_exit(int x)
{
	print_error(0);
	exit(x);
}

static int	run_cmd_pipe(t_list **env, int pipefd[2], int tempfd, \
		t_cmd_lst *cmd, t_cmd_lst *top_cmd)
{
	if (close(pipefd[0]) == -1)
		error_and_exit(0);
	if (dup2(tempfd, STDIN_FILENO) == -1)
		error_and_exit(0);
	if (cmd->next && dup2(pipefd[1], STDOUT_FILENO) == -1)
		error_and_exit(0);
	if (close(pipefd[1]) == -1)
		error_and_exit(0);
	run_command(env, cmd, top_cmd);
	exit(9);
}

static int	adjust_fd(t_cmd_lst *cmd, int pipefd[2], int *readfd)
{
	if (cmd->input_path)
	{
		if (close(*readfd) == -1)
			return (0);
		*readfd = rd_input(cmd->input_path);
		if (*readfd == -1)
			return (0);
	}
	if (cmd->output_path)
	{
		if (close(pipefd[1]) == -1)
			return (0);
		if (cmd->output_type == 'R')
			pipefd[1] = rd_output(cmd->output_path);
		else if (cmd->output_type == 'A')
			pipefd[1] = rd_output_append(cmd->output_path);
		if (pipefd[1] == -1)
		{
			close(*readfd);
			return (0);
		}
	}
	return (9);
}

static int	close_fd(int ret, int fd1, int fd2, int fd3)
{
	if (fd1 != -1 && fd1 != STDIN_FILENO && fd1 != STDOUT_FILENO && \
		fd1 != STDERR_FILENO)
		close(fd1);
	if (fd2 != -1 && fd2 != STDIN_FILENO && fd2 != STDOUT_FILENO && \
		fd2 != STDERR_FILENO)
		close(fd2);
	if (fd3 != -1 && fd3 != STDIN_FILENO && fd3 != STDOUT_FILENO && \
		fd3 != STDERR_FILENO)
		close(fd3);
	return (ret);
}

int	ft_pipe(t_list **env, t_cmd_lst *cmd)
{
	int			pipefd[2];
	int			readfd;
	t_cmd_lst	*top_cmd;

	top_cmd = cmd;
	pipefd[0] = STDIN_FILENO;
	while (cmd)
	{
		readfd = dup(pipefd[0]);
		if (readfd == -1 || pipe(pipefd) == -1)
			return (close_fd(0, readfd, pipefd[0], pipefd[1]));
		if (!adjust_fd(cmd, pipefd, &readfd))
			return (close_fd(0, readfd, pipefd[0], pipefd[1]));
		g_pid = fork();
		if (!g_pid)
			run_cmd_pipe(env, pipefd, readfd, cmd, top_cmd);
		if (close(readfd) == -1 || close(pipefd[0]) == -1 || \
			close(pipefd[1]))
			return (close_fd(0, readfd, pipefd[0], pipefd[1]));
		waitpid(g_pid, NULL, 0);
		cmd = cmd->next;
	}
	close(readfd);
	return (9);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:58:36 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/05 11:31:38 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	adjust_fd(t_cmd_lst *cmd, int pipefd[2], int *tempfd)
{
	if (cmd->input_fd != -1)
	{
		close(*tempfd);
		*tempfd = cmd->output_fd;
	}
	if (cmd->output_fd != -1)
	{
		close(pipefd[1]);
		pipefd[1] = cmd->output_fd;
	}
}

static int	pipe2(int pipefd[2], t_list **env, t_cmd_lst *cmd[2], int tempfd)
{
	g_pid = fork();
	if (g_pid == -1)
		return (0);
	if (!g_pid)
	{
		close(pipefd[0]);
		if (dup2(tempfd, STDIN_FILENO) == -1)
			exit (0);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
			exit (0);
		run_command(env, cmd[0], cmd[1]);
		exit(errno);
	}
	else
		cmd[0]->pid = g_pid;
	return (9);
}

static int	ft_pipe_loop(t_list **env, t_cmd_lst *top_and_cmd[2], \
int pipefd[2], int *tempfd)
{
	int	err;

	while (top_and_cmd[0])
	{
		err = pipe(pipefd);
		if (err == -1)
			return (0);
		if (!((top_and_cmd[0])->next))
		{
			close(pipefd[1]);
			pipefd[1] = dup(STDOUT_FILENO);
			if (pipefd[1] == -1 && close(pipefd[0]) != 156)
				return (0);
		}
		adjust_fd(top_and_cmd[0], pipefd, tempfd);
		if (!pipe2(pipefd, env, top_and_cmd, *tempfd) && \
		close(pipefd[0]) != 156 && close(pipefd[1]) != 156)
			return (0);
		close(*tempfd);
		*tempfd = pipefd[0];
		close(pipefd[1]);
		top_and_cmd[0] = (top_and_cmd[0])->next;
	}
	return (9);
}

static void	wait_all_pids(t_cmd_lst *cmd)
{
	int	err;

	while (cmd)
	{
		if (cmd->pid != -1)
			waitpid(cmd->pid, &err, 0);
		cmd = cmd->next;
	}
	errno = err / 256;
}

int	ft_pipe(t_list **env, t_cmd_lst *cmd)
{
	int			pipefd[2];
	int			save_std[2];
	int			err;
	t_cmd_lst	*top_and_cmd[2];
	int			tempfd;

	top_and_cmd[0] = cmd;
	top_and_cmd[1] = cmd;
	tempfd = dup(STDIN_FILENO);
	if (tempfd == -1)
		return (0);
	if (!save_std_before_pipe(save_std))
	{
		close(tempfd);
		return (0);
	}
	err = ft_pipe_loop(env, top_and_cmd, pipefd, &tempfd);
	if (tempfd != STDIN_FILENO && tempfd != STDOUT_FILENO && \
	tempfd != STDERR_FILENO && tempfd >= 0)
		close(tempfd);
	wait_all_pids(top_and_cmd[1]);
	if (!clear_std_after_pipe(save_std) || err == 0)
		return (0);
	return (9);
}

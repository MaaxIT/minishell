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

static int	run_cmd_pipe(t_list **env, int pipefd[3], \
	t_cmd_lst *cmd, t_cmd_lst *top_cmd)
{
	if (close(pipefd[0]) == -1)
		error_and_exit(0);
	if (dup2(pipefd[2], STDIN_FILENO) == -1)
		error_and_exit(0);
	if (cmd->next && dup2(pipefd[1], STDOUT_FILENO) == -1)
		error_and_exit(0);
	if (close(pipefd[1]) == -1)
		error_and_exit(0);
	run_command(env, cmd, top_cmd);
	exit(9);
}

static int	adjust_fd(t_cmd_lst *cmd, int pipefd[3])
{
	if (cmd->input_path)
	{
		if (close(pipefd[2]) == -1)
			return (0);
		pipefd[2] = rd_input(cmd->input_path);
		if (pipefd[2] == -1)
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
			close(pipefd[2]);
			return (0);
		}
	}
	return (9);
}

static int	ft_pipe_loop(t_list **env, t_cmd_lst *top_cmd, \
	t_cmd_lst *cmd, int pipefd[3])
{
	if (pipe(pipefd) == -1)
		return (0);
	if (!adjust_fd(cmd, pipefd))
		return (0);
	g_pid = fork();
	if (g_pid == -1)
		return (0);
	if (!g_pid)
		run_cmd_pipe(env, pipefd, cmd, top_cmd);
	close(pipefd[2]);
	pipefd[2] = dup(pipefd[0]);
	if (pipefd[2] == -1)
		return (0);
	close(pipefd[0]);
	close(pipefd[1]);
	return (9);
}

int	ft_pipe(t_list **env, t_cmd_lst *cmd)
{
	int			pipefd[3];
	t_cmd_lst	*top_cmd;

	top_cmd = cmd;
	pipefd[2] = dup(STDIN_FILENO);
	if (pipefd[2] == -1)
		return (0);
	while (cmd)
	{
		if (!ft_pipe_loop(env, top_cmd, cmd, pipefd))
			break;
		if (waitpid(g_pid, NULL, 0) == -1)
			break;
		cmd = cmd->next;
	}
	close(pipefd[0]);
	close(pipefd[1]);
	close(pipefd[2]);
	return (9);
}

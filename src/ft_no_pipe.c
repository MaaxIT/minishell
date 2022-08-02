/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_no_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:51:36 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:51:52 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	close_fd(int x, int fd1, int fd2)
{
	if (fd1 != -1 && close(fd1) == -1)
	{
		if (fd2 != -1)
			close(fd2);
		return (0);
	}
	if (fd2 != -1 && close(fd2) == -1)
		return (0);
	return (x);
}

static int	adjust_fd(int *in, int *out, t_cmd_lst *cmd)
{
	if (cmd->input_fd != -1)
	{
		*in = dup(STDIN_FILENO);
		if (*in == -1)
			return (close_fd(0, *in, *out));
		if (dup2(cmd->input_fd, STDIN_FILENO) == -1)
			return (close_fd(0, *in, *out));
	}
	if (cmd->output_fd != -1)
	{
		*out = dup(STDOUT_FILENO);
		if (*out == -1)
			return (close_fd(0, *in, *out));
		if (dup2(cmd->output_fd, STDOUT_FILENO) == -1)
			return (close_fd(0, *in, *out));
	}
	return (9);
}

static int	run_no_pipe(t_list **env, t_cmd_lst *cmd)
{
	int	in;
	int	out;

	in = -1;
	out = -1;
	if (!adjust_fd(&in, &out, cmd))
		return (0);
	run_command(env, cmd, cmd);
	if (cmd->input_fd != -1 && dup2(in, STDIN_FILENO) == -1)
		return (close_fd(0, in, out));
	if (cmd->output_fd != -1 && dup2(out, STDOUT_FILENO) == -1)
		return (close_fd(0, in, out));
	close_fd(0, in, out);
	return (9);
}

int	no_pipe(t_list **env, t_cmd_lst *cmd)
{
	int	ret;

	ret = run_no_pipe(env, cmd);
	close_fd(0, cmd->input_fd, cmd->output_fd);
	return (ret);
}

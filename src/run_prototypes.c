/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_prototypes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:14:44 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:15:03 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_command(t_list **env, t_cmd_lst *cmd, t_cmd_lst *top_cmd)
{
	int		fd;
	int		err;
	int		len;

	fd = STDOUT_FILENO;
	len = ft_strlen(cmd->binary);
	if (!ft_strncmp(cmd->binary, "echo", len))
		err = bi_echo(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "cd", len))
		err = bi_cd(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "pwd", len))
		err = bi_pwd(fd);
	else if (!ft_strncmp(cmd->binary, "exit", len))
		err = bi_exit(fd, env, top_cmd);
	else if (!ft_strncmp(cmd->binary, "env", len))
		err = bi_env(fd, *env);
	else if (!ft_strncmp(cmd->binary, "export", len))
		err = bi_export(fd, env, cmd);
	else if (!ft_strncmp(cmd->binary, "unset", len))
		err = bi_unset(env, cmd);
	else
		err = exec_with_path(env, cmd->binary, cmd->arg_v);
	return (err);
}

int	run(t_list **env, t_cmd_lst *cmd)
{
	t_cmd_lst	*top_cmd;
	int			ret;

	if (!cmd)
		return (0);
	top_cmd = cmd;
	if (!cmd->next)
		ret = no_pipe(env, cmd);
	else
		ret = ft_pipe(env, top_cmd);
	return (ret);
}

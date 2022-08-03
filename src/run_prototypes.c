/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_prototypes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:14:44 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 21:25:54 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_command(t_list **env, t_cmd_lst *cmd, t_cmd_lst *top_cmd)
{
	int		fd;
	int		err;

	fd = STDOUT_FILENO;
	if (!ft_strncmp(cmd->binary, "echo", -1))
		err = bi_echo(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "cd", -1))
		err = bi_cd(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "pwd", -1))
		err = bi_pwd(fd);
	else if (!ft_strncmp(cmd->binary, "exit", -1))
		err = bi_exit(fd, env, top_cmd);
	else if (!ft_strncmp(cmd->binary, "env", -1))
		err = bi_env(fd, *env);
	else if (!ft_strncmp(cmd->binary, "export", -1))
		err = bi_export(env, cmd);
	else if (!ft_strncmp(cmd->binary, "unset", -1))
		err = bi_unset(env, cmd);
	else
		err = exec_with_path(env, cmd->binary, cmd->arg_v);
	return (err);
}

int	run(t_list **env, t_cmd_lst *cmd)
{
	int	ret;

	if (!cmd)
		return (0);
	if (errno != 130)
	{
		if (!cmd->next)
			ret = no_pipe(env, cmd);
		else
			ret = ft_pipe(env, cmd);
	}
	else
		ret = 9;
	return (ret);
}

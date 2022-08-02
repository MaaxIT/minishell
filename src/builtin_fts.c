/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fts.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:04:24 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/29 17:42:21 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_echo(int fd, t_cmd_lst *cmd)
{
	int	i;
	int	newline;

	i = 0;
	newline = 1;
	while (i < cmd->input_c)
	{
		ft_putstr_fd(fd, cmd->input_v[i]);
		if (cmd->input_v[i + 1])
			write(fd, " ", 1);
		if (cmd->options_v && cmd->options_v[i] && \
			!ft_strncmp(cmd->options_v[i], "-n", -1))
			newline = 0;
		i++;
	}
	if (newline)
		write(fd, "\n", 1);
	return (9);
}

int	bi_cd(int fd, t_cmd_lst *cmd)
{
	int	err;

	(void)fd;
	if (!cmd->input_v || !cmd->input_v[0])
		return (0);
	err = chdir(cmd->input_v[0]);
	if (err == -1)
	{
		ft_putstr_fd(STDERR_FILENO, "SuperShell: cd: ");
		ft_putstr_fd(STDERR_FILENO, cmd->input_v[0]);
		ft_putstr_fd(STDERR_FILENO, ": ");
		print_error(0);
		errno = 1;
		return (0);
	}
	return (9);
}

int	bi_pwd(int fd)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		print_error(0);
		return (0);
	}
	ft_putstr_fd(fd, cwd);
	write(fd, "\n", 1);
	free(cwd);
	return (9);
}

int	bi_exit(int fd, t_list **env, t_cmd_lst *top_cmd)
{
	long int 	ex;
	t_cmd_lst	*cmd;

	(void)fd;
	cmd = top_cmd;
	ex = 0;
	while (cmd && cmd->binary && ft_strncmp(cmd->binary, "exit", -1))
		cmd = cmd->next;
	if (cmd && cmd->binary && !ft_strncmp(cmd->binary, "exit", -1) && \
		cmd->input_v && cmd->input_v[0])
		ex = ft_atol(cmd->input_v[0]);
	if (ex < 0 || ex > 2147483647)
	{
		ft_putstr_fd(STDERR_FILENO, \
			"Supershell: exit: Illegal number: ");
		ft_putstr_fd(STDERR_FILENO, cmd->input_v[0]);
		ft_putstr_fd(STDERR_FILENO, "\n");
		return (0);
	}
	ft_lstclear(env);
	free_command_lst(top_cmd);
	rl_clear_history();
	ft_putstr_fd(STDERR_FILENO, "exit\n");
	exit(ex);
}

int	bi_env(int fd, t_list *env)
{
	env = env->next;
	while (env)
	{
		if (env->id && env->value)
		{
			ft_putstr_fd(fd, env->id);
			write(fd, "=", 1);
			ft_putstr_fd(fd, env->value);
			write(fd, "\n", 1);
		}
		env = env->next;
	}
	return (9);
}

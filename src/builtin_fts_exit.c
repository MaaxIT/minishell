/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fts_exit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl </var/mail/mbennafl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 13:34:29 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/04 20:38:27 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_clear(t_list **env, t_cmd_lst *top_cmd)
{
	ft_lstclear(env);
	free_command_lst(top_cmd);
	rl_clear_history();
}

static int	bi_exit_execute(t_list **env, t_cmd_lst *top_cmd, \
const char *str)
{
	int			ex;
	t_cmd_lst	*cmd;

	cmd = top_cmd;
	ex = 0;
	while (cmd && cmd->binary && ft_strncmp(cmd->binary, "exit", -1))
		cmd = cmd->next;
	if (cmd && cmd->binary && !ft_strncmp(cmd->binary, "exit", -1))
		ex = ft_atoll_mod_256(str);
	ft_putstr_fd(STDERR_FILENO, "exit\n");
	if (ex == -1 && errno == -735)
	{
		errno = 0;
		ft_putstr_fd(STDERR_FILENO, "Supershell: exit: ");
		ft_putstr_fd(STDERR_FILENO, str);
		ft_putstr_fd(STDERR_FILENO, ": numeric argument required\n");
		exit_clear(env, top_cmd);
		exit(255);
	}
	exit_clear(env, top_cmd);
	exit(ex);
}

int	bi_exit(int fd, t_list **env, t_cmd_lst *top_cmd)
{
	t_cmd_lst	*cmd;

	(void)fd;
	cmd = top_cmd;
	while (cmd && cmd->binary && ft_strncmp(cmd->binary, "exit", -1))
		cmd = cmd->next;
	if (!cmd)
	{
		exit_clear(env, top_cmd);
		ft_putstr_fd(STDERR_FILENO, "exit\n");
		exit (0);
	}
	if (cmd->input_c + cmd->options_c > 1)
	{
		ft_putstr_fd(STDERR_FILENO, "exit: too many arguments\n");
		return (9);
	}
	if (cmd->input_v && cmd->input_v[0])
		return (bi_exit_execute(env, top_cmd, cmd->input_v[0]));
	else if (cmd->options_v && cmd->options_v[0])
		return (bi_exit_execute(env, top_cmd, cmd->options_v[0]));
	exit_clear(env, top_cmd);
	ft_putstr_fd(STDERR_FILENO, "exit\n");
	exit (0);
}

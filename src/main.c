/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 21:53:34 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	treat_and_call_cmd(t_list **env, char *cmd)
{
	int	fd;

	(void)cmd;
	(void)fd;
	(void)env;

	fd = 1;
	return (9);
}

int	new_cmd(t_list **env)
{
	char	*cmd;

	cmd = readline(SHELL_PREFIX); //PROTECT AGAINST READLINE ERRORS?
	if (!cmd)
		return (0);
	if (!treat_and_call_cmd(env, cmd))
		print_error(0);		//IS THAT ENOUGH?
	new_cmd(env);
	return (9);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_list	*env;

	argc = (int)argc;
	argv = (char **)argv;
	env = NULL;
	init_env_list(&env, envp); // Still need to manage errors
	signals_init();
	if (!new_cmd(&env))
		return (0);//TREAT ERROR WITH ERRNO
	return (0);
}

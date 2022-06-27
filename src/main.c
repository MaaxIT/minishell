/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/28 01:21:51 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	treat_and_call_cmd(t_list **env, char *cmd)
{
	int			fd;
	int			idx;
	t_command	*command;

	/* Just an example */
	command = parse_cmd(cmd);
	if (!command)
		return (0);
	idx = 0;
	printf("------\nNew command executed\nBinary command: '%s'\n", command->bin);
	while (idx < command->argc)
	{
		printf("Argument n°%d: %s\n", idx + 1, command->argv[idx]);
		idx++;
	}
	/* Just an example */
	
	// Now that we don't need it anymore, free it
	free_command(command);
	printf("Command freed\n------\n");

	fd = 1;
	(void)env;
	fd = (int)fd;
	return (9);
}

int	new_cmd(t_list **env)
{
	char	*cmd;

	cmd = readline(SHELL_PREFIX); //PROTECT AGAINST READLINE ERRORS?
	if (!cmd)
		bi_exit(-1, env);
	add_history(cmd);
	if (!treat_and_call_cmd(env, cmd))
		print_error(0);		//IS THAT ENOUGH?
	update_env_return(env);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/28 02:55:56 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void test_to_delete(t_command *command)
{
	int	idx;
	
	idx = 0;
	printf("------\nNew command executed\nBinary command: '%s'\n", command->bin);
	while (idx < command->argc)
	{
		printf("Argument n°%d: %s\n", idx + 1, command->argv[idx]);
		idx++;
	}
	printf("Command freed\n------\n");
}

static int	treat_and_call_cmd(t_list **env, char *cmd)
{
	t_command	*command;

	command = parse_cmd(cmd);
	if (!command)
		return (0);
	test_to_delete(command);

	/* TODO: Add command execution here
	As said in the bash man, in the case there is a '/' in the command, it should not search in the path
	Otherwise, if there is one, it must search for a valid program
	*/
	if (ft_strincludes(command->bin, '/'))
	{}	// Don't search
	else
	{}	// Search

	free_command(command);

	(void)env;
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
	free(cmd);
	if (!update_env_return(env))
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

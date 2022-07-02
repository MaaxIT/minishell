/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maaxit <maaxit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/01 06:13:39 by maaxit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	run_command(t_list **env, char *line)
{
	(void)env;(void)line;

/*	Parsing test	*/
/*	t_command	*cmd_t;

	cmd_t = initialize_comand(line);
	if (!cmd_t)
		return (0);

	printf("- Original: %s\n", cmd_t->original);
	printf("- Binary: %s\n", cmd_t->binary);
	printf("- Options count: %i\n", cmd_t->options_c);
	printf("- Options first value: %s\n", (cmd_t->options_v ? cmd_t->options_v[0] : "No options"));
	printf("- Input count: %i\n", cmd_t->input_c);
	printf("- Input first value: %s\n", (cmd_t->input_v ? cmd_t->input_v[0] : "No input"));
	printf("- Args count: %i\n", cmd_t->arg_c);
	printf("- Args first value: %s\n", (cmd_t->arg_v ? cmd_t->arg_v[0] : "No args"));
	free_command(cmd_t);*/
/*	Pars		*/

/*	Pipe test	*/
	char	*argv1[3];
	argv1[0] = "ls"; argv1[1] = "-l"; argv1[2] = 0;
	char	*argv2[3];
	argv2[0] = "wc"; argv2[1] = "-l"; argv2[2] = 0;
	ft_pipe(env, argv1, argv2);
/*	Pipe		*/

	return (1);
}

int	new_cmd(t_list **env)
{
	char	*cmd;

	cmd = readline(SHELL_PREFIX); //PROTECT AGAINST READLINE ERRORS?
	if (!cmd)
		bi_exit(-1, env);
	add_history(cmd);
	if (!run_command(env, cmd))
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

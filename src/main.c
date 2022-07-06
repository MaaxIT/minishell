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

void	parsing_test_to_del(t_cmd_lst *cmd_t)
{
/*	Parsing test	*/

	printf("- Original: %s\n", cmd_t->original);
	printf("- Binary: %s\n", cmd_t->binary);
	printf("- Options count: %i\n", cmd_t->options_c);
	printf("- Options first value: %s\n", (cmd_t->options_v ? cmd_t->options_v[0] : "No options"));
	printf("- Input count: %i\n", cmd_t->input_c);
	printf("- Input first value: %s\n", (cmd_t->input_v ? cmd_t->input_v[0] : "No input"));
	printf("- Args count: %i\n", cmd_t->arg_c);
	printf("- Args first value: %s\n", (cmd_t->arg_v ? cmd_t->arg_v[0] : "No args"));
	free_command(cmd_t);
/*	Pars		*/
}

void	pipe_test_to_del(t_list **env)
{
/*	Pipe test	*/
	t_cmd_lst	*cmd1;
	t_cmd_lst	*cmd2;
	t_cmd_lst	*cmd3;

	cmd1 = initialize_command("ls");
	cmd2 = initialize_command("grep mini");
	cmd3 = initialize_command("wc -l");
	cmd1->next = cmd2;
	cmd2->next = cmd3;
	cmd3->next = 0;
	ft_pipe(env, cmd1);
/*	Pipe		*/
}


int	new_cmd(t_list **env)
{
	char	*cmd_str;
	t_cmd_lst	*cmd;

	cmd_str = readline(SHELL_PREFIX); //PROTECT AGAINST READLINE ERRORS?
	if (!cmd_str)
		bi_exit(-1, env);
	add_history(cmd_str);
	cmd = initialize_command(cmd_str);
	if (!cmd)
		print_error(0);
	if (cmd && !run_command(env, cmd))
		print_error(0);		//IS THAT ENOUGH?
	if (cmd_str)
		free(cmd_str);
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

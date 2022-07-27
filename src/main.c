/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/24 21:33:59 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_and_run(t_list **env, char *cmd_str)
{
	t_cmd_lst	*cmd_t;

	add_history(cmd_str);//PROTECT?
	cmd_t = initialize_command(cmd_str, *env);
	if (!cmd_t)
		print_error(0);
	if (!cmd_t->binary || ft_is_a_whitespace_or_empty_string(cmd_t->binary))
		return;
	if (cmd_t && !run(env, cmd_t))
		print_error(0); //IS THAT ENOUGH?
	if (cmd_t)
		free_command_lst(cmd_t);
	if (!update_env_return(env))
		print_error(0); //IS THAT ENOUGH?
}
 
static int	new_cmd(t_list **env)
{
	char	*cmd_str;

	g_pid = 0;
	cmd_str = readline(SHELL_PREFIX); //PROTECT AGAINST READLINE ERRORS?
	if (!cmd_str)
		bi_exit(1, env, NULL);
	if (!ft_is_a_whitespace_or_empty_string(cmd_str))
		parse_and_run(env, cmd_str);
	if (cmd_str)
		free(cmd_str);
	if (env)
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

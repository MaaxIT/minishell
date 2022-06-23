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

	// Just to test, remove that shit
	if (ft_strncmp(cmd, "env", 3) == 0)
	{
		t_list *head;
		head = *env;
		while (head->next)
		{
			printf("\033[1;35m%s\033[0;31m=\033[0;34m%s\033[m\n", head->id, head->value);
			head = head->next;
		}
	}

	// TESTS
	// fd = rd_output_append("./test");
	// if (fd == -1)
	// 	return (0);
	// if (!bi_echo(fd, 1, "It\'s an echo"))
	// 	return (0);
	// if (close(fd) == -1)
	// 	return (0);
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

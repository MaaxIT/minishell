/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/04 14:55:01 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	parse_and_run(t_list **env, char **cmd_str)
{
	t_cmd_lst	*cmd_t;

	add_history(*cmd_str);
	cmd_t = initialize_command(*cmd_str, *env);
	if (!cmd_t)
		return ;
	free(*cmd_str);
	*cmd_str = NULL;
	if (!cmd_t->binary || ft_is_a_whitespace_or_empty_string(cmd_t->binary))
	{
		errno = 0;
		if (!update_env_return(env))
			print_error(0);
		return ;
	}
	if (cmd_t && !run(env, cmd_t))
		print_error(0);
	if (cmd_t)
		free_command_lst(cmd_t);
	if (!update_env_return(env))
		print_error(0);
}

int	trim_whitespaces(char **str)
{
	char	*cpy;
	int		start;
	int		end;

	if (!(*str) || ft_strlen(*str) == 0)
		return (0);
	start = 0;
	while ((*str)[start] && ((*str)[start] == 32 || \
				((*str)[start] >= 9 && (*str)[start] <= 13)))
		start++;
	end = ft_strlen(*str) - 1;
	while ((*str)[end] && ((*str)[end] == 32 || \
				((*str)[end] >= 9 && (*str)[end] <= 13)))
		end--;
	cpy = ft_strndup(*str + start, (end - start + 1));
	free(*str);
	if (!cpy)
		return (-1);
	*str = cpy;
	return (0);
}

static int	new_cmd(t_list **env)
{
	char	*cmd_str;

	g_pid = -2;
	cmd_str = readline(SHELL_PREFIX);
	if (!cmd_str)
		bi_exit(1, env, NULL);
	if (!ft_is_a_whitespace_or_empty_string(cmd_str))
	{
		if (trim_whitespaces(&cmd_str) == -1)
			bi_exit(1, env, NULL);
		parse_and_run(env, &cmd_str);
	}
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
	if (argc != 1 && argv && argv[1])
	{
		ft_putstr_fd(STDOUT_FILENO, "Supershell: ");
		ft_putstr_fd(STDOUT_FILENO, "Can't open ");
		ft_putstr_fd(STDOUT_FILENO, argv[1]);
		ft_putstr_fd(STDOUT_FILENO, "\n");
		exit(127);
	}
	if (init_env_list(&env, envp) == -1)
		return (-1);
	if (!signals_init())
	{
		print_error(0);
		return (0);
	}
	if (!new_cmd(&env))
		return (0);
	return (0);
}

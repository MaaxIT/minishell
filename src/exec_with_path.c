/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:47:07 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 20:33:35 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_the_right_path_index(char **paths)
{
	int	i;

	i = 0;
	while (paths[i] && access(paths[i], F_OK) == -1)
		i++;
	return (i);
}

static int	exec(const char *path, char **argv, char **envp)
{
	int	err;

	g_pid = fork(); // NOT SURE OF THIS TRICK AND NEEDA PROTECT
	err = 0;
	if (g_pid == 0)
		err = execve(path, argv, envp); // PROTECT FROM EXECVE ERRORS
	else
		waitpid(g_pid, NULL, 0); // PROTECT FROM WAITPID ERRORS
	if (err == -1)
	{
		print_error(0);
		return (0);
	}
	else
		errno = 0;
	return (9);
}

static int	relative_path(t_list **env, const char *cmd, char **argv)
{
	char	**paths;
	char	**envp;
	int		i;

	paths = find_paths(*env, cmd);
	envp = NULL;
	if (!paths)
		return (0); //ENOUGH? Not in fd?
	i = get_the_right_path_index(paths);
	if (access(paths[i], F_OK) == 0)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (ewp_clear(0, paths, NULL));
		exec(paths[i], argv, envp);
	}
	else
	{
		ft_putstr_fd(STDERR_FILENO, cmd); // PROTECT
		ft_putstr_fd(STDERR_FILENO, ": command not found\n"); // PROTECT
		errno = 127;
	}
	return (ewp_clear(9, paths, envp));
}

int	exec_with_path(t_list **env, const char *cmd, char **argv)
{
	char	**envp;

	envp = NULL;
	if (cmd && cmd[0] == '/' && access(cmd, F_OK) == 0)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (0);
		exec(cmd, argv, envp);
	}
	else
	{
		if (!relative_path(env, cmd, argv))
			return (0);
	}
	return (ewp_clear(9, envp, NULL));
}

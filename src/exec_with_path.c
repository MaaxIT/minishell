/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:47:07 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 21:20:20 by mbennafl         ###   ########.fr       */
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
	int	execve_ret;
	int	err;

	g_pid = fork();
	if (g_pid == -1)
		return (0);
	execve_ret = 0;
	if (g_pid == 0)
		execve_ret = execve(path, argv, envp);
	else
	{
		if (waitpid(g_pid, &err, 0) == -1)
			return (0);
		if (errno != 128 + SIGQUIT && errno != 128 + SIGINT)
			errno = err / 256;
	}
	if (execve_ret == -1)
	{
		print_error(0);
		return (0);
	}
	return (9);
}

static void	ft_not_found(const char *cmd)
{
	ft_putstr_fd(STDERR_FILENO, "SuperShell: ");
	ft_putstr_fd(STDERR_FILENO, cmd);
	ft_putstr_fd(STDERR_FILENO, ": command not found\n");
	errno = 127;
}

static int	relative_path(t_list **env, const char *cmd, char **argv)
{
	char	**paths;
	char	**envp;
	int		i;

	paths = find_paths(*env, cmd);
	if (!paths)
	{
		ft_not_found(cmd);
		return (0);
	}
	envp = NULL;
	i = get_the_right_path_index(paths);
	if (access(paths[i], F_OK) == 0)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (ewp_clear(0, paths, NULL));
		exec(paths[i], argv, envp);
	}
	else
		ft_not_found(cmd);
	return (ewp_clear(9, paths, envp));
}

int	exec_with_path(t_list **env, const char *cmd, char **argv)
{
	char	**envp;

	envp = NULL;
	if (cmd && ft_strlensep(cmd, '/') != ft_strlen(cmd) && access(cmd, F_OK) == 0)
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

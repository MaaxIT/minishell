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

	g_pid = fork();		// NOT SURE OF THIS TRICK AND NEEDA PROTECT
	err = 0;
	if (g_pid == 0)
		err = execve(path, argv, envp); // PROTECT FROM EXECVE ERRORS
	else
		waitpid(g_pid, NULL, 0); // PROTECT FROM WAITPID ERRORS
	if (err ==  -1)
	{
		print_error(0);
		return (0);
	}
	return (9);
}

int	exec_with_path(t_list **env, const char *cmd, char **argv)
{
	char	**paths;
	int	i;
	char	**envp;

	envp = NULL;
	paths = 0;
	if (cmd && cmd[0] == '/' && access(cmd, F_OK) == 0)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (ewp_clear(0, paths, NULL));
		exec(cmd, argv, envp);
	}
	else
	{
		paths = find_paths(*env, cmd);
		if (!paths)
			return (print_error(0)); //ENOUGH? Not in fd?
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
			ft_putstr_fd(STDOUT_FILENO, cmd); // PROTECT
			ft_putstr_fd(STDOUT_FILENO, ": command not found\n"); // PROTECT
			errno = 127;
		}
	}
	return (ewp_clear(9, paths, envp));
}

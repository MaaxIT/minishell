#include "minishell.h"

static int	get_the_right_path_index(char **paths)
{
	int	i;

	i = 0;
	while (paths[i] && access(paths[i], F_OK) == -1)
		i++;
	return (i);
}

static int	exec(char *path, char **argv, char **envp)
{
	pid_t	pid;

	pid = fork();		// NOT SURE OF THIS TRICK AND NEEDA PROTECT
	if (pid == 0)
		execve(path, argv, envp); // PROTECT FROM EXECVE ERRORS
	else
		waitpid(pid, NULL, 0); // PROTECT FROM WAITPID ERRORS
	return (9);
}

int	exec_with_path(t_list **env, const char *cmd, char **argv)
{
	char	**paths;
	int	i;
	int	err;
	char	**envp;

	paths = find_paths(*env, cmd);
	if (!paths)
		return (print_error(0)); //ENOUGH? Not in fd?
	i = get_the_right_path_index(paths);
	if (paths[i] && access(paths[i], F_OK) != -1)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (ewp_clear(0, paths, NULL));
		err = exec(paths[i], argv, envp);
		if (err == 0)
			return (0); // ENOUGH? FREE?
	}
	else
	{
		ft_putstr_fd(1, strerror(127));
		return (ewp_clear(9, paths, NULL));
	}
	return (ewp_clear(9, paths, envp));
}

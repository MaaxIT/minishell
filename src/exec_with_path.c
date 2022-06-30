#include "minishell.h"

static int	get_the_right_path(char **paths)
{
	int	i;

	i = 0;
	while (paths[i] && access(paths[i], F_OK) == -1)
		i++;
	return (i);
}

int	exec_with_path(t_list **env, const char *cmd, char **argv)
{
	char	**paths;
	char	**envp;
	int	i;
	pid_t	pid;

	paths = find_paths(*env, cmd);
	if (!paths)
		return (print_error(0)); //ENOUGH? Not in fd?
	i = get_the_right_path(paths);
	if (paths[i] && access(paths[i], F_OK) != -1)
	{
		envp = create_envp((*env)->next);
		if (!envp)
			return (ewp_clear(0, paths, 0));
		pid = fork();	// NOT SURE OF THIS TRICK and needa protect
		if (pid == 0)
			execve(paths[i], argv, envp); // PROTECT FROM EXECVE ERRORS
		else
			waitpid(pid, NULL, 0); // PROTECT FROM WAITPID ERRORS
	}
	else
	{
		ft_putstr_fd(1, strerror(127));
		return (ewp_clear(9, paths, 0));
	}
	return (ewp_clear(9, paths, envp));
}

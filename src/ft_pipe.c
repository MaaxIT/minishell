#include "minishell.h"

static int	first_cmd(t_list **env, int pipefd[2], char **argv)
{
	if (close(pipefd[0]) == -1)
		return (0);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (0);
	if (close(pipefd[1]) == -1)
		return (0);
	exec_with_path(env, argv[0], argv);
	exit(0);
}

static int	second_cmd(t_list **env, int pipefd[2], char **argv)
{
	if (close(pipefd[1]) == -1)
		return (0);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
		return (0);
	if (close(pipefd[0]) == -1)
		return (0);
	exec_with_path(env, argv[0], argv);
	exit(0);
}

int	ft_pipe(t_list **env, char **argv1, char **argv2)
{
	int	pipefd[2];
	int	pid[2];

	if (pipe(pipefd) == -1)
		return (0); // ENOUGH?
	pid[0] = fork();
	if (pid[0] == 0)
		first_cmd(env, pipefd, argv1); // NEEDA PROTECT
	pid[1] = fork();
	if (pid[1] == 0)
		second_cmd(env, pipefd, argv2); // NEEDA PROTECT
	if (close(pipefd[1]) == -1)
		return (0);
	if (close(pipefd[0]) == -1)
		return (0);
	waitpid(pid[0], NULL, 0);
	waitpid(pid[1], NULL, 0);
	return (9);
}

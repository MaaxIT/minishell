#include "minishell.h"

int	first_cmd(int pipefd[2], char **argv)
{
(void)argv;

	if (close(pipefd[0]) == -1)
		return (0);
	if (dup2(pipefd[1], 1) == -1)
		return (0);;
	// Execute cmd with execve
	return (9);
}

int	second_cmd(int pipefd[2], char **argv)
{
(void)argv;

	if (close(pipefd[1]) == -1)
		return (0);
	if (dup2(pipefd[0], 0) == -1)
		return (0);
	// Execute cmd with execve
	return (9);
}

int	ft_pipe(char **argv1, char **argv2)
{
	int	pipefd[2];
	int	pid;
	int	stdin_fd;

	if (pipe(pipefd) == -1)
		return (0); // ENOUGH?
	stdin_fd = dup(1);
	pid = fork();
	if (pid != 0 && first_cmd(pipefd, argv1) == 0)
		return (0);
	else if (pid == 0 && second_cmd(pipefd, argv2) == 0)
		return (0);
	dup2(stdin_fd, 1);
	return (9);
}

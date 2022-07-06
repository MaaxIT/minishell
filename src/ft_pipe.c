#include "minishell.h"

static int	run_cmd_pipe(t_list **env, int pipefd[2], int tempfd, t_cmd_lst *cmd)
{
	if (close(pipefd[0]) == -1)
		return (0);
	if (dup2(tempfd, STDIN_FILENO) == -1)
		return (0);
	if (cmd->next && dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (0);
	if (close(pipefd[1]) == -1)
		return (0);
	run_command(env, cmd);
	exit(9);
}

int	ft_pipe(t_list **env, t_cmd_lst *cmd)
{
	int	pipefd[2];
	int	pid;
	int	tempfd;

	tempfd = dup(STDIN_FILENO);
	while (cmd)
	{
		if (pipe(pipefd) == -1)
			return (0); // ENOUGH?
		pid = fork();
		if (!pid)
			run_cmd_pipe(env, pipefd, tempfd, cmd);
		close(tempfd);
		tempfd = dup(pipefd[0]);
		close(pipefd[0]);
		close(pipefd[1]);
		waitpid(pid, NULL, 0);
		cmd = cmd->next;
	}
	close(tempfd);
	return (0);
}

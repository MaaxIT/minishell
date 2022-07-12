#include "minishell.h"

static int	close_fd(int fd1, int fd2)
{
	if (fd1 != -1 && close(fd1) == -1)
	{
		if (fd2 != -1)
			close(fd2);
		return (0);
	}
	if (fd2 != -1 && close(fd2) == -1)
		return (0);
	return (9);
}

static int	get_out_fd(t_cmd_lst *cmd)
{
	if (cmd->output_type == 'R')
		return (rd_output(cmd->output_path));
	else
		return (rd_output_append(cmd->output_path));
}

static int	run_no_pipe(t_list **env, t_cmd_lst *cmd, int in_fd, int out_fd)
{
//	int	pid;
	int	in;
	int	out;

	in = dup(STDIN_FILENO);
	out = dup(STDOUT_FILENO);

/*	if (ft_strncmp(cmd->binary, "exit", ft_strlen("exit")))
	{
		pid = fork();
		if (pid == -1)
			return (0);
		if (!pid)
		{
*/			dup2(in_fd, STDIN_FILENO);
			dup2(out_fd, STDOUT_FILENO); // PROTECTION FOR ALL THIS
/*			exit(run_command(env, cmd, cmd)); // HOW TO GET RETURN VALUE FOR ERRORS????
		}
		waitpid(pid, NULL, 0);
	}
	else
		bi_exit(out_fd, env, cmd);
*/		
	run_command(env, cmd, cmd);
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	return (9);
}

int no_pipe(t_list **env, t_cmd_lst *cmd)
{
	int	in_fd;
	int	out_fd;
	int	ret;

	in_fd = rd_input(cmd->input_path);
	if (in_fd == -1)
		return (0); // RNOUGH?
	out_fd = get_out_fd(cmd);
	if (out_fd == -1)
	{
		close(in_fd); // ENOUGH?
		return (0);
	}
	ret = run_no_pipe(env, cmd, in_fd, out_fd);
	close_fd(in_fd, out_fd); // PROTECTION
	return (ret);
}

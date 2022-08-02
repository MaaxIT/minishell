#include "minishell.h"

static int	open_input(t_cmd_lst *cmd)
{
	if (cmd->input_type == 'D')
	{
		cmd->input_fd = rd_input(cmd->input_path);
		if (cmd->input_fd == -1)
			return (0);
	}
	else if (cmd->input_type == 'C')
	{
		cmd->input_fd = rd_input(cmd->input_path);
		if (cmd->input_fd == -1)
			return (0);
	}
	return (9);
}

static int	open_output(t_cmd_lst *cmd)
{
	if (cmd->output_type == 'R')
	{
		cmd->output_fd = rd_output(cmd->output_path);
		if (cmd->output_fd == -1)
			return (0);
	}
	else if (cmd->output_type == 'A')
	{
		cmd->output_fd = rd_output_append(cmd->output_path);
		if (cmd->output_fd == -1)
			return (0);
	}
	return (9);
}

int	open_redirections(t_cmd_lst *cmd)
{
	if (cmd->input_path && !open_input(cmd))
		return (0);
	if (cmd->output_path && !open_output(cmd))
		return (0);
	return (9);
}

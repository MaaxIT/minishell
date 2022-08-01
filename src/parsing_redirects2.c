#include "minishell.h"

int	input_not_existing(int x, char *input_path)
{
	ft_putstr_fd(STDOUT_FILENO, "Supershell: ");
	ft_putstr_fd(STDOUT_FILENO, input_path);
	ft_putstr_fd(STDOUT_FILENO, ": ");
	print_error(0);
	return (x);
}

int	concat_callback(t_cmd_lst *cmd_t, int *idx, int len, int *i)
{
	int	k;
	int	offset;
	int	rtrn;

	offset = 0;
	if (cmd_t->output_type == 'A')
		offset = -1;
	k = *idx - offset;
	while (k < len)
	{
		rtrn = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx - offset);
		if (rtrn == -1)
			return (-1);
		else if (rtrn == 1)
		{
			(*i)--;
			break ;
		}
		k++;
	}
	return (0);
}

int	gen_path_concat(t_cmd_lst *cmd_t, char **path_type)
{
	int	fd;

	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		rd_delimiter(cmd_t->input_path);
		free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	else
	{
		if (path_type == &cmd_t->input_path)
			fd = open(*path_type, O_RDONLY, 0644);
		else
			fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
		else if (path_type == &cmd_t->input_path)
			return (-1);
	}
	return (0);
}

int	separated_callback(t_cmd_lst *cmd_t, char **path_type, int i, int *idx)
{
	int	res;
	
	res = replace_sub(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "");
	if (res < 0)
		return (-1);
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		if (res == 0)
			free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	*idx = 0;
	return (0);
}

int	gen_path_separated(t_cmd_lst *cmd_t, char **path_type)
{
	int	fd;
	
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
		rd_delimiter(cmd_t->input_path);
	else
	{
		if (path_type == &cmd_t->input_path)
			fd = open(*path_type, O_RDONLY, 0644);
		else
			fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
		else if (path_type == &cmd_t->input_path)
			return (-1);
	}
	return (0);
}
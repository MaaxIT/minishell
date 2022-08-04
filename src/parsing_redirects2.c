/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirects2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 00:00:22 by maxime            #+#    #+#             */
/*   Updated: 2022/08/04 02:03:48 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_not_existing(int x, char *input_path)
{
	ft_putstr_fd(STDOUT_FILENO, "Supershell: ");
	ft_putstr_fd(STDOUT_FILENO, input_path);
	ft_putstr_fd(STDOUT_FILENO, ": ");
	print_error(0);
	errno = 1;
	return (x);
}

int	concat_callback(t_cmd_lst *cmd_t, int *idx, int len, int *i)
{
	int	k;
	int	rtrn;

	k = *idx;
	while (k < len)
	{
		rtrn = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx, 1);
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
	if (path_type == &cmd_t->input_path)
	{
		if (cmd_t->input_fd != -1)
			close(cmd_t->input_fd);
		if (cmd_t->input_type == 'D')
			cmd_t->input_fd = rd_delimiter(*path_type);
		else if (cmd_t->input_type == 'C')
			cmd_t->input_fd = rd_input(*path_type);
		if (cmd_t->input_fd == -1)
			return (-1);
	}
	else if (path_type == &cmd_t->output_path)
	{
		if (cmd_t->output_fd != -1)
			close(cmd_t->output_fd);
		if (cmd_t->output_type == 'R')
			cmd_t->output_fd = rd_output(*path_type);
		else if (cmd_t->output_type == 'A')
			cmd_t->output_fd = rd_output_append(*path_type);
	}
	return (0);
}

int	separated_callback(t_cmd_lst *cmd_t, char **path_type, int i, int *idx)
{
	int	res;

	res = replace_sub(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "", 1);
	if (res < 0)
		return (-1);
	if (path_type == &cmd_t->input_path && cmd_t->input_type == 'D')
	{
		if (res == 0)
			free(*path_type);
		*path_type = NULL;
	}
	*idx = 0;
	return (0);
}

int	gen_path_separated(t_cmd_lst *cmd_t, char **path_type)
{
	if (path_type == &cmd_t->input_path)
	{
		if (cmd_t->input_fd != -1)
			close(cmd_t->input_fd);
		if (cmd_t->input_type == 'D')
			cmd_t->input_fd = rd_delimiter(*path_type);
		else if (cmd_t->input_type == 'C')
			cmd_t->input_fd = rd_input(*path_type);
		if (cmd_t->input_fd == -1)
			return (-1);
	}
	else if (path_type == &cmd_t->output_path)
	{
		if (cmd_t->output_fd != -1)
			close(cmd_t->output_fd);
		if (cmd_t->output_type == 'R')
			cmd_t->output_fd = rd_output(*path_type);
		else if (cmd_t->output_type == 'A')
			cmd_t->output_fd = rd_output_append(*path_type);
	}
	return (0);
}

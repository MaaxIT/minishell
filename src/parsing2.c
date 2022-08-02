/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/03 00:21:50 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Remove an empty argument from arrays while synchronizing */
int	sync_arg_empty(t_cmd_lst *cmd_t, int idx, int input_idx, int is_bin)
{
	if (cmd_t->parsing_v && cmd_t->parsing_v[idx])
	{
		cmd_t->parsing_v = ft_pop(cmd_t->parsing_v, idx, cmd_t->arg_c);
		if (!cmd_t->parsing_v)
			return (-1);
	}
	cmd_t->arg_v = ft_pop(cmd_t->arg_v, idx, cmd_t->arg_c--);
	if (!cmd_t->arg_v)
		return (-1);
	if (input_idx >= 0)
		cmd_t->input_v[input_idx] = NULL;
	if (is_bin)
	{
		if (cmd_t->arg_c > 0)
		{
			cmd_t->binary = cmd_t->arg_v[0];
			if (rem_from_both(cmd_t, cmd_t->arg_v[0]) == -1)
				return (-1);
		}
		else
			cmd_t->binary = NULL;
	}
	if (update_inputv_optionsv_after_redir(cmd_t) == -1)
		return (-1);
	return (0);
}

/* Synchronize arguments between each array of the structure */
int	sync_arg(t_cmd_lst *cmd_t, char *old, char *new, int after_parsing)
{
	int	idx;
	int	is_bin;
	int	input_idx;
	int	next;

	idx = 0;
	while (idx < cmd_t->arg_c)
	{
		if (!ft_strncmp(cmd_t->arg_v[idx], old, -1))
		{
			if (after_parsing == 1)
			{
				next = get_next_id(cmd_t, cmd_t->arg_c, cmd_t->arg_v, old);
				if (next != idx)
				{
					idx++;
					continue ;
				}
			}
			is_bin = (cmd_t->arg_v[idx] == cmd_t->binary);
			input_idx = get_input_idx(cmd_t, old);
			free(cmd_t->arg_v[idx]);
			cmd_t->arg_v[idx] = new;
			if (is_bin)
				cmd_t->binary = cmd_t->arg_v[idx];
			else if (input_idx >= 0)
				cmd_t->input_v[input_idx] = cmd_t->arg_v[idx];
			if (ft_strlen(new) == 0)
			{
				if (sync_arg_empty(cmd_t, idx, input_idx, is_bin) == -1)
					return (-1);
				idx--;
			}
			break ;
		}
		idx++;
	}
	return (0);
}

/* Put options into the options_v array */
int	parse_options(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;
	int	is_opt;

	cmd_t->options_v = malloc(sizeof(char *) * (cmd_t->options_c + 1));
	if (!cmd_t->options_v)
		return (-1);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		is_opt = cmd_t->arg_v[i][0] == '-';
		if (i > 1)
			is_opt = (is_opt && ft_strncmp(cmd_t->binary, "echo", 6) != 0);
		if (is_opt)
			cmd_t->options_v[idx++] = cmd_t->arg_v[i];
		i++;
	}
	cmd_t->options_v[idx] = NULL;
	return (0);
}

/* Put inputs into the input_v array */
int	parse_input(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->input_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));
	if (!cmd_t->input_v)
		return (-1);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] != '-' || \
		(ft_strncmp(cmd_t->binary, "echo", 6) == 0 && i > 1))
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (0);
}

/* Calculate size of each array of the structure before allocating them */
void	parse_counts(t_cmd_lst *cmd_t)
{
	int	idx;
	int	is_opt;

	idx = 1;
	cmd_t->input_c = 0;
	cmd_t->options_c = 0;
	while (idx < cmd_t->arg_c)
	{
		is_opt = cmd_t->arg_v[idx][0] == '-';
		if (idx > 1)
			is_opt = (is_opt && ft_strncmp(cmd_t->binary, "echo", 6) != 0);
		if (is_opt)
			cmd_t->options_c++;
		else
			cmd_t->input_c++;
		idx++;
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:47:50 by maxime            #+#    #+#             */
/*   Updated: 2022/08/02 01:26:50 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize cmd_t */
void	initialize_structure(t_cmd_lst *cmd_t)
{
	cmd_t->original = NULL;
	cmd_t->binary = NULL;
	cmd_t->options_c = 0;
	cmd_t->options_v = NULL;
	cmd_t->input_c = 0;
	cmd_t->input_v = NULL;
	cmd_t->arg_c = 0;
	cmd_t->arg_v = NULL;
	cmd_t->parsing_v = NULL;
	cmd_t->input_path = NULL;
	cmd_t->input_fd = -1;
	cmd_t->input_type = 0;
	cmd_t->output_path = NULL;
	cmd_t->output_fd = -1;
	cmd_t->output_type = 0;
	cmd_t->delimiter = NULL;
	cmd_t->next = NULL;
}

/* Get the input_v index of a string value */
int	get_input_idx(t_cmd_lst *cmd_t, char *str)
{
	int	idx;

	idx = 0;
	while (idx < cmd_t->input_c)
	{
		if (str == cmd_t->input_v[idx])
			return (idx);
		idx++;
	}
	return (-1);
}

/* Remove a value from the options_v */
static void	remove_from_options(t_cmd_lst *cmd_t, char *addr, int *cnt)
{
	int	i;

	i = 0;
	while (i < cmd_t->options_c)
	{
		if (cmd_t->options_v[i] == addr)
		{
			cmd_t->options_v[i] = NULL;
			(*cnt)--;
		}
		i++;
	}
}

/* Remove a value from the input_v */
static void	remove_from_inputs(t_cmd_lst *cmd_t, char *addr, int *cnt)
{
	int	i;

	i = 0;
	while (i < cmd_t->input_c)
	{
		if (cmd_t->input_v[i] == addr)
		{
			cmd_t->input_v[i] = NULL;
			(*cnt)--;
		}
		i++;
	}
}

/* Remove a value from input_v and options_v */
int	rem_from_both(t_cmd_lst *cmd_t, char *addr)
{
	int		options_new_c;
	int		input_new_c;

	options_new_c = cmd_t->options_c;
	input_new_c = cmd_t->input_c;
	remove_from_options(cmd_t, addr, &options_new_c);
	remove_from_inputs(cmd_t, addr, &input_new_c);
	if (update_inputv_optionsv_after_redir(cmd_t) == -1)
		return (-1);
	cmd_t->options_c = options_new_c;
	cmd_t->input_c = input_new_c;
	return (0);
}

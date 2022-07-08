/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/08 21:30:10 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_command(t_cmd_lst *cmd_t)
{
	int	idx;

	if (cmd_t->options_v)
		free(cmd_t->options_v);
	if (cmd_t->input_v)
		free(cmd_t->input_v);
	if (cmd_t->arg_v)
	{
		idx = 0;
		while (idx < cmd_t->arg_c)
			free(cmd_t->arg_v[idx++]);
		free(cmd_t->arg_v);
	}
	free(cmd_t);
	return (0);
}

static void	initialize_structure(t_cmd_lst *cmd_t)
{
	cmd_t->original = NULL;
	cmd_t->binary = NULL;
	cmd_t->options_c = 0;
	cmd_t->options_v = NULL;
	cmd_t->input_c = 0;
	cmd_t->input_v = NULL;
	cmd_t->arg_c = 0;
	cmd_t->arg_v = NULL;
}

static int	parse_options(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->options_v = malloc(sizeof(char *) * (cmd_t->options_c + 1));
	if (!cmd_t->options_v)
		return (0);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] == '-')
			cmd_t->options_v[idx++] = cmd_t->arg_v[i];	
		i++;
	}
	cmd_t->options_v[idx] = NULL;
	return (1);
}

static int	parse_input(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->input_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));
	if (!cmd_t->input_v)
		return (0);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] != '-')
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];	
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (1);
}

static void	parse_counts(t_cmd_lst *cmd_t)
{
	int	idx;

	idx = 1;
	cmd_t->input_c = 0;
	cmd_t->options_c = 0;
	while (idx < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[idx][0] == '-')
			cmd_t->options_c++;
		else
			cmd_t->input_c++;
		idx++;
	}
}

t_cmd_lst	*initialize_command(char *line)
{
	t_cmd_lst	*cmd_t;
	char		**split;
	int			idx;

	if (!line)
		return (NULL);
	cmd_t = malloc(sizeof(t_cmd_lst));
	if (!cmd_t)
		return (NULL);
	initialize_structure(cmd_t);

	split = split_command(line);
	if (!split)
		return (NULL);

	idx = 0;
	while (split[idx])
		idx++;
	
	cmd_t->original = line;
	cmd_t->arg_c = idx;
	cmd_t->arg_v = split;
	cmd_t->binary = cmd_t->arg_v[0];
	parse_counts(cmd_t);
	if (cmd_t->options_c > 0)
		parse_options(cmd_t);
	if (cmd_t->input_c > 0)
		parse_input(cmd_t);
	return (cmd_t);
}

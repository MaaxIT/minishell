/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/24 15:49:38 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	free_command(t_cmd_lst *cmd_t)
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
	if (cmd_t->parsing_v)
	{
		idx = 0;
		while (idx < cmd_t->input_c)
			free(cmd_t->parsing_v[idx++]);
		free(cmd_t->parsing_v);
	}
	free(cmd_t);
	return (0);
}

int	free_command_lst(t_cmd_lst *cmd_t)
{
	t_cmd_lst	*tmp;

	while (cmd_t)
	{
		tmp = cmd_t->next;
		free_command(cmd_t);
		cmd_t = tmp;
	}
	return (0);
}

static int	parse_order(t_cmd_lst *cmd_t, t_list *env, int i)
{
	parse_counts(cmd_t);
	if (cmd_t->options_c > 0)
		parse_options(cmd_t);
	if (cmd_t->input_c > 0)
	{
		if (parse_input(cmd_t) == -1)
			return (-1);
	}
	if (parse_quotes(cmd_t, env) == -1)
		return (-1);
	if (remove_quotes_from_bin(cmd_t, i) == -1)
		return (-1);
	if (parse_redirections(cmd_t) == -1)
		return (-1);
	return (0);
}

/*
returns:
	-1 = NULL
	0 = do nothing
*/
static int	loop_new_command(t_cmd_lst *cmd_t, t_list *env, int i, char **pipes)
{
	char	**split;
	int		idx;

	initialize_structure(cmd_t);
	split = split_cmd_lst(pipes[i]);
	if (!split)
		return (-1); // NOT ENOUGH, NEED TO FREE PIPESPLT
	idx = 0;
	while (split[idx])
		idx++;
	cmd_t->original = pipes[i];
	cmd_t->arg_c = idx;
	cmd_t->arg_v = split;
	cmd_t->binary = cmd_t->arg_v[0];
	return (parse_order(cmd_t, env, i));
}

t_cmd_lst	*initialize_command(char *line, t_list *env)
{
	t_cmd_lst	*cmd_t;
	t_cmd_lst	*head;
	t_cmd_lst	*head_bckp;
	char		**pipe_split;
	int			i;

	pipe_split = ft_split_out_quotes(line, '|');
	if (!pipe_split)
		return (NULL);
	i = 0;
	while (pipe_split[i])
	{
		cmd_t = malloc(sizeof(t_cmd_lst));
		if (!cmd_t)
			return (NULL); // NOT ENOUGH, FREE PIPESPLT FST
		if (loop_new_command(cmd_t, env, i, pipe_split) == -1)
			return (NULL);
		init_next_command(&cmd_t, &head_bckp, &head, i);
		i++;
	}
	ft_free_2d_table(pipe_split);
	return (head_bckp);
}

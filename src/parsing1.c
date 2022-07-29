/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/29 16:12:25 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	parse_order(t_cmd_lst *cmd_t, t_list *env)
{
	parse_counts(cmd_t);
	if (cmd_t->options_c > 0)
		parse_options(cmd_t);
	if (cmd_t->input_c > 0)
	{
		if (parse_input(cmd_t) == -1)
			return (-1);
	}
	print_structure(cmd_t);
	if (parse_quotes(cmd_t, env) == -1)
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
static t_cmd_lst	*loop_new_command(t_list *env, int i, char **pipes)
{
	char	**split;
	int		idx;
	t_cmd_lst	*cmd_t;

	cmd_t = malloc(sizeof(t_cmd_lst));
	if (!cmd_t)
		return (NULL); // NOT ENOUGH, FREE PIPESPLT FST
	initialize_structure(cmd_t);
	split = split_cmd_lst(pipes[i]);
	if (!split)
		return (NULL); // NOT ENOUGH, NEED TO FREE PIPESPLT
	idx = 0;
	while (split[idx])
		idx++;
	cmd_t->original = pipes[i];
	cmd_t->arg_c = idx;
	cmd_t->arg_v = split;
	cmd_t->binary = cmd_t->arg_v[0];
	if (parse_order(cmd_t, env) == -1)
		return (NULL);
	else
		return (cmd_t);
}

t_cmd_lst	*initialize_command(char *line, t_list *env)
{
	t_cmd_lst	*cmd_t;
	t_cmd_lst	*head_bckp;
	char		**pipe_split;
	int			i;

	// pipe_split = ft_split_out_quotes(line, '|');
	// if (!pipe_split)
		// return (NULL);
	
	pipe_split = malloc(sizeof(char *) * 2);
	pipe_split[0] = ft_strdup(line);
	pipe_split[1] = NULL;

	i = 0;
	while (pipe_split[i])
	{
		cmd_t = loop_new_command(env, i, pipe_split);
		if (!cmd_t)
			return (NULL); // NOT ENOUG, FREE PIPE SPLT FIRST
		if (!i)
			head_bckp = cmd_t;
		else
			ft_cmd_lstadd_back(&head_bckp, cmd_t);
		i++;
	}
	ft_free_2d_table(pipe_split);
	return (head_bckp);
}

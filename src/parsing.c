/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/15 23:06:35 by mpeharpr         ###   ########.fr       */
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
	cmd_t->parsing_v = NULL;
	cmd_t->input_path = NULL;
	cmd_t->output_path = NULL;
	cmd_t->output_type = 0;
	cmd_t->next = NULL;
}

t_cmd_lst	*initialize_command(char *line, t_list *env)
{
	t_cmd_lst	*cmd_t;
	t_cmd_lst	*head;
	t_cmd_lst	*head_bckp;
	char		**pipe_split;
	char		**split;
	int			i;
	int			idx;
	size_t		len;

	if (!line)
		return (NULL);
	pipe_split = ft_split_out_quotes(line, '|');
	if (!pipe_split)
		return (NULL);
	i = 0;
	while (pipe_split[i])
	{
		cmd_t = malloc(sizeof(t_cmd_lst));
		if (!cmd_t)
			return (NULL); // NOT ENOUGH, FREE PIPESPLT FST
		initialize_structure(cmd_t);

		split = split_cmd_lst(pipe_split[i]);
		if (!split)
			return (NULL); // NOT ENOUGH, NEED TO FREE PIPESPLT

		idx = 0;
		while (split[idx])
			idx++;
		
		cmd_t->original = pipe_split[i];
		cmd_t->arg_c = idx;
		cmd_t->arg_v = split;
		cmd_t->binary = cmd_t->arg_v[0];
		parse_counts(cmd_t);
		if (cmd_t->options_c > 0)
			parse_options(cmd_t);
		if (cmd_t->input_c > 0)
		{
			if (parse_input(cmd_t) == -1)
				return (NULL);
		}
		if (parse_quotes(cmd_t, env) == -1)
			return (NULL);
		print_structure(cmd_t);

		// Remove potential quotes from the binary
		len = 0;
		while (len < ft_strlen(cmd_t->binary))
		{
			idx = replace_sub_in_str(cmd_t, &cmd_t->arg_v[i], "\"", "");
			if (idx == -1)
				return (NULL);
			else if (idx == 0)
			{
				len = 0;
				continue ;
			}
			idx = replace_sub_in_str(cmd_t, &cmd_t->arg_v[i], "'", "");
			if (idx == -1)
				return (NULL);
			else if (idx == 0)
			{
				len = 0;
				continue ;
			}
			len++;
		}
			
		if (parse_redirections(cmd_t) == -1)
			return (NULL);

		if (i == 0)
		{
			head_bckp = cmd_t;
			head = cmd_t;
		}
		else
		{
			head->next = cmd_t;
			head = head->next;
		}
		i++;
	}
	return (head_bckp);
}

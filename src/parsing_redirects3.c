/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirects3.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 00:46:13 by maxime            #+#    #+#             */
/*   Updated: 2022/08/04 01:24:09 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_concat2(t_cmd_lst *cmd_t, char **path_type, int *i, int *idx)
{
	int	len;

	len = *idx;
	while (cmd_t->arg_v[*i][len] && cmd_t->arg_v[*i][len] != '>' && \
	cmd_t->arg_v[*i][len] != '<')
		len++;
	*path_type = ft_strndup(cmd_t->arg_v[*i] + *idx, len - *idx);
	if (!*path_type)
		return (-1);
	if (gen_path_concat(cmd_t, path_type) == -1)
		return (input_not_existing(-1, cmd_t->input_path));
	if (concat_callback(cmd_t, idx, len, i) == -1)
		return (-1);
	*idx = 0;
	return (0);
}

void	loop_parsing_2(t_cmd_lst *cmd_t, int *i, int *idx, char c)
{
	if (cmd_t->arg_v[*i][*idx + 1] == c)
	{
		if (c == '<')
			cmd_t->input_type = 'D';
		else
			cmd_t->output_type = 'A';
		(*idx)++;
	}
	else if (c == '>')
		cmd_t->output_type = 'R';
	else if (c == '<')
		cmd_t->input_type = 'C';
}

int	loop_parsing_3(t_cmd_lst *cmd_t, int *i, int *idx, char **path_type)
{
	int	good;

	good = 1;
	if (cmd_t->arg_v[*i][*idx + 1])
	{
		if (is_concat(cmd_t, path_type, idx, i) == -1)
			return (-1);
	}
	else if (cmd_t->arg_v[*i + 1])
	{
		if (is_separated(cmd_t, path_type, idx, i) == -1)
			return (-1);
	}
	else
		good = 0;
	if (good == 1)
	{
		if (*i < 0)
			*i = 0;
		return (1);
	}
	return (0);
}

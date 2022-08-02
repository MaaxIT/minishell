/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:57:25 by maxime            #+#    #+#             */
/*   Updated: 2022/08/02 23:58:21 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse redirection that are next to the redirection character 
returns:
	- 0 = continue
	- -1 = memory error
*/
static int	is_concat(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int	len;
	int	res;

	if ((path_type == &cmd_t->output_path && cmd_t->output_type == 'A') || \
		(path_type == &cmd_t->input_path && cmd_t->input_type == 'D'))
		(*idx)--;
	res = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx);
	if (res == -1)
		return (-1);
	else if (res == 1)
		(*i)--;
	if ((path_type == &cmd_t->output_path && cmd_t->output_type == 'A') || \
		(path_type == &cmd_t->input_path && cmd_t->input_type == 'D'))
	{
		res = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx);
		if (res == -1)
			return (-1);
		else if (res == 1)
			(*i)--;
	}
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

static int	is_separated(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int	input_idx;

	if (rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) \
	== 1)
		(*i)--;
	if ((cmd_t->output_type == 'A' || cmd_t->input_type == 'D') && \
	rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) == 1)
		(*i)--;
	input_idx = 0;
	while (cmd_t->arg_v[*i + 1][input_idx] && \
	cmd_t->arg_v[*i + 1][input_idx] != '>' && \
	cmd_t->arg_v[*i + 1][input_idx] != '<')
		(input_idx)++;
	*path_type = ft_strndup(cmd_t->arg_v[*i + 1], input_idx);
	if (!*path_type)
		return (-1);
	if (gen_path_separated(cmd_t, path_type) == -1)
		return (input_not_existing(-1, cmd_t->input_path));
	if (separated_callback(cmd_t, path_type, *i, idx) == -1)
		return (-1);
	return (0);
}

/* Callback for every looped character 
returns:
	- -1 = memory error
	- 0 = do nothing
	- 1 = continue
*/
static int	loop(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int		good;
	char	c;

	c = '>';
	if (path_type == &cmd_t->input_path)
		c = '<';
	if (cmd_t->parsing_v[*i][*idx] == 'S' || cmd_t->parsing_v[*i][*idx] == 'D')
	{
		(*idx)++;
		return (1);
	}
	if (cmd_t->arg_v[*i][*idx] == c)
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
	}
	return (0);
}

static int	first_loop(t_cmd_lst *cmd_t, char **path_type)
{
	int	i;
	int	idx;
	int	rtrn;

	i = 0;
	while (i < cmd_t->arg_c)
	{
		idx = 0;
		while (cmd_t->arg_v && cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
		{
			rtrn = loop(cmd_t, path_type, &idx, &i);
			if (rtrn == -1)
				return (-1);
			else if (rtrn == 1)
				continue ;
			idx++;
		}
		i++;
	}
	return (0);
}

/* Parse redirections */
int	parse_redirections(t_cmd_lst *cmd_t)
{
	first_loop(cmd_t, &cmd_t->input_path);
	first_loop(cmd_t, &cmd_t->output_path);
	return (0);
}

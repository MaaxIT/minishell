/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:58:54 by maxime            #+#    #+#             */
/*   Updated: 2022/08/04 01:00:23 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Delete quotes marked for deletion from our strings */
static int	parse_quotes_delete(t_cmd_lst *cmd_t, int *i, int *idx)
{
	int	rtrn;

	while (*i < cmd_t->arg_c)
	{
		*idx = 0;
		while (cmd_t->arg_v[*i][*idx])
		{
			if (cmd_t->parsing_v[*i][*idx] == 'E')
			{
				rtrn = parse_quotes_delete2(cmd_t, idx, i);
				if (rtrn == -1)
					return (-1);
				if (rtrn == 1)
					break ;
			}
			else
				(*idx)++;
		}
		(*i)++;
	}
	*i = 0;
	return (0);
}

static void	calculate_quote(t_cmd_lst *cmd_t, size_t *len, int *idx, int i)
{
	*len = 0;
	(*idx)++;
	if (cmd_t->arg_v[i][*idx] != '?')
	{
		while (cmd_t->parsing_v[i][*idx] && \
		(cmd_t->parsing_v[i][*idx] == 'D' || \
		cmd_t->parsing_v[i][*idx] == 'M') && \
		cmd_t->arg_v[i][*idx] != '$' && cmd_t->arg_v[i][*idx] != '\'' && \
		cmd_t->arg_v[i][*idx] != '\"' && \
		is_envchar(cmd_t->arg_v[i][*idx], (*len == 0)))
		{
			(*len)++;
			(*idx)++;
		}
	}
	else
	{
		*len = 1;
		(*idx)++;
	}
}

static int	parse_quotes_loop(t_cmd_lst *cmd_t, t_list *env, int *idx, int i)
{
	char	*values[3];
	size_t	len;
	t_list	*val;

	calculate_quote(cmd_t, &len, idx, i);
	values[2] = ft_substr(cmd_t->arg_v[i], *idx - len - 1, len + 1);
	values[0] = ft_substr(cmd_t->parsing_v[i], *idx - len - 1, len + 1);
	if (!values[2] || !values[0])
		return (free_both_and_return(values[2], values[0], -1));
	val = get_env_by_id(env, values[2] + 1);
	if (val)
	{
		values[1] = val->value;
		if (valid_envvar(cmd_t, &values[2], values, i) == -1)
			return (-1);
		*idx += (ft_strlen(val->value) - (len + 2));
	}
	else
	{
		if (invalid_envvar(cmd_t, &values[2], values[0], i) == -1)
			return (-1);
		*idx -= (len + 2);
	}
	free(values[0]);
	return (0);
}

/* Parse quote for every argument */
int	parse_quotes(t_cmd_lst *cmd_t, t_list *env)
{
	int		i;
	int		idx;

	if (parse_quotes_init(cmd_t, &i) == -1)
		return (-1);
	if (parse_quotes_delete(cmd_t, &i, &idx) == -1)
		return (-1);
	while (i < cmd_t->arg_c)
	{
		idx = 0;
		while (cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
		{
			if (cmd_t->arg_v[i][idx] == '$' && \
			(is_envchar(cmd_t->arg_v[i][idx + 1], 1) || \
			cmd_t->arg_v[i][idx + 1] == '?') && cmd_t->parsing_v[i][idx] == 'M')
			{
				if (parse_quotes_loop(cmd_t, env, &idx, i) == -1)
					return (-1);
			}
			idx++;
		}
		i++;
	}
	return (0);
}

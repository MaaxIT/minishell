/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:58:54 by maxime            #+#    #+#             */
/*   Updated: 2022/08/02 01:26:16 by maxime           ###   ########.fr       */
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
				rtrn = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx);
				if (rtrn == -1)
					return (-1);
				if (rtrn == 1)
				{
					(*i)--;
					break ;
				}
				if (rem_char(NULL, &cmd_t->parsing_v[*i], *idx) == -1)
					return (-1);
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
	char	*subparse;
	size_t	len;
	t_list	*val;
	char	*sub;

	calculate_quote(cmd_t, &len, idx, i);
	sub = ft_substr(cmd_t->arg_v[i], *idx - len - 1, len + 1);
	if (!sub)
		return (-1);
	subparse = ft_substr(cmd_t->parsing_v[i], *idx - len - 1, len + 1);
	if (!subparse)
		return (-1);
	val = get_env_by_id(env, sub + 1);
	if (val)
	{
		if (valid_envvar(cmd_t, &sub, subparse, val->value, i) == -1)
			return (-1);
		*idx += (ft_strlen(val->value) - (len + 2));
	}
	else
	{
		if (invalid_envvar(cmd_t, &sub, subparse, i) == -1)
			return (-1);
		*idx -= (len + 2);
	}
	free(subparse);
	return (0);
}

int    print_structure(t_cmd_lst *cmd_t)
{
    int    idx;

    printf("\n-===- Debugging structure -===-\n");
    printf("- original: |%s|\n", cmd_t->original);
    printf("- binary: |%s|\n", cmd_t->binary);
    printf("- options_c: %d\n", cmd_t->options_c);
    printf("- options_v:\n");
    idx = 0;
    while (idx < cmd_t->options_c)
    {
        printf("        %d:  |%s|\n", idx, cmd_t->options_v[idx]);
        idx++;
    }
    printf("- input_c: %d\n", cmd_t->input_c);
    printf("- input_v:\n");
    idx = 0;
    while (idx < cmd_t->input_c)
    {
        printf("        %d:  |%s|\n", idx, cmd_t->input_v[idx]);
        idx++;
    }
    printf("- arg_c: %d\n", cmd_t->arg_c);
    printf("- arg_v:\n");
    idx = 0;
    while (idx < cmd_t->arg_c)
    {
        printf("        %d:  |%s|\n", idx, cmd_t->arg_v[idx]);
        idx++;
    }
    if (cmd_t->parsing_v)
    {
        printf("- parsing_v:\n");
        idx = 0;
        while (idx < cmd_t->arg_c)
        {
            printf("        %d:  |%s|\n", idx, cmd_t->parsing_v[idx]);
            idx++;
        }
    }
    printf("- output_type: %c\n", cmd_t->output_type);
    printf("- output_path: %s\n", cmd_t->output_path);
    printf("- input_path: %s\n", cmd_t->input_path);
    printf("- next: %p\n", cmd_t->next);
    printf("-===- End of debugging structure -===-\n\n");
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:08:26 by maxime            #+#    #+#             */
/*   Updated: 2022/08/03 13:44:02 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Initialize the quotes parsing */
int	parse_quotes_init(t_cmd_lst *cmd_t, int *i)
{
	cmd_t->parsing_v = malloc(sizeof(char *) * (cmd_t->arg_c + 1));
	if (!cmd_t->parsing_v)
		return (-1);
	*i = 0;
	while (*i < cmd_t->arg_c)
	{
		cmd_t->parsing_v[*i] = ft_strdup_char('M', ft_strlen(cmd_t->arg_v[*i]));
		if (!cmd_t->parsing_v[*i])
			return (ft_free_2d_table(cmd_t->parsing_v));
		(*i)++;
	}
	cmd_t->parsing_v[*i] = NULL;
	*i = 0;
	while (*i < cmd_t->arg_c)
	{
		parse_input_quotes(cmd_t->arg_v[*i], cmd_t->parsing_v[*i]);
		(*i)++;
	}
	*i = 0;
	return (0);
}

/* Called by parse_quotes when a env value has been found */
int	valid_envvar(t_cmd_lst *cmd_t, char **sub, char *subparse_and_val[2], int i)
{
	int		backup;
	char	*parse;
	char	*val;

	parse = subparse_and_val[0];
	val = subparse_and_val[1];
	backup = cmd_t->arg_c;
	if (replace_sub(cmd_t, &cmd_t->arg_v[i], *sub, val) < 0)
		return (-1);
	free(*sub);
	*sub = NULL;
	if (backup == cmd_t->arg_c)
	{
		*sub = ft_strdup_char('M', ft_strlen(val));
		if (!*sub)
			return (-1);
		if (replace_sub_parse(cmd_t, &cmd_t->parsing_v[i], parse, *sub) != 0)
			return (free_and_return(*sub));
		free(*sub);
		*sub = NULL;
	}
	return (0);
}

/* Called by parse_quotes when the env value has not been found */
int	invalid_envvar(t_cmd_lst *cmd_t, char **sub, char *parse, int i)
{
	int	backup;

	backup = cmd_t->arg_c;
	if (replace_sub(cmd_t, &cmd_t->arg_v[i], *sub, "") < 0)
		return (-1);
	free(*sub);
	*sub = NULL;
	if (backup == cmd_t->arg_c)
	{
		if (replace_sub_parse(cmd_t, &cmd_t->parsing_v[i], parse, "") != 0)
			return (-1);
	}
	return (0);
}

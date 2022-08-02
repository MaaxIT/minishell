/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quotes3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 14:00:08 by maxime            #+#    #+#             */
/*   Updated: 2022/08/03 00:01:00 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse double quotes, called from parse_input_quotes */
static void	doubles_quotes(char *input, char *parse, size_t *idx, size_t *i)
{
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\"' && parse[*idx] != 'S')
		(*idx)++;
	if (*idx != ft_strlen(input))
		parse[*i] = 'E';
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\"')
	{
		if (input[*idx] != '$')
			parse[*idx] = 'D';
		(*idx)++;
	}
	if (*idx < ft_strlen(input))
		parse[*idx] = 'E';
	*i = *idx;
}

/* Parse single quotes, called from parse_input_quotes */
static void	single_quotes(char *input, char *parse, size_t *idx, size_t *i)
{
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\'' && parse[*idx] != 'S')
		(*idx)++;
	if (*idx != ft_strlen(input))
		parse[*i] = 'E';
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\'')
		parse[(*idx)++] = 'S';
	if (*idx < ft_strlen(input))
		parse[*idx] = 'E';
	*i = *idx;
}

/*
Parse quotes of a line
(custom algorithm made by mpeharpr, have fun trying to understand it)
	
parsing_v[x] does contains:
	D = Escaped by double quotes (no meaning except on env vars)
	S = Escaped by simple quotes (no meaning)
	M = Meaning
	E = This quote has been managed, remove it from the string
By default, all chars have meaning (= M)
*/
int	parse_input_quotes(char *input, char *parse)
{
	size_t	i;
	size_t	idx;

	i = 0;
	while (input[i])
	{
		if (!input[i + 1] || \
		contains_mean_char(input + i + 1, parse + i + 1, input[i]))
		{
			if (input[i] == '\"' && parse[i] != 'S')
				doubles_quotes(input, parse, &idx, &i);
			else if (input[i] == '\'' && parse[i] != 'D')
				single_quotes(input, parse, &idx, &i);
		}
		i++;
		if (i > ft_strlen(input))
			i = ft_strlen(input);
	}
	return (0);
}

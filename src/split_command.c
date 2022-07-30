/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:11:22 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/30 13:21:54 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_last_split(char **arritem, char *cmd, int i, int last)
{
	if (i > last)
	{
		*arritem = ft_substr(cmd, last, i - last);
		if (!*arritem)
			return (-1);
	}
	return (0);
}

static void	init_split(char *cmd, int *idx, int *capt, int *i)
{
	*idx = 0;
	*capt = 0;
	*i = 0;
	while (cmd[*i] && cmd[*i] == ' ')
		(*i)++;
}

/* Split the str with spaces + simple & double quotes */
static int	split_with_quotes(char *cmd, char **arr, char *parsing)
{
	int	i;
	int	idx;
	int	capt;
	int	last;

	init_split(cmd, &idx, &capt, &i);
	last = i;
	while (cmd[i])
	{
		if (cmd[i] && parsing[i] != 'E' && (cmd[i] == '\'' || cmd[i] == '\"') \
		&& cmd[i + 1])
			capt = !capt;
		else if (cmd[i] && cmd[i] == ' ' && !capt)
		{
			arr[idx] = ft_substr(cmd, last, i - last);
			if (!arr[idx])
				return (-1);
			idx++;
			while (cmd[i + 1] && cmd[i + 1] == ' ')
				i++;
			last = i + 1;
		}
		i++;
	}
	return (alloc_last_split(&arr[idx], cmd, i, last));
}

static char	**split_cmd_lst_fill(char *cmd, char **splitv, \
		char *parsing, int splitc)
{
	int	idx;

	idx = 0;
	while (idx <= splitc)
		splitv[idx++] = NULL;
	if (split_with_quotes(cmd, splitv, parsing) == -1)
	{
		free(parsing);
		return (free_split(splitv));
	}
	return (splitv);
}

/* Splitting the command line and return a 2D array */
char	**split_cmd_lst(char *cmd)
{
	int		splitc;
	char	**splitv;
	char	*parsing;

	if (!cmd)
		return (NULL);
	parsing = ft_strdup_char('M', ft_strlen(cmd));
	if (!parsing)
		return (NULL);
	if (parse_input_quotes(cmd, parsing) == -1)
		return ((char **)free_and_return_split(parsing));
	splitc = count_splits(cmd, parsing);
	if (splitc <= 0)
		return ((char **)free_and_return_split(parsing));
	splitv = malloc(sizeof(char *) * (splitc + 1));
	if (!splitv)
		return ((char **)free_and_return_split(parsing));
	if (!split_cmd_lst_fill(cmd, splitv, parsing, splitc))
		return (NULL);
	free(parsing);
	return (splitv);
}

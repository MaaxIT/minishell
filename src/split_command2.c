/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:17:50 by maxime            #+#    #+#             */
/*   Updated: 2022/07/29 20:27:16 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Free the split and return NULL in case of an error */
char	**free_split(char **arr)
{
	int	idx;

	idx = 0;
	while (arr[idx])
		free(arr[idx++]);
	free(arr);
	return (NULL);
}

static void	count_splits_loop(char *cmd, char *parsing, size_t i[2], int *capt)
{
	while (cmd[i[0]] && cmd[i[0]] == ' ' && !*capt)
		i[0]++;
	if (cmd[i[0]] && parsing[i[0]] != 'E' && \
			(cmd[i[0]] == '\'' || cmd[i[0]] == '\"') && cmd[i[0] + 1])
	{
		if (!*capt)
			i[1]++;
		*capt = !*capt;
	}
	else if (cmd[i[0]] && cmd[i[0]] != ' ' && !*capt)
		i[1]++;
	i[0]++;
	if (i[0] < ft_strlen(cmd))
	{
		while (cmd[i[0]] && cmd[i[0]] != ' ' && !capt)
			i[0]++;
	}
	else
		i[0] = ft_strlen(cmd);
}

/* Count the amount of splits we will have in order to pre-allocate */
int	count_splits(char *cmd, char *parsing)
{
	size_t	i[2];
	int		idx;
	int		capt;

	capt = 0;
	i[0] = 0;
	i[1] = 0;
	while (cmd[i[0]])
	{
		count_splits_loop(cmd, parsing, i, &capt);
	}
	return (idx);
}

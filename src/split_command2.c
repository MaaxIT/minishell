/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 16:17:50 by maxime            #+#    #+#             */
/*   Updated: 2022/07/27 20:09:40 by maxime           ###   ########.fr       */
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

/* Count the amount of splits we will have in order to pre-allocate */
int	count_splits(char *cmd)
{
	int	i;
	int	idx;
	int	capt;

	idx = 0;
	capt = 0;
	i = 0;
	while (cmd[i])
	{
		while (cmd[i] && cmd[i] == ' ' && !capt)
			i++;
		if (cmd[i] && (cmd[i] == '\'' || cmd[i] == '\"') && cmd[i + 1])
		{
			if (!capt)
				idx++;
			capt = !capt;
		}
		else if (cmd[i] && cmd[i] != ' ' && !capt)
			idx++;
		printf("%s (%c) (%d)\n", cmd, cmd[i + 1], i + 1);
		while (cmd[i + 1] && cmd[i + 1] != ' ' && !capt)
			i++;
		i++;
	}
	return (idx);
}

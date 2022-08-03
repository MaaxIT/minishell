/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils2.C                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 00:37:40 by maxime            #+#    #+#             */
/*   Updated: 2022/08/03 16:37:35 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_id(t_cmd_lst *cmd_t, int size, char **arr, char *input)
{
	int		values[3];

	values[0] = 0;
	while (values[0] < size)
	{
		values[2] = 0;
		if (arr == cmd_t->parsing_v)
			values[2] = (ft_strncmp(arr[values[0]], input, -1) == 0);
		else
			values[2] = (ft_strnstr(arr[values[0]], input, -1) != NULL);
		if (values[2])
		{
			values[1] = 0;
			while (cmd_t->parsing_v[values[0]][values[1]])
			{
				if (cmd_t->parsing_v[values[0]][values[1]] == 'S')
					break ;
				values[1]++;
			}
			if ((size_t)values[1] == ft_strlen(cmd_t->parsing_v[values[0]]))
				return (values[0]);
		}
		values[0]++;
	}
	return (-1);
}

int	free_both_and_return(char *str1, char *str2, int rtrn)
{
	if (str1)
		free(str1);
	if (str2)
		free(str2);
	return (rtrn);
}

int	free_and_return(char *str, int rtrn)
{
	if (str)
		free(str);
	return (rtrn);
}

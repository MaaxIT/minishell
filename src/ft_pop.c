/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pop.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:06:54 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/27 19:54:29 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fill(char **tab, int idx, int tabsize, char **ret)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < tabsize && i < tabsize - 1)
	{
		if (j == idx)
			j++;
		ret[i] = tab[j];
		i++;
		j++;
	}
}

char	**ft_pop(char **tab, int idx, int tabsize)
{
	char	**ret;

	if (idx >= tabsize || !tab)
	{
		ft_free_2d_table(tab);
		return (NULL);
	}
	ret = malloc(sizeof(char *) * tabsize);
	if (!ret)
	{
		ft_free_2d_table(tab);
		return (NULL);
	}
	fill(tab, idx, tabsize, ret);
	ret[tabsize - 1] = NULL;
	if (tab[idx])
		free(tab[idx]);
	free(tab);
	return (ret);
}

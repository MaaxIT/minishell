/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_arrays.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:13:16 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/27 14:54:38 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**ft_pop_null(char **tab, int *tabsize)
{
	int	i;

	i = 0;
	while (i < *tabsize)
	{
		if (tab[i] == NULL)
		{
			tab = ft_pop(tab, i, *tabsize);
			(*tabsize)--;
			if (!tab)
			{
				ft_free_2d_table(tab);
				return (NULL);
			}
			i = -1;
		}
		i++;
	}
	return (tab);
}

int	update_inputv_optionsv_after_redir(t_cmd_lst *cmd_t)
{
	int	parsing_c;
	
	parsing_c = cmd_t->input_c;
	if (cmd_t->input_v)
	{
		cmd_t->input_v = ft_pop_null(cmd_t->input_v, &cmd_t->input_c);
		if (!cmd_t->input_v)
			return (-1);
	}
	if (cmd_t->options_v)
	{
		cmd_t->options_v = ft_pop_null(cmd_t->options_v, &cmd_t->options_c);
		if (!cmd_t->options_v)
			return (-1);
	}
	if (cmd_t->parsing_v)
	{
		cmd_t->parsing_v = ft_pop_null(cmd_t->parsing_v, &parsing_c);
		if (!cmd_t->parsing_v)
			return (-1);
	}
	return (0);
}

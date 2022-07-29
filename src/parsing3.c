/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:47:50 by maxime            #+#    #+#             */
/*   Updated: 2022/07/29 17:14:48 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
returns:
	-1 = NULL
	0 = do nothing
*/
int	remove_quotes_from_bin(t_cmd_lst *cmd_t, int i)
{
	size_t	len;
	int		idx;

	len = 0;
	while (len < ft_strlen(cmd_t->binary)) // why was this part commented?
	{
		idx = replace_sub(cmd_t, &cmd_t->arg_v[i], "\"", "");
		if (idx == -1)
			return (-1);
		else if (idx == 0)
		{
			len = 0;
			continue ;
		}
		idx = replace_sub(cmd_t, &cmd_t->arg_v[i], "'", "");
		if (idx == -1)
			return (-1);
		else if (idx == 0)
		{
			len = 0;
			continue ;
		}
		len++;
	}
	return (0);
}

void	initialize_structure(t_cmd_lst *cmd_t)
{
	cmd_t->original = NULL;
	cmd_t->binary = NULL;
	cmd_t->options_c = 0;
	cmd_t->options_v = NULL;
	cmd_t->input_c = 0;
	cmd_t->input_v = NULL;
	cmd_t->arg_c = 0;
	cmd_t->arg_v = NULL;
	cmd_t->parsing_v = NULL;
	cmd_t->input_path = NULL;
	cmd_t->output_path = NULL;
	cmd_t->output_type = 0;
	cmd_t->next = NULL;
}

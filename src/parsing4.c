/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/03 01:38:10 by maxime            #+#    #+#             */
/*   Updated: 2022/08/04 00:45:19 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sync_arg2(t_cmd_lst *cmd_t, int idx, int *is_bin, char *new)
{
	*is_bin = (cmd_t->arg_v[idx] == cmd_t->binary);
	free(cmd_t->arg_v[idx]);
	cmd_t->arg_v[idx] = new;
}

void	sync_arg3(t_cmd_lst *cmd_t, int idx, int is_bin, int input_idx)
{
	if (is_bin)
		cmd_t->binary = cmd_t->arg_v[idx];
	else if (input_idx >= 0)
		cmd_t->input_v[input_idx] = cmd_t->arg_v[idx];
}

int	sync_arg4(t_cmd_lst *cmd_t, int *idx, int is_bin, int input_idx)
{
	if (sync_arg_empty(cmd_t, *idx, input_idx, is_bin) == -1)
		return (1);
	(*idx)--;
	return (0);
}

int	sync_arg_diff(t_cmd_lst *cmd_t, char *old, int *idx)
{
	if (get_next_id(cmd_t, cmd_t->arg_c, cmd_t->arg_v, old) != *idx)
	{
		(*idx)++;
		return (1);
	}
	return (0);
}

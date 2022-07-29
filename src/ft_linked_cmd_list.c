/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_linked_cmd_list.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 17:07:43 by maxime            #+#    #+#             */
/*   Updated: 2022/07/29 17:07:44 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_cmd_lstadd_back(t_cmd_lst **top, t_cmd_lst *new)
{
	t_cmd_lst	*i;

	if (*top)
	{
		i = *top;
		while (i->next)
			i = i->next;
		i->next = new;
	}
	else
		*top = new;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pipe_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl </var/mail/mbennafl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 00:37:22 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/05 00:37:23 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	save_std_before_pipe(int save_std[2])
{
	save_std[0] = dup(STDIN_FILENO);
	if (save_std[0] == -1)
		return (0);
	save_std[1] = dup(STDOUT_FILENO);
	if (save_std[1] == -1)
	{
		close(save_std[0]);
		return (0);
	}
	return (9);
}

int	clear_std_after_pipe(int save_std[2])
{
	if (dup2(save_std[0], STDIN_FILENO) == -1)
	{
		close(save_std[0]);
		close(save_std[1]);
		return (0);
	}
	if (dup2(save_std[0], STDOUT_FILENO) == -1)
	{
		close(save_std[0]);
		close(save_std[1]);
		return (0);
	}
	close(save_std[0]);
	close(save_std[1]);
	return (9);
}

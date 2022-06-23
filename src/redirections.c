/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:40:36 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 17:18:20 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rd_output(char *path)
{
	int	fd;

	if (!path)
		fd = 1;
	else
	{
		if (!access(path, W_OK) || access(path, F_OK) == -1)
			fd = open(path, O_WRONLY | O_CREAT);
		else
			fd = -1;
	}
	return (fd);
}

int	rd_output_append(char *path)
{
	int	fd;

	if (!path)
		fd = 1;
	else
	{
		if (!access(path, W_OK) || access(path, F_OK) == -1)
			fd = open(path, O_WRONLY | O_APPEND | O_CREAT);
		else
			fd = -1;
	}
	return (fd);
}

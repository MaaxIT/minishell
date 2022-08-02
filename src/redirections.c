/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:40:36 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/29 17:11:43 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	rd_input(char *path)
{
	int	fd;

	if (!path)
		fd = dup(STDIN_FILENO);
	else
		fd = open(path, O_RDONLY, 0644);
	return (fd);
}

int	rd_output(char *path)
{
	int	fd;

	if (access(path, F_OK) == 0 && unlink(path) == -1)
		return (-1);
	if (!path)
		fd = dup(STDOUT_FILENO);
	else
		fd = open(path, O_WRONLY | O_CREAT, 0644);
	return (fd);
}

int	rd_output_append(char *path)
{
	int	fd;

	if (!path)
		fd = dup(STDOUT_FILENO);
	else
			fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
	return (fd);
}

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

int	rd_input(char *path)
{
	int	fd;

	if (!path)
		fd = dup(STDIN_FILENO); // PROTECTIONS IN THE WHOLE FILE
	else
		fd = open(path, O_RDONLY, 0644);
	return (fd);
}

int	rd_output(char *path)
{
	int	fd;

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
/*	{
		if (!access(path, W_OK) || access(path, F_OK) == -1)
*/			fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0644);
/*		else
			fd = -1;
	}
*/
	return (fd);
}

void	rd_delimiter(char *delimiter)
{
	char	*line;
	int	cmp;

	line = readline("> "); // PROTECT AGAINST READLINE ERRORS?
	cmp = ft_strncmp(delimiter, line, ft_strlen(delimiter));
	free(line);
	if (cmp)
		rd_delimiter(delimiter);
}

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

void	rd_delimiter_child(int fd, char *delimiter)
{
	char	*line;

	line = readline("> ");
	while (ft_strncmp(delimiter, line, -1))
	{
		ft_putstr_fd(fd, line);
		write(fd, "\n", 1);
		if (line)
			free(line);
		line = readline("> ");
	}
	if (line)
		free(line);
	exit(0);
}

int	rd_delimiter(char *delimiter)
{
	int	fd;

	fd = open("./delim", O_CREAT | O_WRONLY, 0644); 
	if (fd == -1)
		return (-1);
	g_pid = fork();
	if (g_pid == -1)
		return (-1);
	if (!g_pid)
		rd_delimiter_child(fd, delimiter);
	waitpid(g_pid, NULL, 0); // PROTECT + SHOULD GET THE VALUE FOR ERRNO?
	return (fd);
}

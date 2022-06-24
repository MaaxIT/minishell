/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:04:24 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 17:28:23 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_echo(int fd, int newline, char *str)
{
	int	err;

	err = ft_putstr_fd(fd, str);
	if (err == -1)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	if (newline)
		err = write(fd, "\n", 1);
	if (err == -1)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	return (9);
}

int	bi_cd(int fd, char *str)
{
	int	err;

	(void)fd;

	err = chdir(str);
	if (err == -1)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	return (9);
}

int	bi_pwd(int fd)
{
	char	*cwd;
	int		err;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	err = ft_putstr_fd(fd, cwd);
	if (err == -1)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	if (write(fd, "\n", 1) == -1)
		return (0);		//IS THAT ENOUGH? PRINTING THE ERR IN TREAT CMD FUNCTION
	return (9);
}

int	bi_exit(int fd, t_list **env)
{
	(void)fd;

	ft_lstclear(env);
	rl_clear_history();
	exit(0);
}

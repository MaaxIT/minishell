/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:57:32 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 18:08:25 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	treat_and_call_cmd(char *cmd)
{
	int	fd;

	(void)cmd;
	(void)fd;

// TESTS
/*
	fd = rd_output_append("./");
	if (fd == -1)
		return (0);
	if (!bi_echo(fd, 1, "It\'s an echo"))
		return (0);
	if (close(fd) == -1)
		return (0);
*/	return (9);
}

static int	new_cmd(void)
{
	char	*cmd;

	if (ft_putstr_fd(1, "supershell:>") == -1)
		return (0);//TREAT ERROR WITH ERRNO? OR JUST STOP?
	cmd = readline(NULL);//PROTECT AGAINST READLINE ERRORS?
	if (!treat_and_call_cmd(cmd))
		print_error(0);		//IS THAT ENOUGH?
	new_cmd();
	return (9);
}

int	main(void)
{
	if (!new_cmd())
		return (0);//TREAT ERROR WITH ERRNO
	return (0);
}

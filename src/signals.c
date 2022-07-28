/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:47:05 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 21:12:16 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int signo)
{
	(void)signo;
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (!g_pid)
		rl_redisplay();
	else
		errno = 128 + signo;
}

static void	sigquit_handler(int signo)
{
	(void)signo;
	if (g_pid)
	{
		kill(g_pid, signo); // PROTECT
		ft_putstr_fd(STDERR_FILENO, "Quit: 3\n");
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	if (g_pid)
		errno = 128 + signo;
}

void	signals_init(void)
{
	signal(SIGINT, sigint_handler); // ERROR IF == SIG_ERR
	signal(SIGQUIT, sigquit_handler); // ERROR IF == SIG_ERR
}

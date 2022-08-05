/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:47:05 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/05 12:19:22 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler(int signo)
{
	if (!g_pid)
		return ;
	if (g_pid > 0)
		kill(g_pid, SIGTERM);
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_pid == -2)
		rl_redisplay();
	else
		errno = 128 + signo;
}

static void	sigquit_handler(int signo)
{
	if (!g_pid || errno == -8)
		return ;
	else if (g_pid == -2)
	{
		rl_on_new_line();
		return ;
	}
	if (g_pid > 0)
	{
		kill(g_pid, SIGTERM);
		ft_putstr_fd(STDERR_FILENO, "Quit: 3\n");
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	if (g_pid > 0)
		errno = 128 + signo;
}

int	signals_init(void)
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		return (0);
	if (signal(SIGQUIT, sigquit_handler) == SIG_ERR)
		return (0);
	return (9);
}

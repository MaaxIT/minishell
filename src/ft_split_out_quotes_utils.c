/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_out_quotes_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl </var/mail/mbennafl>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/04 12:15:15 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/05 12:57:43 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	closing_idx(const char *str, int i, char quote)
{
	while (str[i] && str[i] != quote)
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

void	print_err_unclosed_pipe(int *nbr_words)
{
	ft_putstr_fd(STDERR_FILENO, \
	"Supershell: syntax error near unexpected token `|'\n");
	*nbr_words = -1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:17:30 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 15:13:01 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 *	Printing str in fd, returning the number of characters printed or -1 if
 *		an error occurs.
 */

int	ft_putstr_fd(int fd, char *str)
{
	int	i;
	int	err;

	i = 0;
	while (str[i])
	{
		err = write(fd, &str[i], 1);
		if (err == -1)
			return (-1);//TREAT ERROR WITH ERRNO
		i++;
	}
	return (i);
}

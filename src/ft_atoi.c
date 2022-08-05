/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/05 12:35:09 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/05 12:35:39 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long int	calc(long int val, const char *str, size_t i, int sign)
{
	while (str[i] > 47 && str[i] < 58)
	{
		val += str[i] - 48;
		if (str[i + 1] > 47 && str[i + 1] < 58)
			val *= 10;
		i++;
	}
	if (sign * val > 2147483647)
		return (0);
	else if (sign * val < -2147483648)
		return (-1);
	return (val);
}

int	ft_atoi(const char *str)
{
	long int	val;
	size_t		i;
	int			sign;

	i = 0;
	val = 0;
	sign = 1;
	while (str[i] == 32 || (str[i] > 8 && str[i] < 14))
		i++;
	if (str[i] == 45)
	{
		sign = -1;
		i++;
	}
	else if (str[i] == 43)
		i++;
	val = calc(val, str, i, sign);
	return (sign * (int)val);
}

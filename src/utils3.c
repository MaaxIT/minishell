/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:05:05 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:09:41 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_check_fit(char *str, char *set)
{
	size_t	i;
	size_t	set_len;

	i = 0;
	set_len = ft_strlen(set);
	while (str[i] && set[i] && i < set_len)
	{
		if (str[i] == set[i])
		{
			if (i + 1 == set_len)
				return (1);
		}
		else
			return (0);
		i++;
	}
	return (0);
}

char	*ft_strnstr(char *str, char *set, size_t len)
{
	size_t	i;
	size_t	set_len;

	if (!str)
		return (NULL);
	i = 0;
	set_len = ft_strlen(set);
	if (set_len == 0 || !set || !set[0])
		return (str);
	while (str[i] && (i + set_len) <= len)
	{
		if (str[i] == set[0] && ft_check_fit(&str[i], set))
			return (str + i);
		i++;
	}
	return (NULL);
}

int	ft_strlensep(const char *str, char sep)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	return (i);
}

void	ft_bzero(void *str, size_t size)
{
	char	*str_cpy;
	size_t	i;

	i = 0;
	str_cpy = str;
	while (i < size)
		str_cpy[i++] = '\0';
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 01:10:42 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/02 01:30:25 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strdup(const char *str)
{
	size_t	i;
	size_t	str_len;
	char	*ptr;

	str_len = ft_strlen(str);
	ptr = malloc((str_len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < str_len)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	size_t	str_len;
	char	*ret;

	str_len = ft_strlen(str);
	if (n >= str_len)
		ret = ft_strdup(str);
	else
	{
		ret = malloc(sizeof(char) * (n + 1));
		if (!ret)
			return (NULL);
		i = 0;
		while (str[i] && i < n)
		{
			ret[i] = str[i];
			i++;
		}
		ret[i] = '\0';
	}
	return (ret);
}

int	ft_is_a_whitespace_or_empty_string(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (-1);
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i])
		return (0);
	return (9);
}

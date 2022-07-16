/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:17:30 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/11 03:57:08 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Printing str in fd, returning the number of characters printed
** or -1 if an error occurs. */
int	ft_putstr_fd(int fd, const char *str)
{
	int	i;
	int	err;

	i = 0;
	while (str[i])
	{
		err = write(fd, &str[i], 1);
		if (err == -1)
			return (-1);
		i++;
	}
	return (i);
}

/* Return the length of the string */
size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str && str[len])
		len++;
	return (len);
}

/* Copy src into dst up to n bytes */
size_t	ft_strlcpy(char *dst, const char *src, size_t n)
{
	size_t	i;
	size_t	src_len;

	src_len = ft_strlen((char *)src);
	if (!dst || !src || n == 0)
		return (src_len);
	i = 0;
	while (src[i] && i < (n - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (src_len);
}

/* Return the string with an offset of start and a length of len */
char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	unsigned int	total;
	char			*sub;

	if (!s)
		return (NULL);
	total = 0;
	if (start > ft_strlen((char *)s))
	{
		sub = malloc(sizeof(char));
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	s += start;
	while (s[total] && len--)
		total++;
	sub = malloc((total + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	ft_strlcpy(sub, s, total + 1);
	return (sub);
}

/* Compare 2 strings up to nbr bytes */
int	ft_strncmp(const char *s1, const char *s2, size_t nbr)
{
	size_t	i;

	i = 0;
	while ((s1[i] || s2[i]) && i < nbr)
	{
		if (s1[i] != s2[i])
			return (((unsigned char *)s1)[i] - ((unsigned char *)s2)[i]);
		i++;
	}
	return (0);
}

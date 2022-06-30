/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maaxit <maaxit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/28 02:50:08 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/06/30 05:24:12 by maaxit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Check if the string contains a character */
int ft_strincludes(const char *str, char c)
{
    int i;
    
    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (1);
        i++;
    }
    return (0);
}

/* Concatenate 2 strings */
size_t	ft_strlcat(char *dst, char const *src, size_t nbr)
{
	size_t	dst_len;
	size_t	src_len;

	dst_len = ft_strlen(dst);
	src_len = ft_strlen((char *)src);
	if (nbr >= dst_len)
	{
		dst += dst_len;
		nbr -= dst_len;
		ft_strlcpy(dst, src, nbr);
		return (dst_len + src_len);
	}
	else
		return (nbr + src_len);
	return (0);
}

/* Convert a splitted 2d array into a single string separated by a character */
char    *ft_arrjoin(char **split, size_t len, char sep)
{
    size_t  i;
    size_t  strlen;
    char    *str;

    i = 0;
    strlen = 0;
    while (i < len)
        strlen += ft_strlen(split[i++]) + 1;
    
    str = malloc(strlen * sizeof(char));
    if (!str)
        return (NULL);

    i = 0;
    while (i < strlen)
        str[i++] = '\0';
        
    i = 0;
    while (i < len)
    {
        ft_strlcat(str, split[i], -1);
        if (i < len - 1)
            str[ft_strlen(str)] = sep;
        i++;
    }
    return (str);
}

/* Set \0 up to size */
void	ft_bzero(void *str, size_t size)
{
	char	*str_cpy;
	size_t	i;

	i = 0;
	str_cpy = str;
	while (i < size)
		str_cpy[i++] = '\0';
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 02:48:21 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/11 06:01:41 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Replace a substring in str and reallocate it with the good size */
int str_replace_sub(char *origin, char *new, size_t start, size_t end)
{
    char    *backup;

    backup = ft_strdup(origin);
    if (!backup)
        return (-1);
    free(origin);
    origin = malloc(sizeof(char) * (start + ft_strlen(new)));
    if (!origin)
    {
        origin = backup;
        return (-1);
    }
    ft_bzero(origin, (start + ft_strlen(new)));
    ft_strlcpy(origin, backup, start);
    ft_strlcpy(origin + ft_strlen(origin), new, -1);
    ft_strlcpy(origin + ft_strlen(origin), backup + end, -1);
    free(backup);
    return (0);
}

/* Return a new allocated string of str without the character at index idx */
char    *new_str_without_char(char *str, int idx, int freestr)
{
    char    *new;
    int     i;
    int     ins;

    new = malloc(sizeof(char) * ft_strlen(str));
    if (!new)
        return (NULL);
    i = 0;
    ins = 0;
    while (str[i])
    {
        if (i != idx)
            new[ins++] = str[i];
        i++;
    }
    new[ins] = '\0';
    if (freestr)
        free(str);
    return (new);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 02:48:21 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/15 05:34:48 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Replace a substring in str and reallocate it with the good size */
int str_replace_sub(char *origin, char *new, size_t start, size_t end)
{
    char    *backup;
    size_t  max;

    backup = ft_strdup(origin);
    if (!backup)
        return (-1);
    free(origin);
    max = start + ft_strlen(new);
    if (ft_strlen(backup) > max)
        max = ft_strlen(backup);
    origin = malloc(sizeof(char) * (max + 1));
    if (!origin)
    {
        origin = backup;
        return (-1);
    }
    ft_bzero(origin, max);
    ft_strlcpy(origin, backup, start + 1);
    ft_strlcpy(origin + ft_strlen(origin), new, -1);
    ft_strlcpy(origin + ft_strlen(origin), backup + end, -1);
    free(backup);
    return (0);
}

int replace_sub_in_str(t_cmd_lst *cmd_t, char **str, char *old_sub, char *new_sub)
{
    char    *found;
    char    *new;
    size_t  sep_idx;
    size_t  len;

    found = ft_strnstr(*str, old_sub, -1);
    if (!found)
        return (-1);
    sep_idx = ft_strlen(*str) - ft_strlen(found);
    if (sep_idx == 0)
        return (-1);
    len = ft_strlen(*str) + (ft_strlen(new_sub) - ft_strlen(old_sub));
    new = malloc(sizeof(char) * len + 1);
    if (!new)
        return (-1);
    ft_bzero(new, len + 1);
    ft_strlcpy(new, *str, sep_idx + 1);
    ft_strlcat(new, new_sub, -1);
    ft_strlcat(new, *str + sep_idx + ft_strlen(old_sub), -1);
    if (cmd_t)
        sync_arg(cmd_t, *str, new);
    free(*str);
    *str = new;
    return (0);
}

/* Return a new allocated string of str without the character at index idx */
char    *new_str_without_char(char *str, int idx, int freestr)
{
    char    *new;
    int     i;
    int     ins;

    if (!str)
        return (NULL);
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

/* Generate a new string with a repeating character */
char    *ft_strdup_char(char c, size_t repeats)
{
    size_t  j;
    char    *sub;
    
    sub = malloc(sizeof(char) * (repeats + 1));
    if (!sub)
        return (NULL); 
    j = 0;
    while (j < repeats)
        sub[j++] = c;
    sub[j] = '\0';
    return (sub);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 02:48:21 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/01 03:05:59 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	replace_sub_end(t_cmd_lst *cmd_t, char **str, char *new)
{
	int	ret;

	ret = 0;
	if (cmd_t)
	{
		ret = (ft_strlen(new) == 0);
		if (sync_arg(cmd_t, *str, new) == -1)
			return (-1);
	}
	else
	{
		free(*str);
		*str = new;
	}
	return (ret);
}

int	replace_sub(t_cmd_lst *cmd_t, char **str, char *old, char *newsub)
{
	char	*found;
	char	*new;
	size_t	sep_idx;
	size_t	len;

	found = ft_strnstr(*str, old, -1);
	if (!found)
		return (-2);
	sep_idx = ft_strlen(*str) - ft_strlen(found);
	len = ft_strlen(*str) + (ft_strlen(newsub) - ft_strlen(old));
	new = malloc(sizeof(char) * len + 1);
	if (!new)
		return (-1);
	ft_bzero(new, len + 1);
	ft_strlcpy(new, *str, sep_idx + 1);
	ft_strlcat(new, newsub, -1);
	ft_strlcat(new, *str + sep_idx + ft_strlen(old), -1);
	return (replace_sub_end(cmd_t, str, new));
}

/* Remove a character from a string */
int	remove_char_from_str(t_cmd_lst *cmd_t, char **str, int idx)
{
	char	*cpy;
	int		rtrn;

	rtrn = 0;
	cpy = malloc(ft_strlen(*str) * sizeof(char));
	if (!cpy)
		return (-1);
	ft_strlcpy(cpy, *str, idx + 1);
	ft_strlcat(cpy, *str + idx + 1, -1);
	if (cmd_t)
	{	
		rtrn = (ft_strlen(cpy) == 0);
		if (sync_arg(cmd_t, *str, cpy) == -1)
			return (-1);
	}
	else
	{
		free(*str);
		*str = cpy;
	}
	return (rtrn);
}

/* Generate a new string with a repeating character */
char	*ft_strdup_char(char c, size_t repeats)
{
	size_t	j;
	char	*sub;

	sub = malloc(sizeof(char) * (repeats + 1));
	if (!sub)
		return (NULL);
	j = 0;
	while (j < repeats)
		sub[j++] = c;
	sub[j] = '\0';
	return (sub);
}

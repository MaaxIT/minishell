/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 02:48:21 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/04 02:07:53 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	replace_sub_end(t_cmd_lst *cmd_t, char **str, char *new, int do_pop)
{
	int	ret;

	ret = 0;
	if (cmd_t)
	{
		ret = (ft_strlen(new) == 0);
		if (sync_arg(cmd_t, *str, new, 1, do_pop) == -1)
			return (-1);
	}
	else
	{
		free(*str);
		*str = new;
	}
	return (ret);
}

int	replace_sub(t_cmd_lst *cmd_t, char **str, char *old, char *newsub, int do_pop)
{
	char	*found;
	char	*new;
	size_t	sep_idx;
	size_t	len;
	int		idx;

	idx = get_next_id(cmd_t, cmd_t->arg_c, cmd_t->arg_v, old);
	if (idx != -1)
		str = &cmd_t->arg_v[idx];
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
	return (replace_sub_end(cmd_t, str, new, do_pop));
}

int	replace_sub_parse(t_cmd_lst *cmd_t, char **str, char *old, char *newsub, int do_pop)
{
	char	*found;
	char	*new;
	size_t	sep_idx;
	size_t	len;
	int		idx;

	idx = get_next_id(cmd_t, cmd_t->arg_c, cmd_t->parsing_v, old);
	if (idx != -1)
		str = &cmd_t->parsing_v[idx];
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
	return (replace_sub_end(NULL, str, new, do_pop));
}

/* Remove a character from a string */
int	rem_char(t_cmd_lst *cmd_t, char **str, int idx, int do_pop)
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
		if (sync_arg(cmd_t, *str, cpy, 0, do_pop) == -1)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/11 02:48:21 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/01 18:08:41 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_next_id(t_cmd_lst *cmd_t, int size, char **arr, char *input)
{
	int	i;
	int	j;

	i = 0;
	while (i < size)
	{
		if (!ft_strncmp(arr[i], input, -1))
		{
			j = 0;
			while (cmd_t->parsing_v[i][j])
			{
				if (cmd_t->parsing_v[i][j] == 'S')
					break ;
				j++;
			}
			if ((size_t)j == ft_strlen(cmd_t->parsing_v[i]))
				return (i);
		}
		i++;
	}
	return (-1);
}

static int	replace_sub_end(t_cmd_lst *cmd_t, char **str, char *new)
{
	int	ret;

	ret = 0;
	if (cmd_t)
	{
		ret = (ft_strlen(new) == 0);
		if (sync_arg(cmd_t, *str, new, 1) == -1)
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
	int		idx;

	idx = get_next_id(cmd_t, cmd_t->arg_c, cmd_t->arg_v, old);
	printf("- (%d) |%s| |%s| |%s|\n", idx, cmd_t->arg_v[idx], old, newsub);
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
	return (replace_sub_end(cmd_t, str, new));
}

int	replace_sub_parse(t_cmd_lst *cmd_t, char **str, char *old, char *newsub)
{
	char	*found;
	char	*new;
	size_t	sep_idx;
	size_t	len;
	int		idx;

	idx = get_next_id(cmd_t, cmd_t->arg_c, cmd_t->parsing_v, old);
	printf("= (%d) |%s| |%s| |%s|\n", idx, cmd_t->parsing_v[idx], old, newsub);
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
	return (replace_sub_end(NULL, str, new));
}

/* Remove a character from a string */
int	rem_char(t_cmd_lst *cmd_t, char **str, int idx)
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
		if (sync_arg(cmd_t, *str, cpy, 0) == -1)
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

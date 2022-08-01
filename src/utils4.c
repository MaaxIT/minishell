/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 01:10:42 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/27 18:47:35 by maxime           ###   ########.fr       */
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
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i])
		return (0);
	return (9);
}

static void	free_command(t_cmd_lst *cmd_t)
{
	int	idx;

	if (cmd_t->options_v)
		free(cmd_t->options_v);
	if (cmd_t->input_v)
		free(cmd_t->input_v);
	if (cmd_t->arg_v)
	{
		idx = 0;
		while (idx < cmd_t->arg_c)
			free(cmd_t->arg_v[idx++]);
		free(cmd_t->arg_v);
	}
	if (cmd_t->parsing_v)
	{
		idx = 0;
		while (idx < cmd_t->arg_c)
			free(cmd_t->parsing_v[idx++]);
		free(cmd_t->parsing_v);
	}
	if (cmd_t->input_path)
		free(cmd_t->input_path);
	if (cmd_t->output_path)
		free(cmd_t->output_path);
	free(cmd_t);
}

int	free_command_lst(t_cmd_lst *cmd_t)
{
	t_cmd_lst	*tmp;

	while (cmd_t)
	{
		tmp = cmd_t->next;
		free_command(cmd_t);
		cmd_t = tmp;
	}
	return (0);
}

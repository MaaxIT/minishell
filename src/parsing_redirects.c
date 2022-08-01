/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:57:25 by maxime            #+#    #+#             */
/*   Updated: 2022/08/01 21:04:34 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	concat_callback(t_cmd_lst *cmd_t, int *idx, int len, int *i)
{
	int	k;
	int	offset;
	int	rtrn;

	offset = 0;
	if (cmd_t->output_type == 'A')
		offset = -1;
	k = *idx - offset;
	while (k < len)
	{
		rtrn = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx - offset);
		if (rtrn == -1)
			return (-1);
		else if (rtrn == 1)
		{
			(*i)--;
			break ;
		}
		k++;
	}
	return (0);
}

static int	gen_path_concat(t_cmd_lst *cmd_t, char **path_type)
{
	int	fd;

	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		rd_delimiter(cmd_t->input_path);
		free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	else
	{
		if (path_type == &cmd_t->input_path)
			fd = open(*path_type, O_RDONLY, 0644);
		else
			fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
		else if (path_type == &cmd_t->input_path)
			return (-1);
	}
	return (0);
}

static int	separated_callback(t_cmd_lst *cmd_t, char **path_type, int i, int *idx)
{
	int	res;
	
	res = replace_sub(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "");
	if (res < 0)
		return (-1);
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		if (res == 0)
			free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	*idx = 0;
	return (0);
}

static int	gen_path_separated(t_cmd_lst *cmd_t, char **path_type)
{
	int	fd;
	
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
		rd_delimiter(cmd_t->input_path);
	else
	{
		if (path_type == &cmd_t->input_path)
			fd = open(*path_type, O_RDONLY, 0644);
		else
			fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
		else if (path_type == &cmd_t->input_path)
			return (-1);
	}
	return (0);
}

/* Parse redirection that are next to the redirection character 
returns:
	- 0 = continue
	- -1 = memory error
*/
static int	is_concat(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int		len;

	len = *idx + 1;
	while (cmd_t->arg_v[*i][len] && cmd_t->arg_v[*i][len] != '>' && \
	cmd_t->arg_v[*i][len] != '<')
		len++;
	*path_type = ft_strndup(cmd_t->arg_v[*i] + *idx + 1, len - (*idx + 1));
	if (!*path_type)
		return (-1);
	if (gen_path_concat(cmd_t, path_type) == -1)
		return (-1); // karibou
	if (concat_callback(cmd_t, idx, len, i) == -1)
		return (-1);
	*idx = 0;
	return (0);
}

static int	is_separated(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int	input_idx;

	if (rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) \
	== 1)
		(*i)--;
	if (cmd_t->output_type == 'A' && \
	rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) == 1)
		(*i)--;
	input_idx = 0;
	while (cmd_t->arg_v[*i + 1][input_idx] && \
	cmd_t->arg_v[*i + 1][input_idx] != '>' && \
	cmd_t->arg_v[*i + 1][input_idx] != '<')
		(input_idx)++;
	*path_type = ft_strndup(cmd_t->arg_v[*i + 1], input_idx);
	if (!*path_type)
		return (-1);
	if (gen_path_separated(cmd_t, path_type) == -1)
		return (-1); // karibou
	if (separated_callback(cmd_t, path_type, *i, idx) == -1)
		return (-1);
	return (0);
}

/* Callback for every looped character 
returns:
	- -1 = memory error
	- 0 = do nothing
	- 1 = continue
*/
static int	loop(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int		good;
	char	c;

	c = '>';
	if (path_type == &cmd_t->input_path)
		c = '<';
	if (cmd_t->parsing_v[*i][*idx] == 'S' || cmd_t->parsing_v[*i][*idx] == 'D')
	{
		(*idx)++;
		return (1);
	}
	if (cmd_t->arg_v[*i][*idx] == c)
	{
		if (cmd_t->arg_v[*i][*idx + 1] == c)
		{
			cmd_t->output_type = 'A';
			(*idx)++;
		}
		else
			cmd_t->output_type = 'R';
		good = 1;
		if (cmd_t->arg_v[*i][*idx + 1])
		{
			if (is_concat(cmd_t, path_type, idx, i) == -1)
				return (-1);
		}
		else if (cmd_t->arg_v[*i + 1])
		{
			if (is_separated(cmd_t, path_type, idx, i) == -1)
				return (-1);
		}
		else
			good = 0;
		if (good == 1)
		{
			if (*i < 0)
				*i = 0;
			return (1);
		}
	}
	return (0);
}

/* Parse redirections */
int	parse_redirections(t_cmd_lst *cmd_t)
{
	int		i;
	int		j;
	int		idx;
	int		rtrn;
	char	**path_type;

	path_type = &cmd_t->input_path;
	j = 0;
	while (j < 2)
	{
		if (j == 1)
			path_type = &cmd_t->output_path;
		i = 0;
		while (i < cmd_t->arg_c)
		{
			idx = 0;
			while (cmd_t->arg_v && cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
			{
				rtrn = loop(cmd_t, path_type, &idx, &i);
				if (rtrn == -1)
					return (-1);
				else if (rtrn == 1)
					continue ;
				idx++;
			}
			i++;
		}
		j++;
	}
	return (0);
}

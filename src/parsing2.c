/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/08/01 04:58:47 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Get the input_v index of a string value */
int	get_input_idx(t_cmd_lst *cmd_t, char *str)
{
	int	idx;

	idx = 0;
	while (idx < cmd_t->input_c)
	{
		if (str == cmd_t->input_v[idx])
			return (idx);
		idx++;
	}
	return (-1);
}

/* Remove a value from the options_v */
void	remove_from_options(t_cmd_lst *cmd_t, char *addr, int *cnt)
{
	int	i;

	i = 0;
	while (i < cmd_t->options_c)
	{
		if (cmd_t->options_v[i] == addr)
		{
			cmd_t->options_v[i] = NULL;
			(*cnt)--;
		}
		i++;
	}
}

/* Remove a value from the input_v */
void	remove_from_inputs(t_cmd_lst *cmd_t, char *addr, int *cnt)
{
	int	i;

	i = 0;
	while (i < cmd_t->input_c)
	{
		if (cmd_t->input_v[i] == addr)
		{
			cmd_t->input_v[i] = NULL;
			(*cnt)--;
		}
		i++;
	}
}

/* Remove a value from input_v and options_v */
int	rem_from_both(t_cmd_lst *cmd_t, char *addr)
{
	int		options_new_c;
	int		input_new_c;

	options_new_c = cmd_t->options_c;
	input_new_c = cmd_t->input_c;
	remove_from_options(cmd_t, addr, &options_new_c);
	remove_from_inputs(cmd_t, addr, &input_new_c);
	if (update_inputv_optionsv_after_redir(cmd_t) == -1)
		return (-1);
	cmd_t->options_c = options_new_c;
	cmd_t->input_c = input_new_c;
	return (0);
}

/* Remove an empty argument from arrays while synchronizing */
int	sync_arg_empty(t_cmd_lst *cmd_t, int idx, int input_idx, int is_bin)
{
	if (cmd_t->parsing_v && cmd_t->parsing_v[idx])
	{
		cmd_t->parsing_v = ft_pop(cmd_t->parsing_v, idx, cmd_t->arg_c);
		if (!cmd_t->parsing_v)
			return (-1);
	}
	cmd_t->arg_v = ft_pop(cmd_t->arg_v, idx, cmd_t->arg_c--);
	if (!cmd_t->arg_v)
		return (-1);
	if (input_idx >= 0)
		cmd_t->input_v[input_idx] = NULL;
	if (is_bin)
	{
		if (cmd_t->arg_c > 0)
		{
			cmd_t->binary = cmd_t->arg_v[0];
			if (rem_from_both(cmd_t, cmd_t->arg_v[0]) == -1)
				return (-1);
		}
		else
			cmd_t->binary = NULL;
	}
	if (update_inputv_optionsv_after_redir(cmd_t) == -1)
		return (-1);
	return (0);
}

/* Synchronize arguments between each array of the structure */
int	sync_arg(t_cmd_lst *cmd_t, char *old_input, char *new_input)
{
	int	idx;
	int	is_bin;
	int	input_idx;

	idx = 0;
	while (idx < cmd_t->arg_c)
	{
		if (ft_strncmp(cmd_t->arg_v[idx], old_input, -1) == 0)
		{
			is_bin = (cmd_t->arg_v[idx] == cmd_t->binary);
			input_idx = get_input_idx(cmd_t, old_input);
			free(cmd_t->arg_v[idx]);
			cmd_t->arg_v[idx] = new_input;
			if (is_bin)
				cmd_t->binary = cmd_t->arg_v[idx];
			else if (input_idx >= 0)
				cmd_t->input_v[input_idx] = cmd_t->arg_v[idx];
			if (ft_strlen(new_input) == 0 && \
			sync_arg_empty(cmd_t, idx, input_idx, is_bin) == -1)
				return (-1);
			break ;
		}
		idx++;
	}
	return (0);
}

/* Parse redirection that are next to the redirection character 
returns:
	- 0 = continue
	- -1 = memory error
*/
int	parse_redirections_concat(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int		k;
	int		fd;
	int		len;

	len = *idx + 1;
	while (cmd_t->arg_v[*i][len] && cmd_t->arg_v[*i][len] != '>' && \
	cmd_t->arg_v[*i][len] != '<')
		len++;
	*path_type = ft_strndup(cmd_t->arg_v[*i] + *idx + 1, len - (*idx + 1));
	if (!*path_type)
		return (-1);
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		rd_delimiter(cmd_t->input_path);
		free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	else
	{
		fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
	}
	if (cmd_t->output_type == 'A')
	{
		k = *idx - 1;
		while (k < len)
		{
			if (remove_char_from_str(cmd_t, &cmd_t->arg_v[*i], *idx - 1) == 1)
			{
				(*i)--;
				break ;
			}
			k++;
		}
	}
	else
	{
		k = *idx;
		while (k < len)
		{
			if (remove_char_from_str(cmd_t, &cmd_t->arg_v[*i], *idx) == 1)
			{
				(*i)--;
				break ;
			}
			k++;
		}
	}
	*idx = 0;
	return (0);
}

int	parse_redirections_separated(t_cmd_lst *cmd_t, char **path_type, int *i)
{
	int	fd;
	int	input_idx;

	if (remove_char_from_str(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) == 1)
		(*i)--;
	if (cmd_t->output_type == 'A' && remove_char_from_str(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1) == 1)
		(*i)--;
	input_idx = 0;
	while (cmd_t->arg_v[*i + 1][input_idx] && cmd_t->arg_v[*i + 1][input_idx] != '>' && cmd_t->arg_v[*i + 1][input_idx] != '<')
		(input_idx)++;
	*path_type = ft_strndup(cmd_t->arg_v[*i + 1], input_idx);
	if (!*path_type)
		return (-1);
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
		rd_delimiter(cmd_t->input_path);
	else
	{
		fd = open(*path_type, O_CREAT, 0644);
		if (fd >= 0)
			close(fd);
	}
	replace_sub(cmd_t, &cmd_t->arg_v[*i + 1], *path_type, "");
	if (cmd_t->output_type == 'A' && cmd_t->input_path)
	{
		free(*path_type);
		*path_type = NULL;
		cmd_t->input_path = NULL;
	}
	return (0);
}

/* Callback for every looped character 
returns:
	- -1 = memory error
	- 0 = do nothing
	- 1 = continue
*/
int	parse_redirections_loop(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
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
			if (parse_redirections_concat(cmd_t, path_type, idx, i) == -1)
				return (-1);
		}
		else if (cmd_t->arg_v[*i + 1])
		{
			if (parse_redirections_separated(cmd_t, path_type, i) == -1)
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

	path_type = &cmd_t->output_path;
	j = 0;
	while (j < 2)
	{
		if (j == 1)
			path_type = &cmd_t->input_path;
		i = 0;
		while (i < cmd_t->arg_c)
		{
			idx = 0;
			while (cmd_t->arg_v && cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
			{
				rtrn = parse_redirections_loop(cmd_t, path_type, &idx, &i);
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

/* Parse double quotes, called from parse_input_quotes */
void	parse_double_quotes(char *input, char *parse, size_t *idx, size_t *i)
{
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\"' && parse[*idx] != 'S')
		(*idx)++;
	if (*idx != ft_strlen(input))
		parse[*i] = 'E';
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\"')
	{
		if (input[*idx] != '$')
			parse[*idx] = 'D';
		(*idx)++;
	}
	if (*idx < ft_strlen(input))
		parse[*idx] = 'E';
	*i = *idx;
}

/* Parse single quotes, called from parse_input_quotes */
void	parse_single_quotes(char *input, char *parse, size_t *idx, size_t *i)
{
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\'' && parse[*idx] != 'S')
		(*idx)++;
	if (*idx != ft_strlen(input))
		parse[*i] = 'E';
	*idx = *i + 1;
	while (input[*idx] && input[*idx] != '\'')
		parse[(*idx)++] = 'S';
	if (*idx < ft_strlen(input))
		parse[*idx] = 'E';
	*i = *idx;
}

/*
Parse quotes of a line (custom algorithm made by mpeharpr, have fun trying to understand it)
	
parsing_v[x] does contains:
	D = Escaped by double quotes (no meaning except on env vars)
	S = Escaped by simple quotes (no meaning)
	M = Meaning
	E = This quote has been managed, remove it from the string
By default, all chars have meaning (= M)
*/
int	parse_input_quotes(char *input, char *parse)
{
	size_t	i;
	size_t	idx;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' && parse[i] != 'S')
			parse_double_quotes(input, parse, &idx, &i);
		else if (input[i] == '\'' && parse[i] != 'D')
			parse_single_quotes(input, parse, &idx, &i);
		i++;
		if (i > ft_strlen(input))
			i = ft_strlen(input);
	}
	return (0);
}

/* Initialize the quotes parsing */
int	parse_quotes_init(t_cmd_lst *cmd_t, int *i)
{
	cmd_t->parsing_v = malloc(sizeof(char *) * (cmd_t->arg_c + 1));
	if (!cmd_t->parsing_v)
		return (-1);
	*i = 0;
	while (*i < cmd_t->arg_c)
	{
		cmd_t->parsing_v[*i] = ft_strdup_char('M', ft_strlen(cmd_t->arg_v[*i]));
		if (!cmd_t->parsing_v[*i])
			return (-1); // Memory error
		(*i)++;
	}
	cmd_t->parsing_v[*i] = NULL;
	*i = 0;
	while (*i < cmd_t->arg_c)
	{
		parse_input_quotes(cmd_t->arg_v[*i], cmd_t->parsing_v[*i]);
		(*i)++;
	}
	*i = 0;
	return (0);
}

/* Delete quotes marked for deletion from our strings */
int	parse_quotes_delete(t_cmd_lst *cmd_t, int *i, int *idx)
{
	int	rtrn;

	while (*i < cmd_t->arg_c)
	{
		*idx = 0;
		while (cmd_t->arg_v[*i][*idx])
		{
			if (cmd_t->parsing_v[*i][*idx] == 'E')
			{
				rtrn = remove_char_from_str(cmd_t, &cmd_t->arg_v[*i], *idx);
				if (rtrn == -1)
					return (-1);
				if (rtrn == 1)
				{
					(*i)--;
					break ;
				}
				if (remove_char_from_str(NULL, &cmd_t->parsing_v[*i], *idx) == -1)
					return (-1);
			}
			else
				(*idx)++;
		}
		(*i)++;
	}
	*i = 0;
	return (0);
}

int	parse_quotes_found_value(t_cmd_lst *cmd_t, char **sub, char *subparse, char *value, int i)
{
	int	backup;

	backup = cmd_t->arg_c;
	if (replace_sub(cmd_t, &cmd_t->arg_v[i], *sub, value) != 0)
		return (-1);
	free(*sub);
	*sub = NULL;
	if (backup == cmd_t->arg_c)
	{
		*sub = ft_strdup_char('M', ft_strlen(value));
		if (!*sub)
			return (-1);
		if (replace_sub(NULL, &cmd_t->parsing_v[i], subparse, *sub) != 0)
			return (-1);
		free(*sub);
		*sub = NULL;
	}
	return (0);
}

int	parse_quotes_not_found(t_cmd_lst *cmd_t, char **sub, char *subparse, int i)
{
	int	backup;

	backup = cmd_t->arg_c;
	if (replace_sub(cmd_t, &cmd_t->arg_v[i], *sub, "") != 0)
		return (-1);
	free(*sub);
	*sub = NULL;
	if (backup == cmd_t->arg_c)
	{
		if (replace_sub(NULL, &cmd_t->parsing_v[i], subparse, "") != 0)
			return (-1);
	}
	return (0);
}

void	parse_quotes_calculate(t_cmd_lst *cmd_t, size_t *len, int *idx, int i)
{
	*len = 0;
	(*idx)++;
	if (cmd_t->arg_v[i][*idx] != '?')
	{
		while (cmd_t->parsing_v[i][*idx] && \
		(cmd_t->parsing_v[i][*idx] == 'D' || cmd_t->parsing_v[i][*idx] == 'M') && \
		cmd_t->arg_v[i][*idx] != '$' && cmd_t->arg_v[i][*idx] != '\'' && \
		cmd_t->arg_v[i][*idx] != '\"' && is_envchar(cmd_t->arg_v[i][*idx], (*len == 0)))
		{
			(*len)++;
			(*idx)++;
		}
	}
	else
	{
		*len = 1;
		(*idx)++;
	}
}

int	parse_quotes_loop(t_cmd_lst *cmd_t, t_list *env, int *idx, int i)
{
	char	*subparse;
	size_t	len;
	t_list	*val;
	char	*sub;

	parse_quotes_calculate(cmd_t, &len, idx, i);
	sub = ft_substr(cmd_t->arg_v[i], *idx - len - 1, len + 1);
	if (!sub)
		return (-1);
	subparse = ft_substr(cmd_t->parsing_v[i], *idx - len - 1, len + 1);
	if (!subparse)
		return (-1);
	val = get_env_by_id(env, sub + 1);
	if (val)
	{
		if (parse_quotes_found_value(cmd_t, &sub, subparse, val->value, i) == -1)
			return (-1); // Memory error
		*idx += (ft_strlen(val->value) - (len + 2));
	}
	else
	{
		if (parse_quotes_not_found(cmd_t, &sub, subparse, i) == -1)
			return (-1); // Memory error
		*idx -= (len + 2);
	}
	free(subparse);
	return (0);
}

/* Parse quote for every argument */
int	parse_quotes(t_cmd_lst *cmd_t, t_list *env)
{
	int		i;
	int		idx;

	if (parse_quotes_init(cmd_t, &i) == -1)
		return (-1);
	if (parse_quotes_delete(cmd_t, &i, &idx) == -1)
		return (-1);
	while (i < cmd_t->arg_c)
	{
		idx = 0;
		while (cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
		{
			if (cmd_t->arg_v[i][idx] == '$' && (is_envchar(cmd_t->arg_v[i][idx + 1], 1) || \
			cmd_t->arg_v[i][idx + 1] == '?') && cmd_t->parsing_v[i][idx] == 'M')
			{
				if (parse_quotes_loop(cmd_t, env, &idx, i) == -1)
					return (-1);
			}
			idx++;
		}
		i++;
	}
	return (0);
}

/* Put options into the options_v array */
int	parse_options(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;
	int	is_opt;

	cmd_t->options_v = malloc(sizeof(char *) * (cmd_t->options_c + 1));
	if (!cmd_t->options_v)
		return (-1);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		is_opt = cmd_t->arg_v[i][0] == '-';
		if (i > 1)
			is_opt = (is_opt && ft_strncmp(cmd_t->binary, "echo", 6) != 0);
		if (is_opt)
			cmd_t->options_v[idx++] = cmd_t->arg_v[i];
		i++;
	}
	cmd_t->options_v[idx] = NULL;
	return (0);
}

/* Put inputs into the input_v array */
int	parse_input(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->input_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));
	if (!cmd_t->input_v)
		return (-1);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] != '-' || \
		(ft_strncmp(cmd_t->binary, "echo", 6) == 0 && i > 1))
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (0);
}

/* Calculate size of each array of the structure before allocating them */
void	parse_counts(t_cmd_lst *cmd_t)
{
	int	idx;
	int	is_opt;

	idx = 1;
	cmd_t->input_c = 0;
	cmd_t->options_c = 0;
	while (idx < cmd_t->arg_c)
	{
		is_opt = cmd_t->arg_v[idx][0] == '-';
		if (idx > 1)
			is_opt = (is_opt && ft_strncmp(cmd_t->binary, "echo", 6) != 0);
		if (is_opt)
			cmd_t->options_c++;
		else
			cmd_t->input_c++;
		idx++;
	}
}

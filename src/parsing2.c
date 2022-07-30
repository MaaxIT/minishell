/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/31 00:59:35 by maxime           ###   ########.fr       */
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

/* Remove a value from input_v and options_v */
int	remove_from_input_and_options(t_cmd_lst *cmd_t, char *addr)
{
	int	i;
	int	options_new_c;
	int	input_new_c;

	options_new_c = cmd_t->options_c;
	input_new_c = cmd_t->input_c;
	i = 0;
	while (i < cmd_t->options_c)
	{
		if (cmd_t->options_v[i] == addr)
		{
			cmd_t->options_v[i] = NULL;
			options_new_c--;
		}
		i++;
	}
	i = 0;
	while (i < cmd_t->input_c)
	{
		if (cmd_t->input_v[i] == addr)
		{
			cmd_t->input_v[i] = NULL;
			input_new_c--;
		}
		i++;
	}
	if (update_inputv_optionsv_after_redir(cmd_t) == -1)
		return (-1);
	cmd_t->options_c = options_new_c;
	cmd_t->input_c = input_new_c;
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
			if (ft_strlen(new_input) == 0)
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
						if (remove_from_input_and_options(cmd_t, cmd_t->arg_v[0]) == -1)
							return (-1);
					}
					else
						cmd_t->binary = NULL;
				}
				if (update_inputv_optionsv_after_redir(cmd_t) == -1)
					return (-1);
			}
			break ;
		}
		idx++;
	}
	return (0);
}

/* Parse redirections */
int	parse_redirections(t_cmd_lst *cmd_t)
{
	int		i;
	int		j;
	int		k;
	int		fd;
	int		idx;
	int		len;
	int		input_idx;
	char	c;
	char	**path_type;

	path_type = &cmd_t->output_path;
	c = '>';
	j = 0;
	while (j < 2)
	{
		if (j == 1)
		{
			path_type = &cmd_t->input_path;
			c = '<';
		}
		i = 0;
		while (i < cmd_t->arg_c)
		{
			idx = 0;
			while (cmd_t->arg_v && cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
			{
				if (cmd_t->parsing_v[i][idx] == 'S' || cmd_t->parsing_v[i][idx] == 'D')
				{
					idx++;
					continue ;
				}
				if (cmd_t->arg_v[i][idx] == c)
				{
					if (cmd_t->arg_v[i][idx + 1] == c)
					{
						cmd_t->output_type = 'A';
						idx++;
					}
					else
						cmd_t->output_type = 'R';
					if (cmd_t->arg_v[i][idx + 1])
					{
						len = idx + 1;
						while (cmd_t->arg_v[i][len] && cmd_t->arg_v[i][len] != '>' && \
						cmd_t->arg_v[i][len] != '<')
							len++;
						*path_type = ft_strndup(cmd_t->arg_v[i] + idx + 1, len - (idx + 1));
						if (!path_type)
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
							k = idx - 1;
							while (k < len)
							{
								if (remove_char_from_str(cmd_t, &cmd_t->arg_v[i], idx - 1) == 1)
								{
									i--;
									break ;
								}
								k++;
							}
						}
						else
						{
							k = idx;
							while (k < len)
							{
								if (remove_char_from_str(cmd_t, &cmd_t->arg_v[i], idx) == 1)
								{
									i--;
									break ;
								}
								k++;
							}
						}
						idx = 0;
						if (i < 0)
							i = 0;
						continue ;
					}
					else if (cmd_t->arg_v[i + 1])
					{
						// Parse the char part (the previous argument)
						if (remove_char_from_str(cmd_t, &cmd_t->arg_v[i], ft_strlen(cmd_t->arg_v[i]) - 1) == 1)
							i--;
						if (cmd_t->output_type == 'A' && remove_char_from_str(cmd_t, &cmd_t->arg_v[i], ft_strlen(cmd_t->arg_v[i]) - 1) == 1)
							i--;
						// Parse the pathname part
						input_idx = 0;
						while (cmd_t->arg_v[i + 1][input_idx] && cmd_t->arg_v[i + 1][input_idx] != '>' && cmd_t->arg_v[i + 1][input_idx] != '<')
							input_idx++;
						*path_type = ft_strndup(cmd_t->arg_v[i + 1], input_idx);
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
						replace_sub(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "");
						if (cmd_t->output_type == 'A' && cmd_t->input_path)
						{
							free(*path_type);
							*path_type = NULL;
							cmd_t->input_path = NULL;
						}
						if (i < 0)
							i = 0;
						continue ;
					}
				}
				idx++;
			}
			i++;
		}
		j++;
	}
	return (0);
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
		{
			idx = i + 1;
			while (input[idx] && input[idx] != '\"' && parse[idx] != 'S')
				idx++;
			if (idx != ft_strlen(input))
				parse[i] = 'E';
			idx = i + 1;
			while (input[idx] && input[idx] != '\"')
			{
				if (input[idx] != '$')
					parse[idx] = 'D';
				idx++;
			}
			if (idx < ft_strlen(input))
				parse[idx] = 'E';
			i = idx;
		}
		else if (input[i] == '\'' && parse[i] != 'D')
		{
			idx = i + 1;
			while (input[idx] && input[idx] != '\'' && parse[idx] != 'S')
				idx++;
			if (idx != ft_strlen(input))
				parse[i] = 'E';
			idx = i + 1;
			while (input[idx] && input[idx] != '\'')
				parse[idx++] = 'S';
			if (idx < ft_strlen(input))
				parse[idx] = 'E';
			i = idx;
		}
		i++;
		if (i > ft_strlen(input))
			i = ft_strlen(input);
	}
	return (0);
}

/* Parse quote for every argument */
int	parse_quotes(t_cmd_lst *cmd_t, t_list *env)
{
	int		i;
	int		idx;
	int		rtrn;
	size_t	len;
	t_list	*val;
	char	*sub;
	char	*subparse;

	cmd_t->parsing_v = malloc(sizeof(char *) * (cmd_t->arg_c + 1));
	if (!cmd_t->parsing_v)
		return (-1);
	i = 0;
	while (i < cmd_t->arg_c)
	{
		cmd_t->parsing_v[i] = ft_strdup_char('M', ft_strlen(cmd_t->arg_v[i]));
		if (!cmd_t->parsing_v[i])
			return (-1); // Memory error
		i++;
	}
	cmd_t->parsing_v[i] = NULL;
	i = 0;
	while (i < cmd_t->arg_c)
	{
		parse_input_quotes(cmd_t->arg_v[i], cmd_t->parsing_v[i]);
		i++;
	}
	i = 0;
	while (i < cmd_t->arg_c)
	{
		idx = 0;
		while (cmd_t->arg_v[i][idx])
		{
			if (cmd_t->parsing_v[i][idx] == 'E')
			{
				rtrn = remove_char_from_str(cmd_t, &cmd_t->arg_v[i], idx);
				if (rtrn == -1)
					return (-1);
				if (rtrn == 1)
				{
					i--;
					break ;
				}
				rtrn = remove_char_from_str(NULL, &cmd_t->parsing_v[i], idx);
				if (rtrn == -1)
					return (-1);
				if (rtrn == 1)
				{
					i--;
					break ;
				}
			}
			else
				idx++;
		}
		i++;
	}
	sub = NULL;
	i = 0;
	while (i < cmd_t->arg_c)
	{
		idx = 0;
		while (cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
		{
			if (cmd_t->arg_v[i][idx] == '$' && (is_envchar(cmd_t->arg_v[i][idx + 1], 1) ||
			!ft_strncmp(cmd_t->arg_v[i], "$?", -1)) && cmd_t->parsing_v[i][idx] == 'M')
			{
				len = 0;
				idx++;
				while (cmd_t->parsing_v[i][idx] && \
					(cmd_t->parsing_v[i][idx] == 'D' || cmd_t->parsing_v[i][idx] == 'M') && \
					cmd_t->arg_v[i][idx] != '$' && cmd_t->arg_v[i][idx] != '\'' && \
					cmd_t->arg_v[i][idx] != '\"' && (is_envchar(cmd_t->arg_v[i][idx], (len == 0)) || \
					!ft_strncmp(cmd_t->arg_v[i], "$?", -1)))
				{
					len++;
					idx++;
				}
				sub = ft_substr(cmd_t->arg_v[i], idx - len - 1, len + 1);
				if (!sub)
					return (-1);
				subparse = ft_substr(cmd_t->parsing_v[i], idx - len - 1, len + 1);
				if (!subparse)
					return (-1);
				val = get_env_by_id(env, sub + 1);
				if (val)
				{
					if (replace_sub(cmd_t, &cmd_t->arg_v[i], sub, val->value) == -1)
						return (-1); // memory error
					free(sub);
					sub = ft_strdup_char('M', ft_strlen(val->value));
					if (!sub)
						return (-1); // memory error
					if (replace_sub(NULL, &cmd_t->parsing_v[i], subparse, sub) == -1)
						return (-1); // memory error
					free(sub);
					free(subparse);
					idx += (ft_strlen(val->value) - (len + 2));
				}
				else
				{
					if (replace_sub(cmd_t, &cmd_t->arg_v[i], sub, "") == -1)
						return (-1);
					free(sub);
					if (replace_sub(NULL, &cmd_t->parsing_v[i], subparse, "") == -1)
						return (-1); // memory error
					free(subparse);
					idx -= (len + 2);
				}
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

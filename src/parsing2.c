/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/24 15:54:34 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sync_arg(t_cmd_lst *cmd_t, char *old_input, char *new_input)
{
	int	idx;
	int	is_bin;

	idx = 0;
	while (idx < cmd_t->arg_c)
	{
		if (ft_strncmp(cmd_t->arg_v[idx], old_input, -1) == 0)
		{
			is_bin = (cmd_t->arg_v[idx] == cmd_t->binary);
			free(cmd_t->arg_v[idx]);
			cmd_t->arg_v[idx] = new_input;
			if (is_bin)
				cmd_t->binary = new_input;
			break ;
		}
		idx++;
	}
}

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

int	parse_redirections(t_cmd_lst *cmd_t)
{
	int		i;
	int		j;
	int		k;
	int		fd;
	int		idx;
	int 	len;
	int 	input_idx;
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
			while (cmd_t->arg_v[i] && cmd_t->arg_v[i][idx])
			{
				input_idx = get_input_idx(cmd_t, cmd_t->arg_v[i]);
				if (input_idx > -1)
				{	
					if (cmd_t->parsing_v[input_idx][idx] == 'S' || cmd_t->parsing_v[input_idx][idx] == 'D')
					{
						idx++;
						continue ;
					}
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
						// PARSE echo bon>jour
						// idx:          |
						// idx+1:         |

						len = idx + 1;
						while (cmd_t->arg_v[i][len] && cmd_t->arg_v[i][len] != '>' && cmd_t->arg_v[i][len] != '<')
							len++;

						*path_type = ft_strndup(cmd_t->arg_v[i] + idx + 1, len - (idx + 1));
						if (!path_type)
							return (-1);

						fd = rd_output(*path_type);
						close(fd);

						if (cmd_t->output_type == 'A')
						{
							k = idx - 1;
							while (k < len)
							{
								remove_char_from_str(cmd_t, &cmd_t->arg_v[i], idx - 1);
								k++;
							}
						}
						else
						{
							k = idx;
							while (k < len)
							{
								remove_char_from_str(cmd_t, &cmd_t->arg_v[i], idx);
								k++;
							}
						}

						input_idx = get_input_idx(cmd_t, cmd_t->arg_v[i]);
						if (input_idx >= 0)
						{
							cmd_t->input_v[input_idx] = cmd_t->arg_v[i];

							if (ft_strlen(cmd_t->input_v[input_idx]) == 0)
							{
								cmd_t->arg_v = ft_pop(cmd_t->arg_v, i, cmd_t->arg_c--);
								if (!cmd_t->arg_v)
									return (-1);
								cmd_t->input_v[input_idx] = NULL;
								if (update_inputv_optionsv_after_redir(cmd_t) == -1)
									return (-1);
							}
						}

						idx = 0;
						continue;

					}
					else if (cmd_t->arg_v[i + 1])
					{

						// Parse the char part (the previous argument)
						input_idx = get_input_idx(cmd_t, cmd_t->arg_v[i]);
						if (!input_idx)
							return (-1); // this is not possible.
						remove_char_from_str(cmd_t, &cmd_t->arg_v[i], ft_strlen(cmd_t->arg_v[i]) - 1);
						if (cmd_t->output_type == 'A')
							remove_char_from_str(cmd_t, &cmd_t->arg_v[i], ft_strlen(cmd_t->arg_v[i]) - 1);
						cmd_t->input_v[input_idx] = cmd_t->arg_v[i];

						if (ft_strlen(cmd_t->input_v[input_idx]) == 0)
						{
							cmd_t->arg_v = ft_pop(cmd_t->arg_v, i--, cmd_t->arg_c--);
							if (!cmd_t->arg_v)
								return (-1);
							cmd_t->input_v[input_idx] = NULL;
							if (update_inputv_optionsv_after_redir(cmd_t) == -1)
								return (-1);
						}

						// Parse the pathname part
						input_idx = 0;
						while (cmd_t->arg_v[i + 1][input_idx] && cmd_t->arg_v[i + 1][input_idx] != '>' && cmd_t->arg_v[i + 1][input_idx] != '<')
							input_idx++;

						*path_type = ft_strndup(cmd_t->arg_v[i + 1], input_idx);
						if (!*path_type)
							return (-1);

						fd = rd_output(*path_type);
						close(fd);

						input_idx = get_input_idx(cmd_t, cmd_t->arg_v[i + 1]);
						replace_sub_in_str(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "");
						cmd_t->input_v[input_idx] = cmd_t->arg_v[i + 1];
							
						if (ft_strlen(cmd_t->input_v[input_idx]) == 0)
						{
							cmd_t->arg_v = ft_pop(cmd_t->arg_v, i + 1, cmd_t->arg_c--);
							if (!cmd_t->arg_v)
								return (-1);
							cmd_t->input_v[input_idx] = NULL;
							if (update_inputv_optionsv_after_redir(cmd_t) == -1)
								return (-1);
						}

						idx = 0;
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
	By default, all chars have meaning
	D = Escaped by double quotes (no meaning)
	S = Escaped by simple quotes (no meaning)
	M = Meaning
	E = This quote has been managed, remove it from the string
*/
int	parse_input_quotes(char *input, char *parse)
{
	int		i;
	int		idx;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' && parse[i] != 'S')
		{
			parse[i] = 'E';
			idx = i + 1;
			while (input[idx] != '\"')
			{
				if (input[idx] != '$')
					parse[idx] = 'D';
				idx++;
			}
			parse[idx] = 'E';
			i = idx;
		}
		else if (input[i] == '\'' && parse[i] != 'D')
		{
			parse[i] = 'E';
			idx = i + 1;
			while (input[idx] != '\'')
				parse[idx++] = 'S';
			parse[idx] = 'E';
			i = idx;
		}
		i++;
	}
	return (0);
}

int	parse_quotes(t_cmd_lst *cmd_t, t_list *env)
{
	int		i;
	int		idx;
	size_t	len;
	t_list	*val;
	char	*sub;
	char 	*subparsing;

	cmd_t->parsing_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));
	if (!cmd_t->parsing_v)
		return (-1);

	i = 0;
	while (i < cmd_t->input_c)
	{
		cmd_t->parsing_v[i] = malloc(sizeof(char) * (ft_strlen(cmd_t->input_v[i]) + 1));
		len = 0;
		while (len < ft_strlen(cmd_t->input_v[i]))
			cmd_t->parsing_v[i][len++] = 'M';
		cmd_t->parsing_v[i][len] = '\0';
		i++;
	}

	i = 0;
	while (i < cmd_t->input_c)
	{
		parse_input_quotes(cmd_t->input_v[i], cmd_t->parsing_v[i]);
		i++;
	}

	i = 0;
	while (i < cmd_t->input_c)
	{
		idx = 0;
		while (cmd_t->input_v[i][idx])
		{
			if (cmd_t->parsing_v[i][idx] == 'E')
			{
				if (remove_char_from_str(cmd_t, &cmd_t->input_v[i], idx) == -1)
					return (-1);
				if (remove_char_from_str(NULL, &cmd_t->parsing_v[i], idx) == -1)
					return (-1);
			}
			else
				idx++;
		}
		i++;
	}

	sub = NULL;
	i = 0;
	while (i < cmd_t->input_c)
	{
		idx = 0;
		while (cmd_t->input_v[i] && cmd_t->input_v[i][idx])
		{
			if (cmd_t->input_v[i][idx] == '$' && cmd_t->parsing_v[i][idx] == 'M')
			{
				len = 0;
				idx++;
				while (cmd_t->parsing_v[i][idx] && \
					(cmd_t->parsing_v[i][idx] == 'D' || cmd_t->parsing_v[i][idx] == 'M') && \
					cmd_t->input_v[i][idx] != '$' && cmd_t->input_v[i][idx] != '\'' && cmd_t->input_v[i][idx] != '\"')
				{
					len++;
					idx++;
				}
				
				sub = ft_substr(cmd_t->input_v[i], idx - len, len);
				if (!sub)
					return (-1);
				subparsing = ft_substr(cmd_t->parsing_v[i], idx - len, len);
				if (!subparsing)
					return (-1);
				val = get_env_by_id(env, sub);
				if (val)
				{
					if (remove_char_from_str(cmd_t, &cmd_t->input_v[i], idx - len - 1) == -1)
						return (-1); // memory error
					if (replace_sub_in_str(cmd_t, &cmd_t->input_v[i], sub, val->value) == -1)
						return (-1); // memory error
					free(sub);
					sub = ft_strdup_char('M', ft_strlen(val->value));
					if (!sub)
						return (-1); // memory error
					if (replace_sub_in_str(NULL, &cmd_t->parsing_v[i], subparsing, sub) == -1)
						return (-1); // memory error
					free(sub);
					free(subparsing);
					idx += (ft_strlen(val->value) - (len + 2));
				}
				else
				{
					if (replace_sub_in_str(cmd_t, &cmd_t->input_v[i], sub, "") == -1)
						return (-1);
					free(sub);
					if (replace_sub_in_str(NULL, &cmd_t->parsing_v[i], subparsing, "") == -1)
						return (-1); // memory error
					free(subparsing);
					idx -= (len + 2);
				}
			}
			idx++;
		}
		i++;
	}
	return (0);
}

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
		if (cmd_t->arg_v[i][0] != '-' || (ft_strncmp(cmd_t->binary, "echo", 6) == 0 && i > 1))
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (0);
}

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

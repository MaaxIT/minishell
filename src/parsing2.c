/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/28 17:52:39 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int    print_structure(t_cmd_lst *cmd_t)
{
	(void)cmd_t;
	int     idx;

	printf("\n-===- Debugging structure -===-\n");
	printf("- original: |%s|\n", cmd_t->original);
	printf("- binary: |%s|\n", cmd_t->binary);
	printf("- options_c: %d\n", cmd_t->options_c);
	printf("- options_v:\n");
	idx = 0;
	while (idx < cmd_t->options_c)
	{
		printf("        %d:  |%s|\n", idx, cmd_t->options_v[idx]);
		idx++;
	}
	printf("- input_c: %d\n", cmd_t->input_c);
	printf("- input_v:\n");
	idx = 0;
	while (idx < cmd_t->input_c)
	{
		printf("        %d:  |%s|\n", idx, cmd_t->input_v[idx]);
		idx++;
	}
	printf("- arg_c: %d\n", cmd_t->arg_c);
	printf("- arg_v:\n");
	idx = 0;
	while (idx < cmd_t->arg_c)
	{
		printf("        %d:  |%s|\n", idx, cmd_t->arg_v[idx]);
		idx++;
	}
	if (cmd_t->parsing_v)
	{
		printf("- parsing_v:\n");
		idx = 0;
		while (idx < cmd_t->arg_c)
		{
			printf("        %d:  |%s|\n", idx, cmd_t->parsing_v[idx]);
			idx++;
		}
	}
	printf("- output_type: %c\n", cmd_t->output_type);
	printf("- output_path: %s\n", cmd_t->output_path);
	printf("- input_path: %s\n", cmd_t->input_path);
	printf("- next: %p\n", cmd_t->next);
	printf("-===- End of debugging structure -===-\n\n");
	return (0);
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

void	remove_from_input_and_options(t_cmd_lst *cmd_t, char *addr)
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
		return ; // EXIT HERE, MEMORY ERROR
	cmd_t->options_c = options_new_c;
	cmd_t->input_c = input_new_c;
}

void	sync_arg(t_cmd_lst *cmd_t, char *old_input, char *new_input)
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
						return ; // EXIT HERE, MEMORY ERROR
				}
				cmd_t->arg_v = ft_pop(cmd_t->arg_v, idx, cmd_t->arg_c--);
				if (!cmd_t->arg_v)
					return ; // EXIT HERE, MEMORY ERROR
				if (input_idx >= 0)
					cmd_t->input_v[input_idx] = NULL;
				if (is_bin)
				{
					if (cmd_t->arg_c > 0)
					{
						cmd_t->binary = cmd_t->arg_v[0];
						remove_from_input_and_options(cmd_t, cmd_t->arg_v[0]);
					}
					else
						cmd_t->binary = NULL;
				}
				if (update_inputv_optionsv_after_redir(cmd_t) == -1)
					return ; // EXIT HERE, MEMORY ERROR
			}
			break ;
		}
		idx++;
	}
}

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
						replace_sub_in_str(cmd_t, &cmd_t->arg_v[i + 1], *path_type, "");
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
	By default, all chars have meaning (= M)
	D = Escaped by double quotes (no meaning except on env vars)
	S = Escaped by simple quotes (no meaning)
	M = Meaning
	E = This quote has been managed, remove it from the string
*/
int	parse_input_quotes(char *input, char *parse)
{
	size_t	i;
	int		idx;

	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' && parse[i] != 'S')
		{
			parse[i] = 'E';
			idx = i + 1;
			while (input[idx] && input[idx] != '\"')
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
			while (input[idx] && input[idx] != '\'')
				parse[idx++] = 'S';
			parse[idx] = 'E';
			i = idx;
		}
		i++;
		if (i > ft_strlen(input))
			i = ft_strlen(input);
		printf("%zu\n", i);
	}
	return (0);
}

int	parse_quotes(t_cmd_lst *cmd_t, t_list *env)
{
	int		i;
	int		idx;
	int		rtrn;
	size_t	len;
	t_list	*val;
	char	*sub;
	char	*subparsing;

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
				print_structure(cmd_t);
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
			if (cmd_t->arg_v[i][idx] == '$' && is_env_char(cmd_t->arg_v[i][idx + 1], 1) && cmd_t->parsing_v[i][idx] == 'M')
			{
				len = 0;
				idx++;
				while (cmd_t->parsing_v[i][idx] && \
					(cmd_t->parsing_v[i][idx] == 'D' || cmd_t->parsing_v[i][idx] == 'M') && \
					cmd_t->arg_v[i][idx] != '$' && cmd_t->arg_v[i][idx] != '\'' && cmd_t->arg_v[i][idx] != '\"' \
					&& is_env_char(cmd_t->arg_v[i][idx], (len == 0)))
				{
					len++;
					idx++;
				}
				sub = ft_substr(cmd_t->arg_v[i], idx - len - 1, len + 1);
				if (!sub)
					return (-1);
				subparsing = ft_substr(cmd_t->parsing_v[i], idx - len - 1, len + 1);
				if (!subparsing)
					return (-1);
				val = get_env_by_id(env, sub + 1);
				if (val)
				{
					if (replace_sub_in_str(cmd_t, &cmd_t->arg_v[i], sub, val->value) == -1)
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
					if (replace_sub_in_str(cmd_t, &cmd_t->arg_v[i], sub, "") == -1)
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/14 03:56:29 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sync_input_args(t_cmd_lst *cmd_t)
{
	int	idx;
	int	offset;

	offset = 1 + cmd_t->options_c;
	idx = offset;
	while (idx < cmd_t->arg_c && (idx - offset) < cmd_t->input_c)
	{
		if (cmd_t->arg_v[idx] != cmd_t->input_v[(idx - offset)])
		{
			free(cmd_t->arg_v[idx]);
			cmd_t->arg_v[idx] = cmd_t->input_v[(idx - offset)];
		}
		idx++;
	}
}

int	parse_redirections(t_cmd_lst *cmd_t)
{
	int		i;
	int		j;
	int		idx;
	int		done;
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
			while (cmd_t->arg_v[i][idx])
			{
				done = 0;
				if (cmd_t->arg_v[i][idx] == c)
				{
					cmd_t->output_type = 'R';
					if (cmd_t->arg_v[i][idx + 1] == c)
					{
						cmd_t->output_type = 'A';
						idx++;	
					}
					if (cmd_t->arg_v[i][idx + 1])
					{
						*path_type = ft_strdup(&cmd_t->arg_v[i][idx + 1]);
						if (!*path_type)
							return (-1); // memory error
						str_replace_sub(cmd_t->arg_v[i], "", idx + 1, ft_strlen(cmd_t->arg_v[i]));
						done = 1;
					}
					else if (cmd_t->arg_v[i + 1])
					{
						*path_type = ft_strdup(cmd_t->arg_v[i + 1]); // here and above - instead of using strdup - looping until we found another > or \0
						if (!*path_type)
							return (-1); // memory error
						str_replace_sub(cmd_t->arg_v[i], "", 0, ft_strlen(cmd_t->arg_v[i]));
						str_replace_sub(cmd_t->arg_v[i + 1], "", 0, ft_strlen(cmd_t->arg_v[i + 1]));
						done = 1;
					}
					if (done == 0)
						cmd_t->output_type = 0;
				}
				idx++;
			}
			i++;
		}
		j++;
	}

	for (i = 0; i < cmd_t->arg_c; i++)
		printf("%d: %s\n", i, cmd_t->arg_v[i]);
	printf("Path: %s(input) %s(output)\n", cmd_t->input_path, cmd_t->output_path);
	printf("Type: %c\n", cmd_t->output_type);

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

	cmd_t->parsing_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));

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
		// printf("---- INPUT BEFORE %d ----\n-> %s            (VALUE (%zu))\n-> %s            (PARSING (%zu))\n-> 0123456789...    (INDEXES)\n------------------\n\n", i, cmd_t->input_v[i], ft_strlen(cmd_t->input_v[i]), cmd_t->parsing_v[i], ft_strlen(cmd_t->parsing_v[i]));
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
				cmd_t->input_v[i] = new_str_without_char(cmd_t->input_v[i], idx, 1);
				cmd_t->parsing_v[i] = new_str_without_char(cmd_t->parsing_v[i], idx, 1);
			}
			else
				idx++;
		}
		i++;
	}

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
				printf("==> Environment variable detected: |%s|\n", sub);
				val = get_env_by_id(env, sub);
				free(sub);
				if (val)
				{
					if (str_replace_sub(cmd_t->input_v[i], val->value, idx - len, idx) == -1)
						return (-1); // memory error
					sub = ft_strdup_char('D', ft_strlen(val->value));
					if (!sub)
						return (-1); // memory error
					if (str_replace_sub(cmd_t->parsing_v[i], sub, idx - len, idx) == -1)
						return (-1); // memory error
					free(sub);
					idx += (ft_strlen(val->value) - (len + 2));
				}
				else
				{
					if (str_replace_sub(cmd_t->input_v[i], "", idx - len, idx) == -1)
						return (-1); // memory error
					if (str_replace_sub(cmd_t->parsing_v[i], "", idx - len, idx) == -1)
						return (-1); // memory error
					idx -= (len + 2);
				}
				sync_input_args(cmd_t);
			}
			idx++;
		}
		i++;
	}

	// i = 0;
	// while (i < cmd_t->input_c)
	// {
	// 	printf("---- INPUT BEFORE %d ----\n-> %s            (VALUE (%zu))\n-> %s            (PARSING (%zu))\n-> 0123456789...    (INDEXES)\n------------------\n\n", i, cmd_t->input_v[i], ft_strlen(cmd_t->input_v[i]), cmd_t->parsing_v[i], ft_strlen(cmd_t->parsing_v[i]));
	// 	i++;
	// }

	return (0);
}

int	parse_options(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->options_v = malloc(sizeof(char *) * (cmd_t->options_c + 1));
	if (!cmd_t->options_v)
		return (-1);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] == '-')
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
		if (cmd_t->arg_v[i][0] != '-')
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];	
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (0);
}

void	parse_counts(t_cmd_lst *cmd_t)
{
	int	idx;

	idx = 1;
	cmd_t->input_c = 0;
	cmd_t->options_c = 0;
	while (idx < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[idx][0] == '-')
			cmd_t->options_c++;
		else
			cmd_t->input_c++;
		idx++;
	}
}

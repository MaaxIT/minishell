/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/15 03:02:46 by mpeharpr         ###   ########.fr       */
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
	int		k;
	int		idx;
	int 	len;
	char	c;
	char	**path_type;

	printf("arg=%s type=%c output_path=%s\n", cmd_t->arg_v[1], cmd_t->output_type, cmd_t->output_path);

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

				
				if (cmd_t->arg_v[i][idx] == c)
				{
					if (cmd_t->arg_v[i][idx + 1] == c)
					{
						cmd_t->output_type = 'A';
						idx++;
					}
					if (cmd_t->arg_v[i][idx + 1])
					{
						// PARSE echo bon>jour
						// idx:          |
						// idx+1:         |

						len = idx + 1;
						while (cmd_t->arg_v[i][len] && cmd_t->arg_v[i][len] != '>' && cmd_t->arg_v[i][len] != '<')
							len++;

						*path_type = malloc(sizeof(char) * len - idx);
						k = 0;
						while (k < len - (idx + 1))
						{
							(*path_type)[k] = cmd_t->arg_v[i][idx + 1 + k];
							k++;
						}
						(*path_type)[k] = '\0';
						if (cmd_t->output_type == 'A')
							str_replace_sub(cmd_t->arg_v[i], "", idx - 1, len);
						else
						{
							str_replace_sub(cmd_t->arg_v[i], "", idx, len);
							cmd_t->output_type = 'R';
						}
						printf("%s\n", cmd_t->arg_v[1]);
						i = -1;
						break ;

					}
					else if (cmd_t->arg_v[i + 1])
					{

						// PARSE echo bon> jour

					}
				}

				idx++;
			}
			i++;
			idx = 0;
			while (idx < cmd_t->arg_c)
			{
				if (ft_strlen(cmd_t->arg_v[idx]) == 0)
					ft_pop(cmd_t->arg_v, idx, cmd_t->arg_c--);
				else
					idx++;
			}
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
				subparsing = ft_substr(cmd_t->parsing_v[i], idx - len, len);
				if (!subparsing)
					return (-1);
				printf("==> Environment variable detected: |%s|\n", sub);
				val = get_env_by_id(env, sub);
				if (val)
				{
					if (replace_sub_in_str(&cmd_t->input_v[i], sub, val->value) == -1)
						return (-1); // memory error
					free(sub);
					sub = ft_strdup_char('D', ft_strlen(val->value));
					if (!sub)
						return (-1); // memory error
					if (replace_sub_in_str(&cmd_t->parsing_v[i], subparsing, sub) == -1)
						return (-1); // memory error
					free(sub);
					free(subparsing);
					idx += (ft_strlen(val->value) - (len + 2));
				}
				else
				{
					if (replace_sub_in_str(&cmd_t->input_v[i], sub, "") == -1)
						return (-1);
					free(sub);
					if (replace_sub_in_str(&cmd_t->parsing_v[i], subparsing, "") == -1)
						return (-1); // memory error
					free(subparsing);
					idx -= (len + 2);
				}
				sync_input_args(cmd_t);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 13:57:25 by maxime            #+#    #+#             */
/*   Updated: 2022/08/04 02:11:06 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Parse redirection that are next to the redirection character 
returns:
	- 0 = continue
	- -1 = memory error
*/
int	is_concat(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int	res;

	if ((path_type == &cmd_t->output_path && cmd_t->output_type == 'A') || \
		(path_type == &cmd_t->input_path && cmd_t->input_type == 'D'))
		(*idx)--;
	res = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx, 1);
	if (res == -1)
		return (-1);
	else if (res == 1)
		(*i)--;
	if ((path_type == &cmd_t->output_path && cmd_t->output_type == 'A') || \
		(path_type == &cmd_t->input_path && cmd_t->input_type == 'D'))
	{
		res = rem_char(cmd_t, &cmd_t->arg_v[*i], *idx, 1);
		if (res == -1)
			return (-1);
		else if (res == 1)
			(*i)--;
	}
	if (is_concat2(cmd_t, path_type, i, idx) == -1)
		return (-1);
	return (0);
}

int	is_separated(t_cmd_lst *cmd_t, char **path_type, int *idx, int *i)
{
	int	input_idx;

	if (rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1, 1) \
	== 1)
		(*i)--;
	if (((path_type == &cmd_t->output_path && cmd_t->output_type == 'A') \
	|| (path_type == &cmd_t->input_path && cmd_t->input_type == 'D')) && \
	rem_char(cmd_t, &cmd_t->arg_v[*i], ft_strlen(cmd_t->arg_v[*i]) - 1, 1) \
	== 1)
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
		return (input_not_existing(-1, cmd_t->input_path));
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
	int		res;
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
		loop_parsing_2(cmd_t, i, idx, c);
		res = loop_parsing_3(cmd_t, i, idx, path_type);
		if (res != 0)
			return (res);
	}
	return (0);
}

static int	first_loop(t_cmd_lst *cmd_t, char **path_type)
{
	int	i;
	int	idx;
	int	rtrn;

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
	return (0);
}

/* Parse redirections */
int	parse_redirections(t_cmd_lst *cmd_t)
{
	if (first_loop(cmd_t, &cmd_t->input_path) == -1)
		return (-1);
	if (first_loop(cmd_t, &cmd_t->output_path) == -1)
		return (-1);
	return (0);
}

int    print_structure(t_cmd_lst *cmd_t)
{
    int    idx;

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

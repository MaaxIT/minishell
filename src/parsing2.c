/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/08 21:47:17 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/09 01:37:48 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
Know if we need to interpret this arg + remove quotes from it, just keep the arg itself
return values:
	- -1: memory error
	- 0: do not interpret
	- 1: interpret
*/
int manage_arg_quotes(t_cmd_lst *cmd_t, int input_idx)
{
	int		interpret;
	int		catch_end;
	int		catch_start;
	int		i;
	char	*str;
	char	*backup;

	catch_end = -1;
	catch_start = 0;
	interpret = 1;
	while (cmd_t->input_v[input_idx][i])
	{
		if ((cmd_t->input_v[input_idx][i] != '\'' && cmd_t->input_v[input_idx][i] != '\"') || catch_end >= 0)
		{
			if (cmd_t->input_v[input_idx][i] == '\'' || cmd_t->input_v[input_idx][i] == '\"')
				break ;
			else
				catch_end = i;
			i++;
		}
		else
		{
			if (cmd_t->input_v[input_idx][i] == '\'')
				interpret = !interpret;
			i++;
			catch_start++;
		}
	}
	if (catch_end > -1)
	{
		str = ft_substr(cmd_t->input_v[input_idx], catch_start, (catch_end - catch_start + 1));
		if (!str)
			return (-1);
		backup = cmd_t->input_v[input_idx];
		edit_parsing_struct(cmd_t, cmd_t->input_v[input_idx], str);
		free(backup);
	}
	return (interpret);
}

int	replace_env_var(t_list *env, t_cmd_lst *cmd_t)
{
	int		i;
	int		interpret;
	t_list	*found;
	char	*str;
	char	*backup;

	while (i < cmd_t->input_c)
	{
		interpret = manage_arg_quotes(cmd_t, i);
		if (interpret == -1)
			return (-1);
		if (cmd_t->input_v[i][0] == '$' && interpret == 1)
		{
			found = get_env_by_id(env, cmd_t->input_v[i] + 1);
			if (!found)
				return (-1); // Env ID not found -> return error on the line
			str = ft_strdup(found->value);
			if (!str)
				return (-1); // Memory allocation error -> do smth?
			backup = cmd_t->input_v[i];
			edit_parsing_struct(cmd_t, cmd_t->input_v[i], str);
			free(backup);
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
		return (0);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] == '-')
			cmd_t->options_v[idx++] = cmd_t->arg_v[i];	
		i++;
	}
	cmd_t->options_v[idx] = NULL;
	return (1);
}

int	parse_input(t_cmd_lst *cmd_t)
{
	int	i;
	int	idx;

	cmd_t->input_v = malloc(sizeof(char *) * (cmd_t->input_c + 1));
	if (!cmd_t->input_v)
		return (0);
	i = 1;
	idx = 0;
	while (i < cmd_t->arg_c)
	{
		if (cmd_t->arg_v[i][0] != '-')
			cmd_t->input_v[idx++] = cmd_t->arg_v[i];	
		i++;
	}
	cmd_t->input_v[idx] = NULL;
	return (1);
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
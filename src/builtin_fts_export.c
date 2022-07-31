/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fts_export.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 17:10:29 by maxime            #+#    #+#             */
/*   Updated: 2022/07/29 17:20:50 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_export(t_list **env_address, t_list *env, char *id, char *value)
{
	t_list	*new;

	if (!id)
		return (0);
	while (env)
	{
		if (!ft_strncmp(id, env->id, -1))
		{
			if (env->value)
				free(env->value);
			env->value = value;
			return (9);
		}
		env = env->next;
	}
	new = ft_lstnew(id, value);
	if (!new)
		return (0);
	ft_lstadd_back(env_address, new);
	return (9);
}

static void	ft_export_no_arg(t_list *env)
{
	while (env)
	{
		if (env->id && !ft_strncmp(env->id, "_", -1))
		{
			env = env->next;
			continue ;
		}
		ft_putstr_fd(STDOUT_FILENO, "declare -x ");
		if (env->id)
			ft_putstr_fd(STDOUT_FILENO, env->id);
		if (env->value)
		{
			write(STDOUT_FILENO, "=\"", 2);
			ft_putstr_fd(STDOUT_FILENO, env->value);
			write(STDOUT_FILENO, "\"", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		env = env->next;
	}
}

int	bi_export_loop(t_list **env_address, t_cmd_lst *cmd, int i)
{
	int	ret;
	char	**split;

	if (!ft_isvalid_export(cmd, i))
		return (0);
	split = ft_split(cmd->input_v[i], '=');
	if (!split)
		return (0);
	if (!split[1] && ft_strlensep(cmd->input_v[i], '=') != \
		ft_strlen(cmd->input_v[i]))
		split[1] = ft_strdup("");
	if (!split[0] || (!split[1] && ft_strlensep(cmd->input_v[i], '=') \
		!= ft_strlen(cmd->input_v[i])))
	{
		ft_free_2d_table(split);
		return (0);
	}
	ret = ft_export(env_address, (*env_address)->next, split[0], split[1]);
	free(split);
	return (ret);
}

int	bi_export(t_list **env_address, t_cmd_lst *cmd)
{
	int	i;

	if (!cmd->input_v)
	{
		ft_export_no_arg((*env_address)->next);
		return (9);
	}
	i = 0;
	while (cmd->input_v[i])
	{
		bi_export_loop(env_address, cmd, i);
		i++;
	}
	return (9);
}

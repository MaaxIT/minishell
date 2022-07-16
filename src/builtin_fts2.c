/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fts2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:24:22 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:24:28 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	bi_env(int fd, t_list *env)
{
	env = env->next;
	while (env)
	{
		if (ft_putstr_fd(fd, env->id) == -1)
			return (0);
		if (write(fd, "=", 1) == -1)
			return (0);
		if (ft_putstr_fd(fd, env->value) == -1)
			return (0);
		if (write(fd, "\n", 1) == -1)
			return (0);
		env = env->next;
	}
	return (9);
}

int	bi_export(int fd, t_list **env_address, t_cmd_lst *cmd)
{
// NOT READY, NEED TO TREAT CASE WITH NO ARG
	(void)fd; (void)cmd;
// NEED TO PARSE THE OTHER
	t_list	*new;
	t_list	*env;
	char	*value;
	char	*id;
	char	**split;
	
	value = NULL;
	id = NULL;

	if (!cmd->input_v || !cmd->input_v[0])
		return (9); // RETURN TRE RIGHT CODE HERE!!
	split = ft_split(cmd->input_v[0], '=');
	if (split[0])
		id = split[0];
	if (split[1])
		value = split[1];
	if (!id || !value)
		return (9); // RETURN TRE RIGHT CODE HERE!!
	free(split);

	env = (*env_address)->next;
	while (env)
	{
		if (!ft_strncmp(id, env->id, ft_strlen(id)))
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

int	bi_unset(t_list **env_address, t_cmd_lst *cmd)
{
	t_list	*tmp;
	t_list	*env;
	char	*id;

	env = (*env_address)->next;
	if (!cmd->input_v || !cmd->input_v[0])
		return (0);
	id = cmd->input_v[0];
	if (!ft_strncmp(id, env->id, ft_strlen(id)))
	{
		*env_address = env->next;
		ft_lstdelone(env);
	}
	while (env->next)
	{
		if (!ft_strncmp(id, env->next->id, ft_strlen(id)))
		{
			tmp = env->next;
			env->next = env->next->next;
			ft_lstdelone(tmp);
			break;
		}
		env = env->next;
	}
	return (9);
}

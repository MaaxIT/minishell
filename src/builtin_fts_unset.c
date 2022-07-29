/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_fts_unset.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:24:22 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/29 17:08:02 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_print_invalid(int ret, char *arg)
{
	ft_putstr_fd(STDERR_FILENO, "Supershell: unset: `");
	ft_putstr_fd(STDERR_FILENO, arg);
	ft_putstr_fd(STDERR_FILENO, "' not a valid identifier\n");
	return (ret);
}

static int	ft_isvalid(t_cmd_lst *cmd)
{
	int		i;
	char	*str;

	if (!cmd->input_v || !cmd->input_v[0])
		return (0);
	str = cmd->input_v[0];
	if (str[0] && str[0] >= '0' && str[0] <= '9')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (9);
}

static void	ft_unset(t_list **env_address, t_list *env, char *id)
{
	t_list	*tmp;

	if (!ft_strncmp(id, env->id, -1))
	{
		(*env_address)->next = env->next;
		ft_lstdelone(env);
	}
	while (env->next)
	{
		if (!ft_strncmp(id, env->next->id, -1))
		{
			tmp = env->next;
			env->next = env->next->next;
			ft_lstdelone(tmp);
			break ;
		}
		env = env->next;
	}
}

int	bi_unset(t_list **env_address, t_cmd_lst *cmd)
{
	if (!ft_isvalid(cmd))
	{
		if (cmd->input_v[0])
			return (ft_print_invalid(9, cmd->input_v[0]));
		return (9);
	}
	ft_unset(env_address, (*env_address)->next, cmd->input_v[0]);
	return (9);
}

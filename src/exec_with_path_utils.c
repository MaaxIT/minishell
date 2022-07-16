/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_with_path_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/16 19:49:28 by mbennafl          #+#    #+#             */
/*   Updated: 2022/07/16 19:50:00 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**malloc_paths(char **dirpaths)
{
	int		i;
	char	**paths;

	i = 0;
	while (dirpaths[i])
		i++;
	paths = malloc(sizeof(char *) * (i + 1));
	if (!paths)
		return (NULL);
	paths[i] = NULL;
	return (paths);
}

static char	**add_cmd_to_path(char **dirpaths, const char *cmd)
{
	int		i;
	char	**paths;
	char	*tmp;

	paths = malloc_paths(dirpaths); // if !path, does not enter the while loop and return path = NULL
	i = 0;
	while (paths && dirpaths[i])
	{
		tmp = ft_strjoin(dirpaths[i], "/");
		if (!tmp)
		{
			ft_free_2d_table(paths);
			return (NULL);
		}
		paths[i] = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!paths[i])
		{
			ft_free_2d_table(paths);
			return (NULL);
		}
		i++;
	}
	return (paths);
}

char	**find_paths(t_list *env, const char *cmd)
{
	const char	*id = "PATH";
	char		*global_paths;
	char		**dirpaths;
	char		**paths;

	while (env && ft_strncmp(id, env->id, ft_strlen(id)))
		env = env->next;
	if (!ft_strncmp(id, env->id, ft_strlen(id)))
		global_paths = env->value;
	else
		return (NULL);
	dirpaths = ft_split(global_paths, ':');
	paths = add_cmd_to_path(dirpaths, cmd);
	ft_free_2d_table(dirpaths);
	if (!paths)
		return (NULL);
	return (paths);
}

char	**create_envp(t_list *env)
{
	char	**envp;
	int		i;
	char	*tmp;

	i = 0;
	envp = malloc(sizeof(char *) * (ft_lstsize(env) + 1));
	if (!envp)
		return (NULL);
	envp[ft_lstsize(env)] = NULL;
	while (env)
	{
		tmp = ft_strjoin(env->id, "=");
		if (!tmp)
			return (NULL);
		envp[i] = ft_strjoin(tmp, env->value);
		if (!envp[i])
			return (NULL);
		free(tmp);
		env = env->next;
		i++;
	}
	return (envp);
}

int	ewp_clear(int ret, char **paths, char **envp)
{
	if (paths)
		ft_free_2d_table(paths);
	if (envp)
		ft_free_2d_table(envp);
	return (ret);
}

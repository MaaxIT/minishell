/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:55:04 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/24 21:36:58 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Updates $? global variable to current errno value */
int	update_env_return(t_list **env)
{
	static int	must_free = 9;

	if (!env)
		return (0);
	if (must_free)
		free((*env)->value);
	(*env)->value = ft_itoa(errno);
	if (!(*env)->value)
		must_free = 0;
	else
		must_free = 9;
	return (must_free);
}

/* Return the offset from where the value is starting 
Example: "TEST=42" will return 5 because "42" (the value) 
has 5 bytes offset from the start
*/
static int	env_value_offset(char *envline)
{
	int	i;

	if (!envline)
		return (-1);
	i = 0;
	while (envline[i])
	{
		if (envline[i] == '=')
			return (i);
		i++;
	}
	return (0);
}

static int	init_env_list_loop(char **envp, t_list **head, char *id, char *val)
{
	t_list	*new;
	int		offset;

	while (*envp)
	{
		offset = env_value_offset(*envp);
		if (offset == -1)
			return (-1);
		if (offset > 0) // otherwise the line has no = (what we do so?)
		{
			id = ft_substr(*envp, 0, offset);
			if (!id)
				return (-1);
			val = ft_substr(*envp, offset + 1, -1);
			if (!val)
			{
				free(id);
				return (-1);
			}
			new = ft_lstnew(id, val);
			ft_lstadd_back(head, new);
		}
		envp++;
	}
	return (0);
}

/* Initialize the environment linked list 
Return -1 on failure
Return 0 on success
*/
int	init_env_list(t_list **head, char **envp)
{
	t_list	*new;
	char	*id;
	char	*value;

	if (!envp)
		return (-1);
	id = malloc(2 * sizeof(char));
	if (!id)
		return (0);//PROTECT
	ft_strlcpy(id, "?", 2);
	value = malloc(2 * sizeof(char));
	if (!value)
		return (0);//PROTECT
	ft_strlcpy(value, "0", 2);
	new = ft_lstnew(id, value);
	ft_lstadd_back(head, new);
	if (init_env_list_loop(envp, head, id, value) == -1)
		return (-1);
	return (0);
}

/* Get environment structure by its id */
t_list	*get_env_by_id(t_list *env, char *id)
{
	t_list	*head;

	head = env;
	while (head)
	{
		if (ft_strncmp(head->id, id, -1) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

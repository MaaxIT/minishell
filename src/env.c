/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:55:04 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/06/23 21:39:25 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Return the offset from where the value is starting 
Example: "TEST=42" will return 5 because "42" (the value) 
has 5 bytes offset from the start
*/
int	env_value_offset(char *envline)
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

/* Initialize the environment linked list 
Return -1 on failure
Return 0 on success
*/
int	init_env_list(t_list **head, char **envp)
{
	t_list	*new;
	int		offset;
	char	*id;
	char	*value;

	if (!envp)
		return (-1);
	while (*envp)
	{
		offset = env_value_offset(*envp);
		if (offset == -1)
			return (-1); // This will NEVER happen since -1 is returned when *envp == NULL (can't be possible)
		if (offset > 0) // otherwise the line has no = and it should not hpapend (what do we do then?)
		{
			id = ft_substr(*envp, 0, offset);
			if (!id)
				return (-1);
			value = ft_substr(*envp, offset + 1, -1);
			if (!value)
			{
				free(id);
				return (-1);
			}
			new = ft_lstnew(id, value);
			ft_lstadd_back(head, new);
		}
		envp++;
	}
	return (0);
}
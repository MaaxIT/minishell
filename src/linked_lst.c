/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_lst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 20:41:48 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/06/23 20:53:31 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Add a new link in the list */
void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*last;

	if (!lst || !new)
		return ;
	if (*lst)
	{
		last = *lst;
		while (last && last->next)
			last = last->next;
		last->next = new;
	}
	else
		*lst = new;
}

/* Create a new chain */
t_list	*ft_lstnew(char *id, char *value)
{
	t_list	*head;

	head = malloc(sizeof(t_list));
	if (!head)
		return (NULL);
	head->id = id;
	head->value = value;
	head->next = NULL;
	return (head);
}

/* Get the amount of links in the list */
int	ft_lstsize(t_list *lst)
{
	size_t	size;

	if (!lst)
		return (0);
	size = 0;
	while (lst)
	{
		size++;
		lst = lst->next;
	}
	return (size);
}

/* Clear (free) a single link of the list */
void	ft_lstdelone(t_list *lst)
{
	if (!lst)
		return ;
	free(lst->id);
	free(lst->value);
	free(lst);
}

/* Clear (free) the whole list */
void	ft_lstclear(t_list **lst)
{
	t_list	*head;
	t_list	*backup;

	if (!lst || !*lst)
		return ;
	head = *lst;
	while (head)
	{
		backup = head->next;
		ft_lstdelone(head);
		head = backup;
	}
	*lst = NULL;
}
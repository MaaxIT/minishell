#include "minishell.h"

void	ft_cmd_lstadd_back(t_cmd_lst **top, t_cmd_lst *new)
{
	t_cmd_lst	*i;

	if (*top)
	{
		i = *top;
		while (i->next)
			i = i->next;
		i->next = new;
	}
	else
		*top = new;
}

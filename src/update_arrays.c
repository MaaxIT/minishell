#include "minishell.h"

static char	**ft_pop_null(char **tab, int *tabsize)
{
	int	i;

	i = 0;
	while (i < *tabsize)
	{
		if (tab[i] == NULL)
		{
			tab = ft_pop(tab, i, *tabsize);
			(*tabsize)--;
			if (!tab)
			{
				ft_free_2d_table(tab);
				return (NULL);
			}
			i = -1;
		}
		i++;
	}
	return (tab);
}

int	update_inputv_optionsv_after_redir(t_cmd_lst *cmd)
{
	cmd->options_v = ft_pop_null(cmd->options_v, &cmd->options_c);
	if (!cmd->options_v)
		return (0);
	cmd->input_v = ft_pop_null(cmd->input_v, &cmd->input_c);
	if (!cmd->input_v)
		return (0);
	return (9);
}

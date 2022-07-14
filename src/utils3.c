#include "minishell.h"

char	**ft_pop(char **tab, int idx, int tabsize)
{
	char	**ret;
	int	i;
	int	j;

	if (idx >= tabsize || !tab)
	{
		ft_free_2d_table(tab);
		return (0);
	}
	ret = malloc(sizeof(char *) * tabsize);
	if (!ret)
	{
		ft_free_2d_table(tab);
		return (NULL);
	}
	ret[tabsize - 1] = NULL;
	i = 0;
	j = 0;
	while (j < tabsize && i < tabsize - 1)
	{
		if (j == idx)
			j++;
		ret[i] = tab[j];
		i++;
		j++;
	}
	free(tab[idx]);
	free(tab);
	return (ret);
}

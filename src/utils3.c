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
	if (tab[idx])
		free(tab[idx]);
	free(tab);
	return (ret);
}

static int	ft_check_fit(char *str, char *set)
{
	size_t	i;
	size_t	set_len;

	i = 0;
	set_len = ft_strlen(set);
	while (str[i] && set[i] && i < set_len)
	{
		if (str[i] == set[i])
		{
			if (i + 1 == set_len)
				return (1);
		}
		else
			return (0);
		i++;
	}
	return (0);
}

char	*ft_strnstr(char *str, char *set, size_t len)
{
	size_t	i;
	size_t	set_len;

	i = 0;
	set_len = ft_strlen(set);
	if (set_len == 0 || !set || !set[0])
		return (str);
	while (str[i] && (i + set_len) <= len)
	{
		if (str[i] == set[0] && ft_check_fit(&str[i], set))
			return (str + i);
		i++;
	}
	return (NULL);
}

int	ft_strlensep(const char *str, char sep)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	return (i);
}

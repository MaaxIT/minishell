#include "minishell.h"

static int	ft_strlensep(const char *str, char sep)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != sep)
		i++;
	return (i);
}

static char	**malloc_words(const char *str, char sep)
{
	int	i;
	int	nbr_words;
	char	**ret;

	nbr_words = 0;
	if (str[0] != sep)
		nbr_words = 1;
	else
		nbr_words = 0;
	i = 1;
	while (str[i])
	{
		if (str[i] && str[i - 1] == sep && str[i] != sep)
			nbr_words++;
		i++;
	}
	ret = malloc(sizeof(char *) * (nbr_words + 1));
	if (!ret)
		return (NULL);
	ret[nbr_words + 1] = NULL;
	return (ret);
}

char	**ft_split(const char *str, char sep)
{
	char	**ret;
	int	i;
	int	j;

	ret = malloc_words(str, sep);
	if (!ret)
		return (NULL);
	i = 0;
	j = 0;
	while (str[j])
	{
		while (str[j] && str[j] == sep)
			j++;
		if (!str[j])
			break;
		ret[i] = malloc(sizeof(char) * (ft_strlensep(&str[j], sep) + 1));
		if (!ret[i])
		{
			ft_free_2d_table(ret);
			return (NULL);
		}
		ft_strlcpy(ret[i], &str[j], ft_strlensep(&str[j], sep) + 1);
		while (str[j] && str[j] != sep)
			j++;
		i++;
	}
	return (ret);
}

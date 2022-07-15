#include "minishell.h"

static int	find_closing_quote_idx(const char *str, int i, char quote)
{
	while (str[i] && str[i] != quote)
		i++;
	return (i);
}

static char	**malloc_words(const char *str, char sep)
{
	int	i;
	int	nbr_words;
	char	**ret;

	if (str[0] != sep)
		nbr_words = 1;
	else
		nbr_words = 0;
	i = -1;
	while (str[++i])
	{
		while (str[i] && str[i] == sep)
			i++;
		if (i && str[i - 1] == sep && str[i] != sep)
			nbr_words++;
		if (str[i] == '\'')
			i = find_closing_quote_idx(str, i + 1, '\'');
		else if (str[i] == '\"')
			i = find_closing_quote_idx(str, i + 1, '\"');
	}
	ret = malloc(sizeof(char *) * (nbr_words + 1));
	if (!ret)
		return (NULL);
	ret[nbr_words] = NULL;
	return (ret);
}

static int	find_next_sep_out_quotes_idx(const char *str, int i, char sep)
{
	int	capt_single;
	int	capt_double;

	capt_single = 0;
	capt_double = 0;
	while (str[i])
	{
		if (str[i] && str[i] == '\'')
			capt_single = !capt_single;
		else if (str[i] && str[i] == '\"')
			capt_double = !capt_double;
		else if (str[i] && str[i] == sep && \
			!capt_single && !capt_double)
			return (i);
		i++;
	}
	return (i);
}

char	**ft_split_out_quotes(const char *str, char sep)
{
	char	**ret;
	int	i;
	int	j;

	ret = malloc_words(str, sep);
	if (!ret)
		return (NULL);	
	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && str[i] == sep)
			i++;
		if (!str[i])
			break;
		ret[j] = malloc(sizeof(char) * (-i + 1 + \
			find_next_sep_out_quotes_idx(str, i, sep)));
		if (!ret[j])
		{
			ft_free_2d_table(ret);
			return (NULL);
		}
		ft_strlcpy(ret[j], &str[i], \
			find_next_sep_out_quotes_idx(str, i, sep) - i + 1);
		i = find_next_sep_out_quotes_idx(str, i, sep);
		j++;
	}
	return (ret);
}

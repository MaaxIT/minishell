#include "minishell.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	int	len;
	char	*ret;
	int	i;
	int	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		ret[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
	{
		ret[i] = s2[j];
		i++;
		j++;
	}
	ret[i] = '\0';
	return (ret);
}

void	ft_double_free(char **to_free)
{
	int	i;

	i = 0;
	while (to_free[i])
		free(to_free[i++]);
	free(to_free);
}

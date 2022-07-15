#include "minishell.h"

char	*ft_strdup(const char *str)
{
	size_t	i;
	size_t	str_len;
	char	*ptr;

	str_len = ft_strlen(str);
	ptr = malloc((str_len + 1) * sizeof(char));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < str_len)
	{
		ptr[i] = str[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ft_strndup(const char *str, size_t n)
{
	size_t	i;
	size_t	str_len;
	char	*ret;

	str_len = ft_strlen(str);
	if (n >= str_len)
		ret = ft_strdup(str);
	else
	{

		ret = malloc(sizeof(char) * (n + 1));
		if (!ret)
			return (NULL);
		i = 0;
		while (str[i] && i < n)
		{
			ret[i] = str[i];
			i++;
		}
		ret[i] = '\0';
	}
	return (ret);
}

#include "minishell.h"

static int	ft_nbrlen(int nbr)
{
	int	len;

	if (!nbr)
		return (1);
	len = 0;
	if (nbr < 0)
		len++;
	while (nbr)
	{
		nbr /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int nbr)
{
	char	*ret;
	int	i;
	int	sign;

	ret = malloc(sizeof(char) * (ft_nbrlen(nbr) + 1));
	if (!ret)
		return (NULL);// IS THAT ENOUGH?
	sign = 1;
	if (nbr < 0)
		sign = -1;
	i = ft_nbrlen(nbr);
	nbr *= sign;
	ret[i--] = '\0';
	while (i >= 0)
	{
		if (sign == -1 && i == 0)
		{
			ret[i] = '-';
			break;
		}
		ret[i] = nbr % 10 + '0';
		nbr /= 10;
		i--;
	}
	return (ret);
}

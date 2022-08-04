#include "minishell.h"

int	closing_idx(const char *str, int i, char quote)
{
	while (str[i] && str[i] != quote)
		i++;
	if (!str[i])
		return (-1);
	return (i);
}

void	print_err_unclosed_pipe(void)
{
	ft_putstr_fd(STDERR_FILENO, \
	"Supershell: syntax error near unexpected token `|'\n");
}

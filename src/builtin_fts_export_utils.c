#include "minishell.h"

int	ft_print_invalid_export(int ret, char *arg)
{
	ft_putstr_fd(STDERR_FILENO, "Supershell: export: `");
	ft_putstr_fd(STDERR_FILENO, arg);
	ft_putstr_fd(STDERR_FILENO, "' not a valid identifier\n");
	return (ret);
}

int	ft_isvalid_export(t_cmd_lst *cmd, int idx)
{
	int		i;
	char	*str;

	if (!cmd->input_v || !cmd->input_v[idx])
		return (0);
	str = cmd->input_v[idx];
	i = 0;
	if (str[0] && str[0] >= '0' && str[0] <= '9')
		return (ft_print_invalid_export(0, str));
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ft_print_invalid_export(0, str));
		i++;
	}
	if (str[i] && str[i] != '=')
		return (0);
	return (9);
}

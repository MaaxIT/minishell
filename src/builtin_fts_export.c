#include "minishell.h"

static int	ft_print_invalid(int ret, char *arg)
{
	ft_putstr_fd(STDERR_FILENO, "Supershell: export: `");
	ft_putstr_fd(STDERR_FILENO, arg);
	ft_putstr_fd(STDERR_FILENO, "' not a valid identifier\n");
	return (ret);
}

static int	ft_isvalid(t_cmd_lst *cmd)
{
	int	i;
	char	*str;

	if (!cmd->input_v || !cmd->input_v[0])
		return (0);
	str = cmd->input_v[0];
	i = 0;
	if (str[0] && str[0] >= '0' && str[0] <= '9')
		return(ft_print_invalid(0, str));
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return(ft_print_invalid(0, str));
		i++;
	}
	if (!str[i] || str[i] != '=')
		return (0);
	return (9);
}

static int	ft_export(t_list **env_address, t_list *env, char *id, char *value)
{
	t_list	*new;

	if (!id)
		return (9); // RETURN TRE RIGHT CODE HERE!!
	else if (!value)
		value = ft_strdup("");
	if (!value)
		return (9);
	while (env)
	{
		if (!ft_strncmp(id, env->id, -1))
		{
			if (env->value)
				free(env->value);
			env->value = value;
			return (9);
		}
		env = env->next;
	}
	new = ft_lstnew(id, value);
	if (!new)
		return (0);
	ft_lstadd_back(env_address, new);
	return (9); //RETURN RIGHT CODE
}

int	bi_export(t_list **env_address, t_cmd_lst *cmd)
{
	int	ret;
	char	**split;

	if (!cmd->input_v)
	{
	//	ft_export_no_arg();
		return (9); //OK?
	}
	if (!ft_isvalid(cmd))
		return (9); // RETURN THE RIGHT CODE HERE!!
	split = ft_split(cmd->input_v[0], '=');
	if (!split)
		return (9); //RETURN RIGHT CODE!!
	if (!split[0])
	{
		ft_free_2d_table(split);
		return (9); //RETURN RIGHT CODE!!
	}
	ret = ft_export(env_address, (*env_address)->next, split[0], split[1]);
	free(split);
	return (ret);
}

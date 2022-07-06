#include "minishell.h"

int	bi_env(int fd, t_list *env)
{
	env = env->next;
	while (env)
	{
		if (ft_putstr_fd(fd, env->id) == -1)
			return (0);
		if (write(fd, "=", 1) == -1)
			return (0);
		if (ft_putstr_fd(fd, env->value) == -1)
			return (0);
		if (write(fd, "\n", 1) == -1)
			return (0);
		env = env->next;
	}
	return (9);
}

int	bi_export(int fd, t_list **env_address, t_cmd_lst *cmd)
{
// NOT READY, NEED TO TREAT CASE WITH NO ARG
	(void)fd; (void)cmd; char *value = "val"; char *id = "id";
// NEED TO PARSE THE OTHER
	t_list	*new;
	t_list	*env;

	env = (*env_address)->next;
	while (env)
	{
		if (!ft_strncmp(id, env->id, ft_strlen(id)))
		{
			free(env->value);
			env->value = malloc(sizeof(char) * ft_strlen(value));
			if (!env->value)
				return (0);
			ft_strlcpy(env->value, value, ft_strlen(value));
			return (9);
		}
		env = env->next;
	}
	new = ft_lstnew(id, value);
	if (!new)
		return (0);
	ft_lstadd_back(env_address, new);
	return (9);
}

int	bi_unset(t_list **env_address, t_cmd_lst *cmd)
{
	t_list	*tmp;
	t_list	*env;
	char	*id;

	env = (*env_address)->next;
	if (!cmd->input_v || !cmd->input_v[0])
		return (0);
	id = cmd->input_v[0];
	if (!ft_strncmp(id, env->id, ft_strlen(id)))
	{
		*env_address = env->next;
		ft_lstdelone(env);
	}
	while (env->next)
	{
		if (!ft_strncmp(id, env->next->id, ft_strlen(id)))
		{
			tmp = env->next;
			env->next = env->next->next;
			ft_lstdelone(tmp);
			break;
		}
		env = env->next;
	}
	return (9);
}

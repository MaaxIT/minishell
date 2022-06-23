#include "minishell.h"

int	bi_env(int fd, t_list *env)
{
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

void	bi_export(t_list **env_address, char *id, char *value)
{
	t_list	*new;
	t_list	*env;

	env = *env_address;
	while (env)
	{
		if (!ft_strncmp(id, env->id, ft_strlen(id)))
			return;
		env = env->next;
	}
	new = ft_lstnew(id, value);
	ft_lstadd_back(env_address, new);
}

void	bi_unset(t_list **env_address, char *id)
{
	t_list	*tmp;
	t_list	*env;

	env = *env_address;
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
}

#include "minishell.h"

int	run_command(t_list **env, t_cmd_lst *cmd)
{
	int		fd;
	int		err;

// NEED TO PARSE AND INITIATE THE FD HERE
	fd = 1;
	if (!ft_strncmp(cmd->arg_v[0], "echo", ft_strlen(cmd->arg_v[0])))
		err = bi_echo(fd, cmd);
	else if (!ft_strncmp(cmd->arg_v[0], "cd", ft_strlen(cmd->arg_v[0])))
		err = bi_cd(fd, cmd);
	else if (!ft_strncmp(cmd->arg_v[0], "pwd", ft_strlen(cmd->arg_v[0])))
		err = bi_pwd(fd);
	else if (!ft_strncmp(cmd->arg_v[0], "exit", ft_strlen(cmd->arg_v[0])))
		err = bi_exit(fd, env);
	else if (!ft_strncmp(cmd->arg_v[0], "env", ft_strlen(cmd->arg_v[0])))
		err = bi_env(fd, *env);
	else if (!ft_strncmp(cmd->arg_v[0], "export", ft_strlen(cmd->arg_v[0])))
		err = bi_export(fd, env, cmd);
	else if (!ft_strncmp(cmd->arg_v[0], "unset", ft_strlen(cmd->arg_v[0])))
		err = bi_unset(env, cmd);
	else
		err = exec_with_path(env, cmd->arg_v[0], cmd->arg_v);
	return (err);
}

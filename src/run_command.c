#include "minishell.h"

int	run_command(t_list **env, t_cmd_lst *cmd)
{
	int		fd;
	int		err;
	int		len;

	// NEED TO PARSE AND INITIATE THE FD HERE
	fd = 1;
	len = ft_strlen(cmd->binary);
	if (!ft_strncmp(cmd->binary, "echo", len))
		err = bi_echo(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "cd", len))
		err = bi_cd(fd, cmd);
	else if (!ft_strncmp(cmd->binary, "pwd", len))
		err = bi_pwd(fd);
	else if (!ft_strncmp(cmd->binary, "exit", len))
		err = bi_exit(fd, env, cmd);
	else if (!ft_strncmp(cmd->binary, "env", len))
		err = bi_env(fd, *env);
	else if (!ft_strncmp(cmd->binary, "export", len))
		err = bi_export(fd, env, cmd);
	else if (!ft_strncmp(cmd->binary, "unset", len))
		err = bi_unset(env, cmd);
	else
		err = exec_with_path(env, cmd->binary, cmd->arg_v);
	return (err);
}

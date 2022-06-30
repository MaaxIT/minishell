/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maaxit <maaxit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:53:19 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/30 05:25:01 by maaxit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Internal libraries */
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Prefix constants */
# define SHELL_PREFIX	"\033[1;36mSuperShell\033[1;32m:>\033[m"

/* Our structures */
typedef struct s_list {
	char			*id;
	char			*value;
	struct s_list	*next;
}	t_list;

typedef struct s_command {
	char	*origin; // The original string command
	char	*bin; // Binary command
	int		argc; // Arguments count
	char	**argv; // Arguments with - before (echo **-n**)
	int		splitc; // Split count
	char	**splitv; // Split of everything except arguments above
	char	*splitstr; // Same as splitv but in a single string
	char	**full; // Full split (gathering both above)
	char	*fullstr; // Full command string without command execution
}	t_command;

/* Signals prototypes */
void		signals_init();

/* Builtins	prototypes */
int			bi_echo(int fd, int newline, char *str);
int			bi_cd(int fd, char *str);
int			bi_pwd(int fd);
int			bi_exit(int fd, t_list **env);
int			bi_env(int fd, t_list *env);
void		bi_export(t_list **env, char *id, char *value);
void		bi_unset(t_list **env, char *id);

/* exec_with_path and its utils */
int		exec_with_path(t_list **env, const char *cmd, char **argv);
char		**create_envp(t_list *env);
char		**find_paths(t_list *env, const char *cmd);
int		ewp_clear(int ret, char **paths, char **env);

/* Redirections prototypes */
int		rd_output(char *path);
int		rd_output_append(char *path);
void		rd_delimiter(char *delimiter);

/* Utils prototypes */
size_t	ft_strlen(const char *str);
int		ft_putstr_fd(int fd, char *str);
char	*ft_substr(const char *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dst, char const *src, size_t n);
int		ft_strncmp(char const *s1, char const *s2, size_t nbr);
char	*ft_itoa(int nbr);
char	*ft_strjoin(const char *s1, const char *s2);
char	**ft_split(const char *str, char sep);
void	ft_double_free(char **to_free);
int			ft_strincludes(const char *str, char c);
size_t		ft_strlcat(char *dst, char const *src, size_t nbr);
char    	*ft_arrjoin(char **split, size_t len, char sep);
void		ft_bzero(void *str, size_t size);

/* Split prototype */
char    	**split_command(char *cmd);

/* Parse prototypes */
t_command	*parse_cmd(char *cmd);
void		free_command(t_command *cmd);

/* Errors prototypes */
int			print_error(int ret);

/* Environment prototypes */
int		init_env_list(t_list **head, char **envp);
int		update_env_return(t_list **env);

/* Linked list prototypes */
void		ft_lstadd_back(t_list **lst, t_list *new);
t_list		*ft_lstnew(char *id, char *value);
int			ft_lstsize(t_list *lst);
void		ft_lstdelone(t_list *lst);
void		ft_lstclear(t_list **lst);

/* Other prototypes */
int			new_cmd(t_list **env);

#endif

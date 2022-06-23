/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:53:19 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 21:53:42 by mpeharpr         ###   ########.fr       */
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
# include <readline/readline.h>

/* Prefix constants */
# define SHELL_PREFIX	"\033[1;36mSuperShell\033[1;32m:>\033[m"

/* Our structures */
typedef struct s_list {
	char			*id;
	char			*value;
	struct s_list	*next;
}	t_list;

/* Signals prototypes */
void	signals_init();

/* Builtins	prototypes */
int		bi_echo(int fd, int newline, char *str);
int		bi_cd(int fd, char *str);
int		bi_pwd(int fd);

/* Redirections prototypes */
int		rd_output(char *path);
int		rd_output_append(char *path);

/* Utils prototypes */
size_t	ft_strlen(char *str);
int		ft_putstr_fd(int fd, char *str);
char	*ft_substr(char const *s, unsigned int start, size_t len);
size_t	ft_strlcpy(char *dst, char const *src, size_t n);
int		ft_strncmp(char const *s1, char const *s2, size_t nbr);

/* Errors prototypes */
int		print_error(int ret);

/* Environment prototypes */
int		init_env_list(t_list **head, char **envp);
int		env_value_offset(char *envline);

/* Linked list prototypes */
void	ft_lstadd_back(t_list **lst, t_list *new);
t_list	*ft_lstnew(char *id, char *value);
int		ft_lstsize(t_list *lst);
void	ft_lstdelone(t_list *lst);
void	ft_lstclear(t_list **lst);

/* Other prototypes */
int	new_cmd(t_list **env);

#endif

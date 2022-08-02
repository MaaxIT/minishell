/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:53:19 by mbennafl          #+#    #+#             */
/*   Updated: 2022/08/02 01:22:32 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* Internal libraries */
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

/* Prefix constants */
# define SHELL_PREFIX	"Supershell:> "

/* Global variable */
pid_t	g_pid;

/* Our structures */
typedef struct s_list {
	char			*id;
	char			*value;
	struct s_list	*next;
}	t_list;

/* Example: echo -n Hello world */
typedef struct s_cmd_lst {
	char	*original;
	char	*binary;
	int		options_c;
	char	**options_v;
	int		input_c;
	char	**input_v;
	int		arg_c;
	char	**arg_v;
	char	**parsing_v;
	char	*input_path;
	int		input_fd;
	char	input_type;
	char	*output_path;
	int		output_fd;
	char	output_type;
	struct s_cmd_lst	*next;
}	t_cmd_lst;

// REMOVE THAT (main.c)
int			print_structure(t_cmd_lst *cmd_t);

/* Signals prototype */
int			signals_init(void);

/* Pipe prototype */
int			ft_pipe(t_list **env, t_cmd_lst *cmd);

/* Builtins	prototypes */
int			bi_echo(int fd, t_cmd_lst *cmd);
int			bi_cd(int fd, t_cmd_lst *cmd);
int			bi_pwd(int fd);
int			bi_exit(int fd, t_list **env, t_cmd_lst *cmd);
int			bi_env(int fd, t_list *env);
int			bi_export(t_list **env, t_cmd_lst *cmd);
int			bi_unset(t_list **env, t_cmd_lst *cmd);

/* Builtins	utils prototypes */
int			ft_print_invalid_export(int ret, char *arg);
int			ft_isvalid_export(t_cmd_lst *cmd, int idx);

/* exec_with_path and its utils */
int			exec_with_path(t_list **env, const char *cmd, char **argv);
char		**create_envp(t_list *env);
char		**find_paths(t_list *env, const char *cmd);
int			ewp_clear(int ret, char **paths, char **env);

/* Redirections prototypes */
int			rd_input(char *path);
int			rd_output(char *path);
int			rd_output_append(char *path);
int			rd_delimiter(char *delimiter);

/* Redirections parsing prototypes */
int			input_not_existing(int x, char *input_path);
int			concat_callback(t_cmd_lst *cmd_t, int *idx, int len, int *i, char **path_type);
int			gen_path_concat(t_cmd_lst *cmd_t, char **path_type);
int			separated_callback(t_cmd_lst *cmd_t, char **path_type, int i, int *idx);
int			gen_path_separated(t_cmd_lst *cmd_t, char **path_type);

/* Utils prototypes */
size_t		ft_strlen(const char *str);
int			ft_putstr_fd(int fd, const char *str);
char		*ft_substr(const char *s, unsigned int start, size_t len);
size_t		ft_strlcpy(char *dst, const char *src, size_t n);
int			ft_strncmp(const char *s1, const char *s2, size_t nbr);
char		*ft_itoa(int nbr);
char		*ft_strjoin(const char *s1, const char *s2);
void		ft_free_2d_table(char **to_free);
int			ft_strincludes(const char *str, char c);
size_t		ft_strlcat(char *dst, const char *src, size_t nbr);
char		*ft_arrjoin(char **split, size_t len, char sep);
char		*ft_strtrim(const char *s1, const char *set);
char		**ft_pop(char **tab, int idx, int tabsize);
char		*ft_strnstr(char *str, char *set, size_t len);
size_t		ft_strlensep(const char *str, char sep);
void		ft_bzero(void *str, size_t size);
char		*ft_strdup(const char *str);
char		*ft_strndup(const char *str, size_t n);
int			ft_is_a_whitespace_or_empty_string(const char *str);
int			ft_isalnum(char c);
int			is_envchar(int ch, int is_first);
long int	ft_atol(const char *str);

/* Split prototype */
char		**split_cmd_lst(char *cmd);
char		**ft_split(const char *str, char sep);
char		**ft_split_out_quotes(const char *str, char sep);
char		**free_split(char **arr);
int			count_splits(char *cmd, char *parsing);
char		**free_and_return_split(char *parsing);
int			contains_mean_char(char *str, char *parse, char c);

/* Parsing prototypes */
t_cmd_lst	*initialize_command(char *line, t_list *env);
int			free_command_lst(t_cmd_lst *cmd);
void		parse_counts(t_cmd_lst *cmd_t);
int			parse_input(t_cmd_lst *cmd_t);
int			parse_options(t_cmd_lst *cmd_t);
int			parse_quotes(t_cmd_lst *cmd_t, t_list *env);
int			parse_redirections(t_cmd_lst *cmd_t);
int			update_inputv_optionsv_after_redir(t_cmd_lst *cmd);
int			sync_arg(t_cmd_lst *cmd_t, char *old_input, char *new_input, int after_parsing);
int			parse_input_quotes(char *input, char *parse);
int			get_input_idx(t_cmd_lst *cmd_t, char *str);
int			rem_from_both(t_cmd_lst *cmd_t, char *addr);
int			parse_quotes_init(t_cmd_lst *cmd_t, int *i);
int			invalid_envvar(t_cmd_lst *cmd_t, char **sub, char *parse, int i);
int			valid_envvar(t_cmd_lst *cmd_t, char **sub, char *parse, char *val, int i);
int			get_next_id(t_cmd_lst *cmd_t, int size, char **arr, char *input);

/* Memory utils prototypes */
char		*insert_str_at_index(char *str, char *to_insert, int idx);
char		*ft_strdup_char(char c, size_t repeats);
int			replace_sub(t_cmd_lst *cmd_t, char **s, char *old, char *nw);
int			replace_sub_parse(t_cmd_lst *cmd_t, char **str, char *old, char *newsub);
int			rem_char(t_cmd_lst *cmd_t, char **s, int idx);
void		initialize_structure(t_cmd_lst *cmd_t);

/* Errors prototypes */
int			print_error(int ret);

/* Environment prototypes */
int			init_env_list(t_list **head, char **envp);
int			update_env_return(t_list **env);
t_list		*get_env_by_id(t_list *env, char *id);

/* Linked (cmd) list prototypes */
void		ft_lstadd_back(t_list **lst, t_list *new);
t_list		*ft_lstnew(char *id, char *value);
int			ft_lstsize(t_list *lst);
void		ft_lstdelone(t_list *lst);
void		ft_lstclear(t_list **lst);

void		ft_cmd_lstadd_back(t_cmd_lst **top, t_cmd_lst *new);

/* Other prototypes */
int			run(t_list **env, t_cmd_lst *cmd);
int			run_command(t_list **env, t_cmd_lst *cmd, t_cmd_lst *top_cmd);
int			no_pipe(t_list **env, t_cmd_lst *cmd);

#endif

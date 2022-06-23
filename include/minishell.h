/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:53:19 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 18:58:00 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>

/* Builtins	prototypes */
int	bi_echo(int fd, int newline, char *str);
int	bi_cd(int fd, char *str);
int	bi_pwd(int fd);

/* Redirections prototypes */
int	rd_output(char *path);
int	rd_output_append(char *path);

/* Utils prototypes */
int	ft_putstr_fd(int fd, char *str);

/* Errors prototypes */
int	print_error(int ret);

#endif

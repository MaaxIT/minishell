/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennafl <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 14:53:19 by mbennafl          #+#    #+#             */
/*   Updated: 2022/06/23 16:50:25 by mbennafl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <fcntl.h>
# include <errno.h>
# include <readline/readline.h>

// Builtins	functions declarations

int	bi_echo(int fd, int newline, char *str);
int	bi_cd(int fd, char *str);
int	bi_pwd(int fd);

// Redirections functions declarations

int	rd_output(char *path);
int	rd_output_append(char *path);

// Utils	functions declarations

int	ft_putstr_fd(int fd, char *str);

// Errors	functions declarations

int	print_error(int ret);

#endif

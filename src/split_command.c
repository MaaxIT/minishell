/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 23:11:22 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/09 00:16:42 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Make sure that there is an even amount of single & doubles quotes in
the command line - this way, we can make sure all quotes are closing and
we don't have any unclosed quote as specified in the subject */
static int  quotes_even(char *cmd)
{
    int i;
    int simples;
    int doubles;

    simples = 0;
    doubles = 0;
    i = 0;
    while (cmd[i])
    {
        if (cmd[i] == '\'')
            simples++;
        else if (cmd[i] == '\"')
            doubles++;
        i++;
    }
    return (simples % 2 == 0 && doubles % 2 == 0);
}

/* Count the amount of splits we will have in order to pre-allocate */
/*static */int  count_splits(char *cmd)
{
    int     i;
    int     idx;
    int     capt;

    idx = 0;
    capt = 0;
    i = 0;
    while (cmd[i])
    {
	while (cmd[i] && cmd[i] == ' ' && !capt)
		i++;
        if (cmd[i] && (cmd[i] == '\'' || cmd[i] == '\"') && cmd[i + 1])
        {
		if (!capt)
			idx++;
		capt = !capt;
	}
        else if (cmd[i] && cmd[i] != ' ' && !capt)
            idx++;
	while (cmd[i + 1] && cmd[i + 1] != ' ' && !capt)
		i++;
        i++;
    }
    return (idx);
}

/* Split the str with spaces + simple & double quotes */
static int  split_with_quotes(char *cmd, char **arr)
{
    int     i;
    int     idx;
    int     capt;
    int     last;

    idx = 0;
    capt = 0;
    i = 0;
    while (cmd[i] && cmd[i] == ' ')
    	i++;
    last = i;
    while (cmd[i])
    {
        if (cmd[i] && (cmd[i] == '\'' || cmd[i] == '\"') && cmd[i + 1])
            capt = !capt;
        else if (cmd[i] && cmd[i] == ' ' && !capt)
        {
            arr[idx] = ft_substr(cmd, last, i - last);
            if (!arr[idx])
                return (-1); // Memory error
            idx++;
	    while (cmd[i + 1] && cmd[i + 1] == ' ')
	    	i++;
            last = i + 1;
        }
        i++;
    }
    if (i > last)
    {
        arr[idx] = ft_substr(cmd, last, i - last);
        if (!arr[idx])
            return (-1); // Memory error
    }
    return (0);
}

/* Free the split and return NULL in case of an error */
static char **free_split(char **arr)
{
    int idx;

    idx = 0;
    while (arr[idx])
        free(arr[idx++]);
    free(arr);
    return (NULL); // just to win lines from the norminette by returning the function (see below)
}

/* The main function that is splitting the command line and returning a 2D array */
char    **split_cmd_lst(char *cmd)
{
    int     splitc;
    char    **splitv;
    int     idx;

    if (!cmd)
        return (NULL); // In case cmd is invalid
    if (!quotes_even(cmd))
        return (NULL); // In case quotes are not closed, SHOULD STILL WORK
    splitc = count_splits(cmd);
    if (splitc <= 0)
        return (NULL); // In case cmd is empty
    splitv = malloc(sizeof(char *) * (splitc + 1));
    if (!splitv)
        return (NULL); // In case of memory error
    idx = 0;
    while (idx <= splitc)
        splitv[idx++] = NULL;
    if (split_with_quotes(cmd, splitv) == -1)
        return (free_split(splitv));
    return (splitv);
}

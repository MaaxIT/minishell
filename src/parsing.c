/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpeharpr <mpeharpr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/06/28 02:57:32 by mpeharpr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Properly free a command */
void    free_command(t_command *cmd)
{
    free(cmd->full);
    free(cmd);
}

/* Initialize the command with values we got */
static void init_command(t_command *new, char **splitted)
{
    int argc;

    argc = 0;
    while (splitted[argc] && *splitted[argc])
        argc++;
    if (argc > 0)
       argc--;
    new->argc = argc;
    new->full = splitted;
    new->bin = splitted[0];
    if (splitted[1])
        new->argv = &splitted[1];
    else
        new->argv = NULL;
}

/* Split the line for every space */
t_command   *parse_cmd(char *cmd)
{
    char        **splitted;
    t_command   *new;

    new = malloc(sizeof(t_command));
    if (!new)
        return (NULL);
    new->full = NULL;
    new->bin = NULL;
    new->argv = NULL;
    new->argc = -1;
    splitted = split_command(cmd);
    if (!splitted || !splitted[0])
    {
        free(new);
        if (splitted != NULL)
            free(splitted);
        return (NULL);
    }
    init_command(new, splitted);
    return (new);
}

/* Translate metacharacters from the command */
void    translate_metachars(t_command *cmd)
{
    // TODO: Check for special characters and so on
    (void)cmd;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maaxit <maaxit@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/27 19:01:52 by mpeharpr          #+#    #+#             */
/*   Updated: 2022/07/01 00:35:09 by maaxit           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Properly free a command */
int free_command(t_command *cmd)
{
    printf("argv: %p \nsplitv: %p \nfull: %p \norigin: %s \nsplitstr: %s \n", cmd->argv, cmd->splitv, cmd->full, cmd->origin, cmd->splitstr);
    if (cmd->argv)
        ft_free_2d_table(cmd->argv);
    if (cmd->splitv)
        ft_free_2d_table(cmd->splitv);
    if (cmd->splitstr)
        free(cmd->splitstr);
    if (cmd->full)
    	ft_free_2d_table(cmd->full);
    if (cmd->origin)
        free(cmd->origin);
    free(cmd);
    return (-1);
}

/* Initialize the command structure */
static int  init_command_struct(t_command *new, int argc, int splitc)
{
    new->origin = NULL;
    new->full = NULL;
    new->bin = NULL;
    new->fullstr = NULL;
    new->splitstr = NULL;
    new->argc = 0;
    new->splitc = 0;
    if (argc > 0)
    {
        new->argv = malloc(argc * sizeof(char *));
        if (!new->argv)
            return (free_command(new));
    }
    else
        new->argv = NULL;
    if (splitc > 0)
    {
        new->splitv = malloc(splitc * sizeof(char *));
        if (!new->splitv)
            return (free_command(new));
    }
    else
        new->splitv = NULL;
    return (0);
}

/* Initialize the command with values we got */
static void init_command(t_command *new, char **splitted, char *cmd)
{
    int argc;
    int splitc;
    int i;

    i = 1;
    argc = 0;
    splitc = 0;
    while (splitted[i] && *splitted[i])
    {
        if (splitted[i][0] == '-')
            argc++;
        else
            splitc++;
        i++;
    }

    if (init_command_struct(new, argc, splitc) == -1)
        return ;

    printf("%d %d\n", argc, splitc);

    i = 1;
    argc = 0;
    splitc = 0;
    while (splitted[i] && *splitted[i])
    {
        if (splitted[i][0] == '-')
            new->argv[argc++] = splitted[i];
        else
            new->splitv[splitc++] = splitted[i]; 
        i++;
    }
    
    new->origin = cmd;
    new->argc = argc;
    new->splitc = splitc;
    new->full = splitted;
    new->bin = splitted[0];
    new->fullstr = cmd;
    new->splitstr = ft_arrjoin(new->splitv, new->splitc, ' ');
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
    init_command(new, splitted, cmd);
    return (new);
}

/* Translate metacharacters from the command */
void    translate_metachars(t_command *cmd)
{
    // TODO: Check for special characters and so on
    (void)cmd;
}

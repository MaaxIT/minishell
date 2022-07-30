/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maxime <maxime@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/24 15:47:50 by maxime            #+#    #+#             */
/*   Updated: 2022/07/30 13:34:25 by maxime           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_structure(t_cmd_lst *cmd_t)
{
	cmd_t->original = NULL;
	cmd_t->binary = NULL;
	cmd_t->options_c = 0;
	cmd_t->options_v = NULL;
	cmd_t->input_c = 0;
	cmd_t->input_v = NULL;
	cmd_t->arg_c = 0;
	cmd_t->arg_v = NULL;
	cmd_t->parsing_v = NULL;
	cmd_t->input_path = NULL;
	cmd_t->output_path = NULL;
	cmd_t->output_type = 0;
	cmd_t->next = NULL;
}

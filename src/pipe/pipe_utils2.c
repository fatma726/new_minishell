/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/11/04 00:00:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	repeat_exec(char **args, char **envp, t_node *node, int pid)
{
	if (!pid)
		exec_child(args, envp, node);
	else
		run_parent_segment(args, envp, node);
	backup_restor(node);
}

void	maybe_handle_exit(char **args, char **envp, t_node *node)
{
	if (args && args[0] && !ft_strncmp(args[0], "exit", 5))
	{
		if (exit_will_terminate(args))
		{
			node->exit_flag = 1;
			cmd_exit(args, envp, node);
		}
	}
}

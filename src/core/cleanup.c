/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	cleanup_env(char **envp, t_node *node)
{
	if (envp)
		strarrfree(envp);
	if (node && node->pwd)
		free(node->pwd);
	if (node && node->path_fallback)
		free(node->path_fallback);
	if (node && node->ori_args)
	{
		strarrfree(node->ori_args);
		node->ori_args = NULL;
	}
	clear_history();
	restore_termios();
	set_exit_status(0);
	if (node)
	{
		node->pwd = NULL;
		node->path_fallback = NULL;
		node->ori_args = NULL;
	}
	envp = NULL;
}

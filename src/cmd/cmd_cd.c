/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/24 17:20:03 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	**cmd_cd(char **args, char **envp, t_node *node)
{
	int		offset;

    set_exit_status(EXIT_SUCCESS);
    /* In pipelines, allow execution when this is the rightmost segment
     * (node->pipe_flag is cleared by exec_parents for the last segment),
     * so that the pipeline exit reflects builtin status. */
	if (node->pipe_flag)
		return (envp);
	offset = 0;
	if (args[1] && !ft_strncmp(args[1], "--", 3))
		offset++;
	if (!validate_cd_args(args, offset))
		return (envp);
	if (args[1 + offset] && !args[1 + offset][0] && !args[2 + offset])
		return (envp);
	return (execute_cd(args, envp, node, offset));
}

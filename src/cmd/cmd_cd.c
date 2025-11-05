/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/24 17:20:03 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	**cmd_cd(char **args, char **envp, t_node *node)
{
    int		offset;

    set_exit_status_n(node, EXIT_SUCCESS);
    if (node->pipe_flag)
        return (envp);
    offset = 0;
    if (args[1] && !ft_strncmp(args[1], "--", 3))
        offset++;
    if (args[1 + offset] && args[1 + offset][0] == '-' &&
        ft_strncmp(args[1 + offset], "-", 2) && ft_strncmp(args[1 + offset], "--", 3))
    {
        set_exit_status_n(node, 2);
        return (envp);
    }
    if (args[1 + offset] && args[2 + offset])
    {
        set_exit_status_n(node, 1);
        ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO);
        return (envp);
    }
    if (!validate_cd_args(args, offset))
        return (envp);
    if (args[1 + offset] && !args[1 + offset][0] && !args[2 + offset])
        return (envp);
    return (execute_cd(args, envp, node, offset));
}

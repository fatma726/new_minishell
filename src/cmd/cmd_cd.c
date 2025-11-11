/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/08 12:51:26 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static int	compute_offset(char **args)
{
	int		offset;

	offset = 0;
	if (args[1] && !ft_strncmp(args[1], "--", 3))
		offset++;
	return (offset);
}

static int	invalid_option(char **args, int offset)
{
	if (args[1 + offset] && args[1 + offset][0] == '-'
		&& ft_strncmp(args[1 + offset], "-", 2)
		&& ft_strncmp(args[1 + offset], "--", 3))
		return (1);
	return (0);
}

static int	too_many_args(char **args, int offset)
{
	int	count;
	int	i;

	count = 0;
	i = 1 + offset;
	while (args[i])
	{
		if (args[i][0] != '\0')
			count++;
		i++;
	}
	return (count > 1);
}

char	**cmd_cd(char **args, char **envp, t_node *node)
{
	int		offset;

	set_exit_status_n(node, EXIT_SUCCESS);
	if (node->pipe_flag)
		return (envp);
	offset = compute_offset(args);
	if (invalid_option(args, offset))
		return (set_exit_status_n(node, 2), envp);
	if (too_many_args(args, offset))
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

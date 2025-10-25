/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:03 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	exec_error(char **args, char **envp, char **paths, t_node *node)
{
	char	bash_line[20];

	ft_strlcpy(bash_line, "minishell: ", 20);
	ft_putstr_fd(bash_line, STDERR_FILENO);
	if (ft_strchr(args[0], '/'))
		handle_absolute_path_error(args, envp, paths, node);
	else
		handle_relative_path_error(args, envp, paths, node);
}

void	checkdot(char **args, char **envp, t_node *node)
{
	char	*msg;
	char	bash_line[20];

	if (!ft_strncmp(args[0], ".", 2))
	{
		ft_strlcpy(bash_line, "minishell: ", 20);
		ft_putstr_fd(bash_line, STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		msg = ft_strdup(": filename argument required\n");
		ft_putstr_fd(msg, STDERR_FILENO);
		free(msg);
		set_exit_status(2);
		cleanup_child_and_exit(args, envp, node);
	}
	else if (!ft_strncmp(args[0], "..", 3) || !ft_strncmp(args[0], ".", 2))
	{
		ft_strlcpy(bash_line, "minishell: ", 20);
		ft_putstr_fd(bash_line, STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		msg = ft_strdup(": command not found\n");
		ft_putstr_fd(msg, STDERR_FILENO);
		free(msg);
		set_exit_status(127);
		cleanup_child_and_exit(args, envp, node);
	}
}

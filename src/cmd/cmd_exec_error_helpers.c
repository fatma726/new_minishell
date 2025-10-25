/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/24 19:10:23 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	handle_absolute_path_error(char **args, char **envp, char **paths,
		t_node *node)
{
	struct stat	st;

	if (stat(args[0], &st) == 0)
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		if (paths)
			strarrfree(paths);
		set_exit_status(126);
		cleanup_child_and_exit(args, envp, node);
	}
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (paths)
		strarrfree(paths);
	set_exit_status(127);
	cleanup_child_and_exit(args, envp, node);
}

void	handle_relative_path_error(char **args, char **envp, char **paths,
		t_node *node)
{
	char	*msg;

	ft_putstr_fd(args[0], STDERR_FILENO);
	msg = ft_strdup(": command not found\n");
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
	if (paths)
		strarrfree(paths);
	set_exit_status(127);
	cleanup_child_and_exit(args, envp, node);
}

static int	derive_abs_path_errno(const char *path, bool end, int *out_err)
{
	struct stat	st;

	(void)end;
	if (stat(path, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
		{
			*out_err = EISDIR;
			return (1);
		}
		if (access(path, X_OK) != 0)
		{
			*out_err = EACCES;
			return (1);
		}
	}
	else
	{
		*out_err = errno;
		return (1);
	}
	return (0);
}

static void	handle_directory_error(char **args, int err, int *status, bool end)
{
	if (err == ENOENT)
		*status = 127;
	else
		*status = 126;
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (err == EISDIR)
		ft_putstr_fd("is a directory", STDERR_FILENO);
	else
		ft_putstr_fd(strerror(err), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (end)
		exit(*status);
}

void	chkdir(char **args, char **envp, bool end, t_node *node)
{
	int		err;
	int		status;

	(void)envp;
	if (derive_abs_path_errno(args[0], end, &err))
	{
		handle_directory_error(args, err, &status, end);
		if (end)
		{
			set_exit_status(status);
			cleanup_child_and_exit(args, envp, node);
		}
		else
			set_exit_status(status);
	}
}

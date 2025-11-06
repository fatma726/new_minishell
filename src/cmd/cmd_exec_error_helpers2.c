/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error_helpers2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

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
	ft_putstr_fd("bash: line 1: ", STDERR_FILENO);
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (err == EISDIR)
		ft_putstr_fd("Is a directory", STDERR_FILENO);
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
			set_exit_status_n(node, status);
			cleanup_child_and_exit(args, envp, node);
		}
		else
			set_exit_status_n(node, status);
	}
}

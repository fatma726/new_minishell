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

static int	derive_abs_path_errno(const char *path, bool end, int *out_err)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		*out_err = EISDIR;
		return (0);
	}
	if (access(path, R_OK | X_OK) != 0)
	{
		if (errno == EACCES || errno == ENOTDIR)
			*out_err = EACCES;
		else if (errno == ENOENT)
			*out_err = ENOENT;
		else
			*out_err = errno;
		return (0);
	}
	if (!end)
		return (1);
	*out_err = 0;
	return (0);
}

static void	handle_directory_error(char **args, int err, int *status, bool end)
{
	char	bash_line[20];
	char	is_dir[20];

	errno = err;
	ft_strlcpy(bash_line, "minishell: ", 20);
	ft_putstr_fd(bash_line, STDERR_FILENO);
	if (err == EISDIR)
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_strlcpy(is_dir, ": is a directory", 20);
		ft_putstr_fd(is_dir, STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	*status = 126 + (end && err != EISDIR && err != EACCES);
}

void	chkdir(char **args, char **envp, bool end, t_node *node)
{
	int			err;
	int			status;

	status = 0;
	if (derive_abs_path_errno(args[0], end, &err))
		return ;
	if (err == EISDIR || access(args[0], R_OK | X_OK) != 0)
		handle_directory_error(args, err, &status, end);
	set_exit_status(status);
	cleanup_child_and_exit(args, envp, node);
}

void	exec_error(char **args, char **envp, char **paths, t_node *node)
{
	char	bash_line[20];
	char	*msg;

	ft_strlcpy(bash_line, "minishell: ", 20);
	ft_putstr_fd(bash_line, STDERR_FILENO);
	if (ft_strchr(args[0], '/'))
	{
		errno = ENOENT;
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		ft_putstr_fd(strerror(errno), STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd(args[0], STDERR_FILENO);
		msg = ft_strdup(": command not found\n");
		ft_putstr_fd(msg, STDERR_FILENO);
		free(msg);
	}
	if (paths)
		strarrfree(paths);
	set_exit_status(127);
	cleanup_child_and_exit(args, envp, node);
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

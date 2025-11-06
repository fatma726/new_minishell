/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*reconstruct_cmd(char **args, char **ori_args);
void	print_escaped_cmd(const char *cmd);
void	chkdir(char **args, char **envp, bool end, t_node *node);

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
		set_exit_status_n(node, 126);
		cleanup_child_and_exit(args, envp, node);
	}
	ft_putstr_fd(args[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (paths)
		strarrfree(paths);
	set_exit_status_n(node, 127);
	cleanup_child_and_exit(args, envp, node);
}

static void	print_cmd_error(char *cmd)
{
	if (cmd[0] == '$' && cmd[1] == '\'')
		ft_putstr_fd(cmd, STDERR_FILENO);
	else
		print_escaped_cmd(cmd);
}

static void	handle_empty_cmd_error(t_node *node)
{
	if (node->ori_args && node->ori_args[0])
	{
		if (node->ori_args[0][0] == '$' && node->ori_args[0][1] == '\'')
			ft_putstr_fd(node->ori_args[0], STDERR_FILENO);
		else
			print_escaped_cmd(node->ori_args[0]);
	}
}

static void	handle_nonempty_cmd_error(char **args, t_node *node)
{
	char	*cmd;

	cmd = reconstruct_cmd(args, node->ori_args);
	if (cmd)
	{
		print_cmd_error(cmd);
		free(cmd);
	}
	else
		print_escaped_cmd(args[0]);
}

void	handle_relative_path_error(char **args, char **envp, char **paths,
		t_node *node)
{
	char	*msg;

	if (!args || !args[0] || !args[0][0])
		handle_empty_cmd_error(node);
	else
		handle_nonempty_cmd_error(args, node);
	msg = ft_strdup(": command not found\n");
	ft_putstr_fd(msg, STDERR_FILENO);
	free(msg);
	if (paths)
		strarrfree(paths);
	set_exit_status_n(node, 127);
	cleanup_child_and_exit(args, envp, node);
}

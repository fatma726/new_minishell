/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:28:27 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* helpers moved to cmd_exit_helpers.c */

void	cleanup_child_and_exit(char **args, char **envp, t_node *node)
{
	if (node->pwd)
		free(node->pwd);
	if (node->path_fallback)
		free(node->path_fallback);
	if (node->ori_args)
	{
		strarrfree(node->ori_args);
		node->ori_args = NULL;
	}
	if (args)
		strarrfree(args);
	if (envp)
		strarrfree(envp);
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	exit(get_exit_status_n(node));
}

void	cleanup_and_exit(char **args, char **envp, t_node *node)
{
	if (node->pwd)
		free(node->pwd);
	if (node->path_fallback)
		free(node->path_fallback);
	if (node->ori_args)
	{
		strarrfree(node->ori_args);
		node->ori_args = NULL;
	}
	if (args)
		strarrfree(args);
	if (envp)
		strarrfree(envp);
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	restore_termios();
	maybe_write_exit_file(node);
	exit(get_exit_status_n(node));
}

bool	handle_if_no_exit_flag(char **args, t_node *node)
{
	if (!node->exit_flag)
	{
		if (strarrlen(args) > 1)
			(void)handle_exit_with_args(args, node);
		else
			set_exit_status_n(node, EXIT_SUCCESS);
		return (true);
	}
	return (false);
}

bool	process_exit_args(char **args, t_node *node, bool *should_exit)
{
	if (strarrlen(args) > 1)
	{
		if (!ft_isalldigit(args[1]))
		{
			handle_numeric_error(args[1], node);
			return (false);
		}
		*should_exit = handle_exit_with_args(args, node);
	}
	else
	{
		set_exit_status_n(node, EXIT_SUCCESS);
		*should_exit = true;
	}
	return (true);
}

void	cmd_exit(char **args, char **envp, t_node *node)
{
	bool	should_exit;

	if (handle_if_no_exit_flag(args, node))
		return ;
	should_exit = true;
	if (!process_exit_args(args, node, &should_exit))
		return ;
	if (should_exit && !node->argmode)
		handle_exit_message();
	if (should_exit)
	{
		cleanup_and_exit(args, envp, node);
		return ;
	}
}

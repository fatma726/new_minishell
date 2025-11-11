/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:08:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* helpers moved to cmd_exit_helpers.c */

static void	exit_cleanup_core(char **args, char **envp, t_node *node)
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
	if (node->full_ori_args)
	{
		strarrfree(node->full_ori_args);
		node->full_ori_args = NULL;
	}
	if (args)
		strarrfree(args);
	if (envp)
		strarrfree(envp);
}

static void	exit_cleanup_parser_temps(char **args, t_node *node)
{
	if (node->parser_tokens)
	{
		if (args == NULL || node->parser_tokens != args)
			strarrfree(node->parser_tokens);
		node->parser_tokens = NULL;
	}
	if (node->parser_tmp_str)
	{
		free(node->parser_tmp_str);
		node->parser_tmp_str = NULL;
	}
}

void	cleanup_child_and_exit(char **args, char **envp, t_node *node)
{
	cleanup_child_resources(args, envp, node);
	exit(get_exit_status_n(node));
}

void	cleanup_and_exit(char **args, char **envp, t_node *node)
{
	exit_cleanup_core(args, envp, node);
	exit_cleanup_parser_temps(args, node);
	if (node->redir_fd >= 0)
	{
		close(node->redir_fd);
		unlink(".temp");
		node->redir_fd = -1;
	}
	if (node->backup_stdout > 2)
		close(node->backup_stdout);
	node->backup_stdout = -1;
	if (node->backup_stdin > 2)
		close(node->backup_stdin);
	node->backup_stdin = -1;
	restore_termios();
	maybe_write_exit_file(node);
	exit(get_exit_status_n(node));
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

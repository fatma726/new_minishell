/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_helpers_more.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	cleanup_node_paths_child_only(t_node *node)
{
	if (node->path)
	{
		free(node->path);
		node->path = NULL;
	}
	if (node->redir_fd >= 0)
	{
		close(node->redir_fd);
		node->redir_fd = -1;
		unlink(".temp");
	}
}

static void	child_post_cleanup(t_node *node)
{
	if (node->stdin_backup > 2)
		close(node->stdin_backup);
	if (node->stdout_backup > 2)
		close(node->stdout_backup);
	if (node->stderr_backup > 2)
		close(node->stderr_backup);
	if (node->backup_stdin > 2)
		close(node->backup_stdin);
	if (node->backup_stdout > 2)
		close(node->backup_stdout);
	node->stdin_backup = -1;
	node->stdout_backup = -1;
	node->stderr_backup = -1;
	node->backup_stdin = -1;
	node->backup_stdout = -1;
	if (node->parser_tokens)
	{
		strarrfree(node->parser_tokens);
		node->parser_tokens = NULL;
	}
	if (node->parser_tmp_str)
	{
		free(node->parser_tmp_str);
		node->parser_tmp_str = NULL;
	}
}

void	cleanup_child_resources(char **args, char **envp, t_node *node)
{
	cleanup_node_paths_child_only(node);
	child_post_cleanup(node);
	(void)args;
	(void)envp;
}

void	handle_exit_message(void)
{
	char	exit_msg[5];

	if (isatty(STDIN_FILENO))
	{
		ft_strlcpy(exit_msg, "exit", 5);
		ft_putendl_fd(exit_msg, STDOUT_FILENO);
	}
}

bool	exit_will_terminate(char **args)
{
	size_t	argc;

	if (!args)
		return (false);
	argc = strarrlen(args);
	if (argc <= 1)
		return (true);
	if (argc == 2 && ft_isalldigit(args[1]))
		return (true);
	return (false);
}

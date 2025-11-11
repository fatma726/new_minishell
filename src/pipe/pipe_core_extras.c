/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_core_extras.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	apply_heredoc_stdin(t_node *node)
{
	if (node->redir_fd >= 0)
	{
		lseek(node->redir_fd, 0, SEEK_SET);
		dup2(node->redir_fd, STDIN_FILENO);
		close(node->redir_fd);
		node->redir_fd = -1;
	}
}

void	setup_child_fds(t_node *node)
{
	close(node->fds[0]);
	if (!node->right_flag)
		dup2(node->fds[1], STDOUT_FILENO);
	close(node->fds[1]);
}

void	reset_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
}

void	child_fast_exit(t_node *node)
{
	if (node->path)
	{
		free(node->path);
		node->path = NULL;
	}
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	exit(get_exit_status_n(node));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 20:44:38 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	write_heredoc_line(bool expand_vars, char *line,
					char **envp, t_node *node)
{
	char	*expanded_line;

	if (node->redir_fd < 0)
		return ;
	if (expand_vars)
	{
		expanded_line = expand_envvar(line, envp, node);
		ft_putendl_fd(expanded_line, node->redir_fd);
		free(expanded_line);
	}
	else
	{
		ft_putendl_fd(line, node->redir_fd);
		free(line);
	}
}

int	finalize_loop_result(int lines_read, bool got_sigint, struct s_hdctx *ctx)
{
	if (lines_read > 0)
		ctx->node->heredoc_swallowed_input = true;
	if (got_sigint)
	{
		if (ctx->node->redir_fd >= 0)
		{
			close(ctx->node->redir_fd);
			ctx->node->redir_fd = -1;
		}
		unlink(".temp");
		return (2);
	}
	return (0);
}

void	print_heredoc_syntax_error(t_node *node)
{
	ft_putstr_fd("bash: -c: line 1: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd("newline", STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	ft_putstr_fd("bash: -c: line 1: `newline'\n", STDERR_FILENO);
	set_exit_status_n(node, 2);
	node->redir_stop = 1;
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/20 18:30:00 by fatmtahmdab      ###   ########.fr       */
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
	return (2);
}

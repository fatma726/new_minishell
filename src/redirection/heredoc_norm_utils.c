/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_norm_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/20 19:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_delimiter(const char *line, const char *delim)
{
	if (ft_strncmp(line, delim, ft_strlen(delim)) == 0
		&& ft_strlen(line) == ft_strlen(delim))
		return (true);
	return (false);
}

int	handle_exit_if_needed(struct s_hdctx *ctx, char *line)
{
	if (!ctx->has_command && ft_strlen(line) == 4
		&& ft_strncmp(line, "exit", 5) == 0)
	{
		free(line);
		unlink(".temp");
		cleanup_and_exit(ctx->args, ctx->envp, ctx->node);
	}
	return (0);
}

int	process_heredoc_iteration(struct s_hdctx *ctx)
{
	char	*line;

	if (check_heredoc_signal())
		return (3);
	line = get_heredoc_line();
	if (!line)
	{
		if (check_heredoc_signal())
			return (3);
		return (2);
	}
	handle_exit_if_needed(ctx, line);
	if (is_delimiter(line, ctx->delimiter))
	{
		free(line);
		return (0);
	}
	write_heredoc_line(ctx->expand_vars, line, ctx->envp, ctx->node);
	return (1);
}

int	run_heredoc_loop(struct s_hdctx *ctx, int *lines_read, bool *got_sigint)
{
    int	result;

    *lines_read = 0;
    *got_sigint = false;
    while (1)
    {
        /* Ignore SIGQUIT inside heredoc: clear and break */
        if (get_signal_number() == SIGQUIT)
        {
            clear_signal_number();
            break ;
        }
        result = process_heredoc_iteration(ctx);
        if (result == 0)
            return (0);
        if (result == 1)
        {
			(*lines_read)++;
			continue ;
		}
		if (result == 3)
		{
			*got_sigint = true;
			break ;
		}
		if (result == 2)
			break ;
	}
	return (-1);
}

int	process_heredoc_loop(struct s_hdctx *ctx)
{
	int		lines_read;
	bool	got_sigint;
	int		ret;

	ret = run_heredoc_loop(ctx, &lines_read, &got_sigint);
	if (ret == 0)
		return (0);
	return (finalize_loop_result(lines_read, got_sigint, ctx));
}

/* process_heredoc_input removed to keep function count under norm limit */

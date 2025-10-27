/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/22 18:35:31 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/*
** Check if the current command contains any non-redirection token.
** This helps detect whether a heredoc belongs to a standalone redirection
** or to a full command (e.g., `cat << stop` vs `<< stop` alone).
*/

static bool	command_has_non_redir_token(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (isdlr(args[i]) || isdrr(args[i]) || islr(args[i])
			|| islrr(args[i]) || isrr(args[i]) || istlr(args[i]))
		{
			if (args[i + 1])
				i += 2;
			else
				i += 1;
			continue ;
		}
		if (args[i][0])
			return (true);
		i++;
	}
	return (false);
}

/*
** Main heredoc loop entry point.
** Builds a context struct, sets signal handling for heredoc,
** runs the heredoc reading loop, and restores normal signals.
*/

int	heredoc_loop(char **args, char **envp, int *i, t_node *node)
{
	char			*clean_delimiter;
	int				unterminated;
	struct s_hdctx	ctx;
    bool                is_standalone;
    int                 saved_rl_catch = 0;
    int                 rc = 0;

    clean_delimiter = clean_delimiter_if_marked(node->ori_args[*i + 1]);
	ctx.args = args;
	ctx.has_command = command_has_non_redir_token(args);
    is_standalone = !ctx.has_command;
    ctx.delimiter = clean_delimiter;
    ctx.expand_vars = should_expand_vars(node->ori_args[*i + 1]);
	ctx.envp = envp;
	ctx.node = node;
	/* Ensure readline does not override our SIGINT handler during heredoc */
    #if defined(RL_READLINE_VERSION)
    saved_rl_catch = rl_catch_signals;
    rl_catch_signals = 0;
    #endif
	set_heredoc_signal();
	unterminated = process_heredoc_loop(&ctx);
    if (get_signal_number() == SIGINT)
    {
        /* Heredoc Ctrl-C -> 130 */
        set_exit_status(130);
        node->redir_stop = 1;
        clear_signal_number();
        rc = 130;
        goto cleanup_heredoc;
    }
	if (unterminated)
	{
		node->heredoc_unterminated = true;
		/* Standalone heredoc terminated by EOF without delimiter -> 1 */
		if (is_standalone)
			set_exit_status(1);
	}
	rc = unterminated;

cleanup_heredoc:
	set_signal(); /* restore prompt-mode handlers */
    #if defined(RL_READLINE_VERSION)
    rl_catch_signals = saved_rl_catch;
    #endif
	return (rc);
}

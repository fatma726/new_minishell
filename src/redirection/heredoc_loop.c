/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/20 17:15:44 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* using external prototypes from headers */

static bool	command_has_non_redir_token(char **args)
{
	int		i;

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

/* forward declarations not needed here */

int	heredoc_loop(char **args, char **envp, int *i, t_node *node)
{
	char			*clean_delimiter;
	int				unterminated;
	struct s_hdctx	ctx;

	clean_delimiter = clean_delimiter_if_marked(args[*i + 1]);
	ctx.args = args;
	ctx.has_command = command_has_non_redir_token(args);
	ctx.delimiter = clean_delimiter;
	ctx.expand_vars = should_expand_vars(clean_delimiter);
	ctx.envp = envp;
	ctx.node = node;
	set_heredoc_signal();
	unterminated = process_heredoc_loop(&ctx);
	set_signal();
	if (unterminated)
		node->heredoc_unterminated = true;
	return (unterminated);
}

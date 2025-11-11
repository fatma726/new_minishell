/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 13:31:37 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* Split a token like echo"text" into two tokens: echo and "text" */
bool	is_echo_with_joined_quote(char *first)
{
	if (!first)
		return (false);
	return (ft_strncmp(first, "echo", 4) == 0
		&& (first[4] == '\'' || first[4] == '"'));
}

char	**rebuild_echo_with_rest(char **args, char *rest)
{
	char	**newv;
	int		n;

	newv = malloc(sizeof(char *) * (strarrlen(args) + 2));
	if (!newv)
		return (args);
	newv[0] = ft_strdup("echo");
	newv[1] = rest;
	n = 1;
	while (args[++n - 1])
		newv[n] = ft_strdup(args[n - 1]);
	newv[n] = NULL;
	strarrfree(args);
	return (newv);
}

char	**split_joined_quote_after_cmd(char **args)
{
	char	*first;
	char	*rest;

	if (!args || !args[0])
		return (args);
	first = args[0];
	if (is_echo_with_joined_quote(first))
	{
		rest = ft_strdup(first + 4);
		if (!rest)
			return (args);
		return (rebuild_echo_with_rest(args, rest));
	}
	return (args);
}

/* segment helpers moved to parser_segments.c to satisfy norm */

static char	**process_semicolon_loop(char **args, char **envp,
								t_node *node, char **old_ori_args)
{
	int			semi_idx;
	int			start;
	t_segctx	ctx;

	start = 0;
	while (1)
	{
		semi_idx = next_semicolon_index(args, start);
		if (semi_idx == -1)
		{
			ctx.start = start;
			ctx.end = strarrlen(args);
			ctx.old_ori_args = old_ori_args;
			envp = process_last_segment_ctx(args, envp, node, &ctx);
			break ;
		}
		ctx.start = start;
		ctx.end = semi_idx;
		ctx.old_ori_args = old_ori_args;
		envp = process_semicolon_segment_ctx(args, envp, node, &ctx);
		start = semi_idx + 1;
	}
	return (envp);
}

char	**process_quotes_and_exec(char **args, char **envp, t_node *node)
{
	char	**old_ori_args;

	args = rm_quotes(args, node);
	if (!args)
		return (strarrfree(envp), exit(EXIT_FAILURE), envp);
	old_ori_args = node->ori_args;
	envp = process_semicolon_loop(args, envp, node, old_ori_args);
	if (args)
		strarrfree(args);
	if (old_ori_args)
	{
		strarrfree(old_ori_args);
		node->ori_args = NULL;
	}
	if (node->full_ori_args && node->full_ori_args != old_ori_args)
		strarrfree(node->full_ori_args);
	node->full_ori_args = NULL;
	if (node->parser_tokens && node->parser_tokens != args)
		strarrfree(node->parser_tokens);
	node->parser_tokens = NULL;
	if (node->parser_tmp_str)
		free(node->parser_tmp_str);
	node->parser_tmp_str = NULL;
	return (envp);
}
/* moved some helpers to parser_helpers2.c to satisfy Norminette */

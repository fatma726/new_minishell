/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/11/02 00:10:00 by fatmtahmdab      ###   ########.fr       */
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

char	**process_quotes_and_exec(char **args, char **envp, t_node *node)
{
	args = rm_quotes(args, node);
	if (!args)
	{
		clear_history();
		strarrfree(envp);
		exit(EXIT_FAILURE);
	}
	envp = execute(args, envp, node);
	strarrfree(args);
	if (node->ori_args)
		strarrfree(node->ori_args);
	return (envp);
}
/* moved some helpers to parser_helpers2.c to satisfy Norminette */

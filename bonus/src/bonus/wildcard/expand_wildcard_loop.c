/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_loop.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static void	process_wildcard_in_arg(char **args, char **newargs, int *i,
		t_node *node)
{
	i[0] = -1;
	while (args[i[2]][++i[0]])
	{
		i[3] = quote_check(args[i[2]], i[0], node);
		if (!i[3] && (args[i[2]][i[0]] == '*' || args[i[2]][i[0]] == '?'))
		{
			wildcard_handler(args, newargs, i, node);
			return ;
		}
	}
	newargs[i[5]++] = ft_strdup(args[i[2]]);
}

void	expand_wildcard_loop(char **args, char **newargs, char **envp,
		t_node *node)
{
	int											i[6];
	bool										skip_wildcard;

	i[2] = -1;
	i[5] = 0;
	while (args[++i[2]])
	{
		skip_wildcard = false;
		if (i[2] > 0 && (isrr(args[i[2] - 1]) || isdrr(args[i[2] - 1])
				|| islr(args[i[2] - 1]) || islrr(args[i[2] - 1])
				|| isdlr(args[i[2] - 1]) || istlr(args[i[2] - 1])))
		{
			newargs[i[5]++] = ft_strdup(args[i[2]]);
			skip_wildcard = true;
		}
		if (!skip_wildcard)
		{
			tilde_handler(args, i, envp);
			process_wildcard_in_arg(args, newargs, i, node);
		}
	}
	newargs[i[5]] = NULL;
}

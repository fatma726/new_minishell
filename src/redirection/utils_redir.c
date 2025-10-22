/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:13 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static bool	check_pipe_pattern(char **args, int i)
{
	if (i > 0 && args[i - 1] && ft_strchr(args[i - 1], '|'))
	{
		if (args[i + 1] && args[i + 1][0] && args[i + 2])
			return (true);
	}
	return (false);
}

/* helper removed; lone redir tokens handled directly in redir_syntax_check */

int	redir_syntax_check(char **args)
{
	int		i;
	bool	is_redir;

	i = -1;
	while (args[++i])
	{
		is_redir = isdlr(args[i]) || isdrr(args[i]) || islr(args[i])
			|| islrr(args[i]) || isrr(args[i]) || istlr(args[i]);
		if (is_redir)
		{
			if (args[i + 1] && (is_ampersand(args[i + 1]) || isda(args[i + 1])))
				return (0);
			if ((!args[i + 1] || !args[i + 1][0])
				&& (isrr(args[i]) || isdrr(args[i]) || islr(args[i])))
				return (0);
			if (check_pipe_pattern(args, i))
				continue ;
		}
	}
	return (1);
}

int	redir_excute(char **args, char **envp, t_node *node, int flag)
{
	(void)flag;
	if (!node->redir_flag)
		return (0);
	return (exec_redir(args, envp, node));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_checks.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

const char	*check_leading_operators(char **args)
{
	if (!args || !args[0])
		return (NULL);
	if (args[1] == NULL && (islr(args[0]) || isdrr(args[0]) || isdlr(args[0])
			|| isrr(args[0])))
		return ("newline");
	if (isdlr(args[0]) && args[1])
		return (NULL);
	if (isp(args[0]))
		return (args[0]);
	return (NULL);
}

const char	*check_consecutive_ops(char **args, int i)
{
	if ((isp(args[i]) || islr(args[i]) || isrr(args[i]) || isdlr(args[i]))
		&& (isp(args[i + 1]) || islr(args[i + 1]) || isrr(args[i + 1])
			|| isdlr(args[i + 1])))
		return (args[i + 1]);
	return (NULL);
}

const char	*check_triple_redir_split(char **args, int i)
{
	if (ft_strchr(args[i], '>') && ft_strchr(args[i], '>'))
		return (">");
	return (NULL);
}

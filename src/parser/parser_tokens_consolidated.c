/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_consolidated.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

/* Basic token checks */
bool	isp(char *str)
{
	return (str && !ft_strncmp(str, "|", 2));
}

bool	isdp(char *str)
{
	return (str && !ft_strncmp(str, "||", 3));
}

bool	isda(char *str)
{
	return (str && !ft_strncmp(str, "&&", 3));
}

bool	istlr(char *str)
{
	return (str && !ft_strncmp(str, "<<<", 4));
}

bool	islrr(char *str)
{
	return (str && !ft_strncmp(str, "<>", 3));
}

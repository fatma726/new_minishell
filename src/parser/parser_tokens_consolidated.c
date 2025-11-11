/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_consolidated.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:33:59 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* Basic token checks */
bool	isp(char *str)
{
	return (str && !ft_strncmp(str, "|", 2));
}

bool	islor(char *str)
{
	return (str && !ft_strncmp(str, "||", 3));
}

bool	islrr(char *str)
{
	return (str && !ft_strncmp(str, "<>", 3));
}

bool	istr(char *str)
{
	return (str && !ft_strncmp(str, ">>>", 3));
}

bool	is_semicolon(char *str)
{
	return (str && !ft_strncmp(str, ";", 2));
}

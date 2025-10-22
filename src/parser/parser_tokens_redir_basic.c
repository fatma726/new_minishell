/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_redir_basic.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

bool	islr(char *str)
{
	return (str && !ft_strncmp(str, "<", 2));
}

bool	isrr(char *str)
{
	return (str && ((!ft_strncmp(str, ">", 2) && ft_strlen(str) == 1)
			|| !ft_strncmp(str, "2>", 3) || !ft_strncmp(str, ">|", 3)
			|| !ft_strncmp(str, "2>|", 4)));
}

bool	istr(char *str)
{
	return (str && !ft_strncmp(str, ">>>", 3));
}

bool	isdrr(char *str)
{
	return (str && !ft_strncmp(str, ">>", 3));
}

bool	isdlr(char *str)
{
	return (str && !ft_strncmp(str, "<<", 3));
}

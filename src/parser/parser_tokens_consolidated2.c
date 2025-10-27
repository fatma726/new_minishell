/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens_consolidated2.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

/* Group token checks */
bool	is_ampersand(char *str)
{
	return (str && !ft_strncmp(str, "&", 2));
}

bool	is_open_paren(char *str)
{
	return (str && !ft_strncmp(str, "(", 2));
}

bool	is_close_paren(char *str)
{
	return (str && !ft_strncmp(str, ")", 2));
}

bool	is_open_brace(char *str)
{
	return (str && !ft_strncmp(str, "{", 2));
}

bool	is_close_brace(char *str)
{
	return (str && !ft_strncmp(str, "}", 2));
}

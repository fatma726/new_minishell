/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_logical_helpers.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

int	need_space_before_logical(char *s, int i)
{
	return (i > 0 && s[i - 1] != ' ' && s[i - 1] != '\t');
}

int	need_space_after_logical(char *s, int i)
{
	return (s[i + 1] && s[i + 1] != ' ' && s[i + 1] != '\t');
}

void	handle_oror_operator(char *str, char *result, int *i, int *j)
{
	if (need_space_before_logical(str, *i))
		result[(*j)++] = ' ';
	result[(*j)++] = '|';
	result[(*j)++] = '|';
	if (need_space_after_logical(str, *i + 1))
		result[(*j)++] = ' ';
	*i += 2;
}

void	handle_andand_operator(char *str, char *result, int *i, int *j)
{
	if (need_space_before_logical(str, *i))
		result[(*j)++] = ' ';
	result[(*j)++] = '&';
	result[(*j)++] = '&';
	if (need_space_after_logical(str, *i + 1))
		result[(*j)++] = ' ';
	*i += 2;
}

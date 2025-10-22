/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_redir_helpers.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	need_space_before_redir(char *s, int i)
{
	return (i > 0 && s[i - 1] != ' ' && s[i - 1] != '\t');
}

static int	need_space_after_redir(char *s, int i)
{
	return (s[i + 1] && s[i + 1] != '&' && s[i + 1] != '|'
		&& s[i + 1] != ' ' && s[i + 1] != '\t');
}

int	copy_double_left(char *str, char *result, int i, int *j)
{
	if (need_space_before_redir(str, i))
		result[(*j)++] = ' ';
	result[(*j)++] = '<';
	result[(*j)++] = '<';
	if (need_space_after_redir(str, i + 1))
		result[(*j)++] = ' ';
	return (i + 2);
}

int	copy_double_right(char *str, char *result, int i, int *j)
{
	if (need_space_before_redir(str, i))
		result[(*j)++] = ' ';
	result[(*j)++] = '>';
	result[(*j)++] = '>';
	if (need_space_after_redir(str, i + 1))
		result[(*j)++] = ' ';
	return (i + 2);
}

int	copy_single_redir(char *str, char *result, int i, int *j)
{
	if (need_space_before_redir(str, i))
		result[(*j)++] = ' ';
	result[(*j)++] = str[i];
	if (need_space_after_redir(str, i))
		result[(*j)++] = ' ';
	return (i + 1);
}

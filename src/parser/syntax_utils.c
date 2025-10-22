/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

bool	check_leading_operators_syntax(char **a)
{
	if (a[0] && (isp(a[0]) || istr(a[0])
			|| isdp(a[0]) || isda(a[0]) || is_ampersand(a[0])))
		return (false);
	if (a[0] && (islr(a[0]) || isrr(a[0]) || isdrr(a[0]) || isdlr(a[0]))
		&& !a[1])
		return (false);
	return (true);
}

bool	check_pipe_redir_combination(char **a, int i)
{
	if (ft_strchr(a[i], '|')
		&& (ft_strchr(a[i + 1], '>') || ft_strchr(a[i + 1], '<'))
		&& a[i + 2] && a[i + 2][0])
		return (true);
	return (false);
}

bool	check_trailing_operators_syntax(char **a)
{
	int		i;

	i = 0;
	while (a[i] && a[i + 1])
		i++;
	if (i >= 0 && a[i] && (isp(a[i]) || islr(a[i]) || isrr(a[i])
			|| isdlr(a[i]) || isdrr(a[i]) || isdp(a[i]) || isda(a[i])
			|| is_ampersand(a[i]))
		&& !ft_strchr(a[i], '\'') && !ft_strchr(a[i], '"'))
		return (false);
	return (true);
}

static bool	is_bare_operator(char *s)
{
	if (!s)
		return (false);
	if (ft_strchr(s, '\'') || ft_strchr(s, '"'))
		return (false);
	return (
		isp(s) || islr(s) || isrr(s) || isdlr(s)
		|| isdrr(s) || isdp(s) || isda(s) || is_ampersand(s)
		|| is_open_paren(s) || is_close_paren(s)
		|| is_open_brace(s) || is_close_brace(s)
	);
}

bool	check_consecutive_operators_syntax(char **a)
{
	int	i;

	i = 0;
	while (a[i] && a[i + 1])
	{
		if (is_bare_operator(a[i]) && is_bare_operator(a[i + 1]))
		{
			if (check_pipe_redir_combination(a, i))
				i++;
			else
				return (false);
		}
		i++;
	}
	return (true);
}

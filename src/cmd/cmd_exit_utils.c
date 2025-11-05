/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/11/04 00:00:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_nested_wrapper(char const *s)
{
	return (has_nested_quote(s));
}

static bool	contains_other_quote(const char *s, size_t start, size_t end,
			char other)
{
	size_t	i;

	i = start;
	while (i < end)
	{
		if (s[i] == other)
			return (true);
		i++;
	}
	return (false);
}

bool	has_nested_quote(char const *s)
{
	size_t	len;
	char	q;
	char	other;

	if (!s)
		return (false);
	len = ft_strlen(s);
	if (len < 2)
		return (false);
	q = s[0];
	if (q != '\'' && q != '"')
		return (false);
	if (s[len - 1] != q)
		return (false);
	if (q == '\'')
		other = '"';
	else
		other = '\'';
	return (contains_other_quote(s, 1, len - 1, other));
}

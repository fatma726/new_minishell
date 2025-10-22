/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_consolidated.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "bonus.h"
#include "../../include/bonus.h"

bool	is_operator_pair(char *s, size_t i, t_node *n)
{
	if (quote_check(s, (int)i, n))
		return (false);
	return ((s[i] == '&' && s[i + 1] == '&')
		|| (s[i] == '|' && s[i + 1] == '|'));
}

static bool	check_redir_after_operator(char *s, size_t j, t_node *n)
{
	size_t	k;

	k = j + 1;
	if (s[k] == '>')
		k++;
	while (s[k] == ' ' || s[k] == '\t')
		k++;
	if (s[k] == '&' && s[k + 1] == '&')
	{
		handle_double_ampersand_error_public(n);
		return (false);
	}
	if (s[k] == '|' && s[k + 1] == '|')
	{
		handle_double_pipe_error_public(n);
		return (false);
	}
	if (!check_redir_token(s[k], n))
		return (false);
	return (true);
}

bool	check_redirection_syntax(char *s, t_node *n)
{
	size_t	j;

	j = 0;
	while (s[j])
	{
		if (s[j] == '>' && !quote_check(s, (int)j, n))
		{
			if (!check_redir_after_operator(s, j, n))
				return (false);
		}
		j++;
	}
	return (true);
}

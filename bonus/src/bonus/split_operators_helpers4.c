/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_helpers4.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bonus.h"
#include "../../include/bonus.h"

char	**handle_syntax_errors(char *s, int i, char **envp, t_node *n)
{
	if (handle_invalid_start_and_report(s, (size_t)i, n))
		return (free(s), envp);
	if (has_triple_ops(s, (size_t)i))
	{
		syntax_err_pair(s, (size_t)i, n, 0);
		return (free(s), envp);
	}
	if (ft_strchr(s, '>') && !check_redirection_syntax(s, n))
		return (free(s), envp);
	return (NULL);
}

bool	has_logical_operators_outside_parens(char *s, size_t l,
	size_t r, t_node *n)
{
	size_t	i;
	int		depth;

	(void)l;
	(void)r;
	depth = 0;
	i = 0;
	while (s[i])
	{
		if (!quote_check(s, (int)i, n))
		{
			if (s[i] == '(')
				depth++;
			else if (s[i] == ')')
				depth--;
			else if (depth == 0 && is_operator_pair(s, i, n))
				return (true);
		}
		i++;
	}
	return (false);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_helpers5.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 07:39:58 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bonus.h"

void	advance_to(char *s, size_t *i, t_node *n, char op)
{
	int	depth;

	depth = 0;
	while (s[*i])
	{
		if (!quote_check(s, (int)*i, n))
		{
			if (s[*i] == '(')
				depth++;
			else if (s[*i] == ')')
				depth -= (depth > 0);
			else if (depth == 0)
			{
				if (op == '|' && s[*i] == '&' && s[*i + 1] == '&')
					break ;
				if (op == '&' && s[*i] == '|' && s[*i + 1] == '|')
					break ;
			}
		}
		(*i)++;
	}
}

static void	scan_for_operators(char *s, t_node *n, int *pos)
{
	int	i;
	int	depth;

	i = 0;
	depth = 0;
	while (s[i])
	{
		if (!quote_check(s, i, n))
		{
			if (s[i] == '(')
				depth++;
			else if (s[i] == ')')
				depth -= (depth > 0);
			else if (depth == 0)
			{
				if (s[i] == '|' && s[i + 1] == '|')
					pos[0] = i;
				else if (s[i] == '&' && s[i + 1] == '&')
					pos[1] = i;
				else if (s[i] == '&' && s[i + 1] != '&')
					pos[2] = i;
			}
		}
		i++;
	}
}

int	find_top_level_op(char *s, t_node *n)
{
	int	pos[3];

	pos[0] = -1;
	pos[1] = -1;
	pos[2] = -1;
	scan_for_operators(s, n, pos);
	if (pos[0] >= 0)
		return (pos[0]);
	if (pos[1] >= 0)
		return (pos[1]);
	if (pos[2] >= 0)
		return (pos[2]);
	return (-1);
}

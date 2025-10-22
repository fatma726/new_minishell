/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/21 10:23:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	pqh_count_step(char *src, int i, t_quote_state *st, int *l)
{
	char	c;

	c = src[i];
	handle_quote_state(c, &st->in_single, &st->in_double);
	if ((c == '\'' && !st->in_double) || (c == '"' && !st->in_single))
		return (i);
	if (c == '\\')
	{
		if (!st->in_single && !st->in_double)
			return (i);
		if (st->in_double && src[i + 1] == '"')
		{
			(*l)++;
			return (i + 1);
		}
	}
	(*l)++;
	return (i);
}

int	pqh_count_valid_chars(char *str, t_node *node)
{
	int				i;
	int				l;
	bool			in_single;
	bool			in_double;
	t_quote_state	st;

	(void)node;
	i = -1;
	l = 0;
	in_single = false;
	in_double = false;
	while (str[++i])
	{
		st.in_single = in_single;
		st.in_double = in_double;
		st.j = 0;
		i = pqh_count_step(str, i, &st, &l);
		in_single = st.in_single;
		in_double = st.in_double;
	}
	return (l);
}

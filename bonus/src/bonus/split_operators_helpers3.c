/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_helpers3.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bonus.h"

static size_t	skip_left_ws(char *s)
{
	size_t	l;

	l = 0;
	while (s[l] == ' ' || s[l] == '\t')
		l++;
	return (l);
}

static size_t	trim_right_ws(char *s, size_t l)
{
	size_t	r;

	r = ft_strlen(s);
	while (r > l && (s[r - 1] == ' ' || s[r - 1] == '\t'))
		r--;
	return (r);
}

static void	set_inner_bounds(size_t l, size_t i,
		size_t *inner_start, size_t *inner_len)
{
	*inner_start = l + 1;
	if (i > (l + 1))
		*inner_len = i - (l + 1);
	else
		*inner_len = 0;
}

static int	scan_wrapped(t_wrap_scan *c)
{
	size_t	i;
	int		depth;

	depth = 0;
	i = c->l;
	while (i < c->r)
	{
		if (!quote_check(c->s, (int)i, c->n))
		{
			if (c->s[i] == '(')
				depth++;
			else if (c->s[i] == ')')
			{
				depth--;
				if (depth == 0 && i == c->r - 1)
				{
					set_inner_bounds(c->l, i, c->inner_start, c->inner_len);
					return (1);
				}
			}
		}
		i++;
	}
	return (0);
}

int	is_wrapped_group(char *s, t_node *n,
	size_t *inner_start, size_t *inner_len)
{
	size_t		l;
	size_t		r;
	t_wrap_scan	ctx;

	l = skip_left_ws(s);
	r = trim_right_ws(s, l);
	if (s[l] != '(')
		return (0);
	ctx.s = s;
	ctx.n = n;
	ctx.l = l;
	ctx.r = r;
	ctx.inner_start = inner_start;
	ctx.inner_len = inner_len;
	if (!scan_wrapped(&ctx))
		return (0);
	if (has_logical_operators_outside_parens(s, 0, 0, n))
		return (0);
	return (1);
}

/* split_operators_tail moved to its own compilation unit */
/* handle_syntax_errors moved to split_operators_helpers4.c */

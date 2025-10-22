/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	handle_quote_state(char c, bool *in_single, bool *in_double)
{
	if (c == '\'' && !*in_double)
		*in_single = !*in_single;
	else if (c == '"' && !*in_single)
		*in_double = !*in_double;
}

static char	*alloc_new_string(char *str, t_node *node)
{
	char	*newstr;
	int		l;

	if (str && str[0] == (char)WILDMARK)
		return (return_marked_unchanged(str));
	l = pqh_count_valid_chars(str, node);
	newstr = malloc(((size_t)l + 1) * sizeof(char));
	if (!newstr)
		return (free(str), exit(EXIT_FAILURE), NULL);
	return (newstr);
}

static int	process_char_with_lookahead(
				char *src, int i, char *dst, t_quote_state *st)
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
			dst[st->j++] = '"';
			return (i + 1);
		}
	}
	dst[st->j++] = c;
	return (i);
}

char	*process_quotes(char *str, t_node *node)
{
	char			*newstr;
	int				i;
	t_quote_state	state;

	newstr = alloc_new_string(str, node);
	if (str && str[0] == (char)WILDMARK)
		return (newstr);
	i = -1;
	state.j = 0;
	state.in_single = false;
	state.in_double = false;
	while (str[++i])
		i = process_char_with_lookahead(str, i, newstr, &state);
	return (newstr[state.j] = '\0', free(str), newstr);
}

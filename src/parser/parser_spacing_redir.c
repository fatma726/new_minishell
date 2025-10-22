/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_redir.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static bool	handle_redir_token(char *str, t_node *node, char *result,
		struct s_redir_copy_state *st)
{
	if (!quote_check(str, st->i, node))
	{
		if (str[st->i] == '<' && str[st->i + 1] == '<')
		{
			st->i = copy_double_left(str, result, st->i, &st->j);
			return (true);
		}
		if (str[st->i] == '>' && str[st->i + 1] == '>')
		{
			st->i = copy_double_right(str, result, st->i, &st->j);
			return (true);
		}
		if ((str[st->i] == '<' || str[st->i] == '>')
			&& str[st->i + 1] != str[st->i])
		{
			st->i = copy_single_redir(str, result, st->i, &st->j);
			return (true);
		}
	}
	return (false);
}

static int	copy_with_redirections(char *str, t_node *node, char *result)
{
	struct s_redir_copy_state	st;

	st.i = 0;
	st.j = 0;
	while (str[st.i])
	{
		if (handle_redir_token(str, node, result, &st))
			continue ;
		result[st.j++] = str[st.i];
		st.i++;
	}
	return (st.j);
}

char	*add_spaces_around_redirections(char *str, t_node *node)
{
	char	*result;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc((len * 2 + 1) * sizeof(char));
	if (!result)
	{
		free(str);
		return (NULL);
	}
	len = (size_t)copy_with_redirections(str, node, result);
	result[len] = '\0';
	free(str);
	return (result);
}

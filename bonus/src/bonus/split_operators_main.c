/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_main.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/bonus.h"

static char	**handle_no_operator(char *s, char **envp, t_node *n)
{
	size_t	inner_start;
	size_t	inner_len;
	char	*inner;

	if (is_wrapped_group(s, n, &inner_start, &inner_len))
	{
		inner = ft_substr(s, (unsigned int)(inner_start), inner_len);
		free(s);
		if (!inner)
			return (envp);
		return (split_operators(inner, envp, n));
	}
	return (parser(s, envp, n));
}

static char	**split_operators_after_index(
	char *s, int i, char **envp, t_node *n)
{
	char	*left;
	char	**result;

	if (i < 0)
		return (handle_no_operator(s, envp, n));
	result = handle_syntax_errors(s, i, envp, n);
	if (result)
		return (result);
	left = ft_substr(s, 0, (size_t)i);
	if (!left)
		return (free(s), envp);
	envp = split_operators(left, envp, n);
	if (n->syntax_flag)
		return (free(s), envp);
	return (split_operators_tail(s, (size_t)i, envp, n));
}

char	**split_operators(char *s, char **envp, t_node *n)
{
	int		i;
	size_t	inner_start;
	size_t	inner_len;
	char	*inner;

	if (n->syntax_flag)
	{
		free(s);
		return (envp);
	}
	if (has_mixed_op_error(s, n))
	{
		free(s);
		return (envp);
	}
	if (is_wrapped_group(s, n, &inner_start, &inner_len))
	{
		inner = ft_substr(s, (unsigned int)(inner_start), inner_len);
		free(s);
		if (!inner)
			return (envp);
		return (split_operators(inner, envp, n));
	}
	i = find_top_level_op(s, n);
	return (split_operators_after_index(s, i, envp, n));
}

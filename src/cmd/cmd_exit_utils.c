/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:14:44 by fatmtahmdab      ###   ########.fr       */
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

bool	handle_if_no_exit_flag(char **args, t_node *node)
{
	if (!node->exit_flag)
	{
		if (strarrlen(args) > 1)
			(void)handle_exit_with_args(args, node);
		else
			set_exit_status_n(node, EXIT_SUCCESS);
		return (true);
	}
	return (false);
}

bool	process_exit_args(char **args, t_node *node, bool *should_exit)
{
	if (strarrlen(args) > 1)
	{
		if (!ft_isalldigit(args[1]))
		{
			handle_numeric_error(args[1], node);
			return (false);
		}
		*should_exit = handle_exit_with_args(args, node);
	}
	else
	{
		set_exit_status_n(node, EXIT_SUCCESS);
		*should_exit = true;
	}
	return (true);
}

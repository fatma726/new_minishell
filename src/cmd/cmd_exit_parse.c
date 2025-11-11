/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:40:14 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"
#include <limits.h>

/* moved parsing helpers to cmd_exit_parse_helpers.c */
bool		parse_strict_ll(const char *s, long long *out);
bool		handle_too_many_args(t_node *node);

bool	handle_exit_with_args(char **args, t_node *node)
{
	long long	exit_num;

	if (node && node->ori_args && node->ori_args[1]
		&& is_nested_wrapper(node->ori_args[1]))
	{
		handle_numeric_error(args[1], node);
		return (true);
	}
	if (!ft_isalldigit(args[1]))
	{
		handle_numeric_error(args[1], node);
		return (true);
	}
	if (strarrlen(args) > 2)
		return (handle_too_many_args(node));
	if (!parse_strict_ll(args[1], &exit_num))
	{
		handle_numeric_error(args[1], node);
		return (true);
	}
	set_exit_status_n(node, (int)((unsigned char)exit_num));
	return (true);
}

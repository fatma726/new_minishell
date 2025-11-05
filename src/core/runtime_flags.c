/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime_flags.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/11/04 00:00:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_interactive_n(const t_node *node)
{
	return (node && node->rt && node->rt->interactive);
}

void	set_interactive_n(t_node *node, bool value)
{
	if (node && node->rt)
		node->rt->interactive = value;
}

bool	get_nontext_input_n(const t_node *node)
{
	return (node && node->rt && node->rt->nontext_input);
}

void	set_nontext_input_n(t_node *node, bool v)
{
	if (node && node->rt)
		node->rt->nontext_input = v;
}

void	clear_nontext_input_n(t_node *node)
{
	if (node && node->rt)
		node->rt->nontext_input = false;
}

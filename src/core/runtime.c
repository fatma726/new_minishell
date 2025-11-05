/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runtime.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

int	get_exit_status_n(const t_node *node)
{
	if (!node || !node->rt)
		return (0);
	return (node->rt->exit_status);
}

void	set_exit_status_n(t_node *node, int status)
{
	if (!node || !node->rt)
		return ;
	node->rt->exit_status = status;
}

/* moved flag helpers to runtime_flags.c */

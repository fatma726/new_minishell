/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/24 17:30:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static volatile sig_atomic_t	g_signal_number = 0;

int	get_signal_number(void)
{
	return ((int)g_signal_number);
}

void	clear_signal_number(void)
{
	g_signal_number = 0;
}

void	set_signal_number(int sig)
{
	g_signal_number = (sig_atomic_t)sig;
}

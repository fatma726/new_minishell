/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:28:27 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static volatile sig_atomic_t	g_signal_number = 0;
static volatile sig_atomic_t	g_reading_input = 0;

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

void	set_reading_input(bool value)
{
	g_reading_input = (sig_atomic_t)value;
}

bool	is_reading_input(void)
{
	return ((bool)g_reading_input);
}

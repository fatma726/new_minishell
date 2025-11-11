/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 18:06:32 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static volatile t_signal_state	g_signal_state = {0, 0};

int	get_signal_number(void)
{
	return ((int)g_signal_state.signal_number);
}

void	clear_signal_number(void)
{
	g_signal_state.signal_number = 0;
}

void	set_signal_number(int sig)
{
	g_signal_state.signal_number = (sig_atomic_t)sig;
}

void	set_reading_input(bool value)
{
	g_signal_state.reading_input = (sig_atomic_t)value;
}

bool	is_reading_input(void)
{
	return ((bool)g_signal_state.reading_input);
}

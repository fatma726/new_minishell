/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/24 17:30:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	get_signal_number(void)
{
	return ((int)ms_slots()->signal_number);
}

void	clear_signal_number(void)
{
	ms_slots()->signal_number = 0;
}

void	set_signal_number(int sig)
{
	ms_slots()->signal_number = (sig_atomic_t)sig;
}

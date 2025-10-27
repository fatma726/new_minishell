/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_state2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	set_interactive_mode(bool value)
{
	ms_slots()->interactive = value;
}

int	_ms_exit_status(int op, int value)
{
	if (op)
		ms_slots()->exit_status = value;
	return (ms_slots()->exit_status);
}

int	get_exit_status(void)
{
	return (_ms_exit_status(0, 0));
}

void	set_exit_status(int status)
{
	(void)_ms_exit_status(1, status);
}

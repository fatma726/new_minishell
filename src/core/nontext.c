/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nontext.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	g_nontext_input = 0;

bool	get_nontext_input(void)
{
	return (g_nontext_input != 0);
}

void	set_nontext_input(bool v)
{
	g_nontext_input = (v != 0);
}

void	clear_nontext_input(void)
{
	g_nontext_input = 0;
}

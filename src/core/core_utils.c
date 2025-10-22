/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

#include <termios.h>

bool	is_blank(const char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\r'
			&& s[i] != '\v' && s[i] != '\f')
			return (false);
		i++;
	}
	return (true);
}

/* From numeric_utils.c */
long long	ft_atoll(const char *str)
{
	size_t		idx;
	int			sign;
	long long	result;

	if (!str)
		return (0);
	idx = 0;
	result = 0;
	sign = 1;
	while (1)
	{
		if ((str[idx] > '\b' && str[idx] <= '\r') || str[idx] == ' ')
			idx++;
		else
			break ;
	}
	if (str[idx] == '-')
		sign = -1;
	idx += (str[idx] == '+' || str[idx] == '-');
	while (str[idx] >= '0' && str[idx] <= '9')
		result = result * 10 + (str[idx++] - '0');
	result *= sign;
	return (result);
}

/* From termios_utils.c */
void	set_termios(void)
{
	struct termios	termios;
	struct termios	original;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &original) == 0)
	{
		termios = original;
		termios.c_lflag &= ~(tcflag_t)ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &termios);
	}
}

void	restore_termios(void)
{
	struct termios	original;

	if (!isatty(STDIN_FILENO))
		return ;
	if (tcgetattr(STDIN_FILENO, &original) == 0)
	{
		original.c_lflag |= (tcflag_t)ECHOCTL;
		tcsetattr(STDIN_FILENO, TCSANOW, &original);
	}
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/09/30 23:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char		nbc;
	int			divisor;
	int			start;
	long int	nb2;

	nb2 = n;
	start = 0;
	divisor = 1000000000;
	if (nb2 < 0)
	{
		write (fd, "-", 1);
		nb2 *= -1;
	}
	while (divisor >= 1 && n != 0)
	{
		nbc = nb2 / divisor % 10 + 48;
		divisor /= 10;
		if (nbc != '0')
			start = 1;
		if (start == 1)
			write (fd, &nbc, 1);
	}
	if (n == 0)
		write (fd, "0", 1);
}

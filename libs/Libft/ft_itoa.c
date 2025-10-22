/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/09/30 23:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

static int	getsize(long n)
{
	int	s;

	s = 1;
	if (!n)
		s++;
	if (n < 0)
		s++;
	while (n && s++)
		n /= 10;
	return (s);
}

char	*ft_itoa(int n)
{
	char		*returned;
	int			i[2];
	long int	n2;

	i[0] = 0;
	i[1] = 1000000000;
	n2 = n;
	returned = malloc(getsize(n) * sizeof(char));
	if (!returned)
		return (returned);
	if (n2 < 0)
	{
		returned[i[0]++] = '-';
		n2 *= -1;
	}
	while (i[1])
	{
		if (n2 / i[1])
			returned[i[0]++] = n2 / i[1] % 10 + '0';
		i[1] /= 10;
	}
	if (!n)
		returned[i[0]++] = '0';
	returned[i[0]++] = '\0';
	return (returned);
}

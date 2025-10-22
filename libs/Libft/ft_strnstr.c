/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/09/30 23:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	i2;

	i = 0;
	i2 = 0;
	if (!haystack || !needle || !needle[0])
		return (haystack);
	while (haystack[i] && i < len)
	{
		while (haystack[i] == needle[i2] && i < len)
		{
			if (!needle[i2 + 1])
			{
				return (&haystack[i - i2]);
			}
			i++;
			i2++;
		}
		i -= i2;
		i++;
		i2 = 0;
	}
	return (0);
}

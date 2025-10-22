/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/09/30 23:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i[2];
	size_t	dest_size;

	i[0] = ft_strlen(dst);
	i[1] = 0;
	dest_size = i[0];
	if (dstsize < dest_size || !dst || !src)
		return (dstsize + ft_strlen(src));
	while (src[i[1]] && i[0] < dstsize)
	{
		dst[i[0]] = src[i[1]];
		i[0]++;
		i[1]++;
	}
	if (dest_size == dstsize)
		dst[dest_size] = '\0';
	else if (i[0] < dstsize)
		dst[i[0]++] = '\0';
	else if (i[0] >= dstsize)
		dst[dstsize - 1] = '\0';
	return (dest_size + ft_strlen(src));
}

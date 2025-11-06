/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

ssize_t	find_nl(const char *buf, ssize_t n)
{
	ssize_t	k;

	k = 0;
	while (k < n)
	{
		if (buf[k] == '\n')
			return (k);
		k++;
	}
	return (-1);
}

int	ensure_cap2(char **res, size_t *cap, size_t *len, size_t need)
{
	char	*tmp;

	if (*cap >= need + 1)
		return (1);
	*cap = (need + 1) * 2;
	tmp = malloc(*cap);
	if (!tmp)
		return (0);
	if (*res)
	{
		ft_memcpy(tmp, *res, *len);
		free(*res);
	}
	*res = tmp;
	return (1);
}

int	append_buf2(char **res, size_t *len, const char *buf, ssize_t n)
{
	if (!*res)
		return (0);
	ft_memcpy(*res + *len, buf, (size_t)n);
	*len += (size_t)n;
	return (1);
}

char	*finalize_line(char *res, size_t len)
{
	if (len == 0)
	{
		free(res);
		return (NULL);
	}
	res[len] = '\0';
	return (res);
}

int	process_buffer(char **res, size_t *len, char *buf, ssize_t n)
{
	ssize_t	pos;

	pos = find_nl(buf, n);
	if (pos >= 0)
	{
		ft_memcpy(*res + *len, buf, (size_t)pos);
		*len += (size_t)pos;
		return (1);
	}
	append_buf2(res, len, buf, n);
	return (0);
}

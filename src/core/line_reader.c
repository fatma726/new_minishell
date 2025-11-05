/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* Inlined two size_t counters to avoid struct/typedef in .c per norm */

static ssize_t	find_nl(const char *buf, ssize_t n)
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

static int	ensure_cap2(char **res, size_t *cap, size_t *len, size_t need)
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

/*
** read_line_fd
** - Reads one line from fd, stripping trailing '\n'.
** - Returns NULL on EOF with no data or on allocation/read error.
*/
static int	append_buf2(char **res, size_t *len, const char *buf, ssize_t n)
{
	if (!*res)
		return (0);
	ft_memcpy(*res + *len, buf, (size_t)n);
	*len += (size_t)n;
	return (1);
}

char	*read_line_fd(int fd)
{
	char		buf[1024];
	char		*res;
	size_t		cap, len;
	ssize_t		n, pos;

	res = NULL;
	cap = 0;
	len = 0;
	while (1)
	{
		n = read(fd, buf, sizeof(buf));
		if (n <= 0)
			break ;
		if (!ensure_cap2(&res, &cap, &len, len + (size_t)n))
		{
			free(res);
			return (NULL);
		}
		pos = find_nl(buf, n);
		if (pos >= 0)
		{
			ft_memcpy(res + len, buf, (size_t)pos);
			len += (size_t)pos;
			res[len] = '\0';
			return (res);
		}
		append_buf2(&res, &len, buf, n);
	}
	if (len == 0)
	{
		free(res);
		return (NULL);
	}
	res[len] = '\0';
	return (res);
}

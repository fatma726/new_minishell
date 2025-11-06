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

ssize_t	find_nl(const char *buf, ssize_t n);
int		ensure_cap2(char **res, size_t *cap, size_t *len, size_t need);
int		append_buf2(char **res, size_t *len, const char *buf, ssize_t n);
char	*finalize_line(char *res, size_t len);
int		process_buffer(char **res, size_t *len, char *buf, ssize_t n);

static char	*read_chunks(int fd)
{
	char		buf[1024];
	char		*res;
	size_t		cap;
	size_t		len;
	ssize_t		n;

	res = NULL;
	cap = 0;
	len = 0;
	while (1)
	{
		n = read(fd, buf, sizeof(buf));
		if (n <= 0)
			break ;
		if (!ensure_cap2(&res, &cap, &len, len + (size_t)n))
			return (free(res), NULL);
		if (process_buffer(&res, &len, buf, n))
			return (finalize_line(res, len));
	}
	return (finalize_line(res, len));
}

char	*read_line_fd(int fd)
{
	return (read_chunks(fd));
}

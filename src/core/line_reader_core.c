/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_reader_core.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

ssize_t	find_nl(const char *buf, ssize_t n);
int		ensure_cap2(char **res, size_t *cap, size_t *len, size_t need);
int		append_buf2(char **res, size_t *len, const char *buf, ssize_t n);
char	*finalize_line(char *res, size_t len);

static struct s_lrctx
{
	char	buf[4096];
	size_t	len;
}	g_remainder_ctx;

static void	shift_remainder(size_t pos)
{
	size_t	j;
	size_t	rem_len;

	rem_len = g_remainder_ctx.len - (size_t)pos - 1;
	j = 0;
	while (j < rem_len)
	{
		g_remainder_ctx.buf[j] = g_remainder_ctx.buf[(size_t)pos + 1 + j];
		j++;
	}
	g_remainder_ctx.len = rem_len;
}

int	consume_remainder(char **res, size_t *cap, size_t *len)
{
	ssize_t	pos;

	if (g_remainder_ctx.len == 0)
		return (0);
	pos = find_nl(g_remainder_ctx.buf, (ssize_t)g_remainder_ctx.len);
	if (pos >= 0)
	{
		if (!ensure_cap2(res, cap, len, (size_t)pos))
			return (-1);
		ft_memcpy(*res, g_remainder_ctx.buf, (size_t)pos);
		*len = (size_t)pos;
		shift_remainder((size_t)pos);
		return (1);
	}
	if (!ensure_cap2(res, cap, len, g_remainder_ctx.len))
		return (-1);
	ft_memcpy(*res, g_remainder_ctx.buf, g_remainder_ctx.len);
	*len = g_remainder_ctx.len;
	g_remainder_ctx.len = 0;
	return (0);
}

int	handle_buf_with_nl(char *buf, ssize_t n, struct s_lr *ctx)
{
	ssize_t	pos;

	pos = find_nl(buf, n);
	if (pos < 0)
		return (0);
	if (!ensure_cap2(&ctx->res, &ctx->cap, &ctx->len, ctx->len + (size_t)pos))
		return (-1);
	ft_memcpy(ctx->res + ctx->len, buf, (size_t)pos);
	ctx->len += (size_t)pos;
	if (pos + 1 < n)
	{
		ft_memcpy(g_remainder_ctx.buf, buf + pos + 1,
			(size_t)(n - pos - 1));
		g_remainder_ctx.len = (size_t)(n - pos - 1);
	}
	return (1);
}

static int	read_loop_internal(int fd, struct s_lr *ctx)
{
	char		buf[1024];
	ssize_t		n;
	int			st;

	while (1)
	{
		n = read(fd, buf, sizeof(buf));
		if (n <= 0)
			break ;
		st = handle_buf_with_nl(buf, n, ctx);
		if (st < 0)
			return (-1);
		if (st > 0)
			return (1);
		if (!ensure_cap2(&ctx->res, &ctx->cap, &ctx->len, ctx->len + (size_t)n))
			return (-1);
		append_buf2(&ctx->res, &ctx->len, buf, n);
	}
	return (0);
}

char	*read_chunks(int fd)
{
	struct s_lr	ctx;
	int			st;

	ctx.res = NULL;
	ctx.cap = 0;
	ctx.len = 0;
	st = consume_remainder(&ctx.res, &ctx.cap, &ctx.len);
	if (st < 0)
		return (NULL);
	if (st > 0)
		return (finalize_line(ctx.res, ctx.len));
	st = read_loop_internal(fd, &ctx);
	if (st < 0)
		return (free(ctx.res), NULL);
	if (st > 0)
		return (finalize_line(ctx.res, ctx.len));
	return (finalize_line(ctx.res, ctx.len));
}

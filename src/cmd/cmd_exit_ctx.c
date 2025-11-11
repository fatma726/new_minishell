/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_ctx.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:07:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	cleanup_child_ctx(t_childctx *ctx)
{
	if (!ctx)
		return ;
	if (ctx->parsed_args)
	{
		strarrfree(ctx->parsed_args);
		ctx->parsed_args = NULL;
	}
	if (ctx->quoted_args)
	{
		strarrfree(ctx->quoted_args);
		ctx->quoted_args = NULL;
	}
	if (ctx->paths)
	{
		strarrfree(ctx->paths);
		ctx->paths = NULL;
	}
}

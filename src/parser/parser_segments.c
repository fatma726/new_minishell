/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_segments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static char	**extract_segment(char **args, int start, int end)
{
	char	**segment;
	int		i;
	int		len;

	len = end - start;
	segment = malloc((len + 1) * sizeof(char *));
	if (!segment)
		return (NULL);
	i = 0;
	while (i < len)
	{
		segment[i] = ft_strdup(args[start + i]);
		if (!segment[i])
			return (strarrfree(segment), NULL);
		i++;
	}
	segment[len] = NULL;
	return (segment);
}

static char	**expand_dlr_q_only(char **segment, t_node *node)
{
	int		i;
	char	*expanded;

	node->only_dollar_q = true;
	i = 0;
	while (segment && segment[i])
	{
		if (ft_strchr(segment[i], '$')
			&& ft_strnstr(segment[i], "$?", ft_strlen(segment[i])))
		{
			expanded = expand_envvar(ft_strdup(segment[i]), NULL, node);
			if (expanded)
			{
				free(segment[i]);
				segment[i] = expanded;
			}
		}
		i++;
	}
	node->only_dollar_q = false;
	return (segment);
}

int	next_semicolon_index(char **args, int start)
{
	int	i;

	i = start;
	while (args[i])
	{
		if (is_semicolon(args[i]))
			return (i);
		i++;
	}
	return (-1);
}

char	**process_last_segment_ctx(char **args, char **envp,
								t_node *node, t_segctx *ctx)
{
	char	**segment;
	char	**ori_segment;

	segment = extract_segment(args, ctx->start, strarrlen(args));
	if (!segment)
		return (envp);
	ori_segment = extract_segment(ctx->old_ori_args, ctx->start,
			strarrlen(ctx->old_ori_args));
	node->ori_args = ori_segment;
	node->semicolon_sequence = false;
	segment = expand_dlr_q_only(segment, node);
	envp = execute(segment, envp, node);
	strarrfree(segment);
	if (ori_segment)
		strarrfree(ori_segment);
	return (envp);
}

char	**process_semicolon_segment_ctx(
	char **args, char **envp, t_node *node, t_segctx *ctx)
{
	char	**segment;
	char	**ori_segment;

	segment = extract_segment(args, ctx->start, ctx->end);
	if (!segment)
		return (envp);
	ori_segment = extract_segment(ctx->old_ori_args, ctx->start, ctx->end);
	node->ori_args = ori_segment;
	node->semicolon_sequence = true;
	segment = expand_dlr_q_only(segment, node);
	envp = execute(segment, envp, node);
	strarrfree(segment);
	if (ori_segment)
		strarrfree(ori_segment);
	return (envp);
}

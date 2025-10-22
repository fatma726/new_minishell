/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes_core.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	*process_quotes(char *str, t_node *node);

static void	check_pipe_in_arg(char *arg, int arg_idx, t_node *node)
{
	int	j;

	j = 0;
	while (arg[j] && arg[j + 1])
	{
		if (arg[j] == '\'' && arg[j + 1] == '|' && arg[j + 2] == '\'')
			node->quota_pipe_idx_arr[node->quota_pipe_cnt++] = arg_idx;
		else if (arg[j] == '"' && arg[j + 1] == '|' && arg[j + 2] == '"')
			node->quota_pipe_idx_arr[node->quota_pipe_cnt++] = arg_idx;
		else if (arg[j] == '\\' && arg[j + 1] == '|')
			node->quota_pipe_idx_arr[node->quota_pipe_cnt++] = arg_idx;
		j++;
	}
}

static void	quote_pipe_check(char **args, t_node *node)
{
	int	i;

	i = 0;
	node->quota_idx_j = 0;
	while (args[i])
	{
		check_pipe_in_arg(args[i], i, node);
		i++;
	}
}

char	**rm_quotes(char **args, t_node *node)
{
	int	i;

	i = -1;
	quote_pipe_check(args, node);
	if (node->escape_skip)
	{
		strip_wildmarks_inplace(args);
		return (args);
	}
	while (args[++i])
		args[i] = process_quotes(args[i], node);
	return (args);
}

char	**rm_quotes_wildcards(char **args, t_node *node)
{
	int	i;

	if (node->escape_skip)
	{
		strip_wildmarks_inplace(args);
		return (args);
	}
	i = -1;
	while (args[++i])
		args[i] = process_quotes(args[i], node);
	return (args);
}

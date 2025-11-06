/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers4.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static const char	*check_counts(int pipe_count, int amp_count)
{
	if (pipe_count >= 3)
		return ("|");
	if (amp_count >= 3)
		return ("&");
	return (NULL);
}

static void	update_counts(char c, int *pipe_count, int *amp_count)
{
	if (c == '|')
	{
		(*pipe_count)++;
		*amp_count = 0;
	}
	else if (c == '&')
	{
		(*amp_count)++;
		*pipe_count = 0;
	}
	else
	{
		*pipe_count = 0;
		*amp_count = 0;
	}
}

static const char	*check_redirect_seq(char **args, int i, int *j)
{
	int		gt;
	int		lt;

	gt = 0;
	lt = 0;
	while (args[i][*j] && (args[i][*j] == '>' || args[i][*j] == '<'))
	{
		if (args[i][*j] == '>')
		{
			gt++;
			lt = 0;
		}
		else
		{
			lt++;
			gt = 0;
		}
		if (gt >= 3)
			return (">>");
		if (lt >= 3)
			return ("<<");
		(*j)++;
	}
	return (NULL);
}

const char	*check_invalid_operator_sequences(char **args, int i)
{
	int			j;
	int			pipe_count;
	int			amp_count;
	const char	*result;

	if (!args[i] || ft_strlen(args[i]) < 3)
		return (NULL);
	j = 0;
	pipe_count = 0;
	amp_count = 0;
	while (args[i][j])
	{
		result = check_redirect_seq(args, i, &j);
		if (result)
			return (result);
		update_counts(args[i][j], &pipe_count, &amp_count);
		result = check_counts(pipe_count, amp_count);
		if (result)
			return (result);
		if (args[i][j])
			j++;
	}
	return (NULL);
}

const char	*check_final_token(char **args, int i)
{
	if (i > 0 && args[i])
	{
		if (isrr(args[i]) || isdrr(args[i])
			|| islr(args[i]) || isdlr(args[i]))
			return ("newline");
		if (isp(args[i]))
			return ("newline");
	}
	return ("newline");
}

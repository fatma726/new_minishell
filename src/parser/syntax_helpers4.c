/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers4.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdab      ###   ########.fr       */
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

const char	*check_invalid_operator_sequences(char **args, int i)
{
	int		pipe_count;
	int		amp_count;
	int		j;
	char	c;

	if (!args[i] || ft_strlen(args[i]) < 3)
		return (NULL);
	pipe_count = 0;
	amp_count = 0;
	j = 0;
	while (args[i][j])
	{
		c = args[i][j];
		update_counts(c, &pipe_count, &amp_count);
		if (check_counts(pipe_count, amp_count))
			return (check_counts(pipe_count, amp_count));
		j++;
	}
	return (NULL);
}

const char	*check_final_token(char **args, int i)
{
	if (i > 0 && args[i])
	{
		if (is_open_paren(args[i]) || is_close_paren(args[i])
			|| is_open_brace(args[i]) || is_close_brace(args[i]))
			return (args[i]);
		if (isdp(args[i]))
			return ("||");
		if (isda(args[i]))
			return ("&&");
		if (isrr(args[i]) || isdrr(args[i]) || islr(args[i]) || isdlr(args[i]))
			return ("newline");
		if (isp(args[i]) || is_ampersand(args[i]))
			return ("newline");
	}
	return ("newline");
}

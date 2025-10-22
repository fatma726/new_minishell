/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static const char	*check_pipe_redir_pattern(char **args, int i)
{
	if (ft_strchr(args[i], '|') && (ft_strchr(args[i + 1], '>')
			|| ft_strchr(args[i + 1], '<')))
	{
		if (args[i + 2] && (ft_strchr(args[i + 2], '>')
				|| ft_strchr(args[i + 2], '<')))
		{
			if (ft_strchr(args[i + 2], '>') && ft_strchr(args[i + 2], '>'))
				return (">>");
			if (ft_strchr(args[i + 2], '>'))
				return (">");
			if (ft_strchr(args[i + 1], '<'))
				return ("<");
		}
		else if (args[i + 2] && ft_strchr(args[i + 2], '|'))
			return ("|");
		else
			return ("newline");
	}
	return (NULL);
}

static const char	*check_loop_operators(char **args, int i)
{
	const char	*result;

	result = check_pipe_redir_pattern(args, i);
	if (result)
		return (result);
	result = check_consecutive_ops(args, i);
	if (result)
		return (result);
	result = check_triple_redir_split(args, i);
	if (result)
		return (result);
	return (NULL);
}

static const char	*evaluate_position(char **args, int i)
{
	const char	*result;

	result = check_loop_operators(args, i);
	if (result)
		return (result);
	result = check_invalid_operator_sequences(args, i);
	if (result)
		return (result);
	if ((isrr(args[i]) || isdrr(args[i]) || islr(args[i]) || isdlr(args[i]))
		&& args[i + 1])
	{
		if (is_ampersand(args[i + 1]))
			return ("&");
		if (ft_strncmp(args[i + 1], "&&", 3) == 0)
			return ("&&");
		if (ft_strncmp(args[i + 1], "||", 3) == 0)
			return ("||");
	}
	return (NULL);
}

const char	*get_error_token(char **args)
{
	int			i;
	const char	*result;

	result = check_leading_operators(args);
	if (result)
		return (result);
	i = 0;
	while (args[i] && args[i + 1])
	{
		result = evaluate_position(args, i);
		if (result)
			return (result);
		i++;
	}
	return (check_final_token(args, i));
}

void	report_syntax_error(char c, t_node *node)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putchar_fd(c, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	set_exit_status(2);
	node->syntax_flag = true;
}

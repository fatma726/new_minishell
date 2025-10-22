/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static bool	check_brace_syntax(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "{", 2) == 0)
		{
			i++;
			while (args[i])
			{
				if (ft_strncmp(args[i], "}", 2) == 0)
					return (true);
				i++;
			}
			return (false);
		}
		if (ft_strncmp(args[i], "}", 2) == 0)
			return (false);
		i++;
	}
	return (true);
}

bool	syntax_check(char **args, char **envp, t_node *node)
{
	(void)envp;
	(void)node;
	if (!args || !args[0])
		return (true);
	if (!check_brace_syntax(args))
		return (false);
	if (!check_leading_operators_syntax(args))
		return (false);
	if (!check_consecutive_operators_syntax(args))
		return (false);
	if (!check_trailing_operators_syntax(args))
		return (false);
	return (true);
}

void	handle_syntax_error(char **envp, t_node *node)
{
	const char	*error_token;

	(void)envp;
	error_token = get_error_token(node->ori_args);
	if (error_token && ft_strncmp(error_token, "{", 2) == 0)
	{
		ft_putstr_fd("minishell: syntax error: unexpected end of file\n",
			STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putstr_fd(error_token, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
	}
	set_exit_status(2);
	if (node)
		node->syntax_flag = true;
}

bool	in_heredoc(char *str, int i)
{
	if (!str || i < 0)
		return (false);
	while (i && !ft_strchr(" \t", str[i]))
		i--;
	while (i && ft_strchr(" \t", str[i]))
		i--;
	return (i > 0 && str[i] == '<' && str[i - 1] == '<');
}

int	quote_check(char const *str, int i, t_node *node)
{
	int	quote;
	int	j;

	if (!str || i < 0)
		return (0);
	quote = 0;
	j = 0;
	while (j <= i && str[j])
	{
		if (str[j] == '\'' && quote != 2)
			quote = (quote == 1) * 0 + (quote != 1) * 1;
		else if (str[j] == '"' && quote != 1)
			quote = (quote == 2) * 0 + (quote != 2) * 2;
		j++;
	}
	(void)node;
	return (quote);
}

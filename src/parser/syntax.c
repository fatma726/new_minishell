/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/08 12:51:26 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

const char	*get_full_original_token(char **ori_args);
const char	*find_fallback_token(char **ori_args);
bool		check_brace_syntax(char **args);

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
	const char	*tok;

	(void)envp;
	error_token = get_error_token(node->ori_args);
	if (error_token && ft_strncmp(error_token, "{", 2) == 0)
	{
		ft_putstr_fd("bash: -c: line 1: ", STDERR_FILENO);
		ft_putstr_fd("syntax error: unexpected end of file\n", STDERR_FILENO);
		ft_putstr_fd("bash: -c: line 1: `{`\n", STDERR_FILENO);
		return (set_exit_status_n(node, 2), node->syntax_flag = true, (void)0);
	}
	if (error_token)
		tok = error_token;
	else
		tok = find_fallback_token(node->ori_args);
	ft_putstr_fd("bash: -c: line 1: ", STDERR_FILENO);
	ft_putstr_fd("syntax error near unexpected token `", STDERR_FILENO);
	(ft_putstr_fd(tok, STDERR_FILENO), ft_putendl_fd("'", STDERR_FILENO));
	ft_putstr_fd("bash: -c: line 1: `", STDERR_FILENO);
	ft_putstr_fd(get_full_original_token(node->ori_args), STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	(set_exit_status_n(node, 2), node->syntax_flag = true);
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
	bool	in_single;
	bool	in_double;
	int		j;
	char	c;

	if (!str || i < 0)
		return (0);
	in_single = false;
	in_double = false;
	j = 0;
	while (j <= i && str[j])
	{
		c = str[j];
		if (c == '\'' && !in_double)
			in_single = !in_single;
		else if (c == '"' && !in_single)
			in_double = !in_double;
		j++;
	}
	(void)node;
	if (in_single)
		return (1);
	if (in_double)
		return (2);
	return (0);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_error_handlers.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../bonus/include/bonus.h"
#include "../../include/bonus.h"

static void	handle_redir_syntax_error(t_node *n, char *token)
{
	n->syntax_flag = true;
	set_exit_status(2);
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

bool	check_redir_token(char c, t_node *n)
{
	if (c == '&')
	{
		handle_redir_syntax_error(n, "&");
		return (false);
	}
	return (true);
}

void	handle_double_ampersand_error_public(t_node *n)
{
	handle_redir_syntax_error(n, "&&");
}

void	handle_double_pipe_error_public(t_node *n)
{
	handle_redir_syntax_error(n, "||");
}

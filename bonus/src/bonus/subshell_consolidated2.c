/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_consolidated2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../bonus/include/bonus.h"

char	*handle_triple_redir_error(char *str, t_node *node)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `>'\n",
		STDERR_FILENO);
	set_exit_status(2);
	node->syntax_flag = true;
	return (free(str), NULL);
}

char	*handle_paren_error(char *str, int count, t_node *node)
{
	if (count == -2)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putendl_fd(")'", STDERR_FILENO);
		set_exit_status(2);
		node->syntax_flag = true;
		return (free(str), NULL);
	}
	if (count != 0)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);
		set_exit_status(2);
		node->syntax_flag = true;
		return (free(str), NULL);
	}
	return (str);
}

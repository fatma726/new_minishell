/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_consolidated.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "bonus.h"
#include "../../include/bonus.h"

char	*create_clean_string(char *str, t_node *node, int length);
int		validate_parens(char *str, t_node *node, int *length);

char	*create_clean_string(char *str, t_node *node, int length)
{
	char	*new_str;
	size_t	size;
	int		i;

	size = (size_t)(length + 1) * sizeof(char);
	new_str = malloc(size);
	i = -1;
	length = 0;
	while (str[++i])
		if ((str[i] != '(' && str[i] != ')') || quote_check(str, i, node))
			new_str[length++] = str[i];
	new_str[length] = '\0';
	free(str);
	return (new_str);
}

static int	redir_ampersand_token(char *str, int i, t_node *node)
{
	if (str[i + 1] == '&'
		|| (str[i + 1] == '>' && str[i + 2] == '&'))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		if (str[i + 1] == '&' && str[i + 2] == '&')
			ft_putendl_fd("&&'", STDERR_FILENO);
		else
			ft_putendl_fd("&'", STDERR_FILENO);
		set_exit_status(2);
		node->syntax_flag = true;
		free(str);
		return (-1);
	}
	return (0);
}

int	validate_parens(char *str, t_node *node, int *length)
{
	int			count;
	int			i;

	count = 0;
	*length = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == '>' && !quote_check(str, i, node))
			if (redir_ampersand_token(str, i, node) == -1)
				return (-1);
		if ((str[i] == '(' || str[i] == ')') && !quote_check(str, i, node))
		{
			if (str[i] == '(')
				count++;
			else if (--count < 0)
				return (-2);
		}
		else
			(*length)++;
	}
	return (count);
}

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

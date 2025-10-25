/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_standalone.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static char	**report_syntax_token(const char *tok, char **envp,
									t_node *n, char *line)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putstr_fd(tok, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
	set_exit_status(2);
	n->syntax_flag = true;
	free(line);
	return (envp);
}

static char	**check_standalone_misc(char *line, int i, char **envp, t_node *n)
{
	if (line[i] == '|' && (line[i + 1] == '\0' || line[i + 1] == '\n'))
		return (report_syntax_token("|", envp, n, line));
	return (NULL);
}

char	**check_standalone_operators(char *line, char **envp, t_node *n)
{
	int		i;
	char	**r;

	i = 0;
	while (line[i] && ft_strchr(" \t", line[i]))
		i++;
	if (!line[i] || line[i] == '\n')
		return (NULL);
	if (!quote_check(line, i, n))
	{
		r = check_standalone_misc(line, i, envp, n);
		if (r)
			return (r);
	}
	return (NULL);
}

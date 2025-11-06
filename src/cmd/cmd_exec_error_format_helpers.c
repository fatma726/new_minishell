/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error_format_helpers.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static int	has_special_chars(const char *cmd)
{
	int	i;

	i = -1;
	while (cmd[++i])
		if (cmd[i] == '\t' || cmd[i] == '\n' || cmd[i] == '\r')
			return (1);
	return (0);
}

static void	print_escaped_char(char c)
{
	if (c == '\t')
		ft_putstr_fd("\\t", STDERR_FILENO);
	else if (c == '\n')
		ft_putstr_fd("\\n", STDERR_FILENO);
	else if (c == '\r')
		ft_putstr_fd("\\r", STDERR_FILENO);
	else
		ft_putchar_fd(c, STDERR_FILENO);
}

static void	print_ansi_c_quoted(const char *cmd)
{
	int	i;

	ft_putstr_fd("$'", STDERR_FILENO);
	i = -1;
	while (cmd[++i])
	{
		if (cmd[i] == '$' && (cmd[i + 1] == '\t'
				|| cmd[i + 1] == '\n' || cmd[i + 1] == '\r'))
			i++;
		else
			print_escaped_char(cmd[i]);
	}
	ft_putchar_fd('\'', STDERR_FILENO);
}

void	print_escaped_cmd(const char *cmd)
{
	if (!cmd || !cmd[0])
		return ;
	if (!has_special_chars(cmd))
		return (ft_putstr_fd(cmd, STDERR_FILENO));
	print_ansi_c_quoted(cmd);
}

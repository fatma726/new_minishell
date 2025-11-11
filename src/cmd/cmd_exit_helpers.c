/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:47:04 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* split to reduce function count per file */

bool	ft_isalldigit(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	if (!str[i])
		return (false);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i] && ft_strchr(" \t", str[i]))
		i++;
	return (str[i] == '\0');
}

void	handle_numeric_error(char *arg, t_node *node)
{
	char	exit_error[30];
	char	numeric_error[30];

	set_exit_status_n(node, 2);
	ft_strlcpy(exit_error, "minishell: exit: ", 20);
	ft_strlcpy(numeric_error, ": numeric argument required\n", 30);
	ft_putstr_fd(exit_error, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(numeric_error, STDERR_FILENO);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/06 21:32:04 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	handle_exit_message(void)
{
	char	exit_msg[5];

	if (isatty(STDIN_FILENO))
	{
		ft_strlcpy(exit_msg, "exit", 5);
		ft_putendl_fd(exit_msg, STDOUT_FILENO);
	}
}

bool	exit_will_terminate(char **args)
{
	size_t	argc;

	if (!args)
		return (false);
	argc = strarrlen(args);
	if (argc <= 1)
		return (true);
	if (argc == 2 && ft_isalldigit(args[1]))
		return (true);
	return (false);
}

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

void	handle_too_many_args(t_node *node)
{
	char	too_many[47];

	set_exit_status_n(node, EXIT_FAILURE);
	ft_strlcpy(too_many, "minishell: exit: too many arguments\n", 37);
	ft_putstr_fd(too_many, STDERR_FILENO);
}

/* moved nested-quote helpers to cmd_exit_utils.c */

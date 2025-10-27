/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_consolidated2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:04 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	print_escaped_value(char *value)
{
	size_t	i;

	i = 0;
	while (value[i])
	{
		if (value[i] == '"' || value[i] == '\\' || value[i] == '$')
			ft_putchar_fd('\\', STDOUT_FILENO);
		ft_putchar_fd(value[i], STDOUT_FILENO);
		i++;
	}
}

bool	is_valid_identifier_start(char c)
{
	return (ft_isalpha((unsigned char)c) || c == '_');
}

bool	is_valid_identifier_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}

void	print_invalid_identifier_error(char *arg)
{
	char	export_error[35];
	char	not_valid[30];

	ft_strlcpy(export_error, "minishell: export: `", 25);
	ft_putstr_fd(export_error, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_strlcpy(not_valid, "': not a valid identifier\n", 30);
	ft_putstr_fd(not_valid, STDERR_FILENO);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/22 17:08:32 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	check_operators_at_depth_zero(const char *s, int i, int *has_andand)
{
	if (s[i] == '&' && s[i + 1] == '&')
		*has_andand = 1;
	else if (s[i] == '|' && s[i + 1] == '|')
	{
		if (*has_andand)
			return (-1);
		return (i);
	}
	return (0);
}

int	scan_for_oror(const char *s, t_node *n, int *has_andand)
{
	int	i;
	int	depth;
	int	result;

	i = 0;
	depth = 0;
	while (s[i])
	{
		if (!quote_check(s, i, n))
		{
			if (s[i] == '(')
				depth++;
			else if (s[i] == ')')
				depth -= (depth > 0);
			else if (depth == 0)
			{
				result = check_operators_at_depth_zero(s, i, has_andand);
				if (result != 0)
					return (result);
			}
		}
		i++;
	}
	return (-1);
}

char	**handle_oror_error(char *left, char *right, char *hashed,
		char **envp)
{
	set_exit_status(2);
	ft_putstr_fd("minishell: syntax error near unexpected token `",
		STDERR_FILENO);
	ft_putendl_fd("||'", STDERR_FILENO);
	free(left);
	free(right);
	free(hashed);
	return (envp);
}

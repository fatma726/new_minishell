/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_command.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static int	check_quote_result(bool in_single, bool in_double, char *which)
{
	if (in_single)
	{
		if (which)
			*which = '\'';
		return (1);
	}
	if (in_double)
	{
		if (which)
			*which = '"';
		return (1);
	}
	return (0);
}

static int	has_unmatched_quotes(const char *s, char *which)
{
	int		i;
	bool	in_single;
	bool	in_double;

	i = 0;
	in_single = false;
	in_double = false;
	while (s && s[i])
	{
		if (s[i] == '\\' && in_double && s[i + 1] == '"')
		{
			i += 2;
			continue ;
		}
		handle_quote_state(s[i], &in_single, &in_double);
		i++;
	}
	return (check_quote_result(in_single, in_double, which));
}

char	**dispatch_line(char *hashed, char **envp, t_node *n)
{
	return (subshell(hashed, envp, n));
}

char	**process_command(char *line, char **envp, t_node *n)
{
	char		*hashed;
	char		**result;
	char		which;

	if (!line || is_blank(line))
		return (free(line), envp);
	if (!isatty(STDIN_FILENO) && has_unmatched_quotes(line, &which))
	{
		ft_putstr_fd("minishell: unexpected EOF while looking for matching `",
			STDERR_FILENO);
		ft_putchar_fd(which, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		set_exit_status(2);
		free(line);
		return (envp);
	}
	result = check_standalone_operators(line, envp, n);
	if (result)
		return (result);
	hashed = hash_handler(line, n);
	envp = dispatch_line(hashed, envp, n);
	if (n->syntax_flag)
		set_exit_status(2);
	return (envp);
}

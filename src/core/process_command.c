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
			i += 2;
		else
		{
			if (s[i] == '\'' && !in_double)
				in_single = !in_single;
			else if (s[i] == '"' && !in_single)
				in_double = !in_double;
			i++;
		}
	}
	if (in_single && which)
		*which = '\'';
	if (in_double && which)
		*which = '"';
	return (in_single || in_double);
}

static char	**handle_or_sections(char *hashed, char **envp, t_node *n)
{
	int		idx;
	int		has_andand;
	char	*parts[2];

	has_andand = 0;
	idx = scan_for_oror(hashed, n, &has_andand);
	if (idx < 0)
		return (subshell(hashed, envp, n));
	if (has_andand)
		return (handle_oror_error(NULL, NULL, hashed, envp));
	parts[0] = ft_substr(hashed, 0, idx);
	parts[1] = ft_strdup(hashed + idx + 2);
	if (!parts[0] || !parts[1] || is_blank(parts[0]) || is_blank(parts[1]))
		return (handle_oror_error(parts[0], parts[1], hashed, envp));
	envp = subshell(parts[0], envp, n);
	if (get_exit_status() != 0)
		envp = subshell(parts[1], envp, n);
	else
		free(parts[1]);
	free(hashed);
	return (envp);
}

char	**dispatch_line(char *hashed, char **envp, t_node *n)
{
	int	i;

	i = 0;
	while (hashed[i] && (hashed[i] == ' ' || hashed[i] == '\t'))
		i++;
	if (hashed[i] == ';')
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
			STDERR_FILENO);
		ft_putendl_fd(";'", STDERR_FILENO);
		set_exit_status(2);
		n->syntax_flag = true;
		free(hashed);
		return (envp);
	}
	return (handle_or_sections(hashed, envp, n));
}

static bool	report_unmatched_if_needed(char *line)
{
	char	which;

	if (isatty(STDIN_FILENO))
		return (false);
	if (!has_unmatched_quotes(line, &which))
		return (false);
	ft_putstr_fd(
		"minishell: unexpected EOF while looking for matching `",
		STDERR_FILENO);
	ft_putchar_fd(which, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	set_exit_status(2);
	free(line);
	return (true);
}

char	**process_command(char *line, char **envp, t_node *n)
{
	char		*hashed;
	char		**result;

	if (!line || is_blank(line))
		return (free(line), envp);
	if (report_unmatched_if_needed(line))
		return (envp);
	result = check_standalone_operators(line, envp, n);
	if (result)
		return (result);
	hashed = hash_handler(line, n);
	envp = dispatch_line(hashed, envp, n);
	if (n->syntax_flag)
		set_exit_status(2);
	return (envp);
}

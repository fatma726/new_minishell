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
    int   i;
    bool  in_single;
    bool  in_double;

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
        if (s[i] == '\'' && !in_double)
            in_single = !in_single;
        else if (s[i] == '"' && !in_single)
            in_double = !in_double;
        i++;
    }
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

char	**dispatch_line(char *hashed, char **envp, t_node *n)
{
	int	i;
    int idx;
    int has_andand;
    char *left;
    char *right;

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
	/* Minimal top-level || support: split and execute right only if left failed */
	has_andand = 0;
	idx = scan_for_oror(hashed, n, &has_andand);
	if (idx >= 0)
	{
		if (has_andand)
			return (handle_oror_error(NULL, NULL, hashed, envp));
		left = ft_substr(hashed, 0, idx);
		right = ft_strdup(hashed + idx + 2);
		if (!left || !right || is_blank(left) || is_blank(right))
			return (handle_oror_error(left, right, hashed, envp));
		/* execute left */
		envp = subshell(left, envp, n);
		/* free left copy consumed by subshell (subshell frees internally) */
		/* if left failed, execute right */
		if (get_exit_status() != 0)
			envp = subshell(right, envp, n);
		else
			free(right);
		free(hashed);
		return (envp);
	}
	return (subshell(hashed, envp, n));
}

char	**process_command(char *line, char **envp, t_node *n)
{
    char		*hashed;
    char		**result;
    char        which;

    if (!line || is_blank(line))
        return (free(line), envp);
    /* In non-interactive input, report unmatched quotes as syntax error */
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/11/02 00:18:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	sanitize_bar_in_word(char **args, t_node *node)
{
	int	k;

	k = 0;
	while (args[k])
	{
		if (args[k][0] == '|' && args[k][1] == '\0')
		{
			args[k] = NULL;
			break ;
		}
		k++;
	}
	node->pipe_word_has_bar = true;
	set_exit_status(127);
}

char	**handle_parser_errors(char **args, char **envp, t_node *node)
{
	if (!args)
		return (envp);
	if (args[0] && !syntax_check(node->ori_args, envp, node))
	{
		handle_syntax_error(envp, node);
		strarrfree(args);
		if (node->ori_args)
		{
			strarrfree(node->ori_args);
			node->ori_args = NULL;
		}
		return (envp);
	}
	return (NULL);
}

/* moved to parser_helpers.c */

static char	**dispatch_builtin_tail(char **a, char **e, t_node *n)
{
	if (!ft_strncmp(a[0], "exit", 5))
	{
		cmd_exit(a, e, n);
		return (e);
	}
	if (!ft_strncmp(a[0], "env", 4))
		return (cmd_env(a, e, n));
	if (!ft_strncmp(a[0], "export", 7))
		return (cmd_export(a, e, n));
	if (!ft_strncmp(a[0], "pwd", 4))
	{
		cmd_pwd(n);
		return (e);
	}
	if (!ft_strncmp(a[0], "echo", 5))
	{
		cmd_echo(a, n);
		return (e);
	}
	if (!ft_strncmp(a[0], "unset", 6))
		return (cmd_unset(a, e, n));
	return (cmd_exec(a, e, n));
}

char	**dispatch_builtin(char **a, char **e, t_node *n)
{
	if (a[0] && a[0][0])
	{
		if (!ft_strncmp(a[0], "cd", 3))
			return (cmd_cd(a, e, n));
		if (!ft_strncmp(a[0], ":", 2))
		{
			set_exit_status(EXIT_SUCCESS);
			return (e);
		}
		if (!ft_strncmp(a[0], "!", 2))
		{
			set_exit_status(1);
			return (e);
		}
		return (dispatch_builtin_tail(a, e, n));
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	set_exit_status(127);
	return (e);
}

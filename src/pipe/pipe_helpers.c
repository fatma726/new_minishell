/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 13:31:37 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static bool	has_non_redir_token(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (isdlr(args[i]) || isdrr(args[i]) || islr(args[i])
			|| islrr(args[i]) || isrr(args[i]) || istlr(args[i]))
		{
			if (args[i + 1])
				i += 2;
			else
				i += 1;
			continue ;
		}
		if (args[i][0])
			return (true);
		i++;
	}
	return (false);
}

static char	**handle_empty_command(char **args, char **envp, t_node *node,
				int redir_ret)
{
	if (redir_ret != 0)
		return (envp);
	if (args && args[0] && !args[0][0])
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		set_exit_status_n(node, 127);
	}
	return (envp);
}

char	**one_commnad(char **args, char **envp, t_node *node)
{
	int		redir_ret;
	char	**old_envp;
	char	**new_envp;

	redir_ret = 0;
	if (redir_chk(node->ori_args))
		redir_ret = exec_redir(args, envp, node);
	if (!has_non_redir_token(args))
		return (handle_empty_command(args, envp, node, redir_ret));
	if (node->redir_fd >= 0 && !node->pipe_flag)
	{
		lseek(node->redir_fd, 0, SEEK_SET);
		dup2(node->redir_fd, STDIN_FILENO);
		close(node->redir_fd);
		node->redir_fd = -1;
	}
	old_envp = envp;
	new_envp = find_command(args, envp, node);
	if (old_envp != new_envp)
	{
		if (old_envp && !is_builtin_command(args))
			strarrfree(old_envp);
	}
	return (new_envp);
}

/* moved pipe helpers to pipe_helpers_more.c */

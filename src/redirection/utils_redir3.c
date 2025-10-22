/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir3.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:13 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	exec_redir_child(char **args, char **envp, t_node *node, int *flag)
{
	node->redir_stop = 0;
	if (!node->redir_flag)
		return ;
	if (node->redir_idx)
	{
		close(node->redir_fds[0]);
		dup2(node->redir_fds[1], STDOUT_FILENO);
		close(node->redir_fds[1]);
		if (exec_redir(args, envp, node))
			*flag = 1;
		return ;
	}
	if (exec_redir(args, envp, node))
		*flag = 1;
}

void	exec_redir_parents(char **args, char **envp, t_node *node, int *flag)
{
	int	status;

	waitpid(-1, &status, 0);
	close(node->redir_fds[1]);
	dup2(node->redir_fds[0], STDIN_FILENO);
	close(node->redir_fds[0]);
	if (exec_redir(args, envp, node))
		*flag = 1;
	args_left_move_i(args, node);
}

bool	is_redir_check(char *str)
{
	return (islr(str) || isrr(str) || isdrr(str) || isdlr(str));
}

int	left_redir_expand(char **args, int i, t_node *node, char **expanded)
{
	char	*tmp;

	tmp = expand_wildcard_redir(args[i + 1], node);
	if (!tmp)
		return (left_redir_error_public(args[i + 1], 0, NULL));
	if (ft_strncmp(tmp, args[i + 1], ft_strlen(args[i + 1]) + 1) == 0
		&& ft_strchr(args[i + 1], '*'))
		return (left_redir_error_public(args[i + 1], 1, tmp));
	if (access(tmp, R_OK))
		return (left_redir_error_public(tmp, 1, tmp));
	*expanded = tmp;
	return (0);
}

void	handle_echo_skip(char **args, t_node *node)
{
	if (!ft_strncmp(args[0], "echo", 5)
		&& !ft_strncmp(args[1], "./", 2)
		&& !args[2])
		node->echo_skip = 1;
}

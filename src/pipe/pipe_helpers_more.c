/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers_more.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	prepare_redirections(char **args, char **envp, t_node *node)
{
	int	redir_err;

	node->redir_flag = redir_chk(node->ori_args);
	redir_err = 0;
	(void)pipe_check(args, node);
	if (node->redir_flag)
		redir_err = redir_excute(args, envp, node, 0);
	(void)pipe_check(args, node);
	return (redir_err);
}

int	maybe_setup_pipe(t_node *node)
{
	int	pid;

	pid = 0;
	if (node->pipe_flag)
	{
		if (pipe(node->fds) == -1)
			return (-1);
		pid = fork();
		if (pid < 0)
		{
			close(node->fds[0]);
			close(node->fds[1]);
			return (-1);
		}
	}
	return (pid);
}

void	run_parent_segment(char **args, char **envp, t_node *node)
{
	char	**new_ori_args;
	char	**new_args;
	char	**new_envp;
	int		status;

	close(node->fds[1]);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status))
		set_exit_status_n(node, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status_n(node, 128 + WTERMSIG(status));
	if (node->ori_args && node->ori_args[node->pipe_idx])
		new_ori_args = strarrdup(node->ori_args + node->pipe_idx);
	else
		new_ori_args = NULL;
	node->ori_args = new_ori_args;
	new_args = strarrdup(args + node->pipe_idx);
	new_envp = strarrdup(envp);
	node->redir_flag = redir_chk(node->ori_args);
	node->redir_stop = 0;
	node->redir_idx = 0;
	exec_parents(new_args, new_envp, node);
	strarrfree(new_args);
	strarrfree(new_envp);
}

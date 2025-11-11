/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_core.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:52:26 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	process_input_redirections(char **args, char **envp, t_node *node);
void	exec_child_envp_handle(char **args, char **envp, t_node *node);
void	apply_heredoc_stdin(t_node *node);
void	setup_child_fds(t_node *node);
void	reset_child_signals(void);
void	child_fast_exit(t_node *node);

void	exec_child(char **args, char **envp, t_node *node)
{
	close_child_backups(node);
	node->exit_flag = 0;
	if (!node->child_die && node->redir_flag)
		process_input_redirections(args, envp, node);
	apply_heredoc_stdin(node);
	setup_child_fds(node);
	reset_child_signals();
	if (!node->child_die)
		exec_child_envp_handle(args, envp, node);
	else
	{
		child_fast_exit(node);
	}
}

void	exec_parents(char **args, char **envp, t_node *node)
{
	node->exit_flag = 0;
	dup2(node->fds[0], STDIN_FILENO);
	close(node->fds[0]);
	node->pipe_flag = 0;
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	envp = repeat(args, envp, node);
	backup_restor(node);
}

/* cloturn function removed - backup FDs now closed directly in execute() */

int	pipe_check(char **args, t_node *node)
{
	int	i;

	i = -1;
	while (args[++i])
	{
		if (isp(node->ori_args[i]))
		{
			if (node->quota_idx_j < node->quota_pipe_cnt
				&& node->quota_pipe_idx_arr[node->quota_idx_j] == i)
				node->quota_idx_j++;
			else
			{
				node->pipe_idx = i + 1;
				node->pipe_flag = 1;
				return (1);
			}
		}
	}
	node->pipe_flag = 0;
	return (0);
}

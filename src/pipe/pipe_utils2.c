/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:13:14 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	repeat_exec(char **args, char **envp, t_node *node, int pid)
{
	if (!pid)
		exec_child(args, envp, node);
	else
		run_parent_segment(args, envp, node);
}

void	maybe_handle_exit(char **args, char **envp, t_node *node)
{
	if (args && args[0] && !ft_strncmp(args[0], "exit", 5))
	{
		if (exit_will_terminate(args))
		{
			node->exit_flag = 1;
			cmd_exit(args, envp, node);
		}
	}
}

void	init_node(t_node *node)
{
	node->child_die = 0;
	node->echo_skip = 0;
	node->escape_skip = false;
	node->argmode = false;
	node->exit_flag = 1;
	node->parent_die = 0;
	node->pipe_flag = 0;
	node->pipe_idx = 0;
	node->quota_pipe_cnt = 0;
	node->redir_idx = 0;
	node->redir_stop = 0;
	node->right_flag = 0;
	node->redir_fd = -1;
	node->cmd = NULL;
	node->heredoc_unterminated = false;
	node->heredoc_swallowed_input = false;
	node->backup_stdout = -1;
	node->backup_stdin = -1;
	node->stdin_backup = -1;
	node->stdout_backup = -1;
	node->stderr_backup = -1;
	node->pipe_word_has_bar = false;
	node->full_ori_args = NULL;
	node->parser_tokens = NULL;
	node->parser_tmp_str = NULL;
}

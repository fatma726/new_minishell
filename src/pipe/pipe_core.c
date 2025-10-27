/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_core.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

#ifdef BUILD_BONUS
# include "../../bonus/include/bonus.h"
#endif

static void	process_input_redirections(char **args, char **envp, t_node *node)
{
	int	i;

	i = 0;
	while (i < node->pipe_idx - 1 && !isp(node->ori_args[i]))
	{
		if (islr(node->ori_args[i]) || islrr(node->ori_args[i]))
		{
			left_redir(args, envp, &i, node);
			break ;
		}
		i++;
	}
}

void	exec_child(char **args, char **envp, t_node *node)
{
	char	**child_args;

	node->exit_flag = 0;
	if (!node->child_die && node->redir_flag)
		process_input_redirections(args, envp, node);
	close(node->fds[0]);
	if (!node->right_flag)
		dup2(node->fds[1], STDOUT_FILENO);
	close(node->fds[1]);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	if (!node->child_die)
	{
		child_args = split_before_pipe_args(args, node);
		envp = shlvl_mod(1, envp);
		envp = find_command(child_args, envp, node);
		envp = shlvl_mod(-1, envp);
		strarrfree(child_args);
	}
	cleanup_child_and_exit(args, envp, node);
}

void	exec_parents(char **args, char **envp, t_node *node)
{
	node->exit_flag = 0;
	/* Parent stays in our signal regime; only children reset to defaults. */
	/* Wire stdin to the read end only when a pipe is actually active. */
	if (node->pipe_flag)
	{
		close(node->fds[1]);
		dup2(node->fds[0], STDIN_FILENO);
		close(node->fds[0]);
	}
	node->pipe_flag = 0;
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
}

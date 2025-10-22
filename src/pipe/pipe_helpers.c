/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_helpers.c                                   :+:      :+:    :+:   */
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

char	**one_commnad(char **args, char **envp, t_node *node)
{
	int	redir_ret;

	redir_ret = 0;
	if (redir_chk(node->ori_args))
		redir_ret = exec_redir(args, envp, node);
	if (!has_non_redir_token(args))
	{
		if (redir_ret == 0 && get_exit_status() == 0)
			set_exit_status(0);
		if (args && args[0] && !args[0][0])
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(": command not found\n", STDERR_FILENO);
			set_exit_status(127);
		}
		return (envp);
	}
	envp = find_command(args, envp, node);
	if (node->redir_flag)
		backup_restor(node);
	return (envp);
}

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

	backup_restor(node);
	new_ori_args = strarrdup(node->ori_args + node->pipe_idx);
	strarrfree(node->ori_args);
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

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	handle_heredoc_prepass(char **args, char **envp, t_node *node)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	while (args[++i]
		&& !isp(node->ori_args[i])
		&& !node->redir_stop
		&& !ret)
		if (isdlr(node->ori_args[i]))
			ret = left_double_redir(args, envp, &i, node);
	return (ret);
}

static int	handle_main_redirs(char **args, char **envp, t_node *node)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	while (args[++i]
		&& !isp(node->ori_args[i])
		&& !node->redir_stop
		&& !ret)
	{
		if (node->pipe_flag && i >= node->pipe_idx - 1)
			break ;
		if (islr(node->ori_args[i]) || islrr(node->ori_args[i]))
			ret = left_redir(args, envp, &i, node);
		else if (isrr(node->ori_args[i]))
			ret = right_redir(args, &i, node);
		else if (isdrr(node->ori_args[i]))
			ret = right_double_redir(args, &i, node);
		else if (istlr(node->ori_args[i]))
			ret = left_double_redir(args, envp, &i, node);
	}
	return (ret);
}

static int	handle_standalone_heredoc(char **args, char **envp, t_node *node)
{
	int	i;
	int	ret;

	ret = 0;
	i = -1;
	while (args[++i]
		&& !isp(node->ori_args[i])
		&& !node->redir_stop
		&& !ret)
		if (isdlr(node->ori_args[i]))
			ret = left_double_redir(args, envp, &i, node);
	if (!ret)
		set_exit_status(0);
	return (ret);
}

/* moved to utils_redir3.c to balance function count */

int	exec_redir(char **args, char **envp, t_node *node)
{
	int	ret;

	ret = 0;
	if (exec_check(args, envp, node))
		node->cmd = ft_strdup(args[0]);
	ret = handle_heredoc_prepass(args, envp, node);
	if (!ret)
		ret = handle_main_redirs(args, envp, node);
	if (!node->cmd && !ret)
		ret = handle_standalone_heredoc(args, envp, node);
	if (node->redir_fd >= 0 && !node->semicolon_sequence)
	{
		cleanup_heredoc_file(node);
		unlink(".temp");
		node->redir_fd = -1;
	}
	if (node->cmd)
		free(node->cmd);
	return (node->cmd = NULL, ret);
}

int	print_err2(char **args, int i)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	errno = ENOENT;
	ft_putstr_fd(args[i + 2], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (1);
}

/* print_err3 removed - not used in codebase */

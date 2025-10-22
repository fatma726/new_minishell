/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	left_redir_post(char **args, char **envp, int *i, t_node *node)
{
	if (!node->cmd && args[*i + 2]
		&& !is_redir_check(node->ori_args[*i + 2])
		&& !exec_check(args + 2, envp, node))
		return (print_err2(args, *i));
	args_left_move(args, *i);
	args_left_move(node->ori_args, *i);
	args_left_move(args, *i);
	args_left_move(node->ori_args, *i);
	*i -= 1;
	return (0);
}

/* handle_heredoc_cleanup removed - unused function */

int	left_redir(char **args, char **envp, int *i, t_node *node)
{
	int		fd;
	char	*expanded_path;

	if (islr(node->ori_args[*i]))
	{
		if (left_redir_expand(args, *i, node, &expanded_path))
			return (1);
		fd = open(expanded_path, O_RDONLY, 0744);
		free(expanded_path);
	}
	else
		fd = open(args[*i + 1], O_CREAT | O_RDWR, 0744);
	if (fd < 0)
		return (1);
	handle_echo_skip(args, node);
	dup2(fd, STDIN_FILENO);
	if (left_redir_post(args, envp, i, node))
	{
		close(fd);
		return (1);
	}
	return (0);
}

int	left_double_redir(char **args, char **envp, int *i, t_node *node)
{
	if (isdlr(node->ori_args[*i]) || istlr(node->ori_args[*i]))
	{
		if (!args[*i + 1] || !args[*i + 1][0])
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `",
				STDERR_FILENO);
			ft_putstr_fd("newline", STDERR_FILENO);
			ft_putendl_fd("'", STDERR_FILENO);
			set_exit_status(2);
			node->redir_stop = 1;
			return (1);
		}
		if (node->redir_fd < 0 && setup_heredoc_file(node))
			return (1);
		if (heredoc_loop(args, envp, i, node))
			return (1);
		move_redir_args(args, node->ori_args, i);
		*i -= 1;
		set_exit_status(0);
		return (0);
	}
	return (0);
}

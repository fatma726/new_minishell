/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 20:44:38 by fatmtahmdab      ###   ########.fr       */
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

void	print_heredoc_syntax_error(t_node *node);

static int	open_left_redir_file(char **args, int *i, t_node *node)
{
	int		fd;
	char	*expanded_path;

	if (islr(node->ori_args[*i]))
	{
		if (left_redir_expand(args, *i, node, &expanded_path))
		{
			set_exit_status_n(node, 1);
			return (-1);
		}
		fd = open(expanded_path, O_RDONLY, 0744);
		free(expanded_path);
	}
	else
		fd = open(args[*i + 1], O_CREAT | O_RDWR, 0744);
	return (fd);
}

int	left_redir(char **args, char **envp, int *i, t_node *node)
{
	int	fd;

	fd = open_left_redir_file(args, i, node);
	if (fd < 0)
	{
		if (islr(node->ori_args[*i]))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(args[*i + 1], STDERR_FILENO);
			ft_putstr_fd(": ", STDERR_FILENO);
			ft_putstr_fd(strerror(errno), STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
		}
		set_exit_status_n(node, 1);
		return (1);
	}
	handle_echo_skip(args, node);
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (left_redir_post(args, envp, i, node))
	{
		set_exit_status_n(node, 1);
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
			print_heredoc_syntax_error(node);
			return (1);
		}
		if (node->redir_fd < 0 && setup_heredoc_file(node))
			return (1);
		if (heredoc_loop(args, envp, i, node))
			return (1);
		move_redir_args(args, node->ori_args, i);
		*i -= 1;
		if (!has_non_redir_token(args))
			set_exit_status_n(node, 127);
		return (0);
	}
	return (0);
}

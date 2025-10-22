/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_redir2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:13 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

int	print_err(char **args, int i, t_node *node)
{
	(void)args;
	(void)i;
	ft_putchar_fd(' ', STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	set_exit_status(1);
	node->parent_die = 1;
	if (pipe_check(args, node))
	{
		if (isp(node->ori_args[i + 1]))
			node->redir_stop = 1;
		args_left_move(args, 1);
		args_left_move(node->ori_args, 1);
		if (isp(node->ori_args[i + 1]))
			node->redir_stop = 1;
		args_left_move(args, 0);
		args_left_move(node->ori_args, 0);
		if (isp(node->ori_args[i + 1]))
			node->redir_stop = 1;
		node->child_die = 1;
		return (0);
	}
	return (1);
}

void	args_left_move(char **args, int i)
{
	while (args[i] && args[i + 1])
	{
		free(args[i]);
		args[i] = ft_strdup(args[i + 1]);
		if (!args[i])
			return ;
		i++;
	}
	if (!args[i + 1])
	{
		free(args[i]);
		args[i] = NULL;
	}
}

bool	is_redir(char **args, int i, int j)
{
	return (args && args[i] && (args[i][j] == '<' || args[i][j] == '>'));
}

/* moved to utils_redir3.c to satisfy norm function count */

int	open_redir_out(char **args, int i, t_node *node, int flags)
{
	int		fd;
	char	*expanded_path;

	expanded_path = expand_wildcard_redir(args[i + 1], node);
	if (!expanded_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[i + 1], STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		set_exit_status(1);
		return (-1);
	}
	fd = open(expanded_path, flags, 0644);
	free(expanded_path);
	if (fd < 0)
	{
		print_err(args, i, node);
		return (-1);
	}
	return (fd);
}

void	args_left_move_i(char **args, t_node *node)
{
	int	i;

	i = 0;
	while (++i < node->redir_idx)
	{
		args_left_move(args, 1);
		args_left_move(node->ori_args, 1);
	}
}

/* moved to utils_redir4.c to satisfy norm function count */

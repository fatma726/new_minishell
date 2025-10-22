/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

int	left_redir_error_public(const char *s, int type, char *tmp)
{
	if (type == 0)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(s, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	}
	set_exit_status(1);
	if (tmp)
		free(tmp);
	return (1);
}

void	double_lmove_idx_change(char **args, int *i)
{
	args_left_move(args, *i);
	args_left_move(args, *i);
	*i -= 1;
}

static void	argu_cleanup(char **args, char **args2, int k, int len)
{
	const int	init_k = k;

	free(args[k]);
	free(args[k + 1]);
	if (k + 2 == len)
	{
		args[k] = NULL;
		args[k + 1] = NULL;
	}
	else
	{
		args[k + 1] = NULL;
		while (k + 2 < len)
		{
			args[k] = ft_strdup(args[k + 2]);
			if (!args[k])
				return ;
			free(args[k + 2]);
			args[k++ + 2] = NULL;
		}
	}
	if (args2)
		argu_cleanup(args2, NULL, init_k, len);
}

void	argu_left_change(char **args, t_node *node)
{
	int	i;
	int	k;
	int	len;

	i = -1;
	len = (int)strarrlen(args);
	while (args[++i])
	{
		if (isdlr(node->ori_args[i]))
		{
			k = i;
			if (k + 1 == len)
			{
				free(args[k]);
				free(node->ori_args[k]);
				args[k] = NULL;
				node->ori_args[k] = NULL;
			}
			else
				argu_cleanup(args, node->ori_args, k, len);
		}
	}
}

int	redir_chk(char **args)
{
	int	i;

	i = -1;
	while (args[++i])
		if (isdlr(args[i]) || isdrr(args[i]) || islr(args[i]) || islrr(args[i])
			|| isrr(args[i]) || istlr(args[i]))
			return (1);
	return (0);
}

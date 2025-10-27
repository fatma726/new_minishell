/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_wrappers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **argv)
{
	t_node	node;

	ft_bzero(&node, sizeof(node));
	cmd_echo(argv, &node);
	return (get_exit_status());
}

int	ft_pwd(void)
{
	t_node	node;

	ft_bzero(&node, sizeof(node));
	node.pwd = getcwd(NULL, 0);
	if (!node.pwd)
		return (1);
	cmd_pwd(&node);
	free(node.pwd);
	return (get_exit_status());
}

int	ft_env(char **env)
{
	char	*argv[2];
	t_node	node;

	ft_bzero(&node, sizeof(node));
	argv[0] = "env";
	argv[1] = NULL;
	(void)cmd_env(argv, env, &node);
	return (get_exit_status());
}

/* moved remaining wrappers to builtin_wrappers2.c to satisfy norm */

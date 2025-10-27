/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_wrappers2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_cd(char **argv, char ***penv)
{
	t_node	node;

	ft_bzero(&node, sizeof(node));
	*penv = cmd_cd(argv, *penv, &node);
	return (get_exit_status());
}

int	bi_unset(char **argv, char ***penv)
{
	t_node	node;

	ft_bzero(&node, sizeof(node));
	*penv = cmd_unset(argv, *penv, &node);
	return (get_exit_status());
}

int	bi_export(char **argv, char ***penv)
{
	t_node	node;

	ft_bzero(&node, sizeof(node));
	*penv = cmd_export(argv, *penv, &node);
	return (get_exit_status());
}

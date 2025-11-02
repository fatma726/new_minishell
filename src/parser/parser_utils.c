/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	*expand_prompt(char *fmt, char **envp, t_node *node)
{
	(void)envp;
	(void)node;
	return (fmt);
}

char	**find_command(char **args, char **envp, t_node *node)
{
	int	i;

	if (!args || !args[0] || !args[0][0])
		return (envp);
	i = 0;
	while (args[i] && args[i + 1] && !isp(node->ori_args[i + 1]))
		i++;
	envp = ft_setenv_envp("_", args[i], envp);
	return (dispatch_builtin(args, envp, node));
}

/* dispatch_builtin moved to parser_helpers.c */

char	*get_pwd_for_prompt(char **envp, t_node *node)
{
	char	*pwd;

	if (ft_getenv("PWD", envp))
		pwd = ft_getenv("PWD", envp);
	else
		pwd = node->pwd;
	return (pwd);
}

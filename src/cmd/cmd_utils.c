/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static char	**get_paths(char *path_env, t_node *node)
{
	(void)node;
	if (!path_env || !path_env[0])
		return (NULL);
	return (ft_split(path_env, ':'));
}

static void	handle_slash_command(char **args, char **envp, t_node *node)
{
	if (ft_strchr(args[0], '/'))
	{
		if (!access(args[0], X_OK))
			envp = exec_pipe(args[0], args, envp, node);
		chkdir(args, envp, 1, node);
	}
}

/* From unset.c */
char	**cmd_unset(char **args, char **envp, t_node *node)
{
	int		flag;

	flag = 1;
	if (args[1] && args[1][0] == '-')
	{
		handle_unset_option_error(args);
		return (envp);
	}
	process_unset_args(args, envp, node, &flag);
	return (envp);
}

/* From export.c */
char	**cmd_export(char **args, char **envp, t_node *node)
{
	set_exit_status(EXIT_SUCCESS);
	if (!args[1] || (node->pipe_idx && isp(node->ori_args[1])))
		return (export_print(envp));
	else if (args[1][0] == '-')
	{
		handle_export_option_error(args);
		return (envp);
	}
	process_export_args(args, &envp, node);
	return (envp);
}

void	exec_proc(char **args, char **envp, t_node *node)
{
	char	**paths;
	char	*path_env;

	if (!args[0][0])
		exec_error(args, envp, 0, node);
	checkdot(args, envp, node);
	handle_slash_command(args, envp, node);
	path_env = ft_getenv("PATH", envp);
	paths = get_paths(path_env, node);
	if (!paths || !paths[0])
		exec_nopath(node, args, envp, paths);
	node->i = -1;
	while (paths[++(node->i)])
		exec_proc_loop(paths, args, envp, node);
	exec_error(args, envp, paths, node);
}

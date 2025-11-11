/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 12:47:04 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_builtin_command(char **args)
{
	if (!args || !args[0])
		return (false);
	return (!ft_strncmp(args[0], "cd", 3)
		|| !ft_strncmp(args[0], "echo", 5)
		|| !ft_strncmp(args[0], "env", 4)
		|| !ft_strncmp(args[0], "exit", 5)
		|| !ft_strncmp(args[0], "export", 7)
		|| !ft_strncmp(args[0], "pwd", 4)
		|| !ft_strncmp(args[0], "unset", 6));
}

void	exec_nopath(t_node *node, char **args, char **envp, char **paths)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	exec_error(args, envp, paths, node);
}

/* helpers moved to cmd_exec_helpers_more2.c */

static void	free_split(char **v)
{
	int	i;

	i = 0;
	while (v && v[i])
		free(v[i++]);
	if (v)
		free(v);
}

char	**exec_pipe(char *path, char **args, char **envp, t_node *node)
{
	char	**temp;
	char	**old_envp;
	char	**new_envp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	old_envp = envp;
	new_envp = ft_setenv_envp("_", path, envp);
	if (old_envp != new_envp && old_envp)
		strarrfree(old_envp);
	if (node->pipe_flag)
	{
		temp = split_before_pipe_args(args, node);
		execve(path, temp, new_envp);
		free_split(temp);
	}
	else
		execve(path, args, new_envp);
	if (new_envp != envp && new_envp)
		strarrfree(new_envp);
	return (envp);
}

void	exec_proc_loop2(char **paths, char **args, char **envp, t_node *node)
{
	if (node->redir_flag && isdlr(node->ori_args[0]))
		argu_left_change(args, node);
	if (!access(node->path, X_OK))
	{
		strarrfree(paths);
		exec_pipe(node->path, args, envp, node);
	}
	free(node->path);
	node->path = NULL;
}

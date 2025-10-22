/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:03 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	*build_candidate(char *dir, char *cmd)
{
	const char	*d;
	char		*path;
	size_t		n;

	if (dir && *dir)
		d = dir;
	else
		d = ".";
	n = ft_strlen(d) + ft_strlen(cmd) + 2;
	path = malloc(n);
	if (!path)
		return (NULL);
	ft_strlcpy(path, d, n);
	ft_strlcat(path, "/", n);
	ft_strlcat(path, cmd, n);
	return (path);
}

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
	char	*test;
	char	*test2;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	test = ft_strjoin(node->pwd, "/");
	test2 = ft_strjoin(test, args[0]);
	free(test);
	if (!access(test2, X_OK))
	{
		envp = ft_setenv_envp("_", test2, envp);
		execve(test2, args, envp);
	}
	free(test2);
	exec_error(args, envp, paths, node);
}

char	**exec_pipe(char *path, char **args, char **envp, t_node *node)
{
	char	**temp;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGPIPE, SIG_IGN);
	envp = ft_setenv_envp("_", path, envp);
	if (node->pipe_flag)
	{
		temp = split_before_pipe_args(args, node);
		execve(path, temp, envp);
		free(temp);
	}
	else
		execve(path, args, envp);
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
}

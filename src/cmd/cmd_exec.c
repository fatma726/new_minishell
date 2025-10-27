/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:03 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	exec_check_loop(char **paths, char **args)
{
	size_t	i;
	char	*path;

	i = 0;
	while (paths[i])
	{
		path = build_candidate(paths[i], args[0]);
		if (!path)
		{
			strarrfree(paths);
			exit(EXIT_FAILURE);
		}
		if (!access(path, X_OK))
		{
			free(path);
			return (true);
		}
		free(path);
		i++;
	}
	return (false);
}

bool	exec_check(char **args, char **envp, t_node *node)
{
	char	**paths;
	bool	ret;
	char	*path_env;

	(void)node;
	if (is_builtin_command(args))
		return (true);
	path_env = ft_getenv("PATH", envp);
	if (!path_env || !path_env[0])
		return (false);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (false);
	ret = exec_check_loop(paths, args);
	strarrfree(paths);
	return (ret);
}

static void	handle_signaled_status(int status)
{
    int	sig;

    sig = WTERMSIG(status);
    if (sig == SIGPIPE)
    {
    }
    set_exit_status(128 + sig);
}

void	post_wait_set_status(int pid, int background)
{
	int	status;

	if (background)
	{
		set_exit_status(0);
		return ;
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
		handle_signaled_status(status);
	else if (WIFEXITED(status))
		set_exit_status(WEXITSTATUS(status));
	else
		set_exit_status(0);
}

char	**cmd_exec(char **args, char **envp, t_node *node)
{
	int	pid;
	int	background;
	int	argc;

	background = 0;
	argc = strarrlen(args);
	if (argc > 0 && ft_strncmp(args[argc - 1], "&", 2) == 0)
	{
		background = 1;
		args[argc - 1] = NULL;
	}
	pid = fork();
	if (!pid)
		exec_proc(args, envp, node);
	post_wait_set_status(pid, background);
	return (envp);
}

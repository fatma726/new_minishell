/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static void	cmd_launch(char **args, char **envs, t_node *node)
{
	char	**args_new;

	if (!ft_strncmp(args[1], "-i", 3) && args[2])
		handle_env_i_option(args, envs, node);
	else
	{
		args_new = strarrdup(args + 1);
		envs = find_command(args_new, envs, node);
		strarrfree(args_new);
	}
}

static void	env_error(char *arg)
{
	char	error_msg[15];

	errno = ENOENT;
	ft_strlcpy(error_msg, "minishell: ", 15);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(strerror(errno), STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	set_exit_status(127);
}

static char	**path_check_loop(char **paths, char **args, char **envp, size_t i)
{
	size_t	n;
	char	*path;

	n = ft_strlen(paths[i]) + ft_strlen(args[0]) + 2;
	path = malloc(n);
	ft_strlcpy(path, paths[i], n);
	ft_strlcat(path, "/", n);
	ft_strlcat(path, args[0], n);
	if (!access(path, X_OK))
	{
		strarrfree(paths);
		envp = ft_setenv_envp("_", path, envp);
		free(path);
		return (envp);
	}
	free(path);
	return (envp);
}

static char	**path_check(char **args, char **envp, t_node *node)
{
	size_t	i;
	char	**paths;

	if (node->path_fallback)
		paths = ft_split(node->path_fallback, ':');
	else
		paths = ft_split(ft_getenv("PATH", envp), ':');
	if (!paths || !paths[0])
		return (envp);
	i = 0;
	while (paths[i])
	{
		envp = path_check_loop(paths, args, envp, i);
		if (ft_strncmp(ft_getenv("_", envp), "env", 4))
			return (envp);
		i++;
	}
	return (envp);
}

char	**cmd_env(char **args, char **envs, t_node *node)
{
	int	i;

	if (node->argmode)
		envs = shlvl_mod(-1, envs);
	envs = path_check(args, envs, node);
	if (!ft_strncmp(ft_getenv("_", envs), "env", 4))
		env_error(args[0]);
	else if (args[1] && !node->pipe_flag)
		cmd_launch(args, envs, node);
	else
	{
		i = -1;
		while (envs[++i])
			if (ft_strchr(envs[i], '='))
				ft_putendl_fd(envs[i], STDOUT_FILENO);
		envs = ft_setenv_envp("_", "env", envs);
		set_exit_status(EXIT_SUCCESS);
	}
	if (node->argmode)
		envs = shlvl_mod(1, envs);
	return (envs);
}

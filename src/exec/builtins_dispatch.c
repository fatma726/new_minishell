/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

int	is_builtin_name(const char *name)
{
	if (!name)
		return (0);
	if (!ft_strncmp(name, "echo", 5))
		return (1);
	if (!ft_strncmp(name, "cd", 3))
		return (1);
	if (!ft_strncmp(name, "pwd", 4))
		return (1);
	if (!ft_strncmp(name, "export", 7))
		return (1);
	if (!ft_strncmp(name, "unset", 6))
		return (1);
	if (!ft_strncmp(name, "env", 4))
		return (1);
	if (!ft_strncmp(name, "exit", 5))
		return (1);
	return (0);
}

static void	redirect_stdout_fd(int fd, int *saved_fd)
{
	*saved_fd = -1;
	if (fd >= 0)
	{
		*saved_fd = dup(STDOUT_FILENO);
		if (*saved_fd >= 0)
			dup2(fd, STDOUT_FILENO);
	}
}

static void	restore_stdout_if_needed(int saved_fd)
{
	if (saved_fd >= 0)
	{
		dup2(saved_fd, STDOUT_FILENO);
		close(saved_fd);
	}
}

static int	call_builtin(char **argv, char ***penv)
{
	if (!ft_strncmp(argv[0], "echo", 5))
		return (ft_echo(argv));
	if (!ft_strncmp(argv[0], "pwd", 4))
		return (ft_pwd());
	if (!ft_strncmp(argv[0], "env", 4))
		return (ft_env(*penv));
	if (!ft_strncmp(argv[0], "cd", 3))
		return (ft_cd(argv, penv));
	if (!ft_strncmp(argv[0], "unset", 6))
		return (bi_unset(argv, penv));
	if (!ft_strncmp(argv[0], "export", 7))
		return (bi_export(argv, penv));
	return (1);
}

int	run_builtin_dispatch(char **argv, char ***penv, t_cmd *cmd)
{
	int		saved;
	int		status;
	int		outfd;

	if (!argv || !argv[0] || !penv)
		return (1);
	outfd = -1;
	if (cmd)
		outfd = cmd->outfile;
	redirect_stdout_fd(outfd, &saved);
	status = call_builtin(argv, penv);
	restore_stdout_if_needed(saved);
	if (!ft_strncmp(argv[0], "exit", 5))
		cmd_exit(argv, *penv, NULL);
	return (status);
}

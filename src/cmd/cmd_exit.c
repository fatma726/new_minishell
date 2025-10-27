/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/22 18:35:20 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	handle_exit_message(void)
{
	char	exit_msg[5];

	if (isatty(STDIN_FILENO))
	{
		ft_strlcpy(exit_msg, "exit", 5);
		ft_putendl_fd(exit_msg, STDOUT_FILENO);
	}
}

bool	exit_will_terminate(char **args)
{
	size_t	argc;

	if (!args)
		return (false);
	argc = strarrlen(args);
	if (argc <= 1)
		return (true);
	if (!ft_isalldigit(args[1]))
		return (true);
	if (argc == 2)
		return (true);
	return (false);
}

void	cleanup_child_and_exit(char **args, char **envp, t_node *node)
{
	if (node->pwd)
		free(node->pwd);
	if (node->path_fallback)
		free(node->path_fallback);
	if (node->ori_args)
	{
		strarrfree(node->ori_args);
		node->ori_args = NULL;
	}
	if (args)
		strarrfree(args);
	if (envp)
		strarrfree(envp);
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	exit(get_exit_status());
}

void	cleanup_and_exit(char **args, char **envp, t_node *node)
{
	if (node->pwd)
		free(node->pwd);
	if (node->path_fallback)
		free(node->path_fallback);
	if (node->ori_args)
	{
		strarrfree(node->ori_args);
		node->ori_args = NULL;
	}
	if (args)
		strarrfree(args);
	if (envp)
		strarrfree(envp);
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	clear_history();
	restore_termios();
	maybe_write_exit_file();
	exit(get_exit_status());
}

void	cmd_exit(char **args, char **envp, t_node *node)
{
	bool	should_exit;

	if (!node->exit_flag)
		return ;
	should_exit = true;
	if (strarrlen(args) > 1)
		should_exit = handle_exit_with_args(args);
	else
		set_exit_status(EXIT_SUCCESS);
	if (should_exit && !node->argmode)
		handle_exit_message();
	if (should_exit)
		cleanup_and_exit(args, envp, node);
}

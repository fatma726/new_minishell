/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	backup_restor(t_node *node)
{
	dup2(node->backup_stdout, STDOUT_FILENO);
	dup2(node->backup_stdin, STDIN_FILENO);
}

char	**split_before_pipe_args(char **args, t_node *node)
{
	char	**temp;
	int		i;
	int		j;

	i = -1;
	temp = malloc(((size_t)node->pipe_idx + 1) * sizeof(char *));
	if (!temp)
		exit(EXIT_FAILURE);
	while (++i < node->pipe_idx - 1)
	{
		temp[i] = malloc(ft_strlen(args[i]) + 1);
		if (!temp[i])
		{
			while (--i >= 0)
				free(temp[i]);
			free(temp);
			exit(EXIT_FAILURE);
		}
		j = -1;
		while (args[i][++j])
			temp[i][j] = args[i][j];
		temp[i][j] = '\0';
	}
	temp[i] = NULL;
	return (temp);
}

static void	repeat_exec(char **args, char **envp, t_node *node, int pid)
{
	if (!pid)
		exec_child(args, envp, node);
	else
		run_parent_segment(args, envp, node);
	backup_restor(node);
}

char	**repeat(char **args, char **envp, t_node *node)
{
	int	pid;
	int	redir_err;

	redir_err = prepare_redirections(args, envp, node);
	if (redir_err)
	{
		if (node->pipe_flag)
			node->child_die = 1;
		else
			return (backup_restor(node), close(node->backup_stdout),
				close(node->backup_stdin), envp);
	}
	pid = maybe_setup_pipe(node);
	if (pid < 0)
		return (backup_restor(node), close(node->backup_stdout),
			close(node->backup_stdin), envp);
	if (!node->pipe_flag)
		return (one_commnad(args, envp, node));
	repeat_exec(args, envp, node, pid);
	return (envp);
}

char	**execute(char **args, char **envp, t_node *node)
{
	char	**result;

	if (args && args[0] && !ft_strncmp(args[0], "exit", 5))
	{
		if (exit_will_terminate(args))
		{
			node->exit_flag = 1;
			cmd_exit(args, envp, node);
		}
	}
	result = handle_parentheses(args, envp, node);
	if (result)
		return (result);
	node->backup_stdout = dup(STDOUT_FILENO);
	node->backup_stdin = dup(STDIN_FILENO);
	envp = repeat(args, envp, node);
	backup_restor(node);
	close(node->backup_stdout);
	close(node->backup_stdin);
	return (envp);
}

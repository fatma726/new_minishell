/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

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

void	backup_restor(t_node *node)
{
	dup2(node->backup_stdout, STDOUT_FILENO);
	dup2(node->backup_stdin, STDIN_FILENO);
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

static int	has_bar_error(char **args, t_node *node)
{
	if (args && args[0]
		&& (node->pipe_word_has_bar
			|| (ft_strchr(args[0], '|') != NULL
				&& ft_strncmp(args[0], "|", 2) != 0)))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		set_exit_status_n(node, 127);
		return (1);
	}
	return (0);
}
char	**execute(char **args, char **envp, t_node *node)
{
	if (has_bar_error(args, node))
		return (envp);
	maybe_handle_exit(args, envp, node);
	node->backup_stdout = dup(STDOUT_FILENO);
	node->backup_stdin = dup(STDIN_FILENO);
	envp = repeat(args, envp, node);
	backup_restor(node);
	close(node->backup_stdout);
	close(node->backup_stdin);
	if (node->pipe_word_has_bar)
		set_exit_status_n(node, 127);
	return (envp);
}

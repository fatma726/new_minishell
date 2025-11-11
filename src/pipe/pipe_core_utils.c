/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_core_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

/* moved low-level child helpers to pipe_core_extras.c */

void	process_input_redirections(char **args, char **envp, t_node *node)
{
	int	i;

	i = 0;
	while (i < node->pipe_idx - 1 && !isp(node->ori_args[i]))
	{
		if (islr(node->ori_args[i]) || islrr(node->ori_args[i]))
		{
			left_redir(args, envp, &i, node);
			break ;
		}
		i++;
	}
}

static char	**child_env_transition(char **envp)
{
	char	**old_envp;
	char	**new_envp;

	old_envp = envp;
	new_envp = shlvl_mod(1, envp);
	if (old_envp != new_envp)
		strarrfree(old_envp);
	return (new_envp);
}

static char	**child_env_restore(char **envp)
{
	char	**old_envp;
	char	**final_envp;

	old_envp = envp;
	final_envp = shlvl_mod(-1, envp);
	if (old_envp != final_envp)
		strarrfree(old_envp);
	return (final_envp);
}

void	exec_child_envp_handle(char **args, char **envp, t_node *node)
{
	char	**child_args;
	char	**final_envp;
	int		i;

	child_args = split_before_pipe_args(args, node);
	envp = child_env_transition(envp);
	envp = find_command(child_args, envp, node);
	final_envp = child_env_restore(envp);
	i = 0;
	while (child_args && child_args[i])
		free(child_args[i++]);
	if (child_args)
		free(child_args);
	if (node->path)
	{
		free(node->path);
		node->path = NULL;
	}
	if (node->backup_stdout >= 0)
		close(node->backup_stdout);
	if (node->backup_stdin >= 0)
		close(node->backup_stdin);
	if (final_envp != envp)
		strarrfree(final_envp);
	exit(get_exit_status_n(node));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	handle_signals(t_node *n)
{
	if (get_signal_number() == SIGINT)
	{
		set_exit_status_n(n, 130);
		clear_signal_number();
	}
}

static char	*get_and_process_prompt(char **envp, t_node *n)
{
	const char	*prompt;
	char		*prompt_copy;

	(void)n;
	prompt = ft_getenv("PS1", envp);
	if (!prompt)
		prompt = "minishell🍭$ ";
	prompt_copy = ft_strdup(prompt);
	return (prompt_copy);
}

static char	**main_loop(char **envp, t_node *n)
{
	char	*line;
	char	*prompt;

	handle_signals(n);
	prompt = get_and_process_prompt(envp, n);
	set_interactive_n(n, true);
	line = get_line(prompt, n);
	set_interactive_n(n, false);
	if (prompt)
		free(prompt);
	n->line_nbr++;
	if (!line)
		handle_eof_exit(envp, n);
	envp = process_command(line, envp, n);
	if (isatty(STDIN_FILENO))
	{
		rl_reset_line_state();
	}
	return (envp);
}

static char	**bootstrap_env(char **argv, char **envp, t_node *node)
{
	init_node(node);
	set_exit_status_n(node, 0);
	envp = strarrdup(envp);
	envp = shlvl_plus_plus(setpwd(node, envp));
	envp = ft_setenv_envp("_", argv[0], envp);
	node->path_fallback = NULL;
	node->line_nbr = 0;
	set_signal();
	return (envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_node		node;
	t_runtime	runtime;

	(void)argc;
	ft_bzero(&node, sizeof(t_node));
	ft_bzero(&runtime, sizeof(t_runtime));
	node.rt = &runtime;
	envp = bootstrap_env(argv, envp, &node);
	while (1)
		envp = main_loop(envp, &node);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	handle_signals(void)
{
	if (get_signal_number() == SIGINT)
	{
		set_exit_status(130);
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

	handle_signals();
	prompt = get_and_process_prompt(envp, n);
	set_interactive_mode(true);
	line = get_line(prompt);
	set_interactive_mode(false);
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
	set_exit_status(0);
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

	(void)argc;
	ft_bzero(&node, sizeof(t_node));
	envp = bootstrap_env(argv, envp, &node);
	while (1)
		envp = main_loop(envp, &node);
}

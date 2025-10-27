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
    int sig;

    sig = get_signal_number();
    if (sig == SIGINT)
    {
        /*
        ** SIGINT on empty prompt -> 1
        ** SIGINT during command -> already set to 130 by handler
        */
        /* Only downgrade to 1 if we are truly idle and
        ** no child just reported a signal-based termination (>=128). */
        if (is_interactive_mode() && get_exit_status() < 128)
            set_exit_status(1);
        clear_signal_number();
    }
    else if (sig == SIGQUIT)
    {
        /* SIGQUIT handled in handler during execution; just clear here */
        clear_signal_number();
    }
}

static char	*get_and_process_prompt(char **envp, t_node *n)
{
	const char	*prompt;
	char		*prompt_copy;

	prompt = ft_getenv("PS1", envp);
	if (!prompt)
		prompt = "minishell🍭$ ";
	prompt_copy = ft_strdup(prompt);
	return (expand_prompt(prompt_copy, envp, n));
}

static char	**main_loop(char **envp, t_node *n)
{
	char	*line;
	char	*prompt;

	prompt = get_and_process_prompt(envp, n);
	set_interactive_mode(true);
	handle_signals();
	line = get_line(prompt);
	set_interactive_mode(false);
	if (prompt)
		free(prompt);
	n->line_nbr++;
	if (!line)
		handle_eof_exit(envp, n);
	envp = process_command(line, envp, n);
	set_interactive_mode(true);
	return (envp);
}

static char	**bootstrap_env(char **argv, char **envp, t_node *node)
{
	init_node(node);
	set_exit_status(0);
	envp = strarrdup(envp);
	envp = shlvl_plus_plus(setpwd(node, envp));
	envp = ft_setenv_envp("OLDPWD", NULL, envp);
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

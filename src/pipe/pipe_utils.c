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

static int debug_enabled(void)
{
    const char *d = getenv("DEBUG");
    return (d && d[0] == '1');
}

static void debug_log_args(const char *tag, char **args)
{
    int i = 0;
    if (!debug_enabled()) return;
    ft_putstr_fd("DBG ", STDERR_FILENO);
    ft_putstr_fd(tag, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    while (args && args[i])
    {
        ft_putstr_fd(args[i], STDERR_FILENO);
        if (args[i + 1]) ft_putstr_fd(" ", STDERR_FILENO);
        i++;
    }
    ft_putstr_fd("\n", STDERR_FILENO);
}

static void debug_log_status(const char *tag, int status)
{
    char *n;
    if (!debug_enabled()) return;
    ft_putstr_fd("DBG ", STDERR_FILENO);
    ft_putstr_fd(tag, STDERR_FILENO);
    ft_putstr_fd(": ", STDERR_FILENO);
    n = ft_itoa(status);
    if (n)
    {
        ft_putstr_fd(n, STDERR_FILENO);
        free(n);
    }
    ft_putstr_fd("\n", STDERR_FILENO);
}

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
    debug_log_args("parsed", args);
    /* If a command word contains an unquoted '|' (e.g., ech|o) and we
     * truncated the pipeline, treat it as command-not-found and do not
     * execute any further segments. */
    if (args && args[0] && (node->pipe_word_has_bar ||
        (ft_strchr(args[0], '|') != NULL && ft_strncmp(args[0], "|", 2) != 0)))
    {
        ft_putstr_fd("minishell: ", STDERR_FILENO);
        ft_putstr_fd(args[0], STDERR_FILENO);
        ft_putstr_fd(": command not found\n", STDERR_FILENO);
        set_exit_status(127);
        debug_log_status("early_guard", 127);
        return (envp);
    }
    if (args && args[0] && !ft_strncmp(args[0], "exit", 5))
	{
		if (exit_will_terminate(args))
		{
			node->exit_flag = 1;
			cmd_exit(args, envp, node);
		}
	}
	node->backup_stdout = dup(STDOUT_FILENO);
	node->backup_stdin = dup(STDIN_FILENO);
    envp = repeat(args, envp, node);
    debug_log_status("final_status", get_exit_status());
    backup_restor(node);
    close(node->backup_stdout);
    close(node->backup_stdin);
    if (node->pipe_word_has_bar)
        set_exit_status(127);
    return (envp);
}

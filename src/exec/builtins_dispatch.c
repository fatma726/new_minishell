/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"
#include <unistd.h>

static int names_eq(const char *a, const char *b)
{
    return (a && b && !ft_strncmp(a, b, ft_strlen(b) + 1));
}

int is_builtin_name(const char *name)
{
    if (!name)
        return (0);
    return (names_eq(name, "echo") || names_eq(name, "cd")
        || names_eq(name, "pwd") || names_eq(name, "export")
        || names_eq(name, "unset") || names_eq(name, "env")
        || names_eq(name, "exit"));
}

static void redirect_stdout_if_needed(t_node *node, int *saved_fd)
{
    *saved_fd = -1;
    if (node && node->redir_flag && node->redir_fd >= 0)
    {
        *saved_fd = dup(STDOUT_FILENO);
        if (*saved_fd >= 0)
            dup2(node->redir_fd, STDOUT_FILENO);
    }
}

static void restore_stdout_if_needed(int saved_fd)
{
    if (saved_fd >= 0)
    {
        dup2(saved_fd, STDOUT_FILENO);
        close(saved_fd);
    }
}

int run_builtin_dispatch(char **argv, char ***penv, t_node *node)
{
    int saved;

    if (!argv || !argv[0] || !penv)
        return (1);
    redirect_stdout_if_needed(node, &saved);
    if (names_eq(argv[0], "echo"))
        cmd_echo(argv, node);
    else if (names_eq(argv[0], "pwd"))
        cmd_pwd(node);
    else if (names_eq(argv[0], "env"))
        *penv = cmd_env(argv, *penv, node);
    else if (names_eq(argv[0], "cd"))
        *penv = cmd_cd(argv, *penv, node);
    else if (names_eq(argv[0], "unset"))
        *penv = cmd_unset(argv, *penv, node);
    else if (names_eq(argv[0], "export"))
        *penv = cmd_export(argv, *penv, node);
    restore_stdout_if_needed(saved);
    if (names_eq(argv[0], "exit"))
        cmd_exit(argv, *penv, node);
    return (get_exit_status());
}


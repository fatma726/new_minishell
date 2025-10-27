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

#include "minishell.h"
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

static void redirect_stdout_fd(int fd, int *saved_fd)
{
    *saved_fd = -1;
    if (fd >= 0)
    {
        *saved_fd = dup(STDOUT_FILENO);
        if (*saved_fd >= 0)
            dup2(fd, STDOUT_FILENO);
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

/* New API requested: uses t_cmd and returns via last_status */
int run_builtin_dispatch(char **argv, char ***penv, t_cmd *cmd, int *last_status)
{
    int saved;
    int status;

    if (!argv || !argv[0] || !penv)
        return (1);
    redirect_stdout_fd(cmd ? cmd->outfile : -1, &saved);
    status = 1;
    if (names_eq(argv[0], "echo"))
        status = ft_echo(argv);
    else if (names_eq(argv[0], "pwd"))
        status = ft_pwd();
    else if (names_eq(argv[0], "env"))
        status = ft_env(*penv);
    else if (names_eq(argv[0], "cd"))
        status = ft_cd(argv, penv);
    else if (names_eq(argv[0], "unset"))
        status = bi_unset(argv, penv);
    else if (names_eq(argv[0], "export"))
        status = bi_export(argv, penv);
    restore_stdout_if_needed(saved);
    if (names_eq(argv[0], "exit"))
    {
        /* Ensure stdout restored before exit behavior */
        cmd_exit(argv, *penv, NULL);
    }
    if (last_status)
        *last_status = status;
    return (status);
}

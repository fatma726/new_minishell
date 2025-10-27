/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <sys/types.h>
#include <stddef.h>

#include "mandatory.h"

typedef struct s_token t_token;

typedef struct s_cmd
{
    char            **argv;
    int             infile;
    int             outfile;
    struct s_cmd    *next;
}   t_cmd;

typedef struct s_token
{
    char            *str;
    int             type;
    struct s_token  *prev;
    struct s_token  *next;
}   t_token;

typedef struct s_shell_state
{
    int         last_status;
    char        **env;
    t_token     *tokens;
    pid_t       active_child;
}   t_shell_state;

/* builtins dispatcher using t_cmd */
int is_builtin_name(const char *name);
int run_builtin_dispatch(char **argv, char ***penv, t_cmd *cmd, int *last_status);

#endif

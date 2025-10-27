/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_consolidated.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:04 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	handle_export_option_error(char **args)
{
	char	export_error[25];
	char	invalid_option[50];
	char	usage[50];

	ft_strlcpy(export_error, "minishell: export: ", 25);
	ft_putstr_fd(export_error, STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_strlcpy(invalid_option, ": invalid option\nexport: usage: export ", 50);
	ft_putstr_fd(invalid_option, STDERR_FILENO);
	ft_strlcpy(usage, "[-fn] [name[=value] ...] or export -p\n", 50);
	ft_putstr_fd(usage, STDERR_FILENO);
	set_exit_status(2);
}

void	process_export_args(char **args, char ***envp, t_node *node)
{
	int		i;
	bool	has_error;

	i = 0;
	has_error = false;
	while (args[++i] && (!node->pipe_idx || i + 1 < node->pipe_idx))
	{
		if (!validate_export_identifier(args[i]))
		{
			if (!has_error)
				print_invalid_identifier_error(args[i]);
			has_error = true;
			continue ;
		}
		(void)process_export_arg(args[i], envp, node);
	}
	if (has_error)
		set_exit_status(EXIT_FAILURE);
}

void	handle_env_i_option(char **args, char **envs, t_node *node)
{
	char	**args_new;
	char	**envp;

	envp = malloc(sizeof(char *));
	if (!envp)
		return ;
	envp[0] = NULL;
	envp = ft_setenv_envp("PATH", ft_getenv("PATH", envs), envp);
	args_new = strarrdup(args + 2);
	if (!args_new)
	{
		strarrfree(envp);
		return ;
	}
	envp = find_command(args_new, envp, node);
	strarrfree(args_new);
	strarrfree(envp);
}

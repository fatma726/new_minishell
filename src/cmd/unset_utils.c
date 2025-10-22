/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	handle_invalid_identifier(char *arg)
{
	char	unset_error[35];
	char	not_valid[30];

	ft_strlcpy(unset_error, "minishell: unset: `", 25);
	ft_putstr_fd(unset_error, STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_strlcpy(not_valid, "': not a valid identifier\n", 30);
	ft_putstr_fd(not_valid, STDERR_FILENO);
}

void	process_unset_args(char **args, char **envp, t_node *node, int *flag)
{
	int		i;
	bool	has_error;

	i = 0;
	has_error = false;
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			handle_invalid_identifier(args[i]);
			has_error = true;
		}
		else
			envp = delete_env(args[i], envp, node, flag);
	}
	if (has_error)
		set_exit_status(EXIT_FAILURE);
	else if (*flag != 1)
		set_exit_status(EXIT_FAILURE);
	else
		set_exit_status(EXIT_SUCCESS);
}

void	handle_unset_option_error(char **args)
{
	char	unset_error[25];
	char	invalid1[35];
	char	invalid2[30];
	char	usage[15];

	ft_strlcpy(unset_error, "minishell: unset: ", 25);
	ft_putstr_fd(unset_error, STDERR_FILENO);
	ft_putstr_fd(args[1], STDERR_FILENO);
	ft_strlcpy(invalid1, ": invalid option\nunset: usage: unset ", 35);
	ft_putstr_fd(invalid1, STDERR_FILENO);
	ft_strlcpy(invalid2, "[-f] [-v] [-n] ", 30);
	ft_putstr_fd(invalid2, STDERR_FILENO);
	ft_strlcpy(usage, "[name ...]\n", 15);
	ft_putstr_fd(usage, STDERR_FILENO);
	set_exit_status(2);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static void	handle_shell_level_warning(int level)
{
	char	warning_msg[50];
	char	reset_msg[30];

	ft_strlcpy(warning_msg, "minishell: warning: shell level (", 50);
	ft_strlcpy(reset_msg, ") too high, resetting to 1", 30);
	ft_putstr_fd(warning_msg, STDERR_FILENO);
	ft_putnbr_fd(level, STDERR_FILENO);
	ft_putendl_fd(reset_msg, STDERR_FILENO);
}

static char	*determine_new_shlvl(char *str)
{
	int	current_level;
	int	new_level;

	if (!str || !ft_isalldigit(str))
		return (ft_itoa(1));
	current_level = ft_atoi(str);
	new_level = current_level + 1;
	if (new_level >= 1000)
	{
		handle_shell_level_warning(new_level);
		return (ft_itoa(1));
	}
	if (new_level <= 0)
		return (ft_itoa(0));
	return (ft_itoa(new_level));
}

char	**shlvl_plus_plus(char **envp)
{
	char	*str;
	char	*new_str;

	str = ft_getenv("SHLVL", envp);
	new_str = determine_new_shlvl(str);
	if (!new_str)
	{
		strarrfree(envp);
		exit(EXIT_FAILURE);
	}
	envp = ft_setenv_envp("SHLVL", new_str, envp);
	free(new_str);
	return (envp);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_print.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:04 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static void	print_declare_line(char *entry)
{
	size_t	i;
	char	*name;
	char	*value;

	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	name = ft_substr(entry, 0, i);
	if (entry[i] == '=')
		value = ft_strdup(entry + i + 1);
	else
		value = NULL;
	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	ft_putstr_fd(name, STDOUT_FILENO);
	if (value)
	{
		ft_putstr_fd("=\"", STDOUT_FILENO);
		print_escaped_value(value);
		ft_putstr_fd("\"", STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
	free(name);
	if (value)
		free(value);
}

static char	*find_next_lowest(char **envp, char *prev, size_t idx)
{
	int		i;
	char	*lowest;

	lowest = NULL;
	i = -1;
	while (envp[++i])
	{
		if ((!lowest || ft_strncmp(envp[i], lowest, ft_strlen(envp[i])) < 0)
			&& (!idx || ft_strncmp(envp[i], prev, ft_strlen(envp[i])) > 0))
			lowest = envp[i];
	}
	return (lowest);
}

static void	print_sorted_env(char **envp)
{
	size_t	idx;
	char	*lowest;
	char	*prev;

	idx = 0;
	prev = NULL;
	while (idx < strarrlen(envp))
	{
		lowest = find_next_lowest(envp, prev, idx);
		if (lowest && ft_strncmp(lowest, "_=", 2))
			print_declare_line(lowest);
		prev = lowest;
		idx++;
	}
}

char	**export_print(char **envp)
{
	print_sorted_env(envp);
	fflush(stdout);
	return (envp);
}

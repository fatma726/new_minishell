/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_export_update.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

void	handle_path_update(char *arg, t_node *node)
{
	if (!ft_strncmp(arg, "PATH=", 5) && node->path_fallback)
		free(node->path_fallback);
}

char	**handle_env_update(char *arg, char **envp, char *name, int j)
{
	char	*tmp;

	if (!ft_getenv(name, envp))
		return (ft_setenv_envp(name, arg + j + 1, envp));
	else if (j > 0 && arg[j - 1] == '+')
	{
		tmp = ft_strjoin(ft_getenv(name, envp), arg + j + 1);
		envp = ft_setenv_envp(name, tmp, envp);
		free(tmp);
		return (envp);
	}
	else
		return (ft_setenv_envp(name, arg + j + 1, envp));
}

bool	validate_export_identifier(char *arg)
{
	int	j;

	j = 0;
	if (!is_valid_identifier_start(arg[0]))
		return (false);
	while (arg[j] && arg[j] != '=')
	{
		if (arg[j] == '+')
		{
			if (arg[j + 1] == '=')
				break ;
			return (false);
		}
		if (!is_valid_identifier_char(arg[j]))
			return (false);
		j++;
	}
	return (true);
}

bool	process_export_arg(char *arg, char ***envp, t_node *node)
{
	int		j;
	char	*name;
	char	*tmp;

	if (!validate_export_identifier(arg))
	{
		print_invalid_identifier_error(arg);
		return (false);
	}
	j = 0;
	while (arg[j] && arg[j] != '=')
		j++;
	tmp = ft_substr(arg, 0, (size_t)j);
	name = ft_strtrim(tmp, "+");
	free(tmp);
	handle_path_update(arg, node);
	if (arg[j] == '=')
		*envp = handle_env_update(arg, *envp, name, j);
	free(name);
	return (true);
}

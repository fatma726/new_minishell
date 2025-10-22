/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0] || ft_isdigit(str[0]))
		return (false);
	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	find_envkey(char *str, char *envp)
{
	int	i;

	i = 0;
	while (str[i] && envp[i] && str[i] == envp[i] && envp[i] != '=')
		i++;
	return (str[i] == '\0' && (envp[i] == '=' || envp[i] == '\0'));
}

static void	clear_path_fallback_if(char *str, t_node *node)
{
	if (!ft_strncmp(str, "PATH", 5) && node->path_fallback)
	{
		free(node->path_fallback);
		node->path_fallback = NULL;
	}
}

static void	remove_env_entry(char **envp, int i, int last)
{
	free(envp[i]);
	if (i != last - 1)
	{
		envp[i] = envp[last - 1];
		envp[last - 1] = NULL;
	}
	else
	{
		envp[i] = NULL;
	}
}

char	**delete_env(char *str, char **envp, t_node *node, int *flag)
{
	int	i;
	int	last;

	last = (int)strarrlen(envp);
	if (last < 1)
		return (envp);
	clear_path_fallback_if(str, node);
	i = -1;
	while (envp[++i])
	{
		if (find_envkey(str, envp[i]))
		{
			remove_env_entry(envp, i, last);
			*flag = 1;
			return (envp);
		}
	}
	*flag = 1;
	return (envp);
}

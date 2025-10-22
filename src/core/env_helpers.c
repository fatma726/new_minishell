/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static char	*build_env_pair_for_envp(const char *name, const char *value)
{
	char	*str;
	size_t	n;

	if (!value)
		return (ft_strdup(name));
	n = ft_strlen(name) + ft_strlen(value) + 2;
	str = malloc(n);
	if (!str)
		return (NULL);
	ft_strlcpy(str, name, n);
	ft_strlcat(str, "=", n);
	ft_strlcat(str, value, n);
	return (str);
}

static int	find_env_index_local(const char *name, char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && (ft_strncmp(envp[i], name, ft_strlen(name))
			|| (envp[i][ft_strlen(name)] != '=' && envp[i][ft_strlen(name)])))
		i++;
	return (i);
}

char	**ft_setenv_envp(const char *name, const char *value, char **envp)
{
	int		i;
	char	*str;

	if (!name || !*name || ft_strchr(name, '='))
		return (envp);
	str = build_env_pair_for_envp(name, value);
	if (!str)
		exit(EXIT_FAILURE);
	i = find_env_index_local(name, envp);
	if (envp[i])
	{
		free(envp[i]);
		envp[i] = str;
	}
	else
		envp = strarradd_take(envp, str);
	return (envp);
}

static bool	env_has_key_any(char **envp, const char *name)
{
	int			i;
	size_t		len;

	len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (!ft_strncmp(envp[i], name, len)
			&& (envp[i][len] == '=' || envp[i][len] == '\0'))
			return (true);
		i++;
	}
	return (false);
}

char	**ensure_oldpwd_export(char **envp)
{
	if (!env_has_key_any(envp, "OLDPWD"))
		envp = ft_setenv_envp("OLDPWD", NULL, envp);
	return (envp);
}

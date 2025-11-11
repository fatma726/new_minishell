/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helpers_oldpwd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

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

static void	replace_envp(char ***dst, char **newv)
{
	if (*dst && *dst != newv)
		strarrfree(*dst);
	*dst = newv;
}

char	**ensure_oldpwd_export(char **envp)
{
	char	**new_envp;

	if (!env_has_key_any(envp, "OLDPWD"))
	{
		new_envp = ft_setenv_envp("OLDPWD", NULL, envp);
		replace_envp(&envp, new_envp);
	}
	return (envp);
}

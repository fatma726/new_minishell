/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_access.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	key_equal(const char *entry, const char *key)
{
	size_t	klen;

	klen = ft_strlen(key);
	return (!ft_strncmp(entry, key, klen)
		&& (entry[klen] == '=' || entry[klen] == '\0'));
}

int	env_find_index(char **env, const char *key)
{
	int	i;

	if (!env || !key)
		return (-1);
	i = 0;
	while (env[i])
	{
		if (key_equal(env[i], key))
			return (i);
		i++;
	}
	return (-1);
}

static char	*join_kv(const char *key, const char *value)
{
	char	*s;
	size_t	n;

	n = ft_strlen(key) + 1 + ft_strlen(value) + 1;
	s = malloc(n);
	if (!s)
		return (NULL);
	s[0] = '\0';
	ft_strlcat(s, key, n);
	ft_strlcat(s, "=", n);
	ft_strlcat(s, value, n);
	return (s);
}

int	env_set_kv(char ***penv, const char *key, const char *value)
{
	int		idx;
	char	*kv;

	if (!penv || !key || !value)
		return (0);
	kv = join_kv(key, value);
	if (!kv)
		return (0);
	idx = env_find_index(*penv, key);
	if (idx >= 0)
	{
		free((*penv)[idx]);
		(*penv)[idx] = kv;
		return (1);
	}
	*penv = strarradd_take(*penv, kv);
	return (*penv != NULL);
}

int	env_unset_key(char ***penv, const char *key)
{
	int	idx;
	int	j;

	if (!penv || !*penv || !key)
		return (0);
	idx = env_find_index(*penv, key);
	if (idx < 0)
		return (1);
	free((*penv)[idx]);
	j = idx;
	while ((*penv)[j])
	{
		(*penv)[j] = (*penv)[j + 1];
		j++;
	}
	return (1);
}

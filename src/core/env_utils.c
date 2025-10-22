/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:06 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static char	**handle_pwd_error(t_node *node, char **envp, char *curdir)
{
	free(node->pwd);
	node->pwd = curdir;
	envp = ft_setenv_envp("PWD", node->pwd, envp);
	return (envp);
}

static char	**handle_pwd_change(t_node *node, char **envp, char *curdir[2])
{
	size_t	n;

	if (ft_strlen(curdir[0]) < ft_strlen(curdir[1]))
		n = ft_strlen(curdir[0]) + 1;
	else
		n = ft_strlen(curdir[1]) + 1;
	if (ft_strncmp(curdir[0], curdir[1], n))
	{
		free(node->pwd);
		node->pwd = curdir[0];
		envp = ft_setenv_envp("PWD", node->pwd, envp);
		chdir(node->pwd);
	}
	else
		free(curdir[0]);
	free(curdir[1]);
	return (envp);
}

char	**setpwd(t_node *node, char **envp)
{
	char	*curdir[2];
	char	*pwd_env;

	curdir[0] = getcwd(0, 0);
	pwd_env = ft_getenv("PWD", envp);
	if (pwd_env)
		node->pwd = ft_strdup(pwd_env);
	else
		node->pwd = NULL;
	if (!node->pwd || chdir(node->pwd) == -1)
		return (handle_pwd_error(node, envp, curdir[0]));
	curdir[1] = getcwd(0, 0);
	return (handle_pwd_change(node, envp, curdir));
}

char	**shlvl_mod(int mod, char **envp)
{
	int		newval;
	char	*tmp;
	char	*shlvl_env;

	shlvl_env = ft_getenv("SHLVL", envp);
	if (shlvl_env)
		newval = ft_atoi(shlvl_env) + mod;
	else
		newval = 1 + mod;
	if (newval > 1000)
		newval = 1;
	if (newval < 0)
		newval = 0;
	tmp = ft_itoa(newval);
	envp = ft_setenv_envp("SHLVL", tmp, envp);
	free(tmp);
	return (envp);
}

char	*ft_getenv(const char *name, char **envp)
{
	int		i;
	size_t	len;

	i = 0;
	len = ft_strlen(name);
	while (envp[i] && (ft_strncmp(envp[i], name, len)
			|| (envp[i][len] != '=' && envp[i][len] != '\0')))
		i++;
	if (!envp[i] || !ft_strchr(envp[i], '='))
		return (NULL);
	return (envp[i] + len + 1);
}

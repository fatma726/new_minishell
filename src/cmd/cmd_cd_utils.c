/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:44:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static char	*join_paths(char *pwd, char *path)
{
	char	*temp;
	char	*new_pwd;

	if (pwd[ft_strlen(pwd) - 1] == '/')
		new_pwd = ft_strjoin(pwd, path);
	else
	{
		temp = ft_strjoin(pwd, "/");
		new_pwd = ft_strjoin(temp, path);
		free(temp);
	}
	return (new_pwd);
}

static bool	handle_relative_path(char *path, t_node *node)
{
	char	*cwd;
	char	*new_pwd;

	if (node->pwd && ft_strlen(node->pwd) > 0)
	{
		new_pwd = join_paths(node->pwd, path);
		free(node->pwd);
		node->pwd = new_pwd;
	}
	else
	{
		cwd = getcwd(NULL, 0);
		if (cwd)
		{
			free(node->pwd);
			node->pwd = ft_strdup(cwd);
			free(cwd);
		}
	}
	return (false);
}

bool	update_pwd(char *path, t_node *node)
{
	char	*resolved_path;

	if (path[0] == '/')
	{
		free(node->pwd);
		node->pwd = ft_strdup(path);
	}
	else
	{
		resolved_path = getcwd(NULL, 0);
		if (resolved_path)
		{
			free(node->pwd);
			node->pwd = ft_strdup(resolved_path);
			free(resolved_path);
		}
		else
			return (handle_relative_path(path, node));
	}
	return (false);
}

void	handle_cd_arguments(char **args, char **envp, t_node *node, int offset)
{
	char	*home_path;

	if (!args[1 + offset])
	{
		free(node->pwd);
		home_path = ft_getenv("HOME", envp);
		if (!home_path)
		{
			ft_putstr_fd("cd: HOME not set\n", 2);
			node->pwd = NULL;
			return ;
		}
		node->pwd = ft_strdup(home_path);
	}
	else if (!ft_strncmp(args[1 + offset], "-", 2))
	{
	}
	else
	{
	}
}

char	**execute_cd(char **args, char **envp, t_node *node, int offset)
{
	char	*old_pwd;

	old_pwd = ft_strdup(ft_getenv("PWD", envp));
	if (checks(args, envp, node, offset))
	{
		free(old_pwd);
		return (envp);
	}
	handle_cd_arguments(args, envp, node, offset);
	envp = ft_setenv_envp("OLDPWD", old_pwd, envp);
	envp = ft_setenv_envp("PWD", node->pwd, envp);
	free(old_pwd);
	return (envp);
}

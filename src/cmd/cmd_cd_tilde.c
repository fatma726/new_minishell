/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_cd_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/13 17:40:50 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/mandatory.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

char	*expand_tilde(char *path, char **envp)
{
	char	*home;
	char	*expanded_path;
	size_t	home_len;
	size_t	path_len;

	if (!path || path[0] != '~')
		return (strdup(path));
	home = ft_getenv("HOME", envp);
	if (!home)
		return (strdup(path));
	home_len = strlen(home);
	path_len = strlen(path);
	expanded_path = malloc(home_len + path_len);
	if (!expanded_path)
		return (strdup(path));
	strcpy(expanded_path, home);
	strcat(expanded_path, path + 1);
	return (expanded_path);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_helpers_more2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*build_candidate(char *dir, char *cmd)
{
	const char	*d;
	char		*path;
	size_t		n;

	if (dir && *dir)
		d = dir;
	else
		d = ".";
	n = ft_strlen(d) + ft_strlen(cmd) + 2;
	path = malloc(n);
	if (!path)
		return (NULL);
	ft_strlcpy(path, d, n);
	ft_strlcat(path, "/", n);
	ft_strlcat(path, cmd, n);
	return (path);
}

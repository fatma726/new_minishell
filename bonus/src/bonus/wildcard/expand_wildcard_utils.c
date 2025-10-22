/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

void	cleanup_and_return(char **newargs, int j, char **args)
{
	while (j > 0)
		free(newargs[--j]);
	free(newargs);
	strarrfree(args);
}

int	process_wildcard_pattern(char **newargs, int *j, char *pattern)
{
	int	matches;

	matches = expand_pattern(newargs, j, pattern);
	if (matches == -1)
		return (-1);
	if (matches == 0)
	{
		newargs[*j] = ft_strdup(pattern);
		if (!newargs[*j])
			return (-1);
		(*j)++;
	}
	return (0);
}

int	process_regular_arg(char **newargs, int *j, char *arg)
{
	newargs[*j] = ft_strdup(arg);
	if (!newargs[*j])
		return (-1);
	(*j)++;
	return (0);
}

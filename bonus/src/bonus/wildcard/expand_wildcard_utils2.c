/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils2.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static int	process_directory_entries(DIR *dir, char **matches, char *pattern)
{
	struct dirent	*dr;
	int				i;

	i = 0;
	dr = readdir(dir);
	while (dr != NULL)
	{
		if (process_directory_entry(dr, pattern))
		{
			matches[i] = ft_strdup(dr->d_name);
			if (!matches[i])
			{
				cleanup_matches(matches, i, dir);
				return (0);
			}
			i++;
		}
		dr = readdir(dir);
	}
	matches[i] = NULL;
	return (1);
}

int	collect_matches(char **matches, char *pattern)
{
	DIR	*dir;

	dir = opendir(".");
	if (!dir)
		return (0);
	if (!process_directory_entries(dir, matches, pattern))
		return (0);
	closedir(dir);
	return (1);
}

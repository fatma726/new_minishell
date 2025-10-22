/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"
#include "../../../../include/mandatory.h"

int	process_directory_entry(struct dirent *dr, char *pattern)
{
	if (dr->d_name[0] == '.' && pattern[0] != '.')
		return (0);
	return (matches_pattern(dr->d_name, pattern));
}

/*
** We mark arguments that originate from wildcard expansion so that
** later quote-stripping does NOT remove literal quote characters
** inside actual filenames (e.g., files named " or '). We prefix
** the argument with WILDMARK and strip it in rm_quotes.c without
** altering the content.
*/
int	add_matching_file(char **newargs, int *j, char *filename)
{
	size_t	len;
	char	*marked;

	if (!newargs || !filename)
		return (0);
	len = ft_strlen(filename);
	marked = (char *)malloc(len + 2);
	if (!marked)
		return (0);
	marked[0] = (char)WILDMARK;
	ft_memcpy(marked + 1, filename, len + 1);
	newargs[*j] = marked;
	(*j)++;
	return (1);
}

void	cleanup_matches(char **matches, int i, DIR *dir)
{
	while (i > 0)
		free(matches[--i]);
	free(matches);
	closedir(dir);
}

int	count_matching_files(char *pattern)
{
	DIR				*dir;
	struct dirent	*dr;
	int				count;

	dir = opendir(".");
	if (!dir)
		return (0);
	count = 0;
	dr = readdir(dir);
	while (dr != NULL)
	{
		if (process_directory_entry(dr, pattern))
			count++;
		dr = readdir(dir);
	}
	closedir(dir);
	return (count);
}

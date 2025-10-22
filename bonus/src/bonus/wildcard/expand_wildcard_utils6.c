/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils6.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:49:44 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static int	expand_pattern_simple(char **newargs, int *j, char *pattern)
{
	int		count;
	char	**matches;

	count = count_matching_files(pattern);
	if (count == 0)
		return (0);
	matches = malloc(sizeof(char *) * ((size_t)count + 1));
	if (!matches)
		return (0);
	if (!collect_matches(matches, pattern))
		return (0);
	sort_matches(matches);
	if (!add_sorted_matches(newargs, j, matches))
		return (0);
	free(matches);
	return (count);
}

int	prepare_pattern_path(char *pattern, char **dir_path, char **file_pattern)
{
	char	*last_slash;

	last_slash = ft_strrchr(pattern, '/');
	if (last_slash == NULL)
		return (0);
	*dir_path = ft_substr(pattern, 0, last_slash - pattern);
	*file_pattern = ft_strdup(last_slash + 1);
	if (!*dir_path || !*file_pattern)
	{
		free(*dir_path);
		free(*file_pattern);
		return (0);
	}
	return (1);
}

int	process_pattern_with_path(char **newargs, int *j, char *dir_path,
		char *file_pattern)
{
	int		count;
	char	**matches;

	count = count_matches_in_dir(dir_path, file_pattern);
	if (count == 0)
		return (0);
	matches = malloc(sizeof(char *) * ((size_t)count + 1));
	if (!matches)
		return (0);
	if (!collect_matches_with_path(matches, dir_path, file_pattern))
	{
		free(matches);
		return (0);
	}
	sort_matches(matches);
	if (!add_sorted_matches(newargs, j, matches))
	{
		free(matches);
		return (0);
	}
	free(matches);
	return (count);
}

int	expand_pattern_original(char **newargs, int *j, char *pattern)
{
	char	*dir_path;
	char	*file_pattern;

	if (!prepare_pattern_path(pattern, &dir_path, &file_pattern))
		return (expand_pattern_simple(newargs, j, pattern));
	if (!process_pattern_with_path(newargs, j, dir_path, file_pattern))
	{
		free(dir_path);
		free(file_pattern);
		return (0);
	}
	free(dir_path);
	free(file_pattern);
	return (1);
}

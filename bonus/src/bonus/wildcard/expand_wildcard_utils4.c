/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:11:01 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static int	create_full_path(char **matches, int i, char *dir_path,
		char **full_path)
{
	size_t	path_len;
	size_t	filename_len;

	path_len = ft_strlen(dir_path);
	filename_len = ft_strlen(matches[i]);
	*full_path = malloc(path_len + filename_len + 2);
	if (!*full_path)
		return (0);
	ft_memcpy(*full_path, dir_path, path_len);
	(*full_path)[path_len] = '/';
	ft_memcpy(*full_path + path_len + 1, matches[i], filename_len + 1);
	(*full_path)[path_len + filename_len + 1] = '\0';
	return (1);
}

static int	process_single_match(t_match_data *data, int i)
{
	char	*full_path;

	if (!create_full_path(data->matches, i, data->dir_path, &full_path))
	{
		while (data->matches[i])
			free(data->matches[i++]);
		free(data->matches);
		return (0);
	}
	if (!add_matching_file(data->newargs, data->j, full_path))
	{
		free(full_path);
		while (data->matches[i])
			free(data->matches[i++]);
		free(data->matches);
		return (0);
	}
	free(full_path);
	free(data->matches[i]);
	return (1);
}

static int	process_matches_with_path(char **newargs, int *j, char **matches,
		char *dir_path)
{
	t_match_data	data;
	int				i;

	data.newargs = newargs;
	data.j = j;
	data.matches = matches;
	data.dir_path = dir_path;
	i = 0;
	while (matches && matches[i])
	{
		if (!process_single_match(&data, i))
			return (0);
		i++;
	}
	return (1);
}

int	add_sorted_matches_with_path(char **newargs, int *j, char **matches,
		char *dir_path)
{
	return (process_matches_with_path(newargs, j, matches, dir_path));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:11:01 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

int	count_matches_in_dir(char *dir_path, char *file_pattern)
{
	DIR				*dir;
	struct dirent	*dr;
	int				count;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	count = 0;
	dr = readdir(dir);
	while (dr != NULL)
	{
		if (process_directory_entry(dr, file_pattern))
			count++;
		dr = readdir(dir);
	}
	closedir(dir);
	return (count);
}

static int	process_single_file_match(t_match_data *data, int i,
		struct dirent *dr, DIR *dir)
{
	char	*temp;

	data->matches[i] = ft_strjoin(data->dir_path, "/");
	if (!data->matches[i])
	{
		cleanup_matches(data->matches, i, dir);
		return (0);
	}
	temp = ft_strjoin(data->matches[i], dr->d_name);
	free(data->matches[i]);
	data->matches[i] = temp;
	if (!data->matches[i])
	{
		cleanup_matches(data->matches, i, dir);
		return (0);
	}
	return (1);
}

int	collect_matches_with_path(char **matches, char *dir_path,
		char *file_pattern)
{
	DIR				*dir;
	struct dirent	*dr;
	int				i;
	t_match_data	data;

	dir = opendir(dir_path);
	if (!dir)
		return (0);
	data.matches = matches;
	data.dir_path = dir_path;
	i = 0;
	dr = readdir(dir);
	while (dr != NULL)
	{
		if (process_directory_entry(dr, file_pattern))
		{
			if (!process_single_file_match(&data, i, dr, dir))
				return (0);
			i++;
		}
		dr = readdir(dir);
	}
	closedir(dir);
	matches[i] = NULL;
	return (1);
}

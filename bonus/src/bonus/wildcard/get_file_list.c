/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_list.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/mandatory.h"

char	**get_file_list(bool hidden)
{
	DIR				*dir;
	struct dirent	*dr;
	char			**files;

	dir = opendir(".");
	dr = readdir(dir);
	while (hidden && dr && (dr->d_name[0] != '.' || !ft_strncmp(dr->d_name, ".",
				2) || !ft_strncmp(dr->d_name, "..", 3)))
		dr = readdir(dir);
	while (!hidden && dr && dr->d_name[0] == '.')
		dr = readdir(dir);
	if (!dr)
	{
		closedir(dir);
		files = malloc(8);
		if (!files)
			exit(EXIT_FAILURE);
		files[0] = 0;
		return (files);
	}
	return (load_lst(dr, dir, hidden));
}

int	match_loop(char **split, char **files, int i)
{
	int		count;
	int		j;
	size_t	last;

	count = 0;
	j = -1;
	last = 0;
	while (split[++j])
	{
		while (!ft_strnstr(files[i], split[j], ft_strlen(files[i]))
			|| (last >= (size_t)ft_strnstr(files[i], split[j],
					ft_strlen(files[i]))))
		{
			files[i] += 1;
			count++;
			if (!files[i][0])
			{
				files[i] -= count;
				files[i][0] = '\0';
				return (-1);
			}
		}
		last = (size_t)ft_strnstr(files[i], split[j], ft_strlen(files[i]));
	}
	return (files[i] -= count, j);
}

void	match(char *str, char **split, char **files, int i)
{
	int	j;

	j = match_loop(split, files, i);
	if (j == -1)
		return ;
	if ((str[0] != '*' && ft_strncmp(split[0], files[i], ft_strlen(split[0])))
		|| (str[ft_strlen(str) - 1] != '*' && ft_strncmp(split[j - 1], files[i]
				+ ft_strlen(files[i]) - ft_strlen(split[j - 1]),
				ft_strlen(split[j - 1]))))
	{
		files[i][0] = '\0';
		return ;
	}
}

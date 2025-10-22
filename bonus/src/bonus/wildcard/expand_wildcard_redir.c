/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_redir.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static void	process_file_matches(char **files, int *match_count, char **result)
{
	int	i;

	i = -1;
	while (files[++i])
	{
		if (files[i][0])
		{
			(*match_count)++;
			if (*match_count == 1)
				*result = ft_strdup(files[i]);
			else if (*match_count == 2)
			{
				free(*result);
				*result = NULL;
				break ;
			}
		}
	}
}

static char	**prepare_files_for_pattern(char *pattern, t_node *node)
{
	char		**files;
	int			i;

	(void)node;
	files = get_file_list(pattern[0] == '.'
			&& ft_strchr(pattern, '.') < ft_strchr(pattern, '*'));
	i = -1;
	while (files[++i])
	{
		if (!matches_pattern(files[i], pattern))
			files[i][0] = '\0';
	}
	return (files);
}

char	*expand_wildcard_redir(char *pattern, t_node *node)
{
	char		**files;
	int			match_count;
	char		*result;

	if (!ft_strchr(pattern, '*') && !ft_strchr(pattern, '?'))
		return (ft_strdup(pattern));
	files = prepare_files_for_pattern(pattern, node);
	match_count = 0;
	result = NULL;
	process_file_matches(files, &match_count, &result);
	strarrfree(files);
	if (match_count == 0)
		return (ft_strdup(pattern));
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_pattern.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:11:01 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

int	expand_pattern(char **newargs, int *j, char *pattern)
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

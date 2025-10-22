/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pattern_matching.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static int	handle_wildcard(char *filename, char *pattern, int *i, int *j)
{
	(*j)++;
	if (!pattern[*j])
	{
		*i = (int)ft_strlen(filename);
		return (1);
	}
	while (filename[*i])
	{
		if (matches_pattern(filename + *i, pattern + *j))
			return (1);
		(*i)++;
	}
	return (0);
}

static int	handle_character(char *filename, char *pattern, int *i, int *j)
{
	if (pattern[*j] == '?' || pattern[*j] == filename[*i])
	{
		(*i)++;
		(*j)++;
		return (1);
	}
	return (0);
}

int	matches_pattern(char *filename, char *pattern)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (pattern[j] && filename[i])
	{
		if (pattern[j] == '*')
		{
			if (!handle_wildcard(filename, pattern, &i, &j))
				return (0);
		}
		else if (!handle_character(filename, pattern, &i, &j))
			return (0);
	}
	while (pattern[j] == '*')
		j++;
	return (!pattern[j] && !filename[i]);
}

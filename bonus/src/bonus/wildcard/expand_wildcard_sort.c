/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_sort.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:11:01 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

void	sort_matches(char **matches)
{
	int		i;
	char	*temp;

	i = 0;
	while (matches[i + 1])
	{
		if (ft_strncmp(matches[i], matches[i + 1], 1000) > 0)
		{
			temp = matches[i];
			matches[i] = matches[i + 1];
			matches[i + 1] = temp;
			i = 0;
		}
		else
			i++;
	}
}

int	add_sorted_matches(char **newargs, int *j, char **matches)
{
	int	i;

	i = 0;
	while (matches[i])
	{
		if (!add_matching_file(newargs, j, matches[i]))
		{
			while (matches[i])
				free(matches[i++]);
			free(matches);
			return (0);
		}
		free(matches[i]);
		i++;
	}
	return (1);
}

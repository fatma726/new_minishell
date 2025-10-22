/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strarrutils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:08 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	**strarradd(char **arr, char *str)
{
	char	**new;
	size_t	i;
	size_t	len;

	len = 0;
	while (arr[len])
		len++;
	new = malloc((len + 2) * sizeof(char *));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = ft_strdup(arr[i]);
		if (!new[i])
			return (strarrfree(new), NULL);
		i++;
	}
	new[len] = ft_strdup(str);
	if (!new[len])
		return (strarrfree(new), NULL);
	new[len + 1] = 0;
	return (new);
}

char	**strarrdup(char **strs)
{
	size_t	i;
	char	**newstrs;

	if (!strs)
		return (NULL);
	i = 0;
	while (strs[i])
		i++;
	newstrs = malloc((i + 1) * sizeof(char *));
	if (!newstrs)
		exit(EXIT_FAILURE);
	i = 0;
	while (strs[i])
	{
		newstrs[i] = ft_strdup(strs[i]);
		if (!newstrs[i])
		{
			strarrfree(newstrs);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	newstrs[i] = 0;
	return (newstrs);
}

char	**strarradd_take(char **arr, char *owned)
{
	size_t	n;
	char	**newarr;
	size_t	i;

	n = 0;
	while (arr && arr[n])
		n++;
	newarr = malloc(sizeof(char *) * (n + 2));
	if (!newarr)
	{
		free(owned);
		return (NULL);
	}
	i = 0;
	while (i < n)
	{
		newarr[i] = arr[i];
		i++;
	}
	newarr[n] = owned;
	newarr[n + 1] = NULL;
	if (arr)
		free(arr);
	return (newarr);
}

void	strarrfree(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = -1;
	while (strs[++i])
		free(strs[i]);
	free(strs);
}

size_t	strarrlen(char **strs)
{
	size_t	i;

	if (!strs)
		return (0);
	i = 0;
	while (strs[i])
		i++;
	return (i);
}

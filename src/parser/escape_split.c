/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   escape_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:08 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static bool	sep(char **s, size_t i, size_t *i3, t_node *node)
{
	i3[3] = (size_t)quote_check(s[0], (int)i, node);
	return (!s[0][i] || (!i3[3] && ft_strchr(s[1], s[0][i])));
}

static void	loop(char **s, char **returned, size_t *i, t_node *n)
{
	if (i[0] && sep(s, i[0], i, n) && !sep(s, i[0] - 1, i, n))
	{
		i[1] = i[0]--;
		while (i[0] && !(sep(s, i[0], i, n) && !sep(s, i[0] - 1, i, n)))
			i[0]--;
		while (sep(s, i[0], i, n))
			i[0]++;
		i[2]--;
		returned[i[2]] = ft_substr(s[0], (unsigned int)i[0], i[1] - i[0]);
		if (!returned[i[2]])
		{
			while (returned[++i[2]])
				free(returned[i[2]]);
			free(returned);
			exit(EXIT_FAILURE);
		}
		i[0] = i[1];
	}
}

char	**escape_split(char *s, char *c, t_node *node)
{
	size_t	i[5];
	char	**returned;
	char	*str[2];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	str[0] = s;
	str[1] = c;
	while (s[i[0]])
	{
		if (!sep(str, i[0], i, node) && sep(str, i[0] + 1, i, node))
			i[2]++;
		i[0]++;
	}
	i[0] = ft_strlen(s) + 1;
	returned = malloc((i[2] + 1) * 8);
	if (!returned)
		exit(EXIT_FAILURE);
	returned[i[2]] = 0;
	while (i[0]--)
		loop(str, returned, i, node);
	return (returned);
}

char	*escape_handler(char *str, t_node *node)
{
	(void)node;
	return (str);
}

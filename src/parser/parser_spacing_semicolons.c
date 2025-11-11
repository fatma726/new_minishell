/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_semicolons.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static int	need_space_around_semicolon(char *s, int i)
{
	int	need_before;
	int	need_after;

	need_before = (i > 0 && s[i - 1] != ' ' && s[i - 1] != '\t');
	need_after = (s[i + 1] && s[i + 1] != ' ' && s[i + 1] != '\t'
			&& s[i + 1] != ';');
	if (need_before && need_after)
		return (3);
	if (need_before)
		return (1);
	if (need_after)
		return (2);
	return (0);
}

static int	copy_with_semicolons(char *str, t_node *node, char *result)
{
	int	i;
	int	j;
	int	need_space;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (!quote_check(str, i, node) && str[i] == ';')
		{
			need_space = need_space_around_semicolon(str, i);
			if (need_space == 1 || need_space == 3)
				result[j++] = ' ';
			result[j++] = ';';
			if (need_space == 2 || need_space == 3)
				result[j++] = ' ';
			i++;
		}
		else
		{
			result[j++] = str[i];
			i++;
		}
	}
	return (j);
}

char	*add_spaces_around_semicolons(char *str, t_node *node)
{
	char	*result;
	size_t	len;
	int		new_len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc((len * 2 + 1) * sizeof(char));
	if (!result)
		return (free(str), NULL);
	new_len = copy_with_semicolons(str, node, result);
	result[new_len] = '\0';
	free(str);
	return (result);
}

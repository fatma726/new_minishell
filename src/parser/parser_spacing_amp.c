/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_amp.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	need_space_before_amp(char *s, int i)
{
	return (i > 0 && s[i - 1] != ' ' && s[i - 1] != '\t');
}

static int	need_space_after_amp(char *s, int i)
{
	return (s[i + 1] && s[i + 1] != '&' && s[i + 1] != ' '
		&& s[i + 1] != '\t');
}

static int	copy_with_ampersands(char *str, t_node *node, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '&' && !quote_check(str, i, node))
		{
			if (i == 0 || str[i - 1] != '&')
			{
				if (need_space_before_amp(str, i))
					result[j++] = ' ';
				result[j++] = '&';
				if (need_space_after_amp(str, i))
					result[j++] = ' ';
			}
			else
				result[j++] = '&';
		}
		else
			result[j++] = str[i];
		i++;
	}
	return (j);
}

char	*add_spaces_around_ampersand(char *str, t_node *node)
{
	char	*result;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc((len * 2 + 1) * sizeof(char));
	if (!result)
	{
		free(str);
		return (NULL);
	}
	len = (size_t)copy_with_ampersands(str, node, result);
	result[len] = '\0';
	free(str);
	return (result);
}

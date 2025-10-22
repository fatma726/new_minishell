/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_spacing_logical.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

static int	copy_with_logical_operators(char *str, t_node *node, char *result)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '|' && str[i + 1] == '|' && !quote_check(str, i, node))
			handle_oror_operator(str, result, &i, &j);
		else if (str[i] == '&' && str[i + 1] == '&'
			&& !quote_check(str, i, node))
			handle_andand_operator(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	return (j);
}

char	*add_spaces_around_logical_operators(char *str, t_node *node)
{
	char	*result;
	size_t	len;

	(void)node;
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc((len * 2 + 1) * sizeof(char));
	if (!result)
	{
		free(str);
		return (NULL);
	}
	len = (size_t)copy_with_logical_operators(str, node, result);
	result[len] = '\0';
	free(str);
	return (result);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:49:44 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*hash_handler(char *str, t_node *node)
{
	unsigned int	i;
	char			*new_str;

	i = 0;
	while (str[i])
	{
		if (!quote_check(str, i, node) && str[i] == '#')
			break ;
		i++;
	}
	if (i == ft_strlen(str))
		return (str);
	new_str = ft_substr(str, 0, i);
	free(str);
	return (new_str);
}

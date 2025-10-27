/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_reject.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-bootstrap                        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 00:00:00 by bootstrap         #+#    #+#             */
/*   Updated: 2025/10/27 00:00:00 by bootstrap        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	parser_reject_non_mandatory(char **args)
{
	int	i;

	i = 0;
	while (args && args[i])
	{
		if (!ft_strncmp(args[i], "&&", 3)
			|| !ft_strncmp(args[i], "||", 3)
			|| !ft_strncmp(args[i], "(", 2)
			|| !ft_strncmp(args[i], ")", 2)
			|| !ft_strncmp(args[i], ";", 2))
			return (1);
		i++;
	}
	return (0);
}

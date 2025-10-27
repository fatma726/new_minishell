/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_parser_main.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../bonus/include/bonus.h"

#ifdef BUILD_BONUS
# include "bonus.h"

/* implemented in wildcard_parser_helpers.c */

/* helper prototypes are provided by headers; avoid redundant decls */

char	**expand_wildcard_if_bonus(char **args, char **envp, t_node *node)
{
	char	**newargs;
	int		i;
	bool	skip_next;
	size_t	size;

	(void)envp;
	break_on_first_pattern(args, &newargs, &i, &skip_next);
	if (!newargs)
		return (args);
	size = count_expanded_size(args);
	if (size == 0)
	{
		free(newargs);
		return (args);
	}
	build_expanded_args(args, newargs);
	if (!newargs[0])
	{
		free(newargs);
		return (args);
	}
	strarrfree(args);
	strip_wildmarks_inplace(newargs);
	(void)node;
	return (newargs);
}

#endif

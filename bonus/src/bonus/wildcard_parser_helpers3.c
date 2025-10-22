/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_parser_helpers3.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:56:40 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifdef BUILD_BONUS
# include "bonus.h"

void	process_one_arg(char **args, char **newargs,
		int *ij, bool *skip_next)
{
	if (*skip_next)
	{
		newargs[(ij[1])] = ft_strdup(args[ij[0]]);
		if (newargs[(ij[1])])
			ij[1]++;
		*skip_next = false;
		ij[0]++;
		return ;
	}
	if (is_redir_token(args[ij[0]]))
	{
		newargs[(ij[1])] = ft_strdup(args[ij[0]]);
		if (newargs[(ij[1])])
			ij[1]++;
		*skip_next = true;
		ij[0]++;
		return ;
	}
	append_arg_or_expand(newargs, &ij[1], args[ij[0]]);
	ij[0]++;
}

void	build_expanded_args(char **args, char **newargs)
{
	int		ij[2];
	bool	skip_next;

	if (!newargs)
		return ;
	ij[0] = 0;
	ij[1] = 0;
	skip_next = false;
	while (args && args[ij[0]])
		process_one_arg(args, newargs, ij, &skip_next);
	newargs[ij[1]] = NULL;
}

#endif

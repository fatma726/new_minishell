/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_wildcard_phase.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

#ifdef BUILD_BONUS
# include "../../bonus/include/bonus.h"

char	**apply_wildcard_phase(char **args, char **envp, t_node *node,
		char *orig)
{
	if (!check_wildcard_redirections(args))
	{
		free(orig);
		strarrfree(args);
		set_exit_status(1);
		return (NULL);
	}
	return (expand_wildcard_if_bonus(args, envp, node));
}

#else

char	**apply_wildcard_phase(char **args, char **envp, t_node *node,
		char *orig)
{
	(void)orig;
	return (expand_wildcard(args, envp, node));
}

#endif

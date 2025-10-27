/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_parser_helpers2.c                       :+:      :+:    :+:   */
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

/* prototypes provided in bonus.h */

static bool	has_unquoted_wildcards(char *arg, t_node *node);

static void	handle_pattern_found(char **args, char ***out)
{
	size_t	size;

	size = count_expanded_size(args);
	if (size > 0)
		*out = (char **)malloc(sizeof(char *) * size);
}

void	break_on_first_pattern(char **args, char ***out, int *i,
							bool *skip)
{
	*out = NULL;
	*i = 0;
	*skip = false;
	while (args && args[*i])
	{
		if (*skip)
		{
			*skip = false;
			(*i)++;
			continue ;
		}
		if (is_redir_token(args[*i]))
		{
			*skip = true;
			(*i)++;
			continue ;
		}
		if (has_unquoted_wildcards(args[*i], NULL))
		{
			handle_pattern_found(args, out);
			break ;
		}
		(*i)++;
	}
}

static bool	has_unquoted_wildcards(char *arg, t_node *node)
{
	int	i;

	if (!arg)
		return (false);
	i = 0;
	while (arg[i])
	{
		if (!quote_check(arg, i, node) && (arg[i] == '*' || arg[i] == '?'))
			return (true);
		i++;
	}
	return (false);
}

void	append_arg_or_expand(char **newargs, int *j, char *arg)
{
	int	matches;

	if (!newargs || !arg)
		return ;
	if (has_unquoted_wildcards(arg, NULL))
	{
		matches = expand_pattern(newargs, j, arg);
		if (matches == 0)
		{
			newargs[(*j)] = ft_strdup(arg);
			if (newargs[(*j)])
				(*j)++;
		}
	}
	else
	{
		newargs[(*j)] = ft_strdup(arg);
		if (newargs[(*j)])
			(*j)++;
	}
}

#endif

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard_utils5.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/16 15:49:44 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

static bool	has_unquoted_wildcards(char *arg, t_node *node);

int	count_wildcard_matches(char **args)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	while (args[++i])
	{
		if (has_unquoted_wildcards(args[i], NULL))
			count += count_matching_files(args[i]);
	}
	return (count);
}

int	process_all_args(char **args, char **newargs)
{
	int	i;
	int	j;

	j = 0;
	i = -1;
	while (args[++i])
	{
		if (has_unquoted_wildcards(args[i], NULL))
		{
			if (process_wildcard_pattern(newargs, &j, args[i]) == -1)
				return (-1);
		}
		else
		{
			if (process_regular_arg(newargs, &j, args[i]) == -1)
				return (-1);
		}
	}
	newargs[j] = NULL;
	return (0);
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

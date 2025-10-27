/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_parser_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../bonus/include/bonus.h"

#ifdef BUILD_BONUS

bool	is_redir_token(char *s)
{
	return (s && (isdlr(s) || isdrr(s) || islr(s)
			|| islrr(s) || isrr(s) || istlr(s)));
}

bool	check_wildcard_redirections(char **args)
{
	int	i;
	int	count;

	i = 0;
	while (args && args[i])
	{
		if (!isdlr(args[i]) && !istlr(args[i]) && is_redir_token(args[i])
			&& args[i + 1] && (ft_strchr(args[i + 1], '*')
				|| ft_strchr(args[i + 1], '?')))
		{
			count = count_matching_files(args[i + 1]);
			if (count > 1)
			{
				ft_putstr_fd("bash: ", STDERR_FILENO);
				ft_putstr_fd(args[i + 1], STDERR_FILENO);
				ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
				return (false);
			}
			if (args[i + 1])
				i++;
		}
		i++;
	}
	return (true);
}

static void	add_wildcard_size(char *arg, size_t *sz)
{
	int	matches;

	matches = count_matching_files(arg);
	if (matches > 0)
		*sz += (size_t)matches;
	else
		(*sz)++;
}

static void	count_size_iter(char **args, size_t *i, size_t *sz, bool *skip)
{
	if (*skip)
	{
		*skip = false;
		(*sz)++;
		(*i)++;
		return ;
	}
	if (is_redir_token(args[*i]))
	{
		(*sz)++;
		*skip = true;
		(*i)++;
		return ;
	}
	if (ft_strchr(args[*i], '*') || ft_strchr(args[*i], '?'))
		add_wildcard_size(args[*i], sz);
	else
		(*sz)++;
	(*i)++;
}

size_t	count_expanded_size(char **args)
{
	size_t	i;
	size_t	sz;
	bool	skip;

	i = 0;
	sz = 0;
	skip = false;
	while (args && args[i])
		count_size_iter(args, &i, &sz, &skip);
	return (sz + 1);
}

#endif

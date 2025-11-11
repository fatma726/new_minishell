/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_parse_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"
#include <limits.h>

static size_t	skip_spaces(const char *s, size_t i)
{
	while ((s[i] > '\b' && s[i] <= '\r') || s[i] == ' ')
		i++;
	return (i);
}

static void	parse_sign(const char *s, size_t *i, int *sign)
{
	*sign = 1;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			*sign = -1;
		*i += 1;
	}
}

static bool	accumulate_digits(const char *s, size_t *i, int sign,
					long long *res)
{
	int			d;

	while (ft_isdigit(s[*i]))
	{
		d = s[*i] - '0';
		if (sign > 0)
		{
			if (*res > (LLONG_MAX - (long long)d) / 10)
				return (false);
			*res = *res * 10 + (long long)d;
		}
		else
		{
			if (*res < (LLONG_MIN + (long long)d) / 10)
				return (false);
			*res = *res * 10 - (long long)d;
		}
		*i += 1;
	}
	return (true);
}

bool	parse_strict_ll(const char *s, long long *out)
{
	long long	res;
	int			sign;
	size_t		i;

	if (!s)
		return (false);
	i = 0;
	i = skip_spaces(s, i);
	parse_sign(s, &i, &sign);
	if (!ft_isdigit(s[i]))
		return (false);
	res = 0;
	if (!accumulate_digits(s, &i, sign, &res))
		return (false);
	i = skip_spaces(s, i);
	if (s[i] != '\0')
		return (false);
	*out = res;
	return (true);
}

bool	handle_too_many_args(t_node *node)
{
	char	too_many[47];

	set_exit_status_n(node, EXIT_FAILURE);
	ft_strlcpy(too_many, "minishell: exit: too many arguments\n", 37);
	ft_putstr_fd(too_many, STDERR_FILENO);
	return (false);
}

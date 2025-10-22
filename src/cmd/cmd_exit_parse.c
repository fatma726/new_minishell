/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exit_parse.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:04 by fatmtahmdab      ###   ########.fr       */
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

static bool	parse_strict_ll(const char *s, long long *out)
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

bool	handle_exit_with_args(char **args)
{
	long long	exit_num;

	if (!ft_isalldigit(args[1]))
	{
		handle_numeric_error(args[1]);
		return (true);
	}
	if (strarrlen(args) > 2)
	{
		handle_too_many_args();
		return (false);
	}
	if (!parse_strict_ll(args[1], &exit_num))
	{
		handle_numeric_error(args[1]);
		return (true);
	}
	set_exit_status((int)((unsigned char)exit_num));
	return (true);
}

/* exit_will_terminate moved to cmd_exit_logic.c to satisfy function count */

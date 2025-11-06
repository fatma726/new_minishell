/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers5.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

const char	*get_full_original_token(char **ori_args)
{
	int	i;

	i = 0;
	while (ori_args && ori_args[i])
	{
		if (ori_args[i][0] == '|' || ori_args[i][0] == '<'
			|| ori_args[i][0] == '>')
			return (ori_args[i]);
		i++;
	}
	return ("newline");
}

static void	set_token_chars(char *tok, char **ori_args, int i)
{
	tok[0] = ori_args[i][0];
	if (ori_args[i][1] == ori_args[i][0])
	{
		tok[1] = ori_args[i][0];
		tok[2] = '\0';
	}
	else if (ft_strlen(ori_args[i]) > 2
		&& (ori_args[i][0] == '>' || ori_args[i][0] == '<'))
	{
		tok[1] = ori_args[i][0];
		tok[2] = '\0';
	}
	else
		tok[1] = '\0';
}

const char	*find_fallback_token(char **ori_args)
{
	int			i;
	static char	tok[3];

	i = 0;
	while (ori_args && ori_args[i])
	{
		if (ori_args[i][0] == '|' || ori_args[i][0] == '<'
			|| ori_args[i][0] == '>')
		{
			set_token_chars(tok, ori_args, i);
			return (tok);
		}
		i++;
	}
	return ("newline");
}

bool	check_brace_syntax(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "{", 2) == 0)
		{
			i++;
			while (args[i])
			{
				if (ft_strncmp(args[i], "}", 2) == 0)
					return (true);
				i++;
			}
			return (false);
		}
		if (ft_strncmp(args[i], "}", 2) == 0)
			return (false);
		i++;
	}
	return (true);
}

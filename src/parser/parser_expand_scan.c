/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_scan.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 13:31:37 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	handle_redirection_chars(int *i, char *str, char *str2)
{
	str2[i[1]++] = ' ';
	str2[i[1]++] = str[i[0]++];
	if (i[0] && str[i[0] - 1] == '2')
		str2[i[1]++] = str[i[0]++];
	else if (i[0] && str[i[0] - 1] == '>')
	{
		if (str[i[0]] == '>' || str[i[0]] == '|')
			str2[i[1]++] = str[i[0]++];
	}
	else if (i[0] && str[i[0] - 1] == '<')
	{
		if (str[i[0]] == '<' || str[i[0]] == '>')
		{
			str2[i[1]++] = str[i[0]++];
			if (str[i[0]] == '<' && str[i[0] - 1] == '<')
				str2[i[1]++] = str[i[0]++];
		}
	}
	str2[i[1]++] = ' ';
}

static void	no_env(int *i, char *str, char *str2)
{
	if (!i[3]
		&& (ft_strchr("<>|()", str[i[0]])
			|| (str[i[0]] == '2' && str[i[0] + 1] == '>')
			|| (str[i[0]] == '<' && str[i[0] + 1] == '<')))
	{
		if (str[i[0]] == '(' || str[i[0]] == ')')
		{
			str2[i[1]++] = ' ';
			str2[i[1]++] = str[i[0]++];
			str2[i[1]++] = ' ';
		}
		else
			handle_redirection_chars(i, str, str2);
	}
	else
		str2[i[1]++] = str[i[0]++];
}

/* moved helpers to parser_expand_helpers2.c for norm */

void	process_envvar(char **str, char **envp, t_node *node, int *i)
{
	i[3] = quote_check(str[0], i[0], node);
	if (in_heredoc(str[0], i[0]))
		no_env(i, str[0], str[1]);
	else if (!node->only_dollar_q
		&& (i[3] != 1 || is_inside_double_quotes(str[0], i[0]))
		&& str[0][i[0]] == '$'
		&& (ft_isalnum(str[0][i[0] + 1]) || str[0][i[0] + 1] == '_'))
		handle_envvar(i, str, envp, node);
	else if ((i[3] != 1 || is_inside_double_quotes(str[0], i[0]))
		&& str[0][i[0]] == '$' && str[0][i[0] + 1] == '\0')
		str[1][i[1]++] = str[0][i[0]++];
	else if ((i[3] != 1 || is_inside_double_quotes(str[0], i[0]))
		&& !ft_strncmp(str[0] + i[0], "$?", 2))
	{
		if (!node->skip_dollar_q)
			insert_int(str[1], i, node);
		else
		{
			str[1][i[1]++] = str[0][i[0]++];
			str[1][i[1]++] = str[0][i[0]++];
		}
	}
	else
		no_env(i, str[0], str[1]);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_scan.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:08 by fatmtahmdab      ###   ########.fr       */
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

static void	process_var_value(char **str, char *var_value, int *i,
							t_node *node)
{
	while (*var_value)
	{
		if (ft_strchr("<>|", *var_value))
		{
			str[1][i[1]++] = '\\';
			node->escape_skip = false;
		}
		str[1][i[1]++] = *var_value++;
	}
}

static void	handle_envvar(int *i, char **str, char **envp, t_node *node)
{
	char	*var_name;
	char	*var_value;

	i[5] = ++i[0];
	while (ft_isalnum(str[0][i[5]]) || str[0][i[5]] == '_')
		i[5]++;
	var_name = ft_substr(str[0], (unsigned int)i[0], (size_t)(i[5] - i[0]));
	if (var_name && ft_strlen(var_name) > 0)
	{
		var_value = ft_getenv(var_name, envp);
		if (var_value)
			process_var_value(str, var_value, i, node);
	}
	if (var_name)
		free(var_name);
	i[0] = i[5];
}

void	process_envvar(char **str, char **envp, t_node *node, int *i)
{
	i[3] = quote_check(str[0], i[0], node);
	if (in_heredoc(str[0], i[0]))
		no_env(i, str[0], str[1]);
	else if (i[3] != 1 && str[0][i[0]] == '$'
		&& (ft_isalnum(str[0][i[0] + 1]) || str[0][i[0] + 1] == '_'))
		handle_envvar(i, str, envp, node);
	else if (i[3] != 1 && str[0][i[0]] == '$' && str[0][i[0] + 1] == '\0')
		str[1][i[1]++] = str[0][i[0]++];
	else if (i[3] != 1 && !ft_strncmp(str[0] + i[0], "$?", 2))
		insert_int(str[1], i);
	else
		no_env(i, str[0], str[1]);
}

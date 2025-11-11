/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expand_helpers2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

bool	is_inside_double_quotes(char const *str, int i)
{
	bool	in_single;
	bool	in_double;
	int		j;
	char	c;

	if (!str || i < 0)
		return (false);
	in_single = false;
	in_double = false;
	j = 0;
	while (j <= i && str[j])
	{
		c = str[j];
		if (c == '\'' && !in_double)
			in_single = !in_single;
		else if (c == '"' && !in_single)
			in_double = !in_double;
		j++;
	}
	return (in_double);
}

void	process_var_value(char **str, char *var_value, int *i, t_node *node)
{
	bool	needs_quoting;

	needs_quoting = ft_strchr(var_value, ' ') != NULL;
	if (needs_quoting)
		str[1][i[1]++] = '"';
	while (*var_value)
	{
		if (ft_strchr("<>|", *var_value))
		{
			str[1][i[1]++] = '\\';
			node->escape_skip = false;
		}
		str[1][i[1]++] = *var_value++;
	}
	if (needs_quoting)
		str[1][i[1]++] = '"';
}

void	handle_envvar(int *i, char **str, char **envp, t_node *node)
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

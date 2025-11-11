/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_length_helpers.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: minishell-norm <norm@42>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 00:00:00 by norm              #+#    #+#             */
/*   Updated: 2025/11/10 00:00:00 by norm             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	process_envvar_value(char **envp, int *i, int j);

static int	getsize(long n)
{
	int	s;

	if (n < 0)
		n = -n;
	s = 1;
	if (!n)
		s++;
	while (n && s++)
		n /= 10;
	return (s);
}

static void	scan_varname(char *str, int *i)
{
	i[1] = i[0];
	while (str[i[1]] && (ft_isalnum(str[i[1]]) || str[i[1]] == '_'))
		i[1]++;
}

static void	find_env_entry(char *str, char **envp, int *i)
{
	i[2] = 0;
	while (envp[i[2]] && (ft_strncmp(envp[i[2]], str + i[0],
				(size_t)(i[1] - i[0])) || envp[i[2]][i[1] - i[0]] != '='))
		i[2]++;
}

int	handle_envvar_length(char *str, char **envp, int *i, t_node *node)
{
	int	j;

	if (!str || !envp || !i)
		return (0);
	j = -1;
	if (str[++i[0]] == '?')
	{
		if (node->skip_dollar_q)
			i[5] += 2;
		else
			i[5] += getsize(get_exit_status_n(node));
		return (1);
	}
	scan_varname(str, i);
	find_env_entry(str, envp, i);
	if (!node->only_dollar_q && envp[i[2]])
	{
		j = i[1] - i[0] + 1;
		process_envvar_value(envp, i, j);
	}
	i[0] = i[1];
	return (0);
}

void	handle_redirection_length(char *str, int *i)
{
	i[5] += 2;
	if (i[0] && str[i[0] - 1] == '2')
	{
		i[5] += 1;
		i[0]++;
	}
	else if (i[0] && str[i[0] - 1] == '>')
	{
		if (str[i[0]] == '>' || str[i[0]] == '|')
		{
			i[5] += 1;
			i[0]++;
		}
	}
	else if (i[0] && str[i[0] - 1] == '<'
		&& (str[i[0]] == '<' || str[i[0]] == '>'))
	{
		i[5] += 1;
		if (str[i[0] + 1] == '<' && str[i[0]] != '>')
		{
			i[5] += 1;
			i[0]++;
		}
		i[0]++;
	}
}

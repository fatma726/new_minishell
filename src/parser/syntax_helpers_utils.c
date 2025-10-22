/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

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

static void	process_envvar_value(char **envp, int *i, int j)
{
	while (envp[i[2]][j])
	{
		i[5] += 1 + (ft_strchr("<>|", envp[i[2]][j]) != 0);
		j++;
	}
}

static int	handle_envvar_length(char *str, char **envp, int *i)
{
	int	j;

	if (!str || !envp || !i)
		return (0);
	j = -1;
	if (str[++i[0]] == '?')
	{
		i[5] += getsize(get_exit_status());
		return (1);
	}
	i[1] = i[0];
	while (str[i[1]] && (ft_isalnum(str[i[1]]) || str[i[1]] == '_'))
		i[1]++;
	i[2] = 0;
	while (envp[i[2]]
		&& (ft_strncmp(envp[i[2]], str + i[0],
				(size_t)(i[1] - i[0])) || envp[i[2]][i[1] - i[0]] != '='))
		i[2]++;
	if (envp[i[2]])
	{
		j = i[1] - i[0] + 1;
		process_envvar_value(envp, i, j);
	}
	i[0] = i[1];
	return (0);
}

static void	handle_redirection_length(char *str, int *i)
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

void	get_length(char *str, char **envp, int *i, t_node *node)
{
	i[3] = quote_check(str, i[0], node);
	if (in_heredoc(str, i[0]))
		i[5]++;
	else if (i[3] < 2
		&& ((str[i[0]] == '$' && (ft_isalnum(str[i[0] + 1])
					|| str[i[0] + 1] == '_'
					|| (i[3] != 1 && str[i[0] + 1] == '\"')
					|| (i[3] < 2 && str[i[0] + 1] == '\'')))
			|| !ft_strncmp(str + i[0], "$?", 2))
		&& handle_envvar_length(str, envp, i))
		return ;
	else if (!i[3]
		&& (ft_strchr("<>|", str[i[0]])
			|| (str[i[0]] == '2' && str[i[0] + 1] == '>')))
		handle_redirection_length(str, i);
	else
		i[5]++;
}

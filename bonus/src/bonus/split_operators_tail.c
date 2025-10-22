/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_operators_tail.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "bonus.h"
#include "../../include/bonus.h"

static char	**continue_split(char *s, size_t i, char **envp, t_node *n)
{
	char	*new_str;
	int		skip;

	if (s[i] == '&' && s[i + 1] != '&')
		skip = 1;
	else
		skip = 2;
	new_str = ft_substr(s, (unsigned int)(i + (size_t)skip),
			ft_strlen(s) - (i + (size_t)skip));
	free(s);
	return (split_operators(new_str, envp, n));
}

char	**split_operators_tail(char *s, size_t i, char **envp, t_node *n)
{
	int	exit_status;

	if (!s[i] || n->syntax_flag)
		return (free(s), envp);
	exit_status = get_exit_status();
	if (exit_status != 0 && s[i] == '|' && s[i + 1] == '|')
		return (continue_split(s, i, envp, n));
	if (exit_status == 0 && s[i] == '&' && s[i + 1] == '&')
		return (continue_split(s, i, envp, n));
	if (s[i] == '&' && s[i + 1] != '&')
		return (continue_split(s, i, envp, n));
	return (free(s), envp);
}

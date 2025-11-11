/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helpers_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/06 21:32:11 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

/* getsize removed; not used after refactor */

/* helpers moved to syntax_length_helpers.c */

void	get_length(char *str, char **envp, int *i, t_node *node)
{
	i[3] = quote_check(str, i[0], node);
	if (in_heredoc(str, i[0]))
		i[5]++;
	else if (i[3] < 2 && !ft_strncmp(str + i[0], "$?", 2)
		&& handle_envvar_length(str, envp, i, node))
		return ;
	else if (i[3] < 2
		&& str[i[0]] == '$'
		&& (ft_isalnum(str[i[0] + 1])
			|| str[i[0] + 1] == '_'
			|| (i[3] != 1 && str[i[0] + 1] == '\"')
			|| (i[3] < 2 && str[i[0] + 1] == '\''))
		&& !node->only_dollar_q
		&& handle_envvar_length(str, envp, i, node))
		return ;
	else if (!i[3]
		&& (ft_strchr("<>|", str[i[0]])
			|| (str[i[0]] == '2' && str[i[0] + 1] == '>')))
		handle_redirection_length(str, i);
	else
		i[5]++;
}

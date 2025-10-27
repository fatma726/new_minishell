/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../bonus/include/bonus.h"

int		validate_parens(char *str, t_node *node, int *length);
char	*handle_triple_redir_error(char *str, t_node *node);
char	*handle_paren_error(char *str, int count, t_node *node);

/* Validate parentheses, then delegate to operator splitter. */
char	**subshell(char *str, char **envp, t_node *node)
{
	int	count;
	int	length;

	(void)length;
	if (str && ft_strlen(str) >= 3 && ft_strncmp(str, ">>>", 3) == 0)
	{
		handle_triple_redir_error(str, node);
		return (envp);
	}
	count = validate_parens(str, node, &length);
	if (count == -1)
		return (free(str), envp);
	str = handle_paren_error(str, count, node);
	if (!str)
		return (envp);
	return (split_operators(str, envp, node));
}

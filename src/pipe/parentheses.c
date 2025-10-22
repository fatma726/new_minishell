/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parentheses.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/22 18:50:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"
#ifdef BUILD_BONUS
# include "../../bonus/include/bonus.h"
#endif

static void	copy_args_to_new(char **args, char **new_args)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (args[i] && !is_close_paren(args[i]))
	{
		new_args[j] = ft_strdup(args[i]);
		if (!new_args[j])
		{
			while (--j >= 0)
				free(new_args[j]);
			free(new_args);
			exit(EXIT_FAILURE);
		}
		i++;
		j++;
	}
	new_args[j] = NULL;
}

static char	**extract_paren_content(char **args, t_node *node)
{
	char	**new_args;

	(void)node;
	new_args = malloc((strarrlen(args) - 1) * sizeof(char *));
	if (!new_args)
		exit(EXIT_FAILURE);
	copy_args_to_new(args, new_args);
	return (new_args);
}

char	**handle_parentheses(char **args, char **envp, t_node *node)
{
	char	**new_args;

	if (!args || !args[0] || !is_open_paren(args[0]))
		return (NULL);
	new_args = extract_paren_content(args, node);
	node->is_subshell = true;
	envp = execute(new_args, envp, node);
	node->is_subshell = false;
	strarrfree(new_args);
	return (envp);
}

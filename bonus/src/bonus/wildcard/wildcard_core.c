/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_core.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../../include/bonus.h"

char	**expand_wildcard(char **args, char **envp, t_node *node)
{
	char	**newargs;
	int		count;

	(void)envp;
	(void)node;
	count = count_wildcard_matches(args);
	if (count == 0)
		return (args);
	newargs = malloc((strarrlen(args) + (size_t)count + 1) * sizeof(char *));
	if (!newargs)
		return (args);
	if (process_all_args(args, newargs) == -1)
	{
		cleanup_and_return(newargs, 0, args);
		return (NULL);
	}
	strarrfree(args);
	return (newargs);
}

void	wildcard_handler(char **args, char **newargs, int *i, t_node *node)
{
	char	*pattern;
	int		j;

	pattern = args[i[2]];
	j = i[5];
	if (expand_pattern(newargs, &j, pattern))
		i[5] = j;
	else
	{
		newargs[i[5]++] = ft_strdup(pattern);
		if (!newargs[i[5] - 1])
		{
			strarrfree(newargs);
			strarrfree(args);
			exit(1);
		}
	}
	(void)node;
}

static void	get_arg_num_loop(char **args, int *i, int *n, t_node *node)
{
	char	**files;
	int		n2;

	(void)node;
	i[1] = -1;
	files = get_file_list(args[i[2]][0] == '.'
			&& ft_strchr(args[i[2]], '.') < ft_strchr(args[i[2]], '*'));
	while (files[++i[1]])
	{
		if (!matches_pattern(files[i[1]], args[i[2]]))
			files[i[1]][0] = '\0';
	}
	i[1] = -1;
	n2 = 0;
	while (files[++i[1]])
		if (files[i[1]][0])
			n2++;
	if (!n2)
		*n += 1;
	else
		*n += n2;
	strarrfree(files);
}

int	get_arg_num(char **args, t_node *node)
{
	int	i[5];
	int	n;

	i[2] = -1;
	n = 0;
	while (args[++i[2]])
	{
		i[0] = -1;
		while (args[i[2]][++i[0]])
		{
			i[3] = quote_check(args[i[2]], i[0], node);
			if (!i[3] && args[i[2]][i[0]] == '*')
			{
				get_arg_num_loop(args, i, &n, node);
				break ;
			}
		}
	}
	return (n);
}

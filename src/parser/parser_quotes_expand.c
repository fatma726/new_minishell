/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_quotes_expand.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:09 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

char	*return_marked_unchanged(char *str)
{
	char	*newstr;
	size_t	len;

	len = ft_strlen(str);
	if (len == 0)
		return (str);
	newstr = (char *)malloc(len);
	if (!newstr)
	{
		free(str);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(newstr, str + 1, len);
	newstr[len - 1] = '\0';
	free(str);
	return (newstr);
}

void	strip_wildmarks_inplace(char **args)
{
	int		i;
	char	*tmp;
	size_t	len;

	i = 0;
	while (args && args[i])
	{
		if (args[i][0] == (char)WILDMARK)
		{
			len = ft_strlen(args[i]);
			tmp = (char *)malloc(len);
			if (!tmp)
				exit(EXIT_FAILURE);
			ft_memcpy(tmp, args[i] + 1, len);
			tmp[len - 1] = '\0';
			free(args[i]);
			args[i] = tmp;
		}
		i++;
	}
}

void	insert_int(char *str, int *i)
{
	int	j;

	j = 1000000000;
	while (j)
	{
		if (get_exit_status() / j)
			str[i[1]++] = (char)(get_exit_status() / j % 10 + '0');
		j /= 10;
	}
	if (!get_exit_status())
		str[i[1]++] = '0';
	i[0] += 2;
}

static void	expand_envvar_loop(char **str, char **envp, t_node *node)
{
	int	i[7];

	i[0] = 0;
	i[1] = 0;
	while (str[0][i[0]])
		process_envvar(str, envp, node, i);
	str[1][i[1]] = '\0';
}

char	*expand_envvar(char *str, char **envp, t_node *node)
{
	int		i[6];
	char	*strs[2];

	i[0] = -1;
	i[5] = 0;
	while (str[++i[0]])
	{
		get_length(str, envp, i, node);
		if (!str[i[0]])
			break ;
	}
	if (i[5] < 4096)
		i[5] = 4096;
	strs[1] = malloc(((size_t)i[5] + 1) * sizeof(char));
	if (!strs[1])
	{
		free(str);
		exit(EXIT_FAILURE);
	}
	strs[0] = str;
	expand_envvar_loop(strs, envp, node);
	free(str);
	return (strs[1]);
}

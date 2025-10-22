/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_lst.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:02 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/mandatory.h"

static char	*lowest(t_list *lst)
{
	char	*ret;
	t_list	*tmp;
	t_list	*low;

	low = lst;
	while (!low->content)
		low = low->next;
	tmp = low->next;
	while (tmp)
	{
		if (ft_strncmp(low->content, tmp->content, ft_strlen(low->content)) > 0)
			low = tmp;
		tmp = tmp->next;
	}
	ret = low->content;
	low->content = 0;
	return (ret);
}

static t_list	*load_lst_loop(struct dirent *dr, DIR *dir, bool hidden)
{
	t_list	*lst;

	lst = ft_lstnew(ft_strdup(dr->d_name));
	while (1)
	{
		dr = readdir(dir);
		if (!dr)
			break ;
		if (!hidden && dr->d_name[0] != '.')
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(dr->d_name)));
		if (hidden && dr->d_name[0] == '.' && ft_strncmp(dr->d_name, ".", 2)
			&& ft_strncmp(dr->d_name, "..", 3))
			ft_lstadd_back(&lst, ft_lstnew(ft_strdup(dr->d_name)));
	}
	return (lst);
}

static char	**load_files(t_list *lst)
{
	char	**files;
	t_list	*next;

	files = malloc(sizeof(char *) * ((size_t)ft_lstsize(lst) + 1));
	if (!files)
	{
		while (lst)
		{
			next = lst->next;
			free(lst);
			lst = next;
		}
		exit(EXIT_FAILURE);
	}
	return (files);
}

char	**load_lst(struct dirent *dr, DIR *dir, bool hidden)
{
	char	**files;
	int		i;
	t_list	*lst;
	t_list	*next_lst;

	lst = load_lst_loop(dr, dir, hidden);
	files = load_files(lst);
	i = -1;
	while (++i < ft_lstsize(lst))
		files[i] = lowest(lst);
	files[i] = 0;
	while (lst)
	{
		next_lst = lst->next;
		free(lst);
		lst = next_lst;
	}
	closedir(dir);
	return (files);
}

void	tilde_handler(char **args, int *i, char **envp)
{
	char	*tmp;

	if (args[i[2]][0] == '~' && (args[i[2]][1] == '/' || !args[i[2]][1]))
	{
		tmp = ft_strjoin(ft_getenv("HOME", envp), args[i[2]] + 1);
		free(args[i[2]]);
		args[i[2]] = tmp;
	}
}

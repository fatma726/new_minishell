/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/22 22:10:00 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	maybe_write_exit_file(t_node *node)
{
	const char	*path;
	int			fd;
	char		*s;

	path = getenv("MINISHELL_EXIT_FILE");
	if (!path || !*path)
		return ;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	s = ft_itoa(get_exit_status_n(node));
	if (s)
	{
		write(fd, s, ft_strlen(s));
		free(s);
	}
	close(fd);
}

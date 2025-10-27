/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_state3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"


void	maybe_write_exit_file(void)
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
	s = ft_itoa(get_exit_status());
	if (s)
	{
		write(fd, s, ft_strlen(s));
		free(s);
	}
	close(fd);
}

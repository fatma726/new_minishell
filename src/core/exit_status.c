/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/20 18:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int	_ms_exit_status(int op, int value)
{
	if (op)
		ms_slots()->exit_status = value;
	return (ms_slots()->exit_status);
}

int	get_exit_status(void)
{
	return (_ms_exit_status(0, 0));
}

void	set_exit_status(int status)
{
	(void)_ms_exit_status(1, status);
}

void	write_exit_status_file_if_requested(void)
{
	const char	*path;
	int			fd;
	char		buf[16];
	int			len;

	path = getenv("MINISHELL_EXIT_FILE");
	if (!path || !*path)
		return ;
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return ;
	len = snprintf(buf, sizeof(buf), "%d\n", get_exit_status());
	if (len > 0)
		(void)write(fd, buf, (size_t)len);
	close(fd);
}

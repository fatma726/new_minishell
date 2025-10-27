/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:12 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "mandatory.h"

/* helpers split from heredoc_loop.c to satisfy norm constraints */
char	*clean_delimiter_if_marked(char *delimiter)
{
	if (delimiter && delimiter[0] == (char)WILDMARK)
		return (delimiter + 1);
	return (delimiter);
}

bool	should_expand_vars(char *clean_delimiter)
{
	return (!ft_strchr(clean_delimiter, '"')
		&& !ft_strchr(clean_delimiter, '\''));
}

static char	*process_tty_input(void)
{
	char	*buf;
	char	*line;
	ssize_t	nread;
	size_t	len;

	write(STDOUT_FILENO, "> ", 2);
	buf = malloc(1024);
	if (!buf)
		return (NULL);
	nread = read(STDIN_FILENO, buf, 1023);
	if (nread <= 0)
	{
		free(buf);
		return (NULL);
	}
	buf[nread] = '\0';
	len = ft_strlen(buf);
	if (len > 0 && buf[len - 1] == '\n')
		buf[len - 1] = '\0';
	line = ft_strdup(buf);
	free(buf);
	return (line);
}

char	*get_heredoc_line(void)
{
	char		*buf;
	ssize_t		nread;
	size_t		len;

	if (isatty(STDIN_FILENO))
		return (process_tty_input());
	buf = NULL;
	nread = getline(&buf, &len, stdin);
	if (nread < 0)
	{
		if (buf)
			free(buf);
		return (NULL);
	}
	if (nread > 0 && buf[nread - 1] == '\n')
		buf[nread - 1] = '\0';
	return (buf);
}

int	check_heredoc_signal(void)
{
    if (get_signal_number() == SIGINT)
    {
        /* Only report to caller; do not set status here.
        ** The caller (heredoc_loop) will set exit status to 130 and clear
        ** the signal in a single place to avoid inconsistent states. */
        return (1);
    }
    return (0);
}

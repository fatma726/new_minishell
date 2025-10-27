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

bool	should_expand_vars(char *raw_delimiter)
{
    if (!raw_delimiter)
        return (true);
    if (raw_delimiter[0] == (char)WILDMARK)
        return (false);
    if (ft_strchr(raw_delimiter, '"') || ft_strchr(raw_delimiter, '\''))
        return (false);
    return (true);
}

static char	*process_tty_input(void)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return (NULL);
	return (line);
}

char	*get_heredoc_line(void)
{
	if (isatty(STDIN_FILENO))
		return (process_tty_input());
	/* Non-TTY: read a raw line from stdin (no readline state) */
	return (read_line_non_tty());
}

int	check_heredoc_signal(void)
{
	if (get_signal_number() == SIGINT)
		return (1);
	return (0);
}

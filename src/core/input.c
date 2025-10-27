/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*read_line_non_tty(void)
{
	char	*buf;
	size_t	cap;
	ssize_t	nread;

	buf = NULL;
	cap = 0;
	nread = getline(&buf, &cap, stdin);
	if (nread < 0)
	{
		free(buf);
		return (NULL);
	}
	if (nread > 0 && buf[nread - 1] == '\n')
		buf[nread - 1] = '\0';
	return (buf);
}

void	handle_eof_exit(char **envp, t_node *node)
{
	if (node)
	{
		free(node->pwd);
		free(node->path_fallback);
	}
	if (envp)
		strarrfree(envp);
	clear_history();
	restore_termios();
	maybe_write_exit_file();
	exit(get_exit_status());
}

int	process_read_line(char **result, char **cur_prompt, char *orig)
{
	char	*line;
	char	*tmp_prompt;

	line = NULL;
	tmp_prompt = NULL;
	line = readline(*cur_prompt);
	if (!line)
	{
		free(*result);
		return (-1);
	}
	if (!append_line(result, line))
		return (-1);
	if (quote_check(*result, (int)ft_strlen(*result), NULL) == 0)
		return (0);
	if (*cur_prompt != orig)
		free(*cur_prompt);
	tmp_prompt = ft_strdup("> ");
	if (!tmp_prompt)
		return (-1);
	*cur_prompt = tmp_prompt;
	return (1);
}

char	*get_line(char *str)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		return (read_line_non_tty());
	prompt = ft_strdup(str);
	if (!prompt)
		return (NULL);
	line = get_continuation_line(prompt);
	if (line && !is_blank(line))
		add_history(line);
	free(prompt);
	return (line);
}

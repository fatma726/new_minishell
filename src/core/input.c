/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@student +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdabrahym  #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdabrahym ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	mark_nontext(const char *buf, size_t nread, t_node *node)
{
	size_t			i;
	unsigned char	c;

	i = 0;
	while (i < nread)
	{
		c = (unsigned char)buf[i];
		if (c != '\n' && ((c < 32 && c != '\t') || c >= 128))
		{
			set_nontext_input_n(node, true);
			break ;
		}
		i++;
	}
}

char	*read_line_non_tty(t_node *node)
{
	char	*line;

	line = read_line_fd(STDIN_FILENO);
	if (!line)
		return (NULL);
	mark_nontext(line, ft_strlen(line), node);
	return (line);
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
	restore_termios();
	maybe_write_exit_file(node);
	exit(get_exit_status_n(node));
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

char	*get_line(char *str, t_node *node)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		return (read_line_non_tty(node));
	prompt = ft_strdup(str);
	if (!prompt)
		return (NULL);
	line = get_continuation_line(prompt);
	if (line && !is_blank(line))
		add_history(line);
	free(prompt);
	return (line);
}

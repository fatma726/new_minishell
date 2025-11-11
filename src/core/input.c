/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/09 08:48:19 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*read_line_non_tty(t_node *node)
{
	char			*line;
	size_t			i;
	unsigned char	c;

	line = read_line_fd(STDIN_FILENO);
	if (!line)
		return (NULL);
	i = 0;
	while (line[i])
	{
		c = (unsigned char)line[i];
		if (c != '\n' && ((c < 32 && c != '\t') || c >= 128))
		{
			set_nontext_input_n(node, true);
			break ;
		}
		i++;
	}
	return (line);
}

static void	eof_cleanup_node(t_node *node)
{
	if (node->pwd)
		free(node->pwd);
	if (node->path_fallback)
		free(node->path_fallback);
	strarrfree(node->ori_args);
	strarrfree(node->full_ori_args);
	strarrfree(node->parser_tokens);
	node->parser_tokens = NULL;
	free(node->parser_tmp_str);
	node->parser_tmp_str = NULL;
	if (node->redir_fd >= 0)
	{
		close(node->redir_fd);
		unlink(".temp");
		node->redir_fd = -1;
	}
	if (node->backup_stdin > 2)
		close(node->backup_stdin);
	node->backup_stdin = -1;
	if (node->backup_stdout > 2)
		close(node->backup_stdout);
	node->backup_stdout = -1;
}

void	handle_eof_exit(char **envp, t_node *node)
{
	if (node)
	{
		eof_cleanup_node(node);
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

	line = readline(*cur_prompt);
	if (!line)
	{
		free(*result);
		return (-1);
	}
	if (!append_line(result, line))
		return (-1);
	if (!check_quote_continuation(*result))
		return (0);
	return (setup_continuation_prompt(cur_prompt, orig));
}

char	*get_line(char *str, t_node *node)
{
	char	*line;
	char	*prompt;

	if (!isatty(STDIN_FILENO))
		return (read_line_non_tty(node));
	set_reading_input(true);
	prompt = ft_strdup(str);
	if (!prompt)
	{
		set_reading_input(false);
		return (NULL);
	}
	line = get_continuation_line(prompt);
	set_reading_input(false);
	if (line && !is_blank(line))
		add_history(line);
	free(prompt);
	return (line);
}

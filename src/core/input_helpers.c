/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/08 13:07:34 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

char	*get_continuation_line(char *prompt)
{
	char	*result;
	char	*current_prompt;
	int		st;

	if (!isatty(STDIN_FILENO))
		return (readline(NULL));
	result = NULL;
	current_prompt = prompt;
	while (1)
	{
		st = process_read_line(&result, &current_prompt, prompt);
		if (st < 0)
		{
			if (current_prompt != prompt)
				free(current_prompt);
			result = NULL;
			return (NULL);
		}
		if (st == 0)
		{
			if (current_prompt != prompt)
				free(current_prompt);
			return (result);
		}
	}
}

int	check_quote_continuation(char *result)
{
	size_t	result_len;
	int		quote_state;

	result_len = ft_strlen(result);
	if (result_len == 0)
		return (0);
	quote_state = quote_check(result, (int)result_len - 1, NULL);
	if (quote_state == 0)
		return (0);
	return (1);
}

int	setup_continuation_prompt(char **cur_prompt, char *orig)
{
	char	*tmp_prompt;

	if (*cur_prompt != orig)
		free(*cur_prompt);
	tmp_prompt = ft_strdup("> ");
	if (!tmp_prompt)
		return (-1);
	*cur_prompt = tmp_prompt;
	return (1);
}

int	append_line(char **result, char *line)
{
	char	*tmp;
	char	*joined;

	tmp = NULL;
	joined = NULL;
	if (*result)
	{
		tmp = ft_strjoin(*result, "\n");
		if (!tmp)
			return (free(line), 0);
		free(*result);
		joined = ft_strjoin(tmp, line);
		free(tmp);
		if (!joined)
			return (free(line), 0);
		*result = joined;
	}
	else
	{
		*result = ft_strdup(line);
		if (!*result)
			return (free(line), 0);
	}
	free(line);
	return (1);
}

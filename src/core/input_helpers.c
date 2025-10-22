/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:07 by fatmtahmdab      ###   ########.fr       */
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
			return (NULL);
		}
		if (st == 0)
		{
			if (current_prompt != prompt)
				free(current_prompt);
			return (result);
		}
	}
	return (result);
}

int	append_line(char **result, char *line)
{
	char	*tmp;

	if (*result)
	{
		tmp = ft_strjoin(*result, "\n");
		free(*result);
		*result = ft_strjoin(tmp, line);
		free(tmp);
	}
	else
		*result = ft_strdup(line);
	free(line);
	return (*result != NULL);
}

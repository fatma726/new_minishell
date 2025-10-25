/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_logical_operators.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/24 19:10:23 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static void	handle_oror_operator(char *str, char *result, int *i, int *j)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = '|';
	result[(*j)++] = '|';
	if (str[*i + 2] && str[*i + 2] != ' ')
		result[(*j)++] = ' ';
	*i += 2;
}

static void	handle_andand_operator(char *str, char *result, int *i, int *j)
{
	if (*j > 0 && result[*j - 1] != ' ')
		result[(*j)++] = ' ';
	result[(*j)++] = '&';
	result[(*j)++] = '&';
	if (str[*i + 2] && str[*i + 2] != ' ')
		result[(*j)++] = ' ';
	*i += 2;
}

static void	process_logical_operators(t_logical_state *state)
{
	if (!quote_check(state->str, *state->i, state->node))
	{
		if (state->str[*state->i] == '|' && state->str[*state->i + 1] == '|')
		{
			handle_oror_operator(state->str, state->result, state->i, state->j);
			return ;
		}
		if (state->str[*state->i] == '&' && state->str[*state->i + 1] == '&')
		{
			handle_andand_operator(state->str, state->result, state->i,
				state->j);
			return ;
		}
	}
	state->result[(*state->j)++] = state->str[(*state->i)++];
}

static char	*init_logical_result(char *str, int *len)
{
	char	*result;

	*len = ft_strlen(str);
	result = malloc((*len * 3 + 1) * sizeof(char));
	if (!result)
	{
		free(str);
		return (NULL);
	}
	return (result);
}

char	*add_spaces_around_logical_operators(char *str, t_node *node)
{
	char				*result;
	int					i;
	int					j;
	int					len;
	t_logical_state		state;

	if (!str)
		return (NULL);
	result = init_logical_result(str, &len);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state.str = str;
	state.result = result;
	state.i = &i;
	state.j = &j;
	state.node = node;
	while (str[i])
		process_logical_operators(&state);
	result[j] = '\0';
	free(str);
	return (result);
}

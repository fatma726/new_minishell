/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_exec_error_format.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/06 17:13:05 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

void	print_escaped_cmd(const char *cmd);

static char	*handle_empty_args(char **ori_args)
{
	int	i;

	if (ori_args[0] && ori_args[0][0] == '$' && ori_args[0][1] == '\''
		&& ori_args[0][ft_strlen(ori_args[0]) - 1] == '\'')
		return (ft_strdup(ori_args[0]));
	if (ori_args[0])
	{
		i = 0;
		while (ori_args[0][i] == '\t' || ori_args[0][i] == '\n'
			|| ori_args[0][i] == '\r' || ori_args[0][i] == ' ')
			i++;
		if (ori_args[0][i] == '\0')
			return (ft_strdup(ori_args[0]));
		return (ft_strdup(ori_args[0]));
	}
	return (NULL);
}

static char	*build_multiline_result(char **args)
{
	char	*result;
	int		i;
	int		len;

	len = 0;
	i = 0;
	while (args[i])
		len += ft_strlen(args[i++]) + 1;
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	len = 0;
	i = 0;
	while (args[i])
	{
		if (i > 0)
			result[len++] = '\n';
		ft_strlcpy(result + len, args[i], ft_strlen(args[i]) + 1);
		len += ft_strlen(args[i]);
		i++;
	}
	result[len] = '\0';
	return (result);
}

char	*reconstruct_cmd(char **args, char **ori_args)
{
	if (!args || !ori_args || !ori_args[0])
		return (NULL);
	if (!args[0] || !args[0][0])
		return (handle_empty_args(ori_args));
	return (build_multiline_result(args));
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:10 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

int	handle_escape_char(t_prompt_data *data)
{
	(*data->fmt)++;
	if (**data->fmt == '$')
	{
		if (!data->user || ft_strncmp(data->user, "root", 5) != 0)
			data->new_fmt[data->j++] = '$';
		else
			data->new_fmt[data->j++] = '#';
	}
	else if (**data->fmt == 'W' && data->pwd)
		data->j += (int)ft_strlcpy(data->new_fmt + data->j, data->pwd,
				ft_strlen(data->pwd) + 1);
	else if (**data->fmt == 's')
		data->j += (int)ft_strlcpy(data->new_fmt + data->j, "bash", 5);
	else if (**data->fmt == 'u' && data->user)
		data->j += (int)ft_strlcpy(data->new_fmt + data->j, data->user,
				ft_strlen(data->user) + 1);
	else if (**data->fmt == 'v')
		data->j += (int)ft_strlcpy(data->new_fmt + data->j, "3.2", 4);
	else if (**data->fmt != 'h')
	{
		data->new_fmt[data->j++] = '\\';
		data->new_fmt[data->j++] = **data->fmt;
	}
	return (data->j);
}

void	handle_pwd_path(char **pwd, char **envp)
{
	char		*home;

	home = ft_getenv("HOME", envp);
	if (!home)
		return ;
	if (!ft_strncmp(*pwd, home, ft_strlen(home)))
		*pwd = ft_strjoin("~", *pwd + ft_strlen(home));
}

char	*expand_loop(char *fmt, char *new_fmt, char *user, char *pwd)
{
	t_prompt_data	data;

	data.fmt = &fmt;
	data.new_fmt = new_fmt;
	data.j = 0;
	data.user = user;
	data.pwd = pwd;
	while (*fmt)
	{
		if (*fmt == '\\')
			data.j = handle_escape_char(&data);
		else
			new_fmt[data.j++] = *fmt;
		fmt++;
	}
	return (new_fmt);
}

static int	handle_escape_len(char *fmt, char **envp, char *pwd, int *i)
{
	char	*user;

	if (fmt[*i] == '$')
		return (1);
	else if (fmt[*i] == 'W')
		return ((int)ft_strlen(pwd));
	else if (fmt[*i] == 's')
		return (4);
	else if (fmt[*i] == 'u')
	{
		user = ft_getenv("USER", envp);
		if (user)
			return ((int)ft_strlen(user));
		return (0);
	}
	else if (fmt[*i] == 'v')
		return (3);
	else if (fmt[*i] != 'h')
	{
		(*i)++;
		return (1);
	}
	return (0);
}

int	promptlen(char *fmt, char **envp, char *pwd, int i)
{
	int	l;

	l = 0;
	while (fmt[i])
	{
		if (fmt[i] == '\\')
		{
			i++;
			l += handle_escape_len(fmt, envp, pwd, &i);
		}
		else
			l++;
		i++;
	}
	return (l);
}

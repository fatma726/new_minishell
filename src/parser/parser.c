/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatmtahmdab       #+#    #+#             */
/*   Updated: 2025/11/10 13:31:37 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mandatory.h"

static bool	check_pipe_ampersand_error(char *str, t_node *node)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!quote_check(str, i, node))
		{
			if (str[i] == '|' && str[i + 1] == '&')
			{
				report_syntax_error('|', node);
				return (true);
			}
			if (str[i] == '&' && str[i + 1] == '|')
			{
				report_syntax_error('&', node);
				return (true);
			}
		}
		i++;
	}
	return (false);
}

/* helpers moved to parser_helpers.c */

static char	**parser_expand_and_split(char *str, char **envp, t_node *node)
{
	char	**args;
	char	charset[4];

	node->skip_dollar_q = true;
	str = expand_envvar(str, envp, node);
	node->skip_dollar_q = false;
	str = add_spaces_around_redirections(str, node);
	str = add_spaces_around_semicolons(str, node);
	ft_strlcpy(charset, " \t", 4);
	args = escape_split(str, charset, node);
	node->parser_tokens = args;
	free(str);
	return (args);
}

static char	**process_parser_input(char *str, char **envp, t_node *node)
{
	char	**args;
	char	*tmp;

	tmp = ft_strdup(str);
	if (!tmp)
		return (NULL);
	args = parser_expand_and_split(str, envp, node);
	if (!args)
	{
		free(tmp);
		return (NULL);
	}
	node->ori_args = strarrdup(args);
	node->full_ori_args = node->ori_args;
	free(tmp);
	return (args);
}

/* helpers moved to parser_helpers.c */

/* helpers moved to parser_helpers.c */

static int	proc_hd_parse(char **args, char **envp, t_node *node)
{
	int	i;
	int	ret;

	if (!args || !node || !node->ori_args)
		return (0);
	i = 0;
	ret = 0;
	while (node->ori_args[i] && !ret)
	{
		if (isdlr(node->ori_args[i]) || istlr(node->ori_args[i]))
		{
			if (!node->ori_args[i + 1] || !node->ori_args[i + 1][0])
				return (1);
			if (node->redir_fd < 0 && setup_heredoc_file(node))
				return (1);
			ret = left_double_redir(args, envp, &i, node);
		}
		i++;
	}
	return (ret);
}

char	**parser(char *str, char **envp, t_node *node)
{
	char	**args;

	if (check_pipe_ampersand_error(str, node))
	{
		free(str);
		return (envp);
	}
	if (!isatty(STDIN_FILENO) && get_nontext_input_n(node))
	{
		clear_nontext_input_n(node);
		set_exit_status_n(node, 127);
		free(str);
		return (envp);
	}
	args = process_parser_input(str, envp, node);
	if (handle_parser_errors(args, envp, node))
		return (envp);
	args = split_joined_quote_after_cmd(args);
	if (proc_hd_parse(args, envp, node))
	{
		strarrfree(args);
		return (envp);
	}
	return (process_quotes_and_exec(args, envp, node));
}

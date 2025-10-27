/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bonus.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/10/06 21:32:01 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BONUS_H
# define BONUS_H

# include "mandatory.h"
# include <dirent.h>

/* Internal scanner context for wrapped groups */
typedef struct s_wrap_scan
{
	char		*s;
	t_node		*n;
	size_t		l;
	size_t		r;
	size_t		*inner_start;
	size_t		*inner_len;
}	t_wrap_scan;

/* Logical operators and parentheses */
char	**split_operators(char *s, char **envp, t_node *n);
char	**subshell(char *str, char **envp, t_node *node);
bool	check_wildcard_redirections(char **args);
char	**expand_wildcard_if_bonus(char **a, char **e, t_node *n);
bool	is_redir_token(char *s);
size_t	count_expanded_size(char **args);
char	**split_operators_tail(char *s, size_t i, char **envp, t_node *n);
bool	check_redirection_syntax(char *s, t_node *n);
bool	is_operator_pair(char *s, size_t i, t_node *n);
void	advance_to(char *s, size_t *i, t_node *n, char op);
int		find_top_level_op(char *s, t_node *n);
int		is_wrapped_group(char *s, t_node *n,
			size_t *inner_start, size_t *inner_len);
bool	has_logical_operators_outside_parens(char *s, size_t l, size_t r,
			t_node *n);
bool	starts_invalid(char *s, size_t i);
bool	has_triple_ops(char *s, size_t i);
bool	has_mixed_op_error(char *s, t_node *n);
bool	handle_invalid_start_and_report(char *s, size_t i, t_node *n);
bool	syntax_err_pair(char *s, size_t i, t_node *n, int pair);
char	**handle_syntax_errors(char *s, int i, char **envp, t_node *n);
char	*handle_triple_redir_error(char *str, t_node *node);
char	*handle_paren_error(char *str, int count, t_node *node);
char	*create_clean_string(char *str, t_node *node, int length);
int		validate_parens(char *str, t_node *node, int *length);

/* Wildcard helpers */
typedef struct s_match_data
{
	char	**newargs;
	int		*j;
	char	**matches;
	char	*dir_path;
}	t_match_data;

int		count_matching_files(char *pattern);
int		expand_pattern(char **newargs, int *j, char *pattern);
int		expand_pattern_original(char **newargs, int *j, char *pattern);
int		count_matches_in_dir(char *dir_path, char *file_pattern);
int		collect_matches_with_path(char **matches, char *dir_path,
			char *file_pattern);
int		prepare_pattern_path(char *pattern, char **dir_path,
			char **file_pattern);
int		process_pattern_with_path(char **newargs, int *j, char *dir_path,
			char *file_pattern);
int		count_wildcard_matches(char **args);
int		process_all_args(char **args, char **newargs);
void	cleanup_and_return(char **newargs, int j, char **args);
int		process_wildcard_pattern(char **newargs, int *j, char *pattern);
int		process_regular_arg(char **newargs, int *j, char *arg);
int		matches_pattern(char *filename, char *pattern);
int		collect_matches(char **matches, char *pattern);
int		process_directory_entry(struct dirent *dr, char *pattern);
void	sort_matches(char **matches);
int		add_sorted_matches(char **newargs, int *j, char **matches);
int		add_sorted_matches_with_path(char **newargs, int *j,
			char **matches, char *dir_path);
int		add_matching_file(char **newargs, int *j, char *filename);
void	cleanup_matches(char **matches, int i, DIR *dir);
char	**get_file_list(bool hidden);
void	wildcard_handler(char **args, char **newargs, int *i, t_node *node);
void	tilde_handler(char **args, int *i, char **envp);
char	**load_lst(struct dirent *dr, DIR *dir, bool hidden);

/* Prototypes implemented across wildcard_parser_helpers*.c */
void	break_on_first_pattern(char **args, char ***out, int *i, bool *skip);
void	append_arg_or_expand(char **newargs, int *j, char *arg);
void	process_one_arg(char **args, char **newargs, int *ij, bool *skip_next);
void	build_expanded_args(char **args, char **newargs);
size_t	count_expanded_size(char **args);

/* Error handlers */
bool	check_redir_token(char c, t_node *n);
void	handle_double_ampersand_error_public(t_node *n);
void	handle_double_pipe_error_public(t_node *n);

#endif

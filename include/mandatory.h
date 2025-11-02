/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandatory.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatmtahmdabrahym <fatmtahmdabrahym@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 1970/01/01 00:00:00 by fatima            #+#    #+#             */
/*   Updated: 2025/11/02 00:00:00 by fatmtahmdab      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MANDATORY_H
# define MANDATORY_H

/* system includes */
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>
# include "../libs/Libft/libft.h"

# define WILDMARK 31

/* global slots */
struct s_global_slots
{
	sig_atomic_t	signal_number;
	int				exit_status;
	sig_atomic_t	interactive;
};

struct s_global_slots	*ms_slots(void);

/* core node */
typedef struct s_node
{
	bool	argmode;
	int		backup_stdin;
	int		backup_stdout;
	int		child_die;
	char	*cmd;
	int		echo_skip;
	bool	escape_skip;
	int		exit_flag;
	int		fds[2];
	int		i;
	bool	last;
	int		line_nbr;
	char	**ori_args;
	int		parent_die;
	char	*path;
	char	*path_fallback;
	int		pipe_flag;
	int		pipe_idx;
	char	*pwd;
	int		quota_idx_j;
	int		quota_pipe_cnt;
	int		quota_pipe_idx_arr[10];
	int		redir_fd;
	int		redir_fds[2];
	int		redir_flag;
	int		redir_idx;
	bool	semicolon_sequence;
	int		redir_stop;
	int		right_flag;
	bool	syntax_flag;
	bool	heredoc_unterminated;
	bool	heredoc_swallowed_input;
	bool	is_subshell;
	bool	pipe_word_has_bar;
}	t_node;

/* prompt data */
typedef struct s_prompt_data
{
	char	**fmt;
	char	*new_fmt;
	int		j;
	char	*user;
	char	*pwd;
}	t_prompt_data;

/* quote state */
typedef struct s_quote_state
{
	bool	in_single;
	bool	in_double;
	int		j;
}	t_quote_state;

/* heredoc context */
struct s_hdctx
{
	char	**args;
	bool	has_command;
	char	*delimiter;
	bool	expand_vars;
	char	**envp;
	t_node	*node;
};

/* global state */
int	get_signal_number(void);
void	clear_signal_number(void);
void	set_signal_number(int sig);
bool	is_interactive_mode(void);
void	set_interactive_mode(bool value);
int	_ms_exit_status(int op, int value);
int	get_exit_status(void);
void	set_exit_status(int status);
bool	get_nontext_input(void);
void	set_nontext_input(bool v);
void	clear_nontext_input(void);
void	set_signal(void);
void	set_heredoc_signal(void);
void	set_termios(void);
void	restore_termios(void);
void	maybe_write_exit_file(void);

/* env */
char	*ft_getenv(const char *name, char **envp);
char	**ft_setenv_envp(const char *name, const char *value, char **envp);
char	**ensure_oldpwd_export(char **envp);
char	**setpwd(t_node *node, char **envp);
char	**shlvl_mod(int mod, char **envp);
char	**shlvl_plus_plus(char **envp);

/* strarr utils */
char	**strarradd(char **arr, char *str);
char	**strarrdup(char **strs);
char	**strarradd_take(char **arr, char *owned);
void	strarrfree(char **strs);
size_t	strarrlen(char **strs);

/* core utils */
bool	is_blank(const char *s);
long long	ft_atoll(const char *str);

/* process_command */
int	check_operators_at_depth_zero(const char *s, int i, int *has_andand);
int	scan_for_oror(const char *s, t_node *n, int *has_andand);
char	**dispatch_line(char *hashed, char **envp, t_node *n);
char	**handle_oror_error(char *left, char *right, char *hashed, char **envp);
char	**process_command(char *line, char **envp, t_node *n);
char	**find_command(char **args, char **envp, t_node *node);
char	**dispatch_builtin(char **args, char **envp, t_node *node);
char	**apply_wildcard_phase(
			char **args,
			char **envp,
			t_node *node,
			char *orig);
char	**expand_wildcard(char **args, char **envp, t_node *node);
char	**rm_quotes(char **args, t_node *node);
char	**parser(char *str, char **envp, t_node *node);
char	**check_standalone_operators(char *line, char **envp, t_node *n);
char	*hash_handler(char *str, t_node *node);

/* parser */
typedef struct s_redir_copy_state
{
	int	i;
	int	j;
}	t_redir_copy_state;

typedef struct s_logical_state
{
	char	*str;
	char	*result;
	int		*i;
	int		*j;
	t_node	*node;
}	t_logical_state;

int	quote_check(char const *str, int i, t_node *node);
char	*return_marked_unchanged(char *str);
void	strip_wildmarks_inplace(char **args);
char	*expand_prompt(char *fmt, char **envp, t_node *node);
void	handle_pwd_path(char **pwd, char **envp);
int	promptlen(char *fmt, char **envp, char *pwd, int i);
int	pqh_count_step(char *src, int i, t_quote_state *st, int *l);
int	pqh_count_valid_chars(char *str, t_node *node);
void	process_envvar(char **str, char **envp, t_node *node, int *i);
void	handle_quote_state(char c, bool *in_single, bool *in_double);
char	*process_quotes(char *str, t_node *node);
char	*expand_envvar(char *str, char **envp, t_node *node);
char	*expand_loop(char *fmt, char *new_fmt, char *user, char *pwd);
char	*get_pwd_for_prompt(char **envp, t_node *node);
bool	isp(char *str);
bool	islor(char *str);
bool	isdlr(char *str);
bool	islr(char *str);
bool	isrr(char *str);
bool	isdrr(char *str);
bool	islrr(char *str);
bool	istlr(char *str);
bool	istr(char *str);
bool	isdp(char *str);
bool	isda(char *str);
bool	is_ampersand(char *str);
bool	is_open_paren(char *str);
bool	is_close_paren(char *str);
bool	is_open_brace(char *str);
bool	is_close_brace(char *str);
bool	is_oror(char *str);
bool	is_andand(char *str);
void	report_syntax_error(char c, t_node *node);
char	*add_spaces_around_ampersand(char *str, t_node *node);
char	*add_spaces_around_redirections(char *str, t_node *node);
char	*add_spaces_around_logical_operators(char *str, t_node *node);
char	**escape_split(char *s, char *c, t_node *node);
bool	syntax_check(char **args, char **envp, t_node *node);
void	handle_syntax_error(char **envp, t_node *node);
bool	in_heredoc(char *str, int i);
void	insert_int(char *str, int *i);
void	get_length(char *str, char **envp, int *i, t_node *node);
bool	check_leading_operators_syntax(char **a);
bool	check_trailing_operators_syntax(char **a);
bool	check_consecutive_operators_syntax(char **a);
const char	*get_error_token(char **args);
int	copy_double_left(char *str, char *result, int i, int *j);
int	copy_double_right(char *str, char *result, int i, int *j);
int	copy_single_redir(char *str, char *result, int i, int *j);
const char	*check_leading_operators(char **args);
const char	*check_consecutive_ops(char **args, int i);
const char	*check_triple_redir_split(char **args, int i);
const char	*check_invalid_operator_sequences(char **args, int i);
const char	*check_final_token(char **args, int i);

/* parser helpers (exposed for cross-file use) */
bool	is_echo_with_joined_quote(char *first);
char	**rebuild_echo_with_rest(char **args, char *rest);
char	**split_joined_quote_after_cmd(char **args);
void	sanitize_bar_in_word(char **args, t_node *node);
char	**handle_parser_errors(char **args, char **envp, t_node *node);
char	**process_quotes_and_exec(char **args, char **envp, t_node *node);

/* pipe */
int	prepare_redirections(char **args, char **envp, t_node *node);
int	maybe_setup_pipe(t_node *node);
void	backup_restor(t_node *node);
void	exec_child(char **args, char **envp, t_node *node);
void	exec_parents(char **args, char **envp, t_node *node);
void	run_parent_segment(char **args, char **envp, t_node *node);
int	pipe_check(char **args, t_node *node);
void	init_node(t_node *node);
char	**split_before_pipe_args(char **args, t_node *node);
char	**repeat(char **args, char **envp, t_node *node);
char	**one_commnad(char **args, char **envp, t_node *node);
char	**execute(char **args, char **envp, t_node *node);
char	**handle_parentheses(char **args, char **envp, t_node *node);

/* redirection */
int	heredoc_loop(char **args, char **envp, int *i, t_node *node);
bool	is_redir_check(char *str);
int	left_double_redir(char **args, char **envp, int *i, t_node *node);
int	right_redir(char **args, int *i, t_node *node);
int	right_double_redir(char **args, int *i, t_node *node);
void	argu_left_change(char **args, t_node *node);
void	args_left_move(char **args, int i);
void	args_left_move_i(char **args, t_node *node);
int	left_redir_expand(char **args, int i, t_node *node, char **expanded);
void	handle_echo_skip(char **args, t_node *node);
int	setup_heredoc_file(t_node *node);
void	move_redir_args(char **args, char **ori_args, int *i);
int	print_err2(char **args, int i);
bool	exec_check(char **args, char **envp, t_node *node);
int	left_redir(char **args, char **envp, int *i, t_node *node);
void	cleanup_heredoc_file(t_node *node);
int	left_redir_error_public(const char *s, int type, char *tmp);
char	*clean_delimiter_if_marked(char *delimiter);
bool	should_expand_vars(char *clean_delimiter);
int	process_heredoc_loop(struct s_hdctx *ctx);
char	*get_heredoc_line(void);
int	check_heredoc_signal(void);
void	write_heredoc_line(
		bool expand_vars,
		char *line,
		char **envp,
		t_node *node);
int	finalize_loop_result(
		int lines_read,
		bool got_sigint,
		struct s_hdctx *ctx);
int	open_redir_out(char **args, int i, t_node *node, int flags);
int	exec_redir(char **args, char **envp, t_node *node);
char	*expand_wildcard_redir(char *pattern, t_node *node);
int	redir_chk(char **args);
int	redir_excute(char **args, char **envp, t_node *node, int flag);

/* cmd */
void	cmd_pwd(t_node *node);
void	cmd_echo(char **args, t_node *node);
char	**cmd_env(char **args, char **envs, t_node *node);
char	**cmd_unset(char **args, char **envp, t_node *node);
bool	is_valid_identifier(char *str);
char	**delete_env(char *str, char **envp, t_node *node, int *flag);
char	**cmd_export(char **args, char **envp, t_node *node);
bool	is_valid_identifier_start(char c);
bool	is_valid_identifier_char(char c);
void	print_invalid_identifier_error(char *arg);
void	print_escaped_value(char *value);
char	**cmd_exec(char **args, char **envp, t_node *node);
void	cmd_exit(char **args, char **envp, t_node *node);
bool	handle_exit_with_args(char **args, t_node *node);
bool	ft_isalldigit(char *str);
bool	exit_will_terminate(char **args);
void	handle_numeric_error(char *arg);
void	handle_too_many_args(void);
void	cleanup_child_and_exit(char **args, char **envp, t_node *node);
void	cleanup_and_exit(char **args, char **envp, t_node *node);
char	**cmd_cd(char **args, char **envp, t_node *node);
char	**execute_cd(char **args, char **envp, t_node *node, int offset);
bool	validate_cd_args(char **args, int offset);
char	*expand_tilde(char *path, char **envp);
bool	update_pwd(char *path, t_node *node);
bool	checks(char **args, char **envp, t_node *node, int offset);
char	*build_candidate(char *dir, char *cmd);
bool	is_builtin_command(char **args);
void	exec_proc(char **args, char **envp, t_node *node);
char	**exec_pipe(char *path, char **args, char **envp, t_node *node);
void	chkdir(char **args, char **envp, bool end, t_node *node);
void	handle_unset_option_error(char **args);
void	process_unset_args(char **args, char **envp, t_node *node, int *flag);
char	**export_print(char **envp);
void	handle_export_option_error(char **args);
void	process_export_args(char **args, char ***envp, t_node *node);
bool	process_export_arg(char *arg, char ***envp, t_node *node);
void	handle_env_i_option(char **args, char **envs, t_node *node);
void	exec_error(char **args, char **envp, char **paths, t_node *node);
void	checkdot(char **args, char **envp, t_node *node);
void	handle_absolute_path_error(
		char **args,
		char **envp,
		char **paths,
		t_node *node);
void	handle_relative_path_error(
		char **args,
		char **envp,
		char **paths,
		t_node *node);
void	exec_nopath(t_node *node, char **args, char **envp, char **paths);
void	exec_proc_loop(char **paths, char **args, char **envp, t_node *node);
void	exec_proc_loop2(char **paths, char **args, char **envp, t_node *node);

/* input */
char	*get_line(char *str);
char	*get_continuation_line(char *prompt);
int	append_line(char **result, char *line);
int	process_read_line(char **result, char **cur_prompt, char *orig);
char	*read_line_non_tty(void);
void	handle_eof_exit(char **envp, t_node *node);

#endif

# Minishell Makefile

CC      := cc
CFLAGS  := -Wall -Wextra -Werror -I./include

# Readline (detect macOS homebrew paths)
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    # Align deployment target to avoid ld warnings from mixed SDK mins
    CFLAGS += -I/opt/homebrew/opt/readline/include -mmacosx-version-min=15.0
    READLINE_LIBS := -L/opt/homebrew/opt/readline/lib -lreadline
else
    READLINE_LIBS := -lreadline
endif

# Libft
LIBFT_DIR  := libs/Libft
LIBFT      := $(LIBFT_DIR)/libft.a
LIBFT_SRCS := ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
              ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_strlcpy.c \
              ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c ft_strrchr.c \
              ft_strncmp.c ft_strnstr.c ft_atoi.c ft_calloc.c ft_strdup.c \
              ft_substr.c ft_strjoin.c ft_strtrim.c ft_split.c ft_itoa.c \
              ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
              ft_lstnew.c ft_lstadd_back.c ft_lstsize.c ft_lstlast.c
LIBFT_OBJS := $(addprefix $(LIBFT_DIR)/, $(LIBFT_SRCS:.c=.o))

# Source files
MAIN   = env_utils env_utils2 env_helpers core_state core_state2 core_state3 \
         input input_helpers main process_command process_command_helpers \
         process_command_standalone core_utils strarrutils stubs signals \
         cleanup hash_handler
CMD    = cmd_cd cmd_cd_helpers cmd_cd_utils cmd_cd_utils2 cmd_cd_tilde basic_commands cmd_utils env cmd_exec cmd_exec_helpers \
         cmd_exec_proc cmd_exec_error cmd_exit cmd_exit_helpers cmd_exit_parse \
         cmd_export_consolidated cmd_export_consolidated2 cmd_export_update cmd_export_print \
         unset unset_utils
EXEC   = exec/builtins_dispatch
PARSER = escape_split parser_utils parser parser_wildcard_phase prompt_helpers \
         parser_tokens_redir_basic parser_tokens_consolidated parser_tokens_consolidated2 \
         parser_tokens_checks parser_spacing_amp parser_spacing_redir_helpers \
         parser_spacing_redir parser_spacing_logical parser_spacing_logical_helpers parser_expand_scan parser_quotes_expand parser_quotes_core \
         parser_quotes_helpers parser_quotes_utils syntax_utils syntax_helpers_utils syntax syntax_helpers3 syntax_helpers4
REDIR  = cmd_redir exec_redir heredoc_utils heredoc_helpers heredoc_loop heredoc_norm_utils redir_helpers redir_utils utils_redir \
         utils_redir2 utils_redir3
PIPE   = pipe_core pipe_utils pipe_helpers parentheses

SRCS   = $(addsuffix .c, $(addprefix src/core/, $(MAIN))) \
         $(addsuffix .c, $(addprefix src/cmd/, $(CMD))) \
         $(addsuffix .c, $(addprefix src/, $(EXEC))) \
         $(addsuffix .c, $(addprefix src/parser/, $(PARSER))) \
         $(addsuffix .c, $(addprefix src/redirection/, $(REDIR))) \
         $(addsuffix .c, $(addprefix src/pipe/, $(PIPE)))

OBJ_DIR := .obj
OBJS    := $(SRCS:%.c=$(OBJ_DIR)/%.o)

# Bonus
BONUS_SRCS := bonus/src/bonus/split_operators_main.c \
              bonus/src/bonus/split_operators_consolidated.c \
              bonus/src/bonus/split_operators_error_handlers.c \
              bonus/src/bonus/split_operators_helpers2.c \
              bonus/src/bonus/split_operators_helpers3.c \
              bonus/src/bonus/split_operators_helpers4.c \
              bonus/src/bonus/split_operators_helpers5.c \
              bonus/src/bonus/split_operators_tail.c \
              bonus/src/bonus/subshell_consolidated.c \
              bonus/src/bonus/subshell_consolidated2.c \
              bonus/src/bonus/subshell_main.c \
              bonus/src/bonus/wildcard/wildcard_core.c \
              bonus/src/bonus/wildcard/expand_wildcard_helpers.c \
              bonus/src/bonus/wildcard/expand_wildcard_pattern.c \
              bonus/src/bonus/wildcard/expand_wildcard_sort.c \
			 bonus/src/bonus/wildcard/expand_wildcard_utils3.c \
			 bonus/src/bonus/wildcard/expand_wildcard_utils4.c \
			 bonus/src/bonus/wildcard/expand_wildcard_utils5.c \
			 bonus/src/bonus/wildcard/expand_wildcard_utils6.c \
			 bonus/src/bonus/wildcard/expand_wildcard_loop.c \
              bonus/src/bonus/wildcard/expand_wildcard_redir.c \
              bonus/src/bonus/wildcard/pattern_matching.c \
              bonus/src/bonus/wildcard/get_file_list.c \
              bonus/src/bonus/wildcard/expand_wildcard_utils.c \
              bonus/src/bonus/wildcard/expand_wildcard_utils2.c \
              bonus/src/bonus/wildcard/load_lst.c \
              bonus/src/bonus/wildcard_parser_helpers.c \
              bonus/src/bonus/wildcard_parser_helpers2.c \
              bonus/src/bonus/wildcard_parser_helpers3.c \
              bonus/src/bonus/wildcard_parser_main.c

BONUS_OBJS := $(BONUS_SRCS:%.c=$(OBJ_DIR)/%.o)


# Targets
NAME       := minishell
BONUS_NAME := minishell_bonus

# Default build: mandatory only
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE_LIBS) -o $@

# Bonus build links mandatory + bonus objects and defines BUILD_BONUS for bonus objs
$(BONUS_NAME): $(OBJS) $(BONUS_OBJS) $(LIBFT)
	@$(CC) $(filter-out $(OBJ_DIR)/src/core/stubs.o, $(OBJS)) \
	       $(BONUS_OBJS) -L$(LIBFT_DIR) -lft $(READLINE_LIBS) -o $@

bonus: $(BONUS_NAME)

$(LIBFT): $(LIBFT_OBJS)
	@ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@


$(OBJ_DIR)/bonus/%.o: bonus/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -DBUILD_BONUS -I./bonus/include -c $< -o $@

$(LIBFT_DIR)/%.o: $(LIBFT_DIR)/%.c
	@$(CC) $(CFLAGS) -Wall -Wextra -Werror -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(LIBFT_OBJS)
	@find src bonus -name '*.o' -delete 2>/dev/null || true

fclean: clean
	@rm -f $(NAME) $(BONUS_NAME) $(LIBFT)

re: fclean all

.PHONY: all bonus clean fclean re

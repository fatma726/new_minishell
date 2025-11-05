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
MAIN   = env_utils env_helpers signal_utils exit_file runtime runtime_flags \
         input line_reader input_helpers main process_command \
         process_command_standalone core_utils strarrutils stubs signals \
         cleanup hash_handler shlvl_utils
CMD    = cmd_cd cmd_cd_helpers cmd_cd_utils basic_commands cmd_utils env cmd_exec cmd_exec_helpers \
         cmd_exec_proc cmd_exec_error cmd_exec_error_helpers cmd_exit cmd_exit_helpers cmd_exit_utils cmd_exit_parse \
         cmd_export_helpers cmd_export_update cmd_export_print cmd_export_main \
         unset unset_utils
PARSER = escape_split parser_utils parser_helpers parser_helpers2 parser \
         parser_tokens_redir_basic \
         parser_tokens_checks parser_tokens_consolidated parser_spacing_redir_helpers \
         parser_spacing_redir parser_expand_scan parser_quotes_expand parser_quotes_core \
         parser_quotes_helpers parser_quotes_utils syntax_utils syntax_helpers_utils syntax syntax_helpers3 syntax_helpers4
REDIR  = cmd_redir exec_redir heredoc_utils heredoc_helpers heredoc_loop heredoc_norm_utils redir_helpers redir_utils utils_redir \
         utils_redir2 utils_redir3
PIPE   = pipe_core pipe_utils pipe_utils2 pipe_helpers

SRCS   = $(addsuffix .c, $(addprefix src/core/, $(MAIN))) \
         $(addsuffix .c, $(addprefix src/cmd/, $(CMD))) \
         $(addsuffix .c, $(addprefix src/parser/, $(PARSER))) \
         $(addsuffix .c, $(addprefix src/redirection/, $(REDIR))) \
         $(addsuffix .c, $(addprefix src/pipe/, $(PIPE)))

OBJ_DIR := .obj
OBJS    := $(SRCS:%.c=$(OBJ_DIR)/%.o)


# Targets
NAME       := minishell

# Default build: mandatory only
all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) -L$(LIBFT_DIR) -lft $(READLINE_LIBS) -o $@

$(LIBFT): $(LIBFT_OBJS)
	@ar rcs $@ $^

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@


$(LIBFT_DIR)/%.o: $(LIBFT_DIR)/%.c
	@$(CC) $(CFLAGS) -Wall -Wextra -Werror -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR) $(LIBFT_OBJS)
	@find src -name '*.o' -delete 2>/dev/null || true

fclean: clean
	@rm -f $(NAME) $(LIBFT)

re: fclean all

.PHONY: all clean fclean re

# Create evaluation-ready zip with mandatory-only files
dist:
	@rm -f minishell-v1.0-eval-clean.zip
	@zip -rq minishell-v1.0-eval-clean.zip \
		Makefile \
		.normignore \
		include \
		src \
		libs/Libft

.PHONY: dist

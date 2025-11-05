#!/bin/bash
set -eo pipefail
TIMEOUT_SEC="${TIMEOUT_SEC:-8}"
ARTIFACT_DIR="$PWD/tests/valgrind_logs"
mkdir -p "$ARTIFACT_DIR"
ORIG="$PWD"
tmp=$(mktemp -d)
cleanup() { cd "${ORIG}"; rm -rf "$tmp"; }; trap cleanup EXIT
cd "$tmp"
cp "$ORIG/minishell" .
chmod +x ./minishell || true
echo "Amour Tu es Horrible" > a; echo 0123456789 > b; echo Prout > c; mkdir -p srcs Docs; : > Makefile
VAL_BASE="valgrind --trace-children=yes -s --leak-check=full --show-leak-kinds=all --track-origins=yes --track-fds=yes --error-exitcode=42"
if [ -f "$ORIG/valgrind_script/ignore_readline_leaks.txt" ]; then VAL_SUPP=" --suppressions=$ORIG/valgrind_script/ignore_readline_leaks.txt"; else VAL_SUPP=""; fi
VAL_CMD="$VAL_BASE$VAL_SUPP"
export ARTIFACT_DIR VAL_CMD
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Empty line (enter)
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Empty_line_enter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Only spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Only_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "   " || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Only tabs
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Only_tabs.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "		" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Colon character
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Colon_character.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ":" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Exclamation mark
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Exclamation_mark.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "!" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Single redirect out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Single_redirect_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Single redirect in
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Single_redirect_in.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Double redirect out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Double_redirect_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Heredoc alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Heredoc_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<<" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Mixed redirects
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Mixed_redirects.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Multiple redirect out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Multiple_redirect_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>>>>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Many redirect out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Many_redirect_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>>>>>>>>>>>>>>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Multiple heredoc
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Multiple_heredoc.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<<<<<" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Many heredoc
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Many_heredoc.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<<<<<<<<<<<<<<<<" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Spaced redirect out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Spaced_redirect_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> > > >" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Spaced double redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Spaced_double_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">> >> >> >>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Mixed spaced redirects
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Mixed_spaced_redirects.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>>> >> >> >>" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Root directory as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Root_directory_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Double slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Double_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "//" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Root dot
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Root_dot.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/." || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Complex path traversal
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Complex_path_traversal.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/./../../../../.." || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Many slashes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Many_slashes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "///////" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Dash as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Dash_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "-" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Pipe alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Pipe_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "|" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Pipe at start
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Pipe_at_start.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "| hola" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Multiple pipes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Multiple_pipes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "| | |" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Double pipe
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Double_pipe.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "||" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Many pipes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Many_pipes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "|||||" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Very many pipes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Very_many_pipes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "|||||||||||||" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Mixed operators
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Mixed_operators.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>|><" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Double ampersand
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Double_ampersand.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "&&" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Multiple ampersands
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Multiple_ampersands.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "&&&&&" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Many ampersands
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Many_ampersands.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "&&&&&&&&&&&&&&" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Quoted string as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Quoted_string_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "\"hola\"" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Single quoted as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Single_quoted_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "'hola'" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Non-existent command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Non-existent_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "hola" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: Non-existent with args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/Non-existent_with_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "hola que tal" || true
mkdir -p $ARTIFACT_DIR/syntax
# syntax :: File as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/syntax/File_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "Makefile" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo Hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo without space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_without_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echoHola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo-n without space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo-n_without_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo-nHola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n with text
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_with_text.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n Hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo quoted -n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_quoted_-n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"-n\" Hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n joined
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_joined.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nHola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with -n after
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_-n_after.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo Hola -n" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo multiple words
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_multiple_words.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo Hola Que Tal" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with spaces before
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_spaces_before.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo         Hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with many spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_many_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo    Hola     Que    Tal" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo double -n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_double_-n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n -n" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo double -n with text
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_double_-n_with_text.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n -n Hola Que" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo invalid flag
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_invalid_flag.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -p" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo multiple n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_multiple_n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nnnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo mixed -n flags
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_mixed_-n_flags.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n -nnn -nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo joined -n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_joined_-n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n-nnn -nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n with text and more -n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_with_text_and_more_-n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n -nnn hola -nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo mixed -n joined
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_mixed_-n_joined.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n -nnn-nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo many dashes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_many_dashes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo --------n" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n with dashes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_with_dashes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nnn --------n" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo complex dashes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_complex_dashes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nnn -----nn---nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dashes ending n
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dashes_ending_n.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nnn --------nnnn" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar sign
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_sign.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo exit code
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_exit_code.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $?" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo exit code with dollar
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_exit_code_with_dollar.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $?$" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo exit code piped
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_exit_code_piped.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $? | echo $? | echo $?" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo HOME variable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_HOME_variable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo escaped HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_escaped_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \\$HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with TERM
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_TERM.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo my shit terminal is [$TERM]" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo TERM4 no bracket
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_TERM4_no_bracket.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo my shit terminal is [$TERM4" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo TERM4 with bracket
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_TERM4_with_bracket.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo my shit terminal is [$TERM4]" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo UID
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_UID.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $UID" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo HOME with digit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_HOME_with_digit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $HOME9" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo digit before HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_digit_before_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $9HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo HOME with percent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_HOME_with_percent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $HOME%" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo UID and HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_UID_and_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $UID$HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo sentence with HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_sentence_with_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo Le path de mon HOME est $HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo non-existent var with star
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_non-existent_var_with_star.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $hola*" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo -n with non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_-n_with_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -nnnn $hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with redirect error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_redirect_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo > <" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo with pipe error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_with_pipe_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo | |" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar empty quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_empty_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $\"\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo quoted dollar
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_quoted_dollar.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$\"\"\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single quote mix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_quote_mix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '$'''''" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar quoted HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_quoted_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $\"HOME\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar empty single HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_empty_single_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $''HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar empty double HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_empty_double_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $\"\"HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo partial quoted HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_partial_quoted_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$HO\"ME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single quoted HO
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_quoted_HO.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '$HO'ME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo split quoted HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_split_quoted_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$HO\"\"ME\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single split HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_split_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '$HO''ME'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty before HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_before_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"\"$HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty space HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_space_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"\" $HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single empty HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_empty_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo ''$HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single empty space HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_empty_space_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '' $HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar split HOMME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_split_HOMME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $\"HO\"\"ME\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar single split
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_single_split.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $'HO''ME'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar single HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_single_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $'HOME'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo quoted dollar HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_quoted_dollar_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$\"HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar equals HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_equals_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $=HOME" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar quoted HOLA
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_quoted_HOLA.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $\"HOLA\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo dollar single HOLA
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_dollar_single_HOLA.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $'HOLA'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo non-existent var
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_non-existent_var.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $DONTEXIST Hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo double quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_double_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"hola\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo 'hola'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty quotes around
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_quotes_around.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo ''hola''" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo split quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_split_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo ''h'o'la''" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo double wrap single
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_double_wrap_single.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"''h'o'la''\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo complex quote mix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_complex_quote_mix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"'\"h'o'la\"'\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo joined quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_joined_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo\"'hola'\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo double wrap single hola
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_double_wrap_single_hola.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"'hola'\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo single wrap double
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_single_wrap_double.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '\"hola\"'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo complex quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_complex_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '''ho\"''''l\"a'''" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo many empty doubles
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_many_empty_doubles.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola\"\"\"\"\"\"\"\"\"\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo singles in doubles
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_singles_in_doubles.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola\"''''''''''\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo many empty singles
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_many_empty_singles.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola''''''''''''" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo doubles in singles
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_doubles_in_singles.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola'\"\"\"\"\"\"\"\"\"'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Command with quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Command_with_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "e\"cho hola\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Command with single quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Command_with_single_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "e'cho hola'" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty joined hola
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_joined_hola.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"\"hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty space hola
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_space_hola.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"\" hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty spaces hola
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_spaces_hola.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"\"             hola" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo joined words
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_joined_words.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola\"\"bonjour" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Complex quote command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Complex_quote_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty var Makefile
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_var_Makefile.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$DONTEXIST\"Makefile" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty var quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_var_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$DONTEXIST\"\"Makefile\"" || true
mkdir -p $ARTIFACT_DIR/echo
# echo :: Echo empty var space quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/echo/Echo_empty_var_space_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo \"$DONTEXIST\" \"Makefile\"" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Dollar question as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Dollar_question_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$?" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Double dollar question
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Double_dollar_question.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$?$?" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Question mark HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Question_mark_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "?$HOME" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Dollar alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Dollar_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: HOME as command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/HOME_as_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$HOME" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: HOME with garbage
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/HOME_with_garbage.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$HOMEdskjhfkdshfsd" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Quoted HOME garbage
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Quoted_HOME_garbage.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "\"$HOMEdskjhfkdshfsd\"" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Single quoted HOME garbage
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Single_quoted_HOME_garbage.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "'$HOMEdskjhfkdshfsd'" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Non-existent variable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Non-existent_variable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$DONTEXIST" || true
mkdir -p $ARTIFACT_DIR/variables
# variables :: Multiple variables
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/variables/Multiple_variables.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "$LESS$VAR" || true
mkdir -p $ARTIFACT_DIR/signals
# signals :: Ctrl-C signal
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/signals/Ctrl-C_signal.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "CTRL-C" || true
mkdir -p $ARTIFACT_DIR/signals
# signals :: Ctrl-D exit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/signals/Ctrl-D_exit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "CTRL-D" || true
mkdir -p $ARTIFACT_DIR/signals
# signals :: Ctrl-\ quit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/signals/Ctrl-_quit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "CTRL-\\" || true
mkdir -p $ARTIFACT_DIR/env
# env :: Print environment
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/env/Print_environment.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "env" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export list
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_list.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export with spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_with_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export       HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export without value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_without_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export Hola" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export alphanumeric
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_alphanumeric.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export Hola9hey" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export $DONTEXIST" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export empty quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_empty_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export equals alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_equals_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export =" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export percent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_percent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export %" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export exit code
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_exit_code.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export $?" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export question equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_question_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export ?=2" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export starts with digit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_starts_with_digit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export 9HOLA=" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export digit in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_digit_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA9=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export underscore start
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_underscore_start.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export _HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export multiple underscores
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_multiple_underscores.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export ___HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export underscores mixed
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_underscores_mixed.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export _HO_LA_=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export with at sign
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_with_at_sign.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL@=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export invalid option
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_invalid_option.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export -HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export double dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_double_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export --HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export dash in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_dash_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA-=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export dash middle
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_dash_middle.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HO-LA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export dot in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_dot_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL.A=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export brace in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_brace_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL}A=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export open brace
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_open_brace.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL{A=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export star in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_star_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HO*LA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export hash in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_hash_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HO#LA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export at in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_at_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HO@LA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export with exit code
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_with_exit_code.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HO$?LA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export plus start
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_plus_start.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export +HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export plus middle
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_plus_middle.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL+A=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export space before equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_space_before_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA =bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export spaces around equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_spaces_around_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA = bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export with space in value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_with_space_in_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bon jour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export space after equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_space_after_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA= bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export HOME value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_HOME_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=$HOME" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export concat HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_concat_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bonjour$HOME" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export HOME prefix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_HOME_prefix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=$HOMEbonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export non-existent var
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_non-existent_var.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bon$jour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export at in value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_at_in_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bon@jour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export quoted value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_quoted_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bon\"\"jour\"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export var in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_var_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA$USER=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export multiple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_multiple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bonjour BYE=casse-toi" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export dollar var name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_dollar_var_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export $HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export quoted spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_quoted_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=\"\"bonjour      \"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export -n in value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_-n_in_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=\"\"   -n bonjour   \"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export quotes in quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_quotes_in_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA='\"\"'" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export simple at
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_simple_at.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=at" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export empty before name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_empty_before_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\"\"\" HOLA=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export command in value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_command_in_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=\"\"cat Makefile | grep NAME\"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export spaced value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_spaced_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=\"\"  bonjour  hey  \"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export equals in name
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_equals_in_name.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL=A=bonjour" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export empty value
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_empty_value.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOL=A=\"\"\"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export all empty
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_all_empty.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\"=\"\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export single empty
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_single_empty.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export ''=''" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export triple equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_triple_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"=\"=\"=\"" || true
mkdir -p $ARTIFACT_DIR/export
# export :: Export single equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/export/Export_single_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export '='='='" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset variable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_variable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOLA" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset empty
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_empty.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset \"\"" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset INEXISTANT" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset PWD
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_PWD.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset PWD" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset OLDPWD
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_OLDPWD.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset OLDPWD" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset starts with digit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_starts_with_digit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset 9HOLA" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset ends with digit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_ends_with_digit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOLA9" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with question
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_question.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL?A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOLA=" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with dot
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_dot.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL.A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with plus
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_plus.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL+A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with equals middle
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_equals_middle.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL=A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with open brace
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_open_brace.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL{A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with close brace
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_close_brace.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL}A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL-A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset invalid option
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_invalid_option.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset -HOLA" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset underscore start
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_underscore_start.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset _HOLA" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset underscore middle
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_underscore_middle.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL_A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset underscore end
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_underscore_end.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOLA_" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with star
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_star.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL*A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with hash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_hash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL#A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset dollar var
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_dollar_var.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset $HOLA" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset dollar PWD
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_dollar_PWD.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset $PWD" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with at
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_at.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL@" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with caret
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_caret.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL^A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset with exit code
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_with_exit_code.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOL$?A" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset =" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset many equals
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_many_equals.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset ======" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset many plus
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_many_plus.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset ++++++" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset many underscores
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_many_underscores.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset _______" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset export
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_export.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset export" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset echo" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset pwd" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset cd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_cd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset cd" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset unset
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_unset.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset unset" || true
mkdir -p $ARTIFACT_DIR/unset
# unset :: Unset sudo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/unset/Unset_sudo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset sudo" || true
mkdir -p $ARTIFACT_DIR/binaries
# binaries :: Absolute path echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/binaries/Absolute_path_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/bin/echo" || true
mkdir -p $ARTIFACT_DIR/binaries
# binaries :: Absolute echo with args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/binaries/Absolute_echo_with_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/bin/echo Hola Que Tal" || true
mkdir -p $ARTIFACT_DIR/binaries
# binaries :: Absolute path env
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/binaries/Absolute_path_env.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/bin/env" || true
mkdir -p $ARTIFACT_DIR/binaries
# binaries :: Absolute cd doesn't exist
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/binaries/Absolute_cd_doesnt_exist.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "/bin/cd Desktop" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Print working directory
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Print_working_directory.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd with args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_with_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd hola" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd with path
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_with_path.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd ./hola" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd multiple args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_multiple_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd hola que tal" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd invalid option
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_invalid_option.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd -p" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd double dash option
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_double_dash_option.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd --p" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd triple dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_triple_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd ---p" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd multiple times
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_multiple_times.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd pwd pwd" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd with ls arg
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_with_ls_arg.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd ls" || true
mkdir -p $ARTIFACT_DIR/pwd
# pwd :: Pwd multiple command args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pwd/Pwd_multiple_command_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd ls env" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to HOME
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_HOME.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd current dir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_current_dir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd current with slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_current_with_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ./" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd multiple dots
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_multiple_dots.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ./././." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd multiple dot slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_multiple_dot_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ././././" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd parent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_parent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd .." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd parent with slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_parent_with_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd two parents
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_two_parents.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../.." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd parent and current
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_parent_and_current.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../.." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd mixed traversal
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_mixed_traversal.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd .././././." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd too many arguments
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_too_many_arguments.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd srcs objs" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd single quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_single_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd 'srcs'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd double quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_double_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd \"srcs\"" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd absolute quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_absolute_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd '/etc'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd partial quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_partial_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd /e'tc'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd partial double quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_partial_double_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd /e\"tc\"" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd sr" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to file
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_file.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd Makefile" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd parent then dir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_parent_then_dir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../minishell" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to root via parents
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_root_via_parents.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../../../../../../.." || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to root
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_root.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd /" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to root quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_root_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd '/'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd triple slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_triple_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ///" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd many slashes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_many_slashes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ////////" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd many slashes quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_many_slashes_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd '////////'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd absolute non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_absolute_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd /minishell" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd underscore
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_underscore.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd _" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd to OLDPWD
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_to_OLDPWD.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd -" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd triple dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_triple_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ---" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd HOME variable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_HOME_variable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd $HOME" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd HOME twice
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_HOME_twice.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd $HOME $HOME" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd HOME subdir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_HOME_subdir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd $HOME/42_works" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd PWD quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_PWD_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd \"$PWD/srcs\"" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd PWD single quoted
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_PWD_single_quoted.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd '$PWD/srcs'" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd tilde
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_tilde.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ~" || true
mkdir -p $ARTIFACT_DIR/cd
# cd :: Cd tilde slash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/cd/Cd_tilde_slash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ~/" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Execute non-executable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Execute_non-executable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "./Makefile" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Ls non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Ls_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls hola" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Execute minishell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Execute_minishell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "./minishell" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Env piped to wc
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Env_piped_to_wc.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "env|\"wc\" -l" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Env piped malformed
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Env_piped_malformed.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "env|\"wc \"-l" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Expr addition
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Expr_addition.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "expr 1 + 1" || true
mkdir -p $ARTIFACT_DIR/bastards
# bastards :: Expr with exit codes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/bastards/Expr_with_exit_codes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "expr $? + $?" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit with non-numeric
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_with_non-numeric.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit exit" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit with string
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_with_string.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit hola" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit with multiple strings
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_with_multiple_strings.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit hola que tal" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit with code
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_with_code.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 42" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit with leading zeros
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_with_leading_zeros.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 000042" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit modulo 256
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_modulo_256.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit too many args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_too_many_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 666 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative too many
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_too_many.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -666 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit non-numeric first
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_non-numeric_first.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit hola 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit many args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_many_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 666 666 666 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit numeric then string
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_numeric_then_string.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 666 hola 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit string then numbers
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_string_then_numbers.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit hola 666 666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit 259 mod 256
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_259_mod_256.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 259" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative small
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_small.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -4" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative 42
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_42.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -42" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative with zeros
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_with_zeros.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -0000042" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative 259
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_259.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -259" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit negative 666
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_negative_666.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit plus 666
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_plus_666.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit +666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit zero
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_zero.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 0" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit plus zero
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_plus_zero.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit +0" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit minus zero
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_minus_zero.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -0" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit plus 42
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_plus_42.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit +42" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit two negatives
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_two_negatives.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -69 -96" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit double dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_double_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit --666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit many plus
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_many_plus.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit ++++666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit many plus zero
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_many_plus_zero.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit ++++++0" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit many minus zero
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_many_minus_zero.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit ------0" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit quoted number
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_quoted_number.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit \"666\"" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit single quoted number
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_single_quoted_number.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '666'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit quoted negative
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_quoted_negative.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '-666'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit quoted plus
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_quoted_plus.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '+666'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit quoted many dash
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_quoted_many_dash.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '----666'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit quoted many plus
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_quoted_many_plus.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '++++666'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit split quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_split_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '6'66" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit multiple splits
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_multiple_splits.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '2'66'32'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit nested quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_nested_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit \"'666'\"" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit nested double in single
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_nested_double_in_single.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '\"666\"'" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit complex quotes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_complex_quotes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit '666'\"666\"666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit plus complex
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_plus_complex.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit +'666'\"666\"666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit minus complex
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_minus_complex.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -'666'\"666\"666" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit LONG_MAX
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_LONG_MAX.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 9223372036854775807" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit overflow
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_overflow.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit 9223372036854775808" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit LONG_MIN
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_LONG_MIN.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -9223372036854775808" || true
mkdir -p $ARTIFACT_DIR/exit
# exit :: Exit underflow
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/exit/Exit_underflow.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit -9223372036854775809" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple cats and ls
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_cats_and_ls.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat | cat | cat | ls" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe exit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_exit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | exit" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe exit 42
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_exit_42.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | exit 42" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Exit pipe ls
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Exit_pipe_ls.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit | ls" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo | echo" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe echo args
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_echo_args.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola | echo que tal" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Pwd pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Pwd_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd | echo hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Env pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Env_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "env | echo hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe cat
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_cat.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo oui | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple pipes grep oui
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_pipes_grep_oui.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo oui | echo non | echo hola | grep oui" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple pipes grep non
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_pipes_grep_non.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo oui | echo non | echo hola | grep non" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple pipes grep hola
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_pipes_grep_hola.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo oui | echo non | echo hola | grep hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo multiple cat -e
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_multiple_cat_-e.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola | cat -e | cat -e | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cd pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cd_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd .. | echo \"hola\"" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cd root pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cd_root_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd / | echo \"hola\"" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cd pipe pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cd_pipe_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd .. | pwd" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ifconfig pipe grep
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ifconfig_pipe_grep.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ifconfig | grep \":\"" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ifconfig grep non-match
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ifconfig_grep_non-match.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ifconfig | grep hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Whoami pipe grep USER
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Whoami_pipe_grep_USER.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "whoami | grep $USER" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe ls error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_ls_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | ls hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Double ls pipe error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Double_ls_pipe_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | ls | hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls error ls
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_error_ls.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | hola | ls" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple pipe with error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_pipe_with_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | ls | hola | rev" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Multiple pipe echo rev
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Multiple_pipe_echo_rev.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls | ls | echo hola | rev" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe grep dot
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_grep_dot.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls -la | grep \".\"" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Ls pipe grep quoted dot
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Ls_pipe_grep_quoted_dot.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls -la | grep \"'.'\"" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Many cat -e pipes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Many_cat_-e_pipes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo test.c | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Many cats grep
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Many_cats_grep.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola | cat | cat | cat | cat | cat | grep hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe cat simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_cat_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola | cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola| cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe space after
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_space_after.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola |cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Echo pipe no spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Echo_pipe_no_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola|cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Triple pipe
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Triple_pipe.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola ||| cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Pipe in command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Pipe_in_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ech|o hola | cat" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cat file double cat -e
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cat_file_double_cat_-e.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Makefile | cat -e | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cat grep cat
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cat_grep_cat.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Makefile | grep srcs | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Cat double grep
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Cat_double_grep.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Makefile | grep srcs | grep srcs | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Pipe to cd error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Pipe_to_cd_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Makefile | grep pr | head -n 5 | cd file_not_exist" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Pipe to non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Pipe_to_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Makefile | grep pr | head -n 5 | hello" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Export pipe cats
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Export_pipe_cats.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export HOLA=bonjour | cat -e | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Unset pipe cat
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Unset_pipe_cat.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "unset HOLA | cat -e" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Export pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Export_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export | echo hola" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Sleep pipe sleep
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Sleep_pipe_sleep.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "sleep 3 | sleep 3" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Sleep pipe exit
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Sleep_pipe_exit.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "sleep 3 | exit" || true
mkdir -p $ARTIFACT_DIR/pipes
# pipes :: Exit pipe sleep
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/pipes/Exit_pipe_sleep.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "exit | sleep 3" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect out simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_out_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect append
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_append.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo que tal >> bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect in simple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_in_simple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat < bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd>bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect many spaces
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_many_spaces.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd >                     bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Double redirect space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Double_redirect_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > > bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Double redirect in space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Double_redirect_in_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola < < bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Triple redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Triple_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola >>> bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect before command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_before_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> bonjour echo hola" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Redirect pipe echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Redirect_pipe_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> bonjour | echo hola" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Non-existent redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Non-existent_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "prout hola > bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Multiple redirects
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Multiple_redirects.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > hello >> hello >> hello" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Mixed redirect order
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Mixed_redirect_order.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola >> hello >> hello > hello" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Create file pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Create_file_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> pwd" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Read non-existent pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Read_non-existent_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "< pwd" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat <pwd" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat append no file
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_append_no_file.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat >>" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat triple redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_triple_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat >>>" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat append heredoc
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_append_heredoc.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat >> <<" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat mixed operators
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_mixed_operators.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat >> > >> << >>" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat redirect non-existent
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_redirect_non-existent.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat < ls" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat redirect both
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_redirect_both.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat < ls > ls" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat out then in error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_out_then_in_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat > ls1 < ls2" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Append alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Append_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">>hola" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola >bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat in no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_in_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat <bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect joined
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_joined.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola>bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat in joined
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_in_joined.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat<bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect space after
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_space_after.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola> bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat in space after
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_in_space_after.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat< bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo many spaces redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_many_spaces_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola               >bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat many spaces in
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_many_spaces_in.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat<                     bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo spaces around redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_spaces_around_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola          >     bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat spaces around in
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_spaces_around_in.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat            <         bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect subdir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_subdir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > srcs/bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat in subdir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_in_subdir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat < srcs/bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect subdir no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_subdir_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola >srcs/bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat in subdir no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_in_subdir_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat <srcs/bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Quoted command redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Quoted_command_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "e'c'\"\"h\"\"o hola > bonjour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Cat quoted filename
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Cat_quoted_filename.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat 'bo'\"\"n\"\"jour" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo redirect with extra arg
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_redirect_with_extra_arg.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > bonjour hey" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Multiple input redirects
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Multiple_input_redirects.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<a cat <b <c" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Multiple output redirects
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Multiple_output_redirects.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">a ls >b >>c >d" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo multiple outs
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_multiple_outs.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola > a > b > c" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Input redirect alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Input_redirect_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<a" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Mixed redirects order
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Mixed_redirects_order.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">d cat <a >>e" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Complex redirect order
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Complex_redirect_order.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "< a > b cat > hey >> d" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo double input
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_double_input.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola <bonjour <hello" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Echo inputs only
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Echo_inputs_only.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo <bonjour <hello" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Multiple out with error in
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Multiple_out_with_error_in.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">bonjour >hello <prout" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: Out error out
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/Out_error_out.valgrind.txt" ./minishell > /dev/null 2> /dev/null' ">bonjour <prout >hello" || true
mkdir -p $ARTIFACT_DIR/redirections
# redirections :: In pipe out same file
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/redirections/In_pipe_out_same_file.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<bonjour cat | wc > bonjour" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc with variable
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_with_variable.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << hola
$HOME
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc single quoted delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_single_quoted_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << 'hola'
$HOME
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc double quoted delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_double_quoted_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << \"hola\"
$HOME
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc partial quoted delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_partial_quoted_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << ho\"la\"
$HOME
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc variable delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_variable_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << $HOME
prout
/home/vietdu91
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc with redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_with_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << hola > bonjour
prout
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc piped
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_piped.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << hola | rev
prout
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc alone
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_alone.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<< hola
chola
hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc no space
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_no_space.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<<hola

hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc no delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_no_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat <<" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Multiple heredocs
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Multiple_heredocs.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat << prout << lol << koala
prout
lol
koala
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Non-existent with heredocs
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Non-existent_with_heredocs.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "prout << lol << cat << koala
prout
lol
cat
koala
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc empty var delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_empty_var_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<< $hola
$hola
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Heredoc complex delimiter
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Heredoc_complex_delimiter.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "<< $\"hola\"$\"b\"
holab
" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Here-string
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Here-string.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola <<< bonjour" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Four less-than
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Four_less-than.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola <<<< bonjour" || true
mkdir -p $ARTIFACT_DIR/heredoc
# heredoc :: Five less-than
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/heredoc/Five_less-than.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola <<<<< bonjour" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And operator success
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_operator_success.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd && ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or operator first success
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_operator_first_success.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd || ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or with echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_with_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola || echo bonjour" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And with echo
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_with_echo.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola && echo bonjour" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or first succeeds
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_first_succeeds.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo bonjour || echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And both succeed
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_both_succeed.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo bonjour && echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And with -n flags
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_with_-n_flags.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo -n bonjour && echo -n hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Multiple and operators
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Multiple_and_operators.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd && ls && echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or and mix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_and_mix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd || ls && echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And or mix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_or_mix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd && ls || echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Multiple or operators
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Multiple_or_operators.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "pwd || ls || echo hola" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or with export error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_with_export_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls || export \"\"" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Export error or ls
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Export_error_or_ls.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\" || ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And with export error
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_with_export_error.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls && export \"\"" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Export error and ls
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Export_error_and_ls.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\" && ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Pipe with or
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Pipe_with_or.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat | echo || ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Pipe with and
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Pipe_with_and.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat | echo && ls" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Or then pipe
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Or_then_pipe.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls || cat | echo" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: And then pipe
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/And_then_pipe.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls && cat | echo" || true
mkdir -p $ARTIFACT_DIR/logical
# logical :: Export error and unset
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/logical/Export_error_and_unset.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "export \"\" && unset \"\"" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Simple subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Simple_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Nested subshells
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Nested_subshells.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "( ( ls ) )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Empty subshell with command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Empty_subshell_with_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "( ( ) ls )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: And with subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/And_with_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls && (ls)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell with and
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_with_and.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls && pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Nested subshell and
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Nested_subshell_and.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "( ( ls&&pwd ) )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Nested mixed spacing
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Nested_mixed_spacing.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "( ( ls ) &&pwd )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Complex nesting
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Complex_nesting.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls && ( ( pwd ) ) )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls && pwd) > hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Redirect and without parens
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Redirect_and_without_parens.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> hola ls && pwd" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Redirect before subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Redirect_before_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> hola (ls && pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Redirect in subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Redirect_in_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(> pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Input redirect in subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Input_redirect_in_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(< pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Many nested subshells
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Many_nested_subshells.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "( ( ( ( ( pwd) ) ) ) )" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Empty parens before command
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Empty_parens_before_command.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "() pwd" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Redirect then subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Redirect_then_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "> pwd (ls)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Complex logical subshells
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Complex_logical_subshells.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls||pwd)&&(ls||pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshells with errors
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshells_with_errors.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(lss||pwd)&&(lss||pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: And subshells with errors
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/And_subshells_with_errors.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(lss&&pwd)&&(lss&&pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell pipe redirect
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_pipe_redirect.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls && pwd | wc) > hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell pipe input
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_pipe_input.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(pwd | wc) < hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell and pipe input
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_and_pipe_input.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls && pwd | wc) < hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell error or pipe
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_error_or_pipe.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls -z || pwd | wc) < hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Subshell complex logic
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Subshell_complex_logic.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls -z || pwd && ls)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Mixed subshells pipes
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Mixed_subshells_pipes.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls || (cat Makefile|grep srcs) && (pwd|wc)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Error and subshells
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Error_and_subshells.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls -z && (ls) && (pwd)" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Nested redirects subshell
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Nested_redirects_subshell.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(ls > Docs/hey && pwd) > hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: And with redirects no parens
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/And_with_redirects_no_parens.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls > Docs/hey && pwd > hola" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Cd and pwd twice
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Cd_and_pwd_twice.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd ../.. && pwd && pwd" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Cd subshell and pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Cd_subshell_and_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "(cd ../.. && pwd) && pwd" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Error or cd and pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Error_or_cd_and_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls -z || cd ../../..&&pwd" || true
mkdir -p $ARTIFACT_DIR/parentheses
# parentheses :: Error or subshell cd pwd
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/parentheses/Error_or_subshell_cd_pwd.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls -z || (cd ../../..&&pwd)" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard all
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_all.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls *" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard with dot
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_with_dot.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls *.*" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard no match
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_no_match.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "ls *.hola" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard in filename
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_in_filename.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat M*le" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard no match cat
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_no_match_cat.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat M*ee" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Wildcard middle
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Wildcard_middle.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cat Make*file" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo wildcard
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_wildcard.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo *" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo quoted wildcard
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_quoted_wildcard.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo '*'" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo wildcard prefix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_wildcard_prefix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo D*" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo wildcard suffix no match
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_wildcard_suffix_no_match.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo *Z" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo wildcard with arg
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_wildcard_with_arg.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo *t hola" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo wildcard suffix
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_wildcard_suffix.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo *t" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo dollar wildcard
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_dollar_wildcard.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $*" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo mixed wildcards
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_mixed_wildcards.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo hola*hola *" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo var wildcard
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_var_wildcard.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $hola*" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Echo HOME wildcard
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Echo_HOME_wildcard.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "echo $HOME*" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Cd wildcard single dir
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Cd_wildcard_single_dir.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd *" || true
mkdir -p $ARTIFACT_DIR/wildcards
# wildcards :: Cd wildcard multiple
timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | "$VAL_CMD" --log-file="$ARTIFACT_DIR/wildcards/Cd_wildcard_multiple.valgrind.txt" ./minishell > /dev/null 2> /dev/null' "cd *" || true

#!/usr/bin/env python3
import json


def parse_tests_from_document():
    """Parse all test cases from the document (static list)."""

    tests = []

    # CARACTERES A LA VOLEE (SYNTAXE)
    tests.extend([
        {"cmd": "", "exit": 0, "desc": "Empty line (enter)", "category": "syntax"},
        {"cmd": "   ", "exit": 0, "desc": "Only spaces", "category": "syntax"},
        {"cmd": "\t\t", "exit": 0, "desc": "Only tabs", "category": "syntax"},
        {"cmd": ":", "exit": 0, "desc": "Colon character", "category": "syntax"},
        {"cmd": "!", "exit": 1, "desc": "Exclamation mark", "category": "syntax"},
        {"cmd": ">", "exit": 2, "desc": "Single redirect out", "category": "syntax"},
        {"cmd": "<", "exit": 2, "desc": "Single redirect in", "category": "syntax"},
        {"cmd": ">>", "exit": 2, "desc": "Double redirect out", "category": "syntax"},
        {"cmd": "<<", "exit": 2, "desc": "Heredoc alone", "category": "syntax"},
        {"cmd": "<>", "exit": 2, "desc": "Mixed redirects", "category": "syntax"},
        {"cmd": ">>>>>", "exit": 2, "desc": "Multiple redirect out", "category": "syntax"},
        {"cmd": ">>>>>>>>>>>>>>>", "exit": 2, "desc": "Many redirect out", "category": "syntax"},
        {"cmd": "<<<<<", "exit": 2, "desc": "Multiple heredoc", "category": "syntax"},
        {"cmd": "<<<<<<<<<<<<<<<<", "exit": 2, "desc": "Many heredoc", "category": "syntax"},
        {"cmd": "> > > >", "exit": 2, "desc": "Spaced redirect out", "category": "syntax"},
        {"cmd": ">> >> >> >>", "exit": 2, "desc": "Spaced double redirect", "category": "syntax"},
        {"cmd": ">>>> >> >> >>", "exit": 2, "desc": "Mixed spaced redirects", "category": "syntax"},
        {"cmd": "/", "exit": 126, "desc": "Root directory as command", "category": "syntax"},
        {"cmd": "//", "exit": 126, "desc": "Double slash", "category": "syntax"},
        {"cmd": "/.", "exit": 126, "desc": "Root dot", "category": "syntax"},
        {"cmd": "/./../../../../..", "exit": 126, "desc": "Complex path traversal", "category": "syntax"},
        {"cmd": "///////", "exit": 126, "desc": "Many slashes", "category": "syntax"},
        {"cmd": "-", "exit": 127, "desc": "Dash as command", "category": "syntax"},
        {"cmd": "|", "exit": 2, "desc": "Pipe alone", "category": "syntax"},
        {"cmd": "| hola", "exit": 2, "desc": "Pipe at start", "category": "syntax"},
        {"cmd": "| | |", "exit": 2, "desc": "Multiple pipes", "category": "syntax"},
        {"cmd": "||", "exit": 2, "desc": "Double pipe", "category": "syntax"},
        {"cmd": "|||||", "exit": 2, "desc": "Many pipes", "category": "syntax"},
        {"cmd": "|||||||||||||", "exit": 2, "desc": "Very many pipes", "category": "syntax"},
        {"cmd": ">>|><", "exit": 2, "desc": "Mixed operators", "category": "syntax"},
        {"cmd": "&&", "exit": 2, "desc": "Double ampersand", "category": "syntax", "bonus": True},
        {"cmd": "&&&&&", "exit": 2, "desc": "Multiple ampersands", "category": "syntax", "bonus": True},
        {"cmd": "&&&&&&&&&&&&&&", "exit": 2, "desc": "Many ampersands", "category": "syntax", "bonus": True},
        {"cmd": "\"hola\"", "exit": 127, "desc": "Quoted string as command", "category": "syntax"},
        {"cmd": "'hola'", "exit": 127, "desc": "Single quoted as command", "category": "syntax"},
        {"cmd": "hola", "exit": 127, "desc": "Non-existent command", "category": "syntax"},
        {"cmd": "hola que tal", "exit": 127, "desc": "Non-existent with args", "category": "syntax"},
        {"cmd": "Makefile", "exit": 127, "desc": "File as command", "category": "syntax"},
    ])

    # ECHO
    tests.extend([
        {"cmd": "echo", "exit": 0, "desc": "Echo alone", "category": "echo"},
        {"cmd": "echo -n", "exit": 0, "desc": "Echo -n alone", "category": "echo"},
        {"cmd": "echo Hola", "exit": 0, "desc": "Echo simple", "category": "echo"},
        {"cmd": "echoHola", "exit": 127, "desc": "Echo without space", "category": "echo"},
        {"cmd": "echo-nHola", "exit": 127, "desc": "Echo-n without space", "category": "echo"},
        {"cmd": "echo -n Hola", "exit": 0, "desc": "Echo -n with text", "category": "echo"},
        {"cmd": "echo \"-n\" Hola", "exit": 0, "desc": "Echo quoted -n", "category": "echo"},
        {"cmd": "echo -nHola", "exit": 0, "desc": "Echo -n joined", "category": "echo"},
        {"cmd": "echo Hola -n", "exit": 0, "desc": "Echo with -n after", "category": "echo"},
        {"cmd": "echo Hola Que Tal", "exit": 0, "desc": "Echo multiple words", "category": "echo"},
        {"cmd": "echo         Hola", "exit": 0, "desc": "Echo with spaces before", "category": "echo"},
        {"cmd": "echo    Hola     Que    Tal", "exit": 0, "desc": "Echo with many spaces", "category": "echo"},
        {"cmd": "echo -n -n", "exit": 0, "desc": "Echo double -n", "category": "echo"},
        {"cmd": "echo -n -n Hola Que", "exit": 0, "desc": "Echo double -n with text", "category": "echo"},
        {"cmd": "echo -p", "exit": 0, "desc": "Echo invalid flag", "category": "echo"},
        {"cmd": "echo -nnnnn", "exit": 0, "desc": "Echo multiple n", "category": "echo"},
        {"cmd": "echo -n -nnn -nnnn", "exit": 0, "desc": "Echo mixed -n flags", "category": "echo"},
        {"cmd": "echo -n-nnn -nnnn", "exit": 0, "desc": "Echo joined -n", "category": "echo"},
        {"cmd": "echo -n -nnn hola -nnnn", "exit": 0, "desc": "Echo -n with text and more -n", "category": "echo"},
        {"cmd": "echo -n -nnn-nnnn", "exit": 0, "desc": "Echo mixed -n joined", "category": "echo"},
        {"cmd": "echo --------n", "exit": 0, "desc": "Echo many dashes", "category": "echo"},
        {"cmd": "echo -nnn --------n", "exit": 0, "desc": "Echo -n with dashes", "category": "echo"},
        {"cmd": "echo -nnn -----nn---nnnn", "exit": 0, "desc": "Echo complex dashes", "category": "echo"},
        {"cmd": "echo -nnn --------nnnn", "exit": 0, "desc": "Echo dashes ending n", "category": "echo"},
        {"cmd": "echo $", "exit": 0, "desc": "Echo dollar sign", "category": "echo"},
        {"cmd": "echo $?", "exit": 0, "desc": "Echo exit code", "category": "echo"},
        {"cmd": "echo $?$", "exit": 0, "desc": "Echo exit code with dollar", "category": "echo"},
        {"cmd": "echo $? | echo $? | echo $?", "exit": 0, "desc": "Echo exit code piped", "category": "echo"},
        {"cmd": "echo $HOME", "exit": 0, "desc": "Echo HOME variable", "category": "echo"},
        {"cmd": "echo \\$HOME", "exit": 0, "desc": "Echo escaped HOME", "category": "echo"},
        {"cmd": "echo my shit terminal is [$TERM]", "exit": 0, "desc": "Echo with TERM", "category": "echo"},
        {"cmd": "echo my shit terminal is [$TERM4", "exit": 0, "desc": "Echo TERM4 no bracket", "category": "echo"},
        {"cmd": "echo my shit terminal is [$TERM4]", "exit": 0, "desc": "Echo TERM4 with bracket", "category": "echo"},
        {"cmd": "echo $UID", "exit": 0, "desc": "Echo UID", "category": "echo"},
        {"cmd": "echo $HOME9", "exit": 0, "desc": "Echo HOME with digit", "category": "echo"},
        {"cmd": "echo $9HOME", "exit": 0, "desc": "Echo digit before HOME", "category": "echo"},
        {"cmd": "echo $HOME%", "exit": 0, "desc": "Echo HOME with percent", "category": "echo"},
        {"cmd": "echo $UID$HOME", "exit": 0, "desc": "Echo UID and HOME", "category": "echo"},
        {"cmd": "echo Le path de mon HOME est $HOME", "exit": 0, "desc": "Echo sentence with HOME", "category": "echo"},
        {"cmd": "echo $hola*", "exit": 0, "desc": "Echo non-existent var with star", "category": "echo"},
        {"cmd": "echo -nnnn $hola", "exit": 0, "desc": "Echo -n with non-existent", "category": "echo"},
        {"cmd": "echo > <", "exit": 2, "desc": "Echo with redirect error", "category": "echo"},
        {"cmd": "echo | |", "exit": 2, "desc": "Echo with pipe error", "category": "echo"},
        {"cmd": "echo $\"\"", "exit": 0, "desc": "Echo dollar empty quotes", "category": "echo"},
        {"cmd": "echo \"$\"\"\"", "exit": 0, "desc": "Echo quoted dollar", "category": "echo"},
        {"cmd": "echo '$\'''''", "exit": 0, "desc": "Echo single quote mix", "category": "echo"},
        {"cmd": "echo $\"HOME\"", "exit": 0, "desc": "Echo dollar quoted HOME", "category": "echo"},
        {"cmd": "echo $''HOME", "exit": 0, "desc": "Echo dollar empty single HOME", "category": "echo"},
        {"cmd": "echo $\"\"HOME", "exit": 0, "desc": "Echo dollar empty double HOME", "category": "echo"},
        {"cmd": "echo \"$HO\"ME", "exit": 0, "desc": "Echo partial quoted HOME", "category": "echo"},
        {"cmd": "echo '$HO'ME", "exit": 0, "desc": "Echo single quoted HO", "category": "echo"},
        {"cmd": "echo \"$HO\"\"ME\"", "exit": 0, "desc": "Echo split quoted HOME", "category": "echo"},
        {"cmd": "echo '$HO''ME'", "exit": 0, "desc": "Echo single split HOME", "category": "echo"},
        {"cmd": "echo \"\"$HOME", "exit": 0, "desc": "Echo empty before HOME", "category": "echo"},
        {"cmd": "echo \"\" $HOME", "exit": 0, "desc": "Echo empty space HOME", "category": "echo"},
        {"cmd": "echo ''$HOME", "exit": 0, "desc": "Echo single empty HOME", "category": "echo"},
        {"cmd": "echo '' $HOME", "exit": 0, "desc": "Echo single empty space HOME", "category": "echo"},
        {"cmd": "echo $\"HO\"\"ME\"", "exit": 0, "desc": "Echo dollar split HOMME", "category": "echo"},
        {"cmd": "echo $'HO''ME'", "exit": 0, "desc": "Echo dollar single split", "category": "echo"},
        {"cmd": "echo $'HOME'", "exit": 0, "desc": "Echo dollar single HOME", "category": "echo"},
        {"cmd": "echo \"$\"HOME", "exit": 0, "desc": "Echo quoted dollar HOME", "category": "echo"},
        {"cmd": "echo $=HOME", "exit": 0, "desc": "Echo dollar equals HOME", "category": "echo"},
        {"cmd": "echo $\"HOLA\"", "exit": 0, "desc": "Echo dollar quoted HOLA", "category": "echo"},
        {"cmd": "echo $'HOLA'", "exit": 0, "desc": "Echo dollar single HOLA", "category": "echo"},
        {"cmd": "echo $DONTEXIST Hola", "exit": 0, "desc": "Echo non-existent var", "category": "echo"},
        {"cmd": "echo \"hola\"", "exit": 0, "desc": "Echo double quoted", "category": "echo"},
        {"cmd": "echo 'hola'", "exit": 0, "desc": "Echo single quoted", "category": "echo"},
        {"cmd": "echo ''hola''", "exit": 0, "desc": "Echo empty quotes around", "category": "echo"},
        {"cmd": "echo ''h'o'la''", "exit": 0, "desc": "Echo split quotes", "category": "echo"},
        {"cmd": "echo \"''h'o'la''\"", "exit": 0, "desc": "Echo double wrap single", "category": "echo"},
        {"cmd": "echo \"'\"h'o'la\"'\"", "exit": 0, "desc": "Echo complex quote mix", "category": "echo"},
        {"cmd": "echo\"'hola'\"", "exit": 0, "desc": "Echo joined quoted", "category": "echo"},
        {"cmd": "echo \"'hola'\"", "exit": 0, "desc": "Echo double wrap single hola", "category": "echo"},
        {"cmd": "echo '\"hola\"'", "exit": 0, "desc": "Echo single wrap double", "category": "echo"},
        {"cmd": "echo '''ho\"''''l\"a'''", "exit": 0, "desc": "Echo complex quotes", "category": "echo"},
        {"cmd": "echo hola\"\"\"\"\"\"\"\"\"\"", "exit": 0, "desc": "Echo many empty doubles", "category": "echo"},
        {"cmd": "echo hola\"''''''''''\"", "exit": 0, "desc": "Echo singles in doubles", "category": "echo"},
        {"cmd": "echo hola''''''''''''", "exit": 0, "desc": "Echo many empty singles", "category": "echo"},
        {"cmd": "echo hola'\"\"\"\"\"\"\"\"\"'", "exit": 0, "desc": "Echo doubles in singles", "category": "echo"},
        {"cmd": "e\"cho hola\"", "exit": 127, "desc": "Command with quotes", "category": "echo"},
        {"cmd": "e'cho hola'", "exit": 127, "desc": "Command with single quotes", "category": "echo"},
        {"cmd": "echo \"\"hola", "exit": 0, "desc": "Echo empty joined hola", "category": "echo"},
        {"cmd": "echo \"\" hola", "exit": 0, "desc": "Echo empty space hola", "category": "echo"},
        {"cmd": "echo \"\"             hola", "exit": 0, "desc": "Echo empty spaces hola", "category": "echo"},
        {"cmd": "echo hola\"\"bonjour", "exit": 0, "desc": "Echo joined words", "category": "echo"},
        {"cmd": "\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r", "exit": 0, "desc": "Complex quote command", "category": "echo"},
        {"cmd": "echo \"$DONTEXIST\"Makefile", "exit": 0, "desc": "Echo empty var Makefile", "category": "echo"},
        {"cmd": "echo \"$DONTEXIST\"\"Makefile\"", "exit": 0, "desc": "Echo empty var quoted", "category": "echo"},
        {"cmd": "echo \"$DONTEXIST\" \"Makefile\"", "exit": 0, "desc": "Echo empty var space quoted", "category": "echo"},
    ])

    # DOLLAR SIGN / VARIABLES
    tests.extend([
        {"cmd": "$?", "exit": 127, "desc": "Dollar question as command", "category": "variables"},
        {"cmd": "$?$?", "exit": 127, "desc": "Double dollar question", "category": "variables"},
        {"cmd": "?$HOME", "exit": 127, "desc": "Question mark HOME", "category": "variables"},
        {"cmd": "$", "exit": 127, "desc": "Dollar alone", "category": "variables"},
        {"cmd": "$HOME", "exit": 126, "desc": "HOME as command", "category": "variables"},
        {"cmd": "$HOMEdskjhfkdshfsd", "exit": 0, "desc": "HOME with garbage", "category": "variables"},
        {"cmd": "\"$HOMEdskjhfkdshfsd\"", "exit": 127, "desc": "Quoted HOME garbage", "category": "variables"},
        {"cmd": "'$HOMEdskjhfkdshfsd'", "exit": 127, "desc": "Single quoted HOME garbage", "category": "variables"},
        {"cmd": "$DONTEXIST", "exit": 0, "desc": "Non-existent variable", "category": "variables"},
        {"cmd": "$LESS$VAR", "exit": 127, "desc": "Multiple variables", "category": "variables"},
    ])

    # SIGNALS (interactive)
    tests.extend([
        {"cmd": "CTRL-C", "exit": 130, "desc": "Ctrl-C signal", "category": "signals", "interactive": True},
        {"cmd": "CTRL-D", "exit": 0, "desc": "Ctrl-D exit", "category": "signals", "interactive": True},
        {"cmd": "CTRL-\\", "exit": 131, "desc": "Ctrl-\\ quit", "category": "signals", "interactive": True},
    ])

    # ENV & EXPORT & UNSET
    tests.extend([
        {"cmd": "env", "exit": 0, "desc": "Print environment", "category": "env"},
        {"cmd": "export", "exit": 0, "desc": "Export list", "category": "export"},
        {"cmd": "export HOLA=bonjour", "exit": 0, "desc": "Export simple", "category": "export"},
        {"cmd": "export       HOLA=bonjour", "exit": 0, "desc": "Export with spaces", "category": "export"},
        {"cmd": "export Hola", "exit": 0, "desc": "Export without value", "category": "export"},
        {"cmd": "export Hola9hey", "exit": 0, "desc": "Export alphanumeric", "category": "export"},
        {"cmd": "export $DONTEXIST", "exit": 0, "desc": "Export non-existent", "category": "export"},
        {"cmd": "export \"\"", "exit": 1, "desc": "Export empty quotes", "category": "export"},
        {"cmd": "export =", "exit": 1, "desc": "Export equals alone", "category": "export"},
        {"cmd": "export %", "exit": 1, "desc": "Export percent", "category": "export"},
        {"cmd": "export $?", "exit": 1, "desc": "Export exit code", "category": "export"},
        {"cmd": "export ?=2", "exit": 1, "desc": "Export question equals", "category": "export"},
        {"cmd": "export 9HOLA=", "exit": 1, "desc": "Export starts with digit", "category": "export"},
        {"cmd": "export HOLA9=bonjour", "exit": 0, "desc": "Export digit in name", "category": "export"},
        {"cmd": "export _HOLA=bonjour", "exit": 0, "desc": "Export underscore start", "category": "export"},
        {"cmd": "export ___HOLA=bonjour", "exit": 0, "desc": "Export multiple underscores", "category": "export"},
        {"cmd": "export _HO_LA_=bonjour", "exit": 0, "desc": "Export underscores mixed", "category": "export"},
        {"cmd": "export HOL@=bonjour", "exit": 1, "desc": "Export with at sign", "category": "export"},
        {"cmd": "export -HOLA=bonjour", "exit": 2, "desc": "Export invalid option", "category": "export"},
        {"cmd": "export --HOLA=bonjour", "exit": 2, "desc": "Export double dash", "category": "export"},
        {"cmd": "export HOLA-=bonjour", "exit": 1, "desc": "Export dash in name", "category": "export"},
        {"cmd": "export HO-LA=bonjour", "exit": 1, "desc": "Export dash middle", "category": "export"},
        {"cmd": "export HOL.A=bonjour", "exit": 1, "desc": "Export dot in name", "category": "export"},
        {"cmd": "export HOL}A=bonjour", "exit": 1, "desc": "Export brace in name", "category": "export"},
        {"cmd": "export HOL{A=bonjour", "exit": 1, "desc": "Export open brace", "category": "export"},
        {"cmd": "export HO*LA=bonjour", "exit": 1, "desc": "Export star in name", "category": "export"},
        {"cmd": "export HO#LA=bonjour", "exit": 1, "desc": "Export hash in name", "category": "export"},
        {"cmd": "export HO@LA=bonjour", "exit": 1, "desc": "Export at in name", "category": "export"},
        {"cmd": "export HO$?LA=bonjour", "exit": 0, "desc": "Export with exit code", "category": "export"},
        {"cmd": "export +HOLA=bonjour", "exit": 1, "desc": "Export plus start", "category": "export"},
        {"cmd": "export HOL+A=bonjour", "exit": 1, "desc": "Export plus middle", "category": "export"},
        {"cmd": "export HOLA =bonjour", "exit": 1, "desc": "Export space before equals", "category": "export"},
        {"cmd": "export HOLA = bonjour", "exit": 1, "desc": "Export spaces around equals", "category": "export"},
        {"cmd": "export HOLA=bon jour", "exit": 0, "desc": "Export with space in value", "category": "export"},
        {"cmd": "export HOLA= bonjour", "exit": 0, "desc": "Export space after equals", "category": "export"},
        {"cmd": "export HOLA=$HOME", "exit": 0, "desc": "Export HOME value", "category": "export"},
        {"cmd": "export HOLA=bonjour$HOME", "exit": 0, "desc": "Export concat HOME", "category": "export"},
        {"cmd": "export HOLA=$HOMEbonjour", "exit": 0, "desc": "Export HOME prefix", "category": "export"},
        {"cmd": "export HOLA=bon$jour", "exit": 0, "desc": "Export non-existent var", "category": "export"},
        {"cmd": "export HOLA=bon@jour", "exit": 0, "desc": "Export at in value", "category": "export"},
        {"cmd": "export HOLA=bon\"\"jour\"\"", "exit": 0, "desc": "Export quoted value", "category": "export"},
        {"cmd": "export HOLA$USER=bonjour", "exit": 0, "desc": "Export var in name", "category": "export"},
        {"cmd": "export HOLA=bonjour BYE=casse-toi", "exit": 0, "desc": "Export multiple", "category": "export"},
        {"cmd": "export $HOLA=bonjour", "exit": 0, "desc": "Export dollar var name", "category": "export"},
        {"cmd": "export HOLA=\"\"bonjour      \"\"", "exit": 0, "desc": "Export quoted spaces", "category": "export"},
        {"cmd": "export HOLA=\"\"   -n bonjour   \"\"", "exit": 0, "desc": "Export -n in value", "category": "export"},
        {"cmd": "export HOLA='\"\"'", "exit": 0, "desc": "Export quotes in quotes", "category": "export"},
        {"cmd": "export HOLA=at", "exit": 0, "desc": "Export simple at", "category": "export"},
        {"cmd": "export \"\"\"\" HOLA=bonjour", "exit": 1, "desc": "Export empty before name", "category": "export"},
        {"cmd": "export HOLA=\"\"cat Makefile | grep NAME\"\"", "exit": 0, "desc": "Export command in value", "category": "export"},
        {"cmd": "export HOLA=\"\"  bonjour  hey  \"\"", "exit": 0, "desc": "Export spaced value", "category": "export"},
        {"cmd": "export HOL=A=bonjour", "exit": 0, "desc": "Export equals in name", "category": "export"},
        {"cmd": "export HOL=A=\"\"\"\"", "exit": 0, "desc": "Export empty value", "category": "export"},
        {"cmd": "export \"\"=\"\"", "exit": 1, "desc": "Export all empty", "category": "export"},
        {"cmd": "export ''=''", "exit": 1, "desc": "Export single empty", "category": "export"},
        {"cmd": "export \"=\"=\"=\"", "exit": 1, "desc": "Export triple equals", "category": "export"},
        {"cmd": "export '='='='", "exit": 1, "desc": "Export single equals", "category": "export"},
        # unset
        {"cmd": "unset HOLA", "exit": 0, "desc": "Unset variable", "category": "unset"},
        {"cmd": "unset \"\"", "exit": 1, "desc": "Unset empty", "category": "unset"},
        {"cmd": "unset INEXISTANT", "exit": 0, "desc": "Unset non-existent", "category": "unset"},
        {"cmd": "unset PWD", "exit": 0, "desc": "Unset PWD", "category": "unset"},
        {"cmd": "unset OLDPWD", "exit": 0, "desc": "Unset OLDPWD", "category": "unset"},
        {"cmd": "unset 9HOLA", "exit": 1, "desc": "Unset starts with digit", "category": "unset"},
        {"cmd": "unset HOLA9", "exit": 0, "desc": "Unset ends with digit", "category": "unset"},
        {"cmd": "unset HOL?A", "exit": 1, "desc": "Unset with question", "category": "unset"},
        {"cmd": "unset HOLA=", "exit": 1, "desc": "Unset with equals", "category": "unset"},
        {"cmd": "unset HOL.A", "exit": 1, "desc": "Unset with dot", "category": "unset"},
        {"cmd": "unset HOL+A", "exit": 1, "desc": "Unset with plus", "category": "unset"},
        {"cmd": "unset HOL=A", "exit": 1, "desc": "Unset with equals middle", "category": "unset"},
        {"cmd": "unset HOL{A", "exit": 1, "desc": "Unset with open brace", "category": "unset"},
        {"cmd": "unset HOL}A", "exit": 1, "desc": "Unset with close brace", "category": "unset"},
        {"cmd": "unset HOL-A", "exit": 1, "desc": "Unset with dash", "category": "unset"},
        {"cmd": "unset -HOLA", "exit": 2, "desc": "Unset invalid option", "category": "unset"},
        {"cmd": "unset _HOLA", "exit": 0, "desc": "Unset underscore start", "category": "unset"},
        {"cmd": "unset HOL_A", "exit": 0, "desc": "Unset underscore middle", "category": "unset"},
        {"cmd": "unset HOLA_", "exit": 0, "desc": "Unset underscore end", "category": "unset"},
        {"cmd": "unset HOL*A", "exit": 1, "desc": "Unset with star", "category": "unset"},
        {"cmd": "unset HOL#A", "exit": 1, "desc": "Unset with hash", "category": "unset"},
        {"cmd": "unset $HOLA", "exit": 0, "desc": "Unset dollar var", "category": "unset"},
        {"cmd": "unset $PWD", "exit": 1, "desc": "Unset dollar PWD", "category": "unset"},
        {"cmd": "unset HOL@", "exit": 1, "desc": "Unset with at", "category": "unset"},
        {"cmd": "unset HOL^A", "exit": 1, "desc": "Unset with caret", "category": "unset"},
        {"cmd": "unset HOL$?A", "exit": 0, "desc": "Unset with exit code", "category": "unset"},
        {"cmd": "unset =", "exit": 0, "desc": "Unset equals", "category": "unset"},
        {"cmd": "unset ======", "exit": 0, "desc": "Unset many equals", "category": "unset"},
        {"cmd": "unset ++++++", "exit": 0, "desc": "Unset many plus", "category": "unset"},
        {"cmd": "unset _______", "exit": 0, "desc": "Unset many underscores", "category": "unset"},
        {"cmd": "unset export", "exit": 0, "desc": "Unset export", "category": "unset"},
        {"cmd": "unset echo", "exit": 0, "desc": "Unset echo", "category": "unset"},
        {"cmd": "unset pwd", "exit": 0, "desc": "Unset pwd", "category": "unset"},
        {"cmd": "unset cd", "exit": 0, "desc": "Unset cd", "category": "unset"},
        {"cmd": "unset unset", "exit": 0, "desc": "Unset unset", "category": "unset"},
        {"cmd": "unset sudo", "exit": 0, "desc": "Unset sudo", "category": "unset"},
    ])

    # BINARY FILES
    tests.extend([
        {"cmd": "/bin/echo", "exit": 0, "desc": "Absolute path echo", "category": "binaries"},
        {"cmd": "/bin/echo Hola Que Tal", "exit": 0, "desc": "Absolute echo with args", "category": "binaries"},
        {"cmd": "/bin/env", "exit": 0, "desc": "Absolute path env", "category": "binaries"},
        {"cmd": "/bin/cd Desktop", "exit": 127, "desc": "Absolute cd doesn't exist", "category": "binaries"},
    ])

    # PWD
    tests.extend([
        {"cmd": "pwd", "exit": 0, "desc": "Print working directory", "category": "pwd"},
        {"cmd": "pwd hola", "exit": 0, "desc": "Pwd with args", "category": "pwd"},
        {"cmd": "pwd ./hola", "exit": 0, "desc": "Pwd with path", "category": "pwd"},
        {"cmd": "pwd hola que tal", "exit": 0, "desc": "Pwd multiple args", "category": "pwd"},
        {"cmd": "pwd -p", "exit": 2, "desc": "Pwd invalid option", "category": "pwd"},
        {"cmd": "pwd --p", "exit": 2, "desc": "Pwd double dash option", "category": "pwd"},
        {"cmd": "pwd ---p", "exit": 2, "desc": "Pwd triple dash", "category": "pwd"},
        {"cmd": "pwd pwd pwd", "exit": 0, "desc": "Pwd multiple times", "category": "pwd"},
        {"cmd": "pwd ls", "exit": 0, "desc": "Pwd with ls arg", "category": "pwd"},
        {"cmd": "pwd ls env", "exit": 0, "desc": "Pwd multiple command args", "category": "pwd"},
    ])

    # CD
    tests.extend([
        {"cmd": "cd", "exit": 0, "desc": "Cd to HOME", "category": "cd"},
        {"cmd": "cd .", "exit": 0, "desc": "Cd current dir", "category": "cd"},
        {"cmd": "cd ./", "exit": 0, "desc": "Cd current with slash", "category": "cd"},
        {"cmd": "cd ./././.", "exit": 0, "desc": "Cd multiple dots", "category": "cd"},
        {"cmd": "cd ././././", "exit": 0, "desc": "Cd multiple dot slash", "category": "cd"},
        {"cmd": "cd ..", "exit": 0, "desc": "Cd parent", "category": "cd"},
        {"cmd": "cd ../", "exit": 0, "desc": "Cd parent with slash", "category": "cd"},
        {"cmd": "cd ../..", "exit": 0, "desc": "Cd two parents", "category": "cd"},
        {"cmd": "cd ../..", "exit": 0, "desc": "Cd parent and current", "category": "cd"},
        {"cmd": "cd .././././.", "exit": 0, "desc": "Cd mixed traversal", "category": "cd"},
        {"cmd": "cd srcs objs", "exit": 1, "desc": "Cd too many arguments", "category": "cd"},
        {"cmd": "cd 'srcs'", "exit": 0, "desc": "Cd single quoted", "category": "cd"},
        {"cmd": "cd \"srcs\"", "exit": 0, "desc": "Cd double quoted", "category": "cd"},
        {"cmd": "cd '/etc'", "exit": 0, "desc": "Cd absolute quoted", "category": "cd"},
        {"cmd": "cd /e'tc'", "exit": 0, "desc": "Cd partial quoted", "category": "cd"},
        {"cmd": "cd /e\"tc\"", "exit": 0, "desc": "Cd partial double quoted", "category": "cd"},
        {"cmd": "cd sr", "exit": 1, "desc": "Cd non-existent", "category": "cd"},
        {"cmd": "cd Makefile", "exit": 1, "desc": "Cd to file", "category": "cd"},
        {"cmd": "cd ../minishell", "exit": 0, "desc": "Cd parent then dir", "category": "cd"},
        {"cmd": "cd ../../../../../../..", "exit": 0, "desc": "Cd to root via parents", "category": "cd"},
        {"cmd": "cd /", "exit": 0, "desc": "Cd to root", "category": "cd"},
        {"cmd": "cd '/'", "exit": 0, "desc": "Cd to root quoted", "category": "cd"},
        {"cmd": "cd ///", "exit": 0, "desc": "Cd triple slash", "category": "cd"},
        {"cmd": "cd ////////", "exit": 0, "desc": "Cd many slashes", "category": "cd"},
        {"cmd": "cd '////////'", "exit": 0, "desc": "Cd many slashes quoted", "category": "cd"},
        {"cmd": "cd /minishell", "exit": 1, "desc": "Cd absolute non-existent", "category": "cd"},
        {"cmd": "cd _", "exit": 1, "desc": "Cd underscore", "category": "cd"},
        {"cmd": "cd -", "exit": 0, "desc": "Cd to OLDPWD", "category": "cd"},
        {"cmd": "cd ---", "exit": 2, "desc": "Cd triple dash", "category": "cd"},
        {"cmd": "cd $HOME", "exit": 0, "desc": "Cd HOME variable", "category": "cd"},
        {"cmd": "cd $HOME $HOME", "exit": 1, "desc": "Cd HOME twice", "category": "cd"},
        {"cmd": "cd $HOME/42_works", "exit": 0, "desc": "Cd HOME subdir", "category": "cd"},
        {"cmd": "cd \"$PWD/srcs\"", "exit": 0, "desc": "Cd PWD quoted", "category": "cd"},
        {"cmd": "cd '$PWD/srcs'", "exit": 1, "desc": "Cd PWD single quoted", "category": "cd"},
        {"cmd": "cd ~", "exit": 0, "desc": "Cd tilde", "category": "cd", "bonus": True},
        {"cmd": "cd ~/", "exit": 0, "desc": "Cd tilde slash", "category": "cd", "bonus": True},
    ])

    # BASTARDS
    tests.extend([
        {"cmd": "./Makefile", "exit": 126, "desc": "Execute non-executable", "category": "bastards"},
        {"cmd": "ls hola", "exit": 2, "desc": "Ls non-existent", "category": "bastards"},
        {"cmd": "./minishell", "exit": 0, "desc": "Execute minishell", "category": "bastards"},
        {"cmd": "env|\"wc\" -l", "exit": 0, "desc": "Env piped to wc", "category": "bastards"},
        {"cmd": "env|\"wc \"-l", "exit": 127, "desc": "Env piped malformed", "category": "bastards"},
        {"cmd": "expr 1 + 1", "exit": 0, "desc": "Expr addition", "category": "bastards"},
        {"cmd": "expr $? + $?", "exit": 0, "desc": "Expr with exit codes", "category": "bastards"},
    ])

    # EXIT
    tests.extend([
        {"cmd": "exit", "exit": 0, "desc": "Exit simple", "category": "exit"},
        {"cmd": "exit exit", "exit": 2, "desc": "Exit with non-numeric", "category": "exit"},
        {"cmd": "exit hola", "exit": 2, "desc": "Exit with string", "category": "exit"},
        {"cmd": "exit hola que tal", "exit": 2, "desc": "Exit with multiple strings", "category": "exit"},
        {"cmd": "exit 42", "exit": 42, "desc": "Exit with code", "category": "exit"},
        {"cmd": "exit 000042", "exit": 42, "desc": "Exit with leading zeros", "category": "exit"},
        {"cmd": "exit 666", "exit": 154, "desc": "Exit modulo 256", "category": "exit"},
        {"cmd": "exit 666 666", "exit": 1, "desc": "Exit too many args", "category": "exit"},
        {"cmd": "exit -666 666", "exit": 1, "desc": "Exit negative too many", "category": "exit"},
        {"cmd": "exit hola 666", "exit": 2, "desc": "Exit non-numeric first", "category": "exit"},
        {"cmd": "exit 666 666 666 666", "exit": 1, "desc": "Exit many args", "category": "exit"},
        {"cmd": "exit 666 hola 666", "exit": 1, "desc": "Exit numeric then string", "category": "exit"},
        {"cmd": "exit hola 666 666", "exit": 2, "desc": "Exit string then numbers", "category": "exit"},
        {"cmd": "exit 259", "exit": 3, "desc": "Exit 259 mod 256", "category": "exit"},
        {"cmd": "exit -4", "exit": 252, "desc": "Exit negative small", "category": "exit"},
        {"cmd": "exit -42", "exit": 214, "desc": "Exit negative 42", "category": "exit"},
        {"cmd": "exit -0000042", "exit": 214, "desc": "Exit negative with zeros", "category": "exit"},
        {"cmd": "exit -259", "exit": 253, "desc": "Exit negative 259", "category": "exit"},
        {"cmd": "exit -666", "exit": 102, "desc": "Exit negative 666", "category": "exit"},
        {"cmd": "exit +666", "exit": 154, "desc": "Exit plus 666", "category": "exit"},
        {"cmd": "exit 0", "exit": 0, "desc": "Exit zero", "category": "exit"},
        {"cmd": "exit +0", "exit": 0, "desc": "Exit plus zero", "category": "exit"},
        {"cmd": "exit -0", "exit": 0, "desc": "Exit minus zero", "category": "exit"},
        {"cmd": "exit +42", "exit": 42, "desc": "Exit plus 42", "category": "exit"},
        {"cmd": "exit -69 -96", "exit": 1, "desc": "Exit two negatives", "category": "exit"},
        {"cmd": "exit --666", "exit": 2, "desc": "Exit double dash", "category": "exit"},
        {"cmd": "exit ++++666", "exit": 2, "desc": "Exit many plus", "category": "exit"},
        {"cmd": "exit ++++++0", "exit": 2, "desc": "Exit many plus zero", "category": "exit"},
        {"cmd": "exit ------0", "exit": 2, "desc": "Exit many minus zero", "category": "exit"},
        {"cmd": "exit \"666\"", "exit": 154, "desc": "Exit quoted number", "category": "exit"},
        {"cmd": "exit '666'", "exit": 154, "desc": "Exit single quoted number", "category": "exit"},
        {"cmd": "exit '-666'", "exit": 102, "desc": "Exit quoted negative", "category": "exit"},
        {"cmd": "exit '+666'", "exit": 154, "desc": "Exit quoted plus", "category": "exit"},
        {"cmd": "exit '----666'", "exit": 2, "desc": "Exit quoted many dash", "category": "exit"},
        {"cmd": "exit '++++666'", "exit": 2, "desc": "Exit quoted many plus", "category": "exit"},
        {"cmd": "exit '6'66", "exit": 154, "desc": "Exit split quotes", "category": "exit"},
        {"cmd": "exit '2'66'32'", "exit": 8, "desc": "Exit multiple splits", "category": "exit"},
        {"cmd": "exit \"'666'\"", "exit": 2, "desc": "Exit nested quotes", "category": "exit"},
        {"cmd": "exit '\"666\"'", "exit": 2, "desc": "Exit nested double in single", "category": "exit"},
        {"cmd": "exit '666'\"666\"666", "exit": 170, "desc": "Exit complex quotes", "category": "exit"},
        {"cmd": "exit +'666'\"666\"666", "exit": 170, "desc": "Exit plus complex", "category": "exit"},
        {"cmd": "exit -'666'\"666\"666", "exit": 86, "desc": "Exit minus complex", "category": "exit"},
        {"cmd": "exit 9223372036854775807", "exit": 255, "desc": "Exit LONG_MAX", "category": "exit"},
        {"cmd": "exit 9223372036854775808", "exit": 2, "desc": "Exit overflow", "category": "exit"},
        {"cmd": "exit -9223372036854775808", "exit": 0, "desc": "Exit LONG_MIN", "category": "exit"},
        {"cmd": "exit -9223372036854775809", "exit": 2, "desc": "Exit underflow", "category": "exit"},
    ])

    # PIPES
    tests.extend([
        {"cmd": "cat | cat | cat | ls", "exit": 0, "desc": "Multiple cats and ls", "category": "pipes"},
        {"cmd": "ls | exit", "exit": 0, "desc": "Ls pipe exit", "category": "pipes"},
        {"cmd": "ls | exit 42", "exit": 42, "desc": "Ls pipe exit 42", "category": "pipes"},
        {"cmd": "exit | ls", "exit": 0, "desc": "Exit pipe ls", "category": "pipes"},
        {"cmd": "echo | echo", "exit": 0, "desc": "Echo pipe echo", "category": "pipes"},
        {"cmd": "echo hola | echo que tal", "exit": 0, "desc": "Echo pipe echo args", "category": "pipes"},
        {"cmd": "pwd | echo hola", "exit": 0, "desc": "Pwd pipe echo", "category": "pipes"},
        {"cmd": "env | echo hola", "exit": 0, "desc": "Env pipe echo", "category": "pipes"},
        {"cmd": "echo oui | cat -e", "exit": 0, "desc": "Echo pipe cat", "category": "pipes"},
        {"cmd": "echo oui | echo non | echo hola | grep oui", "exit": 0, "desc": "Multiple pipes grep oui", "category": "pipes"},
        {"cmd": "echo oui | echo non | echo hola | grep non", "exit": 0, "desc": "Multiple pipes grep non", "category": "pipes"},
        {"cmd": "echo oui | echo non | echo hola | grep hola", "exit": 0, "desc": "Multiple pipes grep hola", "category": "pipes"},
        {"cmd": "echo hola | cat -e | cat -e | cat -e", "exit": 0, "desc": "Echo multiple cat -e", "category": "pipes"},
        {"cmd": "cd .. | echo \"hola\"", "exit": 0, "desc": "Cd pipe echo", "category": "pipes"},
        {"cmd": "cd / | echo \"hola\"", "exit": 0, "desc": "Cd root pipe echo", "category": "pipes"},
        {"cmd": "cd .. | pwd", "exit": 0, "desc": "Cd pipe pwd", "category": "pipes"},
        {"cmd": "ifconfig | grep \":\"", "exit": 0, "desc": "Ifconfig pipe grep", "category": "pipes"},
        {"cmd": "ifconfig | grep hola", "exit": 1, "desc": "Ifconfig grep non-match", "category": "pipes"},
        {"cmd": "whoami | grep $USER", "exit": 0, "desc": "Whoami pipe grep USER", "category": "pipes"},
        {"cmd": "ls | hola", "exit": 127, "desc": "Ls pipe non-existent", "category": "pipes"},
        {"cmd": "ls | ls hola", "exit": 2, "desc": "Ls pipe ls error", "category": "pipes"},
        {"cmd": "ls | ls | hola", "exit": 127, "desc": "Double ls pipe error", "category": "pipes"},
        {"cmd": "ls | hola | ls", "exit": 0, "desc": "Ls error ls", "category": "pipes"},
        {"cmd": "ls | ls | hola | rev", "exit": 0, "desc": "Multiple pipe with error", "category": "pipes"},
        {"cmd": "ls | ls | echo hola | rev", "exit": 0, "desc": "Multiple pipe echo rev", "category": "pipes"},
        {"cmd": "ls -la | grep \".\"", "exit": 0, "desc": "Ls pipe grep dot", "category": "pipes"},
        {"cmd": "ls -la | grep \"'.'\"", "exit": 0, "desc": "Ls pipe grep quoted dot", "category": "pipes"},
        {"cmd": "echo test.c | cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e| cat -e|cat -e|cat -e|cat -e", "exit": 0, "desc": "Many cat -e pipes", "category": "pipes"},
        {"cmd": "echo hola | cat | cat | cat | cat | cat | grep hola", "exit": 0, "desc": "Many cats grep", "category": "pipes"},
        {"cmd": "echo hola | cat", "exit": 0, "desc": "Echo pipe cat simple", "category": "pipes"},
        {"cmd": "echo hola| cat", "exit": 0, "desc": "Echo pipe no space", "category": "pipes"},
        {"cmd": "echo hola |cat", "exit": 0, "desc": "Echo pipe space after", "category": "pipes"},
        {"cmd": "echo hola|cat", "exit": 0, "desc": "Echo pipe no spaces", "category": "pipes"},
        {"cmd": "echo hola ||| cat", "exit": 2, "desc": "Triple pipe", "category": "pipes"},
        {"cmd": "ech|o hola | cat", "exit": 127, "desc": "Pipe in command", "category": "pipes"},
        {"cmd": "cat Makefile | cat -e | cat -e", "exit": 0, "desc": "Cat file double cat -e", "category": "pipes"},
        {"cmd": "cat Makefile | grep srcs | cat -e", "exit": 0, "desc": "Cat grep cat", "category": "pipes"},
        {"cmd": "cat Makefile | grep srcs | grep srcs | cat -e", "exit": 0, "desc": "Cat double grep", "category": "pipes"},
        {"cmd": "cat Makefile | grep pr | head -n 5 | cd file_not_exist", "exit": 1, "desc": "Pipe to cd error", "category": "pipes"},
        {"cmd": "cat Makefile | grep pr | head -n 5 | hello", "exit": 127, "desc": "Pipe to non-existent", "category": "pipes"},
        {"cmd": "export HOLA=bonjour | cat -e | cat -e", "exit": 0, "desc": "Export pipe cats", "category": "pipes"},
        {"cmd": "unset HOLA | cat -e", "exit": 0, "desc": "Unset pipe cat", "category": "pipes"},
        {"cmd": "export | echo hola", "exit": 0, "desc": "Export pipe echo", "category": "pipes"},
        {"cmd": "sleep 3 | sleep 3", "exit": 0, "desc": "Sleep pipe sleep", "category": "pipes"},
        {"cmd": "sleep 3 | exit", "exit": 0, "desc": "Sleep pipe exit", "category": "pipes"},
        {"cmd": "exit | sleep 3", "exit": 0, "desc": "Exit pipe sleep", "category": "pipes"},
    ])

    # REDIRECTIONS
    tests.extend([
        {"cmd": "echo hola > bonjour", "exit": 0, "desc": "Redirect out simple", "category": "redirections"},
        {"cmd": "echo que tal >> bonjour", "exit": 0, "desc": "Redirect append", "category": "redirections"},
        {"cmd": "cat < bonjour", "exit": 0, "desc": "Redirect in simple", "category": "redirections"},
        {"cmd": "pwd>bonjour", "exit": 0, "desc": "Redirect no space", "category": "redirections"},
        {"cmd": "pwd >                     bonjour", "exit": 0, "desc": "Redirect many spaces", "category": "redirections"},
        {"cmd": "echo hola > > bonjour", "exit": 2, "desc": "Double redirect space", "category": "redirections"},
        {"cmd": "echo hola < < bonjour", "exit": 2, "desc": "Double redirect in space", "category": "redirections"},
        {"cmd": "echo hola >>> bonjour", "exit": 2, "desc": "Triple redirect", "category": "redirections"},
        {"cmd": "> bonjour echo hola", "exit": 0, "desc": "Redirect before command", "category": "redirections"},
        {"cmd": "> bonjour | echo hola", "exit": 0, "desc": "Redirect pipe echo", "category": "redirections"},
        {"cmd": "prout hola > bonjour", "exit": 127, "desc": "Non-existent redirect", "category": "redirections"},
        {"cmd": "echo hola > hello >> hello >> hello", "exit": 0, "desc": "Multiple redirects", "category": "redirections"},
        {"cmd": "echo hola >> hello >> hello > hello", "exit": 0, "desc": "Mixed redirect order", "category": "redirections"},
        {"cmd": "> pwd", "exit": 0, "desc": "Create file pwd", "category": "redirections"},
        {"cmd": "< pwd", "exit": 1, "desc": "Read non-existent pwd", "category": "redirections"},
        {"cmd": "cat <pwd", "exit": 1, "desc": "Cat non-existent", "category": "redirections"},
        {"cmd": "cat >>", "exit": 2, "desc": "Cat append no file", "category": "redirections"},
        {"cmd": "cat >>>", "exit": 2, "desc": "Cat triple redirect", "category": "redirections"},
        {"cmd": "cat >> <<", "exit": 2, "desc": "Cat append heredoc", "category": "redirections"},
        {"cmd": "cat >> > >> << >>", "exit": 2, "desc": "Cat mixed operators", "category": "redirections"},
        {"cmd": "cat < ls", "exit": 1, "desc": "Cat redirect non-existent", "category": "redirections"},
        {"cmd": "cat < ls > ls", "exit": 1, "desc": "Cat redirect both", "category": "redirections"},
        {"cmd": "cat > ls1 < ls2", "exit": 1, "desc": "Cat out then in error", "category": "redirections"},
        {"cmd": ">>hola", "exit": 0, "desc": "Append alone", "category": "redirections"},
        {"cmd": "echo hola >bonjour", "exit": 0, "desc": "Echo redirect no space", "category": "redirections"},
        {"cmd": "cat <bonjour", "exit": 0, "desc": "Cat in no space", "category": "redirections"},
        {"cmd": "echo hola>bonjour", "exit": 0, "desc": "Echo redirect joined", "category": "redirections"},
        {"cmd": "cat<bonjour", "exit": 0, "desc": "Cat in joined", "category": "redirections"},
        {"cmd": "echo hola> bonjour", "exit": 0, "desc": "Echo redirect space after", "category": "redirections"},
        {"cmd": "cat< bonjour", "exit": 0, "desc": "Cat in space after", "category": "redirections"},
        {"cmd": "echo hola               >bonjour", "exit": 0, "desc": "Echo many spaces redirect", "category": "redirections"},
        {"cmd": "cat<                     bonjour", "exit": 0, "desc": "Cat many spaces in", "category": "redirections"},
        {"cmd": "echo hola          >     bonjour", "exit": 0, "desc": "Echo spaces around redirect", "category": "redirections"},
        {"cmd": "cat            <         bonjour", "exit": 0, "desc": "Cat spaces around in", "category": "redirections"},
        {"cmd": "echo hola > srcs/bonjour", "exit": 0, "desc": "Echo redirect subdir", "category": "redirections"},
        {"cmd": "cat < srcs/bonjour", "exit": 0, "desc": "Cat in subdir", "category": "redirections"},
        {"cmd": "echo hola >srcs/bonjour", "exit": 0, "desc": "Echo redirect subdir no space", "category": "redirections"},
        {"cmd": "cat <srcs/bonjour", "exit": 0, "desc": "Cat in subdir no space", "category": "redirections"},
        {"cmd": "e'c'\"\"h\"\"o hola > bonjour", "exit": 0, "desc": "Quoted command redirect", "category": "redirections"},
        {"cmd": "cat 'bo'\"\"n\"\"jour", "exit": 0, "desc": "Cat quoted filename", "category": "redirections"},
        {"cmd": "echo hola > bonjour hey", "exit": 0, "desc": "Echo redirect with extra arg", "category": "redirections"},
        {"cmd": "<a cat <b <c", "exit": 0, "desc": "Multiple input redirects", "category": "redirections"},
        {"cmd": ">a ls >b >>c >d", "exit": 0, "desc": "Multiple output redirects", "category": "redirections"},
        {"cmd": "echo hola > a > b > c", "exit": 0, "desc": "Echo multiple outs", "category": "redirections"},
        {"cmd": "<a", "exit": 0, "desc": "Input redirect alone", "category": "redirections"},
        {"cmd": ">d cat <a >>e", "exit": 0, "desc": "Mixed redirects order", "category": "redirections"},
        {"cmd": "< a > b cat > hey >> d", "exit": 0, "desc": "Complex redirect order", "category": "redirections"},
        {"cmd": "echo hola <bonjour <hello", "exit": 0, "desc": "Echo double input", "category": "redirections"},
        {"cmd": "echo <bonjour <hello", "exit": 0, "desc": "Echo inputs only", "category": "redirections"},
        {"cmd": ">bonjour >hello <prout", "exit": 1, "desc": "Multiple out with error in", "category": "redirections"},
        {"cmd": ">bonjour <prout >hello", "exit": 1, "desc": "Out error out", "category": "redirections"},
        {"cmd": "<bonjour cat | wc > bonjour", "exit": 0, "desc": "In pipe out same file", "category": "redirections"},
    ])

    # HEREDOC
    tests.extend([
        {"cmd": "cat << hola\n$HOME\nhola\n", "exit": 0, "desc": "Heredoc with variable", "category": "heredoc", "interactive": True},
        {"cmd": "cat << 'hola'\n$HOME\nhola\n", "exit": 0, "desc": "Heredoc single quoted delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "cat << \"hola\"\n$HOME\nhola\n", "exit": 0, "desc": "Heredoc double quoted delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "cat << ho\"la\"\n$HOME\nhola\n", "exit": 0, "desc": "Heredoc partial quoted delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "cat << $HOME\nprout\n/home/vietdu91\n", "exit": 0, "desc": "Heredoc variable delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "cat << hola > bonjour\nprout\nhola\n", "exit": 0, "desc": "Heredoc with redirect", "category": "heredoc", "interactive": True},
        {"cmd": "cat << hola | rev\nprout\nhola\n", "exit": 0, "desc": "Heredoc piped", "category": "heredoc", "interactive": True},
        {"cmd": "<< hola\nchola\nhola\n", "exit": 0, "desc": "Heredoc alone", "category": "heredoc", "interactive": True},
        {"cmd": "<<hola\n\nhola\n", "exit": 0, "desc": "Heredoc no space", "category": "heredoc", "interactive": True},
        {"cmd": "cat <<", "exit": 2, "desc": "Heredoc no delimiter", "category": "heredoc"},
        {"cmd": "cat << prout << lol << koala\nprout\nlol\nkoala\n", "exit": 0, "desc": "Multiple heredocs", "category": "heredoc", "interactive": True},
        {"cmd": "prout << lol << cat << koala\nprout\nlol\ncat\nkoala\n", "exit": 127, "desc": "Non-existent with heredocs", "category": "heredoc", "interactive": True},
        {"cmd": "<< $hola\n$hola\n", "exit": 0, "desc": "Heredoc empty var delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "<< $\"hola\"$\"b\"\nholab\n", "exit": 0, "desc": "Heredoc complex delimiter", "category": "heredoc", "interactive": True},
        {"cmd": "echo hola <<< bonjour", "exit": 0, "desc": "Here-string", "category": "heredoc", "bonus": True},
        {"cmd": "echo hola <<<< bonjour", "exit": 2, "desc": "Four less-than", "category": "heredoc"},
        {"cmd": "echo hola <<<<< bonjour", "exit": 2, "desc": "Five less-than", "category": "heredoc"},
    ])

    # LOGICAL OPERATORS (BONUS)
    tests.extend([
        {"cmd": "pwd && ls", "exit": 0, "desc": "And operator success", "category": "logical", "bonus": True},
        {"cmd": "pwd || ls", "exit": 0, "desc": "Or operator first success", "category": "logical", "bonus": True},
        {"cmd": "echo hola || echo bonjour", "exit": 0, "desc": "Or with echo", "category": "logical", "bonus": True},
        {"cmd": "echo hola && echo bonjour", "exit": 0, "desc": "And with echo", "category": "logical", "bonus": True},
        {"cmd": "echo bonjour || echo hola", "exit": 0, "desc": "Or first succeeds", "category": "logical", "bonus": True},
        {"cmd": "echo bonjour && echo hola", "exit": 0, "desc": "And both succeed", "category": "logical", "bonus": True},
        {"cmd": "echo -n bonjour && echo -n hola", "exit": 0, "desc": "And with -n flags", "category": "logical", "bonus": True},
        {"cmd": "pwd && ls && echo hola", "exit": 0, "desc": "Multiple and operators", "category": "logical", "bonus": True},
        {"cmd": "pwd || ls && echo hola", "exit": 0, "desc": "Or and mix", "category": "logical", "bonus": True},
        {"cmd": "pwd && ls || echo hola", "exit": 0, "desc": "And or mix", "category": "logical", "bonus": True},
        {"cmd": "pwd || ls || echo hola", "exit": 0, "desc": "Multiple or operators", "category": "logical", "bonus": True},
        {"cmd": "ls || export \"\"", "exit": 0, "desc": "Or with export error", "category": "logical", "bonus": True},
        {"cmd": "export \"\" || ls", "exit": 0, "desc": "Export error or ls", "category": "logical", "bonus": True},
        {"cmd": "ls && export \"\"", "exit": 1, "desc": "And with export error", "category": "logical", "bonus": True},
        {"cmd": "export \"\" && ls", "exit": 1, "desc": "Export error and ls", "category": "logical", "bonus": True},
        {"cmd": "cat | echo || ls", "exit": 0, "desc": "Pipe with or", "category": "logical", "bonus": True},
        {"cmd": "cat | echo && ls", "exit": 0, "desc": "Pipe with and", "category": "logical", "bonus": True},
        {"cmd": "ls || cat | echo", "exit": 0, "desc": "Or then pipe", "category": "logical", "bonus": True},
        {"cmd": "ls && cat | echo", "exit": 0, "desc": "And then pipe", "category": "logical", "bonus": True},
        {"cmd": "export \"\" && unset \"\"", "exit": 1, "desc": "Export error and unset", "category": "logical", "bonus": True},
    ])

    # PARENTHESES (BONUS)
    tests.extend([
        {"cmd": "(ls)", "exit": 0, "desc": "Simple subshell", "category": "parentheses", "bonus": True},
        {"cmd": "( ( ls ) )", "exit": 0, "desc": "Nested subshells", "category": "parentheses", "bonus": True},
        {"cmd": "( ( ) ls )", "exit": 2, "desc": "Empty subshell with command", "category": "parentheses", "bonus": True},
        {"cmd": "ls && (ls)", "exit": 0, "desc": "And with subshell", "category": "parentheses", "bonus": True},
        {"cmd": "(ls && pwd)", "exit": 0, "desc": "Subshell with and", "category": "parentheses", "bonus": True},
        {"cmd": "( ( ls&&pwd ) )", "exit": 0, "desc": "Nested subshell and", "category": "parentheses", "bonus": True},
        {"cmd": "( ( ls ) &&pwd )", "exit": 0, "desc": "Nested mixed spacing", "category": "parentheses", "bonus": True},
        {"cmd": "(ls && ( ( pwd ) ) )", "exit": 0, "desc": "Complex nesting", "category": "parentheses", "bonus": True},
        {"cmd": "(ls && pwd) > hola", "exit": 0, "desc": "Subshell redirect", "category": "parentheses", "bonus": True},
        {"cmd": "> hola ls && pwd", "exit": 0, "desc": "Redirect and without parens", "category": "parentheses", "bonus": True},
        {"cmd": "> hola (ls && pwd)", "exit": 2, "desc": "Redirect before subshell", "category": "parentheses", "bonus": True},
        {"cmd": "(> pwd)", "exit": 0, "desc": "Redirect in subshell", "category": "parentheses", "bonus": True},
        {"cmd": "(< pwd)", "exit": 0, "desc": "Input redirect in subshell", "category": "parentheses", "bonus": True},
        {"cmd": "( ( ( ( ( pwd) ) ) ) )", "exit": 0, "desc": "Many nested subshells", "category": "parentheses", "bonus": True},
        {"cmd": "() pwd", "exit": 2, "desc": "Empty parens before command", "category": "parentheses", "bonus": True},
        {"cmd": "> pwd (ls)", "exit": 2, "desc": "Redirect then subshell", "category": "parentheses", "bonus": True},
        {"cmd": "(ls||pwd)&&(ls||pwd)", "exit": 0, "desc": "Complex logical subshells", "category": "parentheses", "bonus": True},
        {"cmd": "(lss||pwd)&&(lss||pwd)", "exit": 0, "desc": "Subshells with errors", "category": "parentheses", "bonus": True},
        {"cmd": "(lss&&pwd)&&(lss&&pwd)", "exit": 127, "desc": "And subshells with errors", "category": "parentheses", "bonus": True},
        {"cmd": "(ls && pwd | wc) > hola", "exit": 0, "desc": "Subshell pipe redirect", "category": "parentheses", "bonus": True},
        {"cmd": "(pwd | wc) < hola", "exit": 0, "desc": "Subshell pipe input", "category": "parentheses", "bonus": True},
        {"cmd": "(ls && pwd | wc) < hola", "exit": 0, "desc": "Subshell and pipe input", "category": "parentheses", "bonus": True},
        {"cmd": "(ls -z || pwd | wc) < hola", "exit": 0, "desc": "Subshell error or pipe", "category": "parentheses", "bonus": True},
        {"cmd": "(ls -z || pwd && ls)", "exit": 0, "desc": "Subshell complex logic", "category": "parentheses", "bonus": True},
        {"cmd": "ls || (cat Makefile|grep srcs) && (pwd|wc)", "exit": 0, "desc": "Mixed subshells pipes", "category": "parentheses", "bonus": True},
        {"cmd": "ls -z && (ls) && (pwd)", "exit": 130, "desc": "Error and subshells", "category": "parentheses", "bonus": True},
        {"cmd": "(ls > Docs/hey && pwd) > hola", "exit": 0, "desc": "Nested redirects subshell", "category": "parentheses", "bonus": True},
        {"cmd": "ls > Docs/hey && pwd > hola", "exit": 0, "desc": "And with redirects no parens", "category": "parentheses", "bonus": True},
        {"cmd": "cd ../.. && pwd && pwd", "exit": 0, "desc": "Cd and pwd twice", "category": "parentheses", "bonus": True},
        {"cmd": "(cd ../.. && pwd) && pwd", "exit": 0, "desc": "Cd subshell and pwd", "category": "parentheses", "bonus": True},
        {"cmd": "ls -z || cd ../../..&&pwd", "exit": 0, "desc": "Error or cd and pwd", "category": "parentheses", "bonus": True},
        {"cmd": "ls -z || (cd ../../..&&pwd)", "exit": 0, "desc": "Error or subshell cd pwd", "category": "parentheses", "bonus": True},
    ])

    # WILDCARDS (BONUS)
    tests.extend([
        {"cmd": "ls *", "exit": 0, "desc": "Wildcard all", "category": "wildcards", "bonus": True},
        {"cmd": "ls *.*", "exit": 0, "desc": "Wildcard with dot", "category": "wildcards", "bonus": True},
        {"cmd": "ls *.hola", "exit": 2, "desc": "Wildcard no match", "category": "wildcards", "bonus": True},
        {"cmd": "cat M*le", "exit": 0, "desc": "Wildcard in filename", "category": "wildcards", "bonus": True},
        {"cmd": "cat M*ee", "exit": 1, "desc": "Wildcard no match cat", "category": "wildcards", "bonus": True},
        {"cmd": "cat Make*file", "exit": 0, "desc": "Wildcard middle", "category": "wildcards", "bonus": True},
        {"cmd": "echo *", "exit": 0, "desc": "Echo wildcard", "category": "wildcards", "bonus": True},
        {"cmd": "echo '*'", "exit": 0, "desc": "Echo quoted wildcard", "category": "wildcards", "bonus": True},
        {"cmd": "echo D*", "exit": 0, "desc": "Echo wildcard prefix", "category": "wildcards", "bonus": True},
        {"cmd": "echo *Z", "exit": 0, "desc": "Echo wildcard suffix no match", "category": "wildcards", "bonus": True},
        {"cmd": "echo *t hola", "exit": 0, "desc": "Echo wildcard with arg", "category": "wildcards", "bonus": True},
        {"cmd": "echo *t", "exit": 0, "desc": "Echo wildcard suffix", "category": "wildcards", "bonus": True},
        {"cmd": "echo $*", "exit": 0, "desc": "Echo dollar wildcard", "category": "wildcards", "bonus": True},
        {"cmd": "echo hola*hola *", "exit": 0, "desc": "Echo mixed wildcards", "category": "wildcards", "bonus": True},
        {"cmd": "echo $hola*", "exit": 0, "desc": "Echo var wildcard", "category": "wildcards", "bonus": True},
        {"cmd": "echo $HOME*", "exit": 0, "desc": "Echo HOME wildcard", "category": "wildcards", "bonus": True},
        {"cmd": "cd *", "exit": 0, "desc": "Cd wildcard single dir", "category": "wildcards", "bonus": True},
        {"cmd": "cd *", "exit": 1, "desc": "Cd wildcard multiple", "category": "wildcards", "bonus": True},
    ])

    return tests


def _bash_single_quote_escape(s: str) -> str:
    """Escape a string to be safely embedded in single-quoted bash string.
    Allows newlines; handles single-quotes via closing, escaping, reopening.
    """
    return s.replace("'", "'\\''")


def generate_test_data_bash(tests):
    """Generate bash arrays with test data (no fragile field splitting)."""
    lines = []
    lines.append(f"NUM_TESTS={len(tests)}")
    lines.append("declare -a CMD EXIT DESC CAT BONUS INTER")
    for i, t in enumerate(tests):
        cmd = _bash_single_quote_escape(t['cmd'])
        desc = _bash_single_quote_escape(t['desc'])
        cat = _bash_single_quote_escape(t['category'])
        bonus = 1 if t.get('bonus', False) else 0
        inter = 1 if t.get('interactive', False) else 0
        lines.append(f"CMD[{i}]='{cmd}'")
        lines.append(f"EXIT[{i}]={t['exit']}")
        lines.append(f"DESC[{i}]='{desc}'")
        lines.append(f"CAT[{i}]='{cat}'")
        lines.append(f"BONUS[{i}]={bonus}")
        lines.append(f"INTER[{i}]={inter}")
    return "\n".join(lines)


def generate_test_runner():
    tests = parse_tests_from_document()

    # Build the test runner bash script using token replacement to avoid f-string brace issues
    template = """#!/bin/bash

# Minishell Complete Test Suite (auto-generated)
# Total tests: __TEST_COUNT__

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Valgrind base command (suppression added if file exists)
VALGRIND_BASE="valgrind --trace-children=yes -s \\
    --leak-check=full \\
    --show-leak-kinds=all \\
    --track-origins=yes \\
    --track-fds=yes \\
    --log-file=valgrind.log \\
    --error-exitcode=42"

TEST_DIR="/tmp/minishell_test_$$"
RESULTS_FILE="test_results.json"
SUMMARY_FILE=""
ARTIFACT_DIR=""
TIMEOUT_SEC="${TIMEOUT_SEC:-8}"

setup_test_env() {
    mkdir -p "$TEST_DIR"
    ORIG="$PWD"
    cd "$TEST_DIR" || exit 1

    # Create test files
    echo "Amour Tu es Horrible" > a
    echo "0123456789" > b
    echo "Prout" > c
    mkdir -p srcs Docs
    : > Makefile
    # Ensure tests that expect missing infile behave as intended
    [ -f pwd ] && rm -f pwd || true

    # Copy minishell binary from original dir
    if [ -f "$ORIG/minishell" ]; then
        cp "$ORIG/minishell" .
        chmod +x ./minishell || true
    else
        echo "${RED}minishell binary not found in $ORIG${NC}" >&2
        exit 1
    fi

    # Build valgrind command (optionally with suppression file)
    VAL_SUPP=""
    if [ -f "$ORIG/valgrind_script/ignore_readline_leaks.txt" ]; then
        mkdir -p valgrind_script
        cp "$ORIG/valgrind_script/ignore_readline_leaks.txt" valgrind_script/
        VAL_SUPP=" --suppressions=valgrind_script/ignore_readline_leaks.txt"
    fi
    VALGRIND_CMD="$VALGRIND_BASE$VAL_SUPP"

    # Artifacts in project folder
    ARTIFACT_DIR="$ORIG/tests/out"
    mkdir -p "$ARTIFACT_DIR/logs"
    RESULTS_FILE="$ARTIFACT_DIR/report.json"
    SUMMARY_FILE="$ARTIFACT_DIR/summary.txt"
    rm -f "$RESULTS_FILE" "$SUMMARY_FILE"
}

cleanup_test_env() {
    cd "${OLDPWD:-/}" 2>/dev/null || true
    rm -rf "$TEST_DIR"
}

check_memory_leaks() {
    if [ ! -f valgrind.log ]; then
        echo "no_log"
        return 1
    fi

    if grep -q "definitely lost: [1-9]" valgrind.log || \
       grep -q "indirectly lost: [1-9]" valgrind.log || \
        grep -q "Invalid read" valgrind.log || \
        grep -q "Invalid write" valgrind.log || \
        grep -q "FILE DESCRIPTORS" valgrind.log; then
        echo "leak_detected"
        return 1
    fi

    echo "clean"
    return 0
}

slugify() {
    local s="$1"; s=$(printf "%s" "$s" | tr ' /|:' '____'); s=$(printf "%s" "$s" | tr -cd '[:alnum:]_.-'); printf "%s" "${s:0:60}";
}

run_test() {
    local cmd="$1"
    local expected_exit="$2"
    local description="$3"
    local category="$4"
    local is_bonus="$5"
    local is_interactive="$6"

    # Skip bonus if requested
    if [ "$is_bonus" = "1" ] && [ "$SKIP_BONUS" = "1" ]; then
        return 2
    fi

    # Skip interactive tests unless explicitly requested
    if [ "$is_interactive" = "1" ] && [ "$AUTOMATED" = "1" ]; then
        return 2
    fi

    rm -f valgrind.log output.txt error.txt

    # 1) Run WITHOUT valgrind to capture minishell exit code
    timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | ./minishell > output.txt 2> error.txt' "$cmd"
    nv_status=$?
    actual_exit=$nv_status
    local timeout_nonvg=0
    if [ $nv_status -eq 124 ]; then timeout_nonvg=1; fi

    # 2) Run WITH valgrind for memory checks (ignore its exit for result)
    timeout "$TIMEOUT_SEC"s bash -lc 'printf "%s" "$0" | $VALGRIND_CMD ./minishell > /dev/null 2> /dev/null' "$cmd"
    vg_status=$?
    local timeout_vg=0
    if [ $vg_status -eq 124 ]; then timeout_vg=1; fi

    memory_status=$(check_memory_leaks)

    local result="PASS"
    if [ $timeout_nonvg -eq 1 ]; then
        result="TIMEOUT"
    elif [ "$actual_exit" -ne "$expected_exit" ]; then
        result="FAIL_EXIT"
    elif [ "$memory_status" = "leak_detected" ]; then
        result="FAIL_LEAK"
    fi

    # Save valgrind log per test
    local slug; slug=$(slugify "$description")
    mkdir -p "$ARTIFACT_DIR/logs/$category"
    local val_path="$ARTIFACT_DIR/logs/$category/${slug}.valgrind.txt"
    if [ -f valgrind.log ]; then cp valgrind.log "$val_path"; else echo "No valgrind log (status=$vg_status, timeout=$timeout_vg)" > "$val_path"; fi

    # Append JSON result
    echo "{\"cmd\":\"$(printf '%s' "$cmd" | sed -e 's/\\\\/\\\\\\\\/g' -e 's/\"/\\\"/g')\",\"expected\":$expected_exit,\"actual\":$actual_exit,\"result\":\"$result\",\"category\":\"$category\",\"description\":\"$(printf '%s' "$description" | sed 's/\"/\\\"/g')\",\"memory\":\"$memory_status\",\"timeouts\":{\"nonvg\":$timeout_nonvg,\"vg\":$timeout_vg}}" >> "$RESULTS_FILE"

    if [ "$result" = "PASS" ]; then
        echo -e "${GREEN}✓${NC} $description"
        return 0
    else
        echo -e "${RED}✗${NC} $description"
        echo -e "  Command: ${CYAN}$(printf '%s' "$cmd")${NC}"
        echo -e "  Expected exit: $expected_exit, Got: $actual_exit"
        if [ "$memory_status" = "leak_detected" ]; then
            echo -e "  ${RED}Memory leak detected!${NC}"
            grep -A 3 "definitely lost\|indirectly lost\|Invalid" valgrind.log | head -10 || true
        fi
        return 1
    fi
}

# Test data
__TEST_DATA__

# Category counters
declare -A CATEGORY_TOTAL CATEGORY_PASSED CATEGORY_FAILED CATEGORY_SKIPPED

main() {
    local filter_category="$1"

    echo "========================================"
    echo "Minishell Complete Test Suite"
    echo "Total tests: __TEST_COUNT__"
    echo "========================================"
    echo ""

    setup_test_env
    trap cleanup_test_env EXIT

    echo "[" > "$RESULTS_FILE"

    local total=0 passed=0 failed=0 skipped=0 first=1

    for (( i=0; i<NUM_TESTS; i++ )); do
        local cmd="${CMD[$i]}"
        local expected_exit="${EXIT[$i]}"
        local desc="${DESC[$i]}"
        local category="${CAT[$i]}"
        local is_bonus="${BONUS[$i]}"
        local is_interactive="${INTER[$i]}"

        # Filter by category
        if [ -n "$filter_category" ] && [ "$category" != "$filter_category" ]; then
            continue
        fi

        total=$((total + 1))
        CATEGORY_TOTAL[$category]=$(( ${CATEGORY_TOTAL[$category]:-0} + 1 ))

        [ $first -eq 0 ] && echo "," >> "$RESULTS_FILE"
        first=0

        run_test "$cmd" "$expected_exit" "$desc" "$category" "$is_bonus" "$is_interactive"
        result=$?

        if [ $result -eq 0 ]; then
            passed=$((passed + 1))
            CATEGORY_PASSED[$category]=$(( ${CATEGORY_PASSED[$category]:-0} + 1 ))
        elif [ $result -eq 2 ]; then
            skipped=$((skipped + 1))
            CATEGORY_SKIPPED[$category]=$(( ${CATEGORY_SKIPPED[$category]:-0} + 1 ))
        else
            failed=$((failed + 1))
            CATEGORY_FAILED[$category]=$(( ${CATEGORY_FAILED[$category]:-0} + 1 ))
        fi
    done

    echo "]" >> "$RESULTS_FILE"

    echo "" | tee -a "$SUMMARY_FILE"
    echo "========================================" | tee -a "$SUMMARY_FILE"
    echo "Summary by Category" | tee -a "$SUMMARY_FILE"
    echo "========================================" | tee -a "$SUMMARY_FILE"
    for category in "${!CATEGORY_TOTAL[@]}"; do
        local cat_total=${CATEGORY_TOTAL[$category]}
        local cat_passed=${CATEGORY_PASSED[$category]:-0}
        local cat_failed=${CATEGORY_FAILED[$category]:-0}
        local cat_skipped=${CATEGORY_SKIPPED[$category]:-0}
        local line="$category: $cat_passed passed, $cat_failed failed, $cat_skipped skipped / $cat_total total"
        echo -e "${BLUE}$line${NC}"; echo "$line" >> "$SUMMARY_FILE"
    done

    echo "" | tee -a "$SUMMARY_FILE"
    echo "========================================" | tee -a "$SUMMARY_FILE"
    echo "Overall Results" | tee -a "$SUMMARY_FILE"
    echo "========================================" | tee -a "$SUMMARY_FILE"
    echo -e "Total: $total | ${GREEN}Passed: $passed${NC} | ${RED}Failed: $failed${NC} | ${YELLOW}Skipped: $skipped${NC}" | tee -a "$SUMMARY_FILE"
    local pass_rate=0
    if [ $total -gt 0 ]; then
        pass_rate=$((passed * 100 / total))
    fi
    echo -e "Pass rate: $pass_rate%" | tee -a "$SUMMARY_FILE"
    echo "========================================" | tee -a "$SUMMARY_FILE"

    echo "RESULT: $passed/$total $RESULTS_FILE"

    exit $failed
}

# CLI
SKIP_BONUS=0
AUTOMATED=1
CATEGORY=""

while [[ $# -gt 0 ]]; do
    case $1 in
        --category)
            CATEGORY="$2"; shift 2;;
        --with-bonus)
            SKIP_BONUS=0; shift;;
        --no-bonus)
            SKIP_BONUS=1; shift;;
        --interactive)
            AUTOMATED=0; shift;;
        *)
            echo "Unknown option: $1" >&2
            echo "Usage: $0 [--category CATEGORY] [--with-bonus|--no-bonus] [--interactive]" >&2
            exit 1;;
    esac
done

    main "$CATEGORY"
"""

    script = template.replace("__TEST_COUNT__", str(len(tests)))
    script = script.replace("__TEST_DATA__", generate_test_data_bash(tests))
    return script


if __name__ == "__main__":
    script = generate_test_runner()
    with open('run_all_tests.sh', 'w') as f:
        f.write(script)

    tests = parse_tests_from_document()
    print("\u2713 Generated run_all_tests.sh")
    print(f"\u2713 Total test cases: {len(tests)}")
    print("\nTest categories:")
    categories = {}
    bonus_counts = {}
    for t in tests:
        cat = t['category']
        categories[cat] = categories.get(cat, 0) + 1
        if t.get('bonus', False):
            bonus_counts[cat] = bonus_counts.get(cat, 0) + 1
    for cat in sorted(categories):
        b = bonus_counts.get(cat, 0)
        print(f"  - {cat}: {categories[cat]} tests ({b} bonus)")

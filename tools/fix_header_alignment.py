#!/usr/bin/env python3
import re
from pathlib import Path

HEADER = Path('include/mandatory.h')

def transform_line(line: str, in_struct: bool) -> str:
    # Preserve newline
    nl = ''
    if line.endswith('\n'):
        nl = '\n'
        line = line[:-1]

    # Leave preprocessor and header comment lines untouched
    if line.lstrip().startswith('#') or line.startswith('/*') or line.startswith('*/') or line.startswith('*'):
        return line + nl

    # For continuation prototype lines starting with a tab, keep exactly one leading tab
    if line.startswith('\t'):
        body = line.lstrip('\t')
        # Replace any remaining tabs in the body by spaces
        body = body.replace('\t', ' ')
        return '\t' + body + nl

    # For other lines (prototypes or struct content), replace tabs by single spaces in the body
    # but keep inside braces indentation tabs as-is (there are none at col 0 here)
    body = line
    # Normalize multiple spaces/tabs between tokens to single spaces (outside strings)
    body = body.replace('\t', ' ')
    # Collapse multiple spaces to single where appropriate
    body = re.sub(r' {2,}', ' ', body)
    return body + nl

def main():
    text = HEADER.read_text()
    out = []
    in_struct = False
    for raw in text.splitlines(True):
        s = raw.strip()
        if s.startswith('typedef struct'):
            in_struct = True
        if in_struct and '};' in s:
            # close struct happens later after typedef name; still transform lines uniformly
            pass
        out.append(transform_line(raw, in_struct))
        if in_struct and raw.strip().endswith('};'):
            in_struct = False
    HEADER.write_text(''.join(out))

if __name__ == '__main__':
    main()


#!/usr/bin/env python3
from pathlib import Path
import re

HDR = Path('include/mandatory.h')

TWO_WORD_TYPES = {('long','long'), ('unsigned','int'), ('unsigned','long'), ('unsigned','char'), ('unsigned','short')}

def split_decl(content: str):
    # split before trailing ';'
    if ';' not in content:
        return None, None
    body, tail = content.split(';', 1)
    body = body.strip()
    if not body:
        return None, None
    tokens = re.split(r"\s+", body)
    if not tokens:
        return None, None
    # Determine type tokens (1 or 2 words)
    if len(tokens) >= 2 and (tokens[0], tokens[1]) in TWO_WORD_TYPES:
        type_part = ' '.join(tokens[:2])
        rest = ' '.join(tokens[2:])
    else:
        type_part = tokens[0]
        rest = ' '.join(tokens[1:])
    return type_part, rest

def format_struct_field(line: str) -> str:
    # Expect leading tabs then declaration
    # Force a single leading tab for struct fields
    indent = '\t'
    i = 0
    while i < len(line) and line[i] == '\t':
        i += 1
    content = line[i:].rstrip('\n')
    if not content or content.startswith('}'):
        return line
    t, rest = split_decl(content)
    if not t:
        return line
    # glue pointer stars with identifier (rest)
    rest = re.sub(r"\s*\*+\s*", lambda m: m.group(0).replace(' ', ''), rest)
    return f"{indent}{t}\t{rest};\n"

def format_prototype(line: str) -> str:
    # skip preprocessor/comments/blank
    s = line.strip()
    if not s or s.startswith('#') or s.startswith('/*'):
        return line
    # continuation prototype lines should start with one tab then no tabs inside
    if line.startswith('\t'):
        return '\t' + line.lstrip('\t').replace('\t', ' ')
    # one-line prototype
    if not s.endswith(';'):
        return line
    t, rest = split_decl(s)
    if not t:
        return line
    # glue pointer to identifier for return pointer types
    rest = re.sub(r"^\*+\s*", lambda m: m.group(0).replace(' ', ''), rest)
    return f"{t}\t{rest};\n"

def main():
    text = HDR.read_text()
    out = []
    in_struct = False
    for raw in text.splitlines(True):
        sr = raw.strip()
        if sr.startswith('typedef struct') or (sr.startswith('struct') and sr.endswith('{')):
            in_struct = True
            out.append(raw)
            continue
        if in_struct:
        # typedef close line: ensure tab before alias
            if sr.startswith('} '):
                alias = sr[2:]
                out.append('}\t' + alias + '\n')
            else:
                out.append(format_struct_field(raw))
        if sr.endswith('};') or sr == '};' or sr.startswith('} '):
            in_struct = False
            continue
        out.append(format_prototype(raw))
    HDR.write_text(''.join(out))

if __name__ == '__main__':
    main()

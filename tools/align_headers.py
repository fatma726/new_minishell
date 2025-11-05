#!/usr/bin/env python3
import sys
from pathlib import Path
import re

AUTHOR_LOGIN = "fatmtahmdabrahym"
AUTHOR_EMAIL = f"{AUTHOR_LOGIN}@student.42.fr"

def load_template_constants():
    # Canonical 42 header lines (80 chars each)
    const = {
        "L1": "/* ************************************************************************** */",
        "L2": "/*                                                                            */",
        "L3": "/*                                                        :::      ::::::::   */",
        "L5": "/*                                                    +:+ +:+         +:+     */",
        "L7": "/*                                                +#+#+#+#+#+   +#+           */",
        "L10": "/*                                                                            */",
        "L11": "/* ************************************************************************** */",
        # Canonical minimal tokens (no leading spaces); left padding is computed
        "S4": ":+:      :+:    :+:  ",
        "S6": "+#+  +:+       +#+       ",
        "S8": "#+#    #+#            ",
        "S9": "###   ########.fr      ",
    }
    # Sanity check lengths
    for k in ("L1","L2","L3","L5","L7","L10","L11"):
        if len(const[k]) != 80:
            raise SystemExit(f"Bad constant {k} length {len(const[k])}")
    return const

def make_line(content: str) -> str:
    # Build a header line of exactly 80 chars with delimiters
    return "/* " + content.ljust(74) + " */"

def build_header(fname: str, created_line: str, updated_line: str, consts: dict) -> list[str]:
    # Extract dates from existing created/updated lines; fallback if not found
    created_date = "1970/01/01 00:00:00"
    m = re.search(r"Created:\s+([^b]+)by\s+", created_line)
    if m:
        created_date = m.group(1).strip()
    updated_date = created_date
    m = re.search(r"Updated:\s+([^b]+)by\s+", updated_line)
    if m:
        updated_date = m.group(1).strip()

    def var_line(left: str, suffix: str) -> str:
        # Ensure at least one space before suffix, truncating left if needed
        max_left = 74 - len(suffix) - 1
        if len(left) > max_left:
            left = left[:max_left]
        pad = 74 - len(left) - len(suffix)
        return make_line(left + (" " * pad) + suffix)

    # Line 4: file name + suffix
    left4 = f"  {fname}"
    l4 = var_line(left4, consts["S4"])

    # Line 6: By: author <email> + suffix
    left6 = f"  By: {AUTHOR_LOGIN} <{AUTHOR_EMAIL}>"
    l6 = var_line(left6, consts["S6"]) 

    # Line 8: Created
    left8 = f"  Created: {created_date} by {AUTHOR_LOGIN}"
    l8 = var_line(left8, consts["S8"]) 

    # Line 9: Updated
    left9 = f"  Updated: {updated_date} by {AUTHOR_LOGIN}"
    l9 = var_line(left9, consts["S9"]) 

    return [
        consts["L1"],
        consts["L2"],
        consts["L3"],
        l4,
        consts["L5"],
        l6,
        consts["L7"],
        l8,
        l9,
        consts["L10"],
        consts["L11"],
    ]

def fix_file(path: Path, consts: dict) -> bool:
    try:
        text = path.read_text()
    except Exception:
        return False
    lines = text.splitlines()
    if len(lines) < 11:
        return False
    # Check header markers
    if not (lines[0].startswith("/* ") and lines[0].endswith(" */") and lines[10].startswith("/* ")):
        return False
    # Keep original created/updated lines to preserve timestamps when possible
    created_line = lines[7] if len(lines) >= 9 else ""
    updated_line = lines[8] if len(lines) >= 10 else ""
    new_header = build_header(path.name, created_line, updated_line, consts)
    # Replace the first 11 lines
    new_lines = new_header + lines[11:]
    if new_lines != lines:
        path.write_text("\n".join(new_lines) + ("\n" if text.endswith("\n") else ""))
        return True
    return False

def iter_targets():
    roots = [Path("src"), Path("include"), Path("libs/Libft")]
    for root in roots:
        if not root.exists():
            continue
        for p in root.rglob("*"):
            if p.suffix in (".c", ".h") and p.is_file():
                yield p

def main():
    consts = load_template_constants()
    changed = 0
    for p in iter_targets():
        if fix_file(p, consts):
            changed += 1
            print(f"fixed: {p}")
    print(f"Done. Files updated: {changed}")

if __name__ == "__main__":
    main()

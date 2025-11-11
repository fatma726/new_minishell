#!/usr/bin/env python3
"""
Strict valgrind log checker for minishell.

- main process:
    - FILE DESCRIPTORS: exactly 3
    - definitely lost: 0
    - indirectly lost: 0
    - still reachable: 0
- children (--trace-children=yes):
    - FILE DESCRIPTORS: 1..3
    - definitely lost: 0
    - indirectly lost: 0
    - still reachable: 0

Fails if any summary is missing.
"""
import re
import sys
from pathlib import Path

LOG_DIR = Path("valgrind_logs")

# strict mode
MAIN_FD_MIN = 3
MAIN_FD_MAX = 3
MAIN_STILL_REACHABLE_WHITELIST = set()  # empty -> must be 0
IGNORE_CHILD_STILL_REACHABLE = False     # children must be 0 too

RE_PID  = re.compile(r"^==(?P<pid>\d+)==")
RE_CASE = re.compile(r"==== CASE: (?P<label>.*) ====")
RE_FD   = re.compile(r"FILE DESCRIPTORS:\s+(?P<count>\d+)")
RE_DEF  = re.compile(r"definitely lost:\s*(?P<bytes>\d+)\s+bytes in\s+(?P<blocks>\d+)\s+blocks")
RE_IND  = re.compile(r"indirectly lost:\s*(?P<bytes>\d+)\s+bytes in\s+(?P<blocks>\d+)\s+blocks")
RE_SR   = re.compile(r"still reachable:\s*(?P<bytes>\d+)\s+bytes in\s+(?P<blocks>\d+)\s+blocks")
RE_CMD  = re.compile(r"Command:\s*(?P<cmd>.*)")
RE_EXE  = re.compile(r"Executable:\s*(?P<exe>.*)")


class ProcSummary:
    def __init__(self):
        self.fd_count = None
        self.def_lost = None
        self.ind_lost = None
        self.still_reach = None
        self.cmd = None
        self.exe = None
        self.lines = 0

    def merge_line(self, line: str):
        self.lines += 1

        m = RE_FD.search(line)
        if m:
            self.fd_count = int(m.group("count"))

        m = RE_DEF.search(line)
        if m:
            self.def_lost = int(m.group("bytes"))

        m = RE_IND.search(line)
        if m:
            self.ind_lost = int(m.group("bytes"))

        m = RE_SR.search(line)
        if m:
            self.still_reach = int(m.group("bytes"))

        m = RE_CMD.search(line)
        if m:
            self.cmd = m.group("cmd").strip()

        m = RE_EXE.search(line)
        if m:
            self.exe = m.group("exe").strip()

    def check_main(self):
        reasons = []

        # FD: must exist and be exactly 3
        if self.fd_count is None:
            reasons.append("main: no FD summary found")
        elif not (MAIN_FD_MIN <= self.fd_count <= MAIN_FD_MAX):
            reasons.append(f"main fd_count={self.fd_count} (expected {MAIN_FD_MIN}..{MAIN_FD_MAX})")

        # definitely lost: must exist and be 0
        if self.def_lost is None:
            reasons.append("main: no 'definitely lost' summary found")
        elif self.def_lost != 0:
            reasons.append(f"main definitely_lost={self.def_lost}")

        # indirectly lost: must exist and be 0
        if self.ind_lost is None:
            reasons.append("main: no 'indirectly lost' summary found")
        elif self.ind_lost != 0:
            reasons.append(f"main indirectly_lost={self.ind_lost}")

        # still reachable: must exist and be 0 (or whitelisted, but we left it empty)
        if self.still_reach is None:
            reasons.append("main: no 'still reachable' summary found")
        elif self.still_reach != 0 and self.still_reach not in MAIN_STILL_REACHABLE_WHITELIST:
            reasons.append(f"main still_reachable={self.still_reach}")

        return reasons

    def check_child(self):
        reasons = []

        # child FD: be nice but must exist
        if self.fd_count is None:
            reasons.append("child: no FD summary found")
        elif not (1 <= self.fd_count <= 3):
            reasons.append(f"child fd_count={self.fd_count} (expected 1..3)")

        # definitely lost
        if self.def_lost is None:
            reasons.append("child: no 'definitely lost' summary found")
        elif self.def_lost != 0:
            reasons.append(f"child definitely_lost={self.def_lost}")

        # indirectly lost
        if self.ind_lost is None:
            reasons.append("child: no 'indirectly lost' summary found")
        elif self.ind_lost != 0:
            reasons.append(f"child indirectly_lost={self.ind_lost}")

        # still reachable
        if self.still_reach is None:
            reasons.append("child: no 'still reachable' summary found")
        elif self.still_reach != 0 and not IGNORE_CHILD_STILL_REACHABLE:
            reasons.append(f"child still_reachable={self.still_reach}")

        return reasons


def parse_log(path: Path):
    by_pid = {}
    order = []
    label = path.stem

    with path.open("r", errors="ignore") as f:
        for line in f:
            line = line.rstrip("\n")

            mcase = RE_CASE.search(line)
            if mcase:
                label = mcase.group("label")

            mp = RE_PID.match(line)
            if mp:
                pid = mp.group("pid")
                if pid not in by_pid:
                    by_pid[pid] = ProcSummary()
                    order.append(pid)
                by_pid[pid].merge_line(line)
            else:
                # lines without pid -> put in 'root'
                pid = "root"
                if pid not in by_pid:
                    by_pid[pid] = ProcSummary()
                    order.append(pid)
                by_pid[pid].merge_line(line)

    # choose main PID
    main_pid = None
    for pid in order:
        ps = by_pid[pid]
        if (ps.cmd and "minishell" in ps.cmd) or (ps.exe and "minishell" in ps.exe):
            main_pid = pid
            break

    if main_pid is None and order:
        # fallback: most lines
        main_pid = max(order, key=lambda p: by_pid[p].lines)

    if main_pid is None:
        main_pid = "root"

    main_reasons = by_pid[main_pid].check_main()

    child_reasons = []
    for pid in order:
        if pid == main_pid:
            continue
        child_reasons.extend(by_pid[pid].check_child())

    ok = (len(main_reasons) == 0 and len(child_reasons) == 0)
    return label, ok, main_reasons, child_reasons


def main():
    if not LOG_DIR.exists():
        print(f"No logs found: {LOG_DIR}/ (run the valgrind batch first)")
        sys.exit(1)

    logs = sorted([p for p in LOG_DIR.rglob("*.log") if p.is_file()])
    if not logs:
        print(f"No .log files under {LOG_DIR}/")
        sys.exit(1)

    total = 0
    clean = 0
    needs = []

    print("Valgrind summary (one line per log):")
    for log in logs:
        label, ok, main_reasons, child_reasons = parse_log(log)
        total += 1
        if ok:
            clean += 1
            print(f"[CLEAN] {log.name} :: {label}")
        else:
            print(f"[NEEDS_PATCH] {log.name} :: {label}")
            for r in (main_reasons + child_reasons):
                print(f"  - {r}")
            needs.append((log, label, main_reasons, child_reasons))

    print("")
    print(f"Total logs: {total}")
    print(f"CLEAN: {clean}")
    print(f"NEEDS_PATCH: {total - clean}")

    if needs:
        print("")
        print("Items to patch (command label + log path + reasons):")
        for log, label, mains, childs in needs:
            print(f"- {label} -> {log}")
            for r in (mains + childs):
                print(f"    * {r}")


if __name__ == "__main__":
    main()
#!/usr/bin/env python3
"""
Scan valgrind_logs/ and check per-command invariants.

What we consider OK:

- MAIN (the minishell process):
    - must have FD summary, and FD must be in 1..3 (usually 3)
    - EITHER:
        * has the 3 leak lines (definitely, indirectly, still) and all are 0
      OR
        * has "All heap blocks were freed -- no leaks are possible"

- CHILDREN (things run under minishell, like /bin/ls):
    - FD 1..3 is OK (valgrind for /bin/ls often shows 1 open (1 std))
    - missing leak summary is OK
    - still reachable is ignored

So: we stay strict for the main process, relaxed for children.
"""

import re
import sys
from pathlib import Path

# default logs dir (can pass another dir on the CLI)
if len(sys.argv) > 1:
    LOG_DIR = Path(sys.argv[1])
else:
    LOG_DIR = Path("valgrind_logs")

# regexes
RE_PID = re.compile(r"^==(?P<pid>\d+)==")
RE_CASE = re.compile(r"^==== CASE: (?P<label>.*) ====$")
RE_FD = re.compile(r"^==(?P<pid>\d+)== FILE DESCRIPTORS:\s+(?P<count>\d+) open")
RE_DEF = re.compile(r"^==(?P<pid>\d+)==\s+definitely lost:\s*(?P<bytes>\d+) bytes")
RE_IND = re.compile(r"^==(?P<pid>\d+)==\s+indirectly lost:\s*(?P<bytes>\d+) bytes")
RE_SR = re.compile(r"^==(?P<pid>\d+)==\s+still reachable:\s*(?P<bytes>\d+) bytes")
RE_ALL_FREED = re.compile(r"^==(?P<pid>\d+)== All heap blocks were freed -- no leaks are possible")
RE_CMD = re.compile(r"^==(?P<pid>\d+)== Command:\s*(?P<cmd>.*)")
RE_EXE = re.compile(r"^==(?P<pid>\d+)== Executable:\s*(?P<exe>.*)")

MAIN_FD_MIN = 1
MAIN_FD_MAX = 3
IGNORE_CHILD_STILL_REACHABLE = True


class ProcSummary:
    def __init__(self):
        self.fd_count = None
        self.def_lost = None
        self.ind_lost = None
        self.still_reach = None
        self.all_freed = False
        self.cmd = None
        self.exe = None
        self.lines = 0

    def merge_line(self, line: str):
        self.lines += 1

        m = RE_FD.match(line)
        if m:
            self.fd_count = int(m.group("count"))

        m = RE_DEF.match(line)
        if m:
            self.def_lost = int(m.group("bytes"))

        m = RE_IND.match(line)
        if m:
            self.ind_lost = int(m.group("bytes"))

        m = RE_SR.match(line)
        if m:
            self.still_reach = int(m.group("bytes"))

        m = RE_ALL_FREED.match(line)
        if m:
            # treat as best possible leak summary: 0/0/0
            self.all_freed = True
            self.def_lost = 0
            self.ind_lost = 0
            self.still_reach = 0

        m = RE_CMD.match(line)
        if m:
            self.cmd = m.group("cmd").strip()

        m = RE_EXE.match(line)
        if m:
            self.exe = m.group("exe").strip()

    def check_main(self):
        reasons = []

        # FD check
        if self.fd_count is None:
            reasons.append("main: no FD summary found")
        elif not (MAIN_FD_MIN <= self.fd_count <= MAIN_FD_MAX):
            reasons.append(f"main fd_count={self.fd_count} (expected {MAIN_FD_MIN}..{MAIN_FD_MAX})")

        # leak check
        if self.all_freed:
            return reasons  # perfect

        # if we don't have the 3 lines at all, flag it
        if self.def_lost is None and self.ind_lost is None and self.still_reach is None:
            reasons.append("main: no leak summary found")
            return reasons

        if self.def_lost is not None and self.def_lost != 0:
            reasons.append(f"main definitely_lost={self.def_lost}")
        if self.ind_lost is not None and self.ind_lost != 0:
            reasons.append(f"main indirectly_lost={self.ind_lost}")
        if self.still_reach is not None and self.still_reach != 0:
            reasons.append(f"main still_reachable={self.still_reach}")

        return reasons

    def check_child(self):
        reasons = []
        # child FD: accept 1..3, and also accept missing
        if self.fd_count is not None:
            if not (1 <= self.fd_count <= 3):
                reasons.append(f"child fd_count={self.fd_count} (expected 1..3)")
        # child leaks: totally optional; only flag if non-zero and we're not ignoring
        if self.def_lost is not None and self.def_lost != 0:
            reasons.append(f"child definitely_lost={self.def_lost}")
        if self.ind_lost is not None and self.ind_lost != 0:
            reasons.append(f"child indirectly_lost={self.ind_lost}")
        if self.still_reach is not None and self.still_reach != 0 and not IGNORE_CHILD_STILL_REACHABLE:
            reasons.append(f"child still_reachable={self.still_reach}")
        return reasons


def parse_log(path: Path):
    by_pid = {}
    order = []
    label = path.stem

    with path.open("r", errors="ignore") as f:
        for line in f:
            line = line.rstrip("\n")
            mcase = RE_CASE.match(line)
            if mcase:
                label = mcase.group("label")

            mp = RE_PID.match(line)
            if mp:
                pid = mp.group("pid")
                if pid not in by_pid:
                    by_pid[pid] = ProcSummary()
                    order.append(pid)
                by_pid[pid].merge_line(line)
            else:
                # no pid -> root
                pid = "root"
                if pid not in by_pid:
                    by_pid[pid] = ProcSummary()
                    order.append(pid)
                by_pid[pid].merge_line(line)

    # decide main pid
    main_pid = None
    for pid in order:
        ps = by_pid[pid]
        if (ps.cmd and "minishell" in ps.cmd) or (ps.exe and "minishell" in ps.exe):
            main_pid = pid
            break
    if main_pid is None and order:
        # fallback: the pid with most lines
        main_pid = max(order, key=lambda p: by_pid[p].lines)
    if main_pid is None:
        main_pid = "root"

    main_reasons = by_pid[main_pid].check_main()

    child_reasons = []
    for pid in order:
        if pid == main_pid:
            continue
        child_reasons.extend(by_pid[pid].check_child())

    ok = not main_reasons and not child_reasons
    return label, ok, main_reasons, child_reasons


def main():
    if not LOG_DIR.exists():
        print(f"No logs found: {LOG_DIR}/")
        sys.exit(1)

    logs = sorted([p for p in LOG_DIR.rglob("*.log") if p.is_file()])
    if not logs:
        print(f"No .log files under {LOG_DIR}/")
        sys.exit(1)

    total = 0
    clean = 0
    needs = []

    print("Valgrind summary (one line per log):")
    for log in logs:
        label, ok, main_reasons, child_reasons = parse_log(log)
        total += 1
        if ok:
            clean += 1
            print(f"[CLEAN] {log.name} :: {label}")
        else:
            print(f"[NEEDS_PATCH] {log.name} :: {label}")
            for r in main_reasons + child_reasons:
                print(f"  - {r}")
            needs.append((log, label, main_reasons, child_reasons))

    print()
    print(f"Total logs: {total}")
    print(f"CLEAN: {clean}")
    print(f"NEEDS_PATCH: {total - clean}")

    if needs:
        print()
        print("Items to patch (command label + log path + reasons):")
        for log, label, mrs, crs in needs:
            print(f"- {label} -> {log}")
            for r in mrs + crs:
                print(f"    * {r}")


if __name__ == "__main__":
    main()

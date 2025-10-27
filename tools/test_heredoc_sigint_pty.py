#!/usr/bin/env python3
import os, pty, time, signal, re, glob
from select import select

PROMPT_WAIT = 1.0

def read_all(fd, timeout=1.0):
    out = b""
    end = time.time() + timeout
    while time.time() < end:
        r, _, _ = select([fd], [], [], 0.05)
        if fd in r:
            try:
                chunk = os.read(fd, 8192)
            except OSError:
                break
            if not chunk:
                break
            out += chunk
    return out

pid, fd = pty.fork()
if pid == 0:
    os.execv("./minishell", ["./minishell"])

# drain initial prompt
read_all(fd, PROMPT_WAIT)

# enter heredoc then send Ctrl-C
os.write(fd, b"cat << EOF\n")
read_all(fd, 0.2)
os.kill(pid, signal.SIGINT)
time.sleep(0.15)

# wait until minishell prints __RC: then exit
def read_until(fd, pattern=b"__RC:", timeout=2.0):
    buf = b""
    end = time.time() + timeout
    while time.time() < end:
        chunk = read_all(fd, 0.1)
        if chunk:
            buf += chunk
            if pattern in buf:
                return buf
    return buf

out = read_all(fd, 0.2)
# wait for prompt tail to appear to avoid readline redraw race
_ = read_until(fd, b"$ ", timeout=2.0)
os.write(fd, b"\n")
read_all(fd, 0.2)
# save status, print it, and exit with that status to preserve EXIT=130
os.write(fd, b'status="$?"; printf "__RC:%s\\n" "$status"; exit "$status"\n')
out = read_until(fd, b"__RC:", timeout=2.0)
out += read_all(fd, 0.8)

# collect exit status and temp files
m = re.search(rb"__RC:(\d+)", out)
rc = m.group(1).decode() if m else "MISSING"
temps = len(glob.glob(".temp*")) + len(glob.glob("heredoc*")) + len(glob.glob(".here*"))

try:
    _, status = os.waitpid(pid, 0)
    exit_info = f"EXIT={os.WEXITSTATUS(status)}" if os.WIFEXITED(status) else f"SIGNAL={os.WTERMSIG(status)}"
except ChildProcessError:
    exit_info = "WAITPID-ERROR"

print("SCENARIO1_OUT:")
print(out.decode(errors="ignore"))
print(f"SCENARIO1_PARSED_RC={rc}")
print(f"SCENARIO1_TEMP_COUNT={temps}")
print(f"SCENARIO1_{exit_info}")
ok1 = (rc == "130" and temps == 0 and exit_info == "EXIT=130")

# Scenario 2: interrupt during second heredoc in the same command
pid2, fd2 = pty.fork()
if pid2 == 0:
    os.execv("./minishell", ["./minishell"])

read_all(fd2, PROMPT_WAIT)
os.write(fd2, b"cat <<A <<B\n")
read_all(fd2, 0.2)
# Provide content for first heredoc then close it
os.write(fd2, b"ignored line for A\n")
read_all(fd2, 0.1)
os.write(fd2, b"A\n")
read_all(fd2, 0.2)
# Now interrupt while reading second heredoc (B)
os.kill(pid2, signal.SIGINT)
time.sleep(0.15)
_ = read_until(fd2, b"$ ", timeout=2.0)
os.write(fd2, b"\n")
read_all(fd2, 0.2)
os.write(fd2, b'status="$?"; printf "__RC:%s\\n" "$status"; exit "$status"\n')
out2 = read_until(fd2, b"__RC:", timeout=2.0)
out2 += read_all(fd2, 0.8)
try:
    _, status2 = os.waitpid(pid2, 0)
    exit_info2 = f"EXIT={os.WEXITSTATUS(status2)}" if os.WIFEXITED(status2) else f"SIGNAL={os.WTERMSIG(status2)}"
except ChildProcessError:
    exit_info2 = "WAITPID-ERROR"
temps2 = len(glob.glob(".temp*")) + len(glob.glob("heredoc*")) + len(glob.glob(".here*"))
m2 = re.search(rb"__RC:(\d+)", out2)
rc2 = m2.group(1).decode() if m2 else "MISSING"

print("SCENARIO2_OUT:")
print(out2.decode(errors="ignore"))
print(f"SCENARIO2_PARSED_RC={rc2}")
print(f"SCENARIO2_TEMP_COUNT={temps2}")
print(f"SCENARIO2_{exit_info2}")
ok2 = (rc2 == "130" and temps2 == 0 and exit_info2 == "EXIT=130")

if not (ok1 and ok2):
    raise SystemExit(1)

#!/usr/bin/env python3
"""One branch-only compiler diagnostic. Captured tool output stays private."""
import hashlib
import json
import os
from pathlib import Path
import re
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
UNIT = ROOT / "src/dds2/game/code_001DD390.c"
OBJECT = ROOT / "build/dds2/src/dds2/game/code_001DD390.o"
TARGET = "func_001FA480"
ANCHOR = 'INCLUDE_ASM(const s32, "game/code_001DD390", func_001FA480);'
BASE = "30681a229beca800475559744725bdf133363a54"
NAME = r"[A-Za-z_][A-Za-z0-9_]*"
HEX = r"[0-9A-F]+"
STAGE = "start"
ENV = dict(os.environ)
for key in ("DDS_I386_LIBDIR", "DDS_ALLOW_ASLR", "DDS_AS_UNIT", "DDS_KEEP_S", "DDS_VERSION"):
    ENV.pop(key, None)
ENV["DDS_I386_LIBDIR"] = str(ROOT / "tools/glibc32")

class Failure(Exception):
    def __init__(self, stage, returncode=None):
        self.stage = stage
        self.returncode = returncode

def emit(value):
    print(json.dumps(value, sort_keys=True), flush=True)

def require(condition, stage):
    if not condition:
        raise Failure(stage)

def run(stage, args, allowed=(0,)):
    global STAGE
    STAGE = stage
    result = subprocess.run(args, cwd=ROOT, env=ENV, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, text=True, errors="replace",
                            check=False, timeout=900)
    if result.returncode not in allowed:
        raise Failure(stage, result.returncode)
    return result

def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()

def object_sizes(label):
    OBJECT.unlink(missing_ok=True)
    run(label + "_compile", ["ninja", str(OBJECT.relative_to(ROOT))])
    result = run(label + "_symbols", ["tools/bin/mips-ps2-decompals-nm",
                 "-S", "--defined-only", str(OBJECT)])
    sizes = {}
    for line in result.stdout.splitlines():
        match = re.fullmatch(rf"[0-9a-fA-F]+ ([0-9a-fA-F]+) [Tt] ({NAME})", line.strip())
        if match:
            sizes[match[2]] = int(match[1], 16)
    require(TARGET in sizes, label + "_target_symbol")
    return sizes

def check(label, sizes):
    result = run(label + "_checker", [sys.executable, "tools/check_unit.py",
                 str(UNIT.relative_to(ROOT))], allowed=(0, 1))
    rows, contexts, totals = {}, set(), []
    data_issues = 0
    for line in result.stdout.splitlines():
        match = re.fullmatch(r"(\d+) match, (\d+) differ", line)
        if match:
            totals.append((int(match[1]), int(match[2])))
            continue
        match = re.fullmatch(rf"OK   ({NAME}) @ 0x{HEX} "
            r"\((?:(\d+) bytes|as built in the full unit; the SKIP_ASM compile differs)\)", line)
        if match:
            rows[match[1]] = dict(status="ok", checker_bytes=int(match[2]) if match[2] else None,
                                  differing_words=0)
            continue
        match = re.fullmatch(rf"DIFF ({NAME}) @ 0x{HEX}: (\d+) of (\d+) words differ "
                             rf"\(first at \+0x{HEX}\)", line)
        if match:
            rows[match[1]] = dict(status="diff", checker_bytes=int(match[3]) * 4,
                                  differing_words=int(match[2]))
            continue
        match = re.match(rf"CONTEXT ({NAME}):", line)
        if match:
            contexts.add(match[1])
            continue
        match = re.match(rf"OVER ({NAME}) @ ", line)
        if match:
            rows[match[1]] = dict(status="over", checker_bytes=None, differing_words=None)
            continue
        match = re.match(rf"\?    ({NAME}): no address", line)
        if match:
            rows[match[1]] = dict(status="unknown_address", checker_bytes=None, differing_words=None)
            continue
        if re.match(r"^(?:RODATA |MERGED rodata:|DIFF (?:rodata|jump table|sdata) |"
                    r"PAD (?:rodata|jump table) |SHARED (?:rodata|sdata) |RELOC sdata |DATA )", line):
            data_issues += 1
        # Never print unrecognized lines, instruction words, byte reprs, or stderr.
    require(len(totals) == 1, label + "_summary_missing")
    matched, bad = totals[0]
    require(matched == sum(row["status"] == "ok" for row in rows.values()),
            label + "_summary_inconsistent")
    require(result.returncode == (1 if bad else 0), label + "_exit_inconsistent")
    for name, row in rows.items():
        row["full_object_bytes"] = sizes.get(name)
        row["context_differs"] = name in contexts
    function_issues = sum(row["status"] != "ok" for row in rows.values())
    other = bad - function_issues - len(contexts) - data_issues
    require(other >= 0, label + "_issue_counts_inconsistent")
    return dict(returncode=result.returncode, matched=matched, issues=bad,
                data_issues=data_issues, context_issues=len(contexts), other_issues=other,
                stderr_present=bool(result.stderr), functions=rows)

def apply_source_patch(source, patch):
    """Apply only unique exact source contexts; preserve all unrelated text."""
    hunks = []
    current = None
    for line in patch.splitlines():
        if line.startswith("@@"):
            current = [[], []]
            hunks.append(current)
        elif current is not None and line.startswith(" "):
            current[0].append(line[1:])
            current[1].append(line[1:])
        elif current is not None and line.startswith("-"):
            current[0].append(line[1:])
        elif current is not None and line.startswith("+"):
            current[1].append(line[1:])
    require(len(hunks) == 10, "patch_hunk_count")
    for old_lines, new_lines in hunks:
        old, new = "\n".join(old_lines), "\n".join(new_lines)
        require(bool(old) and source.count(old) == 1, "patch_context_changed")
        source = source.replace(old, new, 1)
    return source

def main():
    original = UNIT.read_bytes()
    blob = hashlib.sha1(b"blob " + str(len(original)).encode() + b"\0" + original).hexdigest()
    expected_blob = run("source_identity", ["git", "rev-parse",
                        "HEAD:" + str(UNIT.relative_to(ROOT))]).stdout.strip()
    require(blob == expected_blob, "baseline_source_changed")
    text = original.decode("utf-8")
    patch = Path(__file__).with_name("candidate.patch").read_text(encoding="utf-8")
    candidate = apply_source_patch(text, patch)
    old_selector = "                        case 36:\n                        default:\n"
    require(candidate.count(old_selector) == 1, "selector_context_changed")
    candidate = candidate.replace(old_selector, "                        case 36:\n", 1)
    require(text.count(ANCHOR) == 1, "anchor_count")
    require(candidate != text and ANCHOR not in candidate, "candidate_input")
    emit(dict(scope="focused_unit_diagnostic", base=BASE,
              full_retail_build_verified=False))
    try:
        run("configure", [sys.executable, "configure.py", "dds2"])
        require(UNIT.read_bytes() == original, "configure_changed_source")
        baseline_sizes = object_sizes("baseline")
        baseline = check("baseline", baseline_sizes)
        emit(dict(phase="baseline", **baseline))
        require(baseline["issues"] == 0, "baseline_not_clean")
        require(TARGET not in baseline["functions"], "target_already_c")
        UNIT.write_text(candidate, encoding="utf-8")
        candidate_sizes = object_sizes("candidate")
        candidate = check("candidate", candidate_sizes)
        emit(dict(phase="candidate", **candidate))
    finally:
        UNIT.write_bytes(original)
    require(UNIT.read_bytes() == original, "restore_failed")
    regressions = []
    for name, old in baseline["functions"].items():
        new = candidate["functions"].get(name)
        if (new is None or new["status"] != "ok" or new["context_differs"]
                or new["full_object_bytes"] != old["full_object_bytes"]):
            regressions.append(name)
    target = candidate["functions"].get(TARGET)
    size_ok = candidate_sizes[TARGET] == baseline_sizes[TARGET]
    clean = (candidate["issues"] == 0 and not regressions and target is not None
             and target["status"] == "ok" and not target["context_differs"] and size_ok)
    emit(dict(scope="focused_unit_diagnostic", focused_checks_clean=clean,
              full_retail_build_verified=False, older_c_functions=len(baseline["functions"]),
              older_regression_count=len(regressions), older_regressions=regressions,
              target_original_bytes=baseline_sizes[TARGET],
              target_candidate_bytes=candidate_sizes[TARGET],
              target_size_preserved=size_ok, source_restored=True))
    return 0 if clean else 1

if __name__ == "__main__":
    try:
        code = main()
    except Failure as exc:
        emit(dict(status="diagnostic_failed", stage=exc.stage,
                  returncode=exc.returncode, full_retail_build_verified=False))
        code = 2
    except BaseException:
        emit(dict(status="diagnostic_failed", stage=STAGE, full_retail_build_verified=False))
        code = 2
    raise SystemExit(code)

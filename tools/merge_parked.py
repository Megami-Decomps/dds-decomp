#!/usr/bin/env python3
"""Put parked candidates that now match back into their units.

    python3 tools/merge_parked.py dds1 [--dry-run]

A parked candidate (build/parked/<v>/<dir>/<unit>/<func>.c: the function's
natural C plus the declarations it needs) can start matching after its
functions move to a unit with the right options (a split sibcall-free file)
or after a tool fix. For each one whose function is still INCLUDE_ASM in
some unit, this compiles the unit with the candidate in place of that line.
When the function then matches, its definition replaces the INCLUDE_ASM line,
the declarations the unit lacks go after its #include lines, and the whole
unit is checked again; anything short of clean restores the unit. Merged
candidates are deleted.
"""
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def check(unit, *extra):
    return subprocess.run([sys.executable, str(ROOT / "tools/check_unit.py"), str(unit), *extra],
                          capture_output=True, text=True)


def split_candidate(text, func):
    """(declarations, definition) of a parked file."""
    m = re.search(rf"^[A-Za-z_][^;\n]*\b{func}\s*\([^;]*?\)\s*(?:\n[^\n{{;]*;)*\s*\{{.*?^\}}\n?", text, re.M | re.S)
    if not m:
        return None
    decls = [l for l in (text[:m.start()] + text[m.end():]).splitlines()
             if l.strip() and not l.startswith("#include")]
    return decls, m.group(0).rstrip() + "\n\n"


def merge(unit, func, cand):
    text = unit.read_text()
    line = re.compile(rf'^INCLUDE_ASM\([^\n]*\b{func}\);\n', re.M)
    parts = split_candidate(cand.read_text(), func)
    if not line.search(text) or not parts:
        return "skip"
    decls, body = parts
    with tempfile.TemporaryDirectory() as tmp:
        src = Path(tmp) / unit.name
        src.write_text(line.sub(lambda _: cand.read_text().strip() + "\n", text, count=1))
        if not re.search(rf"^OK\s+{func}\b", check(unit, "--func", func, "--source", str(src)).stdout, re.M):
            return "differs"
    have = set(text.splitlines())
    new = [d for d in decls if d not in have]
    merged = line.sub(lambda _: body, text, count=1)
    if new:
        incs = list(re.finditer(r"^#include [^\n]*\n", merged, re.M))
        at = incs[-1].end() if incs else 0
        merged = merged[:at] + "\n" + "\n".join(new) + "\n" + merged[at:]
    unit.write_text(merged)
    r = check(unit)
    if r.returncode != 0:
        unit.write_text(text)
        return "unit not clean: " + " | ".join(l for l in r.stdout.splitlines() if not l.startswith("OK"))[:200]
    return "merged"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--dry-run", action="store_true", help="only report which candidates match")
    args = ap.parse_args()
    units = sorted((ROOT / "src" / args.version).rglob("*.c"))
    owner = {}
    for u in units:
        for f in re.findall(r'^INCLUDE_ASM\([^\n]*\b(\w+)\);', u.read_text(), re.M):
            owner[f] = u
    merged = 0
    for cand in sorted((ROOT / "build/parked" / args.version).rglob("*.c")):
        unit = owner.get(cand.stem)
        if unit is None:
            continue
        if args.dry_run:
            with tempfile.TemporaryDirectory() as tmp:
                src = Path(tmp) / unit.name
                src.write_text(re.sub(rf'^INCLUDE_ASM\([^\n]*\b{cand.stem}\);$', lambda _: cand.read_text().strip(),
                                      unit.read_text(), count=1, flags=re.M))
                ok = re.search(rf"^OK\s+{cand.stem}\b", check(unit, "--func", cand.stem, "--source", str(src)).stdout, re.M)
            print(f"{cand.stem} ({unit.relative_to(ROOT)}): {'matches' if ok else '-'}", flush=True)
            continue
        result = merge(unit, cand.stem, cand)
        if result == "merged":
            cand.unlink()
            merged += 1
        if result != "differs":
            print(f"{cand.stem} ({unit.relative_to(ROOT)}): {result}", flush=True)
    if not args.dry_run:
        print(f"{merged} merged")


if __name__ == "__main__":
    main()

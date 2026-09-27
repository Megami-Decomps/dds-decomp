#!/usr/bin/env python3
"""Bulk first pass: romwright export-c drafts -> C that byte-matches, per unit.

    python3 tools/rw_bulk.py dds1 effect/effManager file/fileManager ...
    python3 tools/rw_bulk.py dds1 --all --skip sdf/sdfMotion --skip kernel/...
    python3 tools/rw_bulk.py dds1 --all --dry-run        # report only, no edits

Uses its own romwright store, build/romwright-c: created on first run by importing
the ELF (romwright with MIPS gp seeding) and asserting every symbol_addrs.txt
function name, so drafts speak repo symbols. Drafts are exported
per function (`export-c --address`, in parallel) into build/rw_c/<v>/ and reused
on later runs (--refresh re-exports). A whole-program export publishes nothing
unless every function validates, so one bad function would lose the batch.

For every INCLUDE_ASM function of the selected units:
1. clean the draft into repo style: our func_/D_ names, argN parameters,
   common.h integer types, no Ghidra comments. Drafts with romwright residue
   (storage-view member access, unsupported intrinsics, unknown register inputs,
   Ghidra's `code` type, literal addresses inside the image) are skipped.
2. check it alone (tools/check_unit.py) as two variants: signatures narrowed to
   32 bits first (ee-gcc moves every int with a 64-bit daddu, so romwright infers
   `long`), then the draft as emitted. The first byte-identical variant wins.
3. insert the winners into the unit in order, re-checking the whole unit after
   each one; a function that breaks the unit (conflicting declaration, emitted
   data) is reverted.
"""
import argparse
import concurrent.futures
import json
import os
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RW = os.environ.get("ROMWRIGHT", str(Path.home() / "ventris/target/release/romwright-cli"))
RETAIL = {"dds1": "orig/dds1/SLUS_209.74", "dds2": "orig/dds2/SLUS_211.52"}
STORE = ROOT / "build/romwright-c"
SYMBOL = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);\s*// type:func", re.M)
INCLUDE_ASM = re.compile(r'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*(\w+)\);$', re.M)
ENV = dict(os.environ, DDS_I386_LIBDIR=os.environ.get("DDS_I386_LIBDIR", str(Path.home() / "opt/glibc32/usr/lib")))
TYPES = [
    (r"\bunsigned long long\b", "u64"), (r"\blong long\b", "s64"),
    (r"\bundefined8\b|\bulonglong\b|\bulong\b", "u64"), (r"\blonglong\b", "s64"),
    (r"\bundefined4\b|\buint\b", "u32"), (r"\bundefined2\b|\bushort\b", "u16"),
    (r"\bundefined1\b|\bundefined\b|\bbyte\b|\buchar\b|\bbool\b", "u8"),
    (r"\bunsigned int\b", "u32"), (r"\bunsigned short\b", "u16"), (r"\bunsigned char\b", "u8"),
    (r"\blong\b", "s64"), (r"\bunsigned\b", "u32"),
    (r"\bint\b", "s32"), (r"\bshort\b", "s16"), (r"\bchar\b", "s8"),
]
NARROW = [(r"\bu64\b", "u32"), (r"\bs64\b", "s32")]


def functions(version):
    """name -> vram for every function in config/<v>/symbol_addrs.txt."""
    text = (ROOT / "config" / version / "symbol_addrs.txt").read_text()
    return {name: int(addr, 16) for name, addr in SYMBOL.findall(text)}


def setup_store(version):
    """Import the version into STORE (gp-seeding romwright) and assert our names,
    so drafts use repo symbols. Skipped when the program is already there."""
    listed = subprocess.run([RW, "programs", "--project", str(STORE)], capture_output=True, text=True)
    if re.search(rf"^{version}\b", listed.stdout, re.M):
        return
    STORE.mkdir(parents=True, exist_ok=True)
    subprocess.run([RW, "import", str(ROOT / RETAIL[version]), "--name", version,
                    "--project", str(STORE)], check=True, capture_output=True)
    names = json.dumps([{"function": hex(a), "name": n} for n, a in functions(version).items()])
    subprocess.run([RW, "assert-names", version, "-", "--project", str(STORE)],
                   input=names, text=True, check=True, capture_output=True)


def export(version, funcs, refresh, jobs):
    """Export the drafts for `funcs` not already on disk; return the draft dir."""
    out = ROOT / "build/rw_c" / version
    out.mkdir(parents=True, exist_ok=True)
    address = functions(version)
    missing = [f for f in funcs if f in address and (refresh or not (out / f"{f}.c").exists())]

    def one(func):
        subprocess.run([RW, "export-c", str(ROOT / RETAIL[version]), version, str(out),
                        "--address", f"{address[func]:08X}", "--project", str(STORE)], capture_output=True)

    with concurrent.futures.ThreadPoolExecutor(jobs) as pool:
        list(pool.map(one, missing))
    return out


STORAGE = re.compile(r"^extern union romwright_storage_\w+?_(\d+)_\d+ (\w+);", re.M)
STORAGE_TYPE = {"1": "u8", "2": "u16", "4": "u32", "8": "u64"}
# Residue meaning the draft is not plain C: romwright helpers/views, unsupported
# intrinsics, unknown register inputs, Ghidra sub-field access. Warning banners
# ("irregular: N native warning(s)") are not rejected: the compile check decides.
UNUSABLE = re.compile(r"romwright_|Unsupported intrinsic|\bin_\w+|\._\d+_\d+_|\bcode\b")


def clean(text, func):
    """Draft -> (declarations, definition) in repo style, or None if unusable."""
    # Storage-view unions are romwright's placeholder for untyped globals: keep the
    # width, drop the union and its `.whole` member access.
    text = re.sub(r"^#ifndef ROMWRIGHT_STORAGE_\w+\n.*?^#endif\n", "", text, flags=re.M | re.S)
    text = STORAGE.sub(lambda m: f"extern {STORAGE_TYPE.get(m[1], 'u8')} {m[2]};", text)
    text = re.sub(r"(\w+)\.whole\b", r"\1", text)
    text = re.sub(r"^typedef \w+ \w+ romwright_u?int128 .*\n", "", text, flags=re.M)
    define = re.search(rf"^\w[^;\n]*\b{func.replace('func_', 'FUN_').lower()}\s*\(|^\w[^;\n]*\b{func}\s*\(",
                       text, flags=re.M | re.I)
    if define is None:
        return None
    head, body_text = text[:define.start()], text[define.start():]
    if UNUSABLE.search(body_text) or "Unsupported intrinsic" in head:
        return None
    # A literal inside the loaded image is a global romwright could not name; it would
    # match here but never port to the other game, so the draft needs a symbol first.
    if any(0x100000 <= int(h, 16) < 0x1000000 for h in re.findall(r"\b0x([0-9a-fA-F]{6})\b", body_text)):
        return None
    text = re.sub(r"/\*.*?\*/", "", head, flags=re.S) + "\x00" + re.sub(r"/\*.*?\*/", "", body_text, flags=re.S)
    text = re.sub(r"\bFUN_([0-9a-f]{8})\b", lambda m: "func_" + m[1].upper(), text)
    # DAT_, _DAT_, Ghidra's typed-RAM names (uRam0034e028), string labels
    # (s__2_2_2_00324770) and PTR_ labels are all plain globals.
    text = re.sub(r"\b_?DAT_([0-9a-f]{8})\b|\b[a-z]{1,3}Ram([0-9a-f]{8})\b|\b(?:s|PTR)_\w*?_([0-9a-f]{8})\b",
                  lambda m: "D_" + (m[1] or m[2] or m[3]).upper(), text)
    text = re.sub(r"\bparam_(\d+)\b", lambda m: f"arg{int(m[1]) - 1}", text)
    for pattern, repl in TYPES:
        text = re.sub(pattern, repl, text)
    head, body_text = text.split("\x00")
    decls = [line.rstrip() for line in head.splitlines() if line.startswith("extern ")]
    body_text = body_text.strip()
    body_text = re.sub(r"\)\s*\n\s*\n?\{", ") {", body_text, count=1)
    body_text = re.sub(r"\n\s*\n(\s*\n)+", "\n\n", body_text)
    body_text = re.sub(r"[ \t]+$", "", body_text, flags=re.M)
    body_text = re.sub(r"\{\n\n+", "{\n", body_text, count=1)
    body_text = re.sub(r"\n\s*return;\n\}$", "\n}", body_text)
    body_text = re.sub(r"^( +)", lambda m: m[1] * 2, body_text, flags=re.M)
    body_text = re.sub(r",(?=\S)", ", ", body_text)
    return decls, body_text


def variants(decls, body):
    def narrow(s):
        for pattern, repl in NARROW:
            s = re.sub(pattern, repl, s)
        return s
    head, rest = body.split("{", 1)
    narrowed = ([narrow(d) for d in decls], narrow(head) + "{" + rest)
    out = [narrowed]
    if narrowed != (decls, body):
        out.append((decls, body))
    return out


def unit_text(decls, body):
    return '#include "common.h"\n\n' + "\n".join(decls) + "\n\n" + body + "\n"


def check(path, func=None):
    cmd = [sys.executable, str(ROOT / "tools/check_unit.py"), str(path)]
    if func:
        cmd += ["--func", func]
    r = subprocess.run(cmd, capture_output=True, text=True, env=ENV)
    return r.returncode == 0, r.stdout + r.stderr


def try_isolated(version, func, draft_path):
    parsed = clean(draft_path.read_text(), func)
    if parsed is None:
        return func, None, "unusable draft"
    probe_dir = ROOT / "src" / version / "zz_rwprobe"
    probe_dir.mkdir(exist_ok=True)
    probe = probe_dir / f"{func}.c"
    try:
        for decls, body in variants(*parsed):
            if not re.search(rf"\b{func}\s*\(", body.split("{", 1)[0]):
                return func, None, "definition name differs"
            probe.write_text(unit_text(decls, body))
            ok, _ = check(probe, func)
            if ok:
                return func, (decls, body), "ok"
        return func, None, "differs"
    finally:
        probe.unlink(missing_ok=True)


def tidy(text):
    """Layout only: no trailing blanks, one blank line between top-level items."""
    text = re.sub(r"[ \t]+$", "", text, flags=re.M)
    text = re.sub(r"^\}\n(?=\S)", "}\n\n", text, flags=re.M)
    text = re.sub(r"^(INCLUDE_ASM\(.*\);)\n(?=[^\sI])", r"\1\n\n", text, flags=re.M)
    return re.sub(r"\n{3,}", "\n\n", text)


def merge(unit_path, wins):
    """Insert matched functions one at a time; keep each only if the unit stays clean."""
    kept = []
    for func, (decls, body) in wins:
        before = unit_path.read_text()
        text = re.sub(rf'^INCLUDE_ASM\([^,]+,\s*"[^"]+",\s*{func}\);$', lambda _: body, before, count=1, flags=re.M)
        new_decls = [d for d in decls if d not in text]
        if new_decls:
            at = text.index("\n", text.index("#include")) + 1
            text = text[:at] + "\n" + "\n".join(new_decls) + "\n" + text[at:]
        unit_path.write_text(tidy(text))
        ok, _ = check(unit_path)
        if ok:
            kept.append(func)
        else:
            unit_path.write_text(before)
    return kept


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=sorted(RETAIL))
    ap.add_argument("units", nargs="*", help="unit paths relative to src/<v> without .c")
    ap.add_argument("--all", action="store_true", help="every C unit of the version")
    ap.add_argument("--skip", action="append", default=[], help="unit to leave alone (repeatable)")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--refresh", action="store_true", help="re-export drafts")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count())
    args = ap.parse_args()

    src = ROOT / "src" / args.version
    units = args.units or ([str(p.relative_to(src).with_suffix("")) for p in sorted(src.rglob("*.c"))
                            if "zz_rwprobe" not in p.parts] if args.all else [])
    units = [u for u in units if u not in args.skip and not u.startswith("sdk/")]
    if not units:
        ap.error("no units selected")
    setup_store(args.version)
    wanted = [(unit, func) for unit in units for func in INCLUDE_ASM.findall((src / f"{unit}.c").read_text())]
    drafts = export(args.version, [f for _, f in wanted], args.refresh, args.jobs)

    todo = [(unit, func, drafts / f"{func}.c") for unit, func in wanted if (drafts / f"{func}.c").exists()]
    results = {}
    with concurrent.futures.ThreadPoolExecutor(args.jobs) as pool:
        for func, win, why in pool.map(lambda t: try_isolated(args.version, t[1], t[2]), todo):
            results[func] = (win, why)
    (src / "zz_rwprobe").rmdir() if (src / "zz_rwprobe").exists() else None

    total_kept = 0
    for unit in units:
        funcs = [f for u, f, _ in todo if u == unit]
        wins = [(f, results[f][0]) for f in funcs if results[f][0]]
        if not wins:
            continue
        if args.dry_run:
            print(f"{unit}: {len(wins)} of {len(funcs)} drafts match alone")
            continue
        kept = merge(src / f"{unit}.c", wins)
        total_kept += len(kept)
        print(f"{unit}: {len(kept)} matched ({len(wins) - len(kept)} reverted in unit) of {len(funcs)} drafts")
    reasons = {}
    for win, why in results.values():
        reasons[why] = reasons.get(why, 0) + 1
    print(f"drafts: {len(todo)}; isolated results {reasons}; kept {total_kept}")


if __name__ == "__main__":
    main()

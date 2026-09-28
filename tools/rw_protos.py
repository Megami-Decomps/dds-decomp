#!/usr/bin/env python3
"""Feed the prototypes of matched C functions back into the romwright store.

    python3 tools/rw_protos.py dds1 [--dry-run]

Every function defined in src/<v> matches retail, so its parameter and return
types are known. Asserting them in build/romwright-c lets romwright type the
calls in its drafts of the remaining functions (pointer vs int arguments,
void returns), which turns part of rw_bulk.py's near-miss drafts into
matches. Re-export with `rw_bulk.py <v> --all --refresh` afterwards.
"""
import os
import argparse
import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
RW = os.environ.get("ROMWRIGHT", "romwright-cli")
DEF = re.compile(r"^(?!static\b|extern\b)([A-Za-z_][\w \t\*]*?)\b(\w+)\s*\(([^;{)]*)\)\s*\{?\s*$", re.M)
ROW = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);", re.M)
SCALAR = {"s8": "char", "u8": "uchar", "s16": "short", "u16": "ushort", "s32": "int", "u32": "uint",
          "s64": "longlong", "u64": "ulonglong", "f32": "float", "f64": "double", "void": "void",
          "int": "int", "char": "char", "float": "float", "short": "short"}


def romwright_type(c_type):
    c_type = re.sub(r"\b(const|volatile|struct|union|enum)\b", "", c_type).strip()
    if "*" in c_type or c_type.endswith("]"):
        return "void *"
    return SCALAR.get(c_type.split()[-1] if c_type else "int", "int")


def signature(ret, name, params):
    params = params.strip()
    if not params or params == "void":
        args = "void"
    else:
        out = []
        for i, p in enumerate(params.split(",")):
            p = p.strip()
            if p == "...":
                out.append("...")
                continue
            m = re.match(r"(.*?)(\w+)\s*(\[[^\]]*\])?$", p)
            typ = (m.group(1) + ("*" if m.group(3) else "")) if m and m.group(1).strip() else p
            out.append(f"{romwright_type(typ)} param_{i + 1}")
        args = ", ".join(out)
    return f"{romwright_type(ret)} {name}({args})"


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("--dry-run", action="store_true")
    args = ap.parse_args()
    names = {n: int(a, 16) for n, a in ROW.findall((ROOT / "config" / args.version / "symbol_addrs.txt").read_text())}
    rows = {}
    for c in (ROOT / "src" / args.version).rglob("*.c"):
        text = re.sub(r"/\*.*?\*/|//[^\n]*", "", c.read_text(), flags=re.S)
        for m in DEF.finditer(text):
            ret, name, params = m.groups()
            if not ret.strip() or "(" in ret or name in ("if", "while", "for", "switch", "return"):
                continue
            addr = names.get(name)
            if addr is None and re.fullmatch(r"func_[0-9A-F]{8}", name):
                addr = int(name[5:], 16)
            if addr is None or not re.search(r"\w", params + " ") or re.search(r"[;=]", params):
                continue
            rows[addr] = {"function": f"0x{addr:08X}", "signature": signature(ret, name, params)}
    print(f"{len(rows)} prototypes")
    if args.dry_run:
        for r in list(rows.values())[:10]:
            print(r["signature"])
        return
    out = subprocess.run([RW, "assert-prototypes", args.version, "-", "--source", "matched-c",
                          "--project", str(ROOT / "build/romwright-c")],
                         input=json.dumps(list(rows.values())), capture_output=True, text=True)
    print(out.stdout.strip() or out.stderr.strip())


if __name__ == "__main__":
    main()

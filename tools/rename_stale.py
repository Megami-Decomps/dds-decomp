#!/usr/bin/env python3
"""Replace func_XXXXXXXX references whose address symbol_addrs now names.

check_unit reports these as STALE: a fresh split no longer defines the old
label, so the link fails even though a stale local asm tree still builds.

    tools/rename_stale.py <dds1|dds2> [unit.c ...]
"""
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def main():
    version, units = sys.argv[1], sys.argv[2:]
    names = {}
    for name, addr in re.findall(r"^\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;[^\n]*type:func",
                                 (ROOT / "config" / version / "symbol_addrs.txt").read_text(), re.M):
        if not name.startswith("func_"):
            names.setdefault(int(addr, 16), name)
    paths = [Path(u) for u in units] or sorted((ROOT / "src" / version).rglob("*.c"))
    for path in paths:
        text = path.read_text()
        new = re.sub(r"\bfunc_([0-9A-F]{8})\b", lambda m: names.get(int(m.group(1), 16), m.group(0)), text)
        if new != text:
            path.write_text(new)
            print(path)


if __name__ == "__main__":
    main()

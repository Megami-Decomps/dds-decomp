#!/usr/bin/env python3
"""Replace func_XXXXXXXX / D_XXXXXXXX references whose address symbol_addrs now names.

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
    funcs, data = {}, {}
    rows = (ROOT / "config" / version / "symbol_addrs.txt").read_text()
    for line in rows.splitlines():
        m = re.match(r"\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*;", line)
        if not m or m.group(1).startswith(("func_", "D_")):
            continue
        table = funcs if "type:func" in line else data
        table.setdefault(int(m.group(2), 16), m.group(1))
    paths = [Path(u) for u in units] or sorted((ROOT / "src" / version).rglob("*.c"))
    for path in paths:
        text = path.read_text()
        new = re.sub(r"\bfunc_([0-9A-F]{8})\b", lambda m: funcs.get(int(m.group(1), 16), m.group(0)), text)
        # Data globals named later (e.g. synced from the other game by shared_funcs.py names).
        new = re.sub(r"\bD_([0-9A-F]{8})\b", lambda m: data.get(int(m.group(1), 16), m.group(0)), new)
        if new != text:
            path.write_text(new)
            print(path)


if __name__ == "__main__":
    main()

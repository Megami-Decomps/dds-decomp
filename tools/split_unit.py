#!/usr/bin/env python3
"""Split a C unit at function addresses, for a file boundary the unit hides.

    python3 tools/split_unit.py dds1 game/code_001F6110 0x001FC7D8 [0x001FF030]

Functions from the first address (up to the second, if given) move to a new
unit `game/code_<first>`; with an end address the rest after it becomes
`game/code_<end>`. Each new file gets the declarations its functions use, the
config/<v>/*.yaml text rows are inserted, and INCLUDE_RODATA lines travel with
the function they precede. Afterwards: `configure.py --force-split <v>`, then
`split_rodata.py <v> --write` and `--section lit4 --write`, then split again.

Only split on evidence (a proven TU boundary, a change of compiler options);
record it in the commit and, for options, in config/<v>/cflags.txt.
"""
import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from shared_funcs import DEF, TOKENS, blocks, declared_name  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = json.loads((ROOT / "config/versions.json").read_text())
INCLUDE = re.compile(r'INCLUDE_(ASM|RODATA)\(const s32, "([^"]+)", (\w+)\);')
ROW = re.compile(r"^(\w+) = 0x([0-9A-Fa-f]+);", re.M)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version")
    ap.add_argument("unit")
    ap.add_argument("start")
    ap.add_argument("end", nargs="?")
    args = ap.parse_args()
    start, end = int(args.start, 16), int(args.end, 16) if args.end else None
    names = {n: int(a, 16) for n, a in ROW.findall((ROOT / "config" / args.version / "symbol_addrs.txt").read_text())}
    src = ROOT / "src" / args.version / f"{args.unit}.c"
    head, pieces, pending = [], [], []  # pieces: (address, [blocks])
    for b in blocks(src.read_text()):
        inc = INCLUDE.search(b)
        m = DEF.search(b) if "{" in b and not inc else None
        name = inc.group(3) if inc and inc.group(1) == "ASM" else (m.group(1) if m else None)
        if inc and inc.group(1) == "RODATA":
            pending.append(b)
        elif name:
            addr = names.get(name) or int(name[-8:], 16)
            pieces.append((addr, pending + [b]))
            pending = []
        elif b.startswith("#include"):
            continue
        else:
            head.append(b)
    if pending:
        pieces[-1][1].extend(pending)
    groups = {"head": [], "mid": [], "tail": []}
    for addr, bs in pieces:
        key = "head" if addr < start else ("mid" if end is None or addr < end else "tail")
        groups[key].append(bs)
    new = {"mid": f"game/code_{start:08X}", "tail": f"game/code_{end:08X}" if end else None}

    # Prototypes for the unit's C functions: a function the other part defines
    # (and this one only calls or takes the address of) needs one once they
    # are separate files.
    protos = {}
    for _, bs in pieces:
        for b in bs:
            if "{" in b and not INCLUDE.search(b) and (m := DEF.search(b)):
                header = b[:b.index("{")].strip()
                if re.search(r"\)\s*$", header):  # prototype-style definition
                    protos[m.group(1)] = re.sub(r"\s+", " ", header) + ";"
                else:  # K&R: parameters declared between header and body
                    protos[m.group(1)] = re.sub(r"\(.*", "();", header.split("\n")[0])

    def write(unit, groups_blocks, old_unit):
        body = [b.replace(f'"{old_unit}"', f'"{unit}"') for bs in groups_blocks for b in bs]
        used = set(TOKENS.findall("\n".join(body)))
        defined = {m.group(1) for b in body if "{" in b and (m := DEF.search(b))}
        chosen, changed = set(), True
        while changed:
            changed = False
            for i, d in enumerate(head):
                if i not in chosen and declared_name(d) in used:
                    chosen.add(i)
                    used |= set(TOKENS.findall(d))
                    changed = True
        decls = [head[i] for i in sorted(chosen)]
        declared = {declared_name(d) for d in decls}
        decls += [protos[n] for n in sorted(used & set(protos)) if n not in defined and n not in declared]
        path = ROOT / "src" / args.version / f"{unit}.c"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text('#include "common.h"\n\n' + "\n\n".join(decls + body) + "\n")

    write(args.unit, groups["head"], args.unit)
    write(new["mid"], groups["mid"], args.unit)
    if new["tail"]:
        write(new["tail"], groups["tail"], args.unit)

    yaml = ROOT / "config" / args.version / f"{VERSIONS[args.version]['serial']}.yaml"
    text = yaml.read_text()
    row = re.search(rf"^(\s+- \[)0x([0-9A-F]+), c, {re.escape(args.unit)}\](.*)$", text, re.M)
    rom = lambda vram: vram - 0xFF000  # both ELFs load .text at file offset + 0xFF000
    add = f"\n{row.group(1)}0x{rom(start):X}, c, {new['mid']}]"
    if new["tail"]:
        add += f"\n{row.group(1)}0x{rom(end):X}, c, {new['tail']}]"
    yaml.write_text(text[:row.end()] + add + text[row.end():])
    print(f"{args.unit}: {len(groups['head'])} functions; {new['mid']}: {len(groups['mid'])}"
          + (f"; {new['tail']}: {len(groups['tail'])}" if new["tail"] else ""))


if __name__ == "__main__":
    main()

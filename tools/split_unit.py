#!/usr/bin/env python3
"""Split a C unit at function addresses, for a file boundary the unit hides.

    python3 tools/split_unit.py dds1 game/code_001F6110 0x001FC7D8 [0x001FF030]

Functions from the first address (up to the second, if given) move to a new
unit `game/code_<first>`; with an end address the rest after it becomes
`game/code_<end>`. Each new file gets the declarations its functions use, plus the
ones only an INCLUDE_ASM function of the part needs (its callees are named in the
retail disassembly, asm/<v>/<unit>.s) and every type the unit defines but names
nowhere else (used through a header's `struct X *` member; delete the copies a part
does not need). The config/<v>/*.yaml text rows are inserted; INCLUDE_RODATA lines
go to the part of the first function whose code names the data. Old-style (K&R)
definitions move whole, comment included.

The .rodata/.lit4/.sdata rows are NOT touched: give a part the data its code
references (`split_rodata.py <v> --section ...` computes it from the retail
code); a part's own lit4/sdata is the bytes its C emits (`objdump -h` of its
object), so put the boundary row where the previous part's emitted bytes end.
A part with no INCLUDE_ASM left has no INCLUDE_RODATA step: write each string it
uses as a literal in the C instead of `extern char D_X[]`.

Only split on evidence (a proven TU boundary, a change of compiler options);
record it in the commit and, for options, in config/<v>/cflags.txt. Check each
new unit with check_unit (CONTEXT lines mean the boundary is wrong), then
`configure.py --force-split` and the SHA-1.
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


def retail_references(version, unit):
    """Per function of the unit's retail disassembly (asm/<v>/<unit>.s): the identifiers
    it names. Empty if the unit has no full disassembly."""
    path = ROOT / "asm" / version / f"{unit}.s"
    if not path.exists():
        return {}
    return {m.group(1): set(TOKENS.findall(m.group(2)))
            for m in re.finditer(r"^glabel (\w+)\n(.*?)^endlabel \1", path.read_text(), re.M | re.S)}


def prototype(b):
    """Prototype of a function a part defines, for the parts after it."""
    header = b[:b.index("{")].strip()
    if re.search(r"\)\s*$", header):  # prototype-style definition
        return re.sub(r"\s+", " ", header) + ";"
    return re.sub(r"\(.*", "();", header.split("\n")[0])  # K&R


def split_source(text, names, start, end, unit, refs=None):
    """Cut the unit's C text at function addresses.

    Returns (parts, counts): parts maps "head" (the unit itself, below `start`), "mid"
    (from `start`) and, with `end`, "tail" to the new file text; counts is the number of
    functions in each. `refs` is retail_references(): which function names which data
    symbol, so an INCLUDE_RODATA line goes to the part of the first function that uses it
    and a declaration only an INCLUDE_ASM function needs stays with that function."""
    refs = refs or {}
    # Declarations before the first function form the unit's head. Later ones
    # travel with the function after them, like INCLUDE_RODATA lines: where a
    # declaration sits decides whether earlier calls saw it (an undeclared
    # callee is implicitly `int`, which changes codegen).
    head, pieces, pending, includes = [], [], [], []  # pieces: (address, [blocks], [declarations])
    for b in blocks(text):
        inc = INCLUDE.search(b)
        m = DEF.search(b) if "{" in b and not inc else None
        name = inc.group(3) if inc and inc.group(1) == "ASM" else (m.group(1) if m else None)
        if inc and inc.group(1) == "RODATA":
            pending.append(b)
        elif name and (name in names or re.fullmatch(r"\w*_[0-9A-Fa-f]{8}", name)):
            addr = names.get(name) or int(name[-8:], 16)
            pieces.append((addr, pending + [b], [d for d in pending if not INCLUDE.search(d)], name))
            pending = []
        elif b.startswith("#include"):
            # Every part keeps the unit's headers (fpu.h's fsqrtf would otherwise
            # become an undefined external call).
            includes += [line for line in b.split("\n") if line.startswith("#include") and line not in includes]
        elif pieces:
            # Addressless definitions (static inline helpers) are declarations:
            # every part that uses one gets its own copy.
            pending.append(b)
        else:
            head.append(b)
    if pending:
        pieces[-1][1].extend(pending)
    groups = {"head": [], "mid": [], "tail": []}
    for i, (addr, *_) in enumerate(pieces):
        key = "head" if addr < start else ("mid" if end is None or addr < end else "tail")
        groups[key].append(i)
    new = {"head": unit, "mid": f"game/code_{start:08X}", "tail": f"game/code_{end:08X}" if end else None}
    part_of = {i: key for key, indices in groups.items() for i in indices}

    # INCLUDE_RODATA lines moved with the next function, which need not be the part
    # that uses the data: re-home each one with the first function whose code names it.
    rodata_home = {}
    for i, (addr, body, _, _) in enumerate(pieces):
        for b in body:
            inc = INCLUDE.search(b)
            if inc and inc.group(1) == "RODATA":
                users = sorted((names.get(f) or int(f[-8:], 16), f) for f, syms in refs.items()
                               if inc.group(3) in syms and (f in names or re.fullmatch(r"\w*_[0-9A-Fa-f]{8}", f)))
                if users:
                    rodata_home[b] = "head" if users[0][0] < start else (
                        "mid" if end is None or users[0][0] < end else "tail")
    stay = {i: [b for b in body if rodata_home.get(b, part_of[i]) == part_of[i]] for i, (_, body, _, _) in enumerate(pieces)}
    moved = {key: [b for i, (_, body, _, _) in enumerate(pieces) for b in body
                   if rodata_home.get(b, part_of[i]) == key and part_of[i] != key] for key in groups}

    # A type defined in the unit and named nowhere else is used through a header's
    # `struct X *` member (`unit->ext->info->flags`): every part needs it complete.
    def is_type(d):
        return "{" in d and re.match(r"\s*(?:/\*.*?\*/\s*)*(?:typedef|struct|union|enum)\b", d, re.S) is not None

    unit_tokens = TOKENS.findall(text)
    orphan_types = [d for d in head + [d for p in pieces for d in p[2]]
                    if is_type(d) and declared_name(d)
                    and unit_tokens.count(declared_name(d)) == TOKENS.findall(d).count(declared_name(d))]

    def write(key):
        indices = groups[key]
        body = [b.replace(f'"{unit}"', f'"{new[key]}"') for i in indices for b in stay[i]]
        body += [b.replace(f'"{unit}"', f'"{new[key]}"') for b in moved[key]]
        first = indices[0] if indices else len(pieces)
        # Declarations the original placed before this part: the head, and the
        # ones travelling with earlier parts' functions.
        pool = head + [d for i in range(first) for d in pieces[i][2]]
        used = set(TOKENS.findall("\n".join(body)))
        # An INCLUDE_ASM function's callees are named only in its assembly.
        for i in indices:
            if any((inc := INCLUDE.search(b)) and inc.group(1) == "ASM" for b in pieces[i][1]):
                used |= refs.get(pieces[i][3], set())
        defined = {m.group(1) for b in body if "{" in b and (m := DEF.search(b))}
        chosen, changed = set(), True
        while changed:
            changed = False
            for i, d in enumerate(pool):
                if i not in chosen and (declared_name(d) in used or d in orphan_types):
                    chosen.add(i)
                    used |= set(TOKENS.findall(d))
                    changed = True
        decls = [pool[i] for i in sorted(chosen)]
        declared = {declared_name(d) for d in decls} | {declared_name(b) for b in body if "{" not in b}
        protos = {}
        for i in range(first):
            for b in pieces[i][1]:
                if "{" in b and not INCLUDE.search(b) and (m := DEF.search(b)):
                    protos[m.group(1)] = prototype(b)
        decls += [protos[n] for n in sorted(used & set(protos)) if n not in defined and n not in declared]
        return "\n".join(includes or ['#include "common.h"']) + "\n\n" + "\n\n".join(decls + body) + "\n"

    parts = {key: write(key) for key in groups if key != "tail" or new["tail"]}
    return parts, {key: len(groups[key]) for key in parts}, new


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
    parts, counts, new = split_source(src.read_text(), names, start, end, args.unit,
                                      retail_references(args.version, args.unit))
    for key, text in parts.items():
        path = ROOT / "src" / args.version / f"{new[key]}.c"
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text)

    yaml = ROOT / "config" / args.version / f"{VERSIONS[args.version]['serial']}.yaml"
    text = yaml.read_text()
    row = re.search(rf"^(\s+- \[)0x([0-9A-F]+), c, {re.escape(args.unit)}\](.*)$", text, re.M)
    rom = lambda vram: vram - 0xFF000  # both ELFs load .text at file offset + 0xFF000
    add = f"\n{row.group(1)}0x{rom(start):X}, c, {new['mid']}]"
    if new["tail"]:
        add += f"\n{row.group(1)}0x{rom(end):X}, c, {new['tail']}]"
    yaml.write_text(text[:row.end()] + add + text[row.end():])
    print(f"{args.unit}: {counts['head']} functions; {new['mid']}: {counts['mid']}"
          + (f"; {new['tail']}: {counts['tail']}" if new["tail"] else ""))


if __name__ == "__main__":
    main()

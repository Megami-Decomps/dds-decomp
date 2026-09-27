#!/usr/bin/env python3
"""Project real source-file names from the Nocturne Dec 26 2002 debug build onto DDS.

The Nocturne prototype (SLPM_652.42, SHA-1 69d03ff7...) keeps __FILE__ strings
("../effect/src/effPCPMisc.c", "sdfModel.c", ...). Each function referencing one
of them is labelled; every Nocturne function between a file's first and last
labelled function inherits the label. Labels move to DDS only through pairs whose
code is byte-identical after relocation masking, so the output is conservative.

Inputs: romwright stores for DDS (build/romwright) and Nocturne
(build/romwright-noct), plus `romwright-cli diff <v> --reference build/romwright-noct
--json` in build/noct_diff_<v>.json.
With --us, US Nocturne (SLUS_209.11, store build/romwright-noctus) is a second hop:
prototype labels reach it through identical pairs (build/noctus_diff_noct.json), are
re-spanned there, then reach DDS through build/noctus_diff_<v>.json. Prototype-direct
labels win on conflict.
Output: build/<v>/nocturne_tus.txt
"""
import argparse
import bisect
import collections
import json
import re
import struct
from pathlib import Path

from pairing import RETAIL, ROOT, Build, identical, load_segments, off_to_va, va_to_off


def file_refs(elf, segs, text_va, text_size):
    """Map function-independent code addresses to __FILE__ strings via lui/addiu pairs."""
    strings = {}
    for m in re.finditer(rb"[A-Za-z0-9_./]*[A-Za-z0-9_]+\.(?:c|cpp)\x00", elf):
        va = off_to_va(segs, m.start())
        if va is not None and va >= text_va + text_size:
            strings[va] = m.group()[:-1].decode()
    text_off = va_to_off(segs, text_va)
    refs = []
    hi = {}
    for i in range(0, text_size, 4):
        w = struct.unpack_from("<I", elf, text_off + i)[0]
        op, rs, rt, imm = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 0xFFFF
        if op == 0x0F:
            hi[rt] = (imm << 16, i)
        elif op in (0x09, 0x19) and rs in hi and i - hi[rs][1] < 256:
            addr = (hi[rs][0] + (imm - 0x10000 if imm & 0x8000 else imm)) & 0xFFFFFFFF
            if addr in strings:
                refs.append((text_va + i, strings[addr]))
    return refs


def text_section(elf):
    shoff = struct.unpack_from("<I", elf, 0x20)[0]
    shnum, shstrndx = struct.unpack_from("<HH", elf, 0x30)
    sh = [struct.unpack_from("<10I", elf, shoff + i * 40) for i in range(shnum)]
    names = sh[shstrndx][4]
    for s in sh:
        if elf[names + s[0]:elf.index(b"\0", names + s[0])] == b".text":
            return s[3], s[5]
    raise SystemExit("no .text in Nocturne ELF")


def transfer(src, labels, dst, diff_path, src_is_target):
    """Carry labels from src to dst through byte-identical romwright pairs."""
    out = {}
    for m in json.loads(Path(diff_path).read_text())["matches"]:
        t, r = m["target"]["offset"], m["reference"]["offset"]
        s, d = (t, r) if src_is_target else (r, t)
        if s in labels and identical(src, s, dst, d):
            out[d] = labels[s]
    return out


def runs_of(labels):
    runs = []
    for t in sorted(labels):
        name = labels[t]
        if runs and runs[-1][0] == name:
            runs[-1][2] = t
            runs[-1][3] += 1
        else:
            runs.append([name, t, t, 1])
    best = {}
    for run in runs:
        if run[0] not in best or run[3] > best[run[0]][3]:
            best[run[0]] = run
    return best, [r for r in runs if best[r[0]] is not r]


def fill_spans(build, spans):
    """Label every function between a file's first and last anchor, skipping overlapping spans."""
    order = sorted(spans.items(), key=lambda kv: kv[1][0])
    bad = set()
    for (n1, (_, hi1)), (n2, (lo2, _)) in zip(order, order[1:]):
        if lo2 <= hi1:
            bad |= {n1, n2}
    label = {}
    for name, (lo, hi) in spans.items():
        if name in bad:
            continue
        for func in build.entries[bisect.bisect_left(build.entries, lo):bisect.bisect_right(build.entries, hi)]:
            label[func] = name
    return label, bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("version", choices=sorted(RETAIL))
    ap.add_argument("--nocturne", required=True, help="SLPM_652.42 from the Dec 26 2002 build")
    ap.add_argument("--noct-store", default=ROOT / "build/romwright-noct/project.sqlite", type=Path)
    ap.add_argument("--us", help="optional US Nocturne SLUS_209.11 used as a second hop")
    ap.add_argument("--us-store", default=ROOT / "build/romwright-noctus/project.sqlite", type=Path)
    ap.add_argument("--store", default=ROOT / "build/romwright/project.sqlite", type=Path)
    args = ap.parse_args()

    noct = Build(args.nocturne, args.noct_store, "noct")
    text_va, text_size = text_section(noct.data)

    def owner(addr):
        i = bisect.bisect_right(noct.entries, addr) - 1
        e = noct.entries[i] if i >= 0 else None
        return e if e is not None and addr < e + max(noct.sizes[e], 4) else None

    direct = collections.defaultdict(set)
    for addr, name in file_refs(noct.data, noct.segs, text_va, text_size):
        func = owner(addr)
        if func is not None:
            direct[func].add(name)
    spans = {}
    for func, names in direct.items():
        if len(names) == 1:
            name = next(iter(names))
            lo, hi = spans.get(name, (func, func))
            spans[name] = (min(lo, func), max(hi, func))
    label, _ = fill_spans(noct, spans)

    dds = Build(ROOT / RETAIL[args.version], args.store, args.version)
    labelled = {t: (name, "proto") for t, name in
                transfer(noct, label, dds, ROOT / f"build/noct_diff_{args.version}.json", False).items()}
    via_note = ""
    if args.us:
        us = Build(args.us, args.us_store, "noctus")
        us_direct = transfer(noct, label, us, ROOT / "build/noctus_diff_noct.json", True)
        best, _ = runs_of(us_direct)
        us_label, dropped = fill_spans(us, {n: (r[1], r[2]) for n, r in best.items()})
        added = 0
        for t, name in transfer(us, us_label, dds, ROOT / f"build/noctus_diff_{args.version}.json", False).items():
            if t not in labelled:
                labelled[t] = (name, "us")
                added += 1
        via_note = f"; {added} added via US Nocturne ({len(dropped)} overlapping US spans skipped)"

    best, strays = runs_of({t: name for t, (name, _) in labelled.items()})

    out = ROOT / f"build/{args.version}/nocturne_tus.txt"
    out.parent.mkdir(parents=True, exist_ok=True)
    with out.open("w") as f:
        f.write(f"# Nocturne Dec 26 2002 __FILE__ names projected onto {args.version}\n")
        f.write(f"# {len(labelled)} functions labelled through byte-identical pairs{via_note}; "
                f"{len(best)} files; {len(strays)} stray runs listed last\n")
        f.write("# first_func  last_func  funcs  file  (DDS range is a lower bound on the file)\n")
        for name, lo, hi, n in sorted(best.values(), key=lambda r: r[1]):
            f.write(f"0x{lo:06X}  0x{hi:06X}  {n:4d}  {name}\n")
        f.write("\n# stray runs (probably duplicated helpers or mispairs)\n")
        for name, lo, hi, n in strays:
            f.write(f"0x{lo:06X}  0x{hi:06X}  {n:4d}  {name}\n")
        f.write("\n# per-function labels: dds_addr  source  file\n")
        for t in sorted(labelled):
            f.write(f"0x{t:06X}  {labelled[t][1]:5s}  {labelled[t][0]}\n")
    print(f"{args.version}: {len(labelled)} functions{via_note}, {len(best)} files, "
          f"{len(strays)} stray runs -> {out.relative_to(ROOT)}")


if __name__ == "__main__":
    main()

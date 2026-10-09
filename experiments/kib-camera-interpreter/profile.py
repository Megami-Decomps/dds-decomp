#!/usr/bin/env python3
"""Locate camera code-shape differences without publishing executable inputs.

Call-order equality is required before dividing this large body at its calls.
Coarse encoding alignment is diagnostic only; it is never an exact-match test.
"""
from collections import Counter
from difflib import SequenceMatcher
import struct
import sys
import diagnose as d

sys.path.insert(0, str(d.ROOT / "tools"))
import check_unit as c

def shape(word):
    op = word >> 26
    if op == 0:
        return (op, word & 63)
    if op in (16, 17, 18):
        rs = (word >> 21) & 31
        if rs == 8:
            return (op, rs, (word >> 16) & 31)
        return (op, rs, word & 63)
    if op == 1:
        return (op, (word >> 16) & 31)
    if op == 28:
        return (op, word & 63, (word >> 6) & 31)
    return (op,)

def profile():
    syms = c.symbols("dds2")
    text = c.text_section(d.OBJECT)
    relocs = c.relocations(d.OBJECT)[".text"]
    symbols = d.run("profile_symbols", ["tools/bin/mips-ps2-decompals-nm",
                   "-S", "--defined-only", str(d.OBJECT)]).stdout
    functions = []
    for line in symbols.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in ("T", "t"):
            functions.append((int(p[0], 16), int(p[1], 16), p[3]))
    found = [row for row in functions if row[2] == d.TARGET]
    d.require(len(found) == 1, "profile_target_count")
    start, size, _ = found[0]
    mine = list(struct.unpack("<%dI" % (size // 4), text[start:start + size]))
    retail = (d.ROOT / c.RETAIL["dds2"]).read_bytes()
    addr = c.address(d.TARGET, syms)
    at = c.va_to_off(c.load_segments(retail), addr)
    want = list(struct.unpack("<1314I", retail[at:at + 5256]))

    candidate_calls, native_calls = [], []
    for index, word in enumerate(mine):
        if word >> 26 != 3:
            continue
        rel = relocs.get(start + index * 4)
        d.require(rel is not None and rel[0] == "R_MIPS_26", "profile_call_relocation")
        base = rel[1].split("+")[0]
        addend = (word & 0x3FFFFFF) * 4
        if base == ".text":
            owners = [r for r in functions if r[0] <= addend < r[0] + r[1]]
            d.require(len(owners) == 1, "profile_call_owner")
            offset, _, name = owners[0]
            dest = c.address(name, syms)
            d.require(dest is not None, "profile_call_address")
            dest += addend - offset
        else:
            dest = c.address(base, syms)
            d.require(dest is not None, "profile_external_call")
            dest += addend
        candidate_calls.append((index, dest))
    for index, word in enumerate(want):
        if word >> 26 == 3:
            native_calls.append((index, ((addr + index * 4 + 4) & 0xF0000000)
                                 | ((word & 0x3FFFFFF) * 4)))
    d.require([v for _, v in candidate_calls] == [v for _, v in native_calls],
              "profile_call_order_changed")
    # Include each call's delay slot in the preceding interval.
    ca = [0] + [i + 2 for i, _ in candidate_calls] + [len(mine)]
    na = [0] + [i + 2 for i, _ in native_calls] + [len(want)]
    intervals = []
    for ordinal, (a, b, x, y) in enumerate(zip(ca, ca[1:], na, na[1:])):
        ms, ns = [shape(w) for w in mine[a:b]], [shape(w) for w in want[x:y]]
        edits = [dict(kind=t, candidate_words=j-i, native_words=l-k)
                 for t, i, j, k, l in SequenceMatcher(None, ms, ns,
                                                     autojunk=False).get_opcodes()
                 if t != "equal"]
        intervals.append(dict(interval=ordinal, candidate_start=a*4, native_start=x*4,
            candidate_words=b-a, native_words=y-x, coarse_shape_equal=ms == ns,
            shape_edits=edits))
    d.emit(dict(scope="camera_call_partition_profile", exact_match_proof=False,
        call_order_equal=True, static_calls=len(candidate_calls),
        candidate_bytes=size, native_bytes=5256, intervals=intervals))

def main():
    original = d.UNIT.read_bytes()
    try:
        d.run("configure", [sys.executable, "configure.py", "dds2"])
        d.require(d.UNIT.read_bytes() == original, "configure_changed_source")
        source = original.decode()
        patch = d.Path(__file__).with_name("candidate.patch").read_text()
        d.UNIT.write_text(d.apply_source_patch(source, patch), encoding="utf-8")
        sizes = d.object_sizes("profile")
        d.require(sizes[d.TARGET] == 5248, "profile_size_changed")
        profile()
    finally:
        d.UNIT.write_bytes(original)
    d.require(d.UNIT.read_bytes() == original, "restore_failed")
    d.emit(dict(source_restored=True))

if __name__ == "__main__":
    try:
        main()
    except d.Failure as exc:
        d.emit(dict(status="diagnostic_failed", stage=exc.stage, returncode=exc.returncode))
        raise SystemExit(2)
    except BaseException:
        d.emit(dict(status="diagnostic_failed", stage=d.STAGE))
        raise SystemExit(2)

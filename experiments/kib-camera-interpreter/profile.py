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

def peer_interval_profile(mine, want, candidate_calls, native_calls, syms):
    """Static physical interval only; no native bytes, registers or PCs emitted."""
    d.require(len(candidate_calls) == len(native_calls) == 52,
              "peer_profile_call_count")
    d.require([v for _, v in candidate_calls] ==
              [v for _, v in native_calls], "peer_profile_call_order")
    runtime = c.address("btlGetRuntime", syms)
    clear = c.address("btlClearUnitDefeatCandidate", syms)
    d.require(runtime is not None and clear is not None,
              "peer_profile_symbols")
    ordinals = [j for j, (_, dst) in enumerate(native_calls) if dst == runtime]
    d.require(len(ordinals) == 2, "peer_profile_runtime_count")
    ordinal = ordinals[1]
    d.require(ordinal + 1 < 52 and native_calls[ordinal + 1][1] == clear,
              "peer_profile_following_clear")

    offsets = {0x114: "0x114", 0x18: "0x18",
               0x11C: "0x11C", 0x110: "0x110"}
    # These are actual scalar/quad load encodings, not inferred C types.
    loads = {32: (1, "sign"), 33: (2, "sign"), 35: (4, "sign"),
             36: (1, "zero"), 37: (2, "zero"), 39: (4, "zero"),
             55: (8, "full"), 30: (16, "full")}

    def control(w):
        op, rs, rt, fn = w >> 26, (w >> 21) & 31, (w >> 16) & 31, w & 63
        if op in (2, 3):
            return "jump_direct" if op == 2 else "call_direct"
        if op == 0 and fn in (8, 9):
            return "jump_register" if fn == 8 else "call_register"
        if op == 1:
            return {0: "branch_negative", 1: "branch_nonnegative",
                    2: "branch_negative_likely", 3: "branch_nonnegative_likely",
                    16: "branch_negative_link", 17: "branch_nonnegative_link",
                    18: "branch_negative_link_likely",
                    19: "branch_nonnegative_link_likely"}.get(rt, "regimm_other")
        if op in (4, 5, 20, 21):
            name = "branch_eq" if op in (4, 20) else "branch_ne"
            name += "_zero" if rs == 0 or rt == 0 else "_pair"
            if op in (20, 21):
                name += "_likely"
            return name
        if op in (6, 7, 22, 23):
            return {6: "branch_le_zero", 7: "branch_gt_zero",
                    22: "branch_le_zero_likely",
                    23: "branch_gt_zero_likely"}[op]
        if op in (16, 17, 18) and rs == 8:
            return "cop_branch_%d_%d" % (op, rt)
        return None

    def inspect(words, calls):
        a = calls[ordinal][0] + 2
        b = calls[ordinal + 1][0] + 2
        d.require(0 <= a < b <= len(words), "peer_profile_interval")
        # A known incoming direct relative branch invalidates a linear proof.
        incoming = set()
        for i, w in enumerate(words):
            op, rs = w >> 26, (w >> 21) & 31
            if (op in (1, 4, 5, 6, 7, 20, 21, 22, 23)
                    or (op in (16, 17, 18) and rs == 8)):
                imm = w & 0xFFFF
                if imm & 0x8000:
                    imm -= 0x10000
                incoming.add(i + 1 + imm)

        provenance = {}
        reset_after = set()
        selected, flow, control_classes = [], [], []
        counts = Counter()
        all_loads = Counter()
        for i in range(a, b):
            w = words[i]
            op, rs, rt, rd = w >> 26, (w >> 21) & 31, (w >> 16) & 31, (w >> 11) & 31
            fn, sa, imm = w & 63, (w >> 6) & 31, w & 0xFFFF
            if i in incoming or i in reset_after:
                provenance.clear()
            cls = control(w)
            if cls is not None:
                # Do not carry a proof through either kind of delay slot.
                provenance.clear()
                reset_after.add(i + 2)
                control_classes.append(cls)
                flow.append({"event": "control", "class": cls})
            elif op == 0 and fn in (42, 43):
                cls = "compare_signed_lt" if fn == 42 else "compare_unsigned_lt"
                control_classes.append(cls)
                flow.append({"event": "compare", "class": cls})
            elif op in (10, 11):
                cls = "compare_signed_lt_immediate" if op == 10 else "compare_unsigned_lt_immediate"
                control_classes.append(cls)
                flow.append({"event": "compare", "class": cls})

            if op in loads:
                width, extension = loads[op]
                all_loads["%d:%s" % (width, extension)] += 1
                if imm in offsets:
                    n = len(selected)
                    row = {"read": n, "offset": offsets[imm], "bytes": width,
                           "extension": extension,
                           "straight_line_base_from_selected_read": provenance.get(rs)}
                    selected.append(row)
                    flow.append(dict(event="read", **row))
                    counts["%s:%d:%s" % (offsets[imm], width, extension)] += 1
                    if rt:
                        provenance[rt] = n
                else:
                    provenance.pop(rt, None)
                continue
            # No inference across an unaligned memory operation.
            if op in (34, 38, 26, 27):
                provenance.clear()
                continue
            # Preserve a local loaded-pointer chain through genuine zero moves.
            if op == 0 and fn in (33, 37, 45) and (rs == 0 or rt == 0):
                src = rt if rs == 0 else rs
                value = provenance.get(src)
                provenance.pop(rd, None)
                if rd and value is not None:
                    provenance[rd] = value
            elif op == 28 and fn == 0x29 and sa == 0x12 and (rs == 0 or rt == 0):
                src = rt if rs == 0 else rs
                value = provenance.get(src)
                provenance.pop(rd, None)
                if rd and value is not None:
                    provenance[rd] = value
            elif op in (9, 25) and imm == 0:
                value = provenance.get(rs)
                provenance.pop(rt, None)
                if rt and value is not None:
                    provenance[rt] = value
            elif op == 0:
                if fn not in (8, 12, 13, 17, 19, 24, 25, 26, 27,
                              28, 29, 30, 31, 48, 49, 50, 51, 52, 54):
                    provenance.pop(rd, None)
            elif op in (8, 9, 10, 11, 12, 13, 14, 15, 24, 25):
                provenance.pop(rt, None)
            elif op in (16, 17, 18) and rs in (0, 1, 2):
                provenance.pop(rt, None)
            elif op in (40, 41, 43, 63, 31, 49, 54, 57, 62):
                pass  # Stores and non-GPR loads cannot define a pointer GPR.
            else:
                # Unknown operations lose proof rather than guessing a write set.
                provenance.clear()
        return {"words": b - a, "selected_read_counts": dict(sorted(counts.items())),
                "all_known_gpr_load_counts": dict(sorted(all_loads.items())),
                "selected_reads": selected, "read_control_order": flow,
                "control_compare_classes": control_classes,
                "shapes": [shape(w) for w in words[a:b]]}

    def selftests():
        def iw(op, rs=0, rt=0, imm=0):
            return (op << 26) | (rs << 21) | (rt << 16) | (imm & 0xFFFF)
        def rw(fn, rs=0, rt=0, rd=0):
            return (rs << 21) | (rt << 16) | (rd << 11) | fn
        def sample(body, tail=()):
            words = [iw(3), 0] + list(body) + [iw(3), 0] + list(tail)
            calls = list(candidate_calls)
            calls[ordinal] = (0, runtime)
            calls[ordinal + 1] = (2 + len(body), clear)
            return inspect(words, calls)

        # Pointer def-use through an ordinary zero move; retain separate LD/LW.
        row = sample([iw(35, 16, 8, 0x114), rw(33, 8, 0, 9),
                      iw(35, 9, 10, 0x18), iw(36, 10, 11, 0x11C),
                      iw(55, 10, 12, 0x110), iw(35, 10, 13, 0x110),
                      rw(43, 11, 13, 14)])
        reads = row["selected_reads"]
        d.require([x["straight_line_base_from_selected_read"] for x in reads]
                  == [None, 0, 1, 1, 1], "peer_selftest_base_chain")
        d.require([x["bytes"] for x in reads] == [4, 4, 1, 8, 4],
                  "peer_selftest_widths")
        d.require(reads[2]["extension"] == "zero"
                  and row["selected_read_counts"]["0x110:8:full"] == 1
                  and row["selected_read_counts"]["0x110:4:sign"] == 1,
                  "peer_selftest_load_classes")
        d.require("compare_unsigned_lt" in row["control_compare_classes"],
                  "peer_selftest_unsigned_compare")

        # An overwritten base, unknown operation or control edge loses proof.
        for middle in ([iw(9, 0, 8, 7)], [iw(19)],
                       [iw(5, 4, 0, 1), 0]):
            row = sample([iw(35, 16, 8, 0x114)] + middle +
                         [iw(35, 8, 10, 0x18)])
            d.require(row["selected_reads"][-1]
                      ["straight_line_base_from_selected_read"] is None,
                      "peer_selftest_provenance_kill")

        # A branch-likely delay-slot definition cannot justify a fallthrough use.
        row = sample([iw(35, 16, 8, 0x114), iw(20, 4, 0, 1),
                      iw(35, 8, 9, 0x18), iw(36, 9, 10, 0x11C)])
        d.require([x["straight_line_base_from_selected_read"]
                   for x in row["selected_reads"]] == [None, None, None],
                  "peer_selftest_likely_delay")
        d.require("branch_eq_zero_likely" in row["control_compare_classes"],
                  "peer_selftest_likely_class")

        # An incoming relative edge from outside the slice breaks a local chain.
        row = sample([iw(35, 16, 8, 0x114), iw(35, 8, 9, 0x18)],
                     tail=[iw(5, 4, 0, -4), 0])
        d.require(row["selected_reads"][1]
                  ["straight_line_base_from_selected_read"] is None,
                  "peer_selftest_incoming_edge")
        d.require(control(iw(17, 8, 3, 1)) == "cop_branch_17_3",
                  "peer_selftest_cop_branch")

    selftests()

    candidate, native = inspect(mine, candidate_calls), inspect(want, native_calls)
    keys = set(candidate["selected_read_counts"]) | set(native["selected_read_counts"])
    delta = {key: candidate["selected_read_counts"].get(key, 0) -
                  native["selected_read_counts"].get(key, 0)
             for key in sorted(keys)}
    delta = {key: count for key, count in delta.items() if count}
    shape_equal = candidate.pop("shapes") == native.pop("shapes")
    d.emit({"scope": "camera_second_peer_call_interval",
            "call_order_equal": True, "static_calls": 52, "selftests": "passed",
            "bounds": "after second runtime call delay slot through following clear call delay slot",
            "candidate": candidate, "native": native,
            "candidate_minus_native_selected_read_counts": delta,
            "selected_read_counts_equal": not delta,
            "selected_read_sequence_equal": candidate["selected_reads"] == native["selected_reads"],
            "read_control_order_equal": candidate["read_control_order"] == native["read_control_order"],
            "control_compare_classes_equal": candidate["control_compare_classes"] == native["control_compare_classes"],
            "coarse_shape_equal": shape_equal, "exact_match_proof": False,
            "limits": "Physical static counts, not dynamic reload counts. Offsets do not establish owners. Base links describe uninterrupted local register def-use only, conditional on entry at that straight-line block; they are not owner or full-CFG dominance proofs. Control transfers and known incoming branch targets erase that proof. Equal summaries do not prove register/layout-only differences."})

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
    peer_interval_profile(mine, want, candidate_calls, native_calls, syms)
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

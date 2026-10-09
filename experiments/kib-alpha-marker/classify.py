#!/usr/bin/env python3
"""Publish only aggregate mismatch counts; all comparison output stays private."""
from collections import Counter
import hashlib
import re
import sys
from pathlib import Path
import diagnose as d
require = d.require
TARGET = d.TARGET
HEX = d.HEX

def register_fields(word):
    """Conservative R5900 register fields; unsupported encodings stay unknown."""
    gs, gt, gd = ("gpr", 21), ("gpr", 16), ("gpr", 11)
    ft, fs, fd = ("fpr", 16), ("fpr", 11), ("fpr", 6)
    op, rs = word >> 26, (word >> 21) & 31
    rt, sa, fn = (word >> 16) & 31, (word >> 6) & 31, word & 63
    if op == 0:
        if fn in (0, 2, 3, 56, 58, 59, 60, 62, 63) and rs == 0:
            return (gt, gd)
        if fn in (4, 6, 7, 20, 22, 23, 10, 11,
                  32, 33, 34, 35, 36, 37, 38, 39,
                  42, 43, 44, 45, 46, 47) and sa == 0:
            return (gs, gt, gd)
    if op in (8, 9, 10, 11, 12, 13, 14, 24, 25,
              32, 33, 35, 36, 37, 39, 40, 41, 43, 55, 63):
        return (gs, gt)
    if op == 15 and rs == 0:
        return (gt,)
    if op in (49, 57):
        return (gs, ft)
    if op == 17:
        if rs in (0, 4) and word & 0x7FF == 0:
            return (gt, fs)
        if rs == 16 and fn in (0, 1, 2, 3):
            return (ft, fs, fd)
        if ((rs == 16 and fn in (5, 6, 7, 36)) or
                (rs == 20 and fn == 32)) and rt == 0:
            return (fs, fd)
    return None

def mismatch_class(mine, retail, noted):
    fields = register_fields(mine)
    other = register_fields(retail)
    if noted or fields is None or other is None:
        return "unknown", ()
    mask = sum(31 << shift for _, shift in fields)
    if fields != other or ((mine ^ retail) & ~mask):
        return "non_register_bits_differ", ()
    substitutions = tuple(
        (bank, (mine >> shift) & 31, (retail >> shift) & 31)
        for bank, shift in fields
        if ((mine >> shift) & 31) != ((retail >> shift) & 31)
    )
    banks = {bank for bank, _, _ in substitutions}
    kind = ("mixed_register_fields" if len(banks) == 2 else
            "gpr_field_only" if banks == {"gpr"} else "fpr_field_only")
    return kind, substitutions

def target_mismatch_aggregate(output):
    """Emit only aggregate metadata, never words, notes or instruction lines."""
    header = re.compile(
        rf"DIFF {re.escape(TARGET)} @ 0x{HEX}: "
        rf"(\d+) of (\d+) words differ \(first at \+0x{HEX}\)"
    )
    detail = re.compile(
        r"       \+0x([0-9A-F]+) mine ([0-9A-F]{8}) "
        r"retail ([0-9A-F]{8}) (.*)"
    )
    lines = output.splitlines()
    hits = [(i, header.fullmatch(line)) for i, line in enumerate(lines)]
    hits = [(i, match) for i, match in hits if match]
    require(len(hits) == 1, "target_diff_header_count")
    index, match = hits[0]
    expected, byte_count = int(match[1]), 4 * int(match[2])
    require((expected, byte_count) == (20, 912), "target_diff_shape_changed")
    seen, groups = set(), {}
    for line in lines[index + 1:]:
        if not line.startswith("       +0x"):
            break
        match = detail.fullmatch(line)
        require(match is not None, "target_diff_line_malformed")
        offset, mine, retail = (int(match[i], 16) for i in (1, 2, 3))
        require(offset not in seen and offset % 4 == 0
                and 0 <= offset < byte_count, "target_diff_offset_invalid")
        seen.add(offset)
        noted = bool(match[4].strip())
        require(noted or mine != retail, "target_diff_equal_words")
        kind, substitutions = mismatch_class(mine, retail, noted)
        group = groups.setdefault(kind, dict(
            words=0, min_offset=offset, max_offset=offset, histogram=Counter()
        ))
        group["words"] += 1
        group["min_offset"] = min(group["min_offset"], offset)
        group["max_offset"] = max(group["max_offset"], offset)
        group["histogram"].update(substitutions)
    require(len(seen) == expected, "target_diff_detail_count")
    require(sum(group["words"] for group in groups.values()) == expected,
            "target_diff_class_count")
    for group in groups.values():
        histogram = group.pop("histogram")
        group["register_substitutions"] = [
            dict(bank=bank, candidate=source, retail=destination, count=count)
            for (bank, source, destination), count in sorted(histogram.items())
        ]
    return dict(parsed_differing_words=expected,
                histogram_unit="changed_operand_fields", classes=groups)


def main():
    original = d.UNIT.read_bytes()
    blob = hashlib.sha1(b"blob " + str(len(original)).encode() + b"\0" + original).hexdigest()
    require(blob == d.SOURCE_BLOB, "source_changed")
    source = original.decode()
    require(source.count(d.ANCHOR) == 1, "anchor_count")
    candidate = Path(__file__).with_name("candidate.inc").read_text().strip()
    require(bool(candidate) and d.ANCHOR not in candidate, "candidate_input")
    try:
        d.run("configure", [sys.executable, "configure.py", "dds2"])
        require(d.UNIT.read_bytes() == original, "configure_changed_source")
        baseline_sizes = d.object_sizes("baseline")
        baseline = d.check("baseline", baseline_sizes)
        require(baseline["issues"] == 0, "baseline_not_clean")
        d.UNIT.write_text(source.replace(d.ANCHOR, candidate, 1))
        sizes = d.object_sizes("candidate")
        result = d.check("candidate", sizes)
        verbose = d.run("classified_check", [sys.executable, "tools/check_unit.py",
                        str(d.UNIT.relative_to(d.ROOT)), "-v"], allowed=(0, 1))
        target = result["functions"].get(TARGET)
        require(target is not None and target["differing_words"] == 20 and
                sizes[TARGET] == baseline_sizes[TARGET] == 912 and
                not target["context_differs"], "target_changed")
        regressions = 0
        for name, before in baseline["functions"].items():
            after = result["functions"].get(name)
            if (after is None or after["status"] != "ok" or after["context_differs"]
                    or after["full_object_bytes"] != before["full_object_bytes"]):
                regressions += 1
        d.emit(dict(scope="focused_unit_classification", target_bytes=sizes[TARGET],
               older_c_functions=len(baseline["functions"]), older_regressions=regressions,
               data_issues=result["data_issues"], context_issues=result["context_issues"],
               other_issues=result["other_issues"],
               mismatch_aggregate=target_mismatch_aggregate(verbose.stdout),
               full_retail_build_verified=False))
    finally:
        d.UNIT.write_bytes(original)
    require(d.UNIT.read_bytes() == original, "restore_failed")
    d.emit(dict(source_restored=True))
    return 1

if __name__ == "__main__":
    try:
        code = main()
    except d.Failure as exc:
        d.emit(dict(status="diagnostic_failed", stage=exc.stage, returncode=exc.returncode))
        code = 2
    except BaseException:
        d.emit(dict(status="diagnostic_failed", stage=d.STAGE))
        code = 2
    raise SystemExit(code)

#!/usr/bin/env python3
"""Candidate-only allocation metadata; no compiler or retail dumps are printed."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import diagnose as d


sys.path.insert(0, str(d.ROOT / "tools"))
import check_unit as cu
from ee_gcc_delay_slots import NODE_HEADER, top_level_forms, balanced_form
from collections import defaultdict
import re

def target_identity(obj):
    listing = cu.run(str(cu.BIN / "mips-ps2-decompals-nm"), "-S", "--defined-only", str(obj))
    found = []
    for line in listing.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[2] in ("T", "t") and parts[3] == d.TARGET:
            found.append((int(parts[0], 16), int(parts[1], 16)))
    d.require(len(found) == 1, "parity_target_symbol")
    start, size = found[0]
    code = cu.text_section(obj)
    d.require(size > 0 and size % 4 == 0 and start + size <= len(code),
              "parity_target_bounds")
    rels = {offset - start: value for offset, value in cu.relocations(obj)[".text"].items()
            if start <= offset < start + size}
    return code[start:start + size], rels

REG = re.compile(r"\(reg(?:/[A-Za-z]+)?(?::[^\s()]+)?\s+(\d+)(?=[\s)])")
INT = re.compile(r"\(const_int\s+(-?\d+)(?=[\s)])")

def reg(expr):
    match = REG.match(expr)
    return int(match[1]) if match else None

def op(expr):
    match = re.match(r"\(([A-Za-z_]+)", expr)
    return match[1] if match else None

def children(expr):
    pos = expr.find(" ")
    return top_level_forms(expr[pos + 1:-1]) if pos >= 0 else []

def alpha_anchors(path):
    text = path.read_text()
    d.require(re.findall(r"(?m)^;; Function (\S+)", text) == [d.TARGET], "rtl_scope")
    rows, first_call = [], None
    for position, form in enumerate(top_level_forms(text)):
        node = NODE_HEADER.match(form)
        if not node:
            continue
        if node[1] == "call_insn" and first_call is None:
            first_call = position
        for match in re.finditer(r"\(set\s+", form):
            statement, _ = balanced_form(form, match.start())
            parts = children(statement)
            if len(parts) == 2 and reg(parts[0]) is not None:
                rows.append(dict(position=position, dst=reg(parts[0]), rhs=parts[1]))
    definitions = defaultdict(list)
    for row in rows:
        definitions[row["dst"]].append(row)
    def literal(expr, seen=frozenset()):
        match = INT.match(expr)
        if match:
            return int(match[1])
        number = reg(expr)
        if number is None or number in seen:
            return None
        defs = definitions[number]
        return literal(defs[0]["rhs"], seen | {number}) if len(defs) == 1 else None
    def copy_base(number):
        seen = set()
        while number is not None and number not in seen:
            seen.add(number)
            defs = definitions[number]
            if len(defs) != 1:
                break
            source = reg(defs[0]["rhs"])
            if source is None:
                break
            number = source
        return number
    incoming = {row["dst"] for row in rows
                if row["dst"] >= 79 and reg(row["rhs"]) == 5
                and first_call is not None and row["position"] < first_call}
    alpha = next(iter(incoming)) if len(incoming) == 1 else None
    colors = []
    for row in rows:
        if op(row["rhs"]) != "ior":
            continue
        operands = children(row["rhs"])
        if len(operands) == 2:
            for index in (0, 1):
                if literal(operands[index]) == 0x808080:
                    other = reg(operands[1-index])
                    if other is not None:
                        colors.append(other)
    color = colors[0] if len(colors) == 1 else None
    packed = []
    for row in rows:
        if color is None or row["dst"] != color or op(row["rhs"]) != "ashift":
            continue
        operands = children(row["rhs"])
        if len(operands) == 2 and literal(operands[1]) == 24:
            packed.append(reg(operands[0]))
    pulse = None
    if alpha is not None and len(packed) == 2 and None not in packed:
        direct = [number for number in packed if copy_base(number) == 5]
        other = [number for number in packed if copy_base(number) != 5]
        if len(direct) == len(other) == 1:
            pulse = other[0]
    return dict(alpha_entry=alpha, color_before_rgb_or=color,
                pulseAlpha_before_pack=pulse,
                complete=None not in (alpha, color, pulse))

def main():
    original = d.UNIT.read_bytes()
    blob = hashlib.sha1(b"blob " + str(len(original)).encode() + b"\0" + original).hexdigest()
    d.require(blob == d.SOURCE_BLOB, "source_changed")
    source = original.decode("utf-8")
    d.require(source.count(d.ANCHOR) == 1, "anchor_count")
    candidate = Path(__file__).with_name("candidate.inc").read_text().strip()
    d.require(bool(candidate) and d.ANCHOR not in candidate, "candidate_input")
    try:
        d.run("configure", [sys.executable, "configure.py", "dds2"])
        d.require(d.UNIT.read_bytes() == original, "configure_changed_source")
        d.UNIT.write_text(source.replace(d.ANCHOR, candidate, 1))
        sizes = d.object_sizes("candidate_prepare")
        d.require(sizes.get(d.TARGET) == 912, "candidate_size_changed")
        with tempfile.TemporaryDirectory(prefix="dds-alpha-trace-") as temp:
            private = Path(temp)
            probe = private / "probe"
            d.run("candidate_probe", [sys.executable, "tools/ee_gcc_probe.py",
                  str(d.UNIT.relative_to(d.ROOT)), "--version", "dds2",
                  "--function", d.TARGET, "--out-dir", str(probe), "--strict-assemble"])
            ordinary, ordinary_relocs = target_identity(d.OBJECT)
            observed, observed_relocs = target_identity(probe / "candidate.o")
            bytes_equal = ordinary == observed
            rels_equal = ordinary_relocs == observed_relocs
            d.emit(dict(scope="candidate_probe_parity", target_bytes_equal=bytes_equal,
                   target_relocations_equal=rels_equal, ordinary_bytes=len(ordinary),
                   probe_bytes=len(observed), ordinary_relocations=len(ordinary_relocs),
                   probe_relocations=len(observed_relocs)))
            d.require(bytes_equal and rels_equal, "probe_codegen_changed")
            anchors = alpha_anchors(probe / "functions" / d.TARGET / "00.rtl")
            d.emit(dict(scope="candidate_source_anchors", **anchors))
            allocation_path = private / "allocations.json"
            d.run("candidate_allocations", [sys.executable, "tools/ee_gcc_allocations.py",
                  str(probe), "--function", d.TARGET, "--json", str(allocation_path)])
            manifest = json.loads((probe / "manifest.json").read_text())
            allocations = json.loads(allocation_path.read_text())
            d.require(manifest.get("function") == d.TARGET and
                      manifest.get("version") == "dds2", "probe_scope")
            d.require(manifest.get("wrapper_returncode") == 0 and
                      manifest.get("cc1_succeeded") is True and
                      manifest.get("assembled") is True, "probe_failed")
            d.require((probe / "input.c").read_bytes() == d.UNIT.read_bytes(),
                      "probe_source_changed")
            functions = allocations.get("functions", [])
            d.require(len(functions) == 1 and functions[0].get("function") == d.TARGET,
                      "allocation_scope")
            keys = ("pseudo", "allocation_kind", "global_order", "selected",
                    "hard_conflicts", "pseudo_conflicts", "hard_preferences")
            metrics = ("references", "live_length", "single_block", "set_count",
                       "calls_crossed", "user_variable", "pointer",
                       "hard_register_width", "allocation_priority", "preferred_class", "alternate_class")
            rows = []
            for row in functions[0]["pseudos"]:
                if row["selected"] not in (4, 21, 16, 17, 18, 19):
                    continue
                out = {key: row.get(key) for key in keys}
                inputs = row.get("priority_inputs")
                out["priority_inputs"] = (None if inputs is None else
                    {key: inputs.get(key) for key in metrics})
                rows.append(out)
            d.emit(dict(scope="candidate_only_rtl", function=d.TARGET,
                   wrapper_ok=True, source_snapshot_equal=True, allocation_rows=rows,
                   source_provenance="requires_def_use_anchors",
                   local_quantity_members="not_available_in_static_dumps"))
    finally:
        d.UNIT.write_bytes(original)
    d.require(d.UNIT.read_bytes() == original, "restore_failed")
    d.emit(dict(scope="candidate_only_rtl", source_restored=True,
                full_retail_build_verified=False))
    return 0

if __name__ == "__main__":
    try:
        result = main()
    except d.Failure as exc:
        d.emit(dict(status="diagnostic_failed", stage=exc.stage, returncode=exc.returncode))
        result = 2
    except BaseException:
        d.emit(dict(status="diagnostic_failed", stage=d.STAGE))
        result = 2
    raise SystemExit(result)

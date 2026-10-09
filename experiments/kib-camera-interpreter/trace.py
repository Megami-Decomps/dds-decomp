#!/usr/bin/env python3
"""Qualify unchanged camera compiler dumps and expose a bounded peer slice."""
import json
import re
import sys
import tempfile
from collections import Counter
from pathlib import Path
import diagnose as d
sys.path.insert(0, str(d.ROOT / "tools"))
import check_unit as cu
from ee_gcc_delay_slots import NODE_HEADER, SYMBOL_REF, top_level_forms

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


def summarize_dump(path):
    text = path.read_text()
    d.require(re.findall(r"(?m)^;; Function (\S+)", text) == [d.TARGET],
              "trace_function_scope")
    nodes = []
    for form in top_level_forms(text):
        m = NODE_HEADER.match(form)
        if m:
            nodes.append((m[1], int(m[3]), form))
    calls = [(i, uid, SYMBOL_REF.search(form).group(1))
             for i, (kind, uid, form) in enumerate(nodes)
             if kind == "call_insn" and SYMBOL_REF.search(form)]
    getters = [(i, uid) for i, uid, name in calls if name == "btlGetRuntime"]
    result = dict(pass_name=path.name, nodes=len(nodes),
                  node_kinds=dict(Counter(kind for kind, _, _ in nodes)),
                  runtime_calls=len(getters))
    if len(getters) != 2:
        return result
    begin = getters[1][0]
    ends = [i for i, _, name in calls if i > begin and name == "btlClearUnitDefeatCandidate"]
    if not ends:
        return result
    end = ends[0]
    rows = []
    for kind, uid, form in nodes[begin:end + 1]:
        # Structural metadata comes only from our compiled C, not retail data.
        regs = sorted(set(int(x) for x in re.findall(
            r"\(reg(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+(\d+)(?=[\s)])", form)))
        ints = sorted(set(int(x) for x in re.findall(
            r"\(const_int\s+(-?\d+)(?=[\s)])", form)))
        ops = Counter(re.findall(r"\(([a-z_]+)(?=[:/\s\[])", form))
        rows.append(dict(uid=uid, kind=kind, registers=regs, constants=ints,
                         operations=dict(ops)))
    result["peer_slice"] = rows
    return result

def main():
    original = d.UNIT.read_bytes()
    try:
        d.run("configure", [sys.executable, "configure.py", "dds2"])
        d.require(d.UNIT.read_bytes() == original, "configure_changed_source")
        # Always trace the qualified historical source, not the negative variant.
        patch = Path(__file__).with_name("candidate.patch").read_text()
        d.UNIT.write_text(d.apply_source_patch(original.decode(), patch), encoding="utf-8")
        sizes = d.object_sizes("trace_ordinary")
        d.require(sizes.get(d.TARGET) == 5248, "ordinary_size_changed")
        with tempfile.TemporaryDirectory(prefix="dds-camera-trace-") as temp:
            probe = Path(temp) / "probe"
            d.run("candidate_probe", [sys.executable, "tools/ee_gcc_probe.py",
                  str(d.UNIT.relative_to(d.ROOT)), "--version", "dds2",
                  "--function", d.TARGET, "--out-dir", str(probe), "--strict-assemble"])
            ordinary, ordinary_relocs = target_identity(d.OBJECT)
            observed, observed_relocs = target_identity(probe / "candidate.o")
            d.require(ordinary == observed and ordinary_relocs == observed_relocs,
                      "probe_codegen_changed")
            manifest = json.loads((probe / "manifest.json").read_text())
            d.require(manifest.get("function") == d.TARGET and
                      manifest.get("version") == "dds2" and
                      manifest.get("wrapper_returncode") == 0 and
                      manifest.get("cc1_succeeded") is True and
                      manifest.get("assembled") is True, "probe_manifest")
            d.require((probe / "input.c").read_bytes() == d.UNIT.read_bytes(),
                      "probe_source_changed")
            d.emit(dict(scope="camera_probe_parity", target_bytes_equal=True,
                        target_relocations_equal=True, target_bytes=len(ordinary),
                        relocations=len(ordinary_relocs), source_snapshot_equal=True))
            directory = probe / "functions" / d.TARGET
            paths = sorted(p for p in directory.iterdir() if p.is_file())
            d.require(bool(paths), "probe_dumps_missing")
            for path in paths:
                d.emit(dict(scope="camera_pass_lineage", **summarize_dump(path)))
    finally:
        d.UNIT.write_bytes(original)
    d.require(d.UNIT.read_bytes() == original, "restore_failed")
    d.emit(dict(source_restored=True, full_retail_build_verified=False))

if __name__ == "__main__":
    try:
        main()
    except d.Failure as exc:
        d.emit(dict(status="diagnostic_failed", stage=exc.stage, returncode=exc.returncode))
        raise SystemExit(2)
    except BaseException:
        d.emit(dict(status="diagnostic_failed", stage=d.STAGE))
        raise SystemExit(2)

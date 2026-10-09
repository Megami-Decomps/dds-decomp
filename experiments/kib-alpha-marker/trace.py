#!/usr/bin/env python3
"""Candidate-only allocation metadata; no compiler or retail dumps are printed."""
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import diagnose as d

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
        with tempfile.TemporaryDirectory(prefix="dds-alpha-trace-") as temp:
            private = Path(temp)
            probe = private / "probe"
            d.run("candidate_probe", [sys.executable, "tools/ee_gcc_probe.py",
                  str(d.UNIT.relative_to(d.ROOT)), "--version", "dds2",
                  "--function", d.TARGET, "--out-dir", str(probe), "--strict-assemble"])
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
                       "hard_register_width", "allocation_priority")
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

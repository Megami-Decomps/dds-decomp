#!/usr/bin/env python3
"""Qualify unchanged camera compiler dumps and expose a bounded peer slice."""
import argparse
import json
import re
import shutil
import subprocess
import sys
import tempfile
from collections import Counter
from functools import partial
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



HEADER = re.compile(
    r"\((note|barrier|code_label|call_insn|jump_insn|insn)"
    r"(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+(\d+)\s+(\d+)\s+(\d+)")

def inventories(text):
    groups, current = [], []
    for form in top_level_forms(text):
        match = HEADER.match(form)
        if not match:
            continue
        uid, previous, following = (int(match[i]) for i in (2, 3, 4))
        if previous == 0:
            if current:
                groups.append(current)
            current = []
        current.append((uid, previous, following, form))
        if following == 0:
            groups.append(current)
            current = []
    if current:
        groups.append(current)
    valid = []
    for group in groups:
        if not group or group[0][1] != 0 or group[-1][2] != 0:
            continue
        if len({row[0] for row in group}) != len(group):
            continue
        if any(group[i][2] != group[i+1][0] or group[i+1][1] != group[i][0]
               for i in range(len(group)-1)):
            continue
        valid.append(group)
    return groups, valid

def summarize_dump(path):
    text = path.read_text()
    d.require(re.findall(r"(?m)^;; Function (\S+)", text) == [d.TARGET],
              "trace_function_scope")
    groups, valid = inventories(text)
    if not valid:
        return dict(pass_name=path.name, executable_inventory=False,
                    raw_inventory_groups=len(groups), valid_inventory_groups=0)
    nodes = []
    for _, _, _, form in valid[-1]:
        m = NODE_HEADER.match(form)
        if m:
            nodes.append((m[1], int(m[3]), form))
    calls = [(i, uid, SYMBOL_REF.search(form).group(1))
             for i, (kind, uid, form) in enumerate(nodes)
             if kind == "call_insn" and SYMBOL_REF.search(form)]
    getters = [(i, uid) for i, uid, name in calls if name == "btlGetRuntime"]
    result = dict(pass_name=path.name, executable_inventory=True,
                  raw_inventory_groups=len(groups), valid_inventory_groups=len(valid),
                  nodes=len(nodes),
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


def run_native_control(stage, command, case, stem):
    """Run exact frozen native argv/environment, keeping compiler output private."""
    d.STAGE = stage
    case.verify()
    with case.logs[stem + "_stdout"].open("wb") as stdout, case.logs[stem + "_stderr"].open("wb") as stderr:
        result = subprocess.run(command, cwd=case.cwd, env=case.env,
                                stdout=stdout, stderr=stderr, check=False,
                                timeout=case.timeout)
    d.require(result.returncode == 0, stage)
    case.verify()


def live_cse_observation(temp, watch_path, ordinary_identity, qemu):
    """Observe the current unchanged candidate only after all target parity gates."""
    import ee_gcc_cse_case as case_builder
    import ee_gcc_cse_role_tracer as cse
    import ee_gcc_observe as observer
    temp, qemu = Path(temp), qemu.resolve()
    d.require(qemu.is_file(), "cse_qemu_missing")
    case_dir, output = temp / "case", temp / "case" / "observed"
    source_path = d.ROOT / case_builder.SOURCE_REL
    assembly_path = d.ROOT / case_builder.ASSEMBLY_REL
    d.require(not source_path.exists() and not assembly_path.exists(),
              "cse_fixed_paths_not_fresh")
    expected_source_hash = d.digest(d.UNIT)
    try:
        d.STAGE = "cse_case_prepare"
        # Preparation checks the selected QEMU interface; it never builds QEMU.
        case_builder.make_case(d.ROOT, d.UNIT, case_dir, qemu, watch_path)
        case = observer.load_case(case_dir / "case.json")
        d.emit(dict(scope="camera_unix_capability", qualified=True,
                    transport=case.transport["kind"], child_confined=True,
                    socket_owned=True, peer_verified=True,
                    no_owned_internet_socket=True, guest_continued=False))
        control = json.loads((case_dir / "native-control.json").read_text())
        d.require(control["cwd"] == str(case.cwd) and control["environment"] == case.env
                  and control["compiler_command"] == case.command[1:]
                  and control["assembler_command"] == case.assembler[1:],
                  "cse_native_control_contract")
        run_native_control("cse_native_cc1", control["compiler_command"], case, "cc1")
        run_native_control("cse_native_assembler", control["assembler_command"], case, "as")
        case.inventory()
        native_object = case_dir / "candidate.o"
        native_identity = target_identity(native_object)
        d.require(native_identity == ordinary_identity, "cse_ordinary_native_codegen_changed")
        native_archive = temp / "native-control"
        native_archive.mkdir()
        shutil.copy2(native_object, native_archive / "candidate.o")
        shutil.copy2(assembly_path, native_archive / "candidate.s")
        d.emit(dict(scope="camera_native_control_parity", target_bytes_equal=True,
                    target_relocations_equal=True, target_bytes=len(native_identity[0]),
                    relocations=len(native_identity[1]), exact_frozen_environment=True))

        # This existing CLI also checks actual source argv against the watch,
        # hashes every executed observer module, and rejects incomplete plans.
        d.run("cse_observer_dry_run", [
            sys.executable, "tools/ee_gcc_cse_role_tracer.py",
            "--config", str(case_dir / "case.json"), "--watch", str(watch_path),
            "--output", str(output), "--dry-run"])
        watch = json.loads(watch_path.read_text())
        case.verify()
        d.STAGE = "cse_paired_observation"
        original_tracer = observer.OperandTracer
        observer.OperandTracer = partial(cse.CseRoleTracer, watch=watch)
        try:
            # In-process invocation keeps the existing runner's owned-QEMU
            # cleanup active if tracing raises. No protocol/transport changes.
            receipt = observer.run(case, output, "cse_roles")
        except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
            categories = (
                ("live CSE UID does not match frozen input identity", "input_identity"),
                ("watched UIDs were not visited", "watch_coverage"),
                ("hook opcode mismatch", "hook_guard"),
                ("unsupported RTX format field", "decoder_format"),
                ("RTL decode bound exceeded", "decoder_bound"),
                ("RTL vector bound exceeded", "decoder_bound"),
                ("bounded trace event limit exceeded", "event_bound"),
                ("Observed artifacts differ", "artifact_equality"),
                ("Artifact inventories differ", "artifact_inventory"),
                ("Input hash changed", "input_changed"),
                ("Missing target", "target_gate"),
            )
            category = next((label for prefix, label in categories
                             if str(error).startswith(prefix)), type(error).__name__)
            d.emit(dict(scope="camera_live_observer_failure", category=category))
            raise d.Failure("cse_paired_observation") from None
        finally:
            observer.OperandTracer = original_tracer
        d.require(receipt.get("all_equal") is True, "cse_observer_not_equal")
        d.STAGE = "cse_native_qemu_parity"
        baseline_identity = target_identity(output / "baseline/candidate.o")
        observed_identity = target_identity(output / "observed/candidate.o")
        d.require(baseline_identity == native_identity == observed_identity,
                  "cse_native_qemu_codegen_changed")
        case.verify()
        d.emit(dict(scope="camera_live_observer_parity", all_declared_artifacts_equal=True,
                    declared_artifacts=len(receipt["artifacts_equal"]),
                    native_qemu_baseline_target_equal=True, native_qemu_observed_target_equal=True,
                    target_bytes=len(native_identity[0]), relocations=len(native_identity[1])))
        d.STAGE = "cse_bounded_decisions"
        summary = cse.summarize_decisions(output, limit_per_uid=12)
        d.emit(dict(scope="camera_cse_decisions", **summary))
        d.require(summary["coverage_complete"], "cse_summary_incomplete")
    finally:
        # Only these initially absent, task-owned scratch files are removed.
        # Never remove a changed source which could belong to another writer.
        if source_path.is_file() and d.digest(source_path) == expected_source_hash:
            source_path.unlink()
        if assembly_path.is_file():
            assembly_path.unlink()


def main(qemu=None):
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
            import ee_gcc_role_lineage as roles_module
            role_manifest, role_source, role_stages = roles_module.load_probe(probe, d.TARGET)
            origin = role_stages["00.rtl"]
            spec = json.loads(Path(__file__).with_name("roles.json").read_text())
            spans = roles_module.role_spans(role_source, spec)
            source_name = role_manifest.get("as_unit") or role_manifest["source"]
            parsed_notes = [row for row in origin.values() if row.get("source_note")]
            executable = [row for row in origin.values() if row["kind"] in roles_module.EXECUTABLE]
            linked = [row for row in executable if row.get("source")]
            matching = [row for row in linked if roles_module.same_source(row["source"]["file"], source_name)]
            lines = [row["source"]["line"] for row in linked]
            d.emit(dict(scope="source_note_metadata",
                        notes=sum(row["kind"] == "note" for row in origin.values()),
                        parsed_source_notes=len(parsed_notes),
                        executable_source_rows=len(linked), canonical_source_rows=len(matching),
                        min_line=min(lines) if lines else None,
                        max_line=max(lines) if lines else None,
                        roles=[dict(name=span["name"], first_line=span["first_line"],
                                    end_line=span["end_line_exclusive"],
                                    selected=sum(span["first_line"] <= row["source"]["line"]
                                                 < span["end_line_exclusive"] for row in matching))
                               for span in spans]))
            role_report, watch = Path(temp) / "roles.json", Path(temp) / "watch.json"
            role_result = d.run("source_role_lineage", [sys.executable, "tools/ee_gcc_role_lineage.py",
                  str(probe), "--roles", str(Path(__file__).with_name("roles.json")),
                  "--json", str(role_report), "--watch", str(watch)], allowed=(0, 2))
            if role_result.returncode:
                prefixes = ("unrecognized executable RTL header", "no complete linked RTL inventory",
                    "incomplete final RTL inventory", "empty or incomplete target RTL",
                    "role ", "ambiguous or missing", "probe is not", "compiler hash",
                    "function mismatch", "source snapshot", "explicit #line", "probe object",
                    "missing required", "RTL artifact", "stale or incomplete", "target missing")
                category = next((p for p in prefixes if role_result.stderr.startswith(p)), "unclassified")
                d.emit(dict(scope="lineage_failure", category=category))
                raise d.Failure("source_role_lineage", role_result.returncode)
            report = json.loads(role_report.read_text())
            d.emit(dict(scope="camera_source_roles", roles=[
                dict(name=role["role"]["name"], seeds=len(role["seed_uids"]),
                     first_transformation=role["first_observed_candidate_transformation"],
                     transitions=[dict(before=row["before_stage"], after=row["after_stage"],
                                       changes=row["change_count"],
                                       surviving_order_changed=row["surviving_uid_order_changed"])
                                  for row in role["transitions"]])
                for role in report["roles"]],
                watched_uids=[row["uid"] for row in report["cse_watch"]["uids"]],
                omitted_watch_uids=report["cse_watch"]["omitted_count"]))
            directory = probe / "functions" / d.TARGET
            paths = sorted(p for p in directory.iterdir() if p.is_file()
                           and re.fullmatch(r"\d{2}\.[a-z0-9]+", p.name))
            d.require(bool(paths), "probe_dumps_missing")
            for path in paths:
                d.emit(dict(scope="camera_pass_lineage", **summarize_dump(path)))
            if qemu is not None:
                live_cse_observation(temp, watch, (ordinary, ordinary_relocs), qemu)
    finally:
        d.UNIT.write_bytes(original)
    d.require(d.UNIT.read_bytes() == original, "restore_failed")
    d.emit(dict(source_restored=True, full_retail_build_verified=False))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--qemu", type=Path, help="use an already prepared Unix-capable QEMU for paired live observation")
    args = parser.parse_args()
    try:
        main(args.qemu)
    except d.Failure as exc:
        d.emit(dict(status="diagnostic_failed", stage=exc.stage, returncode=exc.returncode))
        raise SystemExit(2)
    except BaseException:
        d.emit(dict(status="diagnostic_failed", stage=d.STAGE))
        raise SystemExit(2)

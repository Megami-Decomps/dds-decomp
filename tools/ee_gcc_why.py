#!/usr/bin/env python3
"""Turn two EE GCC probes into a bounded matcher-facing diagnosis."""

from __future__ import annotations

import argparse
import difflib
import json
import re
import sys
from pathlib import Path
from typing import Any

import ee_gcc_allocations as allocations
import ee_gcc_compare as compare
import ee_gcc_delay_slots as delay_slots
import ee_gcc_schedules as schedules


DIAGNOSTIC_CFLAG = re.compile(r"^-fsched-verbose(?:=.*)?$")
RTL_UID = re.compile(
    r"^\((?:insn|jump_insn|call_insn)[^ \t\r\n()]*\s+(\d+)"
)
RTL_PRIMARY_SET = re.compile(
    r"^\((?:insn|jump_insn|call_insn)[^ \t\r\n()]*"
    r"\s+\d+\s+\d+\s+\d+\s+(\(set\b)"
)
DI_REGISTER_DESTINATION = re.compile(
    r"^\(set\s+\(reg(?:/[A-Za-z]+)*:DI\s+(\d+)\b"
)
DI_REGISTER_SOURCE = re.compile(
    r"\(reg(?:/[A-Za-z]+)*:DI\s+(\d+)(?:\s+[^()]*)?\)\s*\)$"
)
SI_TO_DI_EXTENSION = re.compile(
    r"\((zero_extend|sign_extend):DI\s+"
    r"\(reg(?:/[A-Za-z]+)*:SI\s+\d+\b"
)
DI_MEMORY_DESTINATION = re.compile(
    r"^\(set\s+\(mem(?:/[A-Za-z]+)*:DI\b"
)


def read_manifest(directory: Path) -> tuple[dict[str, Any], str | None]:
    path = directory / "manifest.json"
    if not path.is_file():
        return {}, f"missing manifest: {path}"
    try:
        data = json.loads(path.read_text())
    except (OSError, json.JSONDecodeError) as error:
        return {}, f"cannot read manifest {path}: {error}"
    if not isinstance(data, dict):
        return {}, f"manifest is not a JSON object: {path}"
    return data, None


def provenance(manifest: dict[str, Any]) -> dict[str, Any]:
    compiler = manifest.get("compiler")
    compiler_hash = compiler.get("sha256") if isinstance(compiler, dict) else None
    return {
        "version": manifest.get("version"),
        "as_unit": manifest.get("as_unit"),
        "source_sha256": manifest.get("source_sha256"),
        "compiler_sha256": compiler_hash,
        "extra_cflags": effective_cflags(manifest),
        "wrapper_returncode": manifest.get("wrapper_returncode"),
        "cc1_succeeded": manifest.get("cc1_succeeded"),
        "object": manifest.get("object"),
    }


def effective_cflags(manifest: dict[str, Any]) -> list[str] | None:
    """Read probe flags, recovering them from older manifests when possible."""
    recorded = manifest.get("extra_cflags")
    if isinstance(recorded, list):
        return [str(flag) for flag in recorded]
    command = manifest.get("command")
    if not isinstance(command, list):
        return None
    args = [str(item) for item in command]
    try:
        start = args.index("-dumpbase") + 2
    except (ValueError, IndexError):
        return None
    source = manifest.get("compiled_source") or manifest.get("source")
    try:
        end = args.index(str(source), start) if source else args.index("-o", start)
    except ValueError:
        return None
    return args[start:end]


def semantic_cflags(flags: Any) -> list[str] | None:
    """Exclude flags which only add text to captured diagnostics."""
    if not isinstance(flags, list):
        return None
    return [str(flag) for flag in flags if not DIAGNOSTIC_CFLAG.match(str(flag))]


def resolve_function_scope(left: Path, right: Path,
                           left_manifest: dict[str, Any],
                           right_manifest: dict[str, Any],
                           requested: str | None) -> tuple[str | None, bool]:
    """Infer a shared extracted function, or reject an ambiguous scope."""
    left_function = left_manifest.get("function")
    right_function = right_manifest.get("function")
    left_aliases = compare.replacement_aliases(left)
    right_aliases = compare.replacement_aliases(right)

    def canonical(value: Any, aliases: dict[str, str]) -> str | None:
        return compare.canonical_name(value, aliases) if isinstance(value, str) and value else None

    left_name = canonical(left_function, left_aliases)
    right_name = canonical(right_function, right_aliases)
    if requested:
        wanted_left = canonical(requested, left_aliases)
        wanted_right = canonical(requested, right_aliases)
        if left_name is not None and left_name != wanted_left:
            raise ValueError(f"left probe extracted {left_function!r}, not {requested!r}")
        if right_name is not None and right_name != wanted_right:
            raise ValueError(f"right probe extracted {right_function!r}, not {requested!r}")
        return requested, False
    if left_name is None and right_name is None:
        return None, False
    if left_name is None or right_name is None:
        raise ValueError("probe scope mismatch: only one manifest records an extracted function")
    if left_name != right_name:
        raise ValueError(
            f"probe scope mismatch: {left_function!r} != {right_function!r}"
        )
    return left_name, True


def object_comparison(left: dict[str, Any], right: dict[str, Any]) -> dict[str, Any]:
    left_object = left.get("object")
    right_object = right.get("object")
    left_ok = left.get("wrapper_returncode") == 0 and left.get("cc1_succeeded") is True
    right_ok = right.get("wrapper_returncode") == 0 and right.get("cc1_succeeded") is True
    left_raw = left_object.get("sha256") if isinstance(left_object, dict) else None
    right_raw = right_object.get("sha256") if isinstance(right_object, dict) else None
    left_hash = left_object.get("path_normalized_sha256") if isinstance(left_object, dict) else None
    right_hash = right_object.get("path_normalized_sha256") if isinstance(right_object, dict) else None
    if not (left_ok and right_ok and left_hash and right_hash):
        status = "unavailable"
    else:
        status = "same" if left_hash == right_hash else "different"
    return {
        "scope": "translation-unit",
        "status": status,
        "raw_status": (
            "same" if left_raw and right_raw and left_raw == right_raw else
            "different" if left_raw and right_raw else "unavailable"
        ),
        "left_sha256": left_raw,
        "right_sha256": right_raw,
        "left_path_normalized_sha256": left_hash,
        "right_path_normalized_sha256": right_hash,
        "note": "path-normalized hashes mask only the wrapper's random same-length scratch directory",
    }


def artifact_path(directory: Path, function: str | None, artifact: str) -> Path | None:
    aliases = compare.replacement_aliases(directory)
    if function:
        root = compare.function_directory(directory, function, aliases)
        if root is None:
            return None
        name = "final.s" if artifact == "assembly" else artifact.removeprefix("rtl.")
        return root / name
    if artifact == "assembly":
        return directory / "candidate.s"
    return directory / (artifact if artifact.startswith("rtl.") else f"rtl.{artifact}")


def normalized_diff(left: Path, right: Path, function: str | None,
                    artifact: str, limit: int = 80) -> list[str]:
    a = artifact_path(left, function, artifact)
    b = artifact_path(right, function, artifact)
    if a is None or b is None or not a.is_file() or not b.is_file():
        return []
    left_aliases = compare.replacement_aliases(left)
    right_aliases = compare.replacement_aliases(right)
    lines = list(difflib.unified_diff(
        compare.normalize_artifact(
            a.read_text(errors="replace"), artifact, left_aliases
        ).splitlines(),
        compare.normalize_artifact(
            b.read_text(errors="replace"), artifact, right_aliases
        ).splitlines(),
        fromfile="left/" + a.name,
        tofile="right/" + b.name,
        lineterm="",
    ))
    if len(lines) > limit:
        return lines[:limit] + [f"... diff truncated at {limit} lines"]
    return lines


def parse_side(directory: Path, function: str | None, artifact: str,
               parser: Any) -> list[dict[str, Any]]:
    path = artifact_path(directory, function, artifact)
    if path is None or not path.is_file():
        raise FileNotFoundError(f"missing {artifact}: {path or directory}")
    if artifact == "rtl.20.greg":
        # An extracted dump contains exactly one function.  Parsing without a
        # name also works when a controlled rename changed its printed name.
        return parser(path.read_text(errors="replace"), None)
    return parser(path.read_text(errors="replace"))


def primary_set(form: str) -> str | None:
    """Return an instruction's outer SET, excluding SETs in notes."""
    match = RTL_PRIMARY_SET.match(form)
    if match is None:
        return None
    try:
        result, _ = delay_slots.balanced_form(form, match.start(1))
    except ValueError:
        return None
    return result


def integer_widening_stores(text: str, lookahead: int = 8) -> list[dict[str, Any]]:
    """Find unambiguous SI-to-DI results which feed a nearby DI store."""
    forms = delay_slots.top_level_forms(text)
    observations: list[dict[str, Any]] = []
    for index, form in enumerate(forms):
        set_form = primary_set(form)
        if set_form is None:
            continue
        destination = DI_REGISTER_DESTINATION.match(set_form)
        kinds = set(SI_TO_DI_EXTENSION.findall(form))
        if destination is None or len(kinds) != 1:
            continue
        pseudo = int(destination.group(1))
        extension_uid = RTL_UID.match(form)
        for later in forms[index + 1:index + 1 + lookahead]:
            later_set = primary_set(later)
            if later_set is None:
                continue
            source = DI_REGISTER_SOURCE.search(later_set)
            if (
                DI_MEMORY_DESTINATION.match(later_set)
                and source is not None
                and int(source.group(1)) == pseudo
            ):
                store_uid = RTL_UID.match(later)
                observations.append({
                    "kind": next(iter(kinds)),
                    "from_mode": "SI",
                    "to_mode": "DI",
                    "destination_pseudo": pseudo,
                    "extension_uid": (
                        int(extension_uid.group(1)) if extension_uid else None
                    ),
                    "store_uid": int(store_uid.group(1)) if store_uid else None,
                })
                break
            redefinition = DI_REGISTER_DESTINATION.match(later_set)
            if redefinition is not None and int(redefinition.group(1)) == pseudo:
                break
    return observations


def integer_widening_comparison(left: Path, right: Path,
                                function: str | None) -> dict[str, Any] | None:
    """Compare pass-00 widening-to-store observations without pairing pseudos."""
    paired: dict[str, Any] = {}
    for label, directory in (("left", left), ("right", right)):
        path = artifact_path(directory, function, "rtl.00.rtl")
        if path is None or not path.is_file():
            return None
        paired[label] = integer_widening_stores(path.read_text(errors="replace"))
    left_rows = paired["left"]
    right_rows = paired["right"]
    paired["opposite_signedness"] = (
        len(left_rows) == 1
        and len(right_rows) == 1
        and {left_rows[0]["kind"], right_rows[0]["kind"]}
        == {"zero_extend", "sign_extend"}
    )
    return paired


def diagnosis_for(comparison: dict[str, Any], object_report: dict[str, Any],
                  function: str | None, compiler_evidence: bool = True,
                  widening: dict[str, Any] | None = None) -> dict[str, Any]:
    first = comparison["first_semantic_divergence"]
    if not compiler_evidence:
        return {
            "class": "insufficient-evidence",
            "basis": ["at least one manifest reports incomplete compiler artifacts"],
            "next_step": "recapture the failed probe before interpreting codegen differences",
        }
    if first is None:
        if comparison["assembly"]["status"] == "missing":
            scope = f"function {function}" if function else "translation unit"
            detail = (
                "neither probe contains usable artifacts"
                if not comparison["passes"] else
                "the final assembly artifact is missing"
            )
            return {
                "class": "insufficient-evidence",
                "basis": [f"{detail} for {scope}"],
                "next_step": "recapture both probes and verify that the requested function was extracted",
            }
        if function is None and object_report["status"] == "different":
            return {
                "class": "object-emission",
                "basis": ["normalized RTL and assembly agree, but successful object hashes differ"],
                "next_step": "inspect assembler, relocation, and object metadata; do not vary C",
            }
        return {
            "class": "no-codegen-difference",
            "basis": ["no normalized target artifact differs"],
            "next_step": "stop: this source change had no observed target-code effect",
        }
    classification = comparison.get("classification") or {}
    number = classification.get("pass_number")
    if first != "assembly" and any(
        row["name"] == first and row["status"] == "missing"
        for row in comparison["passes"]
    ):
        return {
            "class": "insufficient-evidence",
            "basis": [f"the first unmatched artifact is missing on one side: {first}"],
            "next_step": "recapture both probes with the same function and compiler options",
        }
    if comparison["assembly"]["status"] == "same":
        return {
            "class": "final-code-convergence",
            "basis": [
                f"the first normalized difference is {first}, but extracted final assembly agrees"
            ],
            "next_step": (
                "stop changing source for the match; inspect intermediate passes "
                "only for mechanism research"
            ),
        }
    if (
        number == 0
        and widening is not None
        and widening.get("opposite_signedness") is True
    ):
        left_kind = widening["left"][0]["kind"]
        right_kind = widening["right"][0]["kind"]
        return {
            "class": "integer-widening",
            "basis": [
                "pass 00 changes an SI-to-DI wide-store input from "
                f"{left_kind} to {right_kind}"
            ],
            "next_step": (
                "verify the authentic source width and signedness, then test one "
                "truthful type or cast boundary"
            ),
        }
    if number in (19, 20):
        return {
            "class": "register-allocation",
            "basis": [f"the first normalized difference is {first}"],
            "next_step": "inspect per-side lifetimes, register classes, conflicts, and preferences; test one truthful source fact or park it",
        }
    if number == 29:
        return {
            "class": "delay-slot-selection",
            "basis": ["the first captured normalized difference is delayed-branch scheduling"],
            "next_step": "compare donor availability and eligibility before pass 29; avoid cosmetic source variants",
        }
    if number in (17, 25):
        return {
            "class": (
                "pre-reload-scheduling" if number == 17
                else "post-reload-scheduling"
            ),
            "basis": [f"the first normalized difference is {first}"],
            "next_step": (
                "compare ready instructions, dependencies, and register pressure at "
                "sched1; stop if the desired order has no truthful source-level cause"
                if number == 17 else
                "compare ready instructions and dependencies at sched2; stop if the "
                "desired order has no truthful dependency"
            ),
        }
    if first == "assembly":
        return {
            "class": "final-emission",
            "basis": ["all captured RTL agrees and final assembly differs"],
            "next_step": "inspect MIPS final emission/shortening, then confirm the assembled object",
        }
    stage = str(classification.get("stage", "unknown"))
    slug = re.sub(r"[^a-z0-9]+", "-", stage.lower()).strip("-") or "unknown-stage"
    return {
        "class": slug,
        "basis": [f"the first normalized difference is {first} ({stage})"],
        "next_step": f"inspect the normalized {first} diff and test one source fact that acts before {stage}",
    }


def analyze(left: Path, right: Path, function: str | None = None) -> dict[str, Any]:
    left_manifest, left_error = read_manifest(left)
    right_manifest, right_error = read_manifest(right)
    warnings = [error for error in (left_error, right_error) if error]
    function, inferred_scope = resolve_function_scope(
        left, right, left_manifest, right_manifest, function
    )
    if inferred_scope:
        warnings.append(f"inferred function scope from both manifests: {function}")
    left_provenance = provenance(left_manifest)
    right_provenance = provenance(right_manifest)
    for field in ("version", "as_unit", "compiler_sha256", "extra_cflags"):
        a, b = left_provenance[field], right_provenance[field]
        if a is not None and b is not None and a != b:
            warnings.append(f"provenance mismatch: {field} ({a!r} != {b!r})")

    comparison = compare.compare_runs(left, right, function)
    objects = object_comparison(left_manifest, right_manifest)
    compiler_evidence = not any(
        manifest.get("cc1_succeeded") is False
        for manifest in (left_manifest, right_manifest)
    )
    first = comparison["first_semantic_divergence"]
    widening = (
        integer_widening_comparison(left, right, function)
        if first == "rtl.00.rtl" else None
    )
    diagnosis = diagnosis_for(
        comparison, objects, function, compiler_evidence, widening
    )
    incompatible: list[str] = []
    left_flags = semantic_cflags(left_provenance["extra_cflags"])
    right_flags = semantic_cflags(right_provenance["extra_cflags"])
    if left_flags is not None and right_flags is not None and left_flags != right_flags:
        incompatible.append("code-affecting compiler flags differ")
    left_compiler = left_provenance["compiler_sha256"]
    right_compiler = right_provenance["compiler_sha256"]
    if left_compiler and right_compiler and left_compiler != right_compiler:
        incompatible.append("compiler hashes differ")
    if incompatible:
        diagnosis = {
            "class": "insufficient-evidence",
            "basis": incompatible,
            "next_step": "recapture both probes with the same compiler and code-affecting options",
        }
    report: dict[str, Any] = {
        "schema": 1,
        "comparison": comparison,
        "provenance": {"left": left_provenance, "right": right_provenance},
        "warnings": warnings,
        "object": objects,
        "diagnosis": diagnosis,
        "first_diff": normalized_diff(left, right, function, first) if first else [],
    }
    if widening is not None:
        report["integer_widening"] = widening

    evidence_gaps: list[str] = []
    number = (comparison.get("classification") or {}).get("pass_number")
    if number in (19, 20):
        paired: dict[str, Any] = {}
        for label, directory in (("left", left), ("right", right)):
            try:
                paired[label] = allocations.parse_probe(directory, function)
            except (FileNotFoundError, ValueError) as error:
                evidence_gaps.append(str(error))
        report["allocation"] = paired
    elif number == 29:
        paired = {}
        for label, directory in (("left", left), ("right", right)):
            try:
                paired[label] = parse_side(
                    directory, function, "rtl.29.dbr", delay_slots.analyze_dump
                )
            except (FileNotFoundError, ValueError) as error:
                evidence_gaps.append(str(error))
        report["delay_slots"] = paired
    elif number in (17, 25):
        paired = {}
        artifact = "rtl.17.sched" if number == 17 else "rtl.25.sched2"
        for label, directory in (("left", left), ("right", right)):
            try:
                paired[label] = parse_side(
                    directory, function, artifact, schedules.parse_dump
                )
            except (FileNotFoundError, ValueError) as error:
                evidence_gaps.append(str(error))
        report["schedules"] = paired
    if evidence_gaps:
        report["evidence_gaps"] = evidence_gaps
        diagnosis["basis"].append("supporting stage evidence is incomplete")
    return report


def render(report: dict[str, Any]) -> str:
    comparison = report["comparison"]
    diagnosis = report["diagnosis"]
    lines = [
        f"first       {comparison['first_semantic_divergence'] or 'none'}",
        f"diagnosis   {diagnosis['class']}",
        f"object      {report['object']['status']} (whole translation unit)",
    ]
    for basis in diagnosis["basis"]:
        lines.append(f"basis       {basis}")
    lines.append(f"next        {diagnosis['next_step']}")
    for warning in report["warnings"]:
        lines.append(f"warning     {warning}")
    for gap in report.get("evidence_gaps", []):
        lines.append(f"gap         {gap}")
    if "integer_widening" in report:
        for side in ("left", "right"):
            for row in report["integer_widening"][side]:
                lines.append(
                    f"{side} widen uid {row['extension_uid']} -> store "
                    f"{row['store_uid']}: {row['kind']}:{row['to_mode']}"
                )
    if "allocation" in report:
        for side in ("left", "right"):
            functions = report["allocation"].get(side, [])
            lines.append(f"{side} alloc  {len(functions)} function(s)")
            for function in functions:
                order = " ".join(f"r{value}" for value in function["global_allocation_order"])
                lines.append(
                    f"  {function['function']}: order {order or '(none)'}; "
                    f"retries {function['retry_count']}; spills {len(function['spills'])}; "
                    f"reloads {len(function['reloads'])}"
                )
    if "delay_slots" in report:
        for side in ("left", "right"):
            sequences = report["delay_slots"].get(side, [])
            lines.append(f"{side} slots  {len(sequences)} sequence(s)")
            for sequence in sequences:
                branch = sequence["branch"]
                donors = ", ".join(
                    f"uid {slot['uid']} {slot['operation']}"
                    for slot in sequence["slots"]
                ) or "none"
                lines.append(f"  uid {branch['uid']} -> {donors}")
    if "schedules" in report:
        for side in ("left", "right"):
            functions = report["schedules"].get(side, [])
            count = sum(function["contested_count"] for function in functions)
            lines.append(f"{side} sched  {count} contested selection(s)")
            emitted = 0
            for function in functions:
                for block in function["blocks"]:
                    for choice in block["contested"]:
                        if emitted >= 4:
                            break
                        ready = " ".join(str(uid) for uid in choice["ready_uids"])
                        lines.append(
                            f"  bb {block['block']} t={choice['clock']}: "
                            f"uid {choice['selected_uid']} from {ready}"
                        )
                        emitted += 1
                    if emitted >= 4:
                        break
                if emitted >= 4:
                    break
    if report["first_diff"]:
        lines.append("first diff")
        lines.extend(f"  {line}" for line in report["first_diff"])
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("left", type=Path)
    parser.add_argument("right", type=Path)
    parser.add_argument("--function", help="diagnose one extracted function")
    parser.add_argument("--json", type=Path, help="also write the complete report")
    args = parser.parse_args(argv)
    for directory in (args.left, args.right):
        if not directory.is_dir():
            parser.error(f"probe directory not found: {directory}")
    try:
        report = analyze(args.left, args.right, args.function)
    except (OSError, ValueError, json.JSONDecodeError) as error:
        print(f"cannot compare probes: {error}", file=sys.stderr)
        return 2
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(render(report))
    if report["diagnosis"]["class"] == "insufficient-evidence":
        return 2
    return 1 if report["comparison"]["first_semantic_divergence"] else 0


if __name__ == "__main__":
    raise SystemExit(main())

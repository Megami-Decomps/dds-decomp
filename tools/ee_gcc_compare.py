#!/usr/bin/env python3
"""Compare two EE GCC probe directories and find the first semantic divergence.

Known observation noise is normalized before comparison: the wrapper's random
same-length scratch directory and raw lexical-block pointers printed in RTL
notes.  Raw and normalized equality are reported separately so address-layout
evidence is not discarded.
"""

from __future__ import annotations

import argparse
import difflib
import json
import re
import sys
from pathlib import Path


SCRATCH_DIR = re.compile(r"\.[0-9a-fA-F]{2,4}/(?=dds[12]/)")
BLOCK_POINTER = re.compile(r"0x[0-9a-fA-F]+(?=\s+NOTE_INSN_BLOCK_(?:BEG|END))")
# GCC 2.96's special-note union is not consistently initialized before dumps.
# Its final decimal field can therefore contain an arena pointer in one run and
# zero in another for any NOTE_INSN_* kind. It is observation noise, not RTL.
SPECIAL_NOTE_PAYLOAD = re.compile(r"(?m)(NOTE_INSN_[A-Z_]+ )-?\d+(\)\s*$)")
CODE_LABEL_NUMBER = re.compile(r"(?m)^(\(code_label\s+\d+\s+\d+\s+\d+\s+)\d+")
LOCAL_ASM_LABEL = re.compile(r"\$L\d+\b")
ASM_FUNCTION = re.compile(
    r"(?ms)^\s*\.ent\s+([^\s]+)\s*$.*?^\s*\.end\s+\1\s*$"
)
PASS_FILE = re.compile(r"^rtl\.(\d+)\.(.+)$")
EXTRACTED_PASS_FILE = re.compile(r"^(\d+)\.(.+)$")
PASS_INFO = {
    0: ("RTL expansion", "the front end or initial RTL expansion"),
    1: ("sibling calls", "sibling/tail-call optimization"),
    2: ("jump", "initial jump optimization and CFG cleanup"),
    3: ("CSE", "the first common-subexpression elimination pass"),
    4: ("address-of", "address-of purging"),
    5: ("SSA", "conversion to SSA"),
    6: ("DCE", "SSA dead-code elimination"),
    7: ("SSA exit", "conversion out of SSA"),
    8: ("GCSE", "global common-subexpression elimination"),
    9: ("loop", "loop optimization"),
    10: ("CSE2", "post-loop CSE or its preparatory jump/if-conversion steps"),
    11: ("CFG", "CFG construction and cleanup"),
    12: ("branch probabilities", "branch-probability analysis"),
    13: ("liveness", "life analysis"),
    14: ("combine", "instruction combination"),
    15: ("if-conversion", "post-combine if-conversion"),
    16: ("regmove", "register-move optimization"),
    17: ("sched1", "pre-reload instruction scheduling"),
    19: ("local allocation", "register-class selection or local allocation"),
    20: ("global allocation", "global allocation, reload, or reload CSE"),
    21: ("flow2", "post-reload flow, prologue, or epilogue processing"),
    22: ("if-conversion 2", "post-reload if-conversion"),
    24: ("register rename", "hard-register renaming"),
    25: ("sched2", "post-reload instruction scheduling"),
    26: ("block reorder", "basic-block reordering"),
    27: ("jump2", "late jump optimization"),
    28: ("machine reorg", "MIPS machine-dependent reorganization"),
    29: ("delay slots", "delayed-branch scheduling"),
}


def replacement_aliases(directory: Path) -> dict[str, str]:
    manifest = directory / "manifest.json"
    if not manifest.is_file():
        return {}
    data = json.loads(manifest.read_text())
    return {
        item["new"]: item["old"]
        for item in data.get("replacements", [])
        if item.get("new") and item.get("old")
    }


def normalize(text: str, aliases: dict[str, str] | None = None) -> str:
    if aliases:
        for name in sorted(aliases, key=len, reverse=True):
            text = re.sub(rf"\b{re.escape(name)}\b", aliases[name], text)
    text = SCRATCH_DIR.sub(".XX/", text)
    text = BLOCK_POINTER.sub("0xPTR", text)
    text = SPECIAL_NOTE_PAYLOAD.sub(r"\1PTR\2", text)
    text = CODE_LABEL_NUMBER.sub(r"\1LABELNO", text)
    labels: dict[str, str] = {}

    def local_label(match: re.Match[str]) -> str:
        label = match.group(0)
        if label not in labels:
            labels[label] = f"$LLOCAL{len(labels)}"
        return labels[label]

    return LOCAL_ASM_LABEL.sub(local_label, text)


def normalize_artifact(text: str, artifact: str,
                       aliases: dict[str, str] | None = None) -> str:
    """Normalize an artifact without treating dump commentary as compiler IR."""
    match = PASS_FILE.match(artifact)
    if match and int(match.group(1)) in (17, 25):
        # -fsched-verbose changes scheduler reports (ready lists, visual tables,
        # truncated symbol names) but not the RTL forms which follow them.
        text = "\n".join(
            line for line in text.splitlines()
            if line.strip() and not line.lstrip().startswith(";;")
        ) + "\n"
    return normalize(text, aliases)


def assembly_functions(text: str, aliases: dict[str, str] | None = None) -> dict[str, str]:
    result: dict[str, str] = {}
    for match in ASM_FUNCTION.finditer(text):
        name = aliases.get(match.group(1), match.group(1)) if aliases else match.group(1)
        result[name] = normalize(match.group(0), aliases)
    return result


def dump_files(directory: Path) -> list[Path]:
    def key(path: Path) -> tuple[int, str]:
        match = PASS_FILE.match(path.name)
        return (int(match.group(1)), match.group(2)) if match else (999, path.name)

    return sorted(directory.glob("rtl.[0-9][0-9].*"), key=key)


def canonical_name(name: str, aliases: dict[str, str]) -> str:
    return aliases.get(name, name)


def function_directory(directory: Path, function: str,
                       aliases: dict[str, str]) -> Path | None:
    root = directory / "functions"
    if not root.is_dir():
        return None
    wanted = canonical_name(function, aliases)
    for candidate in root.iterdir():
        if candidate.is_dir() and canonical_name(candidate.name, aliases) == wanted:
            return candidate
    return None


def function_dump_files(directory: Path, function: str,
                        aliases: dict[str, str]) -> dict[str, Path]:
    root = function_directory(directory, function, aliases)
    if root is None:
        return {}
    result: dict[str, Path] = {}
    for path in root.iterdir():
        match = EXTRACTED_PASS_FILE.match(path.name)
        if match:
            result[f"rtl.{path.name}"] = path
    return result


def classify_artifact(artifact: str | None) -> dict[str, object] | None:
    if artifact is None:
        return None
    if artifact == "assembly":
        return {
            "artifact": artifact,
            "pass_number": None,
            "stage": "final emission",
            "likely_origin": "final MIPS assembly emission after the last RTL dump",
        }
    match = PASS_FILE.match(artifact)
    if not match:
        return {"artifact": artifact, "pass_number": None, "stage": "unknown", "likely_origin": "unknown"}
    number = int(match.group(1))
    stage, origin = PASS_INFO.get(number, (match.group(2), f"pass {number} ({match.group(2)})"))
    return {"artifact": artifact, "pass_number": number, "stage": stage, "likely_origin": origin}


def compare_text(left: Path, right: Path, left_aliases: dict[str, str] | None = None,
                 right_aliases: dict[str, str] | None = None,
                 artifact: str = "assembly") -> dict[str, object]:
    a = left.read_text(errors="replace")
    b = right.read_text(errors="replace")
    return {
        "raw_equal": a == b,
        "normalized_equal": (
            normalize_artifact(a, artifact, left_aliases)
            == normalize_artifact(b, artifact, right_aliases)
        ),
        "left": a,
        "right": b,
    }


def compare_runs(left: Path, right: Path, function: str | None = None) -> dict[str, object]:
    left_aliases = replacement_aliases(left)
    right_aliases = replacement_aliases(right)
    if function:
        left_dumps = function_dump_files(left, function, left_aliases)
        right_dumps = function_dump_files(right, function, right_aliases)
    else:
        left_dumps = {path.name: path for path in dump_files(left)}
        right_dumps = {path.name: path for path in dump_files(right)}
    pass_rows: list[dict[str, object]] = []
    for name in sorted(set(left_dumps) | set(right_dumps),
                       key=lambda item: (int(item.split(".")[1]), item)):
        if name not in left_dumps or name not in right_dumps:
            pass_rows.append({"name": name, "status": "missing"})
            continue
        comparison = compare_text(
            left_dumps[name], right_dumps[name], left_aliases, right_aliases, name
        )
        status = (
            "same" if comparison["raw_equal"] else
            "observation-noise" if comparison["normalized_equal"] else
            "semantic-difference"
        )
        pass_rows.append({"name": name, "status": status})

    assembly_row: dict[str, object] = {"status": "missing", "changed_functions": []}
    if function:
        left_root = function_directory(left, function, left_aliases)
        right_root = function_directory(right, function, right_aliases)
        left_assembly = left_root / "final.s" if left_root else left / "functions" / function / "final.s"
        right_assembly = right_root / "final.s" if right_root else right / "functions" / function / "final.s"
    else:
        left_assembly = left / "candidate.s"
        right_assembly = right / "candidate.s"
    if left_assembly.is_file() and right_assembly.is_file():
        comparison = compare_text(
            left_assembly, right_assembly, left_aliases, right_aliases, "assembly"
        )
        status = (
            "same" if comparison["raw_equal"] else
            "observation-noise" if comparison["normalized_equal"] else
            "semantic-difference"
        )
        if function:
            changed = [] if comparison["normalized_equal"] else [function]
        else:
            left_functions = assembly_functions(comparison["left"], left_aliases)
            right_functions = assembly_functions(comparison["right"], right_aliases)
            changed = sorted(
                name for name in set(left_functions) | set(right_functions)
                if left_functions.get(name) != right_functions.get(name)
            )
        assembly_row = {"status": status, "changed_functions": changed}

    first = next(
        (row["name"] for row in pass_rows if row["status"] in {"semantic-difference", "missing"}),
        None,
    )
    if first is None and assembly_row["status"] == "semantic-difference":
        first = "assembly"
    return {
        "scope": {"function": function} if function else {"function": None},
        "passes": pass_rows,
        "assembly": assembly_row,
        "first_semantic_divergence": first,
        "classification": classify_artifact(first),
    }


def render_diff(left: Path, right: Path, artifact: str, limit: int,
                function: str | None = None) -> None:
    left_aliases = replacement_aliases(left)
    right_aliases = replacement_aliases(right)
    if function:
        left_root = function_directory(left, function, left_aliases)
        right_root = function_directory(right, function, right_aliases)
        if not left_root or not right_root:
            print(f"cannot find extracted function {function} in both probes", file=sys.stderr)
            return
        name = "final.s" if artifact == "assembly" else artifact.removeprefix("rtl.")
        a, b = left_root / name, right_root / name
    elif artifact == "assembly":
        a, b = left / "candidate.s", right / "candidate.s"
    else:
        name = artifact if artifact.startswith("rtl.") else f"rtl.{artifact}"
        a, b = left / name, right / name
    if not a.is_file() or not b.is_file():
        print(f"cannot diff missing artifact: {a} / {b}", file=sys.stderr)
        return
    lines = difflib.unified_diff(
        normalize_artifact(a.read_text(errors="replace"), artifact, left_aliases).splitlines(),
        normalize_artifact(b.read_text(errors="replace"), artifact, right_aliases).splitlines(),
        fromfile=str(a),
        tofile=str(b),
        lineterm="",
    )
    for index, line in enumerate(lines):
        if index >= limit:
            print(f"... diff truncated at {limit} lines")
            break
        print(line)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("left", type=Path)
    parser.add_argument("right", type=Path)
    parser.add_argument("--json", type=Path, help="write the comparison report")
    parser.add_argument("--function", help="compare only this function's extracted pass artifacts")
    parser.add_argument("--diff", help="show normalized diff for assembly or pass like 03.cse")
    parser.add_argument("--diff-limit", type=int, default=240)
    args = parser.parse_args()
    for directory in (args.left, args.right):
        if not directory.is_dir():
            parser.error(f"probe directory not found: {directory}")

    report = compare_runs(args.left, args.right, args.function)
    print(f"left        {args.left}")
    print(f"right       {args.right}")
    if args.function:
        print(f"function    {args.function}")
    print("passes")
    for row in report["passes"]:
        print(f"  {row['name']:18} {row['status']}")
    assembly = report["assembly"]
    print(f"assembly    {assembly['status']}")
    changed = assembly["changed_functions"]
    if changed:
        print(f"functions   {len(changed)} changed: {', '.join(changed[:20])}")
    print(f"first       {report['first_semantic_divergence'] or 'none'}")
    if report["classification"]:
        classification = report["classification"]
        print(f"stage       {classification['stage']}")
        print(f"origin      {classification['likely_origin']}")

    if args.json:
        args.json.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
        print(f"wrote       {args.json}")
    if args.diff:
        print()
        artifact = args.diff
        if artifact == "first":
            artifact = report["first_semantic_divergence"] or "assembly"
        render_diff(args.left, args.right, artifact, args.diff_limit, args.function)
    return 1 if report["first_semantic_divergence"] else 0


if __name__ == "__main__":
    sys.exit(main())

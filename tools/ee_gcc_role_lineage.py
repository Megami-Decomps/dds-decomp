#!/usr/bin/env python3
"""Bounded source-role lineage through one unchanged EE GCC candidate's RTL.

This is same-compilation UID evidence, not a reconstruction of retail RTL.
Deleted UIDs are never silently mapped to a similar-looking replacement.
"""
from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path
import re

from ee_gcc_delay_slots import balanced_form
from ee_gcc_probe import extract_dump_function

COMPILER = "d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1"
REQUIRED = ("00.rtl", "02.jump", "03.cse", "08.gcse", "17.sched",
            "19.lreg", "20.greg", "25.sched2", "27.jump2")
NODE = re.compile(r"(?m)^\((insn|jump_insn|call_insn|code_label|note|barrier)[^\s()]*\s+(\d+)\s+(-?\d+)\s+(-?\d+)\s*")
QUOTE = re.compile(r'"(?:\\.|[^"\\])*"')
EXECUTABLE = {"insn", "jump_insn", "call_insn"}


def digest(data):
    return hashlib.sha256(data).hexdigest()


def compact(value):
    """Collapse whitespace outside strings; asm string contents remain exact."""
    parts, cursor = [], 0
    for match in QUOTE.finditer(value):
        parts.append(re.sub(r"\s+", " ", value[cursor:match.start()]))
        parts.append(match.group())
        cursor = match.end()
    parts.append(re.sub(r"\s+", " ", value[cursor:]))
    return "".join(parts).strip()


def features(pattern):
    """Bounded identity check shared with the live CSE decoder."""
    clean = QUOTE.sub('""', pattern)
    return {
        "codes": re.findall(r"\(([A-Za-z_]\w*)(?=[/:\s)\[])", clean),
        "expression_modes": [[code, mode or "VOID"] for code, mode in re.findall(
            r"\(([A-Za-z_]\w*)(?:/[A-Za-z]+)*(?::([A-Za-z0-9_]+))?(?=[\s)\[])", clean)],
        "registers": sorted([[int(n), mode or "VOID"] for mode, n in
            re.findall(r"\(reg(?:/[A-Za-z]+)*(?::([A-Za-z0-9_]+))?\s+(\d+)", clean)]),
        "constants": sorted(int(n) for n in re.findall(r"\(const_int\s+(-?\d+)", clean)),
        "symbols": sorted(json.loads(s) for s in re.findall(
            r'\(symbol_ref(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+\(?("(?:\\.|[^"\\])*")',
            pattern)),
        "targets": sorted(int(n) for n in re.findall(r"\(label_ref(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+(\d+)", clean)),
    }


def mask_c(text):
    # Preserve byte/line positions while removing comments and quoted literals.
    return re.sub(r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\'',
                  lambda m: re.sub(r"[^\n]", " ", m.group()), text, flags=re.S)


def function_span(text, function):
    masked = mask_c(text)
    matches = list(re.finditer(r"\b" + re.escape(function) + r"\s*\([^;{}]*\)\s*\{", masked))
    if len(matches) != 1:
        raise ValueError("expected one complete function definition: " + function)
    opening = masked.index("{", matches[0].start())
    depth = 1
    for index in range(opening + 1, len(masked)):
        depth += (masked[index] == "{") - (masked[index] == "}")
        if depth == 0:
            return matches[0].start(), index + 1
    raise ValueError("unterminated source function")


def role_spans(source, spec):
    if spec.get("schema") != 1 or not isinstance(spec.get("roles"), list):
        raise ValueError("roles require schema 1 and a roles array")
    begin, end = function_span(source, spec["function"])
    body = source[begin:end]
    names, result = set(), []
    for role in spec["roles"]:
        if not {"name", "begin", "end"} <= set(role) or set(role) - {"name", "begin", "end", "call_bridge"}:
            raise ValueError("role requires name/begin/end and optional call_bridge")
        name = role["name"]
        if not isinstance(name, str) or not name or name in names:
            raise ValueError("empty or duplicate role name")
        names.add(name)
        offsets = []
        for key in ("begin", "end"):
            anchor = role[key]
            if not isinstance(anchor, str) or not anchor or body.count(anchor) != 1:
                raise ValueError(f"ambiguous or missing {key} anchor for {name}")
            # A leading newline can anchor indentation exactly without making
            # the previous source line part of the selected role.
            skip = len(anchor) - len(anchor.lstrip("\n"))
            offsets.append(begin + body.index(anchor) + skip)
        if offsets[1] <= offsets[0]:
            raise ValueError("role end must follow begin: " + name)
        result.append({"name": name,
                       "first_line": source.count("\n", 0, offsets[0]) + 1,
                       "end_line_exclusive": source.count("\n", 0, offsets[1]) + 1,
                       "begin": role["begin"], "end": role["end"],
                       "start_offset": offsets[0], "end_offset_exclusive": offsets[1],
                       **({"call_bridge": role["call_bridge"]} if "call_bridge" in role else {})})
    if not result or len(result) > 16:
        raise ValueError("expected 1 to 16 roles")
    return result


def same_source(a, b):
    left, right = Path(a).parts, Path(b).parts
    width = min(3, len(left), len(right))
    return width > 0 and left[-width:] == right[-width:]



def direct_call_target(pattern):
    match = re.search(r'\(call\s+\(mem(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+'
                      r'\(symbol_ref(?:/[A-Za-z]+)*(?::[^\s()]+)?\s+\(?'
                      r'("(?:\\.|[^"\\])*")', pattern)
    return json.loads(match.group(1)) if match else None


def bridge_seeds(source, function, span, original):
    bridge = span["call_bridge"]
    if not isinstance(bridge, dict) or set(bridge) != {"begin", "end"}:
        raise ValueError("call_bridge requires begin/end")
    start, end = function_span(source, function)
    masked = mask_c(source)
    endpoints = []
    evidence = {}
    for boundary in ("begin", "end"):
        selector = bridge[boundary]
        if not isinstance(selector, dict) or set(selector) != {"symbol", "occurrence"}:
            raise ValueError("call bridge endpoint requires symbol/occurrence")
        symbol, occurrence = selector["symbol"], selector["occurrence"]
        if not isinstance(symbol, str) or not re.fullmatch(r"[A-Za-z_]\w*", symbol):
            raise ValueError("invalid source call symbol")
        if type(occurrence) is not int or not 1 <= occurrence <= 256:
            raise ValueError("call occurrence must be 1..256")
        source_calls = list(re.finditer(r"\b" + re.escape(symbol) + r"\s*\(", masked[start:end]))
        rtl_calls = [row for row in original.values()
                     if row["kind"] == "call_insn" and direct_call_target(row["pattern"]) == symbol]
        if len(source_calls) != len(rtl_calls) or occurrence > len(source_calls):
            raise ValueError("source/RTL call census mismatch for " + symbol)
        location = start + source_calls[occurrence - 1].start()
        if not span["start_offset"] <= location < span["end_offset_exclusive"]:
            raise ValueError("selected source call lies outside its exact C span")
        row = rtl_calls[occurrence - 1]
        endpoints.append(row)
        evidence[boundary] = {"symbol": symbol, "occurrence": occurrence,
                              "source_call_count": len(source_calls), "rtl_call_count": len(rtl_calls),
                              "source_line": source.count("\n", 0, location) + 1, "uid": row["uid"]}
    if endpoints[0]["order"] >= endpoints[1]["order"]:
        raise ValueError("source call bridge does not delimit a forward RTL interval")
    seeds = {uid for uid, row in original.items() if row["kind"] in EXECUTABLE
             and endpoints[0]["order"] <= row["order"] <= endpoints[1]["order"]}
    return seeds, evidence

def parse_dump(text, function, stage=None):
    body = extract_dump_function(text, function)
    if body is None:
        raise ValueError("target missing from RTL dump: " + function)
    rows, cursor = [], 0
    for match in NODE.finditer(body):
        if match.start() < cursor:
            continue
        form, cursor = balanced_form(body, match.start())
        kind, uid = match.group(1), int(match.group(2))
        remainder = form[match.end() - match.start():]
        source_note = None
        if kind == "note":
            location = re.match(r'\(?("(?:\\.|[^"\\])*")\)?\s+(\d+)\s*\)', remainder)
            if location and int(location.group(2)) > 0:
                source_note = {"file": json.loads(location.group(1)),
                               "line": int(location.group(2))}
        pattern = ""
        if kind in EXECUTABLE:
            if not remainder.startswith("("):
                raise ValueError("unrecognized executable RTL header at UID " + str(uid))
            pattern, _ = balanced_form(remainder, 0)
        rows.append({"uid": uid, "kind": kind, "previous": int(match.group(3)),
                     "next": int(match.group(4)), "pattern": compact(pattern),
                     "source_note": source_note, "features": features(pattern)})
    if not any(row["kind"] in EXECUTABLE for row in rows):
        if stage in ("17.sched", "25.sched2"):
            return {}, {"kind": "commentary_only", "complete_inventories": 0}
        raise ValueError("empty or incomplete target RTL")
    # GCC's GCSE dump may contain more than one full inventory. Only a complete
    # linked chain is an inventory; optimizer diagnostic snippets are not.
    inventories, invalid_starts = [], []
    for start, row in enumerate(rows):
        if row["previous"] != 0:
            continue
        inventory, source, prior = {}, None, None
        for offset in range(start, len(rows)):
            item = dict(rows[offset])
            if item["uid"] in inventory or (prior is not None and (
                    item["previous"] != prior["uid"] or prior["next"] != item["uid"])):
                invalid_starts.append(start)
                break
            if item["source_note"] is not None:
                source = item["source_note"]
            item["source"] = source
            item["order"] = len(inventory)
            inventory[item["uid"]] = item
            if item["next"] == 0:
                if any(r["kind"] in EXECUTABLE for r in inventory.values()):
                    inventories.append((start, offset, inventory))
                break
            prior = item
        else:
            invalid_starts.append(start)
    if not inventories:
        raise ValueError("no complete linked RTL inventory")
    last_start, last_end, result = inventories[-1]
    if any(start > last_start for start in invalid_starts):
        raise ValueError("incomplete final RTL inventory")
    return result, {"kind": "rtl", "complete_inventories": len(inventories),
                    "selected_inventory": len(inventories) - 1,
                    "selected_nodes": len(result),
                    "ignored_diagnostic_nodes": len(rows) - sum(end - start + 1
                        for start, end, _ in inventories)}

def load_probe(probe, function):
    manifest = json.loads((probe / "manifest.json").read_text())
    if (manifest.get("wrapper_returncode") != 0
            or manifest.get("cc1_succeeded") is not True
            or manifest.get("assembled") is not True):
        raise ValueError("probe is not a successful assembled compile")
    if manifest.get("compiler", {}).get("sha256") != COMPILER:
        raise ValueError("compiler hash is not the pinned EE GCC")
    if manifest.get("function") != function or manifest.get("replacements"):
        raise ValueError("function mismatch or renamed-source probe")
    source_bytes = (probe / "input.c").read_bytes()
    if digest(source_bytes) != manifest.get("compiled_source_sha256"):
        raise ValueError("source snapshot hash mismatch")
    source = source_bytes.decode("utf-8")
    if re.search(r"(?m)^\s*#\s*(?:line\s+)?\d+", source):
        raise ValueError("explicit #line source requires a reviewed line map")
    obj = manifest.get("object") or {}
    if not obj.get("path") or digest((probe / obj["path"]).read_bytes()) != obj.get("sha256"):
        raise ValueError("probe object hash mismatch")
    artifacts = {r["name"]: r for r in manifest["artifacts"]}
    paths = sorted((probe / "functions" / function).glob("[0-9][0-9].*"))
    paths = [p for p in paths if int(p.name[:2]) <= 27]
    if set(REQUIRED) - {p.name for p in paths}:
        raise ValueError("missing required dumps: " + ", ".join(sorted(set(REQUIRED) - {p.name for p in paths})))
    stages, metadata = {}, {}
    for path in paths:
        full_name = "rtl." + path.name
        raw = (probe / full_name).read_bytes()
        if digest(raw) != artifacts.get(full_name, {}).get("sha256"):
            raise ValueError("RTL artifact hash mismatch: " + full_name)
        body = extract_dump_function(raw.decode("utf-8"), function)
        if body is None or path.read_text() != body:
            raise ValueError("stale or incomplete function extraction: " + path.name)
        stages[path.name], metadata[path.name] = parse_dump(body, function, path.name)
    manifest["_lineage_stage_metadata"] = metadata
    return manifest, source, stages


def describe(record):
    if record is None:
        return None
    return {"uid": record["uid"], "kind": record["kind"],
            "pattern_sha256": digest(record["pattern"].encode()),
            "registers": record["features"]["registers"],
            "expression_modes": record["features"]["expression_modes"],
            "constants": record["features"]["constants"],
            "symbols": record["features"]["symbols"],
            "branch_targets": record["features"]["targets"]}

def role_transitions(stages, seeds, detail_limit):
    reports = []
    all_names = list(stages)
    names = [name for name in all_names if stages[name]]
    for left_name, right_name in zip(names, names[1:]):
        left, right = stages[left_name], stages[right_name]
        changes = []
        for uid in sorted(seeds):
            a, b = left.get(uid), right.get(uid)
            if a and a["kind"] not in EXECUTABLE:
                a = None
            if b and b["kind"] not in EXECUTABLE:
                b = None
            if a is None and b is None:
                continue
            if a is None:
                kind = "same_uid_reappeared"
            elif b is None:
                kind = "deleted_or_absent"
            elif a["pattern"] != b["pattern"] or a["kind"] != b["kind"]:
                kind = "pattern_changed"
            else:
                continue
            changes.append({"uid": uid, "change": kind,
                            "before": describe(a), "after": describe(b)})
        common = {uid for uid in seeds if uid in left and uid in right
                  and left[uid]["kind"] in EXECUTABLE and right[uid]["kind"] in EXECUTABLE}
        before_order = sorted(common, key=lambda uid: left[uid]["order"])
        after_order = sorted(common, key=lambda uid: right[uid]["order"])
        reports.append({"before_stage": left_name, "after_stage": right_name,
                        "unobserved_intervening_stages": [name for name in
                            all_names[all_names.index(left_name) + 1:all_names.index(right_name)]
                            if not stages[name]],
                        "change_count": len(changes), "changes": changes[:detail_limit],
                        "omitted_changes": max(0, len(changes) - detail_limit),
                        "surviving_uid_order_changed": before_order != after_order,
                        "order_before": before_order if before_order != after_order else [],
                        "order_after": after_order if before_order != after_order else []})
    return reports


def analyze(probe, spec, detail_limit=12, watch_limit=32):
    if not (1 <= detail_limit <= 64 and 1 <= watch_limit <= 64):
        raise ValueError("detail/watch limits must be 1..64")
    function = spec["function"]
    manifest, source, stages = load_probe(probe, function)
    spans = role_spans(source, spec)
    source_name = manifest.get("as_unit") or manifest["source"]
    original = stages["00.rtl"]
    reports, all_seeds, watch_roles = [], set(), {}
    for span in spans:
        bridge_evidence = None
        if "call_bridge" in span:
            seeds, bridge_evidence = bridge_seeds(source, function, span, original)
            origin = "reviewed_source_call_ordinal_bridge"
        else:
            seeds = {uid for uid, row in original.items()
                     if row["kind"] in EXECUTABLE and row["source"]
                     and same_source(row["source"]["file"], source_name)
                     and span["first_line"] <= row["source"]["line"] < span["end_line_exclusive"]}
            origin = "00.rtl_preceding_source_line_note"
        if not seeds or len(seeds) > 768:
            raise ValueError(f"role {span['name']} has {len(seeds)} source-linked UIDs; expected 1..768")
        all_seeds.update(seeds)
        transitions = role_transitions(stages, seeds, detail_limit)
        first = next((r for r in transitions
                      if r["change_count"] or r["surviving_uid_order_changed"]), None)
        reports.append({"role": span, "origin": origin, "bridge_evidence": bridge_evidence,
                        "seed_uids": sorted(seeds),
                        "first_observed_candidate_transformation": first["after_stage"] if first else None,
                        "transitions": transitions})
        for uid in seeds:
            watch_roles.setdefault(uid, []).append(span["name"])
    before, after = stages["02.jump"], stages["03.cse"]
    eligible = sorted(uid for uid in all_seeds if uid in before and before[uid]["kind"] in EXECUTABLE
                      and (uid not in after or after[uid]["kind"] not in EXECUTABLE
                           or before[uid]["pattern"] != after[uid]["pattern"]))
    selected = eligible[:watch_limit]
    watch = {"schema": 1, "function": function, "compiler_sha256": COMPILER,
             "source_sha256": manifest["compiled_source_sha256"],
             "probe_manifest_sha256": digest((probe / "manifest.json").read_bytes()),
             "stage": "first_cse", "input_stage": "02.jump", "output_stage": "03.cse",
             "role_origins": {row["role"]["name"]: row["origin"] for row in reports},
             "eligible_count": len(eligible), "omitted_count": len(eligible) - len(selected),
             "uids": [{"uid": uid, "roles": watch_roles[uid],
                       "source": original[uid]["source"],
                       "input_pattern_sha256": digest(before[uid]["pattern"].encode()),
                       "input_features": before[uid]["features"]} for uid in selected]}
    return {"schema": 1, "function": function, "compiler_sha256": COMPILER,
            "source_sha256": manifest["compiled_source_sha256"],
            "probe_manifest_sha256": watch["probe_manifest_sha256"],
            "stages": manifest["_lineage_stage_metadata"], "roles": reports,
            "limits": {"details_per_role_transition": detail_limit,
                       "cse_watch_uids": watch_limit},
            "interpretation": [
                "Earliest observed candidate transformation, not retail-vs-compiler divergence.",
                "Multiple complete linked RTL inventories use the final inventory.",
                "Scheduler commentary-only files do not establish a full RTL boundary.",
                "Source-note anchors identify origin; optimized source notes are not lifetime proof.",
                "Explicit call bridges bind reviewed C spans to matching direct-call ordinals in 00.rtl, after checking full symbol call counts on both sides.",
                "A call bridge selects only the inclusive endpoint-call interval, not every instruction in the surrounding C span.",
                "Same UID is tracked. A deleted UID has no inferred replacement.",
                "New UIDs are not assigned a source role without separate data-flow evidence.",
                "08.gcse and 20.greg are pass boundaries, not proof of a PRE or reload subroutine.",
                "Ordinary-output parity must be established independently before causal use."],
            "cse_watch": watch}, watch


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("probe", type=Path)
    parser.add_argument("--roles", type=Path, required=True)
    parser.add_argument("--json", type=Path, required=True)
    parser.add_argument("--watch", type=Path, required=True)
    parser.add_argument("--detail-limit", type=int, default=12)
    parser.add_argument("--watch-limit", type=int, default=32)
    args = parser.parse_args()
    try:
        report, watch = analyze(args.probe, json.loads(args.roles.read_text()),
                                args.detail_limit, args.watch_limit)
        for path, data in ((args.json, report), (args.watch, watch)):
            if path.exists():
                raise ValueError("output already exists: " + str(path))
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n")
    except (OSError, ValueError, KeyError, TypeError) as error:
        parser.exit(2, str(error) + "\n")
    for role in report["roles"]:
        print(role["role"]["name"], "seeds", len(role["seed_uids"]), "first",
              role["first_observed_candidate_transformation"] or "unchanged")
        for row in role["transitions"]:
            if row["change_count"] or row["surviving_uid_order_changed"]:
                print(" ", row["before_stage"], "->", row["after_stage"],
                      "changed", row["change_count"],
                      "order_changed", row["surviving_uid_order_changed"])
    print("CSE watch", len(watch["uids"]), "of", watch["eligible_count"], "changed input UIDs")


if __name__ == "__main__":
    main()

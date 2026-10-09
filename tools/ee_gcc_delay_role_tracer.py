#!/usr/bin/env python3
"""Read-only, one-role delayed-branch decisions in the pinned EE GCC.

The observer runner, transport, RTX decoder, and paired artifact checks are
unchanged. This module deliberately watches only the actor-status guard and
its first position-call argument, not every branch or instruction in a unit.
Raw watches and events are private compiler diagnostics.
"""
from __future__ import annotations

import argparse
from functools import partial
import json
from pathlib import Path
import re
import struct

import ee_gcc_observe as runner
from _ee_gcc_observer import Tracer, signed
from ee_gcc_cse_role_tracer import CseRoleTracer, decoded_features
from ee_gcc_role_lineage import (COMPILER, digest, direct_call_target,
                                 function_span, mask_c, parse_dump)

# Symbols, i386 disassembly and STABS in the exact pinned image establish these
# sites. No source-line claim is made about an unavailable vendor source tree.
HOOKS = {
    0x08075764: ("target_gate", "5589e583ec285756"),
    0x08167a00: ("dbr_enter", "5589e583ec445756"),
    0x08166ee6: ("eager_choice", "85c07e7e578b55dc"),
    0x0816656c: ("fill_enter", "5589e583ec585756"),
    0x08166dd2: ("fill_exit", "8d659c5b5e5f89ec"),
    0x0816661c: ("opposite_ready", "6a008b4510508b40"),
    0x081666c8: ("references_prior_sets", "83c40c8d4dc4894d"),
    0x081666e5: ("sets_prior_sets", "83c40c85c00f85a8"),
    0x081666fc: ("sets_prior_needed", "83c40c85c00f8591"),
    0x0816679d: ("sets_opposite_needed", "83c40c85c0755057"),
    0x081667aa: ("may_trap", "83c40485c0754389"),
    0x081667e8: ("eligible_for_delay", "83c41085c0757ee9"),
    0x081d23e4: ("live_enter", "5589e581ec880100"),
    0x081d24c7: ("live_block", "833d04fc2308000f"),
    0x081d252a: ("live_cache_hit", "e9510e0000906a20"),
    0x081d25a4: ("live_recompute", "c785b0feffff3ce7"),
    0x081d3083: ("live_unknown_block", "8b4510c74004ffff"),
    0x081d29e0: ("live_scan", "8b55a489955cffff"),
    0x081d30d5: ("live_before_forward", "c645df00c645de00"),
    0x081d3380: ("live_exit", "8da56cfeffff5b5e"),
}
HOOKS = {address: (name, bytes.fromhex(raw)) for address, (name, raw) in HOOKS.items()}
DECISIONS = {"references_prior_sets", "sets_prior_sets", "sets_prior_needed",
             "sets_opposite_needed", "may_trap", "eligible_for_delay"}
GUARD = "if (actor->status.flags & 0x80000)"
CALL = "btlUnitGetPosVU"


def build_watch(probe, function="func_001FA480"):
    """Resolve a source call occurrence into exact same-compilation RTL UIDs."""
    probe = Path(probe)
    manifest = json.loads((probe / "manifest.json").read_text())
    source_bytes = (probe / "input.c").read_bytes()
    if (manifest.get("compiler", {}).get("sha256") != COMPILER
            or manifest.get("wrapper_returncode") != 0
            or manifest.get("cc1_succeeded") is not True
            or manifest.get("assembled") is not True
            or manifest.get("compiled_source_sha256") != digest(source_bytes)):
        raise ValueError("delay watch requires a successful pinned, source-verified probe")
    dumps = {}
    hashes = {}
    declared = {item["name"]: item["sha256"] for item in manifest["artifacts"]}
    for stage in ("00.rtl", "28.mach"):
        raw = (probe / ("rtl." + stage)).read_bytes()
        hashes[stage] = digest(raw)
        if declared.get("rtl." + stage) != hashes[stage]:
            raise ValueError("probe dump hash mismatch: " + stage)
        dumps[stage], _ = parse_dump(raw.decode(errors="replace"), function, stage)
    source = source_bytes.decode()
    start, end = function_span(source, function)
    body = mask_c(source)[start:end]
    if body.count(GUARD) != 1:
        raise ValueError("expected one exact actor-status source guard")
    position = start + body.index(GUARD)
    calls = list(re.finditer(r"\b" + CALL + r"\s*\(", mask_c(source)[start:end]))
    guarded = re.match(re.escape(GUARD) + r"\s*\{\s*" + CALL
                       + r"\s*\(\s*actor\s*,\s*0\s*\)\s*;",
                       mask_c(source)[position:end])
    if guarded is None:
        raise ValueError("actor guard does not immediately guard the expected position call")
    occurrence = next((i for i, call in enumerate(calls)
                       if start + call.start() > position), None)
    original_calls = [row for row in dumps["00.rtl"].values()
                      if row["kind"] == "call_insn" and direct_call_target(row["pattern"]) == CALL]
    if occurrence is None or len(calls) != len(original_calls):
        raise ValueError("source/initial-RTL position-call census mismatch")
    call_uid = original_calls[occurrence]["uid"]
    early = list(dumps["00.rtl"].values())
    late = list(dumps["28.mach"].values())
    def preceding(rows, uid):
        indices = [i for i, row in enumerate(rows) if row["uid"] == uid]
        if len(indices) != 1:
            raise ValueError("watched source call did not survive uniquely")
        return [row for row in rows[:indices[0]]
                if row["kind"] in ("insn", "jump_insn", "call_insn")]
    before = preceding(early, call_uid)
    branches = [row for row in before[-6:] if row["kind"] == "jump_insn"]
    if len(branches) != 1:
        raise ValueError("ambiguous initial actor guard branch")
    branch_uid = branches[0]["uid"]
    guard_window = before[max(0, before.index(branches[0]) - 3):before.index(branches[0])]
    if not any(524288 in row["features"]["constants"] for row in guard_window):
        raise ValueError("initial actor branch is not tied to its status-mask definition")
    current = dumps["28.mach"]
    branch = current.get(branch_uid)
    preceding_late = preceding(late, call_uid)
    if branch is None or branch["kind"] != "jump_insn" or branch not in preceding_late[-4:]:
        raise ValueError("actor branch topology changed before delayed-branch scheduling")
    between = preceding_late[preceding_late.index(branch) + 1:]
    moves = [row for row in between if re.fullmatch(
        r"\(set \(reg:SI 4(?: a0)?\) \(reg(?:/v)?:SI \d+(?: [^()]*)?\)\)", row["pattern"])]
    if len(moves) != 1 or direct_call_target(current[call_uid]["pattern"]) != CALL:
        raise ValueError("missing unique actor ABI argument move before its source call")
    targets = branch["features"]["targets"]
    if len(targets) != 1 or current.get(targets[0], {}).get("kind") != "code_label":
        raise ValueError("actor branch requires one resolved label target")
    watched = [branch, moves[0], current[call_uid]]
    # Save the unchanged input donor chain through its first control transfer.
    pointer, donor = current[targets[0]]["next"], []
    for _ in range(32):
        row = current.get(pointer)
        if row is None:
            raise ValueError("target chain is incomplete")
        if row["kind"] in ("insn", "jump_insn", "call_insn"):
            donor.append(row)
            if row["kind"] != "insn":
                break
        pointer = row["next"]
    else:
        raise ValueError("target chain exceeds the bounded watch")
    watched.extend(donor)
    result = {"schema": 1, "kind": "delay_roles", "function": function,
              "compiler_sha256": COMPILER, "stage": "delayed_branch", "input_stage": "28.mach",
              "source_sha256": digest(source_bytes), "dump_sha256": hashes,
              "source_role": {"name": "actor_status_position_call",
                              "guard": GUARD, "line": source.count("\n", 0, position) + 1,
                              "call_occurrence": occurrence + 1, "call_census": len(calls)},
              "branch_uid": branch_uid, "argument_uid": moves[0]["uid"], "call_uid": call_uid,
              "target_uid": targets[0], "target_chain_uids": [row["uid"] for row in donor],
              "uids": [{"uid": row["uid"], "kind": row["kind"],
                        "input_features": row["features"]} for row in watched]}
    return validate_watch(result, function)


def validate_watch(watch, function):
    if (watch.get("schema") != 1 or watch.get("kind") != "delay_roles"
            or watch.get("compiler_sha256") != COMPILER or watch.get("function") != function
            or watch.get("stage") != "delayed_branch" or watch.get("input_stage") != "28.mach"):
        raise ValueError("wrong delay watch schema, compiler, function, or input stage")
    if not re.fullmatch(r"[0-9a-f]{64}", watch.get("source_sha256", "")):
        raise ValueError("delay watch lacks a source hash")
    role = watch.get("source_role", {})
    if (role.get("guard") != GUARD or role.get("name") != "actor_status_position_call"
            or not isinstance(role.get("call_occurrence"), int)
            or not 1 <= role["call_occurrence"] <= role.get("call_census", 0)):
        raise ValueError("delay watch lacks a qualified source role")
    rows = watch.get("uids")
    if not isinstance(rows, list) or not 4 <= len(rows) <= 16:
        raise ValueError("delay watch must cover its branch, argument, call, and donor chain")
    uids = [row.get("uid") for row in rows]
    if any(type(uid) is not int or uid <= 0 for uid in uids) or len(set(uids)) != len(uids):
        raise ValueError("delay watch UIDs must be unique positive integers")
    ids = {row["uid"]: row for row in rows}
    for key, kind in (("branch_uid", "jump_insn"), ("argument_uid", "insn"), ("call_uid", "call_insn")):
        if ids.get(watch.get(key), {}).get("kind") != kind:
            raise ValueError("delay role has wrong or absent instruction kind: " + key)
    expected = {watch["branch_uid"], watch["argument_uid"], watch["call_uid"]}
    chain = watch.get("target_chain_uids", [])
    if (not isinstance(chain, list) or not chain or len(set(chain)) != len(chain)
            or expected.intersection(chain) or set(uids) != expected.union(chain)):
        raise ValueError("delay watch has an incomplete or unrelated donor chain")
    for row in rows:
        f = row.get("input_features")
        if not isinstance(f, dict) or set(f) != {
                "codes", "expression_modes", "registers", "constants", "symbols", "targets"}:
            raise ValueError("delay watch lacks an exact input-pattern identity")
    if ids[watch["branch_uid"]]["input_features"]["targets"] != [watch.get("target_uid")]:
        raise ValueError("delay watch target disagrees with its branch identity")
    return watch


def verify_hook_specs(compiler):
    """Offline, read-only verification against the exact installed ELF image."""
    image = Path(compiler).read_bytes()
    if digest(image) != COMPILER or image[:6] != b"\x7fELF\x01\x01":
        raise ValueError("hook verification requires the pinned little-endian ELF32 compiler")
    header = struct.unpack_from("<HHIIIIIHHHHHH", image, 16)
    segments = [struct.unpack_from("<IIIIIIII", image, header[4] + header[8] * i)
                for i in range(header[9])]
    for address, (_, expected) in HOOKS.items():
        choices = [s for s in segments if s[0] == 1 and s[2] <= address
                   and address + len(expected) <= s[2] + s[4]]
        if len(choices) != 1:
            raise ValueError("hook does not map uniquely into pinned compiler text")
        segment = choices[0]
        offset = segment[1] + address - segment[2]
        if not segment[6] & 1 or image[offset:offset + len(expected)] != expected:
            raise ValueError("offline delay hook signature mismatch")
    return {"compiler_verified": True, "static_hooks_verified": len(HOOKS)}


class DelayRoleTracer(Tracer):
    # Reuse the complete bounded decoder, never the base's clipped display RTX.
    rtx = CseRoleTracer.rtx
    instruction = CseRoleTracer.instruction

    def __init__(self, *args, watch, **kwargs):
        super().__init__(*args, **kwargs)
        self.watch = validate_watch(watch, self.function)
        self.wanted = {row["uid"]: row for row in watch["uids"]}
        self.found = self.completed = False
        self.dynamic = {}
        self.checked_dynamic_hooks = set()
        self.fill = None
        self.live = []
        self.fill_count = self.live_count = self.trial_count = self.eager_count = 0
        self.scan_count = 0

    def log(self, event, **kwargs):
        if self.seq >= 12000:
            raise RuntimeError("bounded delay trace event limit exceeded")
        super().log(event, **kwargs)

    def bp(self, address, name):
        if address not in HOOKS and address not in self.dynamic:
            raise RuntimeError("unknown delay hook " + hex(address))
        if address in self.bps and self.bps[address] != name:
            raise RuntimeError("conflicting delay hook roles")
        if address not in self.bps:
            self.r.bp(address, True)
        self.bps[address] = name

    def verify_hook(self, address):
        static = address in HOOKS
        expected = HOOKS[address][1] if static else self.dynamic[address]
        checked = self.checked_hooks if static else self.checked_dynamic_hooks
        if address not in checked:
            if self.r.mem(address, 8) != expected:
                raise RuntimeError("delay hook opcode mismatch " + hex(address))
            checked.add(address)

    def dynamic_return(self, address, name):
        if address in HOOKS:
            raise RuntimeError("delay return collides with static hook")
        self.dynamic.setdefault(address, self.r.mem(address, 8))
        self.bp(address, name)

    def uid(self, pointer):
        return self.r.u32(pointer + 4) if pointer else None

    def resources(self, pointer):
        data = self.r.mem(pointer, 20)
        return {"memory": bool(data[0]), "unch_memory": bool(data[1]),
                "volatil": bool(data[2]), "cc": bool(data[3]),
                "registers": [i for i in range(128) if data[4 + i // 8] & (1 << (i % 8))]}

    def selected_hooks(self, enabled):
        for address, (name, _) in HOOKS.items():
            if name in DECISIONS or name.startswith("live_") or name == "opposite_ready":
                self.bp(address, name) if enabled else self.remove(address)

    def verify_input(self, first):
        found, seen = {}, set()
        while first:
            if first in seen or len(seen) >= 12000:
                raise RuntimeError("cyclic or oversized delayed-branch input chain")
            seen.add(first)
            header = self.r.mem(first, 24)
            uid = struct.unpack_from("<I", header, 4)[0]
            if uid in self.wanted:
                if uid in found:
                    raise RuntimeError("duplicate watched UID in live input")
                insn = self.instruction(first)
                expected = self.wanted[uid]
                if (insn["kind"] != expected["kind"]
                        or decoded_features(insn["pattern"]) != expected["input_features"]):
                    raise RuntimeError("live delay input differs from frozen role identity")
                found[uid] = first
            first = struct.unpack_from("<I", header, 20)[0]
        if set(found) != set(self.wanted):
            raise RuntimeError("missing watched delayed-branch input UID")
        self.pointers = found
        self.log("input_verified", watched_uids=sorted(found), input_nodes=len(seen))

    def handle(self, name, r):
        pc, frame = r["eip"], r["ebp"]
        if name == "target_gate":
            if self.fn() == self.function:
                self.found = True
                self.remove(pc)
                self.bp(0x08167a00, "dbr_enter")
            return
        if name == "dbr_enter":
            if self.fn() != self.function:
                raise RuntimeError("delayed-branch target function changed")
            self.active = "delayed_branch"
            self.remove(pc)
            self.verify_input(self.args(r, 1)[0])
            self.dynamic_return(self.r.u32(r["esp"]), "dbr_exit")
            for address in (0x08166ee6, 0x0816656c, 0x08166dd2):
                self.bp(address, HOOKS[address][0])
            self.log("dbr_enter", function=self.function, branch_uid=self.watch["branch_uid"],
                     argument_uid=self.watch["argument_uid"], source_role=self.watch["source_role"])
            return
        if name == "dbr_exit":
            if self.fill or self.live:
                raise RuntimeError("unfinished selected delay/live calculation")
            coverage = (self.eager_count > 0 and self.fill_count > 0
                        and self.live_count > 0 and self.trial_count > 0)
            self.log("dbr_exit", coverage_complete=coverage, eager_choices=self.eager_count,
                     fill_calls=self.fill_count, live_calls=self.live_count,
                     argument_trials=self.trial_count, live_scan_steps=self.scan_count)
            if not coverage:
                raise RuntimeError("missing watched argument or opposing-live-set decision coverage")
            self.completed = True
            for address in list(self.bps):
                self.remove(address)
            return
        if name == "eager_choice":
            if self.uid(r["esi"]) == self.watch["branch_uid"]:
                self.eager_count += 1
                self.log(name, prediction=signed(r["eax"]), own_target=signed(r["ebx"]),
                         own_fallthrough=signed(self.r.u32(frame - 0x1c)),
                         target_uid=self.uid(self.r.u32(frame - 0x14)),
                         fallthrough_uid=self.uid(self.r.u32(frame - 0x18)))
            return
        if name == "fill_enter":
            args = self.args(r, 10)
            if self.uid(args[0]) != self.watch["branch_uid"]:
                return
            if self.fill or self.fill_count >= 16:
                raise RuntimeError("nested or excessive selected delay fill")
            self.fill_count += 1
            self.fill = {"call_id": self.fill_count, "frame": r["esp"] - 4,
                         "thread_if_true": signed(args[5])}
            self.selected_hooks(True)
            self.log(name, call_id=self.fill_count, thread_uid=self.uid(args[2]),
                     opposite_uid=self.uid(args[3]), likely=signed(args[4]),
                     thread_if_true=signed(args[5]), own_thread=signed(args[6]),
                     slots_to_fill=signed(args[7]), slots_filled=self.r.u32(args[8]),
                     has_delay_list=bool(args[9]))
            return
        if name == "fill_exit":
            if not self.fill or frame != self.fill["frame"]:
                return
            if self.live:
                raise RuntimeError("selected fill exited during live calculation")
            self.log(name, call_id=self.fill["call_id"], has_delay_list=bool(r["eax"]),
                     must_annul=bool(self.r.u32(frame - 0x44)),
                     slots_filled=self.r.u32(self.r.u32(frame + 0x28)))
            self.selected_hooks(False)
            self.fill = None
            return
        if not self.fill:
            raise RuntimeError("ungated delayed-branch decision hook")
        fill_id = self.fill["call_id"]
        if name == "opposite_ready":
            self.log(name, call_id=fill_id, resources=self.resources(frame - 0x14))
            return
        if name in DECISIONS:
            if self.uid(r["esi"]) == self.watch["argument_uid"]:
                self.trial_count += name == "references_prior_sets"
                self.log("argument_decision", call_id=fill_id, check=name,
                         result=signed(r["eax"]), thread_if_true=self.fill["thread_if_true"],
                         argument=self.instruction(r["esi"]),
                         opposite_needed=self.resources(frame - 0x14))
            return
        if name == "live_enter":
            if len(self.live) >= 16 or self.live_count >= 128:
                raise RuntimeError("opposing-live calculation bound exceeded")
            args = self.args(r, 3)
            self.live_count += 1
            state = {"live_id": self.live_count, "frame": r["esp"] - 4,
                     "target_uid": self.uid(args[1]), "resources_pointer": args[2]}
            parent = self.live[-1]["live_id"] if self.live else None
            self.live.append(state)
            self.log(name, call_id=fill_id, live_id=self.live_count,
                     parent_live_id=parent, target_uid=state["target_uid"])
            return
        if not self.live or self.live[-1]["frame"] != frame:
            raise RuntimeError("live-resource frame stack mismatch")
        state = self.live[-1]
        fields = {"call_id": fill_id, "live_id": state["live_id"]}
        if name == "live_block":
            self.log(name, **fields, basic_block=signed(self.r.u32(frame - 0x54)),
                     cache_entry_present=bool(self.r.u32(frame - 0x58)))
        elif name in {"live_cache_hit", "live_recompute", "live_unknown_block"}:
            self.log(name, **fields)
        elif name == "live_scan":
            self.scan_count += 1
            if self.scan_count > 8000:
                raise RuntimeError("opposing-live instruction scan bound exceeded")
            pointer = self.r.u32(frame - 0x5c)
            self.log(name, **fields, scanning_uid=self.uid(pointer),
                     current_live=self.bitset(0x0824e72c),
                     pending_dead=self.bitset(0x0824e73c))
        elif name == "live_before_forward":
            self.log(name, **fields, resources=self.resources(state["resources_pointer"]))
        elif name == "live_exit":
            self.log(name, **fields, resources=self.resources(state["resources_pointer"]))
            self.live.pop()
        else:
            raise RuntimeError("unknown delayed-branch hook " + name)

    def run(self):
        self.r.cmd("qSupported")
        self.r.cmd("?")
        self.bp(0x08075764, "target_gate")
        while True:
            stop = self.r.cmd("c")
            if stop.startswith(("W", "X")):
                self.log("exit", status=stop)
                break
            if not stop.startswith(("T05", "S05")):
                raise RuntimeError("unexpected delay observer stop " + stop)
            registers = self.r.regs()
            pc = registers["eip"]
            name = self.bps.get(pc)
            if name is None:
                raise RuntimeError("unexpected delay observer trap " + hex(pc))
            self.verify_hook(pc)
            self.handle(name, registers)
            retained = self.bps.get(pc)
            if retained:
                self.r.bp(pc, False)
            step = self.r.cmd("s")
            if not step.startswith(("T05", "S05")):
                raise RuntimeError("unexpected delay observer single-step " + step)
            if retained:
                self.r.bp(pc, True)
        self.f.close()
        if not (self.found and self.completed):
            raise RuntimeError("missing target delayed-branch completion gate")


def summarize_decisions(archive):
    """Return only checked role decisions, never raw patterns, pointers or words."""
    archive = Path(archive)
    receipt = json.loads((archive / "equivalence.json").read_text())
    equality = receipt.get("artifacts_equal")
    if (receipt.get("mode") != "delay_roles" or receipt.get("all_equal") is not True
            or not isinstance(equality, dict) or not equality
            or not all(value is True for value in equality.values())):
        raise ValueError("delay summary requires a successful all-equal pair")
    events = [json.loads(line) for line in (archive / "events.jsonl").read_text().splitlines() if line]
    if (len(events) != receipt.get("events") or not events
            or [e.get("seq") for e in events] != list(range(1, len(events) + 1))
            or events[-1].get("event") != "exit"):
        raise ValueError("delay event log is incomplete or reordered")
    finishes = [e for e in events if e.get("event") == "dbr_exit"]
    if (len([e for e in events if e.get("event") == "dbr_enter"]) != 1
            or len([e for e in events if e.get("event") == "input_verified"]) != 1
            or len(finishes) != 1 or finishes[0].get("coverage_complete") is not True):
        raise ValueError("delay log lacks complete role/input/completion gates")
    calls, live, decisions, returned, provenance = {}, {}, [], [], []
    live_details, active_live = {}, []
    def observe_transition(detail, key, current, observed_at_uid=None):
        prior = detail.get("_previous_" + key)
        if prior is not None and prior[1] != current and "first_" + key + "_transition" not in detail:
            detail["first_" + key + "_transition"] = {
                "after_scanning_uid": prior[0], "observed_at_uid": observed_at_uid,
                "before": prior[1], "after": current}
        detail["_previous_" + key] = (observed_at_uid, current)
    for e in events:
        kind = e["event"]
        if kind == "fill_enter":
            if e["call_id"] in calls:
                raise ValueError("duplicate selected fill")
            calls[e["call_id"]] = e
        elif kind == "fill_exit":
            if e["call_id"] not in calls:
                raise ValueError("fill return without entry")
            entered = calls.pop(e["call_id"])
            returned.append({"thread": "target" if entered["thread_if_true"] else "fallthrough",
                             "owned": bool(entered["own_thread"]),
                             "filled": e["has_delay_list"], "annulled": e["must_annul"]})
        elif kind == "live_enter":
            if e["live_id"] in live_details or e["call_id"] not in calls:
                raise ValueError("duplicate or unowned live calculation")
            if e["parent_live_id"] != (active_live[-1] if active_live else None):
                raise ValueError("live calculation nesting mismatch")
            live[e["live_id"]] = e
            active_live.append(e["live_id"])
            live_details[e["live_id"]] = {
                "live_id": e["live_id"], "parent_live_id": e["parent_live_id"],
                "call_id": e["call_id"], "target_uid": e["target_uid"],
                "routes": [], "scanned_instructions": 0}
        elif kind in {"live_block", "live_cache_hit", "live_recompute", "live_unknown_block",
                      "live_scan", "live_before_forward"}:
            if not active_live or active_live[-1] != e["live_id"]:
                raise ValueError("live provenance outside its active calculation")
            detail = live_details[e["live_id"]]
            if kind == "live_block":
                detail["basic_block"] = e["basic_block"]
                detail["cache_entry_present"] = e["cache_entry_present"]
            elif kind in {"live_cache_hit", "live_recompute", "live_unknown_block"}:
                detail["routes"].append(kind.removeprefix("live_"))
            elif kind == "live_scan":
                current = 4 in e["current_live"]
                detail.setdefault("initial_argument_register_live", current)
                detail["scanned_instructions"] += 1
                observe_transition(detail, "argument_live", current, e["scanning_uid"])
                observe_transition(detail, "argument_pending_dead", 4 in e["pending_dead"],
                                   e["scanning_uid"])
            else:
                current = 4 in e["resources"]["registers"]
                detail["argument_live_before_forward_scan"] = current
                observe_transition(detail, "argument_live", current)
        elif kind == "live_exit":
            if (e["live_id"] not in live or not active_live
                    or active_live[-1] != e["live_id"]):
                raise ValueError("live return without entry")
            active_live.pop()
            entry = live.pop(e["live_id"])
            live_details[e["live_id"]]["returned_argument_register_live"] = 4 in e["resources"]["registers"]
            if entry["parent_live_id"] is None:
                provenance.append({"call_id": e["call_id"],
                                   "argument_register_live": 4 in e["resources"]["registers"]})
        elif kind == "argument_decision":
            decisions.append({"check": e["check"], "result": e["result"],
                              "argument_register_live_on_opposite_path":
                                  4 in e["opposite_needed"]["registers"]})
    if calls or live or active_live or not decisions or not provenance:
        raise ValueError("incomplete selected decision or live-mask coverage")
    clean_live = [{key: value for key, value in detail.items() if not key.startswith("_")}
                  for detail in live_details.values()]
    return {"schema": 1, "kind": "bounded_delay_decisions", "function": receipt["function"],
            "artifacts_all_equal": True, "coverage_complete": True,
            "eager_choices": [{"prediction": e["prediction"],
                                "owns_fallthrough": bool(e["own_fallthrough"])}
                               for e in events if e["event"] == "eager_choice"],
            "argument_decisions": decisions, "fill_outcomes": returned,
            "opposing_live_results": provenance,
            "live_provenance": clean_live[:32],
            "live_provenance_count": len(clean_live),
            "omitted_live_provenance": max(0, len(clean_live) - 32),
            "cache_hits": sum(e["event"] == "live_cache_hit" for e in events),
            "unknown_block_fallbacks": sum(e["event"] == "live_unknown_block" for e in events),
            "interpretation": "Observed candidate decisions only; no native RTL or original source inferred."}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", required=True, type=Path)
    parser.add_argument("--watch", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()
    try:
        case = runner.load_case(args.config)
        watch = validate_watch(json.loads(args.watch.read_text()), case.function)
        source_args = [a for a in case.command[case.command.index(str(case.compiler)) + 1:]
                       if not a.startswith("-") and Path(a).suffix in (".c", ".i")]
        if len(source_args) != 1:
            raise ValueError("expected one explicit compiler source")
        source = Path(source_args[0])
        source = source if source.is_absolute() else case.cwd / source
        if case.inputs.get(source.resolve()) != watch["source_sha256"]:
            raise ValueError("compiler source does not match delay watch")
        modules = [Path(__file__).resolve(), args.watch.resolve(), *[
            Path(__file__).with_name(name).resolve() for name in (
                "ee_gcc_cse_role_tracer.py", "ee_gcc_role_lineage.py", "ee_gcc_delay_slots.py",
                "ee_gcc_probe.py", "ee_gcc_observe.py", "_ee_gcc_observer.py",
                "ee_gcc_qemu_loopback.py", "ee_gcc_unix_capability.py")]]
        for path in modules:
            if case.inputs.get(path) != digest(path.read_bytes()):
                raise ValueError("required delay observer input missing or unhashed: " + str(path))
        case.verify()
        if args.dry_run:
            case.inventory(required=False)
            if args.output.exists():
                raise ValueError("output already exists")
            print(json.dumps({"validated": True, "executed": False, "watch_roles": 1}))
            return
        original = runner.OperandTracer
        runner.OperandTracer = partial(DelayRoleTracer, watch=watch)
        try:
            receipt = runner.run(case, args.output, "delay_roles")
        finally:
            runner.OperandTracer = original
        print(json.dumps({"all_equal": receipt["all_equal"], "events": receipt["events"],
                          "output": str(args.output.resolve())}))
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        parser.exit(2, str(error) + "\n")


if __name__ == "__main__":
    main()

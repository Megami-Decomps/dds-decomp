#!/usr/bin/env python3
"""Read-only, source-role-selected first-CSE decisions in the pinned EE GCC.

Uses the existing observer's paired runner and unchanged protocol/transport.
No compiler bytes, guest registers, optimizer costs, or source are changed.
"""
from __future__ import annotations

import argparse
from functools import partial
import json
from pathlib import Path
import struct

import ee_gcc_observe as runner
from _ee_gcc_observer import Tracer, signed
from ee_gcc_role_lineage import COMPILER, digest

# The already qualified private direct-guard CSE hooks, not guessed addresses.
HOOKS = {
    0x08075764: ("target_gate", "5589e583ec285756"),
    0x0810905c: ("cse_main", "5589e583ec705756"),
    0x08105904: ("cse_insn", "5589e581ecb40000"),
    0x08101a64: ("helper_related", "5589e583ec085756"),
    0x080ff204: ("helper_notreg_cost", "5589e556538b5d08"),
    0x080fefb8: ("helper_approx_reg_cost", "5589e583ec385756"),
    0x08175a4c: ("helper_validate_change", "5589e55756538b45"),
    0x08175bb0: ("helper_apply_change_group", "5589e583ec0c5756"),
    0x08101acd: ("related_candidate", "8b06508b550852e8"),
    0x08101af6: ("related_selected", "85db74c885db7435"),
    0x08101b27: ("related_delta", "57568b0350e81bf6"),
}
HOOKS = {address: (name, bytes.fromhex(raw)) for address, (name, raw) in HOOKS.items()}


def validate_watch(watch, function):
    if (watch.get("schema") != 1 or watch.get("function") != function
            or watch.get("compiler_sha256") != COMPILER
            or watch.get("stage") != "first_cse"
            or watch.get("input_stage") != "02.jump"):
        raise ValueError("wrong watch schema, compiler, function, or CSE pass")
    rows = watch.get("uids")
    if not isinstance(rows, list) or not 1 <= len(rows) <= 64:
        raise ValueError("watch must contain 1 to 64 current input UIDs")
    uids = [row.get("uid") for row in rows]
    if any(type(uid) is not int or uid <= 0 for uid in uids) or len(set(uids)) != len(uids):
        raise ValueError("watch UIDs must be unique positive integers")
    for row in rows:
        if not row.get("roles") or not isinstance(row.get("input_features"), dict):
            raise ValueError("watch UID lacks source role or input identity")
    return {row["uid"]: row for row in rows}


def decoded_features(node):
    result = {"codes": [], "expression_modes": [], "registers": [], "constants": [], "symbols": [], "targets": []}
    def visit(value):
        if isinstance(value, list):
            for child in value:
                visit(child)
        elif isinstance(value, dict):
            code = value["code"]
            result["codes"].append(code)
            result["expression_modes"].append([code, value.get("mode", "VOID")])
            if code == "reg":
                result["registers"].append([value["regno"], value["mode"]])
            elif code == "const_int":
                result["constants"].append(value["value"])
            elif code == "symbol_ref":
                result["symbols"].append(value["symbol"])
            elif code == "label_ref":
                result["targets"].append(value["uid"])
            else:
                visit(value.get("fields", []))
    visit(node)
    for key in result:
        if key not in ("codes", "expression_modes"):
            result[key].sort()
    return result


class CseRoleTracer(Tracer):
    def __init__(self, *args, watch, **kwargs):
        super().__init__(*args, **kwargs)
        self.wanted = validate_watch(watch, self.function)
        self.watch = watch
        self.found = self.completed = False
        self.current = None
        self.calls = []
        self.dynamic = {}
        self.checked_dynamic_hooks = set()
        self.visits = {}
        self.call_count = 0

    def log(self, event, **kwargs):
        if self.seq >= 25000:
            raise RuntimeError("bounded trace event limit exceeded; narrow the watch")
        super().log(event, **kwargs)

    def bp(self, address, name):
        if address not in HOOKS and address not in self.dynamic:
            raise RuntimeError("unknown hook " + hex(address))
        if address in self.bps and self.bps[address] != name:
            raise RuntimeError("conflicting breakpoint roles at " + hex(address))
        if address not in self.bps:
            self.r.bp(address, True)
        self.bps[address] = name

    def verify_hook(self, address):
        expected = HOOKS[address][1] if address in HOOKS else self.dynamic[address]
        checked = self.checked_hooks if address in HOOKS else self.checked_dynamic_hooks
        if address not in checked:
            if self.r.mem(address, len(expected)) != expected:
                raise RuntimeError("hook opcode mismatch " + hex(address))
            checked.add(address)

    def dynamic_return(self, address, name):
        if address in HOOKS:
            raise RuntimeError("return PC collides with static hook")
        if address not in self.dynamic:
            self.dynamic[address] = self.r.mem(address, 8)
        self.bp(address, name)

    def rtx(self, pointer, depth=20):
        """Complete bounded decoder; fail instead of silently clipping operands."""
        budget = [4096]
        def decode(p, remaining):
            if not p:
                return None
            budget[0] -= 1
            if remaining <= 0 or budget[0] < 0:
                raise RuntimeError("RTL decode bound exceeded")
            code, mode, flags = struct.unpack("<HBB", self.r.mem(p, 4))
            if code >= 256:
                raise RuntimeError("invalid pinned RTX code")
            key = ("rtx", code)
            if key not in self.static:
                self.static[key] = (self.r.cstr(self.r.u32(136367760 + 4 * code)),
                                    self.r.cstr(self.r.u32(136371812 + 4 * code)))
            name, fmt = self.static[key]
            out = {"code": name, "mode": self.mode(mode)}
            if name == "reg":
                out["regno"] = self.r.u32(p + 4)
            elif name == "const_int":
                out["value"] = struct.unpack("<q", self.r.mem(p + 4, 8))[0]
            elif name == "symbol_ref":
                out["symbol"] = self.r.cstr(self.r.u32(p + 4))
            elif name == "label_ref":
                target = self.r.u32(p + 4)
                if not target:
                    raise RuntimeError("null label reference")
                out["uid"] = self.r.u32(target + 4)
            else:
                fields = []
                for index, ch in enumerate(fmt):
                    address = p + 4 + 8 * index
                    if ch in "eu":
                        fields.append(decode(self.r.u32(address), remaining - 1))
                    elif ch == "i":
                        fields.append(signed(self.r.u32(address)))
                    elif ch == "w":
                        fields.append(struct.unpack("<q", self.r.mem(address, 8))[0])
                    elif ch == "s":
                        fields.append(self.r.cstr(self.r.u32(address)))
                    elif ch in "EV":
                        vector = self.r.u32(address)
                        count = self.r.u32(vector) if vector else 0
                        if count > 256:
                            raise RuntimeError("RTL vector bound exceeded")
                        fields.append([decode(self.r.u32(vector + 4 + 4 * j), remaining - 1)
                                       for j in range(count)])
                    elif ch == "0":
                        fields.append(None)
                    else:
                        raise RuntimeError("unsupported RTX format field " + ch)
                out["fields"] = fields
            return out
        return decode(pointer, depth)

    def instruction(self, pointer):
        code = struct.unpack("<H", self.r.mem(pointer, 2))[0]
        if code >= 256:
            raise RuntimeError("invalid instruction RTX code")
        name = self.r.cstr(self.r.u32(136367760 + 4 * code))
        return {"kind": name, "uid": self.r.u32(pointer + 4),
                "pattern": self.rtx(self.r.u32(pointer + 28))
                if name in ("insn", "jump_insn", "call_insn") else None}

    def table(self, pointer):
        if not pointer:
            return None
        return {"pointer": hex(pointer), "expression": self.rtx(self.r.u32(pointer)),
                "next_same_value": hex(self.r.u32(pointer + 16)),
                "first_same_value": hex(self.r.u32(pointer + 24)),
                "related_value": hex(self.r.u32(pointer + 28))}

    def helper_hooks(self, on):
        for address, (name, _) in HOOKS.items():
            if name.startswith(("helper_", "related_")):
                self.bp(address, name) if on else self.remove(address)

    def handle(self, name, registers):
        pc = registers["eip"]
        if name == "target_gate":
            if self.fn() == self.function:
                self.found = True
                self.remove(pc)
                self.bp(0x0810905c, "cse_main")
            return
        if name == "cse_main":
            if self.fn() != self.function:
                raise RuntimeError("CSE target function changed")
            self.active = "first_cse"
            self.remove(pc)
            self.dynamic_return(self.r.u32(registers["esp"]), "cse_exit")
            self.bp(0x08105904, "cse_insn")
            self.log("cse_enter", function=self.function,
                     watch_source_sha256=self.watch["source_sha256"],
                     requested_uids=sorted(self.wanted))
            return
        if name == "cse_exit":
            if self.current or self.calls:
                raise RuntimeError("unfinished selected CSE instruction/helper")
            missing = sorted(set(self.wanted) - set(self.visits))
            self.log("cse_exit", visits=self.visits, missing_uids=missing,
                     verified_dynamic_return_sites=[hex(a) for a in sorted(self.checked_dynamic_hooks)])
            if missing:
                raise RuntimeError("watched UIDs were not visited: " + str(missing))
            self.completed = True
            for address in list(self.bps):
                self.remove(address)
            return
        if name == "cse_insn":
            pointer = self.r.u32(registers["esp"] + 4)
            uid = self.r.u32(pointer + 4)
            if uid in self.wanted:
                if self.current:
                    raise RuntimeError("nested cse_insn")
                state = self.instruction(pointer)
                identity = decoded_features(state["pattern"])
                if uid not in self.visits and identity != self.wanted[uid]["input_features"]:
                    self.log("input_identity_mismatch", uid=uid,
                             expected=self.wanted[uid]["input_features"], actual=identity)
                    raise RuntimeError("live CSE UID does not match frozen input identity")
                self.visits[uid] = self.visits.get(uid, 0) + 1
                self.current = (pointer, uid)
                self.log("insn_before", uid=uid, visit=self.visits[uid],
                         roles=self.wanted[uid]["roles"], source=self.wanted[uid].get("source"),
                         state=state)
                self.dynamic_return(self.r.u32(registers["esp"]), "insn_exit")
                self.helper_hooks(True)
            return
        if name == "insn_exit":
            if self.calls or not self.current:
                raise RuntimeError("unfinished helper or missing selected instruction")
            pointer, uid = self.current
            self.log("insn_after", uid=uid, state=self.instruction(pointer))
            self.helper_hooks(False)
            self.remove(pc)
            self.current = None
            return
        if name.startswith("helper_") and name != "helper_return":
            if not self.current:
                raise RuntimeError("ungated helper")
            what = name[7:]
            # Read only each helper's actual required arguments.
            count = {"related": 2, "notreg_cost": 1, "approx_reg_cost": 1,
                     "validate_change": 4, "apply_change_group": 0}[what]
            args = self.args(registers, count)
            self.call_count += 1
            context = {"name": what, "return_pc": self.r.u32(registers["esp"]),
                       "uid": self.current[1], "call_id": self.call_count,
                       "parent_call_id": self.calls[-1]["call_id"] if self.calls else None}
            if what == "related":
                context.update(input=self.rtx(args[0]), table=self.table(args[1]))
            elif what in ("notreg_cost", "approx_reg_cost"):
                context["input"] = self.rtx(args[0])
            elif what == "validate_change":
                context.update(object_pointer=hex(args[0]), location_pointer=hex(args[1]),
                               old=self.rtx(self.r.u32(args[1])), new=self.rtx(args[2]),
                               in_group=args[3], location=args[1])
            self.calls.append(context)
            self.dynamic_return(context["return_pc"], "helper_return")
            self.log("call_enter", **{k: v for k, v in context.items() if k != "location"})
            return
        if name == "helper_return":
            if not self.calls:
                raise RuntimeError("empty helper return stack")
            context = self.calls.pop()
            if context["return_pc"] != pc:
                raise RuntimeError("helper return stack mismatch")
            result = (self.rtx(registers["eax"]) if context["name"] == "related"
                      else signed(registers["eax"]))
            extra = {}
            if "location" in context:
                extra["location_after"] = self.rtx(self.r.u32(context["location"]))
            self.log("call_return", uid=context["uid"], name=context["name"],
                     call_id=context["call_id"], result=result, **extra)
            if not any(c["return_pc"] == pc for c in self.calls):
                self.remove(pc)
            return
        if not self.current:
            raise RuntimeError("ungated decision hook")
        uid = self.current[1]
        if name == "related_candidate":
            self.log(name, uid=uid, wanted=self.rtx(self.r.u32(registers["ebp"] + 8)),
                     candidate=self.table(registers["esi"]))
        elif name == "related_selected":
            self.log(name, uid=uid, candidate=self.table(registers["esi"]),
                     register=self.table(registers["ebx"]) if registers["ebx"] else None)
        elif name == "related_delta":
            delta = struct.unpack("<q", struct.pack("<II", registers["esi"], registers["edi"]))[0]
            self.log(name, uid=uid, register=self.rtx(self.r.u32(registers["ebx"])), delta=delta)
        else:
            raise RuntimeError(name)

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
                raise RuntimeError("unexpected stop " + stop)
            registers = self.r.regs()
            pc = registers["eip"]
            name = self.bps.get(pc)
            if not name:
                raise RuntimeError("unexpected trap " + hex(pc))
            self.verify_hook(pc)
            self.handle(name, registers)
            retained = self.bps.get(pc)
            if retained:
                self.r.bp(pc, False)
            step = self.r.cmd("s")
            if not step.startswith(("T05", "S05")):
                raise RuntimeError("unexpected single-step " + step)
            if retained:
                self.r.bp(pc, True)
        self.f.close()
        if not (self.found and self.completed):
            raise RuntimeError("missing target first-CSE completion gate")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", required=True, type=Path)
    parser.add_argument("--watch", required=True, type=Path)
    parser.add_argument("--output", required=True, type=Path)
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()
    try:
        watch = json.loads(args.watch.read_text())
        case = runner.load_case(args.config)
        validate_watch(watch, case.function)
        source_args = [arg for arg in case.command[case.command.index(str(case.compiler)) + 1:]
                       if not arg.startswith("-") and Path(arg).suffix in (".c", ".i")]
        if len(source_args) != 1:
            raise ValueError("expected one explicit C/preprocessed source argument")
        source_path = Path(source_args[0])
        if not source_path.is_absolute():
            source_path = case.cwd / source_path
        if case.inputs.get(source_path.resolve()) != watch["source_sha256"]:
            raise ValueError("actual compiler source argument does not match the watched source hash")
        # All executed observer Python and the UID plan belong in the frozen closure.
        paths = [args.watch.resolve(), Path(__file__).resolve(),
                 Path(__file__).with_name("ee_gcc_role_lineage.py").resolve(),
                 Path(__file__).with_name("ee_gcc_delay_slots.py").resolve(),
                 Path(__file__).with_name("ee_gcc_probe.py").resolve(),
                 Path(__file__).with_name("ee_gcc_observe.py").resolve(),
                 Path(__file__).with_name("_ee_gcc_observer.py").resolve(),
                 Path(__file__).with_name("ee_gcc_qemu_loopback.py").resolve()]
        for path in paths:
            if case.inputs.get(path) != digest(path.read_bytes()):
                raise ValueError("required observer input missing or unhashed: " + str(path))
        case.verify()
        if args.dry_run:
            case.inventory(required=False)
            if args.output.exists():
                raise ValueError("output already exists")
            print(json.dumps({"validated": True, "executed": False,
                              "watch_uids": len(watch["uids"])}))
            return
        # The upstream paired runner dispatches every non-allocation mode through
        # OperandTracer. Supply our tracer through that slot for this call only.
        # Its RSP allowlist, hash checks, endpoint checks, and equality logic stay intact.
        original = runner.OperandTracer
        runner.OperandTracer = partial(CseRoleTracer, watch=watch)
        try:
            receipt = runner.run(case, args.output, "cse_roles")
        finally:
            runner.OperandTracer = original
        print(json.dumps({"all_equal": receipt["all_equal"], "events": receipt["events"],
                          "output": str(args.output.resolve())}))
    except (OSError, ValueError, KeyError, RuntimeError) as error:
        parser.exit(2, str(error) + "\n")


if __name__ == "__main__":
    main()

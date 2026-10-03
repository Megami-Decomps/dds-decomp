#!/usr/bin/env python3
"""Recover exact procedure-level execution edges from a DDS BF/FLW0 script."""

from __future__ import annotations

from bisect import bisect_right
from collections import Counter, defaultdict

import flw0
import flw0_profiles


class Flw0FlowError(ValueError):
    """Raised when a script cannot form an unambiguous procedure graph."""


def _instruction_pcs(
    words: tuple[flw0.InstructionWord, ...],
) -> tuple[int, ...]:
    pcs = []
    pc = 0
    while pc < len(words):
        pcs.append(pc)
        word = words[pc]
        if (
            word.opcode in flw0._EXTENDED_OPCODES
            and word.operand_u16 == 0
            and pc + 1 < len(words)
        ):
            pc += 2
        else:
            pc += 1
    return tuple(pcs)


def analyze(
    script: flw0.Flw0File,
    profile: flw0_profiles.CommandProfile | None = None,
) -> dict:
    """Return procedures, local execution edges, and literal event calls."""

    rows = script.named_rows(0)
    if not rows:
        return {
            "procedures": [],
            "procedureEdges": [],
            "eventEdges": [],
            "deferredBattleExits": [],
            "unresolvedTargets": [],
        }
    starts = [row.start_pc for row in rows]
    if starts != sorted(starts) or len(starts) != len(set(starts)):
        raise Flw0FlowError("procedure starts must be unique and ordered by index")
    if [row.row_index for row in rows] != list(range(len(rows))):
        raise Flw0FlowError("procedure indices must be contiguous and ordered")

    words = script.code_words()
    instruction_pcs = _instruction_pcs(words)
    boundaries = set(instruction_pcs)
    if any(start not in boundaries for start in starts):
        raise Flw0FlowError("procedure start is not an instruction boundary")

    commands = profile.by_id if profile is not None else {}
    events = profile.events_by_id if profile is not None else {}
    command_counts: dict[int, Counter[int]] = defaultdict(Counter)
    local_sites: dict[tuple[int, int, str], list[int]] = defaultdict(list)
    event_sites: dict[tuple[int, int, str, str], list[int]] = defaultdict(list)
    deferred_battle_exits = []
    unresolved = []
    previous_pc: int | None = None
    task_command = (
        profile.by_name.get("CREATE_SCRIPT_TASK") if profile is not None else None
    )
    task_command_id = task_command.command_id if task_command is not None else 0x0A5
    battle_exit_command = (
        profile.by_name.get("DEFER_BATTLE_EXIT") if profile is not None else None
    )
    battle_exit_command_id = (
        battle_exit_command.command_id if battle_exit_command is not None else 0x097
    )
    event_commands = {
        0x066: ("call", "CALL_EVENT"),
        0x067: ("submit", "SUBMIT_EVENT"),
    }
    if profile is not None:
        for name, kind in (("CALL_EVENT", "call"), ("SUBMIT_EVENT", "submit")):
            command = profile.by_name.get(name)
            if command is not None:
                event_commands[command.command_id] = (kind, name)

    for instruction_index, pc in enumerate(instruction_pcs):
        source = bisect_right(starts, pc) - 1
        if source < 0:
            previous_pc = pc
            continue
        word = words[pc]
        opcode = word.opcode
        operand = word.operand_u16

        if opcode == flw0.OPCODE_IDS["COMM"]:
            command_counts[source][operand] += 1
            if operand == battle_exit_command_id:
                argument_pcs = instruction_pcs[
                    max(0, instruction_index - 3) : instruction_index
                ]
                argument_words = [words[argument_pc] for argument_pc in argument_pcs]
                arguments_are_local_literals = len(argument_words) == 3 and all(
                    argument.opcode == flw0.OPCODE_IDS["PUSHIS"]
                    and bisect_right(starts, argument_pc) - 1 == source
                    for argument_pc, argument in zip(argument_pcs, argument_words)
                )
                if not arguments_are_local_literals:
                    unresolved.append(
                        {
                            "source": source,
                            "pc": pc,
                            "kind": "battleExit",
                            "value": None,
                            "command": "DEFER_BATTLE_EXIT",
                        }
                    )
                else:
                    event, field, diagnostic = (
                        argument.operand_u16 for argument in argument_words
                    )
                    deferred_battle_exits.append(
                        {
                            "source": source,
                            "pc": pc,
                            "diagnostic": diagnostic,
                            "field": field,
                            "event": event,
                        }
                    )
            elif operand == task_command_id or operand in event_commands:
                previous = words[previous_pc] if previous_pc is not None else None
                kind = "task" if operand == task_command_id else "event"
                if (
                    previous is None
                    or previous.opcode != flw0.OPCODE_IDS["PUSHIS"]
                    or bisect_right(starts, previous_pc) - 1 != source
                ):
                    unresolved_target = {
                        "source": source,
                        "pc": pc,
                        "kind": kind,
                        "value": None,
                    }
                    if kind == "event":
                        unresolved_target["command"] = event_commands[operand][1]
                    unresolved.append(unresolved_target)
                elif kind == "task" and previous.operand_u16 >= len(rows):
                    unresolved.append(
                        {
                            "source": source,
                            "pc": pc,
                            "kind": kind,
                            "value": previous.operand_u16,
                        }
                    )
                elif kind == "task":
                    local_sites[source, previous.operand_u16, kind].append(pc)
                else:
                    edge_kind, command_name = event_commands[operand]
                    event_sites[
                        source, previous.operand_u16, edge_kind, command_name
                    ].append(pc)
        elif opcode in (flw0.OPCODE_IDS["CALL"], flw0.OPCODE_IDS["JUMP"]):
            kind = "call" if opcode == flw0.OPCODE_IDS["CALL"] else "jump"
            if operand >= len(rows):
                unresolved.append(
                    {"source": source, "pc": pc, "kind": kind, "value": operand}
                )
            else:
                local_sites[source, operand, kind].append(pc)
        previous_pc = pc

    procedures = []
    for index, row in enumerate(rows):
        end_pc = starts[index + 1] if index + 1 < len(rows) else len(words)
        native_calls = []
        for command_id, count in sorted(command_counts[index].items()):
            command = commands.get(command_id)
            item = {"id": command_id, "count": count}
            if command is not None:
                item["name"] = command.name
            native_calls.append(item)
        procedures.append(
            {
                "index": index,
                "name": row.name,
                "startPc": row.start_pc,
                "endPc": end_pc,
                "nativeCalls": native_calls,
            }
        )

    procedure_edges = [
        {
            "source": source,
            "target": target,
            "kind": kind,
            "count": len(sites),
            "sites": sites,
        }
        for (source, target, kind), sites in sorted(local_sites.items())
    ]
    event_edges = []
    for (source, event_id, kind, command), sites in sorted(event_sites.items()):
        edge = {
            "source": source,
            "eventId": event_id,
            "kind": kind,
            "command": command,
            "count": len(sites),
            "sites": sites,
        }
        if event_id in events:
            edge["event"] = events[event_id]
        event_edges.append(edge)
    return {
        "procedures": procedures,
        "procedureEdges": procedure_edges,
        "eventEdges": event_edges,
        "deferredBattleExits": deferred_battle_exits,
        "unresolvedTargets": unresolved,
    }

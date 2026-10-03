#!/usr/bin/env python3
"""Synthetic regression tests for tools/flw0.py."""

from __future__ import annotations

import re
import struct
import subprocess
import sys
import tempfile
import unittest
from hashlib import sha1
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import flw0
import flw0_profiles
import flw0_semantic
import flw0_symbolic
import flw0_view
import msg1


def _named_row(name: str, start_pc: int, reserved: int = 0) -> bytes:
    row = bytearray(0x20)
    encoded = name.encode("ascii")
    row[: min(len(encoded), 0x17)] = encoded[:0x17]
    struct.pack_into("<II", row, 0x18, start_pc, reserved)
    return bytes(row)


def _fixture(
    code_words: list[int] | None = None,
    message_data: bytes = b"",
    procedure_rows: tuple[tuple[str, int], ...] = (("synthetic_001", 0),),
    jump_rows: tuple[tuple[str, int], ...] = (),
) -> bytes:
    """Build the significant shape of a small DDS event script."""

    if code_words is None:
        code_words = [7, (42 << 16) | 29, 9]
    sections = [
        (0, 0x20, len(procedure_rows)),
        (1, 0x20, len(jump_rows)),
        (2, 4, len(code_words)),
        (3, 1, len(message_data)),
        (4, 1, 0xF0),
    ]
    table_end = 0x20 + len(sections) * 0x10
    proc = b"".join(_named_row(name, pc) for name, pc in procedure_rows)
    labels = b"".join(_named_row(name, pc) for name, pc in jump_rows)
    code = b"".join(struct.pack("<I", word) for word in code_words)
    string_padding = bytes(0xF0)
    offsets = [
        table_end,
        table_end + len(proc),
        table_end + len(proc) + len(labels),
        table_end + len(proc) + len(labels) + len(code),
        table_end + len(proc) + len(labels) + len(code) + len(message_data),
    ]
    physical_size = (
        table_end
        + len(proc)
        + len(labels)
        + len(code)
        + len(message_data)
        + len(string_padding)
    )
    declared_size = physical_size - len(string_padding)

    header = struct.pack(
        "<II4sIIIII",
        0,
        declared_size,
        b"FLW0",
        0,
        len(sections),
        0,
        0,
        0,
    )
    table = b"".join(
        struct.pack("<IIII", type_id, size, count, offset)
        for (type_id, size, count), offset in zip(sections, offsets)
    )
    return header + table + proc + labels + code + message_data + string_padding


def _instruction_words(script: flw0.Flw0File) -> tuple[flw0.InstructionWord, ...]:
    """Return instruction heads without treating extended operands as opcodes."""

    words = script.code_words()
    instructions: list[flw0.InstructionWord] = []
    pc = 0
    while pc < len(words):
        word = words[pc]
        instructions.append(word)
        if (
            word.opcode in flw0._EXTENDED_OPCODES
            and word.operand_u16 == 0
            and pc + 1 < len(words)
        ):
            pc += 2
        else:
            pc += 1
    return tuple(instructions)


class Flw0Tests(unittest.TestCase):
    def test_preserve_layout_rewrite_is_exact(self) -> None:
        original = _fixture()
        parsed = flw0.parse(original)
        self.assertEqual(parsed.to_bytes(), original)
        self.assertEqual(parsed.header.declared_size, len(original) - 0xF0)
        self.assertEqual(parsed.physical_size, len(original))

    def test_typed_views_do_not_discard_row_bytes(self) -> None:
        parsed = flw0.parse(_fixture())
        procedure = parsed.named_rows(0)[0]
        self.assertEqual(procedure.name, "synthetic_001")
        self.assertEqual(procedure.start_pc, 0)
        self.assertEqual(procedure.reserved, 0)
        self.assertEqual(len(procedure.raw), 0x20)
        self.assertEqual(
            [word.raw for word in parsed.code_words()],
            [7, (42 << 16) | 29, 9],
        )

    def test_source_round_trip_is_exact_and_readable(self) -> None:
        original = _fixture()
        source = flw0.render_source(flw0.parse(original))
        self.assertIn('proc "synthetic_001" pc=0', source)
        self.assertIn("0000: PROC 0x0000", source)
        self.assertIn("0001: PUSHIS 0x002a", source)
        self.assertIn("zero 240", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), original)

    def test_source_edit_changes_operand(self) -> None:
        original = _fixture()
        source = flw0.render_source(flw0.parse(original))
        edited_source = source.replace("PUSHIS 0x002a", "PUSHIS 0x0063")
        edited = flw0.parse_source(edited_source)
        self.assertEqual(edited.code_words()[1].operand_u16, 99)
        self.assertEqual(len(edited.to_bytes()), len(original))

        signed_source = source.replace("PUSHIS 0x002a", "PUSHIS -1")
        signed = flw0.parse_source(signed_source)
        self.assertEqual(signed.code_words()[1].operand_u16, 0xFFFF)

    def test_source_preserves_extended_and_unknown_words(self) -> None:
        original = _fixture([7, 0, 0xDEADBEEF, 0x1234FFFF, 9])
        source = flw0.render_source(flw0.parse(original))
        self.assertIn("0001: PUSHI 0xdeadbeef", source)
        self.assertIn("0003: WORD 0x1234ffff", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), original)

    def test_physical_source_supports_a_command_profile(self) -> None:
        original = _fixture(
            [
                7,
                (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
            ]
        )
        source = flw0.render_source(flw0.parse(original), "dds1")
        self.assertIn("flw0 1\nprofile dds1\n", source)
        self.assertIn("0002: COMM WAIT_FOR_TIMER_LIMIT", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), original)

        without_profile = source.replace("profile dds1\n", "")
        with self.assertRaisesRegex(
            flw0.Flw0Error, "named COMM operand requires a profile"
        ):
            flw0.parse_source(without_profile)

    def test_message_references_use_names_in_both_source_formats(self) -> None:
        message_data = msg1.encode(
            msg1.Bank(
                (
                    msg1.Message("MSG_A", 0xFFFF, (b"First",)),
                    msg1.Message("MSG_B", 0xFFFF, (b"Second",)),
                ),
                (),
            )
        )
        original = _fixture(
            [
                flw0.OPCODE_IDS["PROC"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
            ],
            message_data,
        )
        script = flw0.parse(original)

        physical = flw0.render_source(script, "dds1")
        symbolic = flw0_symbolic.render(script, "dds1")
        self.assertIn("0001: PUSHMSG MSG_B", physical)
        self.assertIn("  PUSHMSG MSG_B", symbolic)
        self.assertEqual(flw0.parse_source(physical).to_bytes(), original)
        self.assertEqual(flw0.parse_source(symbolic).to_bytes(), original)

        message_a = """\
  message MSG_A speaker=none
    page
      text "First"
    endpage
  endmessage
"""
        message_b = """\
  message MSG_B speaker=none
    page
      text "Second"
    endpage
  endmessage
"""
        reordered = flw0.parse_source(
            symbolic.replace(message_a + message_b, message_b + message_a)
        )
        self.assertEqual(reordered.code_words()[1].operand_u16, 0)
        reordered_bank = msg1.decode(
            reordered.section_bytes(reordered.sections[3])
        )
        self.assertEqual(reordered_bank.dialogs[0].name, "MSG_B")
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown message 'MISSING'"):
            flw0.parse_source(symbolic.replace("PUSHMSG MSG_B", "PUSHMSG MISSING"))

        view = flw0_view.render(script, "dds1")
        self.assertIn("MESSAGE_REQUEST_AND_POLL(message(MSG_B))", view)

    def test_selection_references_are_typed_and_return_the_choice(self) -> None:
        message_data = msg1.encode(
            msg1.Bank(
                (
                    msg1.Message("PROMPT", 0xFFFF, (b"Choose",)),
                    msg1.Selection("CHOICE", 0, 0, 0, (b"Yes", b"No")),
                ),
                (),
            )
        )
        original = _fixture(
            [
                flw0.OPCODE_IDS["PROC"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x003 << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["PUSHREG"],
                (10 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (20 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (30 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x071 << 16) | flw0.OPCODE_IDS["COMM"],
                (0 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x003 << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
            ],
            message_data,
        )
        script = flw0.parse(original)

        physical = flw0.render_source(script, "dds1")
        symbolic = flw0_symbolic.render(script, "dds1")
        self.assertIn("0001: PUSHSELECT CHOICE", physical)
        self.assertIn("  PUSHSELECT CHOICE", symbolic)
        self.assertIn("0008: PUSHIS 0x0000", physical)
        self.assertNotIn("PUSHSELECT PROMPT", symbolic)
        self.assertEqual(flw0.parse_source(physical).to_bytes(), original)
        self.assertEqual(flw0.parse_source(symbolic).to_bytes(), original)

        with self.assertRaisesRegex(
            flw0.Flw0Error, "unknown selection 'PROMPT'"
        ):
            flw0.parse_source(
                symbolic.replace("PUSHSELECT CHOICE", "PUSHSELECT PROMPT")
            )

        view = flw0_view.render(script, "dds1")
        self.assertIn(
            "result = MESSAGE_SELECTION_REQUEST_AND_POLL(selection(CHOICE))", view
        )
        self.assertIn("push result", view)
        self.assertIn("SET_MESSAGE_WINDOW_GEOMETRY(30, 20, 10)", view)

    def test_event_references_use_names_in_both_source_formats(self) -> None:
        original = _fixture(
            [
                flw0.OPCODE_IDS["PROC"],
                (602 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x066 << 16) | flw0.OPCODE_IDS["COMM"],
                (10 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x066 << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
            ]
        )
        script = flw0.parse(original)

        physical = flw0.render_source(script, "dds1")
        symbolic = flw0_symbolic.render(script, "dds1")
        self.assertIn("0001: PUSHEVENT e602", physical)
        self.assertIn("  PUSHEVENT e602", symbolic)
        self.assertIn("0003: PUSHIS 0x000a", physical)
        self.assertIn("  PUSHIS 10", symbolic)
        self.assertEqual(flw0.parse_source(physical).to_bytes(), original)
        self.assertEqual(flw0.parse_source(symbolic).to_bytes(), original)

        view = flw0_view.render(script, "dds1")
        self.assertIn("CALL_EVENT(event(e602))", view)
        self.assertIn("CALL_EVENT(10)", view)

        with self.assertRaisesRegex(flw0.Flw0Error, "unknown dds1 event target"):
            flw0.parse_source(symbolic.replace("PUSHEVENT e602", "PUSHEVENT e010"))
        with self.assertRaisesRegex(
            flw0.Flw0Error, "named event target requires a profile"
        ):
            flw0.parse_source(symbolic.replace("profile dds1\n", ""))

    def test_script_task_targets_use_local_procedure_symbols(self) -> None:
        original = _fixture(
            [
                flw0.OPCODE_IDS["PROC"],
                flw0.OPCODE_IDS["PUSHIS"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x0A5 << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["PUSHREG"],
                (0x0A7 << 16) | flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
                (1 << 16) | flw0.OPCODE_IDS["PROC"],
                flw0.OPCODE_IDS["END"],
            ],
            procedure_rows=(("main", 0), ("worker", 7)),
        )
        script = flw0.parse(original)
        symbolic = flw0_symbolic.render(script, "dds1")

        self.assertIn("  PUSHPROC worker", symbolic)
        self.assertIn("  COMM CREATE_SCRIPT_TASK", symbolic)
        self.assertEqual(flw0.parse_source(symbolic).to_bytes(), original)

        reordered = flw0.parse_source(
            symbolic.replace(
                "procedure main reserved=0x00000000\n"
                "procedure worker reserved=0x00000000",
                "procedure worker reserved=0x00000000\n"
                "procedure main reserved=0x00000000",
            )
        )
        self.assertEqual(reordered.code_words()[2].operand_u16, 0)
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown procedure 'missing'"):
            flw0.parse_source(symbolic.replace("PUSHPROC worker", "PUSHPROC missing"))

        view = flw0_view.render(script, "dds1")
        self.assertIn(
            "result = CREATE_SCRIPT_TASK(procedure(worker), 0)", view
        )
        self.assertIn("WAIT_FOR_TASK_REMOVAL(result)", view)

    def test_duplicate_message_names_keep_numeric_operands(self) -> None:
        message_data = msg1.encode(
            msg1.Bank(
                (
                    msg1.Message("SAME", 0xFFFF, (b"First",)),
                    msg1.Message("SAME", 0xFFFF, (b"Second",)),
                ),
                (),
            )
        )
        original = _fixture(
            [
                flw0.OPCODE_IDS["PROC"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                flw0.OPCODE_IDS["COMM"],
                flw0.OPCODE_IDS["END"],
            ],
            message_data,
        )
        script = flw0.parse(original)
        self.assertIn("0001: PUSHIS 0x0001", flw0.render_source(script, "dds1"))
        self.assertIn("  PUSHIS 1", flw0_symbolic.render(script, "dds1"))

    def test_source_preserves_gap_unknown_section_and_raw_row(self) -> None:
        original = bytearray(_fixture())
        table_end = 0x70
        original[table_end:table_end] = b"\xde\xad\xbe\xef"
        struct.pack_into("<I", original, 0x04, len(original) - 0xF0)
        for section_index in range(5):
            descriptor = 0x20 + section_index * 0x10
            offset = struct.unpack_from("<I", original, descriptor + 0x0C)[0]
            struct.pack_into("<I", original, descriptor + 0x0C, offset + 4)
        struct.pack_into("<I", original, 0x20 + 4 * 0x10, 99)
        original[table_end + 4 + len("synthetic_001") + 1] = 0xA5

        source = flw0.render_source(flw0.parse(bytes(original)))
        self.assertIn("section 4 type=99", source)
        self.assertIn("row ", source)
        self.assertIn("preserve offset=0x70 bytes=deadbeef", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), bytes(original))

    def test_fixed_width_edit_changes_only_one_word(self) -> None:
        original = _fixture()
        parsed = flw0.parse(original)
        edited = parsed.with_code_word(1, (99 << 16) | 29).to_bytes()
        code_offset = parsed.sections_of_type(2)[0].offset
        changed = [i for i, pair in enumerate(zip(original, edited)) if pair[0] != pair[1]]
        self.assertEqual(changed, [code_offset + 6])
        self.assertEqual(flw0.parse(edited).code_words()[1].operand_u16, 99)
        self.assertEqual(len(edited), len(original))

    def test_unknown_bytes_and_gaps_survive(self) -> None:
        original = bytearray(_fixture())
        original[-1] = 0xA5
        original[-17] = 0x5A
        self.assertEqual(flw0.parse(bytes(original)).to_bytes(), bytes(original))

    def test_rejects_truncated_section(self) -> None:
        original = bytearray(_fixture())
        section_4 = 0x20 + 4 * 0x10
        struct.pack_into("<I", original, section_4 + 8, 0x1000)
        with self.assertRaisesRegex(flw0.Flw0Error, "past file size"):
            flw0.parse(bytes(original))

    def test_rejects_a_section_inside_the_table(self) -> None:
        original = bytearray(_fixture())
        struct.pack_into("<I", original, 0x20 + 0x0C, 0x20)
        with self.assertRaisesRegex(flw0.Flw0Error, "inside the header/table"):
            flw0.parse(bytes(original))

    def test_empty_section_may_use_zero_offset(self) -> None:
        original = bytearray(_fixture())
        section_1 = 0x20 + 0x10
        struct.pack_into("<I", original, section_1 + 0x0C, 0)
        parsed = flw0.parse(bytes(original))
        source = flw0.render_source(parsed)
        self.assertEqual(flw0.parse_source(source).to_bytes(), bytes(original))

    def test_symbolic_source_round_trip_is_exact(self) -> None:
        original = _fixture()
        source = flw0_symbolic.render(flw0.parse(original))
        self.assertIn("procedure synthetic_001", source)
        self.assertIn("PROC synthetic_001", source)
        self.assertIn("PUSHIS 42", source)
        self.assertNotIn("declared_size", source)
        self.assertNotIn("physical_size", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), original)

    def test_semantic_source_lowers_exact_calls_assignments_and_expressions(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e670.bfasm"
        script = flw0.parse_source(path.read_text(encoding="utf-8"))
        source = flw0_symbolic.render(script, "dds1", semantic=True)
        self.assertIn(
            "WAIT_FOR_TASK_REMOVAL(CREATE_POLYGON_MOVIE(670, 1))", source
        )
        self.assertNotIn("  PUSHIS 670", source)
        self.assertEqual(flw0.parse_source(source).to_bytes(), script.to_bytes())

        expression = flw0_semantic.parse_expression(
            "3 + local_int[2] * int32(70000)", 1, flw0_profiles.DDS1
        )
        self.assertEqual(
            expression.lower(),
            [
                "PUSHI 70000",
                "PUSHLIX 2",
                "MUL",
                "PUSHIS 3",
                "ADD",
            ],
        )
        self.assertEqual(
            flw0_semantic.parse_expression("-1", 1, None).lower(),
            ["PUSHIS -1"],
        )
        self.assertEqual(
            flw0_semantic.parse_expression("-(1)", 1, None).lower(),
            ["PUSHIS 1", "MINUS"],
        )
        nested_call = flw0_semantic.parse_expression(
            "WAIT_FOR_TASK_REMOVAL(CREATE_SCRIPT_TASK(procedure(main), -1))",
            1,
            flw0_profiles.DDS1,
        )
        self.assertEqual(
            nested_call.lower_call(),
            [
                "PUSHIS -1",
                "PUSHPROC main",
                "COMM CREATE_SCRIPT_TASK",
                "PUSHREG",
                "COMM WAIT_FOR_TASK_REMOVAL",
            ],
        )
        self.assertEqual(
            flw0_semantic.lower_code(
                [(1, "WAIT_FOR_TIMER_LIMIT(local_int[0] == 1)")],
                flw0_profiles.DDS1,
            ),
            [
                (1, "PUSHIS 1"),
                (1, "PUSHLIX 0"),
                (1, "EQ"),
                (1, "COMM WAIT_FOR_TIMER_LIMIT"),
            ],
        )
        selector = flw0_semantic.parse_expression(
            "READ_TREASURE_TABLE_VALUE(ITEM_QUANTITY)",
            1,
            flw0_profiles.DDS1,
        )
        self.assertEqual(
            selector.lower(),
            ["PUSHIS 2", "COMM READ_TREASURE_TABLE_VALUE", "PUSHREG"],
        )
        self.assertEqual(
            selector.render(), "READ_TREASURE_TABLE_VALUE(ITEM_QUANTITY)"
        )
        barrier_value = flw0_semantic.parse_expression(
            "READ_BARRIER_VALUE(SOURCE_VECTOR_ID)",
            1,
            flw0_profiles.DDS1,
        )
        self.assertEqual(
            barrier_value.lower(),
            ["PUSHIS 2", "COMM READ_BARRIER_VALUE", "PUSHREG"],
        )
        self.assertEqual(
            barrier_value.render(), "READ_BARRIER_VALUE(SOURCE_VECTOR_ID)"
        )
        self.assertEqual(
            flw0_semantic.parse_expression(
                "READ_BARRIER_VALUE(3)", 1, flw0_profiles.DDS1
            ).render(),
            "READ_BARRIER_VALUE(3)",
        )
        dynamic_selector = flw0_semantic.parse_expression(
            "READ_TREASURE_TABLE_VALUE(local_int[0])",
            1,
            flw0_profiles.DDS1,
        )
        self.assertEqual(
            dynamic_selector.lower(),
            ["PUSHLIX 0", "COMM READ_TREASURE_TABLE_VALUE", "PUSHREG"],
        )
        with self.assertRaisesRegex(
            flw0.Flw0Error, "not a symbolic value for argument 0"
        ):
            flw0_semantic.parse_expression(
                "SET_SOLAR_OVERLAY_MODE(CONTENT_KIND)",
                1,
                flw0_profiles.DDS1,
            )
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown value 'CONTENT_KIND'"):
            flw0_semantic.parse_expression(
                "CONTENT_KIND", 1, flw0_profiles.DDS1
            )

        result_condition = flw0.parse(
            _fixture(
                [
                    flw0.OPCODE_IDS["PROC"],
                    (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                    (6 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                    (0x21D << 16) | flw0.OPCODE_IDS["COMM"],
                    flw0.OPCODE_IDS["PUSHREG"],
                    flw0.OPCODE_IDS["EQ"],
                    flw0.OPCODE_IDS["IF"],
                    flw0.OPCODE_IDS["END"],
                ],
                jump_rows=(("done", 7),),
            )
        )
        result_source = flw0_symbolic.render(
            result_condition, "dds1", semantic=True
        )
        self.assertIn(
            "if_not (ACTION_WINDOW_REQUEST_AND_POLL_DIRECT(6) == 1) goto done",
            result_source,
        )
        self.assertEqual(
            flw0.parse_source(result_source).to_bytes(), result_condition.to_bytes()
        )

        labelled_result = flw0.parse(
            _fixture(
                [
                    flw0.OPCODE_IDS["PROC"],
                    (0x068 << 16) | flw0.OPCODE_IDS["COMM"],
                    flw0.OPCODE_IDS["PUSHREG"],
                    flw0.OPCODE_IDS["POPLIX"],
                    flw0.OPCODE_IDS["END"],
                ],
                jump_rows=(("capture", 2),),
            )
        )
        labelled_source = flw0_symbolic.render(
            labelled_result, "dds1", semantic=True
        )
        self.assertIn("result = READ_CURRENT_WORLD_OBJECT_ID()", labelled_source)
        self.assertIn("capture:\n  PUSHREG", labelled_source)
        self.assertEqual(
            flw0.parse_source(labelled_source).to_bytes(), labelled_result.to_bytes()
        )

    def test_symbolic_local_aliases_are_exact_and_survive_rendering(self) -> None:
        source = """\
flw0 2
profile dds1
header word00=0 word0c=0 word18=0 word1c=0
locals int=2 float=1
alias counter = local_int[0]
alias limit = local_int[1]
alias fade = local_float[0]
procedure main
code
main:
  PROC main
  counter = 1
  limit = counter + 2
  fade = float32(1.5)
  WAIT_FOR_TIMER_LIMIT(limit)
  return
end
messages
end
strings
end
"""
        indexed = """\
flw0 2
profile dds1
header word00=0 word0c=0 word18=0 word1c=0
locals int=2 float=1
procedure main
code
main:
  PROC main
  local_int[0] = 1
  local_int[1] = local_int[0] + 2
  local_float[0] = float32(1.5)
  WAIT_FOR_TIMER_LIMIT(local_int[1])
  return
end
messages
end
strings
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(
            script.local_aliases,
            (
                flw0.LocalAlias("counter", "local_int", 0),
                flw0.LocalAlias("limit", "local_int", 1),
                flw0.LocalAlias("fade", "local_float", 0),
            ),
        )
        self.assertEqual(script.to_bytes(), flw0.parse_source(indexed).to_bytes())

        rendered = flw0_symbolic.render(script, "dds1", semantic=True)
        self.assertIn("alias counter = local_int[0]", rendered)
        self.assertIn("counter = 1", rendered)
        self.assertIn("limit = counter + 2", rendered)
        self.assertIn("WAIT_FOR_TIMER_LIMIT(limit)", rendered)
        self.assertNotIn("local_int[0] =", rendered)
        reparsed = flw0.parse_source(rendered)
        self.assertEqual(reparsed.local_aliases, script.local_aliases)
        self.assertEqual(reparsed.to_bytes(), script.to_bytes())

        assembly = source.replace(
            "  counter = 1\n"
            "  limit = counter + 2\n"
            "  fade = float32(1.5)\n"
            "  WAIT_FOR_TIMER_LIMIT(limit)\n",
            "  PUSHLIX counter\n"
            "  POPLIX limit\n"
            "  PUSHLFX fade\n"
            "  POPLFX fade\n",
        )
        assembly_script = flw0.parse_source(assembly)
        assembly_rendered = flw0_symbolic.render(assembly_script, "dds1")
        self.assertIn("  PUSHLIX counter", assembly_rendered)
        self.assertIn("  POPLFX fade", assembly_rendered)
        self.assertEqual(
            flw0.parse_source(assembly_rendered).to_bytes(),
            assembly_script.to_bytes(),
        )

        invalid_aliases = {
            "duplicate alias name": "alias counter = local_int[1]",
            "already named 'counter'": "alias other = local_int[0]",
            "outside the declared count 2": "alias other = local_int[2]",
            "alias name 'result' is reserved": "alias result = local_int[0]",
            "alias name 'main' is reserved": "alias main = local_int[0]",
        }
        for error, declaration in invalid_aliases.items():
            malformed = source.replace("procedure main", f"{declaration}\nprocedure main")
            with self.subTest(declaration=declaration):
                with self.assertRaisesRegex(flw0.Flw0Error, error):
                    flw0.parse_source(malformed)

    def test_semantic_source_compiles_canonical_if_else_and_while(self) -> None:
        self.assertEqual(
            flw0_semantic.lower_code(
                [
                    (1, "if (1) {"),
                    (2, "WAIT_FOR_TIMER_LIMIT(1)"),
                    (3, "}"),
                    (4, "done:"),
                ],
                flw0_profiles.DDS1,
            ),
            [
                (1, "PUSHIS 1"),
                (1, "IF done"),
                (2, "PUSHIS 1"),
                (2, "COMM WAIT_FOR_TIMER_LIMIT"),
                (1, "GOTO done"),
                (4, "done:"),
            ],
        )
        source = """\
flw0 2
profile dds1
header word00=0 word0c=0 word18=0 word1c=0
locals int=2 float=0
procedure main
jump_label otherwise
jump_label after_if
jump_label again
jump_label done
code
main:
  PROC main
  if (local_int[0] != 0) {
    WAIT_FOR_TIMER_LIMIT(1)
  } else {
otherwise:
    WAIT_FOR_TIMER_LIMIT(2)
  }
after_if:
again:
  while (3 < local_int[1]) {
    WAIT_FOR_TIMER_LIMIT(local_int[1])
  }
done:
  return
end
messages
end
strings
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(
            [word.raw for word in script.code_words()],
            [
                flw0.OPCODE_IDS["PROC"],
                flw0.OPCODE_IDS["PUSHIS"],
                flw0.OPCODE_IDS["PUSHLIX"],
                flw0.OPCODE_IDS["NEQ"],
                flw0.OPCODE_IDS["IF"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
                (1 << 16) | flw0.OPCODE_IDS["GOTO"],
                (2 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHLIX"],
                (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
                flw0.OPCODE_IDS["LT"],
                (3 << 16) | flw0.OPCODE_IDS["IF"],
                (1 << 16) | flw0.OPCODE_IDS["PUSHLIX"],
                (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
                (2 << 16) | flw0.OPCODE_IDS["GOTO"],
                flw0.OPCODE_IDS["END"],
            ],
        )
        self.assertEqual(
            [(row.name, row.start_pc) for row in script.named_rows(1)],
            [
                ("otherwise", 8),
                ("after_if", 10),
                ("again", 10),
                ("done", 17),
            ],
        )

    def test_semantic_renderer_round_trips_both_symbolic_corpora(self) -> None:
        root = TOOLS.parent
        expected_files = {"dds1": 129, "dds2": 126}
        for game, expected in expected_files.items():
            files = 0
            for path in sorted((root / f"src/{game}/scripts").rglob("*.bfasm")):
                text = path.read_text(encoding="utf-8")
                if not text.startswith("flw0 2\n"):
                    continue
                script = flw0.parse_source(text)
                semantic = flw0_symbolic.render(script, game, semantic=True)
                with self.subTest(source=path.relative_to(root)):
                    self.assertEqual(
                        flw0.parse_source(semantic).to_bytes(), script.to_bytes()
                    )
                files += 1
            self.assertEqual(files, expected)

    def test_structured_source_renderer_round_trips_both_symbolic_corpora(self) -> None:
        root = TOOLS.parent
        expected = {
            "dds1": (129, 4079, 738),
            "dds2": (126, 3416, 1190),
        }
        for game, expected_counts in expected.items():
            files = ifs = loops = 0
            for path in sorted((root / f"src/{game}/scripts").rglob("*.bfasm")):
                text = path.read_text(encoding="utf-8")
                if not text.startswith("flw0 2\n"):
                    continue
                script = flw0.parse_source(text)
                structured = flw0_symbolic.render(script, game, structured=True)
                with self.subTest(source=path.relative_to(root)):
                    self.assertEqual(
                        flw0.parse_source(structured).to_bytes(), script.to_bytes()
                    )
                for line in structured.splitlines():
                    statement = line.lstrip()
                    ifs += statement.startswith("if (")
                    loops += statement.startswith("while (")
                files += 1
            self.assertEqual((files, ifs, loops), expected_counts)

    def test_msg1_source_round_trip_relayouts_dialogs_and_relocations(self) -> None:
        bank = msg1.Bank(
            (
                msg1.Message(
                    "HELLO",
                    0,
                    (
                        bytes.fromhex("f208ffff") + b"Hello\n" + bytes.fromhex("f104"),
                        b"",
                        b"Second page",
                    ),
                ),
                msg1.Selection("CHOICE", 0, 0, 0, (b"Yes", b"No"), b"\0"),
            ),
            (bytes.fromhex("83f48dd48ee1"),),
        )
        binary = msg1.encode(bank)
        self.assertEqual(msg1.decode(binary), bank)
        rendered = msg1.render(binary)
        self.assertIn("  message HELLO speaker=0", rendered)
        self.assertIn('      text "Hello"', rendered)
        self.assertIn("      control f2 08 ff ff", rendered)
        self.assertIn("  select CHOICE ext=0 pattern=0 reserved=0 trailing=00", rendered)
        self.assertIn('    font "人修羅"', rendered)
        content = [(index, line.strip()) for index, line in enumerate(rendered[1:], 1)]
        self.assertEqual(msg1.parse_source(content), binary)

        semantic = msg1.render(binary, semantic=True)
        self.assertIn("      segment-start", semantic)
        self.assertIn("      stream-end", semantic)
        semantic_content = [
            (index, line.strip()) for index, line in enumerate(semantic[1:], 1)
        ]
        self.assertEqual(msg1.parse_source(semantic_content), binary)

        edited = [line.replace('text "Hello"', 'text "A longer greeting"') for line in rendered]
        edited_content = [(index, line.strip()) for index, line in enumerate(edited[1:], 1)]
        edited_binary = msg1.parse_source(edited_content)
        self.assertIn(b"A longer greeting", edited_binary)
        self.assertGreater(len(edited_binary), len(binary))
        self.assertEqual(msg1.encode(msg1.decode(edited_binary)), edited_binary)

        quoted = msg1.Bank((msg1.Message('A "quoted" name', 0xFFFF, (b"Text",)),), ())
        quoted_binary = msg1.encode(quoted)
        quoted_source = msg1.render(quoted_binary)
        quoted_content = [
            (index, line.strip()) for index, line in enumerate(quoted_source[1:], 1)
        ]
        self.assertEqual(msg1.parse_source(quoted_content), quoted_binary)
        with self.assertRaisesRegex(msg1.Msg1Error, "not printable ASCII"):
            msg1.encode(msg1.Bank((msg1.Message("BAD\nNAME", 0xFFFF, ()),), ()))

    def test_msg1_font_text_preserves_preferred_and_ambiguous_glyphs(self) -> None:
        bank = msg1.Bank(
            (msg1.Message("FONT", 0xFFFF, (bytes.fromhex("81b381b281b581b4"),)),),
            (),
        )
        binary = msg1.encode(bank)
        rendered = msg1.render(binary)
        self.assertIn('      font "ア"', rendered)
        self.assertIn("      glyphs 81b2", rendered)
        self.assertIn('      font "イ"', rendered)
        self.assertIn("      glyphs 81b4", rendered)
        content = [(index, line.strip()) for index, line in enumerate(rendered[1:], 1)]
        self.assertEqual(msg1.parse_source(content), binary)

        bad = [
            (1, "message BAD speaker=none"),
            (2, "page"),
            (3, 'font "🙂"'),
            (4, "endpage"),
            (5, "endmessage"),
        ]
        with self.assertRaisesRegex(msg1.Msg1Error, "not in the DDS1 MSG1 map"):
            msg1.parse_source(bad)

    def test_msg1_semantic_controls_preserve_native_operands(self) -> None:
        controls = bytes.fromhex(
            "f208ffff f20602ff f20205ff f20901ff f20781ff "
            "f20301ff f10f f104"
        )
        bank = msg1.Bank((msg1.Message("CONTROL", 0xFFFF, (controls,)),), ())
        binary = msg1.encode(bank)
        source = msg1.render(binary, semantic=True)
        expected = {
            "      segment-start",
            "      font-slot 1",
            "      text-attribute 1 4",
            "      text-attribute 2 0",
            "      text-attribute 3 128",
            "      token 0",
            "      conditional-newline",
            "      stream-end",
        }
        self.assertTrue(expected.issubset(source))
        content = [(index, line.strip()) for index, line in enumerate(source[1:], 1)]
        self.assertEqual(msg1.parse_source(content), binary)

        invalid = [
            (1, "message BAD speaker=none"),
            (2, "page"),
            (3, "text-attribute 4 0"),
            (4, "endpage"),
            (5, "endmessage"),
        ]
        with self.assertRaisesRegex(msg1.Msg1Error, "slot must be 1, 2, or 3"):
            msg1.parse_source(invalid)

    def test_symbolic_source_resolves_names_and_relayouts(self) -> None:
        source = """\
flw0 2
header word00=0 word0c=0 word18=0 word1c=0
locals int=2 float=1
procedure main
procedure helper name="helper_proc" reserved=7
jump_label finished
code
main:
  PROC main
  CALL helper
  IF finished
helper:
  PROC helper
finished:
  END
end
messages
  bytes 010203
end
strings
  zero 16
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(
            [word.raw for word in script.code_words()],
            [7, (1 << 16) | 11, 28, (1 << 16) | 7, 9],
        )
        self.assertEqual(
            [(row.name, row.start_pc, row.reserved) for row in script.named_rows(0)],
            [("main", 0, 0), ("helper_proc", 3, 7)],
        )
        self.assertEqual(
            [(row.name, row.start_pc) for row in script.named_rows(1)],
            [("finished", 4)],
        )
        self.assertEqual(script.header.int_local_count, 2)
        self.assertEqual(script.header.float_local_count, 1)
        self.assertEqual(script.header.declared_size, script.sections[4].offset)

        grown = flw0.parse_source(source.replace("finished:\n", "  PUSHIS -1\nfinished:\n"))
        self.assertEqual(
            grown.sections[2].element_count,
            script.sections[2].element_count + 1,
        )
        self.assertEqual(grown.named_rows(1)[0].start_pc, 5)
        self.assertEqual(grown.named_rows(0)[1].start_pc, 3)
        self.assertEqual(grown.sections[4].offset, script.sections[4].offset + 4)
        self.assertEqual(len(grown.to_bytes()), len(script.to_bytes()) + 4)
        self.assertIn("PUSHIS -1", flw0_symbolic.render(grown))

    def test_symbolic_strings_relocate_named_type5_operands(self) -> None:
        source = """\
flw0 2
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  PROC main
  PUSHTYPE5 camera
  PUSHTYPE5 camera_2
  END
end
messages
end
strings
  string camera "cam01" # first copy
  string camera_2 "cam01"
  zero 3
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(
            [word.operand_u16 for word in script.code_words()[1:3]], [0, 6]
        )
        self.assertEqual(
            script.section_bytes(script.sections[4]), b"cam01\0cam01\0\0\0\0"
        )
        rendered = flw0_symbolic.render(script)
        self.assertIn('string cam01 "cam01"', rendered)
        self.assertIn('string cam01_2 "cam01"', rendered)
        self.assertEqual(flw0.parse_source(rendered).to_bytes(), script.to_bytes())

        grown = flw0.parse_source(
            source.replace('camera "cam01" # first copy', 'camera "camera01"')
        )
        self.assertEqual(
            [word.operand_u16 for word in grown.code_words()[1:3]], [0, 9]
        )

    def test_symbolic_strings_preserve_short_descriptor_count(self) -> None:
        source = """\
flw0 2
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  PROC main
  PUSHTYPE5 tail
  END
end
messages
end
strings count=0
  string tail "text past the logical end"
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(script.sections[4].element_count, 0)
        self.assertEqual(script.sections[4].logical_size, 0)
        self.assertEqual(
            flw0._type5_extent(script, script.sections[4]),
            b"text past the logical end\0",
        )
        rendered = flw0_symbolic.render(script)
        self.assertIn("\nstrings count=0\n", rendered)
        self.assertIn(
            'string text_past_the_logical_end "text past the logical end"', rendered
        )
        self.assertEqual(flw0.parse_source(rendered).to_bytes(), script.to_bytes())

        with self.assertRaisesRegex(flw0.Flw0Error, "string count exceeds payload"):
            flw0.parse_source(source.replace("strings count=0", "strings count=99"))
        with self.assertRaisesRegex(flw0.Flw0Error, "trailing or uncovered bytes"):
            flw0.parse_source(source.replace("  PUSHTYPE5 tail\n", ""))

    def test_symbolic_string_falls_back_for_opaque_reference(self) -> None:
        source = """\
flw0 2
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  PROC main
  PUSHTYPE5 0x0001
  END
end
messages
end
strings
  bytes ff0000
end
"""
        script = flw0.parse_source(source)
        rendered = flw0_symbolic.render(script)
        self.assertIn("PUSHTYPE5 0x0001", rendered)
        self.assertIn("bytes ff0000", rendered)
        self.assertEqual(flw0.parse_source(rendered).to_bytes(), script.to_bytes())

        with self.assertRaisesRegex(flw0.Flw0Error, "unknown string 'missing'"):
            flw0.parse_source(source.replace("PUSHTYPE5 0x0001", "PUSHTYPE5 missing"))

    def test_physical_source_names_strings_past_logical_type4_end(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e503.bfasm"
        script = flw0.parse_source(path.read_text(encoding="utf-8"))
        rendered = flw0.render_source(script, "dds1")
        self.assertIn("section 4 type=4 stride=0x1 count=48", rendered)
        self.assertIn("extent=0x174", rendered)
        self.assertIn('string Camera01_MOTION "Camera01_MOTION"', rendered)
        self.assertIn("PUSHTYPE5 Camera01_MOTION", rendered)
        self.assertEqual(flw0.parse_source(rendered).to_bytes(), script.to_bytes())
        without_extent = rendered.replace(" extent=0x174", "")
        with self.assertRaisesRegex(flw0.Flw0Error, "exceeds declared extent"):
            flw0.parse_source(without_extent)

    def test_physical_named_strings_require_one_type4_section(self) -> None:
        source = """\
flw0 1
header word00=0 declared_size=0 word0c=0 int_locals=0 float_locals=0 word18=0 word1c=0 physical_size=0x42
section 0 type=4 stride=1 count=0 offset=0
end
section 1 type=4 stride=1 count=0 offset=0x40 extent=2
  string value "x"
end
"""
        with self.assertRaisesRegex(
            flw0.Flw0Error, "named strings require exactly one type-4 section"
        ):
            flw0.parse_source(source)

    def test_symbolic_source_rejects_unresolved_names(self) -> None:
        source = """\
flw0 2
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  CALL missing
end
messages
end
strings
end
"""
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown procedure 'missing'"):
            flw0.parse_source(source)

        without_label = source.replace("main:\n", "")
        with self.assertRaisesRegex(flw0.Flw0Error, "has no code label"):
            flw0.parse_source(without_label.replace("  CALL missing\n", "  END\n"))

    def test_dds1_command_profile_resolves_names_and_keeps_numeric_fallback(self) -> None:
        source = """\
flw0 2
profile dds1
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  PROC main
  COMM RESET_DRAW_EFFECTS
  COMM 0x0123
  END
end
messages
end
strings
end
"""
        script = flw0.parse_source(source)
        self.assertEqual(
            [word.raw for word in script.code_words()],
            [7, (0x043 << 16) | 8, (0x123 << 16) | 8, 9],
        )
        rendered = flw0_symbolic.render(script, "dds1")
        self.assertIn("profile dds1", rendered)
        self.assertIn("COMM RESET_DRAW_EFFECTS", rendered)
        self.assertIn("COMM 0x0123", rendered)
        self.assertEqual(flw0.parse_source(rendered).to_bytes(), script.to_bytes())

        unprofiled = flw0_symbolic.render(script)
        self.assertNotIn("profile ", unprofiled)
        self.assertIn("COMM 0x0043", unprofiled)
        self.assertIn("COMM 0x0123", unprofiled)
        self.assertEqual(flw0.parse_source(unprofiled).to_bytes(), script.to_bytes())

    def test_dds1_command_profile_is_explicit(self) -> None:
        source = """\
flw0 2
profile dds1
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  PROC main
  COMM NOT_A_DDS1_COMMAND
  END
end
messages
end
strings
end
"""
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown dds1 command"):
            flw0.parse_source(source)
        with self.assertRaisesRegex(
            flw0.Flw0Error, "named COMM operand requires a profile"
        ):
            flw0.parse_source(source.replace("profile dds1\n", ""))
        with self.assertRaisesRegex(flw0.Flw0Error, "unknown command profile"):
            flw0.parse_source(source.replace("profile dds1", "profile nocturne"))

    def test_command_profile_cli_error_has_no_traceback(self) -> None:
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "invalid.bfasm"
            source.write_text(
                """\
flw0 2
profile unknown
header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0
procedure main
code
main:
  END
end
messages
end
strings
end
""",
                encoding="utf-8",
            )
            result = subprocess.run(
                [
                    sys.executable,
                    str(TOOLS / "flw0.py"),
                    "assemble",
                    str(source),
                    str(root / "invalid.bf"),
                ],
                capture_output=True,
                text=True,
            )
        self.assertEqual(result.returncode, 2)
        self.assertIn("unknown command profile 'unknown'", result.stderr)
        self.assertNotIn("Traceback", result.stderr)

    def test_dds_command_profiles_record_verified_stack_effects(self) -> None:
        expected = {
            "MESSAGE_REQUEST_AND_POLL": (0x000, 1, False),
            "ACTIVATE_MESSAGE_PANEL": (0x001, 0, False),
            "FINISH_SCRIPT_MESSAGE_WINDOW": (0x002, 0, False),
            "MESSAGE_SELECTION_REQUEST_AND_POLL": (0x003, 1, True),
            "TEST_MODEL_FLAG": (0x007, 1, True),
            "SET_MODEL_FLAG": (0x008, 1, False),
            "CLEAR_MODEL_FLAG": (0x009, 1, False),
            "RANDOM_ONE_TO": (0x00A, 1, True),
            "WAIT_FOR_TIMER_START": (0x00D, 0, False),
            "WAIT_FOR_TIMER_LIMIT": (0x00E, 1, False),
            "SCREEN_FADE_A": (0x00F, 2, False),
            "SCREEN_FADE_B": (0x010, 2, False),
            "ADD_EFFECT_UNIT_TO_WORLD": (0x012, 1, False),
            "START_CAMERA_PATH_MOVE": (0x013, 2, False),
            "CREATE_LINKED_CAMERA_VIEWER": (0x015, 2, True),
            "DESTROY_WORLD_UNIT": (0x016, 1, False),
            "START_UNIT_MOVE_TO_POSITION_OBJECT": (0x017, 5, False),
            "ADD_FLAGGED_EFFECT_UNIT_TO_WORLD": (0x019, 1, False),
            "SET_CONTROLLER_VIBRATION": (0x01A, 3, False),
            "FADE_BACKGROUND_IN": (0x01F, 1, False),
            "READ_SOLAR_PHASE": (0x027, 0, True),
            "SUBMIT_EVENT_WITH_MODE": (0x028, 2, False),
            "RESET_DRAW_EFFECTS": (0x043, 0, False),
            "RETURN_TO_TITLE": (0x046, 0, False),
            "WAIT_FOR_UNIT_MOTION": (0x049, 1, False),
            "ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR": (0x04A, 2, False),
            "SET_UNIT_VALUE": (0x04B, 2, False),
            "ACTION_WINDOW_REQUEST_AND_POLL": (0x05E, 1, True),
            "RESTORE_CAMERA_NODE_MODE": (0x060, 0, False),
            "RELEASE_CURRENT_OBJECT": (0x061, 0, False),
            "CALL_EVENT": (0x066, 1, False),
            "SUBMIT_EVENT": (0x067, 1, False),
            "READ_CURRENT_WORLD_OBJECT_ID": (0x068, 0, True),
            "CLEAR_UNIT_LOW_FLAG": (0x069, 1, False),
            "SET_UNIT_LOW_FLAG": (0x06A, 1, False),
            "MOVE_OBJECT_ALONG_PATH": (0x06B, 3, False),
            "WAIT_FOR_OBJECT_PATH": (0x06C, 1, False),
            "CHANGE_ITEM_COUNT": (0x070, 2, False),
            "SET_MESSAGE_WINDOW_GEOMETRY": (0x071, 3, False),
            "PREPARE_UNIT_MOTION_STATE": (0x073, 5, False),
            "START_UNIT_PATH_FOLLOW": (0x08B, 7, False),
            "READ_SECONDARY_WORLD_ID_VALUE": (0x094, 1, True),
            "DEFER_BATTLE_EXIT": (0x097, 3, False),
            "REQUEST_CURRENT_GROUP_SEQUENCE": (0x098, 2, False),
            "RESET_FIELD_EFFECTS": (0x099, 0, False),
            "DESTROY_WORLD_EFFECT_OBJECT": (0x09B, 1, False),
            "COPY_EFFECT_OBJECT_TRANSFORM_FROM_SOURCE": (0x09D, 2, False),
            "FOCUS_CAMERA_ON_OBJECT": (0x0A3, 1, False),
            "UPDATE_FIELD_LOOK_AT_SEGMENT": (0x0A4, 0, False),
            "CREATE_SCRIPT_TASK": (0x0A5, 2, True),
            "DESTROY_REGISTERED_TASK": (0x0A6, 1, False),
            "WAIT_FOR_TASK_REMOVAL": (0x0A7, 1, False),
            "CREATE_POLYGON_MOVIE": (0x0AA, 2, True),
            "SETUP_FADE_FRAMES": (0x0AB, 2, False),
            "SET_SOLAR_OVERLAY_MODE": (0x0C3, 1, False),
            "WAIT_FOR_CAMP_TASK": (0x0C8, 1, False),
            "BIND_MODEL_MOTION_SOUND": (0x0C9, 2, True),
            "CREATE_EVENT_TEXTURE_TASK": (0x0CC, 2, True),
            "CREATE_FLAGGED_EFFECT_OBJECT": (0x0CD, 1, True),
            "SET_EFFECT_MODEL_CUT": (0x0CE, 2, False),
            "SET_EFFECT_MODEL_ROTATION": (0x0CF, 4, False),
            "CREATE_EVENT_BED_EFFECT": (0x0D0, 2, True),
            "ATTACH_EFFECT_TO_PATH": (0x0D1, 2, False),
            "WAIT_FOR_EFFECT_PATH": (0x0D2, 1, False),
            "CREATE_MG1_EFFECT": (0x0D3, 1, True),
            "CREATE_EVENT_MG1_EFFECT": (0x0D4, 2, True),
            "CREATE_MG2_EFFECT": (0x0D5, 1, True),
            "CREATE_EVENT_MG2_EFFECT": (0x0D6, 2, True),
            "SET_MG1_EFFECT_POINTS": (0x0D7, 3, False),
            "SET_MG2_EFFECT_POINTS": (0x0D8, 5, False),
            "START_EVENT_BGM": (0x0D9, 2, False),
            "SET_WORLD_NODE_BASE_MODE": (0x0DB, 1, False),
            "DESTROY_EFFECT_OBJECT": (0x0E3, 1, False),
            "START_CAMP_TASK_IF_ABSENT": (0x0F7, 1, False),
            "CAMP_TASK_READY": (0x0F8, 1, True),
            "REQUEST_ALTERNATE_FIELD_SEQUENCE": (0x100, 2, False),
            "SET_FIELD_ENVIRONMENT": (0x101, 2, False),
            "ENABLE_FIELD_MODELS": (0x103, 4, False),
            "DISABLE_FIELD_MODELS": (0x104, 4, False),
            "ENABLE_FIELD_ANIMATION": (0x105, 4, False),
            "DISABLE_FIELD_ANIMATION": (0x106, 4, False),
            "ENABLE_FIELD_COLLISION": (0x107, 3, False),
            "DISABLE_FIELD_COLLISION": (0x108, 3, False),
            "ENABLE_FIELD_MODEL_GROUP": (0x109, 3, False),
            "DISABLE_FIELD_MODEL_GROUP": (0x10A, 3, False),
            "ENABLE_FIELD_NPCS": (0x10E, 3, False),
            "DISABLE_FIELD_NPCS": (0x10F, 3, False),
            "SET_FIELD_GIMMICK_DISPLAY": (0x110, 4, False),
            "ENABLE_FIELD_MAP_ENTRY": (0x111, 3, False),
            "DISABLE_FIELD_MAP_ENTRY": (0x112, 3, False),
            "SET_FIELD_CAMERA_TABLE": (0x113, 1, False),
            "READ_TREASURE_TABLE_VALUE": (0x114, 1, True),
            "MARK_CURRENT_TREASURE_OPENED": (0x115, 0, False),
            "TEST_CURRENT_TREASURE_OPENED": (0x116, 0, True),
            "START_AND_WAIT_FOR_STREAM_SOUND": (0x11B, 1, False),
            "ADVANCE_STREAM_SOUND_STATE": (0x11C, 0, False),
            "RESET_STREAM_PLAYBACK": (0x11F, 0, False),
            "WAIT_FOR_STREAM_IDLE": (0x121, 0, False),
            "CLEAR_WORLD_OBJECT_STATE_FLAGS": (0x124, 1, False),
            "ADD_PARTY_CURRENCY": (0x139, 1, False),
            "APPLY_PARTY_TRAP_EFFECT": (0x13A, 1, False),
            "SET_MESSAGE_RANGE": (0x13C, 2, False),
            "SUBMIT_EVENT_IMMEDIATE": (0x166, 1, False),
            "QUEUE_WORLD_OBJECT_PENDING_VALUE": (0x1E0, 2, False),
            "CLEAR_WORLD_OBJECT_PENDING_VALUE": (0x1E1, 1, False),
            "CLEAR_PROCESS_CONTROL_FLAG": (0x1E7, 0, False),
            "CONSUME_FIELD_SKILL_END_NOTICE": (0x1F1, 1, True),
            "START_ARCHIVE_SOUND": (0x1F3, 2, False),
            "STOP_ARCHIVE_SOUND": (0x1F4, 2, False),
            "REVEAL_AUTOMAP_RECTANGLE": (0x1F6, 5, False),
            "READ_SUCTION_WARP_VALUE": (0x1FA, 1, True),
            "READ_BARRIER_VALUE": (0x1FB, 1, True),
            "READ_CURRENT_SCENE_SELECTION_RESOURCE": (0x1FE, 0, True),
            "FIND_FIELD_EFFECT_BY_NAME": (0x1FF, 1, True),
            "READ_ELEVATOR_TABLE_VALUE": (0x200, 1, True),
            "RUN_FIELD_DESTINATION_TRANSITION": (0x201, 1, False),
            "SET_CURRENT_TASK_SCENE": (0x202, 0, False),
            "NO_OP_FIELD_TRANSITION": (0x203, 0, False),
            "APPLY_CURRENT_TASK_ENTRY_TRIGGER": (0x204, 0, False),
            "ADVANCE_FIELD_INTERACTION": (0x205, 2, True),
            "READ_FIELD_INTERACTION_VALUE": (0x206, 2, True),
            "READ_FIELD_INTERACTION_KIND": (0x207, 0, True),
            "READ_LADDER_TABLE_VALUE": (0x208, 1, True),
            "CONFIGURE_ELEVATOR_CAMERA_MOVE": (0x209, 2, False),
            "RESET_ELEVATOR_CAMERA_MOVE_TRACKING": (0x20A, 0, False),
            "POLL_ELEVATOR_MOVE_STATE": (0x20B, 0, True),
            "READ_DOOR_WARP_VALUE": (0x20C, 1, True),
            "APPLY_CURRENT_TASK_RECORD_ENTRY": (0x20D, 0, False),
            "START_CURRENT_FIELD_INTERACTION_EVENT": (0x20E, 0, False),
            "RUN_FIELD_TRANSITION_SELECTOR": (0x20F, 1, False),
            "START_SCENE_BGM": (0x210, 1, False),
            "PLAY_FIELD_SE_VOLUME_PAN": (0x214, 1, False),
            "PLAY_FIELD_SE": (0x215, 1, False),
            "RELEASE_CURRENT_BGM": (0x216, 0, False),
            "LOAD_ARCHIVE_SOUND_BANK_AND_WAIT": (0x217, 1, False),
            "READ_WARP_EFFECT_MODE": (0x219, 0, True),
            "ACTION_WINDOW_REQUEST_AND_POLL_DIRECT": (0x21D, 1, True),
            "APPLY_FIELD_MODEL_LIGHTING": (0x21E, 1, False),
            "AI_COUNTER_REACHED_LIMIT": (0x0DF, 1, True),
            "AI_SELECT_ACTION_BY_KIND": (0x0E2, 2, False),
            "TRACE_BATTLE_RETREAT": (0x0E4, 0, False),
            "TRACE_BATTLE_ALL_RETREAT": (0x0E5, 0, False),
            "AI_RESET_COMMAND_CONTEXT": (0x0E6, 0, False),
            "AI_SELECT_LOWEST_HP_TARGET_BLOCKING_ELEMENT": (0x0E7, 1, False),
            "AI_MOVE_CAMERA": (0x0F4, 7, False),
            "TRACE_BATTLE_CAMERA_ORIGINAL": (0x0F5, 0, False),
            "AI_ENABLE_COMMAND_STATE_FLAG": (0x0F6, 0, False),
            "AI_QUEUE_ACTOR_COMMAND_SOUND": (0x0FA, 0, False),
            "TRACE_BATTLE_CAMERA_TWO_SHOT": (0x0FB, 0, False),
            "TRACE_BATTLE_CAMERA_OBSTRUCTION": (0x0FC, 0, False),
            "CALC_SET_RESULT": (0x16C, 1, False),
            "CALC_SOURCE_LEVEL": (0x16D, 0, True),
            "CALC_TARGET_LEVEL": (0x16E, 0, True),
            "CALC_SOURCE_STAT": (0x16F, 1, True),
            "CALC_TARGET_STAT": (0x170, 1, True),
            "CALC_ACTION_HIT_LEVEL": (0x171, 0, True),
            "CALC_ACTION_AILMENT_LEVEL": (0x172, 0, True),
            "CALC_ACTION_POWER": (0x173, 0, True),
            "CALC_ACTION_MAGIC_BASE": (0x174, 0, True),
            "CALC_SOURCE_FLAG_20_CLEAR": (0x175, 0, True),
            "CALC_SOURCE_ACTION_AFFINITY": (0x176, 0, True),
            "CALC_TARGET_ACTION_AFFINITY": (0x177, 0, True),
            "CALC_SOURCE_ATTACK_AFFINITY": (0x178, 0, True),
            "CALC_TARGET_ATTACK_AFFINITY": (0x179, 0, True),
            "CALC_RANDOM_SCALE": (0x17A, 1, True),
            "CALC_CURRENT_RESULT": (0x17B, 0, True),
            "CALC_ACTION_MAGIC_LIMIT": (0x17C, 0, True),
            "CALC_GROUP_AVERAGE_LEVEL": (0x17D, 1, True),
            "CALC_GROUP_AVERAGE_STAT": (0x17E, 2, True),
            "CALC_ENCOUNTER_ZONE_FACTOR": (0x17F, 0, True),
            "CALC_ESCAPE_BONUS_COUNTER": (0x180, 0, True),
            "CALC_SOURCE_HP": (0x181, 0, True),
            "CALC_TARGET_HP": (0x182, 0, True),
            "CALC_SOURCE_MAX_HP": (0x183, 0, True),
            "CALC_TARGET_MAX_HP": (0x184, 0, True),
            "CALC_LEVEL_MAX_HP_FACTOR": (0x186, 0, True),
            "CALC_LEVEL_MAX_MP_FACTOR": (0x187, 0, True),
            "CALC_TARGET_HP_BAND_FACTOR": (0x188, 0, True),
            "CALC_LEVEL_FACTOR_360": (0x189, 0, True),
            "CALC_LEVEL_FACTOR_4EC": (0x18A, 0, True),
            "CALC_ROLL_TARGET_FLAG_RESULT": (0x18B, 0, True),
            "CALC_ACTION_DEATH_TYPE": (0x18C, 0, True),
            "CALC_LEVEL_CRITICAL_FACTOR": (0x18D, 0, True),
            "CALC_LEVEL_RECOVERY_FACTOR": (0x18E, 0, True),
            "CALC_SOURCE_ROSTER_BASE": (0x195, 0, True),
            "CALC_TARGET_HP_FINE_FACTOR": (0x1A5, 0, True),
            "CALC_SOURCE_ATTACK_POWER": (0x1D0, 0, True),
            "CALC_GROUP_AVERAGE_MAX_HP": (0x1D2, 1, True),
            "CALC_GROUP_AVERAGE_HP": (0x1D3, 1, True),
        }
        dds2_only = {
            "CALC_MONEY_BASE": (0x05A, 0, True),
            "CALC_MONEY_LEVEL_FACTOR": (0x162, 0, True),
        }
        dds2_field_only = {
            "APPLY_ROOM_MODE_GROUP_ZERO": (0x1FC, 4, False),
            "APPLY_ROOM_MODE_GROUP_ONE": (0x1FD, 4, False),
            "RESET_FIELD_AFTER_EVENT": (0x21A, 0, False),
        }
        ai_expected = {
            "AI_SELECT_BASIC_ATTACK": (0x030, 0, False),
            "AI_SELECT_ESCAPE": (0x031, 0, False),
            "AI_SELECT_WAIT": (0x032, 0, False),
            "AI_SELECT_SKILL": (0x033, 1, False),
            "AI_SELECT_ACTION_TARGETS": (0x034, 0, False),
            "AI_SELECT_LOWEST_HP_TARGET": (0x035, 0, False),
            "AI_SELECT_TARGETS_WITH_ACTION_MASK": (0x036, 1, False),
            "AI_SELECT_TARGET_BY_ID": (0x037, 1, False),
            "AI_ANY_PLAYER_HAS_ACTION": (0x039, 1, True),
            "AI_ANY_ENEMY_HAS_ACTION": (0x03A, 1, True),
            "AI_SET_BATTLE_REQUEST_ARGUMENT": (0x03B, 1, False),
            "AI_SELECT_TABLE_ACTION": (0x03D, 0, False),
            "AI_CLEAR_SCENE_TRANSITION": (0x03E, 0, False),
            "AI_BEGIN_SCENE_TRANSITION": (0x03F, 0, False),
            "AI_UNIT_HP_AT_OR_BELOW_RATE": (0x07B, 1, True),
            "AI_BOSS_HP_AT_OR_BELOW_RATE": (0x07C, 1, True),
            "AI_SELECT_LOWEST_HP_RATE_TARGET": (0x07D, 0, False),
            "AI_PLAYER_COUNT_AT_MOST": (0x07F, 1, True),
            "AI_HAS_OTHER_ENEMY_UNIT_MODE": (0x085, 1, True),
            "AI_ANY_PLAYER_PASSES_ACTION_CHECK": (0x086, 1, True),
            "AI_ANY_ENEMY_PASSES_ACTION_CHECK": (0x087, 1, True),
            "AI_NO_PLAYER_BLOCKS_QUERY": (0x089, 1, True),
            "AI_UNIT_PASSES_ACTION_TEN_CHECK": (0x08C, 0, True),
            "START_SCREEN_QUAKE": (0x0AB, 2, False),
            "AI_ACTOR_HISTORY_COUNTER": (0x14C, 0, True),
            "AI_SELECT_LOWEST_LEVEL_TARGET": (0x15B, 0, False),
            "AI_ANY_PLAYER_PASSES_QUERY": (0x19A, 1, True),
            "AI_ANY_ENEMY_PASSES_QUERY": (0x19B, 1, True),
            "AI_ALL_PLAYERS_PASS_QUERY": (0x19C, 1, True),
            "AI_ALL_ENEMIES_PASS_QUERY": (0x19D, 1, True),
            "AI_ACTOR_AVAILABLE_WITH_STAT_FLAG_2000": (0x1A0, 0, True),
            "AI_BATTLE_READY_WITH_ZERO_TURNS": (0x1A2, 0, True),
            "AI_CONTEXT_FLAG_TWO_SET": (0x1A3, 0, True),
            "AI_SPECIAL_MODE_EFFECT_VALUE": (0x1A6, 0, True),
            "AI_UNIT_ACTION_MODE_ZERO": (0x1A7, 1, True),
            "AI_ANY_PLAYER_ACTION_MODE_ZERO": (0x1A9, 1, True),
            "AI_ANY_ENEMY_ACTION_MODE_ZERO": (0x1AB, 1, True),
            "AI_SELECT_PLAYER_TARGET_WITHOUT_FLAG_1000": (0x1AF, 0, False),
            "AI_APPEND_SELF_TO_TARGETS": (0x1B0, 0, False),
            "AI_SELECT_OTHER_TARGET_OR_SELF": (0x1B1, 0, False),
            "AI_EFFECT_ACTIVE": (0x1B6, 0, True),
            "AI_BATTLE_PHASE": (0x1B7, 0, True),
            "AI_ANY_PLAYER_NOT_ACTION_MODE_ZERO": (0x1B8, 1, True),
            "AI_SELECT_TARGETS_WITHOUT_ACTION_MASK": (0x1BA, 1, False),
            "AI_EFFECT_VALUE": (0x1BD, 0, True),
            "AI_SCENE_FADE_COUNT": (0x1BE, 0, True),
            "AI_ALL_PLAYERS_LACK_FLAG_1000": (0x1BF, 0, True),
            "AI_ALL_PLAYERS_HAVE_FLAG_1000": (0x1C0, 0, True),
            "AI_SELECT_PLAYER_TARGET_WITH_FLAG_1000": (0x1C1, 0, False),
            "AI_APPEND_EFFECT_ACTOR_TO_TARGETS": (0x1C4, 0, False),
            "AI_GLOBAL_HISTORY_COUNTER": (0x1C5, 0, True),
            "AI_ANY_PLAYER_BLOCKS_ELEMENT": (0x1C6, 1, True),
            "AI_SELECT_TARGETS_BLOCKING_ELEMENT": (0x1C7, 1, False),
            "AI_ANY_ENEMY_HAS_QUEUED_ACTION": (0x1C8, 1, True),
            "AI_ANY_PLAYER_HAS_QUEUED_ACTION": (0x1C9, 1, True),
            "AI_SET_ACTOR_UNIT_PARAMETER": (0x1CA, 1, False),
            "AI_SELECT_HIGHEST_MP_TARGET": (0x1CB, 0, False),
            "AI_SELECT_TARGET_PASSING_QUERY": (0x1CC, 1, False),
            "AI_CLEAR_SPECIAL_ENEMY_ENTRY_FLAGS": (0x1D1, 0, False),
            "AI_QUEUE_UNBOUND_COMMAND_SOUND": (0x1D4, 0, False),
            "AI_SET_CAMERA_BLEND_START": (0x1D5, 7, False),
            "AI_SET_CAMERA_BLEND_END": (0x1D6, 7, False),
            "AI_RUN_CAMERA_BLEND": (0x1D7, 2, False),
        }
        dds1_ai_expected = {
            "AI_ENEMY_COUNT_AT_MOST": (0x07E, 1, True),
            "AI_ANY_ENEMY_HAS_ACTION_MASK": (0x081, 1, True),
            "AI_ANY_PLAYER_HAS_ACTION_MASK": (0x082, 1, True),
            "AI_ALL_PLAYERS_HAVE_ACTION_MASK": (0x083, 1, True),
            "AI_UNIT_MP_AT_OR_BELOW_RATE": (0x14B, 1, True),
            "AI_ENEMY_HAS_ACTION": (0x19E, 1, True),
            "AI_ANY_ENEMY_CURRENT_ACTION_MATCHES": (0x1AD, 1, True),
            "AI_ANY_PLAYER_CURRENT_ACTION_MATCHES": (0x1BB, 1, True),
        }
        dds2_ai_expected = {
            "AI_SELECT_WEIGHTED_TABLE_ENTRY": (0x01D, 1, False),
            "AI_HAS_UNIT_OR_SLOT_ACTION_MASK": (0x020, 2, True),
            "AI_SET_CONTEXT_FLAG_ONE": (0x05B, 0, False),
            "AI_ACTOR_CAN_USE_ACTION": (0x05C, 1, True),
            "AI_ENEMY_COUNT_AT_MOST": (0x07E, 1, True),
            "AI_UNIT_HAS_ACTION_MASK": (0x080, 1, True),
            "AI_ANY_PLAYER_HAS_ACTION_MASK": (0x082, 1, True),
            "AI_ALL_PLAYERS_HAVE_ACTION_MASK": (0x083, 1, True),
            "AI_HAS_PLAYER_UNIT_MODE": (0x084, 1, True),
            "AI_TURN_COUNT": (0x0E0, 0, True),
            "AI_SELECT_DIRECT_ACTION": (0x0E1, 1, False),
            "AI_LINKED_ACTION_SCENE_ACTIVE": (0x122, 0, True),
            "AI_HAS_ELIGIBLE_QUEUED_SPECIAL_ACTION": (0x141, 0, True),
            "AI_HAS_FLAG_800000": (0x151, 1, True),
            "AI_ROLL_ONE_BASED_BUCKET": (0x15A, 0, True),
            "AI_ANY_PLAYER_LACKS_FLAG_1000": (0x1AE, 0, True),
            "AI_ANY_ENEMY_NOT_ACTION_MODE_ZERO": (0x1B9, 1, True),
            "AI_ACTIVE_SUBTASK": (0x1D8, 0, True),
            "AI_SUBTASK_TARGET_MODE": (0x1D9, 0, True),
            "AI_SELECT_TARGETS_BY_UNIT_MODE": (0x1DA, 1, False),
            "AI_SPECIAL_BATTLE_OBJECT_VALUE": (0x1DB, 0, True),
            "AI_APPEND_CURRENT_UNIT_TO_TARGETS": (0x1DC, 0, False),
            "AI_MARKED_ACTION_SCENE_ACTIVE": (0x1DE, 0, True),
            "AI_SET_SPECIAL_EFFECT_ACTOR_BYTE": (0x1E9, 1, False),
        }
        shared_expected = {
            name: contract for name, contract in expected.items()
            if not name.startswith("CALC_")
        }
        battle_expected = {
            name: contract for name, contract in expected.items()
            if name.startswith("CALC_")
        }
        aicalc_shared_expected = {
            name: contract for name, contract in shared_expected.items()
            if name != "SETUP_FADE_FRAMES"
        }
        for profile in (flw0_profiles.DDS1, flw0_profiles.DDS2):
            with self.subTest(profile=profile.name):
                commands = {
                    command.name: (
                        command.command_id,
                        command.stack_pop,
                        command.writes_result,
                    )
                    for command in profile.commands
                }
                profile_expected = shared_expected.copy()
                if profile.name == "dds2":
                    profile_expected.update(dds2_field_only)
                self.assertEqual(commands, profile_expected)
                treasure_fields = profile.by_name[
                    "READ_TREASURE_TABLE_VALUE"
                ].symbols_for_argument(0)
                self.assertIsNotNone(treasure_fields)
                self.assertEqual(
                    treasure_fields.by_value,
                    {
                        0: "CONTENT_KIND",
                        1: "ITEM_ID",
                        2: "ITEM_QUANTITY",
                        3: "TRAP_KIND",
                        4: "AMOUNT",
                    },
                )
                selector_domains = {
                    "SET_CONTROLLER_VIBRATION": {
                        0: "SMALL_MOTOR",
                        1: "LARGE_MOTOR",
                    },
                    "APPLY_PARTY_TRAP_EFFECT": {
                        1: "DAMAGE_TEN_PERCENT_HP",
                        2: "DAMAGE_HALF_HP",
                        3: "REDUCE_HP_TO_ONE",
                        4: "INFLICT_POISON",
                        5: "INFLICT_ACHE",
                        6: "INFLICT_CLOSE",
                    },
                    "CONSUME_FIELD_SKILL_END_NOTICE": {
                        0: "LIGHTOMA",
                        1: "LIFTOMA",
                        2: "RIBERAMA",
                        3: "ESTOMA",
                    },
                    "READ_SUCTION_WARP_VALUE": {
                        0: "STATE_CODE",
                        1: "SOURCE_VECTOR_ID",
                        2: "EFFECT_UNIT_ID",
                        3: "MAP_ENTRY_ID",
                        4: "MOTION_ID",
                    },
                    "READ_BARRIER_VALUE": {
                        0: "BARRIER_MODEL_FLAG",
                        1: "EFFECT_UNIT_ID",
                        2: "SOURCE_VECTOR_ID",
                        5: "COMPLETION_FLAG",
                        6: "MAP_ENTRY_ID",
                    },
                    "READ_ELEVATOR_TABLE_VALUE": {
                        0: "DESTINATION_COUNT",
                        14: "REMAINING_DESTINATIONS",
                    },
                    "READ_LADDER_TABLE_VALUE": {
                        0: "DIRECTION",
                        1: "SOURCE_VECTOR_ID",
                        2: "EFFECT_UNIT_ID",
                        3: "USE_DIRECT_ACTION_WINDOW",
                    },
                    "READ_DOOR_WARP_VALUE": {
                        0: "MOTION_DURATION",
                        1: "FADE_MODE",
                    },
                    "RUN_FIELD_TRANSITION_SELECTOR": {
                        900: "HEAL_FACILITY",
                        901: "SAVE_POINT",
                    },
                }
                for command_name, expected_symbols in selector_domains.items():
                    with self.subTest(command=command_name):
                        symbols = profile.by_name[
                            command_name
                        ].symbols_for_argument(0)
                        self.assertIsNotNone(symbols)
                        self.assertEqual(symbols.by_value, expected_symbols)
                field_info_columns = profile.by_name[
                    "READ_FIELD_INTERACTION_VALUE"
                ].symbols_for_argument(1)
                self.assertIsNotNone(field_info_columns)
                self.assertEqual(
                    field_info_columns.by_value,
                    {0: "ROW_TYPE", 1: "MESSAGE_ID"},
                )

        for profile in (
            flw0_profiles.DDS1_AICALC,
            flw0_profiles.DDS2_AICALC,
        ):
            with self.subTest(profile=profile.name):
                commands = {
                    command.name: (
                        command.command_id,
                        command.stack_pop,
                        command.writes_result,
                    )
                    for command in profile.commands
                }
                self.assertEqual(
                    commands,
                    aicalc_shared_expected
                    | battle_expected
                    | (dds2_only if profile.name == "dds2-aicalc" else {})
                    | ai_expected
                    | (
                        dds2_ai_expected
                        if profile.name == "dds2-aicalc"
                        else dds1_ai_expected
                    ),
                )

    def test_dds_event_namespaces_match_maintained_sources(self) -> None:
        root = TOOLS.parent
        for profile in (flw0_profiles.DDS1, flw0_profiles.DDS2):
            with self.subTest(profile=profile.name):
                source_dir = root / f"src/{profile.name}/scripts/event"
                event_ids = {
                    int(path.stem[1:])
                    for path in source_dir.glob("e*.bfasm")
                    if path.stem[1:].isdigit()
                }
                self.assertEqual(profile.event_ids, event_ids)

    def test_reading_view_lifts_verified_commands_and_result_flow(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e670.bfasm"
        source = path.read_text(encoding="utf-8")
        self.assertEqual(flw0_view.source_profile_name(source), "dds1")
        view = flw0_view.render(flw0.parse_source(source), "dds1")
        self.assertIn("procedure e670_001 @ 0x0000", view)
        self.assertIn("0004: push 1", view)
        self.assertIn("0005: push 670", view)
        self.assertIn("0006: result = CREATE_POLYGON_MOVIE(670, 1)", view)
        self.assertIn("0007: push result", view)
        self.assertIn("0008: WAIT_FOR_TASK_REMOVAL(result)", view)
        self.assertIn("000c: return_or_end", view)

    def test_reading_view_cli_uses_the_source_profile(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e670.bfasm"
        result = subprocess.run(
            [sys.executable, str(TOOLS / "flw0.py"), "view", str(path)],
            capture_output=True,
            text=True,
        )
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("profile dds1", result.stdout)
        self.assertIn("CREATE_POLYGON_MOVIE(670, 1)", result.stdout)

        semantic = subprocess.run(
            [
                sys.executable,
                str(TOOLS / "flw0.py"),
                "view",
                "--semantic",
                str(path),
            ],
            capture_output=True,
            text=True,
        )
        self.assertEqual(semantic.returncode, 0, semantic.stderr)
        self.assertIn("FLW0 semantic reading view", semantic.stdout)
        self.assertIn("result = CREATE_POLYGON_MOVIE(670, 1)", semantic.stdout)
        self.assertIn("WAIT_FOR_TASK_REMOVAL(result)", semantic.stdout)
        self.assertNotIn("push 670", semantic.stdout)
        self.assertNotIn("push result", semantic.stdout)

        structured = subprocess.run(
            [
                sys.executable,
                str(TOOLS / "flw0.py"),
                "view",
                "--structured",
                str(path),
            ],
            capture_output=True,
            text=True,
        )
        self.assertEqual(structured.returncode, 0, structured.stderr)
        self.assertIn("FLW0 structured reading view", structured.stdout)

    def test_semantic_view_keeps_values_before_unknown_consumers(self) -> None:
        code = [
            flw0.OPCODE_IDS["PROC"],
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x123 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(
            flw0.parse(_fixture(code)), "dds1", semantic=True
        )
        self.assertIn("0001: push 9", view)
        self.assertIn("0002: COMM 0x0123  # unknown stack effect", view)

    def test_reading_view_is_conservative_after_unknown_command(self) -> None:
        code = [
            7,
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x123 << 16) | flw0.OPCODE_IDS["COMM"],
            (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds1")
        self.assertIn("0002: COMM 0x0123  # unknown stack effect", view)
        self.assertIn("0004: WAIT_FOR_TIMER_LIMIT(4)", view)
        self.assertNotIn("WAIT_FOR_TIMER_LIMIT(9)", view)

    def test_reading_view_result_state_stops_at_control_transfer(self) -> None:
        code = [
            7,
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (670 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x0AA << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["GOTO"],
            flw0.OPCODE_IDS["PUSHREG"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds1")
        self.assertIn("0003: result = CREATE_POLYGON_MOVIE(670, 1)", view)
        self.assertIn("0005: push result<?>", view)

    def test_reading_view_joins_stack_and_result_across_labels(self) -> None:
        stack_code = [
            flw0.OPCODE_IDS["PROC"],
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["IF"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            (2 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["ADD"],
            flw0.OPCODE_IDS["END"],
        ]
        stack_view = flw0_view.render(
            flw0.parse(
                _fixture(stack_code, jump_rows=(("else", 5), ("join", 6)))
            ),
            "dds1",
        )
        self.assertIn("0007: push (2 + 9)", stack_view)

        result_code = [
            flw0.OPCODE_IDS["PROC"],
            (0x068 << 16) | flw0.OPCODE_IDS["COMM"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["IF"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            flw0.OPCODE_IDS["PUSHREG"],
            flw0.OPCODE_IDS["END"],
        ]
        result_view = flw0_view.render(
            flw0.parse(
                _fixture(result_code, jump_rows=(("else", 5), ("join", 6)))
            ),
            "dds1",
        )
        self.assertIn("0006: push result", result_view)

    def test_reading_view_discards_disagreeing_join_values(self) -> None:
        code = [
            flw0.OPCODE_IDS["PROC"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["IF"],
            (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (2 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["ADD"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(
            flw0.parse(_fixture(code, jump_rows=(("else", 5), ("join", 6)))),
            "dds1",
        )
        self.assertIn("0007: push (2 + <?>)", view)

    def test_semantic_view_folds_equal_values_from_both_branch_arms(self) -> None:
        code = [
            flw0.OPCODE_IDS["PROC"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["IF"],
            (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (1 << 16) | flw0.OPCODE_IDS["GOTO"],
            (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(
            flw0.parse(_fixture(code, jump_rows=(("else", 5), ("join", 6)))),
            "dds1",
            semantic=True,
        )
        self.assertIn("0006: WAIT_FOR_TIMER_LIMIT(3)", view)
        self.assertNotIn("0003: push 3", view)
        self.assertNotIn("0005: push 3", view)

    def test_structured_view_recovers_canonical_if_and_loop(self) -> None:
        if_code = [
            flw0.OPCODE_IDS["PROC"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["IF"],
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["GOTO"],
            flw0.OPCODE_IDS["END"],
        ]
        if_view = flw0_view.render(
            flw0.parse(_fixture(if_code, jump_rows=(("done", 6),))),
            "dds1",
            structured=True,
        )
        self.assertIn("0002: if (1) {", if_view)
        self.assertIn("0004: WAIT_FOR_TIMER_LIMIT(9)", if_view)
        self.assertIn("0005: }", if_view)
        self.assertNotIn("goto done", if_view)

        loop_code = [
            flw0.OPCODE_IDS["PROC"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHLIX"],
            (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["LT"],
            (1 << 16) | flw0.OPCODE_IDS["IF"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x00E << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["GOTO"],
            flw0.OPCODE_IDS["END"],
        ]
        loop_view = flw0_view.render(
            flw0.parse(
                _fixture(loop_code, jump_rows=(("again", 1), ("done", 8)))
            ),
            "dds1",
            structured=True,
        )
        self.assertIn("0004: while ((3 < local_int[1])) {", loop_view)
        self.assertIn("0006: WAIT_FOR_TIMER_LIMIT(1)", loop_view)
        self.assertIn("0007: }", loop_view)
        self.assertNotIn("goto again", loop_view)

    def test_verified_nonwriter_preserves_result_state(self) -> None:
        code = [
            7,
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (670 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x0AA << 16) | flw0.OPCODE_IDS["COMM"],
            (0x043 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds1")
        self.assertIn("0004: RESET_DRAW_EFFECTS()", view)
        self.assertIn("0005: push result", view)

    def test_reading_view_lifts_expression_and_false_branch(self) -> None:
        code = [
            7,
            (2 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (5 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            flw0.OPCODE_IDS["LT"],
            flw0.OPCODE_IDS["IF"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds1")
        self.assertIn("0003: push (5 < 2)", view)
        self.assertIn("0004: if !((5 < 2)) goto jump_label[0x0000]", view)

    def test_reading_view_handles_every_tracked_script(self) -> None:
        source_dir = TOOLS.parent / "src/dds1/scripts"
        type5_uses = 0
        command_uses = 0
        profiled_command_uses = 0
        for path in sorted(source_dir.rglob("*.bfasm")):
            with self.subTest(source=path.relative_to(source_dir)):
                source = path.read_text(encoding="utf-8")
                script = flw0.parse_source(source)
                profile_name = flw0_view.source_profile_name(source)
                self.assertEqual(profile_name, "dds1")
                view = flw0_view.render(script, profile_name)
                self.assertIn("FLW0 reading view", view)
                self.assertIn("messages ", view)
                strings = flw0_view.type5_strings(script)
                for word in _instruction_words(script):
                    if word.opcode == flw0.OPCODE_IDS["COMM"]:
                        command_uses += 1
                        if word.operand_u16 in flw0_profiles.DDS1.by_id:
                            profiled_command_uses += 1
                    if word.opcode == flw0.OPCODE_IDS["PUSHTYPE5"]:
                        type5_uses += 1
                        self.assertIn(word.operand_u16, strings)
                        self.assertIn(
                            f"type5_ref(0x{word.operand_u16:04x}, ",
                            view,
                        )
        self.assertEqual(type5_uses, 7863)
        self.assertEqual(command_uses, 53389)
        self.assertEqual(profiled_command_uses, 51421)

    def test_dds2_reading_view_uses_shared_stack_contracts(self) -> None:
        code = [
            7,
            *((value << 16) | flw0.OPCODE_IDS["PUSHIS"] for value in range(1, 6)),
            (0x073 << 16) | flw0.OPCODE_IDS["COMM"],
            (12 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x094 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            (7 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x007 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            (0x00D << 16) | flw0.OPCODE_IDS["COMM"],
            (0 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (30 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x010 << 16) | flw0.OPCODE_IDS["COMM"],
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x05E << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            (0 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (202 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (101 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x06B << 16) | flw0.OPCODE_IDS["COMM"],
            *(
                (value << 16) | flw0.OPCODE_IDS["PUSHIS"]
                for value in range(1, 5)
            ),
            (0x103 << 16) | flw0.OPCODE_IDS["COMM"],
            (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x114 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            (0x115 << 16) | flw0.OPCODE_IDS["COMM"],
            (0x116 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds2")
        self.assertIn("PREPARE_UNIT_MOTION_STATE(5, 4, 3, 2, 1)", view)
        self.assertIn("result = READ_SECONDARY_WORLD_ID_VALUE(12)", view)
        self.assertIn("push result", view)
        self.assertIn("result = TEST_MODEL_FLAG(7)", view)
        self.assertIn("WAIT_FOR_TIMER_START()", view)
        self.assertIn("SCREEN_FADE_B(30, 0)", view)
        self.assertIn("result = ACTION_WINDOW_REQUEST_AND_POLL(9)", view)
        self.assertIn("MOVE_OBJECT_ALONG_PATH(101, 202, 0)", view)
        self.assertIn("ENABLE_FIELD_MODELS(4, 3, 2, 1)", view)
        self.assertIn("result = READ_TREASURE_TABLE_VALUE(AMOUNT)", view)
        self.assertIn("MARK_CURRENT_TREASURE_OPENED()", view)
        self.assertIn("result = TEST_CURRENT_TREASURE_OPENED()", view)

    def test_semantic_view_handles_both_tracked_corpora(self) -> None:
        root = TOOLS.parent
        expected = {
            "dds1": (143, 77582, 4123),
            "dds2": (140, 59612, 1869),
        }
        for game, expected_counts in expected.items():
            files = 0
            statements = 0
            explicit_pushes = 0
            for path in sorted((root / f"src/{game}/scripts").rglob("*.bfasm")):
                source = path.read_text(encoding="utf-8")
                view = flw0_view.render(
                    flw0.parse_source(source), game, semantic=True
                )
                files += 1
                statements += len(
                    re.findall(r"^  [0-9a-f]{4}:", view, re.MULTILINE)
                )
                explicit_pushes += len(
                    re.findall(r"^  [0-9a-f]{4}: push ", view, re.MULTILINE)
                )
            self.assertEqual(
                (files, statements, explicit_pushes), expected_counts
            )

    def test_structured_view_handles_both_tracked_corpora(self) -> None:
        root = TOOLS.parent
        expected = {
            "dds1": (143, 3522, 813, 1221),
            "dds2": (140, 3497, 1263, 1068),
        }
        for game, expected_counts in expected.items():
            totals = [0, 0, 0, 0]
            for path in sorted((root / f"src/{game}/scripts").rglob("*.bfasm")):
                source = path.read_text(encoding="utf-8")
                view = flw0_view.render(
                    flw0.parse_source(source), game, structured=True
                )
                totals[0] += 1
                totals[1] += len(re.findall(r": if \(", view))
                totals[2] += len(re.findall(r": while \(", view))
                totals[3] += view.count("} else {")
            self.assertEqual(tuple(totals), expected_counts)

    def test_reading_view_uses_world_unit_stack_contracts(self) -> None:
        code = [
            7,
            (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x012 << 16) | flw0.OPCODE_IDS["COMM"],
            (4 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x049 << 16) | flw0.OPCODE_IDS["COMM"],
            (8 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (9 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x04A << 16) | flw0.OPCODE_IDS["COMM"],
            (1 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (2 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x013 << 16) | flw0.OPCODE_IDS["COMM"],
            (3 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x016 << 16) | flw0.OPCODE_IDS["COMM"],
            *(
                (value << 16) | flw0.OPCODE_IDS["PUSHIS"]
                for value in range(1, 6)
            ),
            (0x017 << 16) | flw0.OPCODE_IDS["COMM"],
            *(
                (value << 16) | flw0.OPCODE_IDS["PUSHIS"]
                for value in range(1, 8)
            ),
            (0x08B << 16) | flw0.OPCODE_IDS["COMM"],
            (6 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x0A3 << 16) | flw0.OPCODE_IDS["COMM"],
            (0x068 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["PUSHREG"],
            (10 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (20 << 16) | flw0.OPCODE_IDS["PUSHIS"],
            (0x1E0 << 16) | flw0.OPCODE_IDS["COMM"],
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds1")
        self.assertIn("ADD_EFFECT_UNIT_TO_WORLD(4)", view)
        self.assertIn("WAIT_FOR_UNIT_MOTION(4)", view)
        self.assertIn("ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR(9, 8)", view)
        self.assertIn("START_CAMERA_PATH_MOVE(2, 1)", view)
        self.assertIn("DESTROY_WORLD_UNIT(3)", view)
        self.assertIn("START_UNIT_MOVE_TO_POSITION_OBJECT(5, 4, 3, 2, 1)", view)
        self.assertIn("START_UNIT_PATH_FOLLOW(7, 6, 5, 4, 3, 2, 1)", view)
        self.assertIn("FOCUS_CAMERA_ON_OBJECT(6)", view)
        self.assertIn("result = READ_CURRENT_WORLD_OBJECT_ID()", view)
        self.assertIn("push result", view)
        self.assertIn("QUEUE_WORLD_OBJECT_PENDING_VALUE(20, 10)", view)

    def test_tracked_e670_source_assembles_exact_file(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e670.bfasm"
        source = path.read_text(encoding="utf-8")
        script = flw0.parse_source(source)
        rebuilt = script.to_bytes()
        self.assertEqual(len(rebuilt), 436)
        self.assertEqual(
            sha1(rebuilt).hexdigest(),
            "f662f1c11775216fd98f34fb034fec5f8e0e3177",
        )
        self.assertEqual(
            flw0_symbolic.render(script, "dds1", structured=True), source
        )

    def test_tracked_dds1_script_corpus_assembles_exact_hashes(self) -> None:
        root = TOOLS.parent
        source_dir = root / "src/dds1/scripts"
        manifest = root / "config/dds1/scripts.sha1"
        records = []
        for line in manifest.read_text(encoding="ascii").splitlines():
            expected, output = line.split()
            output_path = Path(output)
            relative = output_path.relative_to("build/dds1/scripts")
            records.append((expected, source_dir / relative.with_suffix(".bfasm")))

        tracked = sorted(source_dir.rglob("*.bfasm"))
        self.assertEqual(sorted(source for _, source in records), tracked)
        self.assertEqual(len(records), 143)

        versions = {1: 0, 2: 0}
        message_banks = 0
        decoded_banks = 0
        dialogs = 0
        pages = 0
        options = 0
        speakers = 0
        code_words = 0
        commands = 0
        profiled_commands = 0
        font_directives = 0
        glyph_directives = 0
        message_references = 0
        selection_references = 0
        event_references = 0
        procedure_references = 0
        short_string_counts = 0
        for expected, source in records:
            with self.subTest(source=source.relative_to(source_dir)):
                text = source.read_text(encoding="utf-8")
                font_directives += len(re.findall(r"^\s+font ", text, re.MULTILINE))
                glyph_directives += len(re.findall(r"^\s+glyphs ", text, re.MULTILINE))
                short_string_counts += "\nstrings count=" in text
                version = int(text.split(None, 2)[1])
                versions[version] += 1
                self.assertIn("\nprofile dds1\n", text)
                message_references += len(re.findall(r"\bPUSHMSG\b|\bmessage\(", text))
                selection_references += len(
                    re.findall(r"\bPUSHSELECT\b|\bselection\(", text)
                )
                event_references += len(re.findall(r"\bPUSHEVENT\b|\bevent\(", text))
                procedure_references += len(
                    re.findall(r"\bPUSHPROC\b|\bprocedure\(", text)
                )
                source_script = flw0.parse_source(text)
                rebuilt = source_script.to_bytes()
                self.assertEqual(sha1(rebuilt).hexdigest(), expected)
                script = flw0.parse(rebuilt)
                code_words += len(script.code_words())
                commands += sum(
                    word.opcode == flw0.OPCODE_IDS["COMM"]
                    for word in _instruction_words(script)
                )
                profiled_commands += sum(
                    word.opcode == flw0.OPCODE_IDS["COMM"]
                    and word.operand_u16 in flw0_profiles.DDS1.by_id
                    for word in _instruction_words(script)
                )
                message_sections = script.sections_of_type(3)
                if message_sections and (
                    message_data := script.section_bytes(message_sections[0])
                ):
                    message_banks += 1
                    try:
                        bank = msg1.decode(message_data)
                    except msg1.Msg1Error:
                        pass
                    else:
                        decoded_banks += 1
                        dialogs += len(bank.dialogs)
                        speakers += len(bank.speakers)
                        pages += sum(
                            len(dialog.pages)
                            for dialog in bank.dialogs
                            if isinstance(dialog, msg1.Message)
                        )
                        options += sum(
                            len(dialog.options)
                            for dialog in bank.dialogs
                            if isinstance(dialog, msg1.Selection)
                        )
                if version == 1:
                    self.assertEqual(flw0.render_source(source_script, "dds1"), text)
                else:
                    self.assertEqual(
                        flw0_symbolic.render(
                            source_script, "dds1", structured=True
                        ),
                        text,
                    )
        self.assertEqual(versions, {1: 14, 2: 129})
        self.assertEqual(
            (message_banks, decoded_banks, dialogs, pages, options, speakers),
            (72, 67, 2702, 3109, 1028, 184),
        )
        self.assertEqual(
            (code_words, commands, profiled_commands), (168829, 53389, 51421)
        )
        self.assertEqual((font_directives, glyph_directives), (1154, 210))
        self.assertEqual(message_references, 2368)
        self.assertEqual(selection_references, 329)
        self.assertEqual(event_references, 31)
        self.assertEqual(procedure_references, 807)
        self.assertEqual(short_string_counts, 30)

    def test_tracked_dds2_script_corpus_assembles_exact_hashes(self) -> None:
        root = TOOLS.parent
        source_dir = root / "src/dds2/scripts"
        manifest = root / "config/dds2/scripts.sha1"
        records = []
        for line in manifest.read_text(encoding="ascii").splitlines():
            expected, output = line.split()
            output_path = Path(output)
            relative = output_path.relative_to("build/dds2/scripts")
            records.append((expected, source_dir / relative.with_suffix(".bfasm")))

        tracked = sorted(source_dir.rglob("*.bfasm"))
        self.assertEqual(sorted(source for _, source in records), tracked)
        self.assertEqual(len(records), 140)

        totals = {
            "versions": {1: 0, 2: 0},
            "message_banks": 0,
            "decoded_banks": 0,
            "dialogs": 0,
            "pages": 0,
            "options": 0,
            "speakers": 0,
            "code_words": 0,
            "commands": 0,
            "profiled_commands": 0,
            "message_references": 0,
            "selection_references": 0,
            "event_references": 0,
            "procedure_references": 0,
            "font": 0,
            "glyphs": 0,
            "short_string_counts": 0,
        }
        for expected, source in records:
            with self.subTest(source=source.relative_to(source_dir)):
                text = source.read_text(encoding="utf-8")
                version = int(text.split(None, 2)[1])
                totals["versions"][version] += 1
                totals["font"] += len(re.findall(r"^\s+font ", text, re.MULTILINE))
                totals["glyphs"] += len(re.findall(r"^\s+glyphs ", text, re.MULTILINE))
                totals["short_string_counts"] += "\nstrings count=" in text
                self.assertIn("\nprofile dds2\n", text)

                script = flw0.parse_source(text)
                rebuilt = script.to_bytes()
                self.assertEqual(sha1(rebuilt).hexdigest(), expected)
                totals["code_words"] += len(script.code_words())
                totals["commands"] += sum(
                    word.opcode == flw0.OPCODE_IDS["COMM"]
                    for word in _instruction_words(script)
                )
                totals["profiled_commands"] += sum(
                    word.opcode == flw0.OPCODE_IDS["COMM"]
                    and word.operand_u16 in flw0_profiles.DDS2.by_id
                    for word in _instruction_words(script)
                )
                totals["message_references"] += len(
                    re.findall(r"\bPUSHMSG\b|\bmessage\(", text)
                )
                totals["selection_references"] += len(
                    re.findall(r"\bPUSHSELECT\b|\bselection\(", text)
                )
                totals["event_references"] += len(
                    re.findall(r"\bPUSHEVENT\b|\bevent\(", text)
                )
                totals["procedure_references"] += len(
                    re.findall(r"\bPUSHPROC\b|\bprocedure\(", text)
                )
                message_sections = script.sections_of_type(3)
                if message_sections and (
                    message_data := script.section_bytes(message_sections[0])
                ):
                    totals["message_banks"] += 1
                    try:
                        bank = msg1.decode(message_data)
                    except msg1.Msg1Error:
                        pass
                    else:
                        totals["decoded_banks"] += 1
                        totals["dialogs"] += len(bank.dialogs)
                        totals["speakers"] += len(bank.speakers)
                        totals["pages"] += sum(
                            len(dialog.pages)
                            for dialog in bank.dialogs
                            if isinstance(dialog, msg1.Message)
                        )
                        totals["options"] += sum(
                            len(dialog.options)
                            for dialog in bank.dialogs
                            if isinstance(dialog, msg1.Selection)
                        )
                if version == 1:
                    self.assertEqual(flw0.render_source(script, "dds2"), text)
                else:
                    self.assertEqual(
                        flw0_symbolic.render(script, "dds2", structured=True), text
                    )

        self.assertEqual(totals["versions"], {1: 14, 2: 126})
        self.assertEqual(
            (
                totals["message_banks"],
                totals["decoded_banks"],
                totals["dialogs"],
                totals["pages"],
                totals["options"],
                totals["speakers"],
            ),
            (66, 59, 2437, 2673, 1149, 177),
        )
        self.assertEqual(
            (totals["code_words"], totals["commands"]), (123441, 38839)
        )
        self.assertEqual(
            (
                totals["profiled_commands"],
                totals["message_references"],
                totals["selection_references"],
            ),
            (37752, 1910, 287),
        )
        self.assertEqual(totals["event_references"], 46)
        self.assertEqual(totals["procedure_references"], 463)
        self.assertEqual((totals["font"], totals["glyphs"]), (433, 159))
        self.assertEqual(totals["short_string_counts"], 22)


if __name__ == "__main__":
    unittest.main()

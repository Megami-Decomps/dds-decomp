#!/usr/bin/env python3
"""Synthetic regression tests for tools/flw0.py."""

from __future__ import annotations

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
import flw0_symbolic


def _named_row(name: str, start_pc: int, reserved: int = 0) -> bytes:
    row = bytearray(0x20)
    encoded = name.encode("ascii")
    row[: min(len(encoded), 0x17)] = encoded[:0x17]
    struct.pack_into("<II", row, 0x18, start_pc, reserved)
    return bytes(row)


def _fixture(code_words: list[int] | None = None) -> bytes:
    """Build the significant shape of a small DDS event script."""

    if code_words is None:
        code_words = [7, (42 << 16) | 29, 9]
    sections = [
        (0, 0x20, 1),
        (1, 0x20, 0),
        (2, 4, len(code_words)),
        (3, 1, 0),
        (4, 1, 0xF0),
    ]
    table_end = 0x20 + len(sections) * 0x10
    proc = _named_row("synthetic_001", 0)
    code = b"".join(struct.pack("<I", word) for word in code_words)
    string_padding = bytes(0xF0)
    offsets = [
        table_end,
        table_end + len(proc),
        table_end + len(proc),
        table_end + len(proc) + len(code),
        table_end + len(proc) + len(code),
    ]
    physical_size = table_end + len(proc) + len(code) + len(string_padding)
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
    return header + table + proc + code + string_padding


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

    def test_dds1_command_profile_records_verified_stack_pops(self) -> None:
        commands = {
            command.name: (command.command_id, command.stack_pop)
            for command in flw0_profiles.DDS1.commands
        }
        self.assertEqual(
            commands,
            {
                "RESET_DRAW_EFFECTS": (0x043, 0),
                "RETURN_TO_TITLE": (0x046, 0),
                "RESET_FIELD_EFFECTS": (0x099, 0),
                "WAIT_FOR_TASK_REMOVAL": (0x0A7, 1),
                "CREATE_POLYGON_MOVIE": (0x0AA, 2),
                "CLEAR_PROCESS_CONTROL_FLAG": (0x1E7, 0),
            },
        )

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
        self.assertEqual(flw0_symbolic.render(script, "dds1"), source)


if __name__ == "__main__":
    unittest.main()

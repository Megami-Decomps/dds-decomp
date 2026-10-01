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
) -> bytes:
    """Build the significant shape of a small DDS event script."""

    if code_words is None:
        code_words = [7, (42 << 16) | 29, 9]
    sections = [
        (0, 0x20, 1),
        (1, 0x20, 0),
        (2, 4, len(code_words)),
        (3, 1, len(message_data)),
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
        table_end + len(proc) + len(code) + len(message_data),
    ]
    physical_size = (
        table_end + len(proc) + len(code) + len(message_data) + len(string_padding)
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
    return header + table + proc + code + message_data + string_padding


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
            "TEST_MODEL_FLAG": (0x007, 1, True),
            "SET_MODEL_FLAG": (0x008, 1, False),
            "CLEAR_MODEL_FLAG": (0x009, 1, False),
            "WAIT_FOR_TIMER_START": (0x00D, 0, False),
            "WAIT_FOR_TIMER_LIMIT": (0x00E, 1, False),
            "SCREEN_FADE_A": (0x00F, 2, False),
            "SCREEN_FADE_B": (0x010, 2, False),
            "RESET_DRAW_EFFECTS": (0x043, 0, False),
            "RETURN_TO_TITLE": (0x046, 0, False),
            "RESTORE_CAMERA_NODE_MODE": (0x060, 0, False),
            "RELEASE_CURRENT_OBJECT": (0x061, 0, False),
            "CALL_EVENT": (0x066, 1, False),
            "PREPARE_UNIT_MOTION_STATE": (0x073, 5, False),
            "READ_SECONDARY_WORLD_ID_VALUE": (0x094, 1, True),
            "RESET_FIELD_EFFECTS": (0x099, 0, False),
            "WAIT_FOR_TASK_REMOVAL": (0x0A7, 1, False),
            "CREATE_POLYGON_MOVIE": (0x0AA, 2, True),
            "SET_SOLAR_OVERLAY_MODE": (0x0C3, 1, False),
            "CLEAR_PROCESS_CONTROL_FLAG": (0x1E7, 0, False),
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
                self.assertEqual(commands, expected)

    def test_reading_view_lifts_verified_commands_and_result_flow(self) -> None:
        path = TOOLS.parent / "src/dds1/scripts/event/e670.bfasm"
        source = path.read_text(encoding="utf-8")
        self.assertEqual(flw0_view.source_profile_name(source), "dds1")
        view = flw0_view.render(flw0.parse_source(source), "dds1")
        self.assertIn("procedure e670_001 @ 0x0000", view)
        self.assertIn("0004: push 1", view)
        self.assertIn("0005: push 670", view)
        self.assertIn("0006: result = CREATE_POLYGON_MOVIE(1, 670)", view)
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
        self.assertIn("CREATE_POLYGON_MOVIE(1, 670)", result.stdout)

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
        self.assertIn("0003: result = CREATE_POLYGON_MOVIE(1, 670)", view)
        self.assertIn("0005: push result<?>", view)

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
        source_dir = TOOLS.parent / "src/dds1/scripts/event"
        type5_uses = 0
        command_uses = 0
        profiled_command_uses = 0
        for path in sorted(source_dir.glob("*.bfasm")):
            with self.subTest(source=path.name):
                source = path.read_text(encoding="utf-8")
                script = flw0.parse_source(source)
                profile_name = flw0_view.source_profile_name(source)
                self.assertEqual(profile_name, "dds1")
                view = flw0_view.render(script, profile_name)
                self.assertIn("FLW0 reading view", view)
                self.assertIn("messages ", view)
                strings = flw0_view.type5_strings(script)
                for word in script.code_words():
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
        self.assertEqual(type5_uses, 315)
        self.assertEqual(command_uses, 3881)
        self.assertEqual(profiled_command_uses, 2351)

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
            flw0.OPCODE_IDS["END"],
        ]
        view = flw0_view.render(flw0.parse(_fixture(code)), "dds2")
        self.assertIn("PREPARE_UNIT_MOTION_STATE(1, 2, 3, 4, 5)", view)
        self.assertIn("result = READ_SECONDARY_WORLD_ID_VALUE(12)", view)
        self.assertIn("push result", view)
        self.assertIn("result = TEST_MODEL_FLAG(7)", view)
        self.assertIn("WAIT_FOR_TIMER_START()", view)
        self.assertIn("SCREEN_FADE_B(0, 30)", view)

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

    def test_tracked_dds1_event_corpus_assembles_exact_hashes(self) -> None:
        root = TOOLS.parent
        source_dir = root / "src/dds1/scripts/event"
        manifest = root / "config/dds1/event_scripts.sha1"
        records = []
        for line in manifest.read_text(encoding="ascii").splitlines():
            expected, output = line.split()
            output_path = Path(output)
            self.assertEqual(output_path.parent, Path("build/dds1/scripts/event"))
            records.append(
                (expected, source_dir / output_path.with_suffix(".bfasm").name)
            )

        tracked = sorted(source_dir.glob("*.bfasm"))
        self.assertEqual(sorted(source for _, source in records), tracked)
        self.assertEqual(len(records), 104)

        versions = {1: 0, 2: 0}
        message_banks = 0
        dialogs = 0
        pages = 0
        options = 0
        speakers = 0
        font_directives = 0
        glyph_directives = 0
        message_references = 0
        for expected, source in records:
            with self.subTest(source=source.name):
                text = source.read_text(encoding="utf-8")
                font_directives += len(re.findall(r"^\s+font ", text, re.MULTILINE))
                glyph_directives += len(re.findall(r"^\s+glyphs ", text, re.MULTILINE))
                version = int(text.split(None, 2)[1])
                versions[version] += 1
                self.assertIn("\nprofile dds1\n", text)
                message_references += text.count("PUSHMSG ")
                rebuilt = flw0.parse_source(text).to_bytes()
                self.assertEqual(sha1(rebuilt).hexdigest(), expected)
                script = flw0.parse(rebuilt)
                message_data = script.section_bytes(script.sections[3])
                if message_data:
                    message_banks += 1
                    bank = msg1.decode(message_data)
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
                        self.assertIn("\n  msg1\n", text)
                        self.assertEqual(flw0.render_source(script, "dds1"), text)
                    else:
                        self.assertIn("\nmessages msg1\n", text)
                        self.assertEqual(flw0_symbolic.render(script, "dds1"), text)
        self.assertEqual(versions, {1: 7, 2: 97})
        self.assertEqual(
            (message_banks, dialogs, pages, options, speakers),
            (36, 179, 257, 16, 27),
        )
        self.assertEqual((font_directives, glyph_directives), (606, 2))
        self.assertEqual(message_references, 188)

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
                    for word in script.code_words()
                )
                totals["profiled_commands"] += sum(
                    word.opcode == flw0.OPCODE_IDS["COMM"]
                    and word.operand_u16 in flw0_profiles.DDS2.by_id
                    for word in script.code_words()
                )
                totals["message_references"] += len(
                    re.findall(r"\bPUSHMSG\b", text)
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
                    self.assertEqual(flw0_symbolic.render(script, "dds2"), text)

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
            (totals["code_words"], totals["commands"]), (123441, 38850)
        )
        self.assertEqual(
            (totals["profiled_commands"], totals["message_references"]),
            (23630, 1910),
        )
        self.assertEqual((totals["font"], totals["glyphs"]), (433, 159))
        self.assertEqual(totals["short_string_counts"], 22)


if __name__ == "__main__":
    unittest.main()

#!/usr/bin/env python3
"""Fail-closed regression tests for production-linked exact-report evidence.

These fixtures deliberately separate objdiff scores, compiler ownership, object
layout, linked bytes, and retail bytes. Agreement in one layer must not stand in
for evidence from another.
"""
import copy
import hashlib
import json
import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import production_report


def words(*values):
    return struct.pack(f"<{len(values)}I", *values)


def report_functions(**sizes):
    return {name: {"name": name, "size": str(size)} for name, size in sizes.items()}


def elf_bytes(functions, code, *, linked=False, text_offset=0x100):
    """Build an ELF32 fixture with a real section/symbol table and PT_LOAD.

    Keeping this writer here makes the tests independent of optional binutils
    and of the production reader being tested.
    """
    section_names = b"\0.text\0.shstrtab\0.symtab\0.strtab\0"
    strings = bytearray(b"\0")
    symbols = bytearray(16)
    for name, offset, size in functions:
        name_offset = len(strings)
        strings.extend(name.encode() + b"\0")
        symbols.extend(struct.pack("<IIIBBH", name_offset,
                                   offset + (0x1000 if linked else 0), size,
                                   0x12, 0, 1))
    data = bytearray(text_offset)
    data.extend(code)
    names_offset = len(data)
    data.extend(section_names)
    strings_offset = len(data)
    data.extend(strings)
    data.extend(b"\0" * (-len(data) % 4))
    symbols_offset = len(data)
    data.extend(symbols)
    sections_offset = len(data)
    section_rows = [
        (0,) * 10,
        (section_names.index(b".text"), 1, 6, 0x1000 if linked else 0,
         text_offset, len(code), 0, 0, 4, 0),
        (section_names.index(b".shstrtab"), 3, 0, 0, names_offset,
         len(section_names), 0, 0, 1, 0),
        (section_names.index(b".symtab"), 2, 0, 0, symbols_offset,
         len(symbols), 4, 1, 4, 16),
        (section_names.index(b".strtab"), 3, 0, 0, strings_offset,
         len(strings), 0, 0, 1, 0),
    ]
    for row in section_rows:
        data.extend(struct.pack("<10I", *row))
    identity = b"\x7fELF\x01\x01\x01" + b"\0" * 9
    struct.pack_into("<16sHHIIIIIHHHHHH", data, 0, identity,
                     2 if linked else 1, 8, 1, 0x1000 if linked else 0,
                     52 if linked else 0, sections_offset, 0,
                     52, 32 if linked else 0, 1 if linked else 0,
                     40, len(section_rows), 2)
    if linked:
        struct.pack_into("<8I", data, 52, 1, text_offset, 0x1000, 0x1000,
                         len(code), len(code), 5, 4)
    return bytes(data)


def production_elf_bytes(functions, code, image):
    """Give the relocatable link a load image containing the retail ELF bytes."""
    data = bytearray(elf_bytes(functions, code, linked=True))
    image_offset = len(data)
    data.extend(image)
    struct.pack_into("<8I", data, 52, 1, image_offset, 0x1000 - 0x140,
                     0, len(image), len(image), 5, 4)
    return bytes(data)


class ProductionFixture(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.serial = "TEST_RETAIL"
        self.elf_path = Path(f"build/dds1/{self.serial}.elf")
        self.map_path = self.elf_path.with_suffix(".map")
        self.receipt_path = Path(str(self.elf_path) + ".proof.json")
        self.object_path = "build/dds1/src/dds1/game/test.o"
        self.assembly_path = self.object_path + ".s"
        self.source_path = "src/dds1/game/test.c"
        self.functions = [("sourceA", 0, 8), ("sourceB", 8, 8), ("fallback", 16, 8)]
        self.code = words(0x03E00008, 0, 0x03E00008, 0, 0x03E00008, 0)
        self.write(self.object_path, elf_bytes(self.functions, self.code))
        self.retail = elf_bytes(self.functions, self.code, linked=True, text_offset=0x140)
        self.write(self.elf_path, production_elf_bytes(self.functions, self.code, self.retail))
        self.write(f"orig/dds1/{self.serial}", self.retail)
        self.write(f"build/dds1/{self.serial}", self.retail)
        self.write(self.map_path, f" .text 0x1000 0x18 {self.object_path}\n")
        self.write(self.source_path, "void sourceA(void) {}\nvoid sourceB(void) {}\n")
        self.assembly = (
            ".ent sourceA\nsourceA:\n jr $31\n nop\n.end sourceA\n"
            ".ent sourceB\nsourceB:\n jr $31\n nop\n.end sourceB\n"
            "#APP\n.include \"build/eeasm/asm/dds1/nonmatchings/game/test/fallback.s\"\n"
            "#NO_APP\n")
        self.write(self.assembly_path, self.assembly)
        self.include_path = "build/eeasm/asm/dds1/nonmatchings/game/test/fallback.s"
        self.write(self.include_path, ".ent fallback\nfallback:\n jr $31\n nop\n.end fallback\n")
        self.write("include/test.h", "/* compiler dependency */\n")
        self.write("include/macro.inc", "/* assembler dependency */\n")
        self.write("config/dds1/cflags.txt", "game/test -O2\n")
        self.write(self.object_path + ".d",
                   f"{self.object_path}: {self.source_path} \\\n include/test.h\n")
        self.syms = {name: 0x1000 + offset for name, offset, _ in self.functions}
        self.write("config/dds1/symbol_addrs.txt",
                   "".join(f"{name} = 0x{value:X};\n" for name, value in self.syms.items()))
        self.config_patch = patch.dict(production_report.VERSIONS, {
            "dds1": {"serial": self.serial,
                     "elf_sha1": hashlib.sha1(self.retail).hexdigest()}
        }, clear=True)
        self.config_patch.start()
        self.addCleanup(self.config_patch.stop)
        self.write("config/versions.json", json.dumps(production_report.VERSIONS))
        self.refresh_receipt()

    def write(self, relative, content):
        path = self.root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_bytes(content.encode() if isinstance(content, str) else content)

    def refresh_receipt(self):
        production_report.record_object(self.root, Path(self.source_path), Path(self.object_path))
        production_report.record_link(self.root, self.elf_path, self.map_path, [self.object_path])
        self.receipt = json.loads((self.root / self.receipt_path).read_text())

    def save_receipt(self):
        self.write(self.receipt_path, json.dumps(self.receipt))

    def proof(self):
        return production_report.Production(self.root, "dds1")

    @staticmethod
    def unit():
        return {
            "name": "game/test", "metadata": {"progress_categories": ["game"]},
            "functions": [
                {"name": "sourceA", "size": "8", "fuzzy_match_percent": 0.0},
                {"name": "sourceB", "size": "8", "fuzzy_match_percent": 75.0},
                {"name": "fallback", "size": "8"},
            ],
            "measures": {"total_code": "24", "total_functions": 3,
                         "matched_code": "0", "matched_functions": 0,
                         "matched_code_percent": 0.0, "matched_functions_percent": 0.0,
                         "fuzzy_match_percent": 25.0},
        }


class CompilerOwnershipTests(unittest.TestCase):
    def test_only_compiler_entries_are_c_owned(self):
        assembly = """
        .text
        .ent sourceFunction
sourceFunction:
        jr $31
        nop
        .end sourceFunction
#APP
        .ent fallbackFunction
fallbackFunction:
        jr $31
        nop
        .end fallbackFunction
#NO_APP
        .ent staticFunction.1
staticFunction.1:
        jr $31
        nop
        .end staticFunction.1
"""
        self.assertEqual(production_report.compiler_functions(assembly),
                         {"sourceFunction", "staticFunction.1"})

    def test_c_function_with_inline_intrinsics_remains_c_owned(self):
        assembly = """
        .ent sourceFunction
sourceFunction:
#APP
        pextlw $2,$4,$5
#NO_APP
        jr $31
        nop
        .end sourceFunction
"""
        self.assertEqual(production_report.compiler_functions(assembly), {"sourceFunction"})

    def test_empty_and_assembly_only_output_have_no_c_functions(self):
        for assembly in ("", "#APP\n.ent fallback\nfallback:\n.end fallback\n#NO_APP\n"):
            with self.subTest(assembly=assembly):
                self.assertEqual(production_report.compiler_functions(assembly), set())

    def test_duplicate_compiler_entries_do_not_disappear_into_a_set(self):
        with self.assertRaises(ValueError):
            production_report.compiler_functions(".ent duplicate\n.end duplicate\n"
                                                 ".ent duplicate\n.end duplicate\n")

    def test_unterminated_or_mispaired_provenance_is_rejected(self):
        for assembly in (".ent unfinished\n", ".ent first\n.end second\n", ".end unknown\n"):
            with self.subTest(assembly=assembly), self.assertRaises(ValueError):
                production_report.compiler_functions(assembly)

    def test_final_top_level_asm_region_may_legitimately_remain_open(self):
        assembly = ".ent source\n.end source\n#APP\n.ent fallback\n.end fallback\n"
        self.assertEqual(production_report.compiler_functions(assembly), {"source"})


class LinkMapTests(unittest.TestCase):
    def test_text_input_ownership_ignores_output_sections_and_symbols(self):
        text = """
.text           0x00001000       0x30
 .text          0x00001000       0x20 build/dds1/src/dds1/source.o
                0x00001000                sourceFunction
 .data          0x00008000       0x20 build/dds1/src/dds1/source.o
 .text          0x00001020       0x10 build/dds1/asm/fallback.o
                0x00001020                fallbackFunction
"""
        self.assertEqual(production_report.parse_link_map(text), {
            "build/dds1/src/dds1/source.o": (0x1000, 0x20),
            "build/dds1/asm/fallback.o": (0x1020, 0x10),
        })

    def test_duplicate_object_text_is_ambiguous_even_when_identical(self):
        line = " .text 0x00001000 0x20 build/source.o\n"
        with self.assertRaises(ValueError):
            production_report.parse_link_map(line + line)

    def test_different_objects_may_not_claim_overlapping_linked_text(self):
        with self.assertRaises(ValueError):
            production_report.parse_link_map(" .text 0x1000 0x20 build/source.o\n"
                                             " .text 0x1010 0x20 build/other.o\n")

    def test_discarded_sections_cannot_supply_live_link_ownership(self):
        text = ("Discarded input sections\n"
                " .text 0x0000 0x20 build/source.o\n"
                "Linker script and memory map\n"
                " .text 0x1000 0x20 build/source.o\n")
        self.assertEqual(production_report.parse_link_map(text), {"build/source.o": (0x1000, 0x20)})

    def test_empty_or_discarded_only_maps_are_rejected(self):
        for text in ("", " .text 0x1000 0x0 build/source.o\n",
                     "Discarded input sections\n .text 0x0000 0x20 build/source.o\n"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                production_report.parse_link_map(text)


class FunctionPartitionTests(unittest.TestCase):
    def setUp(self):
        self.report = report_functions(sourceA=8, sourceB=12, fallback=16)
        self.production = {"sourceA": (0, 8), "sourceB": (8, 12), "fallback": (20, 16)}
        self.c_names = {"sourceA", "sourceB"}
        self.fallback = {"fallback"}

    def validate(self):
        return production_report.validate_partition(
            self.report, self.production, self.c_names, self.fallback)

    def test_production_c_and_assembly_cover_the_whole_denominator(self):
        self.validate()

    def test_zero_objdiff_score_does_not_disqualify_production_c(self):
        self.report["sourceA"]["fuzzy_match_percent"] = 0.0
        self.validate()

    def test_unknown_report_function_fails_closed(self):
        self.report.update(report_functions(unknown=4))
        with self.assertRaises(ValueError):
            self.validate()

    def test_unreported_production_function_fails_closed(self):
        self.production["unreported"] = (36, 4)
        with self.assertRaises(ValueError):
            self.validate()

    def test_unknown_compiler_entry_fails_closed(self):
        self.c_names.add("notInObject")
        with self.assertRaises(ValueError):
            self.validate()

    def test_fallback_must_not_also_be_c_owned(self):
        self.c_names.add("fallback")
        with self.assertRaises(ValueError):
            self.validate()

    def test_unclassified_production_symbol_fails_closed(self):
        self.fallback.clear()
        with self.assertRaises(ValueError):
            self.validate()

    def test_stale_fallback_inventory_fails_closed(self):
        self.fallback.add("deletedFallback")
        with self.assertRaises(ValueError):
            self.validate()

    def test_production_and_report_sizes_must_agree(self):
        self.report["sourceA"]["size"] = "4"
        with self.assertRaises(ValueError):
            self.validate()

    def test_partition_is_independent_of_raw_comparison_score(self):
        self.report["fallback"]["fuzzy_match_percent"] = 1.0
        self.validate()

    def test_identical_address_alias_cannot_double_count_code(self):
        self.production["sourceB"] = (0, 12)
        with self.assertRaises(ValueError):
            self.validate()

    def test_nested_symbol_cannot_double_count_code(self):
        self.production["sourceB"] = (4, 12)
        with self.assertRaises(ValueError):
            self.validate()

    def test_c_and_assembly_ranges_cannot_overlap(self):
        self.production["fallback"] = (16, 16)
        with self.assertRaises(ValueError):
            self.validate()

    def test_nonpositive_function_size_is_not_a_vacuous_proof(self):
        for size in (0, -4):
            with self.subTest(size=size):
                self.production["sourceA"] = (0, size)
                self.report["sourceA"]["size"] = str(size)
                with self.assertRaises(ValueError):
                    self.validate()


class LinkedBytesTests(unittest.TestCase):
    def compare(self, linked_code, retail_code, *, address=0x1000, size=None):
        # The files deliberately place their PT_LOAD payloads at different
        # offsets. Proof must compare virtual addresses, not matching slices.
        linked = b"L" * 0x30 + linked_code + b"X" * 16
        retail = b"R" * 0x50 + retail_code + b"Y" * 16
        return production_report.verify_bytes(
            linked, [(0x1000, 0x30, len(linked_code))],
            retail, [(0x1000, 0x50, len(retail_code))],
            address, len(retail_code) if size is None else size,
        )

    def test_exact_linked_bytes_at_the_same_vma_match(self):
        self.compare(words(0x03E00008, 0), words(0x03E00008, 0))

    def test_relocation_equivalence_uses_actual_linked_instruction_bytes(self):
        # The source object's HI/LO fields could be 0 and 4. Only what the
        # production linker really emitted is evidence for the final address.
        linked = words(0x3C041234, 0x24845674)
        self.compare(linked, words(0x3C041234, 0x24845674))
        with self.assertRaises(ValueError):
            self.compare(words(0x3C040000, 0x24840004), linked)

    def test_changed_linked_address_is_not_masked_away(self):
        with self.assertRaises(ValueError):
            self.compare(words(0x3C041234, 0x24845678), words(0x3C041234, 0x24845674))

    def test_genuine_instruction_mismatch_is_not_exact(self):
        with self.assertRaises(ValueError):
            self.compare(words(0x3C051234, 0x24845674), words(0x3C041234, 0x24845674))

    def test_only_the_requested_function_span_is_compared(self):
        self.compare(words(0xDEADBEEF, 0x03E00008, 0), words(0xFEEDFACE, 0x03E00008, 0),
                     address=0x1004, size=8)

    def test_unmapped_function_address_is_rejected(self):
        with self.assertRaises(ValueError):
            self.compare(words(0, 0), words(0, 0), address=0x2000)

    def test_the_whole_span_must_be_mapped_in_both_files(self):
        for linked, retail in ((words(0), words(0, 0)), (words(0, 0), words(0))):
            with self.subTest(linked_size=len(linked), retail_size=len(retail)):
                with self.assertRaises(ValueError):
                    self.compare(linked, retail, size=8)

    def test_equal_truncated_slices_do_not_prove_missing_bytes(self):
        with self.assertRaises(ValueError):
            production_report.verify_bytes(b"\0" * 4, [(0x1000, 0, 8)],
                                           b"\0" * 4, [(0x1000, 0, 8)], 0x1000, 8)

    def test_nonpositive_span_is_rejected(self):
        for size in (0, -4):
            with self.subTest(size=size), self.assertRaises(ValueError):
                self.compare(words(0, 0), words(0, 0), size=size)

    def test_ambiguous_virtual_mapping_is_rejected(self):
        with self.assertRaises(ValueError):
            production_report.verify_bytes(words(0, 0), [(0x1000, 0, 8), (0x1000, 0, 8)],
                                           words(0, 0), [(0x1000, 0, 8)], 0x1000, 8)


class AssemblyBodyTests(unittest.TestCase):
    @staticmethod
    def assembly(asm_count=8, c_count=2):
        return (".ent routine\nroutine:\n#APP\n" + " pextlw $2,$4,$5\n" * asm_count +
                "#NO_APP\n" + " nop\n" * c_count + ".end routine\n")

    def test_asm_dominated_compiler_function_is_ineligible(self):
        self.assertEqual(production_report.ineligible_asm_bodies(
            self.assembly(), "void routine(void) {}\n"), {"routine"})

    def test_small_intrinsics_and_nonmajority_asm_are_eligible(self):
        for asm_count, c_count in ((7, 2), (8, 8), (8, 9)):
            with self.subTest(asm=asm_count, c=c_count):
                self.assertEqual(production_report.ineligible_asm_bodies(
                    self.assembly(asm_count, c_count), "void routine(void) {}\n"), set())

    def test_documented_vu_exemptions_follow_existing_unit_checker(self):
        for marker in ("libvu0: sceVu0Routine", "vu0 routine: vector transform"):
            with self.subTest(marker=marker):
                self.assertEqual(production_report.ineligible_asm_bodies(
                    self.assembly(), f"/* {marker} */\nvoid routine(void) {{}}\n"), set())


class ReceiptTests(ProductionFixture):
    def read(self):
        return production_report.read_receipt(
            self.root, self.elf_path, self.map_path, self.receipt_path)

    def test_complete_fresh_receipt_is_accepted(self):
        self.assertEqual(self.read(), self.receipt)

    def test_compile_receipt_binds_assembly_headers_and_includes(self):
        receipt = production_report.read_object_receipt(self.root, self.object_path)
        self.assertTrue({self.source_path, self.object_path, self.assembly_path,
                         "include/test.h", "include/macro.inc", self.include_path,
                         "config/dds1/cflags.txt"} <= receipt["files"].keys())
        self.assertTrue(set(receipt["files"]) <= self.receipt["files"].keys())

    def test_changed_link_evidence_or_compile_input_is_rejected(self):
        for path in (str(self.elf_path), str(self.map_path), self.object_path,
                     self.assembly_path, self.source_path, "include/test.h",
                     "include/macro.inc", self.include_path,
                     "config/dds1/cflags.txt", "config/dds1/symbol_addrs.txt"):
            with self.subTest(path=path):
                original = (self.root / path).read_bytes()
                self.write(path, original + b"\nchanged\n")
                with self.assertRaisesRegex(ValueError, "stale"):
                    self.read()
                self.write(path, original)

    def test_old_object_and_receipt_cannot_attest_replaced_compiler_assembly(self):
        self.write(self.assembly_path, self.assembly + "\n# new compiler output\n")
        with self.assertRaisesRegex(ValueError, "stale"):
            production_report.read_object_receipt(self.root, self.object_path)
        with self.assertRaisesRegex(ValueError, "stale"):
            production_report.record_link(self.root, self.elf_path, self.map_path,
                                          [self.object_path])

    def test_compile_receipt_must_name_the_objects_canonical_source(self):
        receipt_path = self.object_path + ".proof.json"
        receipt = json.loads((self.root / receipt_path).read_text())
        other_source = "src/dds1/game/other.c"
        self.write(other_source, "void different_source(void) {}\n")
        receipt["source"] = other_source
        receipt["files"][other_source] = hashlib.sha256((self.root / other_source).read_bytes()).hexdigest()
        self.write(receipt_path, json.dumps(receipt))
        with self.assertRaises(ValueError):
            production_report.read_object_receipt(self.root, self.object_path)

    def test_compile_receipt_cannot_omit_source_object_or_compiler_output(self):
        receipt_path = self.object_path + ".proof.json"
        original = json.loads((self.root / receipt_path).read_text())
        for path in (self.source_path, self.object_path, self.assembly_path):
            with self.subTest(path=path):
                receipt = copy.deepcopy(original)
                del receipt["files"][path]
                self.write(receipt_path, json.dumps(receipt))
                with self.assertRaisesRegex(ValueError, "missing"):
                    production_report.read_object_receipt(self.root, self.object_path)

    def test_separate_objdiff_base_object_keeps_the_same_canonical_source(self):
        base = "build/dds1/base/src/dds1/game/test.o"
        self.write(base, (self.root / self.object_path).read_bytes())
        self.write(base + ".s", self.assembly)
        self.write(base + ".d", f"{base}: {self.source_path} include/test.h\n")
        production_report.record_object(self.root, Path(self.source_path), Path(base))
        receipt = production_report.read_object_receipt(self.root, base)
        self.assertEqual(receipt["source"], self.source_path)
        self.assertEqual(receipt["object"], base)

    def test_duplicate_link_objects_are_rejected(self):
        self.receipt["objects"].append(self.object_path)
        self.save_receipt()
        with self.assertRaises(ValueError):
            self.read()

    def test_missing_link_receipt_fails_closed(self):
        (self.root / self.receipt_path).unlink()
        with self.assertRaises(OSError):
            self.read()

    def test_missing_compile_receipt_fails_closed(self):
        (self.root / (self.object_path + ".proof.json")).unlink()
        with self.assertRaises(OSError):
            self.read()
        with self.assertRaises(OSError):
            production_report.record_link(self.root, self.elf_path, self.map_path,
                                          [self.object_path])

    def test_omitted_compile_receipt_is_not_accepted_as_legacy_evidence(self):
        del self.receipt["files"][self.object_path + ".proof.json"]
        self.save_receipt()
        with self.assertRaises(ValueError):
            self.read()

    def test_omitted_compile_dependency_fails_closed(self):
        del self.receipt["files"]["include/test.h"]
        self.save_receipt()
        with self.assertRaises(ValueError):
            self.read()

    def test_wrong_identity_and_missing_required_hashes_fail_closed(self):
        original = copy.deepcopy(self.receipt)
        for field, value in (("schema", 99), ("elf", "build/other.elf"),
                             ("map", "build/other.map"), ("objects", [])):
            with self.subTest(field=field):
                self.receipt = copy.deepcopy(original)
                self.receipt[field] = value
                self.save_receipt()
                with self.assertRaises(ValueError):
                    self.read()
        self.receipt = copy.deepcopy(original)
        del self.receipt["files"][str(self.map_path)]
        self.save_receipt()
        with self.assertRaises(ValueError):
            self.read()

    def test_receipt_paths_cannot_escape_the_build_root(self):
        for name in ("../outside", "/tmp/outside"):
            with self.subTest(name=name):
                self.receipt["files"][name] = "0" * 64
                self.save_receipt()
                with self.assertRaisesRegex(ValueError, "path"):
                    self.read()
                del self.receipt["files"][name]


class ProductionUnitTests(ProductionFixture):
    def test_raw_zero_and_context_mismatch_are_promoted_with_full_proof(self):
        unit = self.unit()
        evidence = self.proof().unit(unit, "game/test")
        self.assertEqual([f["fuzzy_match_percent"] for f in unit["functions"]], [100.0, 100.0, 0.0])
        self.assertEqual([f["raw_fuzzy_match_percent"] for f in evidence["functions"]], [0.0, 75.0, 0.0])
        self.assertEqual([f["owner"] for f in evidence["functions"]], ["c", "c", "include_asm"])
        self.assertEqual([f["production_exact"] for f in evidence["functions"]], [True, True, False])
        self.assertNotIn("delta", evidence)
        self.assertEqual(unit["measures"]["matched_code"], "16")
        self.assertEqual(unit["measures"]["matched_functions"], 2)
        self.assertEqual(unit["measures"]["total_code"], "24")
        self.assertEqual(unit["measures"]["total_functions"], 3)

    def test_fallback_raw_exact_credit_is_removed_and_preserved_as_evidence(self):
        unit = self.unit()
        unit["functions"][2]["fuzzy_match_percent"] = 100.0
        unit["measures"].update(matched_code="8", matched_functions=1,
                                fuzzy_match_percent=58.333333)
        evidence = self.proof().unit(unit, "game/test")
        self.assertEqual(unit["functions"][2]["fuzzy_match_percent"], 0.0)
        self.assertEqual(evidence["functions"][2]["raw_fuzzy_match_percent"], 100.0)
        self.assertFalse(evidence["functions"][2]["production_exact"])
        self.assertEqual(unit["measures"]["matched_code"], "16")
        self.assertEqual(unit["measures"]["matched_functions"], 2)

    def test_text_section_score_tracks_the_proven_unit_score(self):
        unit = self.unit()
        unit["sections"] = [
            {"name": ".text", "fuzzy_match_percent": 25.0},
            {"name": ".data", "fuzzy_match_percent": 37.5},
        ]
        self.proof().unit(unit, "game/test")
        self.assertEqual(unit["sections"][0]["fuzzy_match_percent"],
                         unit["measures"]["fuzzy_match_percent"])
        self.assertEqual(unit["sections"][1]["fuzzy_match_percent"], 37.5)

    def test_checksum_match_without_compiler_ownership_does_not_promote(self):
        self.write(self.assembly_path, self.assembly.replace(".ent sourceA", "# .ent sourceA")
                   .replace(".end sourceA", "# .end sourceA"))
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "fallback"):
            self.proof().unit(self.unit(), "game/test")

    def test_asm_dominated_function_cannot_gain_c_credit(self):
        self.write(self.assembly_path, self.assembly.replace(
            "sourceA:\n jr $31\n nop\n", "sourceA:\n#APP\n" + " nop\n" * 8 + "#NO_APP\n"))
        self.refresh_receipt()
        unit = self.unit()
        evidence = self.proof().unit(unit, "game/test")
        self.assertEqual(unit["functions"][0]["fuzzy_match_percent"], 0.0)
        self.assertEqual(evidence["functions"][0]["owner"], "asm_body")
        self.assertEqual(unit["measures"]["matched_code"], "8")

    def test_duplicate_report_function_names_fail_before_dict_collapse(self):
        unit = self.unit()
        unit["functions"].append(copy.deepcopy(unit["functions"][0]))
        with self.assertRaisesRegex(ValueError, "duplicate"):
            self.proof().unit(unit, "game/test")

    def test_duplicate_object_symbol_names_fail_before_dict_collapse(self):
        self.write(self.object_path, elf_bytes(self.functions + [self.functions[0]], self.code))
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "duplicate"):
            self.proof().unit(self.unit(), "game/test")

    def test_nested_compiler_name_requires_address_size_and_linked_identity(self):
        renamed = [("sourceA.1" if n == "sourceA" else n, o, s) for n, o, s in self.functions]
        self.write(self.object_path, elf_bytes(renamed, self.code))
        self.write(self.elf_path, production_elf_bytes(renamed, self.code, self.retail))
        self.write(self.assembly_path, self.assembly.replace("sourceA", "sourceA.1"))
        self.refresh_receipt()
        evidence = self.proof().unit(self.unit(), "game/test")
        self.assertEqual(evidence["functions"][0]["production_symbols"], ["sourceA.1"])
        self.assertTrue(evidence["functions"][0]["production_exact"])

    def test_nested_suffix_is_not_a_substitute_for_linked_symbol_identity(self):
        renamed = [("sourceA.1" if n == "sourceA" else n, o, s) for n, o, s in self.functions]
        self.write(self.object_path, elf_bytes(renamed, self.code))
        self.write(self.assembly_path, self.assembly.replace("sourceA", "sourceA.1"))
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "linked symbol"):
            self.proof().unit(self.unit(), "game/test")

    def test_two_nested_names_cannot_collapse_to_one_report_name(self):
        renamed = self.functions + [("sourceA.1", 0, 8)]
        self.write(self.object_path, elf_bytes(renamed, self.code))
        self.write(self.elf_path, production_elf_bytes(renamed, self.code, self.retail))
        self.write(self.assembly_path, self.assembly + ".ent sourceA.1\n.end sourceA.1\n")
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "overlapping"):
            self.proof().unit(self.unit(), "game/test")

    def test_production_function_address_must_equal_its_retail_address(self):
        shifted = [(n, o + 4 if n == "sourceA" else o, s) for n, o, s in self.functions]
        self.write(self.object_path, elf_bytes(shifted, self.code))
        self.refresh_receipt()
        with self.assertRaises(ValueError):
            self.proof().unit(self.unit(), "game/test")

    def merged_c_unit(self):
        unit = self.unit()
        unit["functions"] = [
            {"name": "sourceA", "size": "16", "fuzzy_match_percent": 0.0},
            {"name": "fallback", "size": "8"},
        ]
        unit["measures"]["total_functions"] = 2
        unit["measures"]["fuzzy_match_percent"] = 0.0
        return unit

    def test_adjacent_c_providers_can_tile_one_retail_report_function(self):
        unit = self.merged_c_unit()
        evidence = self.proof().unit(unit, "game/test")
        self.assertEqual(evidence["functions"][0]["production_symbols"], ["sourceA", "sourceB"])
        self.assertTrue(evidence["functions"][0]["production_exact"])
        self.assertEqual(unit["measures"]["matched_code"], "16")
        self.assertEqual(unit["measures"]["matched_functions"], 1)
        self.assertEqual(unit["measures"]["total_functions"], 2)

    def test_incomplete_many_to_one_provider_coverage_is_rejected(self):
        for functions in (
            [("sourceA", 0, 4), ("sourceB", 8, 8), ("fallback", 16, 8)],
            [("sourceA", 0, 8), ("sourceB", 8, 4), ("fallback", 16, 8)],
        ):
            with self.subTest(functions=functions):
                self.write(self.object_path, elf_bytes(functions, self.code))
                self.write(self.elf_path, production_elf_bytes(functions, self.code, self.retail))
                self.refresh_receipt()
                with self.assertRaisesRegex(ValueError, "providers"):
                    self.proof().unit(self.merged_c_unit(), "game/test")

    def test_overlapping_many_to_one_provider_coverage_is_rejected(self):
        functions = [("sourceA", 0, 12), ("sourceB", 8, 8), ("fallback", 16, 8)]
        self.write(self.object_path, elf_bytes(functions, self.code))
        self.write(self.elf_path, production_elf_bytes(functions, self.code, self.retail))
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "overlapping"):
            self.proof().unit(self.merged_c_unit(), "game/test")

    def test_mixed_c_and_assembly_cannot_share_a_credited_report_span(self):
        unit = self.unit()
        unit["functions"] = [
            {"name": "sourceA", "size": "8", "fuzzy_match_percent": 0.0},
            {"name": "sourceB", "size": "16", "fuzzy_match_percent": 0.0},
        ]
        unit["measures"]["total_functions"] = 2
        unit["measures"]["fuzzy_match_percent"] = 0.0
        with self.assertRaisesRegex(ValueError, "mixed C/ASM"):
            self.proof().unit(unit, "game/test")

    def test_asm_body_provider_blocks_a_whole_merged_report_span(self):
        self.write(self.assembly_path, self.assembly.replace(
            "sourceB:\n jr $31\n nop\n", "sourceB:\n#APP\n" + " nop\n" * 8 + "#NO_APP\n"))
        self.refresh_receipt()
        unit = self.merged_c_unit()
        evidence = self.proof().unit(unit, "game/test")
        self.assertFalse(evidence["functions"][0]["production_exact"])
        self.assertEqual(evidence["functions"][0]["owner"], "asm_body")
        self.assertEqual(unit["measures"]["matched_code"], "0")

    def test_incorrect_report_denominator_is_rejected(self):
        for key, value in (("total_code", "28"), ("total_functions", 4)):
            with self.subTest(key=key):
                unit = self.unit()
                unit["measures"][key] = value
                with self.assertRaises(ValueError):
                    self.proof().unit(unit, "game/test")

    def test_raw_exact_totals_must_agree_with_function_scores(self):
        for key, value in (("matched_code", "8"), ("matched_functions", 1)):
            with self.subTest(key=key):
                unit = self.unit()
                unit["measures"][key] = value
                with self.assertRaises(ValueError):
                    self.proof().unit(unit, "game/test")

    def test_raw_fuzzy_aggregate_must_agree_with_weighted_function_scores(self):
        unit = self.unit()
        unit["measures"]["fuzzy_match_percent"] = 50.0
        with self.assertRaises(ValueError):
            self.proof().unit(unit, "game/test")

    def test_permitted_raw_rounding_error_does_not_survive_exact_recalculation(self):
        unit = self.unit()
        unit["measures"]["fuzzy_match_percent"] = 25.000007
        self.proof().unit(unit, "game/test")
        self.assertEqual(unit["measures"]["fuzzy_match_percent"], 66.666667)
        self.assertEqual(unit["measures"]["fuzzy_match_percent"],
                         unit["measures"]["matched_code_percent"])

    def test_raw_function_scores_must_be_finite_percentages(self):
        for score in (-1.0, 100.1, float("nan"), float("inf"), -float("inf")):
            with self.subTest(score=score):
                unit = self.unit()
                unit["functions"][0]["fuzzy_match_percent"] = score
                with self.assertRaises(ValueError):
                    self.proof().unit(unit, "game/test")

    def test_nonfinite_aggregate_cannot_propagate_into_exact_report(self):
        for score in (float("nan"), float("inf")):
            with self.subTest(score=score):
                unit = self.unit()
                unit["measures"]["fuzzy_match_percent"] = score
                with self.assertRaises(ValueError):
                    self.proof().unit(unit, "game/test")

    def test_linked_mismatch_cannot_hide_behind_an_old_matching_binary(self):
        linked = bytearray((self.root / self.elf_path).read_bytes())
        image_offset = struct.unpack_from("<I", linked, 56)[0]
        linked[image_offset + 0x140] ^= 1
        self.write(self.elf_path, bytes(linked))
        self.refresh_receipt()
        with self.assertRaisesRegex(ValueError, "image differ"):
            self.proof()


class ReconciliationTests(ProductionFixture):
    def report(self):
        unit = self.unit()
        empty = {"total_code": "0", "total_functions": 0,
                 "matched_code": "0", "matched_functions": 0,
                 "matched_code_percent": 0.0, "matched_functions_percent": 0.0,
                 "fuzzy_match_percent": 0.0}
        return {"units": [unit], "measures": copy.deepcopy(unit["measures"]),
                "categories": [{"id": name, "measures": copy.deepcopy(unit["measures"] if name == "game" else empty)}
                               for name in ("game", "sdk", "vu1")]}

    def test_omitted_whole_game_object_cannot_shrink_denominator(self):
        missing = "build/dds1/src/dds1/game/missing.o"
        missing_source = "src/dds1/game/missing.c"
        self.write(self.object_path, elf_bytes(self.functions[:2], self.code[:16]))
        self.write(self.assembly_path, self.assembly.split("#APP", 1)[0])
        self.write(missing, elf_bytes([("fallback", 0, 8)], self.code[16:]))
        self.write(missing_source, 'INCLUDE_ASM(int, "game/test", fallback);\n')
        self.write(missing + ".s", f'#APP\n.include "{self.include_path}"\n')
        self.write(missing + ".d", f"{missing}: {missing_source} include/test.h\n")
        self.write(self.map_path, f" .text 0x1000 0x10 {self.object_path}\n"
                                 f" .text 0x1010 0x8 {missing}\n")
        production_report.record_object(self.root, Path(self.source_path), Path(self.object_path))
        production_report.record_object(self.root, Path(missing_source), Path(missing))
        production_report.record_link(self.root, self.elf_path, self.map_path,
                                      [self.object_path, missing])
        report = self.report()
        unit = report["units"][0]
        unit["functions"] = unit["functions"][:2]
        unit["measures"].update(total_code="16", total_functions=2, fuzzy_match_percent=37.5)
        report["measures"] = copy.deepcopy(unit["measures"])
        report["categories"][0]["measures"] = copy.deepcopy(unit["measures"])
        with self.assertRaisesRegex(ValueError, "unit inventory.*game/missing"):
            production_report.reconcile(report, "dds1", self.root)

    def test_reconciliation_keeps_denominators_and_unrelated_categories(self):
        report = self.report()
        original = copy.deepcopy(report)
        evidence = production_report.reconcile(report, "dds1", self.root)
        self.assertEqual(report["measures"]["matched_code"], "16")
        self.assertEqual(report["measures"], report["categories"][0]["measures"])
        self.assertEqual(report["categories"][1:], original["categories"][1:])
        self.assertEqual(report["measures"]["total_code"], "24")
        self.assertEqual(report["measures"]["total_functions"], 3)
        self.assertEqual(evidence["metric"], "production_exact_source")
        self.assertEqual(evidence["units"][0]["functions"][0]["raw_fuzzy_match_percent"], 0.0)

    def test_combined_report_updates_game_and_version_categories(self):
        report = self.report()
        report["units"][0]["name"] = "dds1/game/test"
        report["units"][0]["metadata"]["progress_categories"].append("dds1")
        report["categories"].append({"id": "dds1", "measures": copy.deepcopy(report["measures"])})
        evidence = production_report.reconcile(report, "all", self.root, expected_versions=["dds1"])
        self.assertEqual(report["categories"][-1]["measures"], report["measures"])
        self.assertEqual(evidence["units"][0]["version"], "dds1")

    def test_combined_report_cannot_drop_a_whole_configured_game(self):
        report = self.report()
        report["units"][0]["name"] = "dds1/game/test"
        report["units"][0]["metadata"]["progress_categories"].append("dds1")
        report["categories"].append({"id": "dds1", "measures": copy.deepcopy(report["measures"])})
        # This is internally consistent DDS1-only JSON. It cannot stand in for
        # the dual-game report requested by configure, even if DDS2's category
        # and denominator have also been removed.
        with patch.dict(production_report.VERSIONS, {"dds2": {}}):
            with self.assertRaisesRegex(ValueError, "report versions"):
                production_report.reconcile(report, "all", self.root,
                                            expected_versions=["dds1", "dds2"])

    def test_combined_report_requires_external_version_inventory(self):
        with self.assertRaisesRegex(ValueError, "requires configured versions"):
            production_report.reconcile(self.report(), "all", self.root)

    def test_assembly_only_sdk_units_are_unchanged(self):
        report = self.report()
        sdk = self.unit()
        sdk["name"] = "sdk/libexample"
        sdk["metadata"]["progress_categories"] = ["sdk"]
        for function in sdk["functions"]:
            function["fuzzy_match_percent"] = 0.0
        sdk["measures"]["fuzzy_match_percent"] = 0.0
        report["units"].append(sdk)
        report["categories"][1]["measures"] = copy.deepcopy(sdk["measures"])
        report["measures"].update(total_code="48", total_functions=6, fuzzy_match_percent=12.5)
        original = copy.deepcopy(sdk)
        production_report.reconcile(report, "dds1", self.root)
        self.assertEqual(sdk, original)

    def test_duplicate_game_units_cannot_double_count_production_code(self):
        report = self.report()
        report["units"].append(copy.deepcopy(report["units"][0]))
        with self.assertRaises(ValueError):
            production_report.reconcile(report, "dds1", self.root)

    def test_aggregate_denominators_must_equal_their_units(self):
        for category, key, value in ((None, "total_code", "28"),
                                     ("game", "total_functions", 4),
                                     ("sdk", "total_code", "4")):
            with self.subTest(category=category, key=key):
                report = self.report()
                measures = (report["measures"] if category is None else
                            next(c["measures"] for c in report["categories"] if c["id"] == category))
                measures[key] = value
                with self.assertRaisesRegex(ValueError, "denominator"):
                    production_report.reconcile(report, "dds1", self.root)

    def test_final_aggregates_recompute_exact_totals_without_raw_rounding_drift(self):
        report = self.report()
        report["measures"].update(matched_code="999", matched_functions=99,
                                   fuzzy_match_percent=25.00003)
        report["categories"][0]["measures"]["fuzzy_match_percent"] = 25.00003
        production_report.reconcile(report, "dds1", self.root)
        self.assertEqual(report["measures"]["matched_code"], "16")
        self.assertEqual(report["measures"]["matched_functions"], 2)
        self.assertEqual(report["measures"]["fuzzy_match_percent"], 66.666667)
        self.assertEqual(report["measures"], report["categories"][0]["measures"])

    def test_data_only_and_linker_metadata_units_are_preserved(self):
        report = self.report()
        extras = [
            {"name": "game/only_data", "measures": {"total_data": "32", "matched_data": "16"},
             "metadata": {"progress_categories": ["game"]}},
            {"name": "linker/metadata", "measures": {"total_code": "0", "total_functions": 0},
             "functions": [], "metadata": {"progress_categories": ["game"]}},
        ]
        original = copy.deepcopy(extras)
        report["units"].extend(extras)
        evidence = production_report.reconcile(report, "dds1", self.root)
        self.assertEqual(extras, original)
        self.assertEqual(len(evidence["units"]), 1)
        self.assertEqual(report["measures"]["matched_code"], "16")

    def test_zero_code_units_cannot_claim_functions_or_matching_credit(self):
        for changes in ({"total_functions": 1}, {"matched_code": "4"}, {"matched_functions": 1}):
            with self.subTest(changes=changes):
                report = self.report()
                report["units"].append({
                    "name": "linker/bad_metadata", "measures": {"total_code": "0", **changes},
                    "metadata": {"progress_categories": ["game"]},
                })
                with self.assertRaises(ValueError):
                    production_report.reconcile(report, "dds1", self.root)
        report = self.report()
        report["units"].append({
            "name": "linker/bad_metadata", "measures": {"total_code": "0"},
            "functions": [{"name": "unexpected", "size": "4"}],
            "metadata": {"progress_categories": ["game"]},
        })
        with self.assertRaises(ValueError):
            production_report.reconcile(report, "dds1", self.root)


class LoadImageTests(unittest.TestCase):
    @staticmethod
    def elf(image=b"original payload"):
        data = bytearray(elf_bytes([], b"", linked=True))
        offset = len(data)
        data.extend(image)
        struct.pack_into("<8I", data, 52, 1, offset, 0x1000, 0,
                         len(image), len(image), 5, 4)
        return data

    def test_entire_load_image_is_compared_including_non_code_bytes(self):
        production_report.verify_linked_image(bytes(self.elf()), b"original payload")
        with self.assertRaises(ValueError):
            production_report.verify_linked_image(bytes(self.elf()), b"original payloaD")

    def test_omitted_or_extra_production_bytes_cannot_pass(self):
        for image in (b"original payloa", b"original payload extra"):
            with self.subTest(image=image), self.assertRaises(ValueError):
                production_report.verify_linked_image(bytes(self.elf()), image)

    def test_truncated_or_invalid_program_header_table_is_rejected(self):
        for offset, fmt, value in ((28, "<I", 0xFFFFFFF0), (42, "<H", 16), (44, "<H", 0)):
            with self.subTest(offset=offset):
                data = self.elf()
                struct.pack_into(fmt, data, offset, value)
                with self.assertRaises(ValueError):
                    production_report.verify_linked_image(bytes(data), b"original payload")

    def test_noncontiguous_physical_load_image_is_rejected(self):
        data = self.elf()
        struct.pack_into("<I", data, 64, 1)
        with self.assertRaises(ValueError):
            production_report.verify_linked_image(bytes(data), b"\0original payload")

    def test_overlapping_physical_load_segments_are_rejected(self):
        data = self.elf()
        data[84:116] = data[52:84]
        struct.pack_into("<H", data, 44, 2)
        with self.assertRaisesRegex(ValueError, "overlapping"):
            production_report.verify_linked_image(bytes(data), b"original payload")


if __name__ == "__main__":
    unittest.main()

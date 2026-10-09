#!/usr/bin/env python3
"""Synthetic tests; no game, compiler binary, or private dump inputs."""
import copy
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ee_gcc_role_lineage as lineage
import ee_gcc_cse_role_tracer as cse
import ee_gcc_cse_case as case_builder

SOURCE = """void target(void) {
    /* begin */
    consume();
    advance();
    /* end */
}
"""
SPEC = {"schema": 1, "function": "target",
        "roles": [{"name": "sample", "begin": "    /* begin */",
                   "end": "    /* end */"}]}
HEADER = ";; Function target\n"
DUMP = HEADER + """
(note 1 0 2 ".ci/dds2/game/unit.c" 3)
(insn 2 1 3 (set (reg:SI 84) (const_int 1)) -1 (nil) (nil))
(jump_insn 3 2 4 (set (pc) (if_then_else (ltu:SI (reg:SI 84) (const_int 2)) (label_ref 4) (pc))) -1 (nil) (nil))
(code_label 4 3 0 27 "" "" [1 uses])
"""


def sha(data):
    return hashlib.sha256(data).hexdigest()


def probe_fixture(root):
    (root / "functions/target").mkdir(parents=True)
    (root / "input.c").write_text(SOURCE)
    (root / "candidate.o").write_bytes(b"synthetic object")
    artifacts = []
    for name in lineage.REQUIRED:
        text = DUMP if name not in ("17.sched", "25.sched2") else HEADER + ";; synthetic scheduler commentary\n"
        raw = text.encode()
        (root / ("rtl." + name)).write_bytes(raw)
        extracted = lineage.extract_dump_function(text, "target")
        (root / "functions/target" / name).write_text(extracted)
        artifacts.append({"name": "rtl." + name, "sha256": sha(raw)})
    manifest = {"wrapper_returncode": 0, "cc1_succeeded": True, "assembled": True,
                "compiler": {"sha256": lineage.COMPILER}, "function": "target",
                "source": "/example/src/dds2/game/unit.c", "replacements": [],
                "compiled_source_sha256": sha(SOURCE.encode()),
                "object": {"path": "candidate.o", "sha256": sha(b"synthetic object")},
                "artifacts": artifacts}
    (root / "manifest.json").write_text(json.dumps(manifest))
    return manifest


class LineageTests(unittest.TestCase):
    def test_uid_deletion_is_not_guessed_replacement(self):
        before, _ = lineage.parse_dump(DUMP, "target")
        after, _ = lineage.parse_dump(DUMP.replace(
            "(insn 2 1 3 (set (reg:SI 84) (const_int 1)) -1 (nil) (nil))",
            "(note 2 1 3 NOTE_INSN_DELETED 0)"), "target")
        rows = lineage.role_transitions({"02.jump": before, "03.cse": after}, {2, 3}, 12)
        self.assertEqual(1, rows[0]["change_count"])
        self.assertEqual("deleted_or_absent", rows[0]["changes"][0]["change"])
        self.assertIsNone(rows[0]["changes"][0]["after"])

    def test_changed_branch_target_is_preserved(self):
        before, _ = lineage.parse_dump(DUMP, "target")
        after, _ = lineage.parse_dump(DUMP.replace("(label_ref 4)", "(label_ref 99)"), "target")
        result = lineage.role_transitions({"02.jump": before, "03.cse": after}, {3}, 12)
        change = result[0]["changes"][0]
        self.assertEqual([4], change["before"]["branch_targets"])
        self.assertEqual([99], change["after"]["branch_targets"])
        self.assertNotEqual(change["before"]["pattern_sha256"], change["after"]["pattern_sha256"])

    def test_source_anchor_and_note_coordinates(self):
        span = lineage.role_spans(SOURCE, SPEC)[0]
        self.assertEqual((2, 5), (span["first_line"], span["end_line_exclusive"]))
        records, _ = lineage.parse_dump(DUMP, "target")
        self.assertEqual(3, records[2]["source"]["line"])
        self.assertTrue(lineage.same_source(records[2]["source"]["file"],
                                           "/repo/src/dds2/game/unit.c"))
        self.assertFalse(lineage.same_source(records[2]["source"]["file"],
                                            "/repo/src/dds1/game/unit.c"))

    def test_leading_newline_anchors_exact_indentation(self):
        spec = copy.deepcopy(SPEC)
        spec["roles"][0]["begin"] = "\n    /* begin */"
        spec["roles"][0]["end"] = "\n    /* end */"
        span = lineage.role_spans(SOURCE, spec)[0]
        self.assertEqual((2, 5), (span["first_line"], span["end_line_exclusive"]))

    def test_ambiguous_anchor_fails(self):
        with self.assertRaisesRegex(ValueError, "ambiguous"):
            lineage.role_spans(SOURCE.replace("    consume();", "    /* begin */"), SPEC)

    def test_missing_anchor_fails(self):
        with self.assertRaisesRegex(ValueError, "missing"):
            lineage.role_spans(SOURCE.replace("/* end */", "/* finish */"), SPEC)

    def test_two_gcse_inventories_select_final_chain(self):
        twice = DUMP + "\n" + DUMP[len(HEADER):].replace("(const_int 1)", "(const_int 7)")
        records, meta = lineage.parse_dump(twice, "target", "08.gcse")
        self.assertEqual(2, meta["complete_inventories"])
        self.assertEqual([7], records[2]["features"]["constants"])
        self.assertEqual(4, meta["selected_nodes"])

    def test_incomplete_final_inventory_fails(self):
        text = DUMP + '\n(note 9 0 10 "unit.c" 8)\n(insn 10 9 11 (set (reg:SI 90) (const_int 9)))\n'
        with self.assertRaisesRegex(ValueError, "incomplete final"):
            lineage.parse_dump(text, "target", "08.gcse")

    def test_broken_link_chain_fails(self):
        with self.assertRaisesRegex(ValueError, "complete linked"):
            lineage.parse_dump(DUMP.replace("(insn 2 1 3", "(insn 2 99 3"), "target")

    def test_unterminated_form_fails(self):
        with self.assertRaisesRegex(ValueError, "unterminated"):
            lineage.parse_dump(DUMP + "\n(insn 9 0 0 (set", "target")

    def test_scheduler_commentary_never_means_deletion(self):
        original, _ = lineage.parse_dump(DUMP, "target")
        commentary, meta = lineage.parse_dump(HEADER + ";; dependencies only\n",
                                              "target", "17.sched")
        self.assertEqual({}, commentary)
        self.assertEqual("commentary_only", meta["kind"])
        result = lineage.role_transitions(
            {"16.regmove": original, "17.sched": commentary, "19.lreg": original}, {2, 3}, 12)
        self.assertEqual(0, result[0]["change_count"])
        self.assertEqual(["17.sched"], result[0]["unobserved_intervening_stages"])

    def test_unexpected_empty_dump_fails(self):
        with self.assertRaisesRegex(ValueError, "empty or incomplete"):
            lineage.parse_dump(HEADER + ";; no RTL\n", "target", "03.cse")

    def test_missing_target_fails(self):
        with self.assertRaisesRegex(ValueError, "target missing"):
            lineage.parse_dump(DUMP, "other")

    def test_complete_synthetic_probe(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            probe_fixture(root)
            report, watch = lineage.analyze(root, SPEC)
            self.assertEqual([2, 3], report["roles"][0]["seed_uids"])
            self.assertIsNone(report["roles"][0]["first_observed_candidate_transformation"])
            self.assertEqual([], watch["uids"])
            self.assertEqual("commentary_only", report["stages"]["17.sched"]["kind"])

    def test_missing_required_dump_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            probe_fixture(root)
            (root / "functions/target/08.gcse").unlink()
            with self.assertRaisesRegex(ValueError, "missing required"):
                lineage.analyze(root, SPEC)

    def test_stale_extracted_dump_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            probe_fixture(root)
            (root / "functions/target/03.cse").write_text(HEADER)
            with self.assertRaisesRegex(ValueError, "stale or incomplete"):
                lineage.analyze(root, SPEC)

    def test_source_hash_mismatch_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            probe_fixture(root)
            (root / "input.c").write_text(SOURCE + "\n")
            with self.assertRaisesRegex(ValueError, "source snapshot hash"):
                lineage.analyze(root, SPEC)

    def test_no_raw_pattern_excerpt_in_report(self):
        records, _ = lineage.parse_dump(DUMP, "target")
        self.assertNotIn("pattern_excerpt", lineage.describe(records[2]))


class SourceCallBridgeTests(unittest.TestCase):
    SOURCE = """void target(void) {
    read_runtime();
    clear_peer();
    /* begin */
    read_runtime();
    consume();
    clear_peer();
    /* end */
}
"""
    SPEC = {"schema": 1, "function": "target", "roles": [
        {"name": "peer", "begin": "    /* begin */", "end": "    /* end */",
         "call_bridge": {"begin": {"symbol": "read_runtime", "occurrence": 2},
                         "end": {"symbol": "clear_peer", "occurrence": 2}}}]}

    def inventory(self):
        def call(uid, previous, following, name):
            return (f'(call_insn {uid} {previous} {following} '
                    f'(call (mem:SI (symbol_ref:SI ("{name}")) 0) (const_int 0)))\n')
        text = HEADER + call(2, 0, 3, "read_runtime") + call(3, 2, 4, "clear_peer")
        text += call(4, 3, 5, "read_runtime")
        text += '(insn 5 4 6 (set (reg:SI 89) (reg:SI 90)))\n'
        text += call(6, 5, 0, "clear_peer")
        return lineage.parse_dump(text, "target")[0]

    def test_bridge_works_without_source_notes(self):
        original = self.inventory()
        self.assertTrue(all(row["source"] is None for row in original.values()))
        span = lineage.role_spans(self.SOURCE, self.SPEC)[0]
        seeds, evidence = lineage.bridge_seeds(self.SOURCE, "target", span, original)
        self.assertEqual({4, 5, 6}, seeds)
        self.assertEqual(2, evidence["begin"]["source_call_count"])
        self.assertEqual(2, evidence["begin"]["rtl_call_count"])

    def test_source_call_outside_exact_span_is_rejected(self):
        spec = copy.deepcopy(self.SPEC)
        spec["roles"][0]["call_bridge"]["begin"]["occurrence"] = 1
        span = lineage.role_spans(self.SOURCE, spec)[0]
        with self.assertRaisesRegex(ValueError, "outside"):
            lineage.bridge_seeds(self.SOURCE, "target", span, self.inventory())

    def test_source_and_rtl_call_census_must_agree(self):
        source = self.SOURCE.replace("    consume();", "    read_runtime();")
        span = lineage.role_spans(source, self.SPEC)[0]
        with self.assertRaisesRegex(ValueError, "census mismatch"):
            lineage.bridge_seeds(source, "target", span, self.inventory())

    def test_non_call_symbol_reference_cannot_anchor(self):
        self.assertIsNone(lineage.direct_call_target(
            '(set (reg:SI 90) (symbol_ref:SI ("read_runtime")))'))


class CseTests(unittest.TestCase):
    def test_live_and_dump_feature_identity(self):
        pattern = '(set (reg:SI 84) (plus:SI (symbol_ref:SI ("object")) (const_int -128)))'
        decoded = {"code": "set", "fields": [
            {"code": "reg", "mode": "SI", "regno": 84},
            {"code": "plus", "mode": "SI", "fields": [
                {"code": "symbol_ref", "mode": "SI", "symbol": "object"},
                {"code": "const_int", "value": -128}]}]}
        self.assertEqual(lineage.features(pattern), cse.decoded_features(decoded))

    def test_label_uid_does_not_follow_rtl_chain(self):
        self.assertEqual({"codes": ["label_ref"], "expression_modes": [["label_ref", "VOID"]], "registers": [], "symbols": [],
                          "constants": [], "targets": [14]},
                         cse.decoded_features({"code": "label_ref", "uid": 14}))

    def test_asm_string_parentheses_are_not_rtl_codes(self):
        value = '(asm_operands ("comment (reg:SI 999) (const_int 73)") ("=r") 0 [] [])'
        feature = lineage.features(value)
        self.assertEqual(["asm_operands"], feature["codes"])
        self.assertEqual([], feature["registers"])
        self.assertEqual([], feature["constants"])

    def test_extension_modes_remain_distinct(self):
        a = lineage.features("(zero_extend:SI (mem:QI (reg:SI 95)))")
        b = lineage.features("(zero_extend:DI (mem:QI (reg:SI 95)))")
        self.assertNotEqual(a["expression_modes"], b["expression_modes"])
        self.assertEqual([["zero_extend", "SI"], ["mem", "QI"], ["reg", "SI"]],
                         a["expression_modes"])

    def test_ambiguous_watch_uid_fails(self):
        row = {"uid": 2, "roles": ["sample"], "input_features": {}}
        watch = {"schema": 1, "function": "target", "compiler_sha256": lineage.COMPILER,
                 "stage": "first_cse", "input_stage": "02.jump", "uids": [row, copy.deepcopy(row)]}
        with self.assertRaisesRegex(ValueError, "unique"):
            cse.validate_watch(watch, "target")


class DecisionSummaryTests(unittest.TestCase):
    def archive(self, root):
        symbol = {"code": "symbol_ref", "mode": "SI", "symbol": "SECRET_SYMBOL"}
        register = {"code": "reg", "mode": "SI", "regno": 85}
        def state(value):
            return {"kind": "insn", "uid": 7, "pattern": {"code": "set", "mode": "VOID",
                    "fields": [{"code": "reg", "mode": "SI", "regno": 84}, value]}}
        events = [
            {"event": "cse_enter", "requested_uids": [7], "watch_eligible_count": 2,
             "watch_omitted_count": 1, "watch_source_sha256": "SECRET_HASH",
             "watch_roles": {"7": ["peer"]}},
            {"event": "insn_before", "uid": 7, "roles": ["peer"],
             "source": {"file": "/PRIVATE/path.c", "line": 14}, "state": state(symbol)},
            {"event": "call_enter", "uid": 7, "name": "notreg_cost", "call_id": 1,
             "input": symbol, "return_pc": 3735928559},
            {"event": "call_return", "uid": 7, "name": "notreg_cost", "call_id": 1, "result": 20},
            {"event": "call_enter", "uid": 7, "name": "validate_change", "call_id": 2,
             "old": symbol, "new": register, "in_group": 0, "location_pointer": "0xdeadbeef"},
            {"event": "call_return", "uid": 7, "name": "validate_change", "call_id": 2,
             "result": 1, "location_after": register},
            {"event": "insn_after", "uid": 7, "state": state(register)},
            {"event": "cse_exit", "missing_uids": [], "visits": {"7": 1}},
            {"event": "exit", "status": "W00"},
        ]
        for index, event in enumerate(events, 1):
            event["seq"] = index
        (root / "events.jsonl").write_text("".join(json.dumps(e) + "\n" for e in events))
        (root / "equivalence.json").write_text(json.dumps(
            {"mode": "cse_roles", "function": "target", "all_equal": True,
             "artifacts_equal": {"candidate.o": True}, "events": len(events)}))
        return events

    def test_bounded_summary_retains_validation_costs_and_omissions(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.archive(root)
            result = cse.summarize_decisions(root, limit_per_uid=1)
            self.assertTrue(result["coverage_complete"])
            self.assertEqual(1, result["unwatched_eligible_uids"])
            row = result["uids"][0]
            self.assertEqual("accepted", row["decisions"][0]["validation"])
            self.assertEqual(20, row["cost_summary"][0]["cost"])
            self.assertEqual(1, row["omitted_decisions"])
            self.assertEqual(["became_register_reuse"],
                             row["instruction_outcomes"][0]["observed_transitions"])

    def test_private_fields_never_reach_summary(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.archive(root)
            public = json.dumps(cse.summarize_decisions(root))
            for private in ("SECRET_SYMBOL", "SECRET_HASH", "/PRIVATE", "0xdeadbeef", "3735928559"):
                self.assertNotIn(private, public)
            self.assertNotIn("input_pattern_sha256", public)
            self.assertNotIn('"pattern":', public)

    def test_incomplete_event_log_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            events = self.archive(root)
            (root / "events.jsonl").write_text("".join(json.dumps(e) + "\n" for e in events[:-1]))
            with self.assertRaisesRegex(ValueError, "incomplete"):
                cse.summarize_decisions(root)

    def test_unqualified_pair_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.archive(root)
            (root / "equivalence.json").write_text('{"all_equal":false}')
            with self.assertRaisesRegex(ValueError, "all-equal"):
                cse.summarize_decisions(root)


class CaseBuilderTests(unittest.TestCase):
    WRAPPER = r'''run "$cc1" \
    -Iinclude -Isrc "-DASM_ROOT=\"build/eeasm/asm/$version/nonmatchings/\"" \
    "-DVERSION_$(echo $version | tr a-z A-Z)" \
    -quiet -O2 $flags "$cc_in" -o "$cc_out"
# DDS_KEEP_S=path
run "$ee/ee/bin/as" -EL -G8 -g -Iinclude -o "$out" "$cc_out"
'''

    def test_current_wrapper_tokens_are_preserved(self):
        cc, assembler = case_builder.invocation_templates(
            self.WRAPPER, [], Path("/private/case/rtl"), Path("/private/case/candidate.o"))
        self.assertIn('-DASM_ROOT="build/eeasm/asm/dds2/nonmatchings/"', cc)
        self.assertIn("-DVERSION_DDS2", cc)
        self.assertEqual(["-quiet", "-O2", "-da", "-dumpbase", "/private/case/rtl"],
                         cc[4:9])
        self.assertNotIn("-DSKIP_ASM", cc)
        self.assertEqual(["-EL", "-G8", "-g", "-Iinclude", "-o",
                          "/private/case/candidate.o", case_builder.ASSEMBLY_REL], assembler)

    def test_unit_flags_follow_diagnostic_dump_flags(self):
        cc, _ = case_builder.invocation_templates(
            self.WRAPPER, ["-fexample"], Path("/private/rtl"), Path("/private/candidate.o"))
        self.assertEqual("-fexample", cc[cc.index("-dumpbase") + 2])

    def test_unsupported_shell_expansion_fails(self):
        with self.assertRaisesRegex(ValueError, "unsupported"):
            case_builder.invocation_templates(self.WRAPPER.replace("-quiet", "$UNREVIEWED"),
                                               [], Path("/private/rtl"), Path("/private/a.o"))

    def test_unit_flags_are_selected_exactly(self):
        text = "game/other -fother\ngame/code_001DD390 -fselected # reviewed\n"
        self.assertEqual(["-fselected"], case_builder.flags_for_unit(text))
        self.assertEqual([], case_builder.flags_for_unit("game/other -fother\n"))


if __name__ == "__main__":
    unittest.main()

import os
import tempfile
import unittest
from pathlib import Path

from ee_gcc_contracts import _definition_index, compare, scan_source


class ContractAuditTests(unittest.TestCase):
    def audit(self, source):
        signatures, skipped = scan_source(source, "fixture.c")
        return signatures, compare(signatures, skipped)

    def test_reports_sdf_read_named_resource_conflicts_with_locations(self):
        declaration, skipped = scan_source(
            "extern void *sdfReadNamedResource(void *, u32 *, s32);\n",
            "src/dds1/effect/effManager.c")
        definition, definition_skipped = scan_source(
            "u64 sdfReadNamedResource(u64 name, u32 *info, u32 *flags) {\n    return 0;\n}\n",
            "src/dds1/game/code_002E9708.c")
        report = compare(declaration + definition, skipped + definition_skipped)
        self.assertEqual(len(report["conflicts"]), 1)
        conflict = report["conflicts"][0]
        self.assertEqual(conflict["name"], "sdfReadNamedResource")
        self.assertEqual(conflict["declaration"]["path"], "src/dds1/effect/effManager.c")
        self.assertEqual(conflict["definition"]["path"], "src/dds1/game/code_002E9708.c")
        self.assertEqual(conflict["declaration"]["line"], 1)
        self.assertEqual(conflict["definition"]["line"], 1)
        classes = [item["classification"] for item in conflict["mismatches"]]
        self.assertTrue(any("representation-sensitive" in value for value in classes))

    def test_c89_empty_parameter_list_is_unspecified_and_not_zero_args(self):
        source = """s32 bfTaskUpdate();
s32 bfTaskUpdate(void) { return 0; }
"""
        signatures, report = self.audit(source)
        self.assertEqual(report["conflicts"], [])
        declaration = next(item for item in signatures if item.kind == "declaration")
        definition = next(item for item in signatures if item.kind == "definition")
        self.assertIsNone(declaration.params)
        self.assertEqual(definition.params, ())

    def test_declaration_without_definition_is_not_a_conflict(self):
        _, report = self.audit("extern void *kwlnTaskCreate(const char *, s32, ...);\n")
        self.assertEqual(report["conflicts"], [])

    def test_same_spelling_in_dds1_and_dds2_is_not_cross_compared(self):
        declaration, _ = scan_source("extern void *resource(void *);\n", "src/dds1/a.c")
        definition, _ = scan_source("u32 resource(u32 value) { return value; }\n", "src/dds2/b.c")
        self.assertEqual(compare(declaration + definition, [])["conflicts"], [])

    def test_focused_path_keeps_cross_unit_definition_evidence(self):
        old_cwd = Path.cwd()
        with tempfile.TemporaryDirectory() as tmp:
            try:
                os.chdir(tmp)
                root = Path("src/dds1")
                root.mkdir(parents=True)
                focus = root / "caller.c"
                focus.write_text("extern void resource(void);\n")
                (root / "owner.c").write_text("s32 resource(void) { return 1; }\n")
                definitions = _definition_index([focus])
            finally:
                os.chdir(old_cwd)
        self.assertEqual([(sig.name, sig.kind) for sig in definitions],
                         [("resource", "definition")])

    def test_static_contract_is_file_local(self):
        declaration, _ = scan_source("static void local(void *);\n", "src/dds1/a.c")
        definition, _ = scan_source("static u32 local(u32 value) { return value; }\n", "src/dds1/b.c")
        self.assertEqual(compare(declaration + definition, [])["conflicts"], [])

    def test_unresolved_typedef_alias_is_skipped_not_called_a_conflict(self):
        declaration, _ = scan_source("extern ProjectHandle resolve(void);\n", "src/dds1/a.c")
        definition, _ = scan_source("int resolve(void) { return 0; }\n", "src/dds1/b.c")
        report = compare(declaration + definition, [])
        self.assertEqual(report["conflicts"], [])
        self.assertTrue(any("typedef identity" in item["reason"] for item in report["skipped"]))

    def test_multiline_pointer_qualifiers_and_function_pointer_parameters(self):
        source = """extern const char *loadName(
    const char *name,
    void (*callback)(u32 event, const void *context));
const char *loadName(const char *name, void (*done)(u32 kind, const void *data)) {
    return name;
}
"""
        signatures, report = self.audit(source)
        self.assertEqual(report["conflicts"], [])
        self.assertEqual(len(signatures), 2)

    def test_fixed_parameter_mismatch_is_reported(self):
        _, report = self.audit("extern void *getThing(const char *);\nvoid *getThing(char *name) { return name; }\n")
        self.assertEqual(len(report["conflicts"]), 1)
        self.assertEqual(report["conflicts"][0]["mismatches"][0]["field"], "parameter 1")

    def test_top_level_parameter_qualifier_is_ignored_but_pointee_qualifier_is_not(self):
        compatible = "extern s32 readValue(const u32);\ns32 readValue(u32 value) { return value; }\n"
        _, report = self.audit(compatible)
        self.assertEqual(report["conflicts"], [])
        pointee_mismatch = "extern s32 readValue(const u32 *);\ns32 readValue(u32 *value) { return *value; }\n"
        _, report = self.audit(pointee_mismatch)
        self.assertEqual(len(report["conflicts"]), 1)
        top_level_pointer_qualifier = "extern s32 readValue(u32 * const);\ns32 readValue(u32 *value) { return *value; }\n"
        _, report = self.audit(top_level_pointer_qualifier)
        self.assertEqual(report["conflicts"], [])

    def test_multilevel_pointer_qualifiers_keep_inner_levels(self):
        top_level = "extern s32 readValue(u32 ** const);\ns32 readValue(u32 **value) { return **value; }\n"
        _, report = self.audit(top_level)
        self.assertEqual(report["conflicts"], [])
        inner_level = "extern s32 readValue(u32 * const *);\ns32 readValue(u32 **value) { return **value; }\n"
        _, report = self.audit(inner_level)
        self.assertEqual(len(report["conflicts"]), 1)

    def test_unnamed_base_type_parameters_are_not_mistaken_for_names(self):
        source = "extern int values(unsigned long long, struct Node);\nint values(unsigned long long count, struct Node node) { return (int)count; }\n"
        _, report = self.audit(source)
        self.assertEqual(report["conflicts"], [])

    def test_include_asm_macro_is_not_evidence(self):
        source = """extern s32 phantom(void *);
INCLUDE_ASM(s32, "module/file", phantom);
"""
        signatures, report = self.audit(source)
        self.assertEqual(report["conflicts"], [])
        self.assertEqual([item.kind for item in signatures], ["declaration"])

    def test_continued_macro_body_is_not_parsed_as_source(self):
        source = """#define DECLARE_FAKE() \\
int fake(void); \\
int fake(void) { return 1; }
extern int real(void);
int real(void) { return 0; }
"""
        signatures, report = self.audit(source)
        self.assertEqual([item.name for item in signatures], ["real", "real"])
        self.assertEqual(report["conflicts"], [])

    def test_unsupported_knr_and_malformed_declarators_fail_closed(self):
        source = """int oldStyle(a) { return a; }
int oldStyleWithDeclarations(a) int a; { return a; }
implicitOldStyle(a) { return a; }
extern int (*complexFactory(int))(void);
int ordinary(void) { return 0; }
"""
        signatures, report = self.audit(source)
        self.assertEqual([item.name for item in signatures], ["ordinary"])
        skipped_text = "\n".join(str(item["reason"]) for item in report["skipped"])
        self.assertIn("K&R", skipped_text)
        self.assertTrue(any(item["line"] == 2 for item in report["skipped"]))

    def test_array_parameter_is_explicitly_skipped(self):
        signatures, skipped = scan_source("extern int copyValues(int values[4]);\n", "src/dds1/a.c")
        self.assertEqual(signatures, [])
        self.assertTrue(any("unsupported parameter" in item["reason"] for item in skipped))


if __name__ == "__main__":
    unittest.main()

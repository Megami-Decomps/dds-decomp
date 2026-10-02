import os
import tempfile
import unittest
from pathlib import Path

from ee_gcc_contracts import (
    _definition_index,
    annotate_ignored_result_calls,
    compare,
    scan_ignored_result_calls,
    scan_non_discard_calls,
    scan_source,
)


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

    def test_direct_expression_calls_are_ignored_results(self):
        source = """extern s32 consume(s32);
extern s32 nested(s32);
s32 caller(s32 value) {
    consume(value);
    (void)consume(value + nested(value));
    value = consume(value);
    return consume(value);
}
"""
        calls = scan_ignored_result_calls(source, "src/dds1/a.c")
        self.assertEqual(
            [(call.name, call.caller, call.line, call.explicit_void_cast) for call in calls],
            [("consume", "caller", 4, False), ("consume", "caller", 5, True)],
        )

    def test_other_call_forms_include_result_uses_and_ambiguous_forms(self):
        source = """extern s32 consume(s32);
extern s32 nested(s32);
s32 caller(s32 value) {
    consume(value);
    value = consume(value);
    if (consume(value)) { consume(value); }
    return consume(nested(value));
}
"""
        calls = scan_non_discard_calls(source, "src/dds1/a.c")
        self.assertEqual(
            [(call.name, call.caller, call.line) for call in calls],
            [
                ("consume", "caller", 5),
                ("consume", "caller", 6),
                ("consume", "caller", 7),
                ("nested", "caller", 7),
            ],
        )

    def test_control_headers_and_for_clauses_are_not_call_statements(self):
        source = """extern s32 step(void);
void caller(void) {
    if (step()) { step(); }
    while (step()) { break; }
    for (step(); step(); step()) { }
}
"""
        calls = scan_ignored_result_calls(source, "src/dds1/a.c")
        self.assertEqual([(call.name, call.line) for call in calls], [("step", 3)])

    def test_comments_strings_macros_and_nondirect_expressions_are_excluded(self):
        source = """#define PHANTOM() ignored()
extern s32 ignored(void);
void caller(void) {
    /* ignored(); */
    const char *text = "ignored();";
    ignored(), ignored();
    callback ? ignored() : ignored();
}
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])

    def test_function_macro_invocations_fail_closed(self):
        source = """extern s32 release(void);
#define release() other()
void caller(void) { release(); }
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])
        header_macro = """extern s32 release(void);
void caller(void) { release(); }
"""
        self.assertEqual(
            scan_ignored_result_calls(
                header_macro, "src/dds1/a.c", frozenset({"release"})),
            [],
        )
        self.assertEqual(
            scan_non_discard_calls(
                header_macro, "src/dds1/a.c", frozenset({"release"})),
            [],
        )

    def test_conditional_compilation_file_gets_no_call_evidence(self):
        source = """extern void release(void);
#if 0
extern s32 release(void);
#endif
void caller(void) { release(); }
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])

    def test_return_conflict_gets_direct_ignored_call_evidence(self):
        caller_source = """extern s32 release(void *);
void caller(void *item) {
    release(item);
}
"""
        definition_source = "void release(void *item) { }\n"
        caller_signatures, skipped = scan_source(caller_source, "src/dds1/caller.c")
        definitions, definition_skipped = scan_source(definition_source, "src/dds1/owner.c")
        signatures = caller_signatures + definitions
        report = compare(signatures, skipped + definition_skipped)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(caller_source, "src/dds1/caller.c"),
        )
        call = report["conflicts"][0]["ignored_result_calls"][0]
        self.assertEqual(call["caller"], "caller")
        self.assertEqual(call["line"], 3)
        self.assertEqual(call["mechanism"], "value-return declaration for void definition")
        self.assertIn("do not change", call["disposition"])
        self.assertEqual(report["summary"]["ignored_result_call_sites"], 1)
        self.assertEqual(report["summary"]["other_call_form_sites"], 0)

    def test_return_conflict_reports_mixed_call_forms(self):
        caller_source = """extern s32 release(void *);
s32 caller(void *item) {
    release(item);
    if (item) return release(item);
    return 0;
}
"""
        definition_source = "void release(void *item) { }\n"
        caller_signatures, skipped = scan_source(
            caller_source, "src/dds1/caller.c")
        definitions, definition_skipped = scan_source(
            definition_source, "src/dds1/owner.c")
        signatures = caller_signatures + definitions
        report = compare(signatures, skipped + definition_skipped)
        annotate_ignored_result_calls(
            report,
            signatures,
            scan_ignored_result_calls(caller_source, "src/dds1/caller.c"),
            scan_non_discard_calls(caller_source, "src/dds1/caller.c"),
        )
        conflict = report["conflicts"][0]
        self.assertEqual(conflict["ignored_result_calls"][0]["line"], 3)
        self.assertEqual(
            [(call["caller"], call["line"]) for call in conflict["other_call_forms"]],
            [("caller", 4)],
        )
        self.assertIn("all uses", conflict["other_call_forms"][0]["disposition"])
        self.assertEqual(report["summary"]["other_call_form_sites"], 1)

    def test_parameter_only_conflict_does_not_get_result_evidence(self):
        caller_source = """extern void release(s32);
void caller(s32 item) { release(item); }
"""
        definition_source = "void release(void *item) { }\n"
        caller_signatures, skipped = scan_source(caller_source, "src/dds1/caller.c")
        definitions, definition_skipped = scan_source(definition_source, "src/dds1/owner.c")
        signatures = caller_signatures + definitions
        report = compare(signatures, skipped + definition_skipped)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(caller_source, "src/dds1/caller.c"),
        )
        self.assertNotIn("ignored_result_calls", report["conflicts"][0])
        self.assertEqual(report["summary"]["ignored_result_call_sites"], 0)

    def test_later_declaration_is_not_visible_at_call(self):
        caller_source = """void caller(void *item) { release(item); }
extern s32 release(void *);
"""
        definition_source = "void release(void *item) { }\n"
        caller_signatures, skipped = scan_source(caller_source, "src/dds1/caller.c")
        definitions, definition_skipped = scan_source(definition_source, "src/dds1/owner.c")
        signatures = caller_signatures + definitions
        report = compare(signatures, skipped + definition_skipped)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(caller_source, "src/dds1/caller.c"),
        )
        self.assertNotIn("ignored_result_calls", report["conflicts"][0])

    def test_ambiguous_definitions_fail_closed(self):
        caller_source = """extern s32 release(void *);
void caller(void *item) { release(item); }
"""
        caller_signatures, skipped = scan_source(caller_source, "src/dds1/caller.c")
        owner_a, skipped_a = scan_source("void release(void *item) { }\n", "src/dds1/a.c")
        owner_b, skipped_b = scan_source("void release(void *item) { }\n", "src/dds1/b.c")
        signatures = caller_signatures + owner_a + owner_b
        report = compare(signatures, skipped + skipped_a + skipped_b)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(caller_source, "src/dds1/caller.c"),
        )
        self.assertTrue(report["conflicts"])
        self.assertTrue(all("ignored_result_calls" not in row for row in report["conflicts"]))
        self.assertEqual(report["summary"]["ignored_result_call_sites"], 0)

    def test_static_contract_evidence_is_file_local(self):
        source = """static s32 release(void *);
static void release(void *item) { }
void caller(void *item) { release(item); }
"""
        signatures, skipped = scan_source(source, "src/dds1/caller.c")
        report = compare(signatures, skipped)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(source, "src/dds1/caller.c"),
        )
        # The definition is the most recent visible contract at the call, so
        # the older conflicting declaration is not call-site evidence.
        self.assertNotIn("ignored_result_calls", report["conflicts"][0])

    def test_static_forward_declaration_can_supply_call_evidence(self):
        source = """static s32 release(void *);
void caller(void *item) { release(item); }
static void release(void *item) { }
"""
        signatures, skipped = scan_source(source, "src/dds1/caller.c")
        report = compare(signatures, skipped)
        annotate_ignored_result_calls(
            report, signatures,
            scan_ignored_result_calls(source, "src/dds1/caller.c"),
        )
        self.assertEqual(
            report["conflicts"][0]["ignored_result_calls"][0]["caller"],
            "caller",
        )

    def test_label_and_indirect_or_member_calls_fail_closed(self):
        source = """void caller(void) {
label: direct();
    object.method();
    (*callback)();
    (callback)();
    direct() + 1;
}
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])

    def test_local_function_pointer_and_parameter_shadows_fail_closed(self):
        source = """extern s32 release(void *);
typedef void (*Callback)(void *);
void pointerShadow(void *item) {
    void (*release)(void *) = callback;
    release(item);
}
void typedefShadow(void *item) {
    Callback release = callback;
    release(item);
}
void parameterShadow(void (*release)(void *), void *item) {
    release(item);
}
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])
        self.assertEqual(scan_non_discard_calls(source, "src/dds1/a.c"), [])

    def test_block_scope_prototype_hides_file_scope_contract(self):
        caller_source = """extern s32 release(void);
void caller(void) {
    void release(void);
    release();
}
void other(void) { release(); }
"""
        definition_source = "void release(void) { }\n"
        caller_signatures, skipped = scan_source(
            caller_source, "src/dds1/caller.c")
        definitions, definition_skipped = scan_source(
            definition_source, "src/dds1/owner.c")
        signatures = caller_signatures + definitions
        ignored = scan_ignored_result_calls(
            caller_source, "src/dds1/caller.c")
        other = scan_non_discard_calls(
            caller_source, "src/dds1/caller.c")
        self.assertEqual(
            [(call.caller, call.line) for call in ignored],
            [("other", 6)],
        )
        self.assertEqual(other, [])
        report = compare(signatures, skipped + definition_skipped)
        annotate_ignored_result_calls(report, signatures, ignored, other)
        self.assertEqual(
            [(call["caller"], call["line"])
             for call in report["conflicts"][0]["ignored_result_calls"]],
            [("other", 6)],
        )

    def test_any_same_named_local_makes_function_fail_closed(self):
        source = """extern s32 release(void *);
void caller(void *item) {
    {
        void (*release)(void *) = callback;
        release(item);
    }
    release(item);
}
"""
        self.assertEqual(scan_ignored_result_calls(source, "src/dds1/a.c"), [])

    def test_for_and_comma_declarator_shadows_fail_closed(self):
        source = """extern s32 release(void);
typedef s32 (*Callback)(void);
void forShadow(s32 item) {
    for (Callback release = callback; item; ) { release(); }
    release();
}
void commaShadow(void) {
    Callback other, release;
    release();
}
void unbracedForShadow(s32 item) {
    for (Callback release = callback; item; ) release();
    release();
}
void controlBodyForShadow(s32 item) {
    for (Callback release = callback; item; )
        if (item) { other(); } else { release(); }
}
"""
        calls = scan_ignored_result_calls(source, "src/dds1/a.c")
        self.assertEqual(
            [(call.name, call.line) for call in calls if call.name == "release"],
            [],
        )


if __name__ == "__main__":
    unittest.main()

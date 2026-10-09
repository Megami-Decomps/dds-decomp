#!/usr/bin/env python3
"""Synthetic tests for the bounded delay observer; no private inputs/sockets."""
import copy
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parent))
import ee_gcc_delay_role_tracer as delay
from ee_gcc_role_lineage import digest


SOURCE = """void target(void) {
    if (actor->status.flags & 0x80000) {
        btlUnitGetPosVU(actor, 0);
    }
}
"""
DUMP = ''';; Function target
(note 1 0 2 NOTE_INSN_DELETED 0)
(insn 2 1 3 (set (reg:SI 9) (const_int 524288)) -1 (nil) (nil))
(insn 3 2 4 (set (reg:SI 8) (and:SI (reg:SI 8) (reg:SI 9))) -1 (nil) (nil))
(jump_insn 4 3 5 (set (pc) (if_then_else (eq:SI (reg:SI 8) (const_int 0)) (label_ref 9) (pc))) -1 (nil) (nil))
(insn 5 4 6 (set (reg:SI 4 a0) (reg/v:SI 18 s2)) -1 (nil) (nil))
(insn 6 5 7 (set (reg:SI 5 a1) (const_int 0)) -1 (nil) (nil))
(call_insn 7 6 9 (call (mem:SI (symbol_ref:SI ("btlUnitGetPosVU"))) (const_int 0)) -1 (nil) (nil))
(code_label 9 7 10 42 "" "" [1 uses])
(insn 10 9 11 (set (reg:SI 17) (plus:SI (reg:SI 17) (const_int 16))) -1 (nil) (nil))
(jump_insn 11 10 12 (set (pc) (label_ref 12)) -1 (nil) (nil))
(code_label 12 11 0 43 "" "" [1 uses])
'''


def fixture(root, source=SOURCE, initial=DUMP, late=DUMP):
    (root / "input.c").write_text(source)
    artifacts = []
    for stage, dump in (("00.rtl", initial), ("28.mach", late)):
        (root / ("rtl." + stage)).write_text(dump)
        artifacts.append({"name": "rtl." + stage, "sha256": digest(dump.encode())})
    (root / "manifest.json").write_text(json.dumps({
        "compiler": {"sha256": delay.COMPILER}, "wrapper_returncode": 0,
        "cc1_succeeded": True, "assembled": True,
        "compiled_source_sha256": digest(source.encode()), "artifacts": artifacts}))
    return root


class WatchTests(unittest.TestCase):
    def test_source_call_and_donor_identities(self):
        with tempfile.TemporaryDirectory() as directory:
            watch = delay.build_watch(fixture(Path(directory)), "target")
        self.assertEqual((4, 5, 7, 9), tuple(watch[k] for k in (
            "branch_uid", "argument_uid", "call_uid", "target_uid")))
        self.assertEqual([10, 11], watch["target_chain_uids"])
        self.assertEqual(1, watch["source_role"]["call_occurrence"])
        self.assertEqual(5, len(watch["uids"]))

    def test_source_hash_mismatch_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory))
            (root / "input.c").write_text(SOURCE + "\n")
            with self.assertRaisesRegex(ValueError, "source-verified"):
                delay.build_watch(root, "target")

    def test_dump_hash_mismatch_fails(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory))
            (root / "rtl.28.mach").write_text(DUMP + "\n")
            with self.assertRaisesRegex(ValueError, "dump hash"):
                delay.build_watch(root, "target")

    def test_unassembled_probe_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory))
            path = root / "manifest.json"
            manifest = json.loads(path.read_text())
            manifest["assembled"] = False
            path.write_text(json.dumps(manifest))
            with self.assertRaisesRegex(ValueError, "successful"):
                delay.build_watch(root, "target")

    def test_ambiguous_source_guard_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory), source=SOURCE.replace(
                "    }", "    }\n    if (actor->status.flags & 0x80000) {}"))
            with self.assertRaisesRegex(ValueError, "one exact"):
                delay.build_watch(root, "target")

    def test_call_census_mismatch_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory), source=SOURCE.replace(
                "    }", "    }\n    btlUnitGetPosVU(other, 0);"))
            with self.assertRaisesRegex(ValueError, "census"):
                delay.build_watch(root, "target")

    def test_wrong_guard_mask_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory), initial=DUMP.replace("524288", "262144"))
            with self.assertRaisesRegex(ValueError, "status-mask"):
                delay.build_watch(root, "target")

    def test_argument_must_be_actual_abi_move(self):
        with tempfile.TemporaryDirectory() as directory:
            root = fixture(Path(directory), late=DUMP.replace("reg:SI 4 a0", "reg:SI 6 a2"))
            with self.assertRaisesRegex(ValueError, "ABI argument"):
                delay.build_watch(root, "target")

    def test_deleted_branch_is_not_remapped(self):
        with tempfile.TemporaryDirectory() as directory:
            changed = DUMP.replace("(jump_insn 4 3 5", "(jump_insn 40 3 5").replace(
                "(insn 3 2 4", "(insn 3 2 40").replace("(insn 5 4 6", "(insn 5 40 6")
            root = fixture(Path(directory), late=changed)
            with self.assertRaisesRegex(ValueError, "topology"):
                delay.build_watch(root, "target")

    def test_cse_watch_is_not_accepted(self):
        with self.assertRaisesRegex(ValueError, "schema"):
            delay.validate_watch({"schema": 1, "kind": "cse_roles"}, "target")

    def test_unrelated_uid_and_duplicate_uid_fail(self):
        with tempfile.TemporaryDirectory() as directory:
            watch = delay.build_watch(fixture(Path(directory)), "target")
        other = copy.deepcopy(watch)
        other["uids"].append(copy.deepcopy(other["uids"][0]))
        with self.assertRaisesRegex(ValueError, "unique"):
            delay.validate_watch(other, "target")
        other["uids"][-1]["uid"] = 999
        with self.assertRaisesRegex(ValueError, "unrelated"):
            delay.validate_watch(other, "target")


class FakeRSP:
    def __init__(self):
        self.memory = {}
        self.breakpoints = []

    def put(self, address, value):
        for i, b in enumerate(value):
            self.memory[address + i] = b

    def mem(self, address, size):
        return bytes(self.memory.get(address + i, 0) for i in range(size))

    def u32(self, address):
        return struct.unpack("<I", self.mem(address, 4))[0]

    def ints(self, address, size):
        return [self.u32(address + 4 * i) for i in range(size)]

    def bp(self, address, enabled):
        self.breakpoints.append((address, enabled))


class TracerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.root = Path(self.temp.name)
        self.watch = delay.build_watch(fixture(self.root), "target")
        self.rsp = FakeRSP()
        self.tracer = delay.DelayRoleTracer(self.rsp, self.root / "events.jsonl", "target", watch=self.watch)

    def tearDown(self):
        self.tracer.f.close()
        self.temp.cleanup()

    def test_resources_match_pinned_twenty_byte_layout(self):
        raw = bytearray(20)
        raw[0], raw[3], raw[4], raw[19] = 1, 1, 16, 128
        self.rsp.put(0x1000, raw)
        self.assertEqual({"memory": True, "unch_memory": False, "volatil": False,
                          "cc": True, "registers": [4, 127]}, self.tracer.resources(0x1000))

    def test_hook_guard_fails_closed(self):
        with self.assertRaisesRegex(RuntimeError, "opcode mismatch"):
            self.tracer.verify_hook(0x08167a00)
        self.rsp.put(0x08167a00, delay.HOOKS[0x08167a00][1])
        self.tracer.verify_hook(0x08167a00)
        self.assertIn(0x08167a00, self.tracer.checked_hooks)

    def test_unknown_hook_rejected(self):
        with self.assertRaisesRegex(RuntimeError, "unknown"):
            self.tracer.bp(0x1234, "unknown")

    def test_dynamic_return_static_collision_rejected(self):
        with self.assertRaisesRegex(RuntimeError, "collides"):
            self.tracer.dynamic_return(0x08167a00, "dbr_exit")

    def test_empty_input_chain_cannot_claim_coverage(self):
        with self.assertRaisesRegex(RuntimeError, "missing watched"):
            self.tracer.verify_input(0)

    def test_cyclic_input_chain_rejected(self):
        data = bytearray(24)
        struct.pack_into("<I", data, 4, 50)
        struct.pack_into("<I", data, 20, 0x1000)
        self.rsp.put(0x1000, data)
        with self.assertRaisesRegex(RuntimeError, "cyclic"):
            self.tracer.verify_input(0x1000)

    def test_unwatched_fill_does_not_enable_resources(self):
        self.rsp.put(0x2004, struct.pack("<I", 0x1000))
        self.rsp.put(0x1004, struct.pack("<I", 999))
        self.tracer.handle("fill_enter", {"eip": 0x0816656c, "ebp": 0, "esp": 0x2000})
        self.assertIsNone(self.tracer.fill)
        self.assertEqual([], self.rsp.breakpoints)

    def test_ungated_live_hook_fails(self):
        with self.assertRaisesRegex(RuntimeError, "ungated"):
            self.tracer.handle("live_enter", {"eip": 0x081d23e4, "ebp": 0, "esp": 0x2000})

    def test_missing_trial_coverage_fails(self):
        with self.assertRaisesRegex(RuntimeError, "coverage"):
            self.tracer.handle("dbr_exit", {"eip": 0x1234, "ebp": 0})

    def test_event_bound_fails_before_logging(self):
        self.tracer.seq = 12000
        with self.assertRaisesRegex(RuntimeError, "event limit"):
            self.tracer.log("overflow")

    def test_live_depth_bound_fails(self):
        self.tracer.fill = {"call_id": 1}
        self.tracer.live = [{}] * 16
        with self.assertRaisesRegex(RuntimeError, "bound exceeded"):
            self.tracer.handle("live_enter", {"eip": 0x081d23e4, "ebp": 0, "esp": 0x2000})

    def test_scan_bound_fails(self):
        self.tracer.fill = {"call_id": 1}
        self.tracer.live = [{"frame": 0x2000, "live_id": 1}]
        self.tracer.scan_count = 8000
        with self.assertRaisesRegex(RuntimeError, "scan bound"):
            self.tracer.handle("live_scan", {"eip": 0x081d29e0, "ebp": 0x2000})

    def test_offline_hook_verifier_rejects_unpinned_input(self):
        fake = self.root / "fake-compiler"
        fake.write_bytes(b"not a compiler")
        with self.assertRaisesRegex(ValueError, "pinned"):
            delay.verify_hook_specs(fake)


class SummaryTests(unittest.TestCase):
    def archive(self, root):
        events = [
            {"event": "input_verified"}, {"event": "dbr_enter"},
            {"event": "eager_choice", "prediction": 0, "own_fallthrough": 1},
            {"event": "fill_enter", "call_id": 1, "thread_if_true": 0, "own_thread": 1},
            {"event": "live_enter", "live_id": 1, "call_id": 1, "parent_live_id": None,
             "target_uid": 999, "private": "SECRET_SYMBOL"},
            {"event": "live_recompute", "live_id": 1},
            {"event": "live_exit", "live_id": 1, "call_id": 1, "resources": {"registers": [4]}},
            {"event": "argument_decision", "call_id": 1, "check": "sets_opposite_needed",
             "result": 1, "opposite_needed": {"registers": [4]}, "argument": "SECRET_PATTERN"},
            {"event": "fill_exit", "call_id": 1, "has_delay_list": False, "must_annul": False},
            {"event": "dbr_exit", "coverage_complete": True}, {"event": "exit", "status": "W00"}]
        self.write(root, events)
        return events

    def write(self, root, events, equal=True):
        events = [{**e, "seq": i} for i, e in enumerate(events, 1)]
        (root / "events.jsonl").write_text("".join(json.dumps(e) + "\n" for e in events))
        (root / "equivalence.json").write_text(json.dumps({
            "mode": "delay_roles", "function": "target", "all_equal": equal,
            "artifacts_equal": {"a.o": equal}, "events": len(events)}))

    def test_summary_is_disclosure_bounded(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.archive(root)
            result = delay.summarize_decisions(root)
        self.assertTrue(result["coverage_complete"])
        self.assertTrue(result["argument_decisions"][0]["argument_register_live_on_opposite_path"])
        self.assertNotIn("SECRET", json.dumps(result))
        self.assertNotIn("resources", json.dumps(result))
        self.assertNotIn("pointer", json.dumps(result))
        self.assertNotIn("current_live", json.dumps(result))

    def test_summary_keeps_first_observed_live_transition(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            events = self.archive(root)
            point = next(i for i, e in enumerate(events) if e["event"] == "live_exit")
            events[point:point] = [
                {"event": "live_block", "live_id": 1, "basic_block": 37, "cache_entry_present": True},
                {"event": "live_scan", "live_id": 1, "scanning_uid": 71,
                 "current_live": [17], "pending_dead": []},
                {"event": "live_scan", "live_id": 1, "scanning_uid": 72,
                 "current_live": [4, 17], "pending_dead": [4]},
                {"event": "live_before_forward", "live_id": 1, "resources": {"registers": [4]}}]
            self.write(root, events)
            detail = delay.summarize_decisions(root)["live_provenance"][0]
        self.assertEqual(37, detail["basic_block"])
        self.assertEqual(["recompute"], detail["routes"])
        self.assertEqual({"after_scanning_uid": 71, "observed_at_uid": 72,
                          "before": False, "after": True}, detail["first_argument_live_transition"])
        self.assertEqual(2, detail["scanned_instructions"])

    def test_unowned_provenance_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            events = self.archive(root)
            for event in events:
                if event["event"] == "live_recompute":
                    event["live_id"] = 99
            self.write(root, events)
            with self.assertRaisesRegex(ValueError, "active calculation"):
                delay.summarize_decisions(root)

    def test_unequal_artifacts_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            self.write(root, self.archive(root), equal=False)
            with self.assertRaisesRegex(ValueError, "all-equal"):
                delay.summarize_decisions(root)

    def test_unreturned_live_call_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            events = self.archive(root)
            self.write(root, [e for e in events if e["event"] != "live_exit"])
            with self.assertRaisesRegex(ValueError, "coverage"):
                delay.summarize_decisions(root)

    def test_missing_input_verification_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            events = self.archive(root)
            self.write(root, [e for e in events if e["event"] != "input_verified"])
            with self.assertRaisesRegex(ValueError, "gates"):
                delay.summarize_decisions(root)


if __name__ == "__main__":
    unittest.main()

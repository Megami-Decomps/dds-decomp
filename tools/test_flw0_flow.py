from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import flw0  # noqa: E402
import flw0_flow  # noqa: E402
import flw0_profiles  # noqa: E402


SOURCE = """\
flw0 2
profile dds1

header word00=0 word0c=0 word18=0 word1c=0
locals int=0 float=0

procedure main
procedure worker

code
main:
  PROC main
  call worker
  PUSHIS 0
  PUSHPROC worker
  COMM CREATE_SCRIPT_TASK
  PUSHEVENT e632
  COMM CALL_EVENT
  PUSHEVENT e633
  COMM SUBMIT_EVENT
  PUSHIS 501
  PUSHIS 22
  PUSHIS 1
  COMM DEFER_BATTLE_EXIT
  return
worker:
  PROC worker
  return
end

messages
end

strings
end
"""


class Flw0FlowTests(unittest.TestCase):
    def test_recovers_local_and_external_execution_edges(self) -> None:
        flow = flw0_flow.analyze(
            flw0.parse_source(SOURCE), flw0_profiles.get("dds1")
        )
        self.assertEqual(
            [(row["index"], row["name"]) for row in flow["procedures"]],
            [(0, "main"), (1, "worker")],
        )
        self.assertEqual(
            [
                {key: edge[key] for key in ("source", "target", "kind", "count")}
                for edge in flow["procedureEdges"]
            ],
            [
                {"source": 0, "target": 1, "kind": "call", "count": 1},
                {"source": 0, "target": 1, "kind": "task", "count": 1},
            ],
        )
        self.assertEqual(
            [
                {
                    key: edge[key]
                    for key in (
                        "source",
                        "eventId",
                        "event",
                        "kind",
                        "command",
                        "count",
                    )
                }
                for edge in flow["eventEdges"]
            ],
            [
                {
                    "source": 0,
                    "eventId": 632,
                    "event": "e632",
                    "kind": "call",
                    "command": "CALL_EVENT",
                    "count": 1,
                },
                {
                    "source": 0,
                    "eventId": 633,
                    "event": "e633",
                    "kind": "submit",
                    "command": "SUBMIT_EVENT",
                    "count": 1,
                },
            ],
        )
        self.assertEqual(
            flow["deferredBattleExits"],
            [
                {
                    "source": 0,
                    "pc": 12,
                    "diagnostic": 1,
                    "field": 22,
                    "event": 501,
                }
            ],
        )
        self.assertEqual(flow["unresolvedTargets"], [])

    def test_keeps_dynamic_event_target_unresolved(self) -> None:
        source = SOURCE.replace("  PUSHEVENT e632\n", "  PUSHIX 0\n").replace(
            "  PUSHEVENT e633\n", "  PUSHIX 0\n"
        )
        flow = flw0_flow.analyze(
            flw0.parse_source(source), flw0_profiles.get("dds1")
        )
        self.assertEqual(flow["eventEdges"], [])
        self.assertEqual(
            flow["unresolvedTargets"],
            [
                {
                    "source": 0,
                    "pc": 6,
                    "kind": "event",
                    "value": None,
                    "command": "CALL_EVENT",
                    "dispatch": "call",
                },
                {
                    "source": 0,
                    "pc": 8,
                    "kind": "event",
                    "value": None,
                    "command": "SUBMIT_EVENT",
                    "dispatch": "submit",
                },
            ],
        )

    def test_recovers_selected_event_and_request_id(self) -> None:
        source = SOURCE.replace(
            "  return\nworker:",
            "  SUBMIT_EVENT_WITH_SELECTION(258, event(e634))\n"
            "  PUSHIX 0\n"
            "  COMM SUBMIT_EVENT_IMMEDIATE\n"
            "  return\nworker:",
        )
        flow = flw0_flow.analyze(
            flw0.parse_source(source), flw0_profiles.get("dds1")
        )
        selected = next(
            edge
            for edge in flow["eventEdges"]
            if edge["command"] == "SUBMIT_EVENT_WITH_SELECTION"
        )
        self.assertEqual(
            {
                key: selected[key]
                for key in (
                    "eventId",
                    "event",
                    "requestId",
                    "kind",
                    "command",
                )
            },
            {
                "eventId": 634,
                "event": "e634",
                "requestId": 258,
                "kind": "submit-selection",
                "command": "SUBMIT_EVENT_WITH_SELECTION",
            },
        )
        self.assertIn(
            {
                "source": 0,
                "pc": 17,
                "kind": "event",
                "value": None,
                "command": "SUBMIT_EVENT_IMMEDIATE",
                "dispatch": "submit-immediate",
            },
            flow["unresolvedTargets"],
        )

    def test_keeps_dynamic_battle_exit_unresolved(self) -> None:
        source = SOURCE.replace("  PUSHIS 501\n", "  PUSHIX 0\n")
        flow = flw0_flow.analyze(
            flw0.parse_source(source), flw0_profiles.get("dds1")
        )
        self.assertEqual(flow["deferredBattleExits"], [])
        self.assertIn(
            {
                "source": 0,
                "pc": 12,
                "kind": "battleExit",
                "value": None,
                "command": "DEFER_BATTLE_EXIT",
            },
            flow["unresolvedTargets"],
        )


if __name__ == "__main__":
    unittest.main()

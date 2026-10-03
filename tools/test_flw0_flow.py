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
            {
                key: flow["eventEdges"][0][key]
                for key in ("source", "eventId", "event", "count")
            },
            {"source": 0, "eventId": 632, "event": "e632", "count": 1},
        )
        self.assertEqual(flow["unresolvedTargets"], [])

    def test_keeps_dynamic_event_target_unresolved(self) -> None:
        source = SOURCE.replace("  PUSHEVENT e632\n", "  PUSHIX 0\n")
        flow = flw0_flow.analyze(
            flw0.parse_source(source), flw0_profiles.get("dds1")
        )
        self.assertEqual(flow["eventEdges"], [])
        self.assertEqual(
            flow["unresolvedTargets"],
            [{"source": 0, "pc": 6, "kind": "event", "value": None}],
        )


if __name__ == "__main__":
    unittest.main()

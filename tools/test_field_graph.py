from __future__ import annotations

import sys
import unittest
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import field_graph  # noqa: E402
import wap  # noqa: E402


class FieldGraphTests(unittest.TestCase):
    def test_builds_present_missing_and_conditional_edges(self) -> None:
        table = wap.default_file(wap.PROFILES["dds1"])
        entries = list(table.entries)
        entries[2] = replace(
            entries[2],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            warp_args=(0, 2, 0),
        )
        entries[3] = replace(
            entries[3],
            kind=1,
            area=1,
            name=wap.FixedString("01d_01"),
            flag_mode=2,
            flag=17,
            warp_args=(24, 3, 0),
        )
        entries[4] = replace(
            entries[4],
            kind=6,
            area=1,
            name=wap.FixedString("elevator"),
            warp_type=1,
            warp_args=(2, 4, 0),
        )
        table = replace(table, entries=tuple(entries))

        graph = field_graph.build_graph(
            {"f011_001", "f011_002"},
            {11: table},
        )

        self.assertEqual(
            graph["summary"],
            {
                "fieldSources": 2,
                "transitionTables": 1,
                "areaNodes": 3,
                "fieldTransitions": 2,
                "sourcePresent": 2,
                "targetPresent": 1,
                "conditionalTransitions": 1,
                "multiTransitionActors": 1,
            },
        )
        self.assertEqual(
            [edge["target"] for edge in graph["transitions"]],
            ["f011_002", "f024_003"],
        )
        self.assertEqual(graph["transitions"][1]["gate"], {"mode": 2, "flag": 17})
        missing = next(area for area in graph["areas"] if area["id"] == "f024_003")
        self.assertFalse(missing["hasFieldSource"])

        dot = field_graph.render_dot(graph)
        self.assertIn('"f011_001" -> "f011_002"', dot)
        self.assertIn('"f024_003" [label="f024_003", style="dashed"', dot)
        self.assertIn("01d_01 [3] flag 17 mode 2", dot)


if __name__ == "__main__":
    unittest.main()

#!/usr/bin/env python3
"""Regression tests for field-to-automap runtime linkage."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path


TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import amb  # noqa: E402
import automap_flow  # noqa: E402
import fld  # noqa: E402


def _automap() -> bytes:
    rows = []
    payload = []
    for area in range(2):
        rows.append(
            f"area name=@area_{area}_name sblocks=@area_{area}_sblocks count=1 "
            f"model=@area_{area}_model position=@area_{area}_position"
        )
        payload.extend(
            (
                f"label area_{area}_name\nstring16 \"room_{area}\"",
                (
                    f"label area_{area}_sblocks\n"
                    f"sblock name=@area_{area}_sblock_name node=0 icons=null "
                    f"count=0 floor={area - 1} bounds=null,null"
                ),
                f"label area_{area}_sblock_name\nstring16 \"block_{area}\"",
                (
                    f"label area_{area}_model\n"
                    f"model geometry=@area_{area}_geometry material=@area_{area}_material"
                ),
                (
                    f"label area_{area}_geometry\nmodel_items count=1\n"
                    "model_item node_id=0 parent=-1 rotation=0,0,0 "
                    "position=0,0,0,1 scale=1,1,1,0 bounds=null commands=null"
                ),
                f"label area_{area}_material\nmodel_assets count=0",
                f"label area_{area}_position\nvec3 {area}.0 0.0 0.0",
            )
        )
    source = "\n".join(
        (
            "amb 1",
            "header version=3 magic=ATMP areas=@areas count=2",
            "label areas",
            *rows,
            *payload,
            "label data_end",
            "end_data",
        )
    )
    return amb.encode(amb.parse_source(source))


def _field(selectors: tuple[int, ...]) -> bytes:
    faces = [
        (
            f"face attributes=0x{automap_flow.AUTOMAP_DISCOVERY_ATTRIBUTE:x} "
            "move_floor=0 sound=0 stop=0 place=0 "
            f"automap={selector},{selector + 10} vertices=0,1,2,3 "
            "encounter_type=0 encounter=0 special=0,0"
        )
        for selector in selectors
    ]
    source = "\n".join(
        (
            "fld2 1",
            "header version=23 magic=FLD2 type_count=1 type_table=@types "
            "word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 "
            "word_34=0 word_38=0 word_3c=0",
            "label types",
            "type id=3 count=1 resources=@resources",
            "label resources",
            "resource serial=7 flags=0 type=3 name=@name reserved=0 "
            "transform=null area=null link=null sblock=null data=@collision",
            "label name",
            'string16 "floor"',
            "label collision",
            f"collision vertex_count=4 face_count={len(faces)} extra_count=0 "
            "vertices=@vertices faces=@faces stop=null reserved=0,0",
            "label vertices",
            "vertex 0 0 0 1",
            "vertex 1 0 0 1",
            "vertex 1 0 1 1",
            "vertex 0 0 1 1",
            "label faces",
            *faces,
            "label data_end",
            "end_data",
        )
    )
    return fld.encode(fld.parse_source(source))


class AutomapFlowTests(unittest.TestCase):
    def test_links_room_number_minus_one_and_preserves_native_default(self) -> None:
        sections = automap_flow.build_sections(
            (("f011_002", _field((1, 2, 0))),),
            {11: ("f011", _automap())},
        )

        self.assertEqual(
            sections["automapAreaEdges"],
            [
                {
                    "source": "f011_002",
                    "target": "automap:f011:area:1",
                    "field": 11,
                    "room": 2,
                    "areaIndex": 1,
                    "status": "linked",
                }
            ],
        )
        edges = sections["automapDiscoveryEdges"]
        self.assertEqual(
            [(row["selector"], row["resolution"], row["runtimeFloor"]) for row in edges],
            [(1, "subblock", 1), (2, "default-floor", 1), (0, "default-floor", 1)],
        )
        self.assertEqual(edges[0]["target"], "automap:f011:area:1:subblock:0")
        self.assertEqual([row["upperName"] for row in edges], [11, 12, 10])

    def test_keeps_missing_out_of_range_and_non_field_faces_explicit(self) -> None:
        sections = automap_flow.build_sections(
            (
                ("f012_001", _field((1,))),
                ("f011_003", _field((1,))),
                ("k011_001", _field((1,))),
            ),
            {11: ("f011", _automap())},
        )

        self.assertEqual(
            [row["status"] for row in sections["automapAreaEdges"]],
            ["out-of-range", "missing-map"],
        )
        self.assertEqual(
            [row["source"] for row in sections["automapDiscoveryEdges"]],
            ["f011_003", "f012_001", "k011_001"],
        )
        self.assertTrue(
            all(
                row["resolution"] == "unresolved-area"
                for row in sections["automapDiscoveryEdges"]
            )
        )

    def test_links_complete_maintained_field_corpora(self) -> None:
        expected = {
            "dds1": {
                "automapSourceMaps": 24,
                "automapAreas": 539,
                "automapSubblocks": 1776,
                "automapFieldAreas": 554,
                "linkedAutomapFieldAreas": 464,
                "missingAutomapFieldAreas": 86,
                "outOfRangeAutomapFieldAreas": 4,
                "automapDiscoveryFaces": 2956,
                "resolvedAutomapDiscoveryFaces": 2547,
                "defaultFloorAutomapDiscoveryFaces": 344,
                "unresolvedAutomapDiscoveryFaces": 65,
                "nonFieldAutomapDiscoveryFaces": 63,
                "missingMapAutomapDiscoveryFaces": 0,
                "outOfRangeAutomapDiscoveryFaces": 2,
            },
            "dds2": {
                "automapSourceMaps": 22,
                "automapAreas": 615,
                "automapSubblocks": 1664,
                "automapFieldAreas": 568,
                "linkedAutomapFieldAreas": 477,
                "missingAutomapFieldAreas": 84,
                "outOfRangeAutomapFieldAreas": 7,
                "automapDiscoveryFaces": 2109,
                "resolvedAutomapDiscoveryFaces": 1986,
                "defaultFloorAutomapDiscoveryFaces": 18,
                "unresolvedAutomapDiscoveryFaces": 105,
                "nonFieldAutomapDiscoveryFaces": 102,
                "missingMapAutomapDiscoveryFaces": 0,
                "outOfRangeAutomapDiscoveryFaces": 3,
            },
        }
        for game, expected_summary in expected.items():
            with self.subTest(game=game):
                directory = ROOT / "src" / game / "data" / "field"
                fields = tuple(
                    (
                        source.stem,
                        fld.encode(
                            fld.parse_source(source.read_text(encoding="utf-8"))
                        ),
                    )
                    for source in sorted(directory.glob("*.fldasm"))
                )
                automaps = {
                    int(source.stem[1:]): (
                        source.stem,
                        amb.encode(
                            amb.parse_source(source.read_text(encoding="utf-8"))
                        ),
                    )
                    for source in sorted(directory.glob("f[0-9][0-9][0-9].ambasm"))
                }
                sections = automap_flow.build_sections(fields, automaps)
                self.assertEqual(sections["automapSummary"], expected_summary)


if __name__ == "__main__":
    unittest.main()

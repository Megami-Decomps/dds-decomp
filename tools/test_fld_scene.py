from __future__ import annotations

import struct
import sys
import unittest
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

import amb  # noqa: E402
import fld  # noqa: E402
import fld_model  # noqa: E402
import fld_scene  # noqa: E402
import field_world  # noqa: E402
import inf  # noqa: E402
import wap  # noqa: E402
from test_amb_scene import SOURCE as AUTOMAP_SOURCE  # noqa: E402
from test_fld_model import SOURCE as MODEL_SOURCE  # noqa: E402


SOURCE = """\
fld2 1
header version=23 magic=FLD2 type_count=3 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=3 count=1 resources=@collision_resources
type id=4 count=1 resources=@camera_resources
type id=10 count=1 resources=@placement_resources
label collision_resources
resource serial=2 flags=0 type=3 name=@collision_name reserved=0 transform=@collision_transform area=null link=null sblock=null data=@collision
label camera_resources
resource serial=3 flags=1 type=4 name=@camera_name reserved=0 transform=@camera_transform area=null link=null sblock=null data=@camera
label placement_resources
resource serial=4 flags=1 type=10 name=@placement_name reserved=0 transform=@placement_transform area=null link=null sblock=null data=@placement
label collision_name
string16 01all
label camera_name
string16 01cam_01
label placement_name
string16 01heal_01
label collision_transform
transform position=0,0,0,1 rotation=0,0,0,1 scale=1,1,1,1
label camera_transform
transform position=400,500,600,1 rotation=0,0,0,1 scale=1,1,1,1
label placement_transform
transform position=100,200,300,1 rotation=0,0,0,1 scale=1,1,1,1
label collision
collision vertex_count=4 face_count=1 extra_count=0 vertices=@vertices faces=@faces stop=null reserved=0,0
label vertices
vertex 0 0 0 1
vertex 100 0 0 1
vertex 100 0 100 1
vertex 0 0 100 1
label faces
face attributes=0x00002000 move_floor=0 sound=0 stop=0 place=0 automap=1,1 vertices=0,1,2,3 encounter_zone=7 special=0,0
label camera
camera fovy=0.7853981852531433
label placement
placement kind=8 event=-1 visible=1 payload=@special
label special
special_point kind=heal id=3
label data_end
end_data
"""

EVENT_SOURCE = """\
fld2 1
header version=23 magic=FLD2 type_count=2 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=6 count=1 resources=@event_resources
type id=10 count=1 resources=@placement_resources
label event_resources
resource serial=3 flags=1 type=6 name=@event_name reserved=0 transform=null area=null link=null sblock=null data=@event
label placement_resources
resource serial=4 flags=1 type=10 name=@placement_name reserved=0 transform=null area=null link=null sblock=null data=@placement
label event_name
string16 event_resource
label placement_name
string16 01eve_01
label event
event flags=5 label=@event_label reserved=7,9
label event_label
cstring 001_01eve_01
align 4
label placement
placement kind=1 event=0 visible=1 payload=null
label data_end
end_data
"""


class FldSceneTests(unittest.TestCase):
    def test_loads_paired_field_script_procedure_indices(self) -> None:
        field_path = ROOT / "src/dds1/data/field/f001_000.fldasm"
        script_path = ROOT / "src/dds1/scripts/field/f001.bfasm"
        self.assertEqual(fld_scene._paired_script_path(1, field_path), script_path)
        procedures = fld_scene._script_procedures(script_path)
        self.assertEqual(procedures["warp_label"], 4)

    def test_appends_collision_camera_and_placement(self) -> None:
        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(SOURCE)),
            meters_per_unit=0.01,
            placement_marker_size=50.0,
        )
        wrapper = document["nodes"][-1]
        self.assertEqual(wrapper["name"], "FLD2 field data")
        self.assertEqual(
            wrapper["extras"],
            {
                "ddsCollisionResources": 1,
                "ddsCameraResources": 1,
                "ddsEventResources": 0,
                "ddsPlacementResources": 1,
                "ddsLinkedEventPlacements": 0,
            },
        )
        children = [document["nodes"][index] for index in wrapper["children"]]
        self.assertEqual([node["name"] for node in children], ["01all", "01cam_01", "01heal_01"])
        self.assertEqual(children[1]["extras"]["ddsCameraYFov"], 0.7853981852531433)
        self.assertEqual(children[2]["translation"], [1.0, 2.0, 3.0])
        self.assertEqual(
            children[2]["extras"]["ddsSpecialPoint"], {"kind": "heal", "id": 3}
        )

        collision = document["meshes"][children[0]["mesh"]]
        primitive = collision["primitives"][0]
        accessor = document["accessors"][primitive["indices"]]
        view = document["bufferViews"][accessor["bufferView"]]
        indices = struct.unpack_from("<6H", binary, view["byteOffset"])
        self.assertEqual(indices, (0, 1, 2, 0, 2, 3))
        self.assertEqual(collision["extras"]["ddsTriangleCount"], 2)
        self.assertIn("KHR_materials_unlit", document["extensionsUsed"])
        fld_model.encode_glb(document, binary)

    def test_composes_the_runtime_automap_row_and_discovery_metadata(self) -> None:
        field_data = fld.encode(
            fld.parse_source(
                SOURCE.replace("attributes=0x00002000", "attributes=0x00002800")
            )
        )
        document, binary = fld_scene.build_scene(
            fld.encode(fld.parse_source(MODEL_SOURCE)),
            field_data,
            field_number=11,
            area_number=1,
            automap_data=amb.encode(amb.parse_source(AUTOMAP_SOURCE)),
            meters_per_unit=0.01,
        )

        wrapper = next(
            node for node in document["nodes"] if node["name"] == "FLD2 field data"
        )
        self.assertEqual(
            wrapper["extras"]["ddsAutomapSelection"],
            {
                "source": "f011_001",
                "areaIndex": 0,
                "status": "linked",
                "target": "automap:f011:area:0",
                "name": "001",
            },
        )
        self.assertEqual(wrapper["extras"]["ddsAutomapDiscoveryFaces"], 1)
        collision = next(
            document["nodes"][index]
            for index in wrapper["children"]
            if document["nodes"][index]["name"] == "01all"
        )
        self.assertEqual(
            collision["extras"]["ddsAutomapDiscoveryFaces"],
            [
                {
                    "collisionSerial": 2,
                    "collisionIndex": 0,
                    "face": 0,
                    "selector": 1,
                    "upperName": 1,
                    "resolution": "subblock",
                    "areaStatus": "linked",
                    "subblockIndex": 0,
                    "subblockName": "s01",
                    "floor": -2,
                    "runtimeFloor": -1,
                }
            ],
        )
        automap_node = next(
            node for node in document["nodes"] if node["name"] == "area_001"
        )
        self.assertEqual(automap_node["extras"]["ddsAreaIndex"], 0)
        self.assertEqual(document["asset"]["extras"]["ddsAutomapAreaCount"], 1)
        self.assertEqual(
            document["asset"]["extras"]["ddsAutomapSelectionMode"],
            "runtime-index",
        )
        fld_model.encode_glb(document, binary)

    def test_automatic_automap_selection_rejects_an_unmapped_room_index(self) -> None:
        with self.assertRaisesRegex(fld.FldError, "out-of-range"):
            fld_scene.build_scene(
                fld.encode(fld.parse_source(MODEL_SOURCE)),
                fld.encode(fld.parse_source(SOURCE)),
                field_number=11,
                area_number=2,
                automap_data=amb.encode(amb.parse_source(AUTOMAP_SOURCE)),
            )

    def test_links_event_placement_to_exact_event_resource(self) -> None:
        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(EVENT_SOURCE)),
            meters_per_unit=0.01,
            event_procedures={"001_01eve_01": 7, "unused_procedure": 8},
        )
        wrapper = document["nodes"][-1]
        children = [document["nodes"][index] for index in wrapper["children"]]
        event, placement = children

        self.assertEqual(event["name"], "event_resource")
        self.assertEqual(
            event["extras"],
            {
                "ddsResourceType": 6,
                "ddsResourceSerial": 3,
                "ddsResourceFlags": 1,
                "ddsEventIndex": 0,
                "ddsEventFlags": 5,
                "ddsEventReserved": [7, 9],
                "ddsEventLabel": "001_01eve_01",
                "ddsEventProcedure": {"name": "001_01eve_01", "index": 7},
            },
        )
        self.assertEqual(placement["extras"]["ddsEventIndex"], 0)
        self.assertEqual(placement["extras"]["ddsEventLabel"], "001_01eve_01")
        self.assertEqual(
            placement["extras"]["ddsEventProcedure"],
            {"name": "001_01eve_01", "index": 7},
        )
        self.assertEqual(placement["extras"]["ddsEventResourceSerial"], 3)
        self.assertEqual(wrapper["extras"]["ddsEventResources"], 1)
        self.assertEqual(wrapper["extras"]["ddsLinkedEventPlacements"], 1)
        self.assertEqual(wrapper["extras"]["ddsFieldScriptProcedures"], 2)
        self.assertEqual(wrapper["extras"]["ddsScriptLinkedEventResources"], 1)
        self.assertEqual(wrapper["extras"]["ddsScriptLinkedEventPlacements"], 1)
        fld_model.encode_glb(document, binary)

    def test_rejects_negative_marker_size(self) -> None:
        builder = fld_model.GltfBuilder.create()
        with self.assertRaisesRegex(fld.FldError, "marker size"):
            fld_scene.append_field_scene(
                builder.document,
                bytes(builder.binary),
                fld.encode(fld.parse_source(SOURCE)),
                meters_per_unit=0.01,
                placement_marker_size=-1.0,
            )

    def test_empty_field_omits_empty_children_array(self) -> None:
        source = """\
fld2 1
header version=23 magic=FLD2 type_count=1 type_table=@types word_1c=0 word_20=0 word_24=0 word_28=0 word_2c=0 word_30=0 word_34=0 word_38=0 word_3c=0
label types
type id=99 count=0 resources=@data_end
label data_end
end_data
"""
        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(source)),
            meters_per_unit=1.0,
        )
        self.assertNotIn("children", document["nodes"][-1])
        fld_model.encode_glb(document, binary)

    def test_placement_retains_all_owned_wap_transitions(self) -> None:
        table = wap.default_file(wap.PROFILES["dds1"])
        entries = list(table.entries)
        entries[3] = replace(
            entries[3],
            kind=1,
            area=1,
            name=wap.FixedString("01heal_01"),
            warp_args=(24, 3, 0),
            position=wap.FixedString("03pos_02"),
            camera=wap.FixedString("03cam_01"),
        )
        entries[4] = replace(
            entries[4],
            kind=1,
            area=1,
            name=wap.FixedString("01heal_01"),
            flag_mode=2,
            flag=77,
            warp_type=3,
            warp_args=(606, 0, 12),
            bgm=4,
            footstep=2,
            after_flag=8,
            after_script=wap.FixedString("after_warp"),
            tail=(3, 1, 2, 3, 4, 5, 6, 7),
        )
        entries[5] = replace(
            entries[5],
            kind=1,
            area=1,
            name=wap.FixedString("missing_actor"),
            warp_args=(0, 2, 0),
        )
        table = replace(table, entries=tuple(entries))
        transitions = field_world.area_transitions(table, 11, 1)

        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(SOURCE)),
            meters_per_unit=0.01,
            transitions=transitions,
        )

        wrapper = document["nodes"][-1]
        children = [document["nodes"][index] for index in wrapper["children"]]
        placement = next(node for node in children if node["name"] == "01heal_01")
        linked = placement["extras"]["ddsTransitions"]
        self.assertEqual([row["entry"] for row in linked], [3, 4])
        self.assertEqual(
            linked[0]["destination"],
            {
                "typeId": 0,
                "type": "field",
                "arguments": [24, 3, 0],
                "position": "03pos_02",
                "camera": {"name": "03cam_01", "mode": 0, "table": 0},
                "field": 24,
                "area": 3,
            },
        )
        self.assertEqual(linked[1]["gate"], {"mode": 2, "flag": 77})
        self.assertEqual(linked[1]["destination"]["event"], 606)
        self.assertEqual(linked[1]["destination"]["alternateField"], 12)
        self.assertEqual(
            linked[1]["after"],
            {"bgm": 4, "footstep": 2, "flags": 8, "script": "after_warp"},
        )
        self.assertEqual(
            linked[1]["tail"],
            {"control": 3, "arguments": [1, 2, 3, 4, 5, 6, 7]},
        )
        self.assertEqual(wrapper["extras"]["ddsTransitionRows"], 3)
        self.assertEqual(wrapper["extras"]["ddsLinkedTransitionRows"], 2)
        self.assertEqual(
            wrapper["extras"]["ddsUnlinkedTransitionActors"],
            [{"actor": "missing_actor", "entries": [5]}],
        )
        fld_model.encode_glb(document, binary)

    def test_placement_retains_complete_inf_interaction(self) -> None:
        flags = list(inf.DEFAULT_SET.flags)
        flags[0] = inf.FlagSelector(23, 12, 100)
        messages = list(inf.DEFAULT_SET.messages)
        messages[2] = inf.MessageRow(
            1,
            7,
            (13, 100, 0, 207),
            4,
            5,
            2,
            3,
        )
        views = list(inf.DEFAULT_FILE.views)
        views[2] = inf.View(0, 6, "01pos_04", "01cam_02")
        actions = list(inf.DEFAULT_FILE.extra_actions)
        actions[3] = inf.ExtraAction(9, (10, 20, 30, 40))
        sets = list(inf.DEFAULT_FILE.sets)
        sets[1] = inf.InteractionSet(
            inf.Start(1, 0, 1, 2, "01heal_01"), tuple(flags), tuple(messages)
        )
        sets[2] = replace(
            sets[1], start=inf.Start(0, 0, 1, 3, "missing_actor")
        )
        table = replace(
            inf.DEFAULT_FILE,
            views=tuple(views),
            extra_actions=tuple(actions),
            sets=tuple(sets),
        )
        symbols = (None,) * 7 + ("HEAL_PROMPT",)
        interactions = field_world.area_interactions(table, 1, symbols)

        builder = fld_model.GltfBuilder.create()
        document, binary = fld_scene.append_field_scene(
            builder.document,
            bytes(builder.binary),
            fld.encode(fld.parse_source(SOURCE)),
            meters_per_unit=0.01,
            interactions=interactions,
        )

        wrapper = document["nodes"][-1]
        children = [document["nodes"][index] for index in wrapper["children"]]
        placement = next(node for node in children if node["name"] == "01heal_01")
        interaction = placement["extras"]["ddsInteractions"][0]
        self.assertEqual(
            {key: interaction[key] for key in ("set", "kind", "action", "event")},
            {"set": 1, "kind": "event", "action": 1, "event": "01heal_01"},
        )
        self.assertEqual(
            interaction["flagSelectors"][0],
            {
                "row": 0,
                "flag": 23,
                "off": {"value": 12, "type": "row", "row": 2},
                "on": {"value": 100, "type": "warp"},
            },
        )
        row = interaction["rows"][0]
        self.assertEqual(row["messageName"], "HEAL_PROMPT")
        self.assertEqual(
            row["choices"],
            [
                {"value": 13, "type": "row", "row": 3},
                {"value": 100, "type": "warp"},
                {"value": 0, "type": "complete"},
                {"value": 207, "type": "control"},
            ],
        )
        self.assertEqual(
            row["view"],
            {
                "index": 2,
                "player": 0,
                "motion": 6,
                "position": "01pos_04",
                "camera": "01cam_02",
            },
        )
        self.assertEqual(
            row["extraAction"],
            {"index": 3, "id": 9, "parameters": [10, 20, 30, 40]},
        )
        self.assertEqual(wrapper["extras"]["ddsInteractionSets"], 2)
        self.assertEqual(wrapper["extras"]["ddsLinkedInteractionSets"], 1)
        self.assertEqual(
            wrapper["extras"]["ddsUnlinkedInteractionActors"],
            [{"actor": "missing_actor", "sets": [2]}],
        )
        fld_model.encode_glb(document, binary)


if __name__ == "__main__":
    unittest.main()

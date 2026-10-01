#!/usr/bin/env python3
"""Regression tests for the DDS battle table codec."""

from __future__ import annotations

import hashlib
import sys
import unittest
from collections import Counter
from dataclasses import replace
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))

import battle_tbl  # noqa: E402
import flw0_semantic  # noqa: E402


def _battle_symbols(game: str) -> battle_tbl.BattleSymbols:
    directory = ROOT / f"src/{game}/data/battle"
    source = directory / "msg.tblasm"
    table = battle_tbl.parse_message_source(
        source.read_text(encoding="utf-8"), source.parent
    )
    skill = battle_tbl.parse_skill_source(
        (directory / "skill.tblasm").read_text(encoding="utf-8")
    )
    return battle_tbl.battle_symbols_from_message(table, skill)


class BattleTableTests(unittest.TestCase):
    def test_encount_profiles_and_templates(self) -> None:
        expected_sizes = {"dds1": 0x20FE0, "dds2": 0x1B760}
        for name, size in expected_sizes.items():
            with self.subTest(profile=name):
                profile = battle_tbl.ENCOUNT_PROFILES[name]
                model = battle_tbl.default_encount(profile)
                data = battle_tbl.encode_encount(model)
                self.assertEqual(len(data), size)
                self.assertEqual(battle_tbl.decode_encount(data), model)

    def test_unit_profiles_and_templates(self) -> None:
        expected_sizes = {"dds1": 0x10810, "dds2": 0x10A10}
        for name, size in expected_sizes.items():
            with self.subTest(profile=name):
                profile = battle_tbl.UNIT_PROFILES[name]
                model = battle_tbl.default_unit(profile)
                data = battle_tbl.encode_unit(model)
                self.assertEqual(len(data), size)
                self.assertEqual(battle_tbl.decode_unit(data), model)

    def test_skill_profiles_and_templates(self) -> None:
        expected_sizes = {"dds1": 0x86C0, "dds2": 0x9BD0}
        for name, size in expected_sizes.items():
            with self.subTest(profile=name):
                profile = battle_tbl.SKILL_PROFILES[name]
                model = battle_tbl.default_skill(profile)
                data = battle_tbl.encode_skill(model)
                self.assertEqual(len(data), size)
                self.assertEqual(battle_tbl.decode_skill(data), model)

    def test_every_skill_record_family_round_trips(self) -> None:
        profile = battle_tbl.SKILL_PROFILES["dds2"]
        model = battle_tbl.default_skill(profile)
        attributes = list(model.action_attributes)
        attributes[1] = battle_tbl.SkillActionAttribute(-1, 18)
        actions = list(model.actions)
        actions[1] = battle_tbl.SkillAction(
            flags=1,
            use=2,
            effect_type=2,
            cost_type=1,
            cost=12,
            cost_base=3,
            target_type=1,
            target_area=9,
            target_rule=2,
            target_random=1,
            untargetable_status=0x228,
            target_program=4,
            hit_type=1,
            hit_level=99,
            hit_program=5,
            hits_min=2,
            hits_max=4,
            hp_type=8,
            hp_power=75,
            mp_type=5,
            mp_power=50,
            hp_base=10,
            mp_base=-4,
            effect_percent=100,
            ailment_type=2,
            ailment_level=100,
            base_status=0x4000,
            support_type=0x2AA,
            support_points=-3,
            death_type=25,
            lookup_id=7,
            program=3,
            magic_base=-20,
            magic_limit=30000,
        )
        requirements = list(model.requirements)
        requirements[0] = battle_tbl.SkillRequirement(
            (4, 0x20000002, 0x40000001), 3, 2
        )
        coefficients = list(model.coefficient_bits)
        coefficients[12] = 0x3FC00000
        coefficients[13] = 0x00050005
        party_defaults = list(model.party_defaults)
        party_defaults[3] = battle_tbl.PartySkillDefaults(
            9, 1375, 0x3E4CCCCD, 0x201, 0x100, 6, 1, 4, 264, 1
        )
        items = list(model.items)
        items[4] = battle_tbl.SkillItemEntry(0x402, 160, 25000, 1)
        bonuses = list(model.profile_bonuses)
        bonuses[5] = battle_tbl.ProfileBonus((1, 2, 3, 4, 5), 9)
        groups = list(model.groups)
        groups[1] = battle_tbl.SkillGroup((10, 11, 12))
        model = battle_tbl.SkillTable(
            profile,
            tuple(attributes),
            tuple(actions),
            tuple(requirements),
            tuple(coefficients),
            tuple(party_defaults),
            tuple(items),
            tuple(bonuses),
            tuple(groups),
        )
        battle_tbl.validate_skill_references(model)
        data = battle_tbl.encode_skill(model)
        self.assertEqual(battle_tbl.decode_skill(data), model)
        source = battle_tbl.render_skill_source(model)
        self.assertIn("action-attribute 1 attribute=-1 auxiliary=18", source)
        self.assertIn(
            "action 1 flags=0x1 use=2 effect_type=2 cost_type=1 cost=12 ",
            source,
        )
        self.assertIn("hit_type=1 hit_level=99 hit_program=5", source)
        self.assertIn("support_points=-3 death_type=25", source)
        self.assertIn(
            "requirement 0x1ab conditions=skill:4,unit-mask:0x2,group:1", source
        )
        self.assertIn("coefficient 0x30 value=1.5", source)
        self.assertIn("coefficient 0x34 bits=0x50005", source)
        self.assertIn("profile-bonus 0xc5 stats=1,2,3,4,5 tier=9", source)
        self.assertIn("group 1 skills=10,11,12", source)
        self.assertEqual(battle_tbl.encode_skill(battle_tbl.parse_skill_source(source)), data)

    def test_invalid_skill_references_are_rejected(self) -> None:
        skill = battle_tbl.default_skill(battle_tbl.SKILL_PROFILES["dds2"])
        requirements = list(skill.requirements)
        requirements[0] = battle_tbl.SkillRequirement(
            (0x40000030, 0xFFFFFFFF, 0xFFFFFFFF), 1
        )
        invalid_group = battle_tbl.SkillTable(
            skill.profile,
            skill.action_attributes,
            skill.actions,
            tuple(requirements),
            skill.coefficient_bits,
            skill.party_defaults,
            skill.items,
            skill.profile_bonuses,
            skill.groups,
        )
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "group 48 outside"):
            battle_tbl.encode_skill(invalid_group)

        groups = list(skill.groups)
        groups[0] = battle_tbl.SkillGroup((skill.profile.action_attr_count,))
        invalid_member = battle_tbl.SkillTable(
            skill.profile,
            skill.action_attributes,
            skill.actions,
            skill.requirements,
            skill.coefficient_bits,
            skill.party_defaults,
            skill.items,
            skill.profile_bonuses,
            tuple(groups),
        )
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "skill 672 outside"):
            battle_tbl.encode_skill(invalid_member)

    def test_unit_skill_reference_validation(self) -> None:
        unit = battle_tbl.default_unit(battle_tbl.UNIT_PROFILES["dds2"])
        enemies = list(unit.enemies)
        enemies[7] = battle_tbl.EnemyTemplate(
            level=1,
            skills=(battle_tbl.SKILL_PROFILES["dds2"].action_attr_count,) + (0,) * 7,
        )
        unit = battle_tbl.UnitTable(
            unit.profile,
            unit.party,
            unit.party_affinities,
            unit.alternate_affinities,
            tuple(enemies),
            unit.enemy_affinities,
        )
        skill = battle_tbl.default_skill(battle_tbl.SKILL_PROFILES["dds2"])
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "skill 672 outside"):
            battle_tbl.validate_unit_skill(unit, skill)

    def test_every_unit_record_family_round_trips(self) -> None:
        profile = battle_tbl.UNIT_PROFILES["dds2"]
        model = battle_tbl.default_unit(profile)
        party = list(model.party)
        party[3] = battle_tbl.PartyTemplate(
            flags=0x107,
            affinity_source=12,
            unit_id=3,
            hp=40,
            max_hp=45,
            mp=12,
            max_mp=13,
            status=0x20,
            experience=1234,
            level=7,
            stats=(5, 6, 7, 8, 9),
            unknown_1b_21=bytes.fromhex("01020304050607"),
            skills=tuple(range(24)),
            equipped_bullet=6,
            unknown_54=7,
            current_profile=8,
            tail=bytes([9]) + bytes(profile.party_size - 0x57),
        )
        party_affinities = list(model.party_affinities)
        party_affinities[3] = battle_tbl.AffinityRow(
            (100, 0x80000078) + (0,) * 17
        )
        alternate_affinities = list(model.alternate_affinities)
        alternate_affinities[3] = battle_tbl.AffinityRow((120,) * 19)
        enemies = list(model.enemies)
        enemies[12] = battle_tbl.EnemyTemplate(
            flags=0x1000,
            race=2,
            level=17,
            hp=200,
            max_hp=220,
            mp=80,
            max_mp=90,
            growth_profile=-2,
            unknown_0f=4,
            stats=(10, 11, 12, 13, 14),
            summon_category=2,
            unknown_16_17=b"\x05\x06",
            skills=(1, 2, 3, 4, 5, 6, 7, 8),
            macca=-30,
            experience=100,
            atma_points=200,
            atma_bonus=300,
            unknown_34_3d=bytes(range(10)),
            drop_items=(4, 5),
            drop_rates=(6, 7),
            conditional_drop_flag=0x1234,
            conditional_drop_item=8,
            conditional_drop_rate=9,
            attack_attribute=-3,
            attack_repeats=2,
            result_parameter=10,
            tail=b"\x0b\x0c\x0d",
        )
        enemy_affinities = list(model.enemy_affinities)
        enemy_affinities[12] = battle_tbl.AffinityRow(tuple(range(19)))
        model = battle_tbl.UnitTable(
            profile,
            tuple(party),
            tuple(party_affinities),
            tuple(alternate_affinities),
            tuple(enemies),
            tuple(enemy_affinities),
        )
        data = battle_tbl.encode_unit(model)
        self.assertEqual(battle_tbl.decode_unit(data), model)
        source = battle_tbl.render_unit_source(model)
        self.assertIn("party 3 flags=0x107 affinity_source=12 unit=3 hp=40", source)
        self.assertIn("party-affinity 3 values=100,0x80000078", source)
        self.assertIn("enemy 12 flags=0x1000 race=2 level=17", source)
        self.assertEqual(battle_tbl.encode_unit(battle_tbl.parse_unit_source(source)), data)

    def test_encounter_enemy_reference_validation(self) -> None:
        encount = battle_tbl.default_encount(battle_tbl.ENCOUNT_PROFILES["dds2"])
        encounters = list(encount.encounters)
        encounters[4] = battle_tbl.Encounter(enemies=(7,) + (0,) * 10)
        encount = battle_tbl.EncountTable(
            encount.profile,
            tuple(encounters),
            encount.default_maps,
            encount.zones,
            encount.overrides,
            encount.background_maps,
            encount.visuals,
        )
        unit = battle_tbl.default_unit(battle_tbl.UNIT_PROFILES["dds2"])
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "empty enemy 7"):
            battle_tbl.validate_encount_unit(encount, unit)
        enemies = list(unit.enemies)
        enemies[7] = battle_tbl.EnemyTemplate(level=1)
        battle_tbl.validate_encount_unit(
            encount,
            battle_tbl.UnitTable(
                unit.profile,
                unit.party,
                unit.party_affinities,
                unit.alternate_affinities,
                tuple(enemies),
                unit.enemy_affinities,
            ),
        )

    def test_enemy_ai_reference_validation(self) -> None:
        game = "dds2"
        directory = ROOT / f"src/{game}/data/battle"
        symbols = _battle_symbols(game)
        unit = battle_tbl.parse_unit_source(
            (directory / "unit.tblasm").read_text(encoding="utf-8"), symbols
        )
        aicalc_source = directory / "aicalc.tblasm"
        aicalc = battle_tbl.parse_aicalc_source(
            aicalc_source.read_text(encoding="utf-8"),
            aicalc_source.parent,
            symbols,
        )
        enemy_id = next(
            index
            for index, enemy in enumerate(aicalc.enemies)
            if enemy != battle_tbl.EnemyAi()
        )
        enemies = list(unit.enemies)
        enemies[enemy_id] = battle_tbl.EnemyTemplate()
        invalid_unit = replace(unit, enemies=tuple(enemies))
        with self.assertRaisesRegex(
            battle_tbl.BattleTableError,
            f"enemy AI {enemy_id} references an empty UNIT enemy template",
        ):
            battle_tbl.validate_aicalc_unit(aicalc, invalid_unit)

    def test_every_encount_record_family_round_trips(self) -> None:
        profile = battle_tbl.ENCOUNT_PROFILES["dds2"]
        model = battle_tbl.default_encount(profile)

        encounters = list(model.encounters)
        encounters[3] = battle_tbl.Encounter(
            -2,
            8,
            2,
            9,
            44,
            (1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 5),
            201,
            4,
            0x10204,
            17,
            606,
        )

        default_maps = list(model.default_maps)
        default_entries = list(default_maps[2].entries)
        default_entries[5] = battle_tbl.Selector(7, 101, 8, 102, 9)
        default_maps[2] = battle_tbl.SelectorMap(24, tuple(default_entries))

        zones = list(model.zones)
        pools = list(zones[4].pools)
        slots = list(pools[1].slots)
        slots[2] = battle_tbl.PoolSlot(33, 60, -3, 70)
        pools[1] = battle_tbl.EncounterPool(30, -4, tuple(slots))
        zones[4] = battle_tbl.Zone(
            203,
            2,
            5,
            6,
            ((1, 0x20), (2, 0x30), (3, 0x40)),
            (1, 2, 3, 4, 5, 6, 7, 8),
            tuple(pools),
        )

        overrides = list(model.overrides)
        overrides[1] = battle_tbl.OverrideRule(-1, 88, 25, 9)

        background_maps = list(model.background_maps)
        background_entries = list(background_maps[1].entries)
        background_entries[6] = battle_tbl.Selector(0x20001, 110, 3, 111, 4)
        background_maps[1] = battle_tbl.SelectorMap(23, tuple(background_entries))

        visuals = list(model.visuals)
        groups = list(visuals[0].groups)
        groups[2] = battle_tbl.VisualGroup(3, (1, 2, 3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF), 4)
        visuals[0] = battle_tbl.VisualRow(tuple(range(16)), tuple(groups))

        model = battle_tbl.EncountTable(
            profile,
            tuple(encounters),
            tuple(default_maps),
            tuple(zones),
            tuple(overrides),
            tuple(background_maps),
            tuple(visuals),
        )
        data = battle_tbl.encode_encount(model)
        self.assertEqual(battle_tbl.decode_encount(data), model)
        source = battle_tbl.render_encount_source(model)
        self.assertIn("encounter 3 voice=-2 item=8 item_count=2", source)
        self.assertIn("flags=0x10204", source)
        self.assertIn("routes abc=1 ab=2 ac=3 bc=4 a=5 b=6 c=7 none=8", source)
        self.assertIn("entry 5 zone=7 flag_a=101 zone_a=8 flag_b=102 zone_b=9", source)
        self.assertEqual(battle_tbl.encode_encount(battle_tbl.parse_encount_source(source)), data)

    def test_invalid_layout_and_source_are_rejected(self) -> None:
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "ENCOUNT needs six segments"):
            battle_tbl.decode_encount(bytes(16))

        mixed_routes = """\
battle-table 1 kind=encounter profile=dds1
zone 0
  routes values=0,0,0,0,0,0,0,0 abc=1
end
"""
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "cannot be combined"):
            battle_tbl.parse_encount_source(mixed_routes)

        bad_padding = bytearray(
            battle_tbl.encode_encount(
                battle_tbl.default_encount(battle_tbl.ENCOUNT_PROFILES["dds1"])
            )
        )
        bad_padding[4 + 0xA000] = 1
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "nonzero alignment"):
            battle_tbl.decode_encount(bytes(bad_padding))

    def test_aicalc_semantic_structure_and_references(self) -> None:
        expected = {
            "dds1": (72, 29, 667, 0),
            "dds2": (89, 32, 773, 32),
        }
        for game, counts in expected.items():
            with self.subTest(game=game):
                source = ROOT / f"src/{game}/data/battle/aicalc.tblasm"
                symbols = _battle_symbols(game)
                table = battle_tbl.parse_aicalc_source(
                    source.read_text(encoding="utf-8"), source.parent, symbols
                )
                ai_count = len(battle_tbl.flw0.parse(table.ai_script).named_rows(0))
                formula_count = len(
                    battle_tbl.flw0.parse(table.formula_script).named_rows(0)
                )
                formula_source = source.with_name("aicalc-formulas.bfasm").read_text(
                    encoding="utf-8"
                )
                self.assertIn("CALC_SET_RESULT(", formula_source)
                self.assertNotIn("COMM 0x016c", formula_source)
                self.assertEqual(
                    (
                        ai_count,
                        formula_count,
                        len(table.calculation_words),
                        len(table.weighted_tables),
                    ),
                    counts,
                )
                self.assertEqual(
                    battle_tbl.decode_aicalc(battle_tbl.encode_aicalc(table)), table
                )
                skill_source = ROOT / f"src/{game}/data/battle/skill.tblasm"
                skill = battle_tbl.parse_skill_source(
                    skill_source.read_text(encoding="utf-8")
                )
                battle_tbl.validate_aicalc_references(table, skill)

        dds1_source = ROOT / "src/dds1/data/battle/aicalc.tblasm"
        dds1 = battle_tbl.parse_aicalc_source(
            dds1_source.read_text(encoding="utf-8"),
            dds1_source.parent,
            _battle_symbols("dds1"),
        )
        groups = [list(group) for group in dds1.enemies[0].groups]
        groups[0][0] = battle_tbl.AiChoice(weight=1, action=0x7000)
        enemies = list(dds1.enemies)
        enemies[0] = replace(
            enemies[0], groups=tuple(tuple(group) for group in groups)
        )
        invalid = replace(dds1, enemies=tuple(enemies))
        with self.assertRaisesRegex(
            battle_tbl.BattleTableError, "weighted table 0 outside AICALC"
        ):
            battle_tbl.validate_aicalc_references(invalid)

    def test_message_semantic_structure_and_references(self) -> None:
        expected = {
            "dds1": (
                (256, 98, 384, 384, 192, 32, 16, 624, 64, 256),
                (192, 607, 210, 9),
            ),
            "dds2": (
                (256, 176, 384, 384, 256, 32, 32, 672, 48),
                (256, 672, 254, 9),
            ),
        }
        expected_controls = {
            "dds1": Counter(
                {
                    "segment-start": 1018,
                    "font-slot": 4864,
                    "conditional-newline": 930,
                    "text-attribute": 210,
                    "token": 50,
                    "control": 77,
                }
            ),
            "dds2": Counter(
                {
                    "segment-start": 1191,
                    "font-slot": 1191,
                    "conditional-newline": 932,
                    "text-attribute": 254,
                    "token": 62,
                    "control": 116,
                }
            ),
        }
        for game, (text_counts, bank_counts) in expected.items():
            with self.subTest(game=game):
                source = ROOT / f"src/{game}/data/battle/msg.tblasm"
                table = battle_tbl.parse_message_source(
                    source.read_text(encoding="utf-8"), source.parent
                )
                self.assertEqual(tuple(map(len, table.text_tables)), text_counts)
                self.assertEqual(
                    tuple(
                        len(battle_tbl.msg1.decode(bank).dialogs)
                        for bank in table.message_banks
                    ),
                    bank_counts,
                )
                self.assertEqual(
                    battle_tbl.decode_message(battle_tbl.encode_message(table)), table
                )
                controls = Counter()
                raw_controls = set()
                for kind, bank in zip(
                    battle_tbl.MESSAGE_BANK_KINDS, table.message_banks
                ):
                    path = source.with_name(f"msg-{kind}.msgasm")
                    rendered = "\n".join(
                        battle_tbl.msg1.render(bank, semantic=True)
                    ) + "\n"
                    self.assertEqual(path.read_text(encoding="utf-8"), rendered)
                    for line in rendered.splitlines():
                        directive = line.strip().split(maxsplit=1)
                        if directive and directive[0] in expected_controls[game]:
                            controls[directive[0]] += 1
                            if directive[0] == "control":
                                raw_controls.add(line.strip())
                self.assertEqual(controls, expected_controls[game])
                self.assertEqual(raw_controls, {"control f1 11"})
                skill = battle_tbl.parse_skill_source(
                    (ROOT / f"src/{game}/data/battle/skill.tblasm").read_text()
                )
                symbols = battle_tbl.battle_symbols_from_message(table, skill)
                unit = battle_tbl.parse_unit_source(
                    (ROOT / f"src/{game}/data/battle/unit.tblasm").read_text(),
                    symbols,
                )
                battle_tbl.validate_message_references(table, unit, skill)

        dds2_source = ROOT / "src/dds2/data/battle/msg.tblasm"
        dds2 = battle_tbl.parse_message_source(
            dds2_source.read_text(encoding="utf-8"), dds2_source.parent
        )
        names = {
            profile.directive: values
            for profile, values in zip(dds2.profile.text_profiles, dds2.text_tables)
        }
        self.assertEqual(names["enemy-name"][1], "Skadi")
        self.assertEqual(names["skill-name"][1], "Agi")
        self.assertEqual(names["skill-family-name"][2], "Element Repel Skill")

        text_tables = list(dds2.text_tables)
        enemy_names = list(text_tables[3])
        enemy_names[0] = "x" * 17
        text_tables[3] = tuple(enemy_names)
        with self.assertRaisesRegex(battle_tbl.BattleTableError, "row is 17 bytes"):
            battle_tbl.encode_message(replace(dds2, text_tables=tuple(text_tables)))

    def test_battle_name_symbols_are_stable_and_typed(self) -> None:
        symbols = _battle_symbols("dds1")
        self.assertEqual(symbols.skills.by_name["AGI"], 1)
        self.assertEqual(symbols.skills.by_name["MARAGI_004"], 4)
        self.assertEqual(symbols.skills.by_name["MARAGI_1B0"], 432)
        self.assertEqual(symbols.skills.by_name["SKILL_000"], 0)
        self.assertEqual(len(symbols.skills.by_name), 608)
        self.assertEqual(symbols.enemies.by_name["ISIS_002"], 2)
        self.assertEqual(symbols.enemies.by_name["ISIS_10C"], 268)
        self.assertEqual(symbols.enemies.by_name["ENEMY_000"], 0)
        self.assertEqual(len(symbols.enemies.by_name), 384)

        profile = battle_tbl.aicalc_command_profile("dds1", symbols)
        select = profile.by_name["AI_SELECT_SKILL"]
        queued = profile.by_name["AI_ANY_PLAYER_HAS_QUEUED_ACTION"]
        self.assertEqual(select.symbols_for_argument(0).by_name["AGI"], 1)
        self.assertEqual(
            queued.symbols_for_argument(0).by_name["MAGIC_REPEL_16D"], 365
        )
        with self.assertRaisesRegex(
            battle_tbl.flw0.Flw0Error, "620 is outside the integer domain"
        ):
            flw0_semantic.parse_expression("AI_SELECT_SKILL(620)", 1, profile)
        with self.assertRaisesRegex(
            battle_tbl.flw0.Flw0Error, "not a symbolic value"
        ):
            flw0_semantic.parse_expression(
                "AI_SELECT_SKILL(SKILL_26C)", 1, profile
            )

    def test_tracked_battle_corpus_hashes(self) -> None:
        for game in ("dds1", "dds2"):
            symbols = _battle_symbols(game)
            manifest = ROOT / f"config/{game}/battle_tables.sha1"
            for entry in manifest.read_text(encoding="utf-8").splitlines():
                digest, output = entry.split()
                name = Path(output).stem.lower()
                source = ROOT / f"src/{game}/data/battle/{name}.tblasm"
                source_text = source.read_text(encoding="utf-8")
                if name == "encount":
                    model = battle_tbl.parse_encount_source(source_text, symbols)
                    data = battle_tbl.encode_encount(model)
                    rendered = battle_tbl.render_encount_source(
                        battle_tbl.decode_encount(data), symbols
                    )
                elif name == "unit":
                    model = battle_tbl.parse_unit_source(source_text, symbols)
                    data = battle_tbl.encode_unit(model)
                    rendered = battle_tbl.render_unit_source(
                        battle_tbl.decode_unit(data), symbols
                    )
                elif name == "skill":
                    model = battle_tbl.parse_skill_source(source_text)
                    data = battle_tbl.encode_skill(model)
                    rendered = battle_tbl.render_skill_source(battle_tbl.decode_skill(data))
                elif name == "aicalc":
                    model = battle_tbl.parse_aicalc_source(
                        source_text, source.parent, symbols
                    )
                    data = battle_tbl.encode_aicalc(model)
                    rendered = battle_tbl.render_aicalc_source(
                        battle_tbl.decode_aicalc(data), symbols=symbols
                    )
                elif name == "msg":
                    model = battle_tbl.parse_message_source(source_text, source.parent)
                    data = battle_tbl.encode_message(model)
                    rendered = battle_tbl.render_message_source(
                        battle_tbl.decode_message(data)
                    )
                else:
                    self.fail(f"unhandled tracked battle table {name}")
                self.assertEqual(hashlib.sha1(data).hexdigest(), digest)
                self.assertEqual(rendered, source_text)

            encount = battle_tbl.parse_encount_source(
                (ROOT / f"src/{game}/data/battle/encount.tblasm").read_text(),
                symbols,
            )
            unit = battle_tbl.parse_unit_source(
                (ROOT / f"src/{game}/data/battle/unit.tblasm").read_text(),
                symbols,
            )
            battle_tbl.validate_encount_unit(encount, unit)
            skill = battle_tbl.parse_skill_source(
                (ROOT / f"src/{game}/data/battle/skill.tblasm").read_text()
            )
            battle_tbl.validate_skill_references(skill)
            battle_tbl.validate_unit_skill(unit, skill)
            aicalc_source = ROOT / f"src/{game}/data/battle/aicalc.tblasm"
            aicalc = battle_tbl.parse_aicalc_source(
                aicalc_source.read_text(encoding="utf-8"),
                aicalc_source.parent,
                symbols,
            )
            battle_tbl.validate_aicalc_references(aicalc, skill)
            battle_tbl.validate_aicalc_unit(aicalc, unit)


if __name__ == "__main__":
    unittest.main()

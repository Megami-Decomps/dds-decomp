"""Dialect-specific names for native FLW0 commands."""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class IntegerSymbols:
    values: tuple[tuple[int, str], ...]
    complete: bool = False

    @property
    def by_value(self) -> dict[int, str]:
        return dict(self.values)

    @property
    def by_name(self) -> dict[str, int]:
        return {name: value for value, name in self.values}


@dataclass(frozen=True)
class NativeCommand:
    command_id: int
    name: str
    stack_pop: int
    writes_result: bool | None = None
    argument_symbols: tuple[IntegerSymbols | None, ...] = ()

    def symbols_for_argument(self, index: int) -> IntegerSymbols | None:
        if not self.argument_symbols:
            return None
        return self.argument_symbols[index]


@dataclass(frozen=True)
class CommandProfile:
    name: str
    commands: tuple[NativeCommand, ...]
    event_ids: frozenset[int] = frozenset()

    @property
    def by_id(self) -> dict[int, NativeCommand]:
        return {command.command_id: command for command in self.commands}

    @property
    def by_name(self) -> dict[str, NativeCommand]:
        return {command.name: command for command in self.commands}

    @property
    def events_by_id(self) -> dict[int, str]:
        return {event_id: f"e{event_id:03d}" for event_id in self.event_ids}

    @property
    def events_by_name(self) -> dict[str, int]:
        return {name: event_id for event_id, name in self.events_by_id.items()}


SHARED_DDS_COMMANDS = (
    NativeCommand(0x000, "MESSAGE_REQUEST_AND_POLL", 1, writes_result=False),
    NativeCommand(0x001, "ACTIVATE_MESSAGE_PANEL", 0, writes_result=False),
    NativeCommand(0x002, "FINISH_SCRIPT_MESSAGE_WINDOW", 0, writes_result=False),
    NativeCommand(
        0x003, "MESSAGE_SELECTION_REQUEST_AND_POLL", 1, writes_result=True
    ),
    NativeCommand(0x007, "TEST_MODEL_FLAG", 1, writes_result=True),
    NativeCommand(0x008, "SET_MODEL_FLAG", 1, writes_result=False),
    NativeCommand(0x009, "CLEAR_MODEL_FLAG", 1, writes_result=False),
    NativeCommand(0x00A, "RANDOM_ONE_TO", 1, writes_result=True),
    NativeCommand(0x00D, "WAIT_FOR_TIMER_START", 0, writes_result=False),
    NativeCommand(0x00E, "WAIT_FOR_TIMER_LIMIT", 1, writes_result=False),
    NativeCommand(0x00F, "SCREEN_FADE_A", 2, writes_result=False),
    NativeCommand(0x010, "SCREEN_FADE_B", 2, writes_result=False),
    NativeCommand(0x012, "ADD_EFFECT_UNIT_TO_WORLD", 1, writes_result=False),
    NativeCommand(0x013, "START_CAMERA_PATH_MOVE", 2, writes_result=False),
    NativeCommand(0x015, "CREATE_LINKED_CAMERA_VIEWER", 2, writes_result=True),
    NativeCommand(0x016, "DESTROY_WORLD_UNIT", 1, writes_result=False),
    NativeCommand(
        0x017, "START_UNIT_MOVE_TO_POSITION_OBJECT", 5, writes_result=False
    ),
    NativeCommand(
        0x019, "ADD_FLAGGED_EFFECT_UNIT_TO_WORLD", 1, writes_result=False
    ),
    NativeCommand(
        0x01A,
        "SET_CONTROLLER_VIBRATION",
        3,
        writes_result=False,
        argument_symbols=(
            IntegerSymbols(((0, "SMALL_MOTOR"), (1, "LARGE_MOTOR"))),
            None,
            None,
        ),
    ),
    NativeCommand(0x01F, "FADE_BACKGROUND_IN", 1, writes_result=False),
    NativeCommand(0x027, "READ_SOLAR_PHASE", 0, writes_result=True),
    NativeCommand(0x028, "SUBMIT_EVENT_WITH_MODE", 2, writes_result=False),
    NativeCommand(0x043, "RESET_DRAW_EFFECTS", 0, writes_result=False),
    NativeCommand(0x046, "RETURN_TO_TITLE", 0, writes_result=False),
    NativeCommand(0x049, "WAIT_FOR_UNIT_MOTION", 1, writes_result=False),
    NativeCommand(
        0x04A, "ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR", 2, writes_result=False
    ),
    NativeCommand(0x04B, "SET_UNIT_VALUE", 2, writes_result=False),
    NativeCommand(
        0x05E, "ACTION_WINDOW_REQUEST_AND_POLL", 1, writes_result=True
    ),
    NativeCommand(0x060, "RESTORE_CAMERA_NODE_MODE", 0, writes_result=False),
    NativeCommand(0x061, "RELEASE_CURRENT_OBJECT", 0, writes_result=False),
    NativeCommand(0x066, "CALL_EVENT", 1, writes_result=False),
    NativeCommand(0x067, "SUBMIT_EVENT", 1, writes_result=False),
    NativeCommand(0x068, "READ_CURRENT_WORLD_OBJECT_ID", 0, writes_result=True),
    NativeCommand(0x069, "CLEAR_UNIT_LOW_FLAG", 1, writes_result=False),
    NativeCommand(0x06A, "SET_UNIT_LOW_FLAG", 1, writes_result=False),
    NativeCommand(0x06B, "MOVE_OBJECT_ALONG_PATH", 3, writes_result=False),
    NativeCommand(0x06C, "WAIT_FOR_OBJECT_PATH", 1, writes_result=False),
    NativeCommand(0x070, "CHANGE_ITEM_COUNT", 2, writes_result=False),
    NativeCommand(0x071, "SET_MESSAGE_WINDOW_GEOMETRY", 3, writes_result=False),
    NativeCommand(0x073, "PREPARE_UNIT_MOTION_STATE", 5, writes_result=False),
    NativeCommand(0x08B, "START_UNIT_PATH_FOLLOW", 7, writes_result=False),
    NativeCommand(0x094, "READ_SECONDARY_WORLD_ID_VALUE", 1, writes_result=True),
    NativeCommand(0x097, "DEFER_BATTLE_EXIT", 3, writes_result=False),
    NativeCommand(
        0x098, "REQUEST_CURRENT_GROUP_SEQUENCE", 2, writes_result=False
    ),
    NativeCommand(0x099, "RESET_FIELD_EFFECTS", 0, writes_result=False),
    NativeCommand(0x09B, "DESTROY_WORLD_EFFECT_OBJECT", 1, writes_result=False),
    NativeCommand(
        0x09D,
        "COPY_EFFECT_OBJECT_TRANSFORM_FROM_SOURCE",
        2,
        writes_result=False,
    ),
    NativeCommand(0x0A3, "FOCUS_CAMERA_ON_OBJECT", 1, writes_result=False),
    NativeCommand(0x0A4, "UPDATE_FIELD_LOOK_AT_SEGMENT", 0, writes_result=False),
    NativeCommand(0x0A5, "CREATE_SCRIPT_TASK", 2, writes_result=True),
    NativeCommand(0x0A6, "DESTROY_REGISTERED_TASK", 1, writes_result=False),
    NativeCommand(0x0A7, "WAIT_FOR_TASK_REMOVAL", 1, writes_result=False),
    NativeCommand(0x0AA, "CREATE_POLYGON_MOVIE", 2, writes_result=True),
    NativeCommand(0x0C3, "SET_SOLAR_OVERLAY_MODE", 1, writes_result=False),
    NativeCommand(0x0C8, "WAIT_FOR_CAMP_TASK", 1, writes_result=False),
    NativeCommand(0x0C9, "BIND_MODEL_MOTION_SOUND", 2, writes_result=True),
    NativeCommand(0x0CC, "CREATE_EVENT_TEXTURE_TASK", 2, writes_result=True),
    NativeCommand(0x0CD, "CREATE_FLAGGED_EFFECT_OBJECT", 1, writes_result=True),
    NativeCommand(0x0CE, "SET_EFFECT_MODEL_CUT", 2, writes_result=False),
    NativeCommand(0x0CF, "SET_EFFECT_MODEL_ROTATION", 4, writes_result=False),
    NativeCommand(0x0D0, "CREATE_EVENT_BED_EFFECT", 2, writes_result=True),
    NativeCommand(0x0D1, "ATTACH_EFFECT_TO_PATH", 2, writes_result=False),
    NativeCommand(0x0D2, "WAIT_FOR_EFFECT_PATH", 1, writes_result=False),
    NativeCommand(0x0D3, "CREATE_MG1_EFFECT", 1, writes_result=True),
    NativeCommand(0x0D4, "CREATE_EVENT_MG1_EFFECT", 2, writes_result=True),
    NativeCommand(0x0D5, "CREATE_MG2_EFFECT", 1, writes_result=True),
    NativeCommand(0x0D6, "CREATE_EVENT_MG2_EFFECT", 2, writes_result=True),
    NativeCommand(0x0D7, "SET_MG1_EFFECT_POINTS", 3, writes_result=False),
    NativeCommand(0x0D8, "SET_MG2_EFFECT_POINTS", 5, writes_result=False),
    NativeCommand(0x0D9, "START_EVENT_BGM", 2, writes_result=False),
    NativeCommand(0x0DB, "SET_WORLD_NODE_BASE_MODE", 1, writes_result=False),
    NativeCommand(0x0E3, "DESTROY_EFFECT_OBJECT", 1, writes_result=False),
    NativeCommand(0x0F7, "START_CAMP_TASK_IF_ABSENT", 1, writes_result=False),
    NativeCommand(0x0F8, "CAMP_TASK_READY", 1, writes_result=True),
    NativeCommand(
        0x100, "REQUEST_ALTERNATE_FIELD_SEQUENCE", 2, writes_result=False
    ),
    NativeCommand(0x101, "SET_FIELD_ENVIRONMENT", 2, writes_result=False),
    NativeCommand(0x103, "ENABLE_FIELD_MODELS", 4, writes_result=False),
    NativeCommand(0x104, "DISABLE_FIELD_MODELS", 4, writes_result=False),
    NativeCommand(0x105, "ENABLE_FIELD_ANIMATION", 4, writes_result=False),
    NativeCommand(0x106, "DISABLE_FIELD_ANIMATION", 4, writes_result=False),
    NativeCommand(0x107, "ENABLE_FIELD_COLLISION", 3, writes_result=False),
    NativeCommand(0x108, "DISABLE_FIELD_COLLISION", 3, writes_result=False),
    NativeCommand(
        0x109, "ENABLE_FIELD_MODEL_GROUP", 3, writes_result=False
    ),
    NativeCommand(
        0x10A, "DISABLE_FIELD_MODEL_GROUP", 3, writes_result=False
    ),
    NativeCommand(0x10E, "ENABLE_FIELD_NPCS", 3, writes_result=False),
    NativeCommand(0x10F, "DISABLE_FIELD_NPCS", 3, writes_result=False),
    NativeCommand(
        0x110, "SET_FIELD_GIMMICK_DISPLAY", 4, writes_result=False
    ),
    NativeCommand(0x111, "ENABLE_FIELD_MAP_ENTRY", 3, writes_result=False),
    NativeCommand(0x112, "DISABLE_FIELD_MAP_ENTRY", 3, writes_result=False),
    NativeCommand(0x113, "SET_FIELD_CAMERA_TABLE", 1, writes_result=False),
    NativeCommand(
        0x114,
        "READ_TREASURE_TABLE_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "CONTENT_KIND"),
                    (1, "ITEM_ID"),
                    (2, "ITEM_QUANTITY"),
                    (3, "TRAP_KIND"),
                    (4, "AMOUNT"),
                )
            ),
        ),
    ),
    NativeCommand(
        0x115, "MARK_CURRENT_TREASURE_OPENED", 0, writes_result=False
    ),
    NativeCommand(
        0x116, "TEST_CURRENT_TREASURE_OPENED", 0, writes_result=True
    ),
    NativeCommand(
        0x11B, "START_AND_WAIT_FOR_STREAM_SOUND", 1, writes_result=False
    ),
    NativeCommand(0x11C, "ADVANCE_STREAM_SOUND_STATE", 0, writes_result=False),
    NativeCommand(0x11F, "RESET_STREAM_PLAYBACK", 0, writes_result=False),
    NativeCommand(0x121, "WAIT_FOR_STREAM_IDLE", 0, writes_result=False),
    NativeCommand(
        0x124, "CLEAR_WORLD_OBJECT_STATE_FLAGS", 1, writes_result=False
    ),
    NativeCommand(0x139, "ADD_PARTY_CURRENCY", 1, writes_result=False),
    NativeCommand(
        0x13A,
        "APPLY_PARTY_TRAP_EFFECT",
        1,
        writes_result=False,
        argument_symbols=(
            IntegerSymbols(
                (
                    (1, "DAMAGE_TEN_PERCENT_HP"),
                    (2, "DAMAGE_HALF_HP"),
                    (3, "REDUCE_HP_TO_ONE"),
                    (4, "INFLICT_POISON"),
                    (5, "INFLICT_ACHE"),
                    (6, "INFLICT_CLOSE"),
                )
            ),
        ),
    ),
    NativeCommand(0x13C, "SET_MESSAGE_RANGE", 2, writes_result=False),
    NativeCommand(0x166, "SUBMIT_EVENT_IMMEDIATE", 1, writes_result=False),
    NativeCommand(
        0x1E0, "QUEUE_WORLD_OBJECT_PENDING_VALUE", 2, writes_result=False
    ),
    NativeCommand(
        0x1E1, "CLEAR_WORLD_OBJECT_PENDING_VALUE", 1, writes_result=False
    ),
    NativeCommand(0x1E7, "CLEAR_PROCESS_CONTROL_FLAG", 0, writes_result=False),
    NativeCommand(
        0x1F1,
        "CONSUME_FIELD_SKILL_END_NOTICE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "LIGHTOMA"),
                    (1, "LIFTOMA"),
                    (2, "RIBERAMA"),
                    (3, "ESTOMA"),
                )
            ),
        ),
    ),
    NativeCommand(0x1F3, "START_ARCHIVE_SOUND", 2, writes_result=False),
    NativeCommand(0x1F4, "STOP_ARCHIVE_SOUND", 2, writes_result=False),
    NativeCommand(
        0x1F6, "REVEAL_AUTOMAP_RECTANGLE", 5, writes_result=False
    ),
    NativeCommand(
        0x1FA,
        "READ_SUCTION_WARP_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "STATE_CODE"),
                    (1, "SOURCE_VECTOR_ID"),
                    (2, "EFFECT_UNIT_ID"),
                    (3, "MAP_ENTRY_ID"),
                    (4, "MOTION_ID"),
                )
            ),
        ),
    ),
    NativeCommand(
        0x1FB,
        "READ_BARRIER_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "BARRIER_MODEL_FLAG"),
                    (1, "EFFECT_UNIT_ID"),
                    (2, "SOURCE_VECTOR_ID"),
                    (5, "COMPLETION_FLAG"),
                    (6, "MAP_ENTRY_ID"),
                )
            ),
        ),
    ),
    NativeCommand(
        0x1FE,
        "READ_CURRENT_SCENE_SELECTION_RESOURCE",
        0,
        writes_result=True,
    ),
    NativeCommand(0x1FF, "FIND_FIELD_EFFECT_BY_NAME", 1, writes_result=True),
    NativeCommand(
        0x200,
        "READ_ELEVATOR_TABLE_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "DESTINATION_COUNT"),
                    (14, "REMAINING_DESTINATIONS"),
                )
            ),
        ),
    ),
    NativeCommand(
        0x201, "RUN_FIELD_DESTINATION_TRANSITION", 1, writes_result=False
    ),
    NativeCommand(0x202, "SET_CURRENT_TASK_SCENE", 0, writes_result=False),
    NativeCommand(0x203, "NO_OP_FIELD_TRANSITION", 0, writes_result=False),
    NativeCommand(
        0x204, "APPLY_CURRENT_TASK_ENTRY_TRIGGER", 0, writes_result=False
    ),
    NativeCommand(0x205, "ADVANCE_FIELD_INTERACTION", 2, writes_result=True),
    NativeCommand(
        0x206,
        "READ_FIELD_INTERACTION_VALUE",
        2,
        writes_result=True,
        argument_symbols=(
            None,
            IntegerSymbols(((0, "ROW_TYPE"), (1, "MESSAGE_ID"))),
        ),
    ),
    NativeCommand(0x207, "READ_FIELD_INTERACTION_KIND", 0, writes_result=True),
    NativeCommand(
        0x208,
        "READ_LADDER_TABLE_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(
                (
                    (0, "DIRECTION"),
                    (1, "SOURCE_VECTOR_ID"),
                    (2, "EFFECT_UNIT_ID"),
                    (3, "USE_DIRECT_ACTION_WINDOW"),
                )
            ),
        ),
    ),
    NativeCommand(
        0x209, "CONFIGURE_ELEVATOR_CAMERA_MOVE", 2, writes_result=False
    ),
    NativeCommand(
        0x20A,
        "RESET_ELEVATOR_CAMERA_MOVE_TRACKING",
        0,
        writes_result=False,
    ),
    NativeCommand(0x20B, "POLL_ELEVATOR_MOVE_STATE", 0, writes_result=True),
    NativeCommand(
        0x20C,
        "READ_DOOR_WARP_VALUE",
        1,
        writes_result=True,
        argument_symbols=(
            IntegerSymbols(((0, "MOTION_DURATION"), (1, "FADE_MODE"))),
        ),
    ),
    NativeCommand(
        0x20D, "APPLY_CURRENT_TASK_RECORD_ENTRY", 0, writes_result=False
    ),
    NativeCommand(
        0x20E, "START_CURRENT_FIELD_INTERACTION_EVENT", 0, writes_result=False
    ),
    NativeCommand(
        0x20F,
        "RUN_FIELD_TRANSITION_SELECTOR",
        1,
        writes_result=False,
        argument_symbols=(
            IntegerSymbols(((900, "HEAL_FACILITY"), (901, "SAVE_POINT"))),
        ),
    ),
    NativeCommand(0x210, "START_SCENE_BGM", 1, writes_result=False),
    NativeCommand(0x214, "PLAY_FIELD_SE_VOLUME_PAN", 1, writes_result=False),
    NativeCommand(0x215, "PLAY_FIELD_SE", 1, writes_result=False),
    NativeCommand(0x216, "RELEASE_CURRENT_BGM", 0, writes_result=False),
    NativeCommand(
        0x217, "LOAD_ARCHIVE_SOUND_BANK_AND_WAIT", 1, writes_result=False
    ),
    NativeCommand(0x219, "READ_WARP_EFFECT_MODE", 0, writes_result=True),
    NativeCommand(
        0x21D,
        "ACTION_WINDOW_REQUEST_AND_POLL_DIRECT",
        1,
        writes_result=True,
    ),
    NativeCommand(0x21E, "APPLY_FIELD_MODEL_LIGHTING", 1, writes_result=False),
)


FIELD_RUNTIME_COMMANDS = (
    # AICALC assigns 0x0AB to its battle-AI command table instead.
    NativeCommand(0x0AB, "SETUP_FADE_FRAMES", 2, writes_result=False),
)


DDS2_FIELD_COMMANDS = (
    NativeCommand(0x1FC, "APPLY_ROOM_MODE_GROUP_ZERO", 4, writes_result=False),
    NativeCommand(0x1FD, "APPLY_ROOM_MODE_GROUP_ONE", 4, writes_result=False),
    NativeCommand(0x21A, "RESET_FIELD_AFTER_EVENT", 0, writes_result=False),
)


# AICALC formula procedures run through the ordinary FLW0 VM but use this
# compact group of native accessors for their calculation context.  The names
# describe the values read by the native handlers; they deliberately leave the
# still-unclassified lookup curves as factors rather than assigning gameplay
# meaning that the executable does not establish.
BATTLE_CALC_COMMANDS = (
    NativeCommand(0x16C, "CALC_SET_RESULT", 1, writes_result=False),
    NativeCommand(0x16D, "CALC_SOURCE_LEVEL", 0, writes_result=True),
    NativeCommand(0x16E, "CALC_TARGET_LEVEL", 0, writes_result=True),
    NativeCommand(0x16F, "CALC_SOURCE_STAT", 1, writes_result=True),
    NativeCommand(0x170, "CALC_TARGET_STAT", 1, writes_result=True),
    NativeCommand(0x171, "CALC_ACTION_HIT_LEVEL", 0, writes_result=True),
    NativeCommand(0x172, "CALC_ACTION_AILMENT_LEVEL", 0, writes_result=True),
    NativeCommand(0x173, "CALC_ACTION_POWER", 0, writes_result=True),
    NativeCommand(0x174, "CALC_ACTION_MAGIC_BASE", 0, writes_result=True),
    NativeCommand(0x175, "CALC_SOURCE_FLAG_20_CLEAR", 0, writes_result=True),
    NativeCommand(0x176, "CALC_SOURCE_ACTION_AFFINITY", 0, writes_result=True),
    NativeCommand(0x177, "CALC_TARGET_ACTION_AFFINITY", 0, writes_result=True),
    NativeCommand(0x178, "CALC_SOURCE_ATTACK_AFFINITY", 0, writes_result=True),
    NativeCommand(0x179, "CALC_TARGET_ATTACK_AFFINITY", 0, writes_result=True),
    NativeCommand(0x17A, "CALC_RANDOM_SCALE", 1, writes_result=True),
    NativeCommand(0x17B, "CALC_CURRENT_RESULT", 0, writes_result=True),
    NativeCommand(0x17C, "CALC_ACTION_MAGIC_LIMIT", 0, writes_result=True),
    NativeCommand(0x17D, "CALC_GROUP_AVERAGE_LEVEL", 1, writes_result=True),
    NativeCommand(0x17E, "CALC_GROUP_AVERAGE_STAT", 2, writes_result=True),
    NativeCommand(0x17F, "CALC_ENCOUNTER_ZONE_FACTOR", 0, writes_result=True),
    NativeCommand(0x180, "CALC_ESCAPE_BONUS_COUNTER", 0, writes_result=True),
    NativeCommand(0x181, "CALC_SOURCE_HP", 0, writes_result=True),
    NativeCommand(0x182, "CALC_TARGET_HP", 0, writes_result=True),
    NativeCommand(0x183, "CALC_SOURCE_MAX_HP", 0, writes_result=True),
    NativeCommand(0x184, "CALC_TARGET_MAX_HP", 0, writes_result=True),
    NativeCommand(0x186, "CALC_LEVEL_MAX_HP_FACTOR", 0, writes_result=True),
    NativeCommand(0x187, "CALC_LEVEL_MAX_MP_FACTOR", 0, writes_result=True),
    NativeCommand(0x188, "CALC_TARGET_HP_BAND_FACTOR", 0, writes_result=True),
    NativeCommand(0x189, "CALC_LEVEL_FACTOR_360", 0, writes_result=True),
    NativeCommand(0x18A, "CALC_LEVEL_FACTOR_4EC", 0, writes_result=True),
    NativeCommand(0x18B, "CALC_ROLL_TARGET_FLAG_RESULT", 0, writes_result=True),
    NativeCommand(0x18C, "CALC_ACTION_DEATH_TYPE", 0, writes_result=True),
    NativeCommand(0x18D, "CALC_LEVEL_CRITICAL_FACTOR", 0, writes_result=True),
    NativeCommand(0x18E, "CALC_LEVEL_RECOVERY_FACTOR", 0, writes_result=True),
    NativeCommand(0x195, "CALC_SOURCE_ROSTER_BASE", 0, writes_result=True),
    NativeCommand(0x1A5, "CALC_TARGET_HP_FINE_FACTOR", 0, writes_result=True),
    NativeCommand(0x1D0, "CALC_SOURCE_ATTACK_POWER", 0, writes_result=True),
    NativeCommand(0x1D2, "CALC_GROUP_AVERAGE_MAX_HP", 1, writes_result=True),
    NativeCommand(0x1D3, "CALC_GROUP_AVERAGE_HP", 1, writes_result=True),
)


DDS2_BATTLE_CALC_COMMANDS = (
    NativeCommand(0x05A, "CALC_MONEY_BASE", 0, writes_result=True),
    NativeCommand(0x162, "CALC_MONEY_LEVEL_FACTOR", 0, writes_result=True),
)


# These battle commands are present in both native command tables and are used
# by ordinary battle scripts as well as AICALC.  The four TRACE commands call
# only the retail battle-debug printer; their names deliberately do not imply
# camera or retreat side effects.
BATTLE_RUNTIME_COMMANDS = (
    NativeCommand(0x0DF, "AI_COUNTER_REACHED_LIMIT", 1, writes_result=True),
    NativeCommand(0x0E2, "AI_SELECT_ACTION_BY_KIND", 2, writes_result=False),
    NativeCommand(0x0E4, "TRACE_BATTLE_RETREAT", 0, writes_result=False),
    NativeCommand(0x0E5, "TRACE_BATTLE_ALL_RETREAT", 0, writes_result=False),
    NativeCommand(0x0E6, "AI_RESET_COMMAND_CONTEXT", 0, writes_result=False),
    NativeCommand(
        0x0E7,
        "AI_SELECT_LOWEST_HP_TARGET_BLOCKING_ELEMENT",
        1,
        writes_result=False,
    ),
    NativeCommand(0x0F4, "AI_MOVE_CAMERA", 7, writes_result=False),
    NativeCommand(0x0F5, "TRACE_BATTLE_CAMERA_ORIGINAL", 0, writes_result=False),
    NativeCommand(0x0F6, "AI_ENABLE_COMMAND_STATE_FLAG", 0, writes_result=False),
    NativeCommand(0x0FA, "AI_QUEUE_ACTOR_COMMAND_SOUND", 0, writes_result=False),
    NativeCommand(0x0FB, "TRACE_BATTLE_CAMERA_TWO_SHOT", 0, writes_result=False),
    NativeCommand(
        0x0FC, "TRACE_BATTLE_CAMERA_OBSTRUCTION", 0, writes_result=False
    ),
)


# The AICALC AI programs use the same VM with a battle-command vocabulary.
# Argument counts come from the native command table.  Query names follow the
# packed-action handlers reached by each command; operational names follow the
# command handler and its immediate callee.  Group queries use the same native
# 0x200/0x400 player/enemy filters as the established count and mask commands.
# Commands whose gameplay role is still ambiguous deliberately remain numeric
# in source.
BATTLE_AI_COMMANDS = (
    NativeCommand(0x030, "AI_SELECT_BASIC_ATTACK", 0, writes_result=False),
    NativeCommand(0x031, "AI_SELECT_ESCAPE", 0, writes_result=False),
    NativeCommand(0x032, "AI_SELECT_WAIT", 0, writes_result=False),
    NativeCommand(0x033, "AI_SELECT_SKILL", 1, writes_result=False),
    NativeCommand(0x034, "AI_SELECT_ACTION_TARGETS", 0, writes_result=False),
    NativeCommand(0x035, "AI_SELECT_LOWEST_HP_TARGET", 0, writes_result=False),
    NativeCommand(0x036, "AI_SELECT_TARGETS_WITH_ACTION_MASK", 1, writes_result=False),
    NativeCommand(0x037, "AI_SELECT_TARGET_BY_ID", 1, writes_result=False),
    NativeCommand(0x039, "AI_ANY_PLAYER_HAS_ACTION", 1, writes_result=True),
    NativeCommand(0x03A, "AI_ANY_ENEMY_HAS_ACTION", 1, writes_result=True),
    NativeCommand(0x03B, "AI_SET_BATTLE_REQUEST_ARGUMENT", 1, writes_result=False),
    NativeCommand(0x03D, "AI_SELECT_TABLE_ACTION", 0, writes_result=False),
    NativeCommand(0x03E, "AI_CLEAR_SCENE_TRANSITION", 0, writes_result=False),
    NativeCommand(0x03F, "AI_BEGIN_SCENE_TRANSITION", 0, writes_result=False),
    NativeCommand(0x07B, "AI_UNIT_HP_AT_OR_BELOW_RATE", 1, writes_result=True),
    NativeCommand(0x07C, "AI_BOSS_HP_AT_OR_BELOW_RATE", 1, writes_result=True),
    NativeCommand(0x07D, "AI_SELECT_LOWEST_HP_RATE_TARGET", 0, writes_result=False),
    NativeCommand(0x07F, "AI_PLAYER_COUNT_AT_MOST", 1, writes_result=True),
    NativeCommand(0x085, "AI_HAS_OTHER_ENEMY_UNIT_MODE", 1, writes_result=True),
    NativeCommand(
        0x086, "AI_ANY_PLAYER_PASSES_ACTION_CHECK", 1, writes_result=True
    ),
    NativeCommand(
        0x087, "AI_ANY_ENEMY_PASSES_ACTION_CHECK", 1, writes_result=True
    ),
    NativeCommand(0x089, "AI_NO_PLAYER_BLOCKS_QUERY", 1, writes_result=True),
    NativeCommand(0x08C, "AI_UNIT_PASSES_ACTION_TEN_CHECK", 0, writes_result=True),
    NativeCommand(0x0AB, "START_SCREEN_QUAKE", 2, writes_result=False),
    NativeCommand(0x14C, "AI_ACTOR_HISTORY_COUNTER", 0, writes_result=True),
    NativeCommand(0x15B, "AI_SELECT_LOWEST_LEVEL_TARGET", 0, writes_result=False),
    NativeCommand(0x19A, "AI_ANY_PLAYER_PASSES_QUERY", 1, writes_result=True),
    NativeCommand(0x19B, "AI_ANY_ENEMY_PASSES_QUERY", 1, writes_result=True),
    NativeCommand(0x19C, "AI_ALL_PLAYERS_PASS_QUERY", 1, writes_result=True),
    NativeCommand(0x19D, "AI_ALL_ENEMIES_PASS_QUERY", 1, writes_result=True),
    NativeCommand(
        0x1A0,
        "AI_ACTOR_AVAILABLE_WITH_STAT_FLAG_2000",
        0,
        writes_result=True,
    ),
    NativeCommand(0x1A2, "AI_BATTLE_READY_WITH_ZERO_TURNS", 0, writes_result=True),
    NativeCommand(0x1A3, "AI_CONTEXT_FLAG_TWO_SET", 0, writes_result=True),
    NativeCommand(0x1A6, "AI_SPECIAL_MODE_EFFECT_VALUE", 0, writes_result=True),
    NativeCommand(0x1A7, "AI_UNIT_ACTION_MODE_ZERO", 1, writes_result=True),
    NativeCommand(0x1A9, "AI_ANY_PLAYER_ACTION_MODE_ZERO", 1, writes_result=True),
    NativeCommand(0x1AB, "AI_ANY_ENEMY_ACTION_MODE_ZERO", 1, writes_result=True),
    NativeCommand(
        0x1AF,
        "AI_SELECT_PLAYER_TARGET_WITHOUT_FLAG_1000",
        0,
        writes_result=False,
    ),
    NativeCommand(0x1B0, "AI_APPEND_SELF_TO_TARGETS", 0, writes_result=False),
    NativeCommand(0x1B1, "AI_SELECT_OTHER_TARGET_OR_SELF", 0, writes_result=False),
    NativeCommand(0x1B6, "AI_EFFECT_ACTIVE", 0, writes_result=True),
    NativeCommand(0x1B7, "AI_BATTLE_PHASE", 0, writes_result=True),
    NativeCommand(
        0x1B8, "AI_ANY_PLAYER_NOT_ACTION_MODE_ZERO", 1, writes_result=True
    ),
    NativeCommand(
        0x1BA, "AI_SELECT_TARGETS_WITHOUT_ACTION_MASK", 1, writes_result=False
    ),
    NativeCommand(0x1BD, "AI_EFFECT_VALUE", 0, writes_result=True),
    NativeCommand(0x1BE, "AI_SCENE_FADE_COUNT", 0, writes_result=True),
    NativeCommand(0x1BF, "AI_ALL_PLAYERS_LACK_FLAG_1000", 0, writes_result=True),
    NativeCommand(0x1C0, "AI_ALL_PLAYERS_HAVE_FLAG_1000", 0, writes_result=True),
    NativeCommand(
        0x1C1, "AI_SELECT_PLAYER_TARGET_WITH_FLAG_1000", 0, writes_result=False
    ),
    NativeCommand(0x1C4, "AI_APPEND_EFFECT_ACTOR_TO_TARGETS", 0, writes_result=False),
    NativeCommand(0x1C5, "AI_GLOBAL_HISTORY_COUNTER", 0, writes_result=True),
    NativeCommand(0x1C6, "AI_ANY_PLAYER_BLOCKS_ELEMENT", 1, writes_result=True),
    NativeCommand(0x1C7, "AI_SELECT_TARGETS_BLOCKING_ELEMENT", 1, writes_result=False),
    NativeCommand(0x1C8, "AI_ANY_ENEMY_HAS_QUEUED_ACTION", 1, writes_result=True),
    NativeCommand(0x1C9, "AI_ANY_PLAYER_HAS_QUEUED_ACTION", 1, writes_result=True),
    NativeCommand(0x1CA, "AI_SET_ACTOR_UNIT_PARAMETER", 1, writes_result=False),
    NativeCommand(0x1CB, "AI_SELECT_HIGHEST_MP_TARGET", 0, writes_result=False),
    NativeCommand(
        0x1CC, "AI_SELECT_TARGET_PASSING_QUERY", 1, writes_result=False
    ),
    NativeCommand(0x1D1, "AI_CLEAR_SPECIAL_ENEMY_ENTRY_FLAGS", 0, writes_result=False),
    NativeCommand(0x1D4, "AI_QUEUE_UNBOUND_COMMAND_SOUND", 0, writes_result=False),
    NativeCommand(0x1D5, "AI_SET_CAMERA_BLEND_START", 7, writes_result=False),
    NativeCommand(0x1D6, "AI_SET_CAMERA_BLEND_END", 7, writes_result=False),
    NativeCommand(0x1D7, "AI_RUN_CAMERA_BLEND", 2, writes_result=False),
)


DDS1_BATTLE_AI_COMMANDS = (
    NativeCommand(0x07E, "AI_ENEMY_COUNT_AT_MOST", 1, writes_result=True),
    NativeCommand(0x081, "AI_ANY_ENEMY_HAS_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x082, "AI_ANY_PLAYER_HAS_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x083, "AI_ALL_PLAYERS_HAVE_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x14B, "AI_UNIT_MP_AT_OR_BELOW_RATE", 1, writes_result=True),
    NativeCommand(0x19E, "AI_ENEMY_HAS_ACTION", 1, writes_result=True),
    NativeCommand(
        0x1AD, "AI_ANY_ENEMY_CURRENT_ACTION_MATCHES", 1, writes_result=True
    ),
    NativeCommand(
        0x1BB, "AI_ANY_PLAYER_CURRENT_ACTION_MATCHES", 1, writes_result=True
    ),
)


DDS2_BATTLE_AI_COMMANDS = (
    NativeCommand(0x01D, "AI_SELECT_WEIGHTED_TABLE_ENTRY", 1, writes_result=False),
    NativeCommand(0x020, "AI_HAS_UNIT_OR_SLOT_ACTION_MASK", 2, writes_result=True),
    NativeCommand(0x05B, "AI_SET_CONTEXT_FLAG_ONE", 0, writes_result=False),
    NativeCommand(0x05C, "AI_ACTOR_CAN_USE_ACTION", 1, writes_result=True),
    NativeCommand(0x07E, "AI_ENEMY_COUNT_AT_MOST", 1, writes_result=True),
    NativeCommand(0x080, "AI_UNIT_HAS_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x082, "AI_ANY_PLAYER_HAS_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x083, "AI_ALL_PLAYERS_HAVE_ACTION_MASK", 1, writes_result=True),
    NativeCommand(0x084, "AI_HAS_PLAYER_UNIT_MODE", 1, writes_result=True),
    NativeCommand(0x0E0, "AI_TURN_COUNT", 0, writes_result=True),
    NativeCommand(0x0E1, "AI_SELECT_DIRECT_ACTION", 1, writes_result=False),
    NativeCommand(0x122, "AI_LINKED_ACTION_SCENE_ACTIVE", 0, writes_result=True),
    NativeCommand(
        0x141, "AI_HAS_ELIGIBLE_QUEUED_SPECIAL_ACTION", 0, writes_result=True
    ),
    NativeCommand(0x151, "AI_HAS_FLAG_800000", 1, writes_result=True),
    NativeCommand(0x15A, "AI_ROLL_ONE_BASED_BUCKET", 0, writes_result=True),
    NativeCommand(0x1AE, "AI_ANY_PLAYER_LACKS_FLAG_1000", 0, writes_result=True),
    NativeCommand(
        0x1B9, "AI_ANY_ENEMY_NOT_ACTION_MODE_ZERO", 1, writes_result=True
    ),
    NativeCommand(0x1D8, "AI_ACTIVE_SUBTASK", 0, writes_result=True),
    NativeCommand(0x1D9, "AI_SUBTASK_TARGET_MODE", 0, writes_result=True),
    NativeCommand(0x1DA, "AI_SELECT_TARGETS_BY_UNIT_MODE", 1, writes_result=False),
    NativeCommand(0x1DB, "AI_SPECIAL_BATTLE_OBJECT_VALUE", 0, writes_result=True),
    NativeCommand(0x1DC, "AI_APPEND_CURRENT_UNIT_TO_TARGETS", 0, writes_result=False),
    NativeCommand(0x1DE, "AI_MARKED_ACTION_SCENE_ACTIVE", 0, writes_result=True),
    NativeCommand(0x1E9, "AI_SET_SPECIAL_EFFECT_ACTOR_BYTE", 1, writes_result=False),
)

DDS1_EVENT_IDS = frozenset(
    (
        500, 501, 502, 503, 506, 510, 550, 601, 602, 603, 604, 605,
        606, 607, 608, 609, 610, 611, 612, 613, 614, 615, 616, 617,
        618, 619, 620, 621, 622, 623, 624, 625, 626, 627, 628, 629,
        630, 631, 632, 633, 634, 635, 636, 637, 638, 639, 640, 641,
        642, 643, 644, 645, 646, 647, 648, 649, 650, 651, 652, 653,
        654, 655, 656, 657, 658, 659, 660, 661, 662, 663, 670, 697,
        700, 701, 702, 703, 704, 705, 710, 799, 802, 803, 806, 807,
        809, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911,
        912, 914, 915, 916, 917, 918, 919, 920,
    )
)

DDS2_EVENT_IDS = frozenset(
    (
        562, 601, 602, 603, 604, 605, 606, 607, 608, 609, 610, 611,
        612, 613, 614, 615, 616, 617, 618, 619, 620, 621, 622, 623,
        624, 625, 626, 627, 628, 629, 630, 631, 632, 633, 634, 635,
        636, 637, 638, 639, 640, 641, 642, 643, 644, 645, 646, 647,
        648, 649, 650, 651, 652, 653, 654, 655, 656, 657, 658, 659,
        660, 661, 662, 663, 669, 670, 671, 672, 673, 674, 675, 802,
        803, 901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911,
        912, 913, 914, 915, 916, 917, 918, 919, 920, 921, 922, 923,
        924, 925, 926, 927, 928, 929, 930,
    )
)

DDS1 = CommandProfile(
    "dds1",
    SHARED_DDS_COMMANDS + FIELD_RUNTIME_COMMANDS + BATTLE_RUNTIME_COMMANDS,
    DDS1_EVENT_IDS,
)
DDS2 = CommandProfile(
    "dds2",
    SHARED_DDS_COMMANDS
    + FIELD_RUNTIME_COMMANDS
    + DDS2_FIELD_COMMANDS
    + BATTLE_RUNTIME_COMMANDS,
    DDS2_EVENT_IDS,
)
DDS1_AICALC = CommandProfile(
    "dds1-aicalc",
    SHARED_DDS_COMMANDS
    + BATTLE_RUNTIME_COMMANDS
    + BATTLE_CALC_COMMANDS
    + BATTLE_AI_COMMANDS
    + DDS1_BATTLE_AI_COMMANDS,
    DDS1_EVENT_IDS,
)
DDS2_AICALC = CommandProfile(
    "dds2-aicalc",
    SHARED_DDS_COMMANDS
    + BATTLE_RUNTIME_COMMANDS
    + BATTLE_CALC_COMMANDS
    + DDS2_BATTLE_CALC_COMMANDS
    + BATTLE_AI_COMMANDS
    + DDS2_BATTLE_AI_COMMANDS,
    DDS2_EVENT_IDS,
)

PROFILES = {
    profile.name: profile for profile in (DDS1, DDS2, DDS1_AICALC, DDS2_AICALC)
}


def get(name: str) -> CommandProfile:
    """Return a command profile by its source-format name."""

    return PROFILES[name]


for _profile in PROFILES.values():
    assert len(_profile.by_id) == len(_profile.commands)
    assert len(_profile.by_name) == len(_profile.commands)
    assert len(_profile.events_by_id) == len(_profile.event_ids)
    assert len(_profile.events_by_name) == len(_profile.event_ids)
    for _command in _profile.commands:
        assert not _command.argument_symbols or (
            len(_command.argument_symbols) == _command.stack_pop
        )
        for _symbols in _command.argument_symbols:
            if _symbols is None:
                continue
            assert len(_symbols.by_value) == len(_symbols.values)
            assert len(_symbols.by_name) == len(_symbols.values)
            for _value, _name in _symbols.values:
                assert -0x8000 <= _value <= 0x7FFF
                assert _name.isidentifier() and _name.upper() == _name
                assert _name not in {"NAN", "INF", "RESULT"}

"""Dialect-specific names for native FLW0 commands."""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class NativeCommand:
    command_id: int
    name: str
    stack_pop: int
    writes_result: bool | None = None


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
    NativeCommand(0x00D, "WAIT_FOR_TIMER_START", 0, writes_result=False),
    NativeCommand(0x00E, "WAIT_FOR_TIMER_LIMIT", 1, writes_result=False),
    NativeCommand(0x00F, "SCREEN_FADE_A", 2, writes_result=False),
    NativeCommand(0x010, "SCREEN_FADE_B", 2, writes_result=False),
    NativeCommand(0x012, "ADD_EFFECT_UNIT_TO_WORLD", 1, writes_result=False),
    NativeCommand(
        0x019, "ADD_FLAGGED_EFFECT_UNIT_TO_WORLD", 1, writes_result=False
    ),
    NativeCommand(0x043, "RESET_DRAW_EFFECTS", 0, writes_result=False),
    NativeCommand(0x046, "RETURN_TO_TITLE", 0, writes_result=False),
    NativeCommand(0x049, "WAIT_FOR_UNIT_MOTION", 1, writes_result=False),
    NativeCommand(
        0x04A, "ATTACH_WORLD_OBJECT_TO_SOURCE_VECTOR", 2, writes_result=False
    ),
    NativeCommand(0x04B, "SET_UNIT_VALUE", 2, writes_result=False),
    NativeCommand(0x060, "RESTORE_CAMERA_NODE_MODE", 0, writes_result=False),
    NativeCommand(0x061, "RELEASE_CURRENT_OBJECT", 0, writes_result=False),
    NativeCommand(0x066, "CALL_EVENT", 1, writes_result=False),
    NativeCommand(0x068, "READ_CURRENT_WORLD_OBJECT_ID", 0, writes_result=True),
    NativeCommand(0x069, "CLEAR_UNIT_LOW_FLAG", 1, writes_result=False),
    NativeCommand(0x06A, "SET_UNIT_LOW_FLAG", 1, writes_result=False),
    NativeCommand(0x071, "SET_MESSAGE_WINDOW_GEOMETRY", 3, writes_result=False),
    NativeCommand(0x073, "PREPARE_UNIT_MOTION_STATE", 5, writes_result=False),
    NativeCommand(0x094, "READ_SECONDARY_WORLD_ID_VALUE", 1, writes_result=True),
    NativeCommand(0x099, "RESET_FIELD_EFFECTS", 0, writes_result=False),
    NativeCommand(0x0A5, "CREATE_SCRIPT_TASK", 2, writes_result=True),
    NativeCommand(0x0A6, "DESTROY_REGISTERED_TASK", 1, writes_result=False),
    NativeCommand(0x0A7, "WAIT_FOR_TASK_REMOVAL", 1, writes_result=False),
    NativeCommand(0x0AA, "CREATE_POLYGON_MOVIE", 2, writes_result=True),
    NativeCommand(0x0C3, "SET_SOLAR_OVERLAY_MODE", 1, writes_result=False),
    NativeCommand(
        0x1E0, "QUEUE_WORLD_OBJECT_PENDING_VALUE", 2, writes_result=False
    ),
    NativeCommand(
        0x1E1, "CLEAR_WORLD_OBJECT_PENDING_VALUE", 1, writes_result=False
    ),
    NativeCommand(0x1E7, "CLEAR_PROCESS_CONTROL_FLAG", 0, writes_result=False),
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

DDS1 = CommandProfile("dds1", SHARED_DDS_COMMANDS, DDS1_EVENT_IDS)
DDS2 = CommandProfile("dds2", SHARED_DDS_COMMANDS, DDS2_EVENT_IDS)

PROFILES = {profile.name: profile for profile in (DDS1, DDS2)}


def get(name: str) -> CommandProfile:
    """Return a command profile by its source-format name."""

    return PROFILES[name]


for _profile in PROFILES.values():
    assert len(_profile.by_id) == len(_profile.commands)
    assert len(_profile.by_name) == len(_profile.commands)
    assert len(_profile.events_by_id) == len(_profile.event_ids)
    assert len(_profile.events_by_name) == len(_profile.event_ids)

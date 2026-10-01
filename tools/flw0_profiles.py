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

    @property
    def by_id(self) -> dict[int, NativeCommand]:
        return {command.command_id: command for command in self.commands}

    @property
    def by_name(self) -> dict[str, NativeCommand]:
        return {command.name: command for command in self.commands}


SHARED_DDS_COMMANDS = (
    NativeCommand(0x000, "MESSAGE_REQUEST_AND_POLL", 1, writes_result=False),
    NativeCommand(0x001, "ACTIVATE_MESSAGE_PANEL", 0, writes_result=False),
    NativeCommand(0x002, "FINISH_SCRIPT_MESSAGE_WINDOW", 0, writes_result=False),
    NativeCommand(0x008, "SET_MODEL_FLAG", 1, writes_result=False),
    NativeCommand(0x009, "CLEAR_MODEL_FLAG", 1, writes_result=False),
    NativeCommand(0x00E, "WAIT_FOR_TIMER_LIMIT", 1, writes_result=False),
    NativeCommand(0x00F, "SCREEN_FADE_A", 2, writes_result=False),
    NativeCommand(0x043, "RESET_DRAW_EFFECTS", 0, writes_result=False),
    NativeCommand(0x046, "RETURN_TO_TITLE", 0, writes_result=False),
    NativeCommand(0x066, "CALL_EVENT", 1, writes_result=False),
    NativeCommand(0x073, "PREPARE_UNIT_MOTION_STATE", 5, writes_result=False),
    NativeCommand(0x094, "READ_SECONDARY_WORLD_ID_VALUE", 1, writes_result=True),
    NativeCommand(0x099, "RESET_FIELD_EFFECTS", 0, writes_result=False),
    NativeCommand(0x0A7, "WAIT_FOR_TASK_REMOVAL", 1, writes_result=False),
    NativeCommand(0x0AA, "CREATE_POLYGON_MOVIE", 2, writes_result=True),
    NativeCommand(0x1E7, "CLEAR_PROCESS_CONTROL_FLAG", 0, writes_result=False),
)

DDS1 = CommandProfile("dds1", SHARED_DDS_COMMANDS)
DDS2 = CommandProfile("dds2", SHARED_DDS_COMMANDS)

PROFILES = {profile.name: profile for profile in (DDS1, DDS2)}


def get(name: str) -> CommandProfile:
    """Return a command profile by its source-format name."""

    return PROFILES[name]


for _profile in PROFILES.values():
    assert len(_profile.by_id) == len(_profile.commands)
    assert len(_profile.by_name) == len(_profile.commands)

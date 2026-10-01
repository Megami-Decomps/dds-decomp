"""Dialect-specific names for native FLW0 commands."""

from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class NativeCommand:
    command_id: int
    name: str
    stack_pop: int


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


DDS1 = CommandProfile(
    "dds1",
    (
        NativeCommand(0x043, "RESET_DRAW_EFFECTS", 0),
        NativeCommand(0x046, "RETURN_TO_TITLE", 0),
        NativeCommand(0x099, "RESET_FIELD_EFFECTS", 0),
        NativeCommand(0x0A7, "WAIT_FOR_TASK_REMOVAL", 1),
        NativeCommand(0x0AA, "CREATE_POLYGON_MOVIE", 2),
        NativeCommand(0x1E7, "CLEAR_PROCESS_CONTROL_FLAG", 0),
    ),
)

PROFILES = {DDS1.name: DDS1}


def get(name: str) -> CommandProfile:
    """Return a command profile by its source-format name."""

    return PROFILES[name]


for _profile in PROFILES.values():
    assert len(_profile.by_id) == len(_profile.commands)
    assert len(_profile.by_name) == len(_profile.commands)

#!/usr/bin/env python3
"""Small, shared primitives for comparing MIPS code after relocation.

Object files leave address fields and literal-pool offsets unresolved.  These
helpers describe the linked values that both ``check_unit.py`` and progress
report reconciliation compare against the retail executable.
"""


def sext16(value: int) -> int:
    return ((value & 0xFFFF) ^ 0x8000) - 0x8000


def instruction_shape_matches(mine: int, retail: int, relocation: str) -> bool:
    mask = 0xFC000000 if relocation == "R_MIPS_26" else 0xFFFF0000
    return mine & mask == retail & mask


def linked_jump_field(target: int, addend: int) -> int:
    return ((target >> 2) + addend) & 0x03FFFFFF


def linked_lo_field(target: int, addend: int) -> int:
    return (target + addend) & 0xFFFF


def linked_hi_field(target: int, addend: int) -> int:
    return ((target + addend + 0x8000) >> 16) & 0xFFFF


def linked_gprel_field(target: int, addend: int, gp: int) -> int:
    return (target + addend - gp) & 0xFFFF


def literal_pool_offset(word: int, object_gp: int) -> int:
    return object_gp + sext16(word)


def retail_literal_address(word: int, gp: int) -> int:
    return gp + sext16(word)

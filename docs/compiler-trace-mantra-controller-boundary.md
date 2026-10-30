# Mantra selection: an aggregate boundary, not a register barrier

DDS2 `mnuUpdateMantraSelectionState` (`0x0028A1D0`, 4,064 bytes) matches with ordinary C. The decisive repair is the reconstructed boundary between the menu work and its owner's draw-controller pointer. It needs no inline assembly, fixed-register local, volatile access, artificial dependency, or compiler-flag change.

## Preserved storage, corrected ownership

The recovered owner has these relevant locations:

| Object | Owner-relative offset | Extent |
| --- | ---: | ---: |
| `MantraMenuWork menu` | `0x240` | `0x9C0` |
| Menu draw flags | `0x794` | 4 bytes |
| `selectionController` | `0xC00` | 4 bytes |
| Whole `MnuStatusResource` | `0` | `0xC08` |

Previously, `selectionController` was modeled as the last member of the menu work, making that subobject `0x9C4` bytes. It is now a sibling member of the owner immediately after the `0x9C0` menu. No field address, pointer width, allocation size, or runtime memory access changes. Header assertions check the menu and owner sizes and the controller's `0xC00` offset.

This is a supported reconstruction, not proof of unique original struct nesting. The allocation clears the complete `0xC08` owner. Initialization and destruction address the controller through the owner separately from the menu work, whose equip state ends immediately before the controller. No independent menu-only copy or clear has been found that uniquely establishes the original subobject extent.

The source audit found controller accesses in four owners:

- `game/code_00289058.c`
- `game/code_0028E568.c`
- `game/code_0028FD30.c`
- `game/code_00286BA8.c`

They all access the field through a whole-owner pointer. Their expressions change from `object->menu.selectionController` to `object->selectionController`; there is no invented interior-pointer cast or compatibility union. No menu-work allocation, copy, array stride, or `sizeof` consumer was found that required the former extra four bytes. Other header includers remain covered by the linked-build gates.

## Remaining ordinary source reconstruction

The selection routine's retained source needed several local repairs:

- Traverse the five actual unit entries by index, deriving the current entry pointer in the loop.
- Preserve the real marked and kind values in `u16` locals before their consumers.
- Express the kind dispatch as `if`/`else if`, retaining the same two cases and common loop exit.
- Store the next mode before the wait count in case 9.
- Read the final default-selector coordinates directly for the icon-spawn arguments.

Earlier experiments introduced controller snapshots and a typed flags snapshot to approach the native schedule. They are **not** in the final source. Correcting the aggregate boundary made those experiments unnecessary.

## What the compiler evidence establishes

The paired compiler probes use the pinned EE GCC 2.96 compiler with SHA-256:

```
d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1
```

The ordinary candidate before the boundary repair differs in 27 instruction words. The repaired candidate matches the complete 4,064-byte extent. Normal and dump-enabled compilation agree on the target assembly. For each side, the complete owner `.text`, `.data`, and `.rodata` bytes also agree between normal and dump-enabled compilation, as do the resolved relocation records: 600 text and 69 rodata relocations. This qualifies the paired RTL observations; it is separate from comparison against retail.

Case 16 gives a small causal witness. In the initial RTL, both versions have the same instruction UIDs, offsets and numeric alias IDs:

| Operation | UID | Address | Alias ID |
| --- | ---: | --- | ---: |
| Next-mode byte store | 1778 | menu + `0x03` | 0 |
| Wait-count byte store | 1783 | menu + `0x02` | 0 |
| Flags-word store | 1796 | menu + `0x554` | 25 |
| Controller load (after argument folding) | 1801 | owner + `0xC00` | 49 |

In `17.sched`, the old controller load depends on all three stores: 1778, 1783 and 1796. With the sibling controller field, it depends only on the two byte stores, 1778 and 1783. The flags-word predecessor disappears.

Thus the repair is **not** a changed numeric alias set, changed offset, or instruction substitution. The corrected aggregate/source-expression ancestry allows the compiler to distinguish the flags word from the owner-controller field. Conservative byte-store dependencies remain. Removing the false flags dependency changes scheduler priorities and local lifetimes, producing the native next-mode/wait-count allocation and ordering. The controller load still follows the flags write in source, so the native call delay-slot load is retained.

This also explains why a seemingly equivalent early controller snapshot was insufficient: it removed the forward dependence but created a reverse anti-dependence, leaving the flags store in the call delay slot instead of the controller load. Adding a register constraint alone repaired register homes but not that schedule. Neither technique was needed after the structural repair.

## Validation scope

The four isolated full-owner checks report 37, 13, 47 and 29 matching functions respectively: **126 matches, zero differences**, including the newly emitted selection routine. The candidate also received independent semantic review of the ownership boundary, preserved storage and loop/control-flow changes.

The integrated change passes both games' retail checksum builds and both relocated developer builds, covering all ten header includers. The tooling suite passes **442 tests in 117 batches**, and the retained-C compilation gate passes **84 units, zero failures**. Original executables, compiler/runtime binaries, candidate objects and private probe archives are not part of this documentation.

# Snapshot continuation: an explained allocation correction

The 2026-10-09 continuation of DDS2 `func_001FA480` retains a second,
mechanistically improved candidate alongside `current-body.c`. It is still a
nonmatch. The new source is `snapshot-join-body.c`; `snapshot-join.patch` includes
its complete owning-unit prerequisites against game base
`9e573143fa0ea1e013781dc904138e7c68273990`.

## Source hypothesis

Opcodes 40 and 42 share instruction advancement after either rejecting the time
window, rejecting the linked-actor operation, or restoring the saved camera
parameters. Represent that actual shared continuation explicitly, keeping all
snapshot loads, stores, call arguments, and field types unchanged.

The negative timing guard preserves the existing short-circuit comparisons,
including NaN behavior. The linked-actor provider is called only for in-window
opcode 42. Its nonzero result clears `restoreAlternate` and advances; zero sets
it and continues. The snapshot selector still reads `instruction->kind` after
that call, and the parameter index remains freshly read at its original call.
There is exactly one advancement on each exit. No artificial storage, register,
volatile, alias, barrier, or ABI constraint was introduced.

## Compiler evidence

In the earlier source, first scheduling moved instruction advancement between
the scalar restoration loads. This extended the `parameter110` value's local
allocation lifetime to ten instructions, versus nine for the other six scalars.
It was consequently allocated last to `f6`, while the native restoration uses
`f0` for that value.

The explicit join changes this causal chain:

1. The jump pass contains one shared advancement block with three predecessors.
2. First scheduling keeps advancement separate from the restoration loads.
3. Local allocation gives all seven restored scalar values nine-instruction
   lifetimes. `parameter110` takes `f0`; distance and the other five float fields
   take `f1` through `f6`, agreeing with the native restoration.
4. Late jump optimization merges the advancement with an earlier common
   continuation. The embedded increment and its alignment nop disappear.

The post-snapshot-call interval now has the native coarse instruction sequence
and length: 245 words, previously 247. This is a local correction, not proof
that the entire interval is byte-exact or that remaining differences are only
registers. The pre-capture interval remains four words short.

One additional native-CFG hypothesis placed the advancement block before the
capture body. The compiler produced exactly the same target bytes and
function-relative relocations as the simpler trailing join. That result does
not justify retaining the additional labels, so the simpler source is retained.

## Measured boundary

All runs used diagnostic base `1bce3c8a32351951f632cd3bde7535dfea3aeccc`,
the pinned compiler and assembler, canonical flags and filename geometry,
and official Debian QEMU 10.0.13 through a private prefix-only adapter.
The adapter does not belong to this publication. This is local QEMU validation,
not a new hosted native-control observation.

- Earlier candidate: 5248 bytes, 570 differing words.
- Shared snapshot join: 5240 bytes, 573 differing words.
- Earlier placement of the same join: identical 5240-byte target and relocations.
- All three retain all 515 previously matching C functions, two existing data
  issues, and zero context or other issues.

The positional word metric worsens slightly because the layout changes. It must
not obscure the demonstrated lifetime/allocation correction, and it must not be
reported as a whole-function matching improvement. The earlier 570-difference
source remains available and the standard diagnostic still tests that source.
No full retail-image or runtime test was performed for these nonmatches. No
canonical game source or exact-match credit changes.

`./snapshot-join-results.json` contains the bounded check results. The complete
source and patch make the new candidate reproducible with the existing pinned
build and unit checker; private compiler dumps, objects, retail inputs and raw
assembly are deliberately excluded.

Credit Basalt Finch/team, Obsidian Heron and PiM for prior reconstruction and
compiler work, and the existing status-owner maintainers. KiB / redthing1's dot
continued the source-linked compiler analysis and shared-continuation experiment.

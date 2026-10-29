# DDS2 file prompt rendering

`fileShowPromptDialog` at `0x002CEEC0` is 2,660 bytes. The implementation
reconstructs the observed prompt renderer rather than claiming original source
spelling. It adds no inline assembly or register constraints.

## Observed behavior

The native renderer snapshots all twenty title pointers (80 bytes) before its
early exits. The retained draft instead declared sixteen entries and indexed the
global bank directly. The snapshot is subsequently indexed after the live reveal
callback. Separate prompt-mode tests preserve callback-visible changes; they
must not be replaced with an entry-time cached mode or a single switch.

Direct font calls replace the draft's one-off line helpers and static line
lists. Independent review checked all 41 native font-call tuples, including
coordinates, text symbols and colors, and checked the answer selection arms,
pulse calculation, texture and rectangle arguments, and final surface restore.
The forward `void(void)` declaration is unconditional because a preceding
matched function calls this renderer.

## Compiler-sensitive ordinary C

Two ordinary locals have explicitly non-overlapping value phases. This uses the
project's scratch-scalar rule, not a claim that these were the original names.
There are no additional program operations, dummy consumers, artificial
lifetimes, false types, or address escapes.

* On the first selection, `drawParameter` holds the real gradient RGB through
  the second rectangle's color arguments and cursor-color OR. Only afterward
  does it hold the lower answer Y consumed by the final font call. On the other
  selection it holds that Y throughout. RGB and both possible Y values fit the
  actual signed/unsigned argument domains.
* `renderValue` holds the selected width, 12 or 20, through both real rectangle
  calls. On the second selection only, after the last width consumer, it holds
  RGB `0x00AEC014` for the actual cursor-color OR. Every value fits signed32.
  Alpha remains loaded after the rectangle calls; no mutable read is moved.

Independent semantic review verified each reaching definition, last consumer,
conversion, callback ordering and both selection paths. Ordinary C with
separate locals was not exact; source statement order alone does not constrain
independent computations in this compiler. The shared local phases express
real last-use/overwrite dependencies without adding an asm constraint.

## Qualified compiler evidence

Pinned EE GCC 2.96 complete-owner ordinary and `-da` runs agreed in emitted
assembly and objects except for two temporary source-filename characters.
The following are deciding-pass evidence, not inference from matching scores:

1. Explicit normal/difficulty width-producing arms keep literal pseudo356
   separate from result86. `16.regmove` introduces copy2441; `20.greg` places
   the actual literal in a0 and width in s0. This avoids the incorrect width
   hoist across setup calls.
2. For the first phase reuse, the cursor OR UID1850 precedes Y definition1853
   with a real `REG_DEP_ANTI` in `17.sched`. Both value epochs share pseudo85
   in `20.greg`, reproducing the first-selection schedule.
3. In the second-phase causal pair, the baseline has 18 differing words.
   The exact candidate redefines width pseudo86 with actual RGB at UID2001;
   OR2010 consumes it. `17.sched` splits the constant into high2468/low2469.
   `20.greg` coalesces scaled width380 and both86 epochs into s0. `25.sched2`
   retains anti-dependencies from actual width reads1927/1982/1986: high2468
   moves only after the final outgoing argument read1986, before call1997;
   low2469 remains after that call. The source itself assigns RGB after the
   call, without a fabricated argument snapshot or save.

The evidence sources are `paired-qualification.json`, `PAIRED-TRACE-NOTES.md`,
`second-phase-qualification.json` and `SECOND-PHASE-TRACE.md` in the private
cloud evidence archive `compiler-evidence/prompt`. Independent review receipts
are `SEMANTIC_REVIEW.md` and `PHASE_REUSE_REVIEW.md`. No original binaries or
compiler artifacts belong in this repository.

Reviewed exact candidate SHA256:
`460dae24ca7834f2c33591de286f4608d8954b671b0ad5d8e9a639adf8162bd6`.
The unguarded landing form was separately checked against that qualified
candidate: assembly and object parity differ only in temporary filename bytes.

## Validation

The complete affected owner has 296 exact functions, with owned data and
relocation/context checks clean. Integrated validation passed both games' retail checksum and development
builds, all 442 tooling tests in 117 batches, and compilation of all 85
retained-C units (zero failures). The source and notes received independent
landing review. No original binaries or compiler artifacts are included.

## DDS1 counterpart

DDS1 `fileShowPromptDialog` at `0x0028FB98` is 2,268 bytes. It uses the same
ordinary direct-call reconstruction, but only prompt modes 1–9, fixed 48-pixel
bar halves, and the ordinary yes/no labels. It has no difficulty-mode width
selection. Its title bank was already declared with twenty entries; the change
restores the native consumed 80-byte stack snapshot rather than correcting its
bound. All 36 native font calls, including the two answer labels, are retained.
Separate mode tests and fresh reads after callbacks remain intact.

Unlike the DDS2 implementation, this implementation needs no cross-role scratch
reuse. `highlightRgb` always means RGB `0x00AEC014`. On the first-selection path
it is initialized after the second rectangle and consumed by the cursor-color
OR. On the second-selection path it is initialized before the second rectangle,
passed as its two actual RGB corners, then consumed by the cursor-color OR.
Its address never escapes. The alpha read remains after both rectangle calls;
no program read, call, store or arithmetic operation is added to steer allocation.

This same-role shared local is compiler-sensitive. The qualified complete-owner
EE GCC 2.96 normal/`-da` pairs are in the private cloud evidence archive
`compiler-evidence/dds1-prompt/qualification.json`. Each assembly/object pair is
byte-identical except two temporary source-filename characters. In the baseline
`16.regmove`, RGB pseudo328 is rematerialized at UID1676 after the second
rectangle and consumed by OR1678. In the exact source, the genuine shared RGB
pseudo90 is defined at UID1632 before the second rectangle call1670, and the
same value is consumed by OR1680 afterward. `20.greg` assigns pseudo90 to s0
(`90 in 16`); it must survive the actual call because the cursor consumes it.
This explains the native saved-register/frame difference without a register
binding, empty asm, fabricated dependency, or claim of original source spelling.

The cleaned landing form removes the unused draft-only rendering wrappers,
removes the target fallback guard, and makes the existing forward `void(void)`
declaration unconditional. The complete DDS1 owner has 253 exact functions,
with owned-data and relocation/context checks clean. Integrated validation passed
both games' retail checksum and development builds, all 442 tooling tests in
117 batches, and all 85 retained-C compilation checks (zero failures).
The cleaned landing form was separately qualified against the reviewed candidate;
full assembly and object artifacts differ only in temporary filename bytes.

# DDS2 mantra unit panel

`mnuDrawMantraUnitPanel` at `0x00274A70` is 792 bytes in
`src/dds2/game/code_0026DBF8.c`. It draws the unit's selection list and cost,
then selects the status sprite from a fresh post-draw status read. Opening
states 1/6 apply the position and opacity ramps; states 2/3/5 draw at rest.
The owner, embedded list, alpha, visibility and captured lamp value retain
their actual types and uses.

This is an evidence-backed mechanistic reconstruction. Exact instructions do
not recover original branch spelling, the historical meaning or source spelling
of the threshold constant, or copied-macro provenance. In particular, no copied
macro or sibling source is claimed for the exit-state comparison.

## Live opacity and the native residual

The opening path stages its existing sine result plus one in `opacity`, then
uses that real value in alpha conversion. This reproduces the native evaluation
schedule without adding an operation or unused local.

In states 3/5, retail loads the literal at `0x0043557C` into f0 at `0x00274CD0`
and executes `c.lt.s f1,f0` at `0x00274CD4`. The pool bits are `0x3E999999`
(0.2999999821186066). The pinned compiler emits these bits for `0.3f`.
The left operand is the lamp value loaded from `lamp+8` before alpha calculation,
not a value reloaded after a draw callback. No branch consumes the comparison
in retail. Its FPU condition/exception behavior is part of the native effect;
the reconstruction does not assume that a floating-point comparison is harmless.

The conditional repeats the same real list draw and its actual continuation,
`status = lamp->status`, in both arms. The predicate is the existing captured
value already consumed by alpha, initialized on every reaching path. Exactly
one list draw and one fresh post-call status read execute, with unchanged
arguments and ordering. The status read stays after the callback; it is not
hoisted across the draw. There is no added memory access, runtime call,
independent arithmetic, dummy state or unrelated dead computation.

Qualified ordinary whole-owner RTL establishes the mechanism in the final
source:

- `00.rtl` UID61 captures `lamp->value` in pseudo90; the live alpha calculation
  consumes it before the states 3/5 dispatch.
- Through `25.sched2`, UID391 loads the native pool value, UID388 is the empty
  tied operand, UID392 compares f1 against f0, and UID393 branches. Both real
  draw/status pairs, UIDs414/421 and445/452, still exist.
- `27.jump2` deletes branch393 and duplicate draw/status414/421. It preserves
  load391 and compare392, followed by the single draw/status445/452. The empty
  template emits nothing, so the final load and comparison occupy the exact
  native locations and order above.

The continuation is causal, not decorative. Repeating only the list-address
calculation lets `02.jump` delete the decision and `13.life` delete the compare;
that control is eight bytes short. The call-only control (`unit-panel-equal.c`)
leaves a terminal-call boundary that prevents the required late merge and grows
to 832 bytes. Duplicating the full status switch and optional sprite draw as
well (`unit-panel-equal-draw.c`) instead grows to 864 bytes. The
actual post-call status continuation allows the late merge and produces 792
bytes. This uses the documented equal-arm residual rule, not a general allowance
for invented dead source.

## Minimal allocation constraints

Ordinary source with the qualified equal draw/status arms is 792 bytes but has
38 differing words. The stock `19.lreg`/`20.greg` traces identify two allocation
dependencies, rather than a declaration-order or scheduling guess.

The unconstrained owner pseudo86 has 13 references across 95 instructions;
sprite pseudo87 has 25 across 286. Their global priorities rank the owner first
(approximately 4105 versus 3496 with the compiler's integer scaling), assigning
owner/sprite to s0/s1 instead of native s1/s0. The final source's scoped
`loadedLamp` is initialized directly from the real `item->data` acquisition and
immediately copied to the ordinary `lamp`. In `00.rtl`, UID19 loads the owner
into s1 and UID22 copies it to pseudo86. Its explicit register-bound lifetime
ends at this copy, before any call. It neither invents a value nor claims that
hardware requires s1. The retained copy preference yields native owner/sprite
allocation in `20.greg`; the ordinary owner remains properly call-preserved.

Without the value constraint, visibility pseudo89 has 3 references across 10
instructions while captured-value pseudo90 has 3 across 14, assigning
visibility/value to f1/f2. The localized empty `+f` operand at the real comparison
boundary preserves the captured value's bits and exposes its tied input/output
ownership there. It has no instruction, memory effect or clobber and does not
invent an uninitialized output. In final `19.lreg`, pseudo90 has 6 references
across 16 instructions and two sets; `20.greg` allocates value/visibility to
f1/f2, matching native. Its last use is compare392, before either draw callback.
There is no artificial use or lifetime extension beyond this actual consumer.

Only these two constraints remain. An explicit f1 local binding and an extra
comparison-value local were each removed when controls proved them unnecessary.
The final source constrains the already-existing `value` directly. Whole-function
register bindings and acquisition-time value constraints were not retained.

One-at-a-time controls on the frozen final source establish necessity:

| Source | Extent | Different text words | Result |
| --- | ---: | ---: | --- |
| Final | 792 | 0 / 198 | Complete owner 253 match, 0 differ |
| Remove only owner binding | 792 | 31 / 198 | Native s0/s1 allocation is lost |
| Remove only tied value constraint | 792 | 9 / 198 | Native f1/f2 allocation is lost |

Ordinary direct member access, reversed multiplication spelling, and separate
value acquisition in the visibility arms do not solve the unconstrained
allocation. A genuine cached list-subobject pointer adds a saved register and
grows the body, so it is not retained. No compiler flags, false qualifiers,
dummy uses, unused stores or opcode-emitting assembly were added.

## Qualification and review

The final private source SHA-256 is
`09c966d3fc0b95513db5ce0dcf240f789fd8e1a1c351d2fc23bb31cfbc936981`.
The actual compiler SHA-256 is
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
Both the ordinary compile and the RTL-dump compile use the canonical owner and
normal DDS2 flags. No debugger hook or compiler modification is involved.

For the final source and both remove-one controls, independent ordinary/probed
contracts agree in normalized complete assembly, all six allocated sections'
bytes and metadata, the full 1,653-symbol inventory, 2,132 resolved relocations,
two auxiliary nondebug sections, and ELF ABI. Only transient source-path labels
are normalized; instruction and relocation differences are not hidden.
Both compiler and assembler succeed. The final complete-owner check is
**253 match, 0 differ**, including owned data.

Independent semantic review approved the frozen source and checked its
initialized operands, bound lifetime, callback ordering, native pool bits,
final residual location, late branch/duplicate deletion, and minimal controls.
`CONSTRAINT` checker notes are prompts for that review, not approval by themselves.
The separate pre-existing nonempty assembly in
`mnuDrawMantraPanelOrbitingEffect` is unchanged by this work.

Local reproducibility artifacts are preserved under
`research/resume-20261109/party-status-worker/`:

- `final.c`, `final-complete-owner-check.log`, and
  `final-qualified-parity.json`;
- `final-without-owner-binding.c` and `final-without-value-constraint.c`,
  their focused check logs, ordinary objects/assembly, and qualified probes;
- `final-probe/` with its manifest, whole-owner input/assembly/object/RTL and
  extracted `functions/mnuDrawMantraUnitPanel/` pass history;
- `verify_parity.py` and each ordinary/probe ELF contract JSON;
- `review/` with independent semantic and parity receipts.

Integrated validation passed after independent approval: the complete owner is
253 match, 0 differ; both full retail executable SHA-1 checks pass; all 442
tooling tests in 117 batches pass; and all 81 retained-C units compile. The
source hash above is unchanged in the integrated owner. Private binaries,
compiler dumps and candidate controls are not part of the source commit.

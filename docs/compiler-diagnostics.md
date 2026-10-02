# Compiler diagnostics

EE GCC 2.96 exposes its optimization pipeline through RTL dumps. The project
wrapper can capture those dumps with the same canonical translation-unit path
used by a normal build. This is useful when two plausible C forms produce
different assembly: find the first pass where they differ, then investigate
that mechanism instead of treating the final instruction diff as one problem.

The diagnostics are read-only. They do not alter the compiler, select a
matching result, or make an unnatural source form acceptable.

## Capture a function

The output directory must normally be outside the checkout so multi-megabyte
RTL dumps cannot pollute the worktree:

```sh
python3 tools/ee_gcc_probe.py \
  src/dds1/game/code_001A04C0.c \
  --function sndCreateSystemEffect \
  --out-dir /tmp/snd-baseline
```

The probe calls `tools/cc.sh` with `-da`, preserves the generated assembly,
and writes:

- `rtl.NN.pass`: every dump emitted by the compiler;
- `candidate.s`: final assembly for the translation unit;
- `functions/NAME/`: the named function extracted from every available pass;
- `manifest.json`: source/compiler hashes, exact command, artifacts and
  compiler/assembler status.

The compiler stage can succeed even when the final assembler cannot resolve
the unit's `INCLUDE_ASM` paths in an isolated setup. In that case the RTL and
assembly are retained and the probe reports `assembler failed/skipped`. Use
`--strict-assemble` when an object file is required.

To compile a modified copy while preserving its real unit identity, use
`--as-unit`:

```sh
python3 tools/ee_gcc_probe.py /tmp/code_001A04C0.c \
  --as-unit src/dds1/game/code_001A04C0.c \
  --function sndCreateSystemEffect \
  --out-dir /tmp/snd-candidate
```

`--version dds2` selects the other game. `--cflag FLAG` may be repeated for a
specific evidence-driven flag test. `--replace OLD=NEW` performs a controlled
token rename in a temporary input and records the alias in the manifest; it
is intended for translation-unit context experiments, not bulk source search.

## Locate the first divergence

```sh
python3 tools/ee_gcc_compare.py \
  /tmp/snd-baseline /tmp/snd-candidate \
  --function sndCreateSystemEffect \
  --diff first
```

The comparison normalizes the wrapper's random same-length scratch directory,
raw lexical-block pointers, and an uninitialized numeric payload printed in
old GCC's special RTL notes. It also canonicalizes renames recorded by the
probe. Raw equality and normalized semantic equality remain distinct in the
report.

The command exits zero when no semantic divergence is found and one when it
finds one. `--json REPORT.json` writes the complete result. Omit `--function`
to compare the complete translation unit and list changed assembly functions.

## Reading the first changed pass

The first divergence narrows the next experiment:

| First stage | Investigate next |
|---|---|
| `00` RTL expansion | source types, expression/CFG shape, ABI lowering |
| `03` CSE | equivalent expressions, address materialization, CSE winner |
| `13` liveness | pseudo lifetimes, deaths and overlap |
| `14` combine | combine patterns and operand shape |
| `17` sched1 | dependencies and pre-reload scheduling |
| `19` local allocation | local eligibility, register class and preferences |
| `20` global allocation | allocno conflicts/order, reload and hard registers |
| `25` sched2 | post-reload ready-list order and dependencies |
| `28` machine reorg | MIPS-specific reorganization |
| `29` delay slots | delayed-branch eligibility and donor choice |
| assembly only | final shortening/emission or dump-only blind spot |

A difference at a late pass does not prove that pass caused it: always use the
earliest changed target-function dump. Conversely, a translation-unit dump
can change only because another function changed. Prefer `--function` when
diagnosing one match.

Run the focused tests with:

```sh
python3 tools/test_ee_gcc_diagnostics.py
```

## Explain a filled delay slot

When the first divergence is pass `29`, inspect the sequence that the delayed-
branch pass built:

```sh
python3 tools/ee_gcc_delay_slots.py /tmp/snd-candidate \
  --function sndCreateSystemEffect
```

The report names the call or jump UID and each donor UID placed in its slot.
It also identifies common load destinations and source locations. Use
`--json REPORT.json` when another tool needs the result.

EE GCC fills non-jump slots, including calls, before jump slots. For a call it
scans backward over ordinary instructions, checks resource conflicts, then
applies the MIPS instruction attributes. A label, jump, barrier, delay
sequence, or inline-assembly instruction stops that backward search. A normal
one-word argument load can be eligible: the architectural delay slot executes
before the callee reads the argument register.

There are two independent schedulers to keep in mind. Pass `29` constructs an
explicit RTL delay sequence, while the MIPS assembler may fill an unprotected
slot under `.set reorder`. Consequently, `-fno-delayed-branch` is a diagnostic
experiment, not evidence that final object code will contain a `nop`. Compare
the pass-28 and pass-29 dumps first, and confirm the assembled object before
attributing a residual to source semantics.

The exact `mnuRefreshPartyPanelSlots` twins provide a natural control. DDS1
passes `context + 0x7ec`; that signed-immediate `addiu` is a single donor, and
the explainer reports it in the initialization call slot. DDS2 passes
`context + 0xa928`; materializing the out-of-range positive offset takes two
instructions before the call, so pass `29` has no corresponding sequence and
the slot remains `nop`. Both C functions are exact. This is the useful source
question for a delay-slot residual: did a truthful type, expression, or layout
fact change donor availability before pass `29`? If pass-28 RTL already has an
ordinary eligible instruction immediately before the call, cosmetic spelling
changes are unlikely to suppress the move naturally.

## Assembler relaxation is deterministic

Sony's EE GAS 2.10 keeps a provisional short instruction immediately before
the corresponding long macro expansion. During relaxation it moves the long
form down over the short form. The original assembler calls `memcpy` for that
overlapping move, so modern libc implementations can corrupt the expansion in
a process-layout-dependent way. A typical symptom is a load or store where a
`lui` should begin the long form, even though repeated compiler runs emitted
identical `.s` files.

`tools/download_tools.py` patches the project's exact `ee-as` binary to use a
small forward-copy helper at that one call site. The patch is hash-locked and
fails closed on any other assembler build. It does not change the relaxation
decision: it only makes the selected instruction sequence copy correctly.

Run the focused patch tests with:

```sh
python3 tools/test_ee_as_relax.py
```

# Compiler diagnostics

EE GCC 2.96 exposes its optimization pipeline through RTL dumps. The project
wrapper can capture those dumps with the same canonical translation-unit path
used by a normal build. This is useful when two plausible C forms produce
different assembly: find the first pass where they differ, then investigate
that mechanism instead of treating the final instruction diff as one problem.
The [EE GCC decision atlas](compiler-decision-atlas.md) maps that first pass to
the evidence, source lever and stop rule worth testing.

The diagnostics are read-only. They do not alter the compiler, select a
matching result, or make an unnatural source form acceptable.

## Audit call contracts

`ee_gcc_contracts.py` finds fixed function declarations that disagree with a
definition in the same title's source tree. It honors file-local `static`
linkage and audits file-scope declarations. With no paths it scans `src/dds1`
and `src/dds2`; an explicit file or directory narrows the declarations under
review while the tool still indexes real definitions across that title. Use
`--symbol` for a single suspicious call boundary:

```sh
python3 tools/ee_gcc_contracts.py src/dds1/script/scrScriptProcess.c \
  --symbol sdfReadNamedResource
python3 tools/ee_gcc_contracts.py --version dds1 --symbol sdfReadNamedResource
python3 tools/ee_gcc_contracts.py --json /tmp/contracts.json
```

The scanner uses only source declarations and real definitions. It ignores
`INCLUDE_ASM`, leaves C89 `name()` declarations unspecified rather than
assuming zero parameters, and skips K&R or unsupported declarators. Uncertain
typedef identity is listed as skipped evidence instead of a proven mismatch. A
reported pointer/integer mismatch is representation-sensitive evidence for
review, not an automatic claim that the generated ABI differs. JSON output is
sorted for stable review. A whole-tree run is a broad review inventory; start
with the unit or callee involved in a near-match rather than treating every
historical declaration difference as cleanup work. Focused tests run with
`python3 tools/test_ee_gcc_contracts.py`. The command exits 1 when it reports
review findings and 0 when the selected scope has none.

For a return-type conflict, the report also identifies calls in the audited
source whose complete expression statement is just `callee(...)` or
`(void)callee(...)`. This proves that the source discards the result at that
site and makes the conflict more relevant to post-call register differences.
Assignments, returns, conditions, nested or indirect calls, declarations that
appear after the call, and ambiguous definitions are excluded from the
ignored-result classification. A same-named
parameter or local declarator anywhere in the caller also excludes the site;
this deliberately trades recall for avoiding function-pointer shadow false
positives. Known function-like macros are excluded, and files containing
conditional-compilation branches receive no call-site annotation. This remains
source evidence only: inspect the caller's emitted data flow before changing a
declaration.

When the same visible declaration also has calls whose result is used or whose
source form is ambiguous, the report lists them as `other_call_forms`. These
include assignments, returns, conditions and nested calls. They are a safety
warning, not proof of a particular use: review every listed site before even
experimenting with the declaration. One discarded result does not make a
callee globally safe to retype.

A matching C definition is a comparison anchor, not automatically the original
interface: old-C wrappers can preserve a return register under several source
return types. Confirm a finding against callers and the callee's machine-level
data flow before changing a declaration.

### Inventory old-style call boundaries

Use `--old-style` when a near-match may depend on a K&R definition or an
unspecified `name()` declaration:

```sh
python3 tools/ee_gcc_contracts.py --version dds2 --old-style \
  --symbol fileSetRecordSecondVector
```

This opt-in report lists recognized K&R formal declarations and direct call
sites that see an unspecified contract, including each observed argument count
and a conservative source-expression class. It distinguishes explicit casts,
address expressions, floating constants, default-width integer constants and
EE 64-bit `long`/`long long` constants; other expressions remain unresolved.
A fixed `name(void)` prototype is not an old-style boundary. Static entities
are kept file-local, so a same-spelled external function in another unit is a
separate row.

The inventory is evidence, not a warning or an automatic mismatch. An omitted
argument may leave an incoming register untouched, and an unspecified call can
preserve a wide literal's front-end mode, but source text alone proves neither
the live register contents nor the callee's historical interface. Confirm the
call in assembly and compare pass-00 RTL before changing a declaration. Files
with conditional-compilation branches contribute no call-site observations,
and unsupported declarators remain fail-closed. Old-style rows do not change
the command's exit status; fixed declaration/definition conflicts still do.
Recognized K&R definitions remain listed as skipped by the fixed-contract
comparison even when this separate inventory describes them.

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
  compiler/assembler status. When an object file exists, its relative path,
  size, raw SHA-256, and path-normalized SHA-256 are recorded separately. The
  path-normalized hash masks only the wrapper's random same-length scratch path,
  which old MIPS objects retain in metadata. A failed assembler can leave a
  partial object, so the wrapper return code remains authoritative.

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
For scheduler decision evidence, pass `--cflag=-fsched-verbose=5` (the equals
form keeps the leading dash unambiguous to the command-line parser).

## Recover source names for final locations

The exact compiler can also emit STABS source records without changing its
generated instructions. Capture a sidecar probe, then report the final home it
attributes to each available parameter or local:

```sh
python3 tools/ee_gcc_probe.py src/dds1/game/code_001A04C0.c \
  --function sndCreateSystemEffect --cflag=-gstabs \
  --out-dir /tmp/snd-locations
python3 tools/ee_gcc_source_locations.py /tmp/snd-locations \
  --function sndCreateSystemEffect --json /tmp/snd-locations.json
```

This turns otherwise anonymous hard-register and stack behavior into bounded
source evidence: for example, whether a named local finished in `$s0`, `$f2`,
or a virtual-frame stack slot. GPR debug numbers map directly to MIPS registers;
STABS numbers 38 through 69 map to `$f0` through `$f31`. Repeated records are
preserved rather than collapsed, and a valid function with no usable records
is reported successfully because optimized-away variables may be absent.

Treat the result as a compiler-reported final-home hint, not a location
timeline. It does not identify an RTL pseudo, prove a variable's lifetime, or
explain an allocation decision. Compare the relevant `.19.lreg`/`.20.greg`
data and emitted assembly independently before making a source change. The
project has verified byte-identical `.text` with and without `-gstabs` on
representative small and large units from both games; whole objects differ
because the debug sections are intentionally added.

Run the focused parser tests with:

```sh
python3 tools/test_ee_gcc_source_locations.py
```

## Locate the first divergence

```sh
python3 tools/ee_gcc_compare.py \
  /tmp/snd-baseline /tmp/snd-candidate \
  --function sndCreateSystemEffect \
  --diff first
```

The comparison normalizes the wrapper's random same-length scratch directory,
raw lexical-block pointers, and an uninitialized numeric payload printed in
old GCC's special RTL notes. Scheduler commentary is excluded from semantic
comparison, so `-fsched-verbose` reports and their truncated display names do
not masquerade as changed RTL. It also canonicalizes renames recorded by the
probe. Raw equality and normalized semantic equality remain distinct in the
report.

The command exits zero when no semantic divergence is found and one when it
finds one. `--json REPORT.json` writes the complete result. Omit `--function`
to compare the complete translation unit and list changed assembly functions.

## Get a bounded next action

After capturing a baseline and candidate, run the combined diagnosis:

```sh
python3 tools/ee_gcc_why.py \
  /tmp/snd-baseline /tmp/snd-candidate \
  --function sndCreateSystemEffect \
  --json /tmp/snd-why.json
```

It reports the first normalized divergence, a short diff, provenance warnings,
and one stage-specific next action. At passes 19/20 it includes both allocation
summaries; at pass 29 it includes both delay-slot sequences. Object hashes are
compared only for successful whole-translation-unit outputs. Function-scoped
diagnoses label them as whole-unit evidence instead of attributing an object
difference to the selected function. The raw hash remains useful provenance;
equality decisions use the path-normalized hash. Missing target-function
artifacts and failed compiler captures are reported as insufficient evidence,
never as evidence that a source change had no effect. A compiler-hash or
code-affecting option mismatch also fails closed; diagnostic-only
`-fsched-verbose` differences remain comparable after comment normalization.
The combined command
returns status 2 for insufficient evidence, 1 for a divergence, and 0 only for
a complete comparison with no divergence.

When both manifests name the same extracted function, `ee_gcc_why.py` infers
that scope if `--function` is omitted. It rejects one-sided or conflicting
manifest scopes instead of silently comparing unrelated translation-unit
artifacts. For older probe manifests, it also recovers extra compiler flags
from the recorded command when possible.

The result is deliberately bounded. “No codegen difference” means stop varying
that source idea. Allocation differences call for one truthful lifetime, type,
or expression hypothesis. Sched2 work stops when the desired order has no
truthful dependency, and delay-slot work starts with donor eligibility before
pass 29. The command is an evidence router, not a source permutation engine or
a claim that the first changed dump proves causation.

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

## Explain a contested scheduler choice

When the first difference is sched1 or sched2, recapture with
`--cflag=-fsched-verbose=5` and summarize the selected instruction and every
instruction that was ready at the same clock:

```sh
python3 tools/ee_gcc_schedules.py /tmp/snd-candidate \
  --function sndCreateSystemEffect --stage sched2 \
  --json /tmp/snd-schedule.json
```

The report attaches each ready UID to the priority, cost, incoming dependency
count, forward-dependent count, functional unit, and printed table order in
GCC's own scheduler report. It also prints a clearly labeled heuristic showing
how those visible fields narrow the choice. This is not the comparator order:
the old scheduler considers inputs the text table does not expose, including
the previous instruction's dependency class and, before reload, register
pressure. The report flags choices that the printed-field heuristic does not
explain, without asserting which unprinted input caused the selection.

For instructions in the same basic block, the pinned compiler's
`rank_for_schedule` compares these facts in order:

1. higher priority;
2. smaller register-pressure weight in sched1 only;
3. the relationship to the most recently scheduled instruction;
4. more forward dependents;
5. original RTL LUID, with the earlier instruction winning the final tie.

Sched2 omits the register-pressure comparison. Interblock scheduling adds
speculation and probability tests between the first two groups and the
dependency tests, so do not extend the same-block rule across basic blocks.
Printed cost, incoming dependency count, functional-unit name, and GPR versus
FPR register class are not independent rank keys. They can still matter
indirectly by changing priority, readiness, dependencies, or whether an
instruction can issue in that clock. The MIPS reorder hook is not a generic
GPR/FPR tie-break either: before reload, and only with more than two ready
instructions, it groups operations around the R5900 multiply/divide unit.

This gives a cheap stop rule for a local order mismatch. Find the clock before
the first wrong instruction and verify that both candidate UIDs are in the
ready list. Compare priority first, then inspect real dependencies and forward
dependents. For sched1, remember that register weight is not printed in the
table. If two independent same-block instructions tie through dependent count,
their original RTL order decides; a candidate that leaves those inputs
unchanged cannot reverse them. The remaining credible source levers are a
truthful dependency or critical-path change, or an earlier expansion change
that changes their LUIDs. Adding an artificial dependency or merely renaming
locals does not explain retail code.

This is enough to distinguish two useful outcomes. If the desired order has a
truthful dependency, lifetime, or register-use fact that changes the ready set
or a leading metric, test that one fact. If independent instructions remain
simultaneously ready and differ only in compiler tie-breaking, park the source
search rather than adding a fake dependency. `ee_gcc_why.py` includes these
paired summaries automatically when pass 17 or 25 is the first divergence; without
verbose records it reports the missing evidence and tells you to recapture.

## Explain global allocation

When the first target-function difference is pass `19` or `20`, summarize the
pass-20 allocation records before investigating later scheduling:

```sh
python3 tools/ee_gcc_allocations.py /tmp/snd-candidate \
  --function sndCreateSystemEffect \
  --json /tmp/snd-allocations.json
```

When a pass-19 dump is available, the report combines it with pass 20 and
separates facts that the raw dumps print apart:

- `global order` is the greedy attempt order for global allocnos that remained
  unassigned after local allocation;
- `refs/live/width/priority` shows the numeric inputs and result used to order
  global allocnos;
- `local` rows were assigned before global allocation;
- `global/N` rows are those candidates and show their final hard register,
  hard-register conflicts and preferences;
- `other` rows occur in the final disposition table but are not an allocno
  representative in the global candidate list. They can include locally
  assigned pseudos, additional pseudos grouped into a global allocno, and
  reload-created pseudos.

The compiler can retry global allocation. The JSON report preserves every
printed attempt and the text report calls out the retry count; the displayed
candidate table uses the last attempt. Spill and reload instruction UIDs are
reported as events, but the tool does not infer a spilled source variable from
an instruction UID.

The global numeric priority is
`floor_log2(refs) * refs * hard_register_width * 10000 / live_length`, truncated
to an integer. Higher numeric priority sorts first, with allocno number breaking
exact ties. Hard-register width is pass 20's parenthesized width, normally one;
it is not pass 19's byte size. The pass-19 `calls_crossed`, pointer and
user-variable fields are not part of this ordering formula. The report exposes
them in JSON along with set count and register class.

Pseudo numbers are compiler-internal identities, not source-variable names,
and can change between source forms. Identify a pseudo from its defining and
using RTL inside each probe rather than assuming that `r84` has the same
meaning in both. Then ask which truthful source type, lifetime or expression
fact could change its conflicts or remove it. If pass `19` or `20` already has
the wrong map, sched2 and delay-slot experiments are downstream: they can
reorder the selected hard-register dataflow, but cannot repair the earlier
allocation choice.

This is a classifier and an experiment guide, not a register-binding recipe.
An allocation-driven source idiom should be documented only after natural C
matches exactly and transfers to a function that was not used to derive it.

### Check whether the required saved roles are feasible

For a candidate with a broad saved-register residual, combine the existing
reports instead of starting with declaration permutations:

```sh
python3 tools/ee_gcc_probe.py SOURCE.c --function NAME --cflag=-gstabs \
  --out-dir /tmp/name-locations
python3 tools/ee_gcc_source_locations.py /tmp/name-locations --function NAME
python3 tools/ee_gcc_allocations.py /tmp/name-locations --function NAME
```

Compare the named final-home hints, pass-19/20 dispositions, emitted candidate
data flow, and the distinct roles visible in retail. If the candidate no longer
has enough live roles at the allocation boundary, only a real type, contract,
later-use, or ownership fact can make the missing role allocatable. Test one
such fact when the program supports it; otherwise park the search. See the
[saved-role feasibility gate](compiler-decision-atlas.md#saved-role-feasibility-gate)
for its evidence requirements and fail-closed limits.

## Explain a filled delay slot

When the first divergence is pass `29`, inspect the sequence that the delayed-
branch pass built:

```sh
python3 tools/ee_gcc_delay_slots.py /tmp/snd-candidate \
  --function sndCreateSystemEffect
```

The report names the call or jump UID and each donor UID placed in its slot.
It also identifies common load destinations and source locations. For jumps,
`annulled` means pass 29 selected a branch-likely form. Each slot then reports
whether its donor came from the branch target and whether it executes on the
taken path, the untaken path, or both. Use `--json REPORT.json` when another
tool needs the result.

These facts come directly from GCC's RTL flags: `/u` on the jump records an
annulled branch, while `/s` on a delay-slot instruction records a donor copied
from the branch target. An annulled target donor executes only when the branch
is taken; an annulled non-target donor executes only when it is not taken. An
ordinary branch reports `executes=always`. This distinction is often the whole
cause of a `bne` versus `bnel` residual: inspect which value the donor sets and
why the opposite path still needs it before changing source control flow.

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

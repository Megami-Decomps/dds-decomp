# Mantra selection transition: a copied bracket-phase residual

DDS2 `mnuDrawMantraSelectionTransitionNode` at `0x0026F1F0` matches all
896 native bytes. Its complete owner is `src/dds2/game/code_0026DBF8.c`.
The reconstruction adds no register constraint, inline assembly, qualifier,
helper, type change, or new runtime draw operation.

## The native residual and its live counterpart

The node draws its new entry during frames 0–6 and keeps the final list
entry visible afterward. The transition completes at frame 11; the arrow uses
frames 0–6 and the sine pulse frames 4–8. The entry frame is a snapshot; the
payload, kind, and next
pointer are read afresh at the observed callback boundaries. The final
increment reads the current node frame rather than incrementing the entry
snapshot.

After the final-entry draw, DDS2 still reads `node->next` at `0x0026F344`
and `0x0026F368`. Those two static sites supply the same dynamic read on
alternative paths. No surviving branch consumes this value. It also
computes `frame < 11` early at `0x0026F2EC` / `0x0026F36C`, retaining that
result for the final return. A source with only the live draws emits two
unmerged icon calls and does not reproduce this read or value ownership.

DDS1's related `itfUpdateDisplayListNode` at `0x00256540` has a live bracket
animation at precisely this point. It repeats the next-node test after the
entry callbacks (`0x002566C8` / `0x002566CC`). While `frame < 7`, it derives
the opening ratio `frame / 6.0f` (`0x002566DC`–`0x00256710`); otherwise,
while `frame < 11`, it derives `(17 - frame) / 10.0f`
(`0x00256808`–`0x00256850`). Those values feed DDS1's bracket geometry and
draws. DDS2 has no such bracket draws.

The DDS2 source retains only that same guarded pair of frame-ratio
assignments. Both values are dead before the following live phases. This
uses the documented **dead code that retail still shows** exception: the
native residual and the counterpart's live code establish the specific
copied shape. It does not permit unrelated dead calculations. It is a
compiler-supported reconstruction, not proof of the original source text
or the historical reason the bracket phase is absent.

## Qualified compiler cause

The capture uses the pinned ee-gcc 2.96 `cc1`, SHA-256
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`,
through `tools/ee_gcc_probe.py` with the complete canonical owner,
`--version dds2 --as-unit src/dds2/game/code_0026DBF8.c`,
`--function mnuDrawMantraSelectionTransitionNode`, `--strict-assemble`,
and `--cflag=-fsched-verbose=5`. The companion ordinary compilation uses
the normal `tools/cc.sh` wrapper without dump or scheduler diagnostic flags.

The actual passes show:

1. Pass `00.rtl` expands the fresh next-node read as uid225 and its guard as
   uid226. The opening ratio is uids236/238/239. The closing ratio is
   uids251/253/254/256/257, guarded by the `frame < 11` test uid245/246.
2. Pass `08.gcse` partially anticipates the real next-node read as pseudo193,
   including loads uid552/558/555 in the callback arms. It similarly gives
   the completion test shared pseudo192. This is why the icon call can have
   a real trailing instruction during later cross-jumping.
3. Both ratio chains remain through `11.cfg` and are absent in `13.life`.
   The next-node and frame guards remain. This keeps their lifetime and
   reference topology present during allocation. `20.greg` allocates the
   captured frame pseudo90 to `$18`, the completion pseudo192 to `$16`, and
   the next-node pseudo193 to `$2`. X, Y, depth and draw context receive
   `$19`, `$20`, `$21` and `$22` respectively.
4. The two copied guards uid226/232 still exist in `27.jump2` and `28.mach`.
   They disappear in `29.dbr` (reorg), leaving the native unused next-node
   loads and the early completion test. The guards are **not** deleted by
   jump2 in this qualified candidate.

Without the copied phase, frame has eight references across 157 instructions
in `19.lreg` and is assigned `$22`. With it, frame has eleven references
across 133 instructions and is assigned `$18`. This records the actual
lifetime/allocation difference; it is not a claim that a guessed allocator
sort rule alone proves the result.

## Live phase-ratio reuse

The existing float local is reused for each live phase, and for the sine
input ratio followed by the returned amplitude. Each live value has its own
initialized definition, disjoint lifetime and real consumers. Its address
does not escape, and it is not carried across `sdfSin`: the input is consumed
before the call and the return value supplies the next definition.

Pass `00.rtl` puts the sine-input division in the same real ratio pseudo92
(uid390), consumes it in the argument multiply uid395, then assigns the
sine return at uid400. The complete-unit allocator gives this pseudo `$f4`.
Collapsing the division into the call expression makes that intermediate an
anonymous call-argument temporary and changes five native floating-register
words. No different arithmetic operation or rounding sequence is introduced
by the explicit scalar assignment.

## Semantics and removal controls

Every path reaching a live ratio use overwrites any value from the copied
phase first. The added closing expression runs only for frames 7–10, so
`17 - frame` cannot overflow; both divisors are nonzero. The pinned compiler
removes the copied floating arithmetic entirely. Its emitted floating-point
operations and status effects therefore remain exactly retail. This is not
a claim of equivalence under an arbitrary compiler with strict floating-point
environment observation, where an unused finite division could be inexact.

The source was reduced before review:

- Removing the extra explicit completion local stays exact; it is absent.
- Removing the copied phase gives 892 bytes and 170 differing compared words.
- Keeping only an empty copied control skeleton does not restore the residual.
- Removing only its opening ratio gives 912 bytes and 130 differing compared
  words, overrunning the native extent by 16 bytes.
- Removing only its closing ratio gives 860 bytes and 167 differing compared
  words, leaving 36 native bytes uncovered.
- Keeping the phase but collapsing the live sine-input ratio into the call
  keeps the 896-byte extent and leaves exactly five words different.

The final source and each of the four nonempty removal controls have separate
ordinary/probed qualifications: whole-owner assembly (normalizing only the
compiler's scratch filename), all allocated bytes and section metadata,
complete symbols, resolved relocations, nondebug auxiliary sections and ELF
ABI. Both the compiler and assembler succeed for every capture.

The final complete-owner check is **254 match, 0 differ**. No new data symbol
or retained assembly data include needs removal for this target. Existing
constraint diagnostics belong to already matched neighboring functions.
Independent semantic review approved the frozen source SHA-256
`61318295a115541c0da45f2e36ddc7054835f7243c71074f07272a44357ac373`.
Integrated validation passes the complete owner, both full retail executable
SHA-1 checks, all 442 tooling tests in 117 batches, and compilation of all
80 remaining retained-C units. Private binaries, probes and controls are not
part of the source commit.

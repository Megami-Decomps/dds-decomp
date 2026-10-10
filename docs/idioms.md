# Confirmed source idioms (ee-gcc 2.96, -O2 -G8)

Each idiom was checked by compiling it and comparing with retail. When retail
shows one of these shapes, write the C given here. None of them is a trick:
all are ordinary source that the original compiler turns into exactly the
retail code.

For a rule not covered here, use the [investigation method](matching-investigation.md)
to recover its missing source contract. For compiler-sensitive near-matches,
start with the earliest changed pass in
the [decision atlas](compiler-decision-atlas.md), then use the relevant idiom:

| Retail/candidate difference | First evidence to inspect |
| --- | --- |
| Same FPU instructions, different motion around `abs.s` | [Helper versus builtin](#absolute-values-ffabsf-versus-builtin-fabsf), with the kernel exception |
| Indexed field constants or row-base copies differ | [Nested record and array addressing](#indexed-field-offsets-expose-nested-records-and-coordinate-arrays), pass 00 |
| A store/reload crosses another memory access | [Declared types](#declared-types-change-scheduling-dependencies-alias-sets-readonly) and [union access](#access-through-a-union-versus-its-actual-member-type) |
| Constants stay inside a retail loop | [Loop entry](#loops-that-loopc-never-optimises-a-branch-from-outside-into-the-test), [phony loops](#leading-exits-can-make-loopc-report-a-loop-as-phony), or [cost thresholds](#an-optimised-loop-can-still-retain-invariant-constants), pass 09 |
| Two independent instructions are exchanged | [Actual scheduler comparator](#use-the-actual-gcc-296-comparator), including sched1 register weight |
| A branch next to a call changes annulment | Same-TU callee visibility under Small shape rules; confirm reorg liveness and independent ownership evidence |
| Integer/FPU argument setup differs | [Mixed-class parameter order](#mixed-integer-and-floating-parameter-order), with all callers checked |
| A saved-register pair is exchanged | [Allocation](#saved-register-homes-what-global-allocation-actually-sorts-by) and [GCSE context](#code-that-changes-with-unrelated-text-context) |

An optimisation-disable probe can localise a cause; it does not justify a
production flag. A matching toy or altered declaration is likewise a
diagnostic, not proof of the game's source. Keep natural source and whole-unit
and complete-build verification as the acceptance gate.

## Switches and jump tables

gcc builds a jump table only when a switch has about 5 or more *distinct*
case labels. `case 2: case 3: case 4:` sharing one body counts as a single
label and gives an if-chain. Retail tables whose entries repeat
(`L1, L2, L3, L3, L3`) come from separate cases with identical bodies, which
cross-jumping merged afterwards:

```c
switch (o->kind) {           /* 5-entry table, entries 2..4 identical */
case 0: o->sub->a = v; return;
case 1: o->sub->b = v; return;
case 2: o->sub->c = v; return;
case 3: o->sub->c = v; return;
case 4: o->sub->c = v; return;
}
```

The table goes into the unit's `.rodata`, and check_unit verifies every entry.
Retail example: `func_0018CC98`.

- A `switch` on `u32` with cases 0, 1 and 2 makes a linear `beq` chain;
  changing the selector to `s32` introduces an `slt` split (`func_0027DCE8`).
- In a switch with calls, `case 1: f(); return; case 2: g(); break;`
  keeps the first call a sibling `j`, but gives the last a `jal` and shared
  epilogue (`func_00282850`; also `func_002BE448`).
- Write the same call separately in switch cases ending in `break` when
  retail has one cross-jumped `jal`; computing one argument and calling
  once instead changes the whole dispatch (`func_00285F00`).
- In a sparse switch, an explicit `default: return;` after separately
  written cases can turn duplicate tail calls into one `jal` plus a
  shared epilogue (`func_0025FCD8`).
- A switch that only picks a value (retail arms are `b <common>` with
  `addiu $r, $0, X` in the delay slot, converging on one return) is written
  with an accumulator: `s32 v = 1; switch (k) { case 0: v = 3; break; ... }
  return v;`. A `return X;` per arm gives `j` instead of `b`
  (`func_002C2F40`, `func_0030AAB0`).

## `slt; sltiu 1` vs `slt; xori 1`

A negated comparison written as an expression (`return !(x < 2);`,
`return x >= 2;`, `x < 2 ? 0 : 1`) gives `slti; xori $2,$2,1`. Retail's
`slti; sltiu $2,$2,1` comes from early returns:

```c
if (x < 2) return 0;
return 1;
```

(`if/else` with two returns gives the same code.) Example: `func_0014D0D0`.

The same holds for a sign test: `return x < 0;` compiles to `srl $2,$2,31`,
while retail's `slti $2,$2,0` comes from `if (x < 0) { return 1; } return 0;`
(`func_0010D8E0`, `func_0010D6B8`).

## Square roots: `sqrt.s`

Retail always has a bare `sqrt.s` (33 in 11 DDS1 units) and never the
errno-checking `sqrtf` call. gcc's own sqrt.s (with `-fno-math-errno`) comes
after two hazard nops that retail doesn't have. Use `fsqrtf()` from
`include/fpu.h`, an asm helper wrapped in `.set noreorder` that gives retail's
`sqrt.s; jr $31; nop`:

```c
#include "fpu.h"
return fsqrtf(dx * dx + dy * dy + dz * dz);   /* func_00122BB0 */
```

## Absolute values: `ffabsf` versus builtin `fabsf`

An `abs.s` instruction alone does not distinguish the builtin from an asm
helper. Their scheduling differs: the builtin is an ordinary FPU operation,
while `ffabsf` in `include/fpu.h` has the same asm boundary as `fsqrtf`.
Independent constant loads and argument moves can cross the builtin but stay
after the helper in the matching game-code cases.

Use the existing `ffabsf` helper when the surrounding instruction sequence
supports it. DDS1 `func_00152560` and DDS2 `func_0015A150` are the paired
reference; DDS1 `func_001726E8` is a larger transfer. This is a module
convention, not a global replacement rule: `kwlnAdvanceShakeOffsets` in the
kernel uses builtin `fabsf` and changes code under the helper. Check the whole
unit and preserve the kernel exception. Do not introduce another asm spelling
or replace an unrelated arithmetic expression just to constrain scheduling.

## Scalar outputs and address reuse

The linked-defeat cameras (`func_001E4180` in DDS1 and `func_001F1B00` in
DDS2) combine the SDK `ffabsf` boundary with two independent `f32` extent
outputs. The bounds providers accept independently nullable output pointers;
the three VU vectors remain in a separate, genuinely 16-byte-aligned record.
Both routines match with this source organization and complete 16-byte
pointer-form COP2 memory operands.

Grouping the scalar outputs into the vector record changes more than the
stack layout. Their addresses become pseudo-register expressions before
GCSE, and PRE retains the depth pointer across the two provider branches and
a later query. Independent addressable floats instead enter RTL as
`addressof` nodes. Their expansion assigns the stack addresses directly to
the hard argument registers; the depth address is absent from the PRE
expression table. The edge-vector address remains eligible and is reused,
as retail requires. The dumps establish this representation difference;
the exact internal hard-register eligibility predicate is not established.

The SDK helper alone does not close the grouped-record version, and
independent scalar outputs with builtin `fabsf` also remain unmatched. Check interacting
source contracts before treating either negative as conclusive. Disabling
GCSE also loses the required vector-address reuse and does not match. This
case supports ordinary independent output locals, not invented storage,
barriers, declaration-order search, or a global optimizer flag change.

## Code that changes with unrelated text (CONTEXT)

ee-gcc 2.96's CSE hashes the addresses of symbol-name strings, so the rest of
the preprocessed unit (other functions, asm include lines, declarations) can
tip a function between two equivalent instruction selections. For example,
`lw $2,sym($2)` becomes `lui/addiu/addu`. check_unit compiles every unit a
second time exactly as the build does, and reports `CONTEXT` when a C
function's code differs there. Such a function doesn't match in the real
build. Treat it like `DIFF`: try another natural formulation or park it.
Editing a unit can make an existing function CONTEXT, so always check the
whole unit.

GCSE has a separate spelling-sensitive effect. Its expression table hashes a
`SYMBOL_REF` by the symbol's name; table traversal can change PRE pseudo
creation order and a later allocation tie. This is distinct from CSE's
address-sensitive context effect. The paired `func_001547D8` /
`func_0015C3C8` became exact with the recovered `effDefaultRandomState` name;
the existing sibling generators also remained exact.

Treat this as a reason to inspect the first changed GCSE expression, not to
search identifier spellings. A rename needs independent semantic evidence,
the normal `names.py` provenance, and checks of every user in both games.
Fixing one caller while breaking its donor or sibling is not a valid result.
Names alone cannot recover an unsupported lifetime or allocation role.

## Narrow status queries after dispatch

Use a status getter's recovered return type even when the surrounding
dispatcher has a wider result. A getter used only as a guard need not be
assigned to the dispatcher's result and immediately cleared again.
Return a nonzero dispatch result early, test the getter directly in the
fallback condition, and return zero after the fallback.

This shape preserves the retail code with the true `s32`
`evtGetMessageWindowControlState` declaration in DDS1
`mnuRunPanelWithIdleFallback` (0x00264610) and DDS2
`evtBContinueDispatchOrRestoreTable` (0x0026AFE0). Both complete units
check clean. Their former comma-assignment guards changed code when the
incorrect wide getter declaration was corrected.

The shared fade query `kwlnFadeIsActive` likewise has an `s32` logical-status
contract, not a byte return inferred merely from its boolean range. Both
providers return the integer comparison of the direction bits with zero;
all twelve consumer declarations now agree. The two provider units each
check at 56 match, 0 differ, and narrowing the sole wide consumer declaration
in DDS2 `code_00265AD8` leaves that unit at 21 match, 0 differ.

The kernel task interface also distinguishes real `KwlnTask *` values from
the native 32-bit words retained by game-side tracking slots. Task lookup
takes `const char *`; the registration query returns `s32`, and user data
is transported as `u32`. Decode tracked words at that SDK boundary and use
real pointer callback parameters, not `s64` pointer/status temporaries.
In DDS1 `code_001A1960`, six named-task registration guards match with
nested positive tests for tracked-task presence, registration, and named
task presence. An early registration-failure return differs at the
branch-likely/delay pair even when the SDK result is correctly typed.
The separate `btlGetTaskState6` forwarding query is not solved by this
guard shape: three genuine 32-bit-result forms exceed its retail 0x40-byte
body. It stays ASM with an honest parked candidate; a fabricated wide
return is not an acceptable way to suppress the compiler's tail handling.


## Promoted channel arguments and byte storage

Byte members do not establish a byte-parameter API. `kwlnFadeSetColor` takes
four `s32` channels while the live RGBA record still contains four `s8`
members. The event-viewer caller passes RGB from `lbu` and computes the full
`128 - fade` alpha; narrowing at the caller would change that observed ABI.
The assignments perform the byte-storage conversion instead. Both 40-byte
setter bodies and their complete 56-function provider units remain exact
with the promoted formals.

## `jal` tails through a shared inline helper

A call returned through a `static inline` helper is not turned into a
sibling call: gcc 2.96 returns the inlined value through a temporary and
keeps `jal; epilogue`. That is the natural source when many callers share an
argument pattern. For example, 123 DDS2 functions call `func_002C4038` as
`(ctx + 8, ctx + 0x54, mode, cb)`:

```c
static inline s64 mnuRequest(s32 ctx, u64 mode, s32 cb) {
    return func_002C4038(ctx + 8, (s32 *)(ctx + 0x54), mode, cb);
}
s64 f(s32 cb) { return mnuRequest(func_00101958(), 2, cb); }
```

(found by W13_D2 in DDS2 `code_002B0278`). Use this only when a helper like
that genuinely recurs. Wrapping a one-off call in an inline just to get
`jal` is a codegen lever, not source.

## Narrow return types and `jal` tails

A function whose recovered return type is narrow can return a narrow callee's
result with `jal; epilogue` instead of a sibling `j`. DDS1 `func_0015AD48`
(`u16` over `parGetRestartFlag`) and `func_0015ADB0` (`u8` over `parObjGetMode`)
both match this way. The same call with an `s32` return, with or without a
named local, tail-calls. Use the recovered type from the retail body:
`parObjGetMode` returns the `u8` fields `mode150`/`mode151`, or 0. This is not a
wider or invented return, which the `btlGetTaskState6` note above rules out.

## Lexical nested helpers and the static chain

GNU C nested functions use `$v0` as a hidden static-chain register on EE. A
retail helper is strong evidence for lexical nesting only when both sides of
that interface are present:

- immediately before the direct call, the owner copies its frame address into
  `$v0`; and
- the helper saves incoming `$v0` to its own frame independently of its normal
  `$a0`--`$a3` arguments.

Write that helper inside its sole owner. Even when the helper does not read a
captured variable, gcc gives the incoming static chain a stack home. That
prevents the helper's final call from becoming a sibling `j` and naturally
produces its framed `jal` form. It also proves that the helper and owner belong
to the same source unit. The paired `sdfMovie` copy helpers and owners have this
shape; keeping each pair together reproduces both the seven-instruction helper
and the owner's late ordinary branch.

Do not make a helper nested merely to suppress a tail call. Require the
two-sided `$v0` evidence and a single lexical owner. Address-taken nested
functions use trampolines and are a different case.

## Small shape rules (confirmed while matching)

- `a && b && c` bit tests fold into one masked compare. Nested `if`s keep
  separate `andi`s.
- Chained `*p &= ~2; *p &= ~4;` are not folded into one mask.
- A ternary of two constants can pick the wrong `movn`/`movz` operands;
  `x = default; if (c) x = other;` gives retail's split.
- For two constant arms, their order controls `movz` versus `movn`:
  `x = r != 0 ? 0xA : 0x80` gives `li 0x80; li 0xA; movz`, unlike the
  equivalent `r == 0` ternary (`func_0011E208`).
- `(w + 8) * 16` folds to `w * 16 + 128` unless `w + 8` is its own local.
- A field that retail tests with `srl; andi` and rewrites with `and`/`ori`
  on the whole word is a bitfield. Declare the struct with bitfields; raw
  shifts and masks CSE differently. A test against an unshifted mask
  (`andi $2,$4,0x1FE0000`) is plain mask code, not a bitfield.
- `s16` counters (`x -= 1`) give `addiu -1; sll 16`. `u16` ones give
  `li 0xFFFF; addu`.
- Switch tables whose low cases do nothing still list them
  (`case 0: ... case 5: break;`). Without them gcc builds a compare tree.
- Stores through `void **`-typed pointers let gcc schedule `int` loads above
  them (type-based aliasing), and `s32` stores do not. Pick the pointer type
  the data really has.
- Raw casts like `*(s32 *)(work + 0x4D8) = v` may alias a scalar global, so
  gcc reloads the global afterwards. A store through a struct field
  (`work->resA = v`) doesn't alias, and retail has no reload.
- A loop that retail compiles with the epilogue `ld` in its delay slots calls
  a function *defined in C earlier in the same unit*. While that callee is
  still INCLUDE_ASM, gcc can't see it and never fills the slots. Match the
  callee first (`sndClearList`, `func_00204CC8`).
- Callers that pass fewer arguments than the callee reads want the callee
  unprototyped (K&R). A missing unused parameter is real too: m2c drops
  parameters nothing reads, so check the callers.
- Registers in `$7`–`$10` that stay unchanged until a final call are often
  *parameters* passed straight through, not locals. Declaring them as `s32`
  parameters gives them their home registers (`func_00242780`). As
  `s16`/`u16` parameters they get masked on entry.
- `bltzl` with the fallback constant in the delay slot is
  `if (a + b < 0) x = K; else x += b;`: the sum sits in the condition and
  both arms store. A temporary or a ternary gives `movn`.
- A clamp that retail keeps as a branch (`beql; li MAX-1` or `bnez; daddu`
  with the value in the slot) is a ternary whose every arm reads the field
  itself, after a separate increment, the way a `CLAMP(x, lo, hi)` macro
  expands: `c->frame++; c->frame = c->frame <= 0 ? 0 : c->frame >= 0x7FFF ?
  0x7FFE : c->frame;`. jump_optimize won't turn an arm with a memory read into
  `movn`/`movz` (`may_trap_p`); CSE later folds the reads into the register
  holding the sum. The field must be signed (`s16`/`s8`) so the compare is
  `sll; sra`. Any local temporary if-converts to `li; movn`
  (`func_001F4D70`, `func_001FFC70`, `func_00210DC8`; DDS1 `func_001E60C0`,
  `func_001EEC20`, `func_001FEB90` have the same shape). `abs` written as
  `l->arg < 0 ? -l->arg : l->arg` gives `bltzl; negu` for the same reason.
- `xori t,x,K; movz` with K != 0 is plain `v = d; if (x == K) v = c;`.
  With K = 0 it is an `xor reg,reg` compare that only became 0 after CSE;
  the source shape is still unknown (`func_001E99C0`, DDS1
  `btlLowestSetPairIndex`).
- `li K; mult` for a small K that shifts could do: the multiplier is a named
  local, e.g. `s32 cw = 0xC0, ch = 0x60;`, used as `cols * cw + ch`. Literals
  and `const` locals fold to `sll/addu` (`func_00103790`; DDS1 twin
  `func_001038A0`).
- `beqz; sltiu; bnel` (store in the slot), `b; li 1`, `daddu $2,$0,$0` is an
  `||` guard ahead of an unsigned counter increment:
  `if (f() == 0 || l->count >= 0x1E) return 1; l->count++; return 0;`
  (`func_00210148`, `func_00210850`).
- A load in the first `bnez` delay slot after a leading call is a local read
  right after the call (`actor = link->actor;`); without it reorg leaves a
  `nop` (`func_00210258`, `func_002103F8`).
- A hoisted `daddu $2,$0,$0` with a load in the branch slot is
  `s32 result = 0;` at the top, returned on the zero paths; `return 0;` puts
  `move $2,$0` in the slot (`func_0020EBD0`).
- Loops over parallel per-slot arrays (`set->handle[i]`, `set->active[i]`)
  keep retail's count-up loop. A pointer walk becomes a count-down loop.
- To keep two `slti` where C would fold a range test into `sltiu`, nest the
  tests: `if (m < 0xE) { if (m >= 0xC) ... }`.
- Adjacent independent stores come out roughly reversed. Retail's order
  usually comes from writing the fields in ascending offset order.
- One shared `jal` with its argument chosen by a branch (`bne; daddu $4,$0`
  in the slot, then `li $4,1`) instead of `xori`/`sltiu` comes from a
  `return` in both arms, which cross-jumping merges:
  `if (g() == 1) { f(1); return 1; } f(0); return 1;` (`fldSetFlagFromWorld1`).
- A loop that retail closes with `bne` against a hoisted `li $rN,K` is written
  with `!=` (`i != 2`); a `<` test becomes a count-down loop.
- A `return;` inside a nested block disables the sibcall `j`; write the
  condition as one `if (a != -1 || b < 0x32) { ... }` or an else-if chain.
- `f32 pos[4] = {0, 0, 0, 1.0f};` is the source of retail's `memset` plus a
  single `1.0f` store.
- Declare every prototype before its first caller. A later `extern void`
  leaves earlier calls implicitly `int` and moves values between `$2`/`$3`.
  Confirmed reserve-panel examples: DDS1 `func_001B7F50` and DDS2
  `func_001C3168` become exact with their presentation/text helpers declared
  `void` before use. Recover that contract from the provider and its callers;
  a preferred register alone does not establish a return type.
  Keep new externs in the declaration block at the top of the unit; externs
  added mid-file can flip other functions to CONTEXT.
- **Same-TU callee visibility can change branch annulment.** Test this only
  when a natural near-match has an ordinary-versus-likely branch residual,
  the fall-through starts with a call, and controlled visible/opaque probes
  keep the same pass-28 branch topology and branch-target donor. Without
  `-fexceptions`,
  `rest_of_compilation` marks a function already compiled in the same file
  `TREE_NOTHROW`, and later calls to it get a `REG_EH_REGION 0` note. An opaque
  external call may throw.

  In reorg (`fill_slots_from_thread`), a branch-target donor stays unannulled
  only if it sets nothing in `opposite_needed`, the registers
  `mark_target_live_regs` reports live on the fall-through. That scan runs
  through a known nothrow call but stops at a possibly throwing external call.
  A later call clobber can therefore make the visible-callee form use plain
  `bne`/`beq`, while the opaque form needs `bnel`/`beql` so the target donor
  executes only on the taken path. This mechanism does not explain an
  arbitrary branch difference; use `ee_gcc_why.py` and
  `ee_gcc_delay_slots.py` to confirm the earliest pass and donor.

  The cheapest falsifier does not touch the real source. Copy the unit to a
  scratch file, put a behavior-neutral stub with the observed call contract
  before the caller, then run:

  ```sh
  python3 tools/check_unit.py <unit> --source <copy> --func <caller>
  ```

  Replacing an existing earlier definition with an external declaration tests
  the reverse direction. A predicted branch flip is a controlled compiler
  result, **not proof of historical file ownership**. Before moving a real
  boundary, also require independent provenance such as paired text/rodata
  adjacency, a debug-source identity, or comparably strong source-lineage
  evidence. Then move the authentic definition, not a stub, and validate every
  affected unit and both complete builds. The paired `fldFileResolver` objects
  are the reference case: the real creator and its callback string immediately
  precede the resolver in both games, and restoring that ownership changes the
  sole `bnezl` residual to retail `bnez` without changing either creator. The
  second case is `kernel/dds3AdminiProcess` in both games. The process
  dispatcher `func_00102CD8` / `func_00102BC8` needs the mode-request provider
  visible. The provider immediately precedes the first Nocturne-mapped process
  function (which calls it), and the `"Admini"` task-name sdata goes with it.
  So the unit now starts at `dds3GetAdminTaskWork`. Third case:
  `effect/effPCPThunder` absorbed the following `game/code_00167178` /
  `code_0016EDD0`. `effThunderChainGroupCreate` needs `effThunderFragCreate`
  visible. Its release/update siblings and the Thunder types continue up to
  the mapped `effPCPFlash`, and the merged unit reproduces `.lit4` exactly.

  Stop if pass 28 already differs, the donor or opposite-path liveness differs,
  the controlled visibility test does not change the predicted annul bit, or
  independent ownership evidence is absent. Never ship a stub, block-scope
  declaration, attribute, dummy use, or fake dependency as the fix. A ternary
  `t = c ? a : b;` can separately add a jump to a join label and stop CSE's
  skip-block path (see "Float constants").
- A `"memory"` clobber on a COP2 save/restore asm stops gcc reusing `$4`
  across it; retail's code has none.
- `(n * 6 + 1) << 16` gives retail's `lui $1; addu` large-immediate add.
- A single-bit test used as a value, `x = (f & 2) != 0;`, compiles to
  `srl; andi 1`. Retail's `andi $2,f,2; sltu $3,$0,$2` (a 0/1 flag through
  `$3`) comes from `x = 0; if (f & 2) x = 1;` (and `else if (f & 2) x = 1;`).
  `func_002C5D20` (DDS2 `code_002BE628`).
- Loop initializers fill a branch delay slot in source order: when retail's
  loop-back branch carries `daddu $j,$0,$0` and the loop head starts with the
  pointer setup, write `for (j = 0, p = base; j < 5; j++, p++)`
  (`func_002C5DE0`). A local `u16 t[5][2] = {...}` is the source of a rodata
  table that retail copies to the stack with `ldl/ldr`; remove its
  `INCLUDE_RODATA` when converting the function.
- Nested-if returns and flat early returns give different block order and
  delay slots: `if (a != 0) { if (g() != 0) return 0; ...; return 2; } return 1;`
  matched where `if (g() == 0) {...; return 2;} return 0;` produced `bnel`
  instead of `bne` (`func_002C5EA8`).
- Use early `return 0` rather than a shared `result` variable when retail
  puts `daddu $2,$0,$0` in the branch delay slot; the variable adds a move
  or changes the branch to `beql` (`func_0030D3C0`, `func_00312320`).
- Separate `return 1` exits can keep a `li $2,1` at each branch site;
  one merged return changes the delay slots (`func_002198D8`).
- A small `if/else` whose else is `dir = 1` gives `bgezl; li` when written
  `if (x < 0) { dir = -1; x = -x; } else { dir = 1; }`; testing `x >= 0`
  first gives plain `bgez` (`func_00279CC0`).
- Keeping `s64 t = f(); if (t) { ...; return g(t); } return t;` preserves
  the post-call `move $3,$2`; overwriting `t` with `g(t)` does not
  (`func_001B7AE0`).
- Copy a `u8` state into an `s32` local before testing
  `state == 1 || state == 2` to get `addiu -1; sltiu 2` without
  `andi 0xFF` (`func_001C2A50`).
- `head = head->prev; list->head = head;` reuses the argument's
  home register; a separate accumulator takes another register
  (`func_0027BB80`).
- Chained stores `timer = timerMax = value` reverse the two store
  instructions compared with `timer = value; timerMax = timer`
  (`func_002858A0`).
- Read a field into a local before an unrelated store to let the store
  occupy the following `jal` delay slot (`func_001E64B0`).
- `s32 bank = kind >= 2; return m->banks[bank].x;` retains the indexed
  shift/add, whereas `banks[kind >= 2]` becomes `li/movn`
  (`func_00168448`).
- A word set by `header = h | (w << 60); header |= f << 47; header |= K;`
  loads `K` late with `dli`; one combined OR chain hoists it
  (`func_0033A2D8`).
- A union view `union { u32 word; u16 half; }` preserves an `lhu`
  reload after writing `word`; a plain halfword or bitfield lets gcc
  fold the read away (`func_0025E7B8`).
- `value += 0.005f; value -= whole;` on an `f32` parameter reuses
  `$f12`, where separate temporaries do not (`func_0011DE20`).
- `s64 result = f(); if (result != 0) { ...; return 1; } return result;`
  passes `$2` straight through on the zero path (`func_002B3090`).
- Initialize a scalar counter after the early-return checks, not at
  its declaration, when retail keeps it in a caller-saved register
  (`func_001B2540`).
- `if (f() == 0) { ...; return N; } return 0;` gives an annulled
  `bnel` with zero in its slot; flipping the test to an early
  `return 0` gives plain `bne` (`func_001D0710`).
- `limit = value = word & 0xFF` keeps the low-byte result in the
  register retail uses for both the clamp and its store; splitting
  the assignments changes the pseudo (`func_001B0938`).
- Re-read a just-stored call result through `*(T *)(work + off)` when
  retail passes the call's `$2` straight through: gcc eliminates the
  load via CSE (`func_002A94D0`).
- `fade->state = kind;` followed by a test of `fade->state` against
  6 and 1 retains the copied value used in retail's comparison
  (`func_002710D8`).
- Declaring non-GP data as `extern s32 D_X[]` rather than a scalar
  emits retail's `lui/lw` instead of a `%gp_rel` load
  (`func_002A87F0`).
- `((void (*)(void))p)()` leaves `$4` untouched before an indirect
  call, preserving retail's `jalr; nop` and the earlier `li $4`
  (`func_0028BE78`).
- Declare switch cases' shared `count` and `i` at function scope
  when retail retains the same register assignment across arms
  (`func_002D62D8`).
- `size = a + b; size += c; size += d;` preserves each add in retail
  order, where one sum expression gets reassociated (`func_0029A5E0`).
- Order of independent stores after a call: when two stores have the same
  priority (e.g. `li $3,1; sw $3,4(p)` and `sw $2,8(p)` where `$2` is the
  call result), sched1 breaks the tie towards the LATER source statement.
  To get `sw 4; sw 8` in the asm, write `state->fileHandle = h;` first and
  `state->loaded = 1;` second (`func_00242340` / DDS2 `func_0025D758`; the
  natural order gave `sw 8; sw 4`). The rule only applies to equal
  priority: a store fed by a constant that needs `lui/ori` or `lui/mtc1`
  has a longer chain and is emitted first regardless of order, and moving a
  statement also changes the live ranges seen by global alloc (sched1 runs
  before it), so callee-saved assignments can shift (DDS1 `func_00183EE0`).
- Reading the scheduler: `tools/cc.sh -DSKIP_ASM -dS -fsched-verbose=5 file.c -o x.o`
  (compile the unit copy outside `src/`, so the dump files stay in its
  directory; `-dR` for sched2) writes `file.c.NN.sched`/`.sched2` with, per
  function, the dependence table (`prio`, `cost`, forward dependents) and
  every `Ready list (t = N)` with the insn picked. The list is sorted so the
  LAST entry is issued first. Ties on priority and dependent count keep the
  previous pass's order (sched2 sees sched1's output), which is why two
  independent stores come out in an order unrelated to the source once their
  operand chains differ in length. An insn with an alias-set conflict against
  an earlier store (e.g. a `u32` field store after a struct copy that ends in a
  `u32` word) gets an anti-dependence on it and becomes ready later than a
  float-field store, so it is emitted after it (DDS1 `func_00183EE0`).
- The member types of a block-copied struct decide which later stores may be
  hoisted above the copy: gcc gives the struct copy the struct's alias set, and a
  later store to a field whose type occurs inside that struct is ordered after
  the copy. `typedef struct { u32 word[7]; } Head;` made the `u32 color` store
  wait for the copy and come out after the float `scale` store, while retail has
  `sw color` first; declaring the head as `f32 word[7]` (the copy is still
  `ldl/ldr`) lets the integer store go first and matches (DDS1 `func_00183EE0`,
  DDS2 `func_0018BB38`). When two heap stores come out in the wrong order, try
  the other scalar type for the copied block.
- Equal-priority stores that read registers: sched1's `rank_for_schedule` prefers
  the insn with the smaller `INSN_REG_WEIGHT` (a register that dies at the insn
  counts -1) before it falls back to source order (lowest LUID first). The LAST
  use of a value therefore issues before earlier stores of the same priority:
  with `cosine` stored twice (`matrix[5]` and `matrix[10]`), whichever statement
  comes later in the source carries the REG_DEAD note and is emitted first.
  Measured on DDS1 `func_002DD608` (cos/sin rotation cells, three stores of
  prio 1 plus the `neg.s` store that is not ready for 4 cycles): source order
  [5],[6],[9],[10] gives 0x28,0x14,0x18,0x24; [10],[5],[9],[6] gives
  0x14,0x18,0x28,0x24; [9],[6],[10],[5] gives 0x18,0x14,0x28,0x24 (the dying
  store goes first, ties by source order, and a free memory slot while the
  `neg.s` result is in flight is filled by any ready store). Retail's
  0x14,0x18,0x24,0x28 needs the 0x28 store to be unavailable until the `neg.s`
  store has issued; no source order, pointer/2-D indexing, local, or
  unit-matrix macro variant (memory clobber removed, `"=m"` operand, non-volatile)
  reproduced that, so the 12 rotation builders stay in `build/parked/`.
- A walking pointer that is the call-result variable itself, plus a second
  variable for the unmoved base: `cursor = f(h); base = cursor; cursor +=
  *(u32 *)(cursor + 0xC); for (...; cursor += 8) { r = base + *(u32 *)(cursor + 4);`.
  Retail then shows `daddu $3,$2,$0` (cursor = return value), `daddu $6,$3,$0`
  (base copy, scheduled after the 0xC load) and `addu $3,$3,$2` advancing the same
  register. Two separate pointers (`base = f(h); entry = base + ...`) merge the
  call-result copy into one pseudo and give a different register split
  (DDS2 `func_003074F0`; ~10 words off with every two-pointer variant).
- A stack frame with no (or fewer) stack accesses than the C needs is an unread
  local in the original source. gcc 2.96 deletes stores to a local array that
  nothing reads but keeps its frame slot:
  `float w[4]; w[0] = t * t; w[1] = t * t * t; return t + 1.0f;` compiles to
  `addiu sp,-16 ... addiu sp,16` with no `swc1`. Use it only when retail has such
  a frame, name/type it from the sibling (`f32 w[4];` like `effMathStepBezierSlot`,
  `s32 blended[4];` like `effParModulateColors`), and comment it
  `/* never read; gcc drops the stores but keeps the frame slot */`
  (DDS1 `func_0018E548` frame 0x10 and `func_0015DE88` frame 0x20 with one
  store, plus DDS2 twins). Not a way to change register allocation.
- A single shared failure exit (`goto fail;` ... `fail: return 0;`) is ordinary
  developer C (several guards jumping to one failure exit). Use it only when
  plain `return 0;` provably differs: plain returns hoist `$2 = 0` to function
  entry and let reorg steal the epilogue's first `ld` into annulled `bnezl`/`beqzl`
  slots (two return labels), and merged guard chains lose the per-branch
  `daddu $2,$0,$0` slot copies that retail has. Name the label for its meaning
  (`fail`, `notFound`), never for the epilogue (DDS1/DDS2
  `btlFindEligibleTargetForMultiActorCommand`).
- A void function whose last call sits inside `if (cond) { ... }` sibcalls it
  (`j`, epilogue duplicated). When retail keeps `jal` plus one shared epilogue,
  write the guard as an early `if (!cond) { return; }` followed by the body; the
  `return;` makes the final call a normal call (DDS1 `func_00119F08` / DDS2
  `func_0011A808`: nested form gave `j datMoveCursorY`, early return gives the
  retail `jal`). Returning the callee's result through a local instead only adds
  `daddu` moves.
- A `dsrl $4,$4,N` (64-bit shift) feeding a 32-bit `andi`/`ori`/call argument is
  a shift of a narrow (`u8`/`u16`/`s8`/`s16`) parameter: the register is already
  zero/sign-extended, so cc1 emits `dsrl` and the mask drops the extension. A
  `u64` parameter gives the same `dsrl` but then needs `dsll32/dsra32` before a
  `u32` argument, and a 64-bit `or` with an immediate becomes `li` + `or`;
  `u32`/`s32` give `srl`/`sra`. `void f(u8 channel) { snd(((channel >> 3) & 0xF) |
  0x110, 0, 0, 0); }` matches (DDS1 `func_002E9810`/`func_002E9918`/
  `func_002E94E0`/`func_002E9510`, DDS2 `func_003426B8`/`func_00342388`/
  `func_003423B8`).
- Preserve a 32-bit packet field's unsigned width when storing it through a
  wider lvalue. `u32 descriptor = 0x20000000 | (quadwords & 0xFFFF);` followed
  by `header[0] = descriptor;` and the equivalent direct store
  `header[0] = (u32)(0x20000000 | (quadwords & 0xFFFF));` give the same code.
  Omitting the `u32` boundary makes the expression signed, so its conversion to
  the `u64` element uses sign-extension. Even when the value is always positive,
  that changes local-allocator quantity grouping and can rotate otherwise
  unrelated registers. Use the typed local or cast only when the field really
  is unsigned 32-bit (DDS1 `func_002E0618`, DDS2 `func_003394C8`).
- Mixed `j` and `jal` tails inside one function (one arm `ld...; j f`, the last
  arm `jal f` + shared `jr`) come from an explicit `return;` in an earlier arm:
  the `return;` makes the epilogue label a jump target, so the call that jumps
  to it stays a sibcall but a call that merely falls into the label is not.
  A second `if (v) { f(0x58); }` after `if (v == 0) { f(0x54); return; }` gives
  `j` then `jal` (DDS2 `func_00308F78` matches that way), but that second test is
  redundant and folded away, so it is a codegen lever and is NOT accepted as
  source; the function stays INCLUDE_ASM until a real shape is found. The same
  call in `if/else` without `return;` gives `j` for both, and in a non-void
  `if/else` without returns `jal` for both.
  Toy results (callee extern void, 3 prior calls): early `return;` + fall-through
  last call -> `j`,`jal`; `cond = 0;` statement after the last call -> `jal`;
  a loop wrapped around it -> `jal`; a plain straight-line function -> `j`.
  A file-level -fno-optimize-sibling-calls needs `jal` on EVERY tail; a unit
  with `j` tails in other functions is not such a file.

## Pointer and loop addressing

- `R *r = &w->records[i]; r->field` computes base plus index before
  the field offset (`addu base,idx`); direct `w->records[i].field`
  reverses the operands (`func_002806E8`).
- For byte indexing, `*(arr + i)` and `arr[i]` likewise give opposite
  `addu` operand orders (`func_00287E60`).
- `D + i * 0x1C4 + 0xA60` puts the scaled offset before the
  constant; indexing a pre-biased `(T *)(D + 0xA60)` reverses the
  arithmetic order (`func_002B6C70`).
- Repeated `unit->entrySlots[index].f` accesses retain separate address
  pseudos and `daddu` copies; caching a slot pointer merges them
  (`func_001ADDD0`).
- DDS1's temporary movie-sprite scaler at `0x0026E798` is exact with direct
  `EffectSlotSet.workEntries[index]` accesses for scaled and restored width
  and height. The primary `BdWork` owns the signed source bounds at
  `+0x7C/+0x80`; a cached work pointer preserves the behavior but changes
  row-base registers and post-draw load/store scheduling (18/66 words differ).
  Do not replace this owner with a compact movie-only record view.
- When retail computes one scaled row base and then copies it for several
  component loads, preserve a genuinely homogeneous row as a multidimensional
  scalar array. Paired `mnuDrawSprite` is exact with its signed-halfword table
  declared `s16 rows[][4]` and direct `rows[index][component]` reads: pass 00
  creates one eight-byte row address and the later pointer-copy topology.
  Repeated fields of an array-of-struct view create multiple row additions;
  caching one row pointer instead collapses all of the copies. Require the
  element type, row stride and component offsets to agree with the data, then
  compare `.00.rtl`; if the address topology is unchanged, stop rather than
  adding aliases or redundant pointer locals.
- A countdown pointer loop in retail (`addiu v1,-1; sb ..,0(v0); bgez v1;
  addiu v0,-1`, pointer starting at `base + N-1`) is an ascending index
  loop in the source: `for (i = 0; i < N; i++) buf[i] = c;`. When the
  counter is only the exit test, the loop pass strength-reduces and
  reverses it. Writing it descending gives an indexed `addu` per iteration
  or a folded `lui hi(sym+N-1)` instead (DDS1 `func_00101440`). An
  up-counting `slti/bnez` counter beside pointer ivs means the counter has
  another use.
- A struct containing `u32 slot[25]` keeps `slot[i]`'s `sll 2` outside
  the loop; a bare pointer indexed by `i + 3` strength-reduces
  (`func_0030DAA0`).
- A local `s32 *work = D_0043E5C0` reused for field offsets keeps
  one `lui/addiu` base; direct `D_0043E5C0 + 0x14` biases the base
  instead (`func_0011DB20`).
- An array-element store `D_003BA928[3] = 0x80` produces retail's
  `sb; jr; nop`; using a separate symbol for that byte lets the
  assembler fill the return slot (`func_00106240`).
- Separate `s32 cx = cam->x, cz = cam->z` and compute z before x
  to retain z-first loads (`func_00146900`); direct x-then-z
  expressions match its DDS2 twin (`func_00149E08`).
- A saved-register exchange between a cursor and the loop index often means
  the source indexed (`p[i]`, `entry[i]`) instead of advancing a cursor
  (`*p++`). Loop strength reduction produces the same pointer walk, but its
  induction-variable pseudo has a different creation order and live range
  from a named source cursor, so the s-register priorities flip. A filtered
  collector writes `out[count]`, not a second output cursor. Verified on
  DDS2 `func_001B32F8` (input and output indexed), `func_0029D790`,
  `func_002294D0`. Don't apply blindly: indexed motor, particle-replay and
  memory-map loops did not match.
- DDS1 `func_001BE8A0` (item command rows) likewise indexes its existing byte
  table with `index * 2` and `index * 2 + 1`; gcc derives the saved-register
  pointer walk. A separately advanced source pointer changed allocation and
  loop setup. The indexed form plus the natural `selected == index` comparison
  matches all 476 retail bytes; the reverse comparison differed only in the
  `xor` operand order at +0xE8. The complete unit checked at 255 match, 0 differ.
  Its text helper also now follows the matched font provider's real `u32`
  glyph/color and `char *` text contract, rather than old `u64` placeholders.
  DDS2's corresponding helper now follows matched `func_0019F5E8`'s same
  contract. Reusing the existing `BtlResBlock` and `EffectSlotSet` owners also
  replaces that unit's partial panel views: `resC->workEntries[0x16].geometry.bounds[2]`
  and `.sourceWidth` are the old inner offsets 0xDCC and 0xE3C.
  Do not assume the DDS1 renderer transfers unchanged: DDS2's three honest
  forms still exceeded its 476-byte body and remain parked, not enabled.
- A packed table can have a pointer-typed header but numeric `u32` work handles
  in its payload. Check the consumer before choosing an array-of-pointers
  representation. DDS2 `func_002EB968` returns such a table:
  `effReleaseBillFrameEntries` reads its rows as `u32` and passes each handle
  to `effDestroyClassResourceWork(s32)`. Keeping the header's entry pointer and
  the payload's handle type distinct preserves the resource-slot load before
  the row store. This recovered representation checks at 649 match, 0 differ
  for the complete `code_002DE248` unit.
- `mnuCreateListState` returns an opaque `struct MenuList *`. Keep that
  allocation contract in consumer declarations, and convert explicitly only
  where an existing API stores an encoded integer handle. Five consumer units
  in both games check clean after this correction. The terminal units retain
  unprototyped declarations because their matched callers pass both three and
  four arguments; a callee using only three parameters is not evidence for
  deleting the fourth argument or inventing a variadic interface.
- Share recovered layouts across game twins rather than maintaining duplicate
  byte-offset views. `btl_action.h` now supplies the native 0x70-byte
  `BtlRuntimeTask` header and 0x20-byte `BtlActionAnimationRecord` descriptor.
  Keeping task-constructor conversions at assignment and using native task
  locals removes repeated access casts without changing statement order;
  DDS1 `code_001FF030` checks at 196 match, 0 differ and DDS2 `code_002112C8`
  at 299 match, 0 differ. The descriptor camera-policy constants preserve
  DDS2's identical fixed-preset branch arms.
- A shared world-node header need not imply a universal payload type.
  `NodeA.key` is the lookup key at +0x04; `NodeA.payload` is the kind-dependent
  pointer at +0x18. The kind-0x11 field probes consume that payload as a
  position vector, while the shared header retains `void *` for other kinds.
  Removing both games' casts through `NodeA.pad` checks clean in DDS1
  `code_00126A30` (189 match), DDS2 `code_00136EF8` (65 match), and both
  `dds3WorldBasic` units (11 match each), all with 0 differ.

### Indexed field offsets expose nested records and coordinate arrays

For indexed accesses, a flat struct and an equivalent nested struct can have
different address RTL even when every final field offset agrees. In this
compiler, record field offsets are split into a byte offset and a bit
remainder at `DECL_OFFSET_ALIGN`, normally 16 bytes. `get_inner_reference`
collects the byte offsets into the variable address expression and the bit
remainders into the eventual memory displacement. Each nested record level
contributes its own split.

For scalar fields at offsets `F` in ordinary records, the useful model is:

```text
address expression: base + variable offset + sum(F & ~15)
memory displacement: sum(F & 15)
```

The sums cover scalar field offsets at each nesting level; array strides and
index terms are additional address inputs. Thus a displacement of 16 or more
can come from several nested remainders; it need not imply extra alignment.
A constant array subscript accumulates into the address offset: with a
variable index it joins that expression, while a fixed object may fold it
into the symbol offset. It is not another scalar-field remainder.
Later folding and CSE can obscure this split, especially for one isolated
access, so compare pass-00 RTL for several related fields.

Confirmed examples:

- `EvtUnitVectorSlot` has `vec[12]` followed by scalar `auxX` and `auxY`.
  At `0x38/0x3C`, the setters need an index addition of `0x30` and memory
  displacements `8/12`. Treating those scalars as `vec[12]/vec[13]` changes
  the symbol-address form. DDS1 `evtSetSlotVector` and DDS2 `func_0023E320`
  match with the recovered distinction.
- DDS2 `BtlState.debug` starts at `0x728`; its inner table starts at `0x128`.
  The two remainders, `8 + 8`, explain a `0x10` displacement in
  `func_0022F068`. An invented `aligned(32)` record contradicts the actual
  allocation and layout guards.
- `BattleActorPanelEntry.position[2]` and the nested presentation
  `cursorOffset[2]` reproduce the separate x/y address copies in DDS1
  `func_001B91F0` and DDS2 `func_001C43F8`. Two scalar members give different
  address sharing despite identical byte offsets.

Require the constructor, consumers, stride, extent and layout guards to agree
before changing a shared record. Do not add padding, alignment attributes or
biased pointer views merely to request an address shape. Check every actual
header consumer, including the other title.

The same address structure feeds loop.c's induction-variable grouping.
DDS1 `func_001B7238` and DDS2 `func_001C2450` use two reduced pointers for the
primary presentation channel and position/health fields. Nested channels,
the channel's real two-word array, and a local `s32 entries[4][2]` preserve
those groups; a cached row pointer or a two-scalar row collapses or splits
them differently. Inspect the `09.loop` giv and `combined with` records
before diagnosing a saved-register swap.

One compiler layout hazard: an anonymous union inside an anonymous struct
can lose the union's offset in this cc1. Verify actual member offsets with
the project compiler; a host compiler's `offsetof` is not sufficient.

## Rodata order

The build links the unit's `.rodata` at retail's address, in source order:
each `INCLUDE_RODATA` and each function's string literals (every distinct
literal once per unit, at its first use). Converting the first user of a
shared string to C moves that string ahead of or behind the included items
around it. Keep the string's `INCLUDE_RODATA` and use an `extern` while any asm
still references it, and don't write a literal whose bytes an `INCLUDE_RODATA`
item still supplies. `check_unit` reports `RODATA` when the full unit's
`.rodata` differs from retail.

- A local `f32 up[4] = {0, 0, -1, 1}` produces an `ldl/ldr/sdl/sdr`
  stack copy of its rodata initializer at declaration
  (`func_00146900`).

## 128-bit vector copies (`lq; sq; jr; nop`)

Tiny setters that copy one quadword compile in retail to `lq $2,0(src);
sq $2,0(dst); jr $31; nop`. A plain `u128` copy in C gives `lq; jr; sq`
(the store moves into the delay slot). The SDK implements
`sceVu0CopyVector` as an asm statement, and the game did the same: use
`PCP_COPY_VECTOR(dst, src)` from `include/pcp_vu0.h` (wrapped in
`.set noreorder`, like `fsqrtf`). This is the only non-COP2 asm helper
allowed, and only for this exact instruction pair.

For a copy between four-float vectors,
`PCP_COPY_VECTOR_F32(dst, src)` declares both complete vector objects
without a global memory barrier. Both addresses must designate four float
elements, be 16-byte aligned, and have no evaluation side effects.
It is a float-vector API; do not use it for integer or opaque-byte objects.
The existing `PCP_COPY_VECTOR` keeps its established barrier contract.
A plain C quadword copy was tried first and produced 175 words rather than
retail's 177-word orbit body.

The orbit updaters (`func_0018BB88` / `func_001937C0`, 708 bytes each)
retain the already loaded history owner when the particle count is zero,
and reload it after a nonempty loop. A global copy barrier prevents that
reuse. The bounded float-vector form preserves it. Cache the actual
particle owner before its count and resolve the value owner after the
scalar parameters, as in the native entry.

Use a record containing the four floats for the memory operands.
EE GCC 2.96 converts an array input into `asm_input:SI` and a stack-stored
pointer; the record stays `asm_input:BLK` for the actual vector. A scalar
`u128` operand or a byte-array record produces the same matching updater
here, but neither is a valid generic float-vector alias contract in this
compiler: a diagnostic store/copy/read sequence returns a stale cached
float. The float-member record correctly reloads the destination and
retains all four source stores before the copy. An integer readback remains
a useful negative control for the float-only API. Separate float-component
operands are truthful but keep the origin address in a saved register here,
changing allocation.

GCC documents the general
[record-wrapped fixed-size memory-span idiom](https://gcc.gnu.org/onlinedocs/gcc-3.4.4/gcc/Extended-Asm.html).
The component type still matters for this old compiler's alias analysis;
a record containing bytes does not automatically inherit the universal
alias behavior of a direct character access.

## `memset` as a builtin or a library call

gcc 2.96 keeps `memset` a builtin under an implicit declaration, `extern int
memset();`, `extern void *memset();` or the `size_t` prototype (a same-mode return
type is accepted), so a small constant clear of an aligned pointer is inlined
(`sw $0`). A declaration that conflicts with the builtin drops it:

- `extern void memset();` (VOIDmode return): every call becomes a plain library call
  with no result, which changes the scheduling around later calls in the unit.
- `extern void *memset(void *dst, s32 value, s32 size);` (int size, not `size_t`):
  library calls that still return a value. This is the declaration that keeps a
  four-byte aligned clear a call while the unit's other memsets keep their
  value-returning shape (DDS2 `code_002C96D0`, DDS1 `code_0028A150`).

A `void *` or `char *` destination is never inlined, whatever the declaration.

## Struct assignment vs `memcpy`

`*dst = *src` on a struct of `u32 word[N]` reproduces retail's `ldl/ldr`
copies exactly, including the trailing `lw`/`sw` word and, from 0x40 bytes,
the aligned/unaligned dual loop (`func_001E3810`). `memcpy` with a literal
size gives `lwl/lwr` on the tail word when the operands lack alignment
information. Try the struct assignment first when the tail differs.

- With both operands declared `aligned(4)`, a literal-size `memcpy`
  ending in four bytes uses `lw/sw` rather than `lwl/lwr`, without
  changing the rest of the copy (`func_002BBC70`).

## Unaligned block copies (`ldl/ldr/sdl/sdr`)

Runs of `ldl`/`ldr` and `sdl`/`sdr` pairs copying a fixed-size block come
from `memcpy(dst, src, SIZE)` with a literal size. ee-gcc 2.96 inlines it
(sizes 0x40, 0x74, 0x80 and 0x90 are verified in `effPCPMisc`,
`game/code_0028A0E0` and DDS2 `code_002DC138`). A size held in a variable
gives a call instead.

## Branches kept where C gives `movn`/`movz` (irregular switches)

An if-chain of constant results gets if-converted: `if (v == 1) return 10;
if (v < 4) return 2; return 5;` compiles to `slti; beq; movz`. The same logic
written as a small `switch`, with the cases grouped, keeps real branches, and
retail looks like that:

```c
switch (v) {
case 1: return 10;
case 0: case 2: case 3: return 2;
default: return 5;
}
```

Signs of an irregular switch in the asm: a comparison against a small constant
first (`slti $x, v, 2`), sometimes with a result that looks unused, followed
by several `beq`/`beqz`/`beqzl` branches and no jump table (fewer than 5
distinct labels). A case identical to `default` but written out explicitly
also leaves that stray comparison. Verified on ee-gcc 2.96 (from the
[Decompedia GCC page](https://decomp.wiki/compilers/GCC)).

### Constructor choice versus scalar-argument choice (measured mechanism)

A shared final constructor call does not prove the source chose only its
argument. Require native edges routing accepted selectors directly to separate
argument-setup paths and a behaviorally equivalent, mutually exclusive call
choice. Conceptually, under the existing guard, a scalar choice
`task = createCounterTask(owner, kind == 2 ? 1 : 2);` can instead be:

```c
if (kind == 2 || kind == 4) {
    if (kind == 2)
        task = createCounterTask(owner, 1);
    else
        task = createCounterTask(owner, 2);
    /* Existing common task setup and start remain after the choice. */
}
```

This example illustrates the source distinction; it is not a matching fixture.
On the measured scalar diamonds, pinned ee-gcc 2.96 runs an early
`if_convert(0)` within `02.jump`, before `03.cse` or computed liveness.
Each arm has one predecessor/successor and one pure, nontrapping `SET` to the
same SI pseudo; `noce_try_cmove` succeeds. The constructor is outside the
diamond, so its side effect cannot protect that scalar choice. An if/else
assigning only a local presents the same input. Branch probability and final
register allocation do not decide this observed path.

The exact 636-byte same-TU `btlStartCommandSoundAndEffectTasks` discriminates:
real argument copies and a call in each arm make `last_active_insn_p` fail,
rejecting early noce conversion. Qualified final read-only traces of that
case and the scalar scheduler input each preserved all 28 declared artifacts
byte-for-byte. Subsequent source trials establish this chain through pass
captures:

1. Real constructor arms survive `02.jump` without scalar if-conversion.
2. `03.cse` routes accepted outer equality edges directly into those arms.
3. In the two-selector trials, four static constructor sites survive through
   `25.sched2`; `27.jump2` commons them to two calls after the last
   if-conversion pass. The late-exposed argument choice retains branches.

This transferred from DDS1 `func_001CB410` to DDS2 LINKAGE `func_001D8C80`.
The latter used pass captures, not another live noce trace. Each trial's
ordinary and diagnostic whole-TU objects agreed in all compared meaningful
sections, relocation sections and NOBITS extents. Both complete bodies remain
nonexact. The scheduler's regions still differ; LINKAGE's aligned 57-word
region has six stack-displacement differences. Only LINKAGE's two ten-word
common task-setup blocks are byte-exact after actual call-relocation resolution.
This proves a compiler mechanism, not original source identity or whole-body
matching credit.

Before transfer, verify every accepted/rejected selector value, the provider's
consumed arguments, owner/result types, and one constructor/start per accepted
selector. Preserve captured selectors, fresh flags, post-constructor handle
reads and ordering between independent selectors. Keep common task writes
after the join. Correct value-map errors separately before comparing equivalent
forms. This does not justify duplicate effect stores, added runtime calls,
fake barriers, `volatile`, or register/lifetime manipulation. If the supported
form fails to change the predicted early input and later edges, stop rather
than manufacture effects. Whole-unit and complete-build acceptance still apply.

## Other GCC patterns (Decompedia; check each on ee-gcc 2.96)

These are documented for GCC 2.8–2.9x projects (Paper Mario, Twisted Metal
Black, SOTN). They are worth trying, but not yet confirmed here:

- **Branch-likely flips** (`beql` vs `beq`, `bnel` vs `bne`): invert the
  condition and swap the branches. Inverting `if (p > 0) A else B` does flip
  the branch sense on ee-gcc 2.96 (`blez` ↔ `bgtz`).
- **Unexpected register swaps after a branch:** the original probably
  duplicated the tail in both branches, and GCC merged it. Write
  `if (x) { ...; return f(a); } ...; return f(b);` instead of computing
  operands in the branches and calling once.
- **Negative struct offsets in a loop** (`ptr->unk-1C`): the loop advances a
  pointer biased into the element alongside the counter; write
  `for (i = 1; i < n; i++, p++)`.
- **Range checks** `(u32)(v - 0xE) < 9` are `v >= 14 && v < 23`.
- **Coalesced loads:** `if (t->a || t->b)` on two adjacent aligned `s16`
  fields becomes one `lw` (masked with `lui/ori/and` for bytes).
- **Float division by a constant** can become a 64-bit multiply and shift
  (constant table on the Decompedia page).

## Tail calls: `jal` + epilogue instead of `j`

-O2 turns a call in tail position into `j callee`. Retail keeps
`jal callee; ...; ld $31; jr $31` when:

- the caller is varargs, or aggregate-return handling requires storage.
  A varargs **callee alone** does not prevent a sibling call when its arguments
  fit the ABI registers (tested with one and three integer arguments);
- the caller returns the callee's value and their return modes differ: an
  `s64`/`u64` function returning an `s32` call, or a narrow (`s8`/`u8`/`s16`/
  `u16`) callee returned from a wider caller (`return (u8)f();` too).
  Compatible `void`→`void`, `s32`→`s32` and `u32`→`u32` return modes do not
  themselves inhibit sibling calls. Use the callee's real return type from
  its matched definition, never a made-up one. In particular, `menuRunPanel`
  returns the `s32` scheduler word in `include/mnu.h`; its `u64` mode and
  argument parameters are not evidence for a wide result;
- a local/parameter address escapes, the early RTL frame has local storage, or
  the outgoing stack argument area exceeds the caller's incoming area.
  Stack arguments alone do **not** prohibit sibling calls: a nine-argument
  forwarding function can reuse its incoming stack argument and emit `j`;
- the whole file was built with `-fno-optimize-sibling-calls` (see
  `config/dds1/cflags.txt`; `tools/find_nosibcall.py` finds such files).
- the caller returns a value (non-`void`) but ends in a call to a `void`
  callee, falling off the end: the callee's call is `jal` + epilogue, not a
  sibcall (DDS1 `func_00115970`, DDS2 `func_00115BD8`: a `switch` of
  `void` Magatuhi setup calls). If no C caller uses a result there is no
  evidence for a return type, so write the definition in era-plausible
  implicit-int style with NO type keyword, `func_00115970(EffectObj *obj) {`;
  codegen is identical to `s32` and no made-up type is introduced
  (`tools/check_unit.py` accepts a name at column 0). If a C caller does use
  the result, give the real type instead.
  This only holds when a join point follows the call (a `switch`/`if-else`
  merge before the return). A straight-line `f() { ...; v(); }` or `{ ...; g(r); }`
  in an implicit-int or `int` function still sibcalls (`j`): scratch-tested on
  void/int callees, with loops, `while` waits and `if`, none gives `jal`
  (DDS2 `func_0026CE90`, DDS1 `btlAnyGroup200HasAction`). A wrapper whose only
  `jal` form is `s64` (it matches byte-exact with `s64 btlAnyGroup200HasAction`
  returning the s32 call) needs evidence for the wide return before use.

An ordinary direct `void w(void) { f(0); }` with a scalar/void callee and no
other inhibition sibcalls. Don't "fix" a `jal` tail with dummy code or an
unsupported return type; retain `INCLUDE_ASM` and park the natural candidate.
Flag changes require evidence for the original translation unit.

DDS2 `sdfDestroyTaskWorkerTasks`, `sdfRemoveTaskItem`,
`sdfDestroyTaskResourceWork` and `sdfDestroyGridWork` use `void`, just like
their retail-byte-identical DDS1 counterparts. Their units `code_00310BC8`
and `code_00312850` already have the evidenced no-sibling-call flag; no new
split or wide return is needed. The allocation-release provider is also
really `void`, so cleanup calls must not return a fictitious release result.
The full units gate at 46/0 and 31/0; the external resource-task stop caller
gates at 22/0 after receiving the real worker-destructor declaration.


### Expansion provenance: an inlined indirect call can remain `jal`

The installed `2.96-ee-001003-1/cc1` first decides whether to generate a
`CALL_PLACEHOLDER` containing a sibling alternative in `expand_call`.
It requires a known callee `FUNCTION_DECL`; an indirect call gets only the
normal-call sequence. Later sibling optimization selects an existing
alternative, rather than inventing one after the target becomes constant.

A shared inline taking a correctly typed operation callback can therefore
inline to a **direct** `jal` with a separate epilogue, even in a branch-free
caller with a 0x10 frame. RTL inlining substitutes the constant target into
the callback's ordinary call without regenerating a sibling alternative.
This is different from merely placing a direct call inside a `static inline`:
the direct-forwarding inline control still emitted `j`.

Scratch-only probes used the normal unit flags (`-quiet -O2`, default `-G8`,
original assembler `-EL -G8 -g -Iinclude`) through `tools/ee_gcc_probe.py`.
For DDS2 `func_0026DB28`, `func_0026DB70`, and `func_0026DBB8`, a synthetic
three-site callback inline with the real
`void (*)(DatPartyRecord *, u16, u32)` prototype reproduced all three 32-byte
retail bodies: `check_unit.py --source` reported **3 match, 0 differ** for this
three-function diagnostic, **not** for the full unit. The direct-call and
direct-inline controls emitted sibling jumps. `tools/ee_gcc_why.py` located
the first difference at `rtl.00.rtl`: the direct call had a
`CALL_PLACEHOLDER`, while the callback-inline call did not.
`tools/ee_gcc_delay_slots.py` identified selector setup in the `jal` delay slot
and stack restoration in the return delay slot.

**This establishes a compiler mechanism, not original-source evidence.**
No corresponding callback inline was found in the checked-in `include/` and
`src/` inline inventory. The Nocturne December debug ELF has an empty
`.mdebug.eabi64` section and no symbol/type debug sections; its `__FILE__`
mapping does not recover this missing helper. The synthetic helper was not
landed or saved as a decompilation candidate. Three matching sites do not
justify inventing an abstraction solely to inhibit sibling calls. Recover a
real shared callback helper/API from headers, source, or independent callers
before applying this explanation to a retail function.

The independent-evidence follow-up remains **unresolved**. DDS2 IDA references
to `scrClearEntryFlag` are exactly the three direct calls above; the setter
has no references and the tester has three direct jumps plus one direct call.
No aligned literal pointer to any of these three operation addresses occurs
in the ELF's loaded regions, so no operation-pointer table was identified.
Searches of checked-in callback declarations and flag-dispatch strings did
not establish the `void (*)(DatPartyRecord *, u16, u32)` abstraction.
The clear operation's six-instruction core, also searched with register
operands masked, had no counterpart in DDS1 or either Nocturne ELF. These are
bounded negative findings, not proof that an original header-only helper
never existed; the retail wrappers must remain asm until independent
evidence supplies that missing abstraction.

A local function pointer initialized to a known callee also reproduced `jal`
in a diagnostic: its target remained indirect through `rtl.13.life` and
became direct in `rtl.14.combine`, after sibling selection. Adding such a
meaningless local to a game function is a forbidden matching lever.

Other controls confirmed that typed returns, discarded scalar results,
straight-line implicit-int definitions, K&R definitions, and unprototyped
direct callees still emitted `j`. A variadic caller, an escaped parameter
address, outgoing ninth argument without incoming stack capacity, aggregate
return, and dynamic allocation emitted ordinary calls, but introduced real
ABI/storage work absent from the tiny retail wrappers. The interleaved DDS2
clear/test callbacks are particularly useful flag evidence: retail
`func_0026DB28` calls `scrClearEntryFlag` with `jal`, whereas adjacent
`func_0026DB48` jumps to `scrTestEntryFlag` with `j`. Do not apply a whole-file
no-sibling flag to make only the clear callbacks match.

Primary compiler references: the installed cc1's `expand_call` at
`0x080B5A80` and `optimize_sibling_and_tail_recursive_calls` at `0x081DFCEC`,
corroborated by contemporary upstream GCC
[calls.c](https://github.com/gcc-mirror/gcc/blob/fd442cef30223941cf09068241580e3d8f3fb7ae/gcc/calls.c)
and
[sibcall.c](https://github.com/gcc-mirror/gcc/blob/fd442cef30223941cf09068241580e3d8f3fb7ae/gcc/sibcall.c)
(2000-09-24; upstream, not a claim of identical Sony backend source).

## Calls with no arguments that read `$a0`

When a caller passes nothing and the callee uses `$a0`, the callee is
declared or defined old-style (K&R), not as `f(void)`:

```c
void func_00196390(ctx)
    FrFontCtx *ctx;
{ ... }
```

Example: `interface/frFont.c` `func_00196390`.

## VU0 (COP2 macro mode)

gcc has no VU0 support, so the originals used inline asm (as Sony's libvu0
does). Wrap each block in `.set noreorder`, otherwise ee-as moves the VU op
into a delay slot where retail has a `nop`:

```c
__asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p->sub->v));
```

That gives retail's `lqc2; jr $31; nop` (`func_00217F70`). Only COP2 and MMI
go inside the asm (rules below); everything around it is C. Vector copies
retail does with lqc2/sqc2 are written the same way (`VU0_COPY_MATRIX` and
its siblings in `include/pcp_vu0.h`). Values passed in vf registers across calls:
`void f(void)` using the registers directly (`game/code_002E7C20.c`).

The existing volatile VU0 asm boundary can also interrupt CSE's propagation
of a constant-valued local. DDS2 `func_00136718` assigns `index = 0` after
`evtUnitGetNestedValue`, then uses `index` in the first
`evtSetUnitNormalizedDirection` call after `VU0_LOAD_VF`. That value remains a
pseudo across the intervening work and receives a saved register; the second
call explicitly passes literal `0` and uses `$zero` instead. Inspect the actual
def/use and CSE dumps before generalising this case. It does not justify an
invented local, a new asm barrier, or a `volatile` C object to pin a value.

## Inline asm: COP2 and MMI

gcc 2.96 emits almost no MMI (`pextlb`, `ppach`, `pcpyld`, `pmaddw`, …), so
MMI in the middle of compiled retail code came from inline asm, usually
Sony's libvu0/SDK macros (`sceVu0UnitMatrix` is `qmfc2` + `pextuw`). MMI is
allowed on the same terms as COP2:

1. **C first.** gcc does emit some 128-bit code itself (`lq`/`sq` for `u128`
   copies, `por` for `u128` moves). An instruction that `u128` C reproduces
   may not be written as asm; say in the macro comment or parked note which
   C was tried.
2. **Shared macros, not ad-hoc blocks.** MMI idioms live in `include/ee_mmi.h`
   (COP2 ones in `include/pcp_vu0.h`), one idiom per macro, used in more than
   one place or mirroring a known SDK macro; each macro comment names the
   retail pattern it reproduces. Operands come from C (`"r"`), not hard-coded
   registers, unless the SDK macro hard-codes them. A load or store of the
   macro's own pointer operand is part of the idiom when retail round-trips
   the value through memory.
3. **Small asm, real C around it.** Loops, calls, control flow and addressing
   are C; the asm covers only the COP2/MMI operations. A block that wraps most
   of a function is handwritten code in disguise, except for:
   - copies of SDK routines Sony wrote as C with an asm body (mark with
     `/* libvu0: sceVu0Name */` above the definition);
   - VU0 routines in the same style: the asm takes its values from C operands
     and has no branches (mark with `/* vu0 routine: what it computes */`).

   check_unit prints `ASMBODY` for any other function that is mostly inline
   asm; those stay INCLUDE_ASM.
4. **Handwritten functions stay asm.** Whole functions with `addi`, delay slots
   no compiler fills that way, or `.set noreorder` bodies with branches were
   `.s` files originally. Leave them as INCLUDE_ASM; they are not C matches.

### Macro list

Each macro reproduces one retail instruction pattern; the header comment names
the plain-C forms that were tried. Use these instead of writing the asm again.

`include/ee_mmi.h` (MMI, with the COP2 moves they are fused with):

| Macro | Retail pattern | Origin |
|---|---|---|
| `EE_MMI_UNIT_MATRIX(dst)` | `qmfc2.ni $5,vf0; pextuw $4,$0,$5; pextuw $2,$0,$4; pextuw $3,$4,$0; sq $2..$5` to `dst`, `+0x10`, `+0x20`, `+0x30` | libvu0 `sceVu0UnitMatrix` (scratch `$2`-`$5` hard-coded by the SDK) |
| `EE_MMI_RGBA_UNPACK(src, unit)` | `lw $2,0(src); pextlb $2,$0,$2; pextlh $2,$0,$2; qmtc2.ni $2,vf10; vitof0 vf10,vf10; qmtc2.ni unit,vf2; vmulx vf10,vf10,vf2x` | RGBA8888 word to floats in vf10 (colour modulate/blend routines) |
| `EE_MMI_RGBA_PACK(out)` | `mfc1 $3,128.0f; qmtc2.ni $3,vf2; vmulx vf10,vf10,vf2x; vftoi0 vf10,vf10; qmfc2.ni out,vf10; ppach out,$0,out; ppacb out,$0,out` | vf10 floats back to an RGBA8888 word |
| `EE_MMI_RGBA_PACK_UNCLOBBERED(out)` | the same pack without the `$3` clobber (effect draw templates keep a constant in `$3` across it, DDS2 `func_002E6CB8`) | |
| `EE_MMI_LOAD_VEC3(vf, p)` | `ldr $2,0(p); ldl $2,7(p); lw $3,8(p); pcpyld $2,$3,$2; qmtc2.ni $2,vf` | packed vec3 into a VU0 register (sdf motion blend); retail's ldr-before-ldl order is not gcc's packed-struct load |
| `EE_MMI_LOAD_S16X4_FIXED12(vf, p)` | `ldr; ldl; pextlh; psraw 16; qmtc2.ni; vitof12` | four s16 fixed-point 4.12 keys into floats (sdf motion keys) |
| `EE_MMI_RGBA_PACK_UNIT(out, unit)` | `qmtc2.ni unit,vf2; vmulx vf10,vf10,vf2x; vftoi0 vf10,vf10; qmfc2.ni out,vf10; ppach; ppacb`: the pack with the 128.0f scale (0x43000000) kept in a GPR across several packs; `out` is earlyclobber (event colour opcodes, two colours) |
| `EE_MMI_RGBA_PACK_F128(out)` | `mfc1 $2,128.0f; qmtc2.ni $2,vf2; vmulx; vftoi0; qmfc2.ni out,vf10; ppach; ppacb`: same pack through `$2` (`EE_MMI_RGBA_PACK` uses `$3`); event colour opcodes, one colour |
| `EE_MMI_STORE_VEC3_VALUE(dst, value)` | `pcpyud $2,value,$0; sdr value,0(dst); sdl value,7(dst); sw $2,8(dst)` | loaded `u128` to packed XYZ; aligned `lq` stays ordinary C (F024 compact/wide packet builders) |

`include/pcp_vu0.h` (COP2 and 128-bit copies):

| Macro | Retail pattern |
|---|---|
| `PCP_COPY_VECTOR(dst, src)` | `lq $2,0(src); sq $2,0(dst); jr; nop` (libvu0 `sceVu0CopyVector` style) |
| `VU0_COPY_MATRIX(dst, src)` | `lqc2 vf28..vf31` from `src`, `sqc2 vf28..vf31` to `dst` |
| `VU0_LOAD_MATRIX(src)` / `VU0_STORE_MATRIX(dst)` | the two halves of the copy above (primary matrix bank vf28-vf31) |
| `VU0_LOAD_MATRIX_B(src)` / `VU0_STORE_MATRIX_B(dst)` | same for the second bank vf24-vf27 |
| `VU0_STORE_MATRIX_UNCLOBBERED(dst)` | `VU0_STORE_MATRIX` without the memory clobber (the destination address stays shared with later uses) |
| `VU0_LOAD_VF(vf, src)` | `.set noreorder; lqc2 vf,0(src); .set reorder` (register input, no memory clobber) |
| `VU0_LOAD_VF_MEMORY(vf, src)` | same `lqc2` with a memory clobber |
| `VU0_STORE_VF(vf, dst)` | `.set noreorder; sqc2 vf,0(dst); .set reorder` (register input, memory clobber) |
| `VU0_STORE_VF_UNCLOBBERED(vf, dst)` | same `sqc2` without a memory clobber |
| `VU0_SET_ONES_XYZ(vf)` | `vaddw.xyz vf,vf0,vf0w; vmulx.w vf,vf0,vf0x`: vf = (1,1,1,0), the unit scale stored beside the zero vectors and the unit matrix in object transform setup |
| `VU0_SCALAR_OP(f, "insn")` | `mfc1 $2,f; qmtc2.ni $2,vf2; insn` where insn reads vf2x (`vmulx`, `vaddx.x`, ...): scale or set components from a C float |
| `VU0_SCALAR_OP_CLOBBER(f, "insn")` | the same block with `$2` declared clobbered, where retail keeps a live value out of `$2` around it (DDS2 `func_00206C18`) |
| `VU0_SCALE_VF(vf, f)` | `qmtc2.ni reg,vf2; vmulx.xyzw vf,vf,vf2x` with the float's bits in a gcc-chosen GPR (kept across a loop); `VU0_SCALAR_OP` re-does `mfc1 $2` each use |
| `VU0_LERP_VF10(t)` | `qmtc2.ni reg,vf2; vsubx.w vf3,vf0,vf2x; vmulax.xyzw ACC,vf11,vf2x; vmaddw.xyzw vf10,vf10,vf3w`: vf10 = lerp(vf10, vf11, t), 45 sites (particles, battle tweens) |
| `VU0_MOVE_VF(dst, src)` | `vmove.xyzw dst,src` between calls (keeping vf10's result in vf11) |
| `VU0_APPLY_MATRIX(dst, src)` | `vmulax.xyzw ACC,vf28,srcx; vmadday vf29,srcy; vmaddaz vf30,srcz; vmaddw dst,vf31,srcw`: primary matrix times a vector (libvu0 `sceVu0ApplyMatrix` core) |
| `VU0_ROTATE_VEC(dst, src)` | `vmulax.xyzw ACC,vf28,srcx; vmadday vf29,srcy; vmaddz.xyzw dst,vf30,srcz`: rotation part of the primary matrix times a vector (emitter init/update in `code_00151F58` / `code_00159B48`) |
| `VU0_TRANSFORM_POINT(dst, src)` | as `VU0_APPLY_MATRIX` but `vmaddw dst,vf31,vf0w`: primary matrix times a point (w = 1) |
| `VU0_PERSPECTIVE_DIVIDE_VF10()` | `vdiv Q,vf0w,vf10w; vmove.w vf10,vf0; vwaitq; vmulq.xyzw vf10,vf10,Q` |
| `VU0_SCALE_MATRIX_ROWS(src)` | `vmulx.xyzw vf28,vf28,srcx; vmuly.xyzw vf29,vf29,srcy; vmulz.xyzw vf30,vf30,srcz`: scale the primary matrix rows |
| `VU0_SUB(dst, a, b)` / `VU0_ADD` / `VU0_MUL` | `vsub.xyzw` / `vadd.xyzw` / `vmul.xyzw dst,a,b` between calls (position differences, blend sums) |
| `VU0_FTOI4(dst, src)` | `vftoi4.xyzw dst,src`: convert all four float components to signed 28.4 fixed-point integers (packed screen projection in DDS1 `func_001F6300` / DDS2 `func_00207C28`) |
| `VU0_NEGATE_XYZ(vf)` | `vsub.xyz vf,vf0,vf`: negate xyz, w kept (reversed direction after a matrix apply; 95 sites) |
| `VU0_CLEAR_W(vf)` / `VU0_SET_W_ONE(vf)` | `vmulx.w vf,vf,vf0x` (w = 0) / `vmove.w vf,vf0` (w = 1): the w fix-up before a colour pack or point store |
| `VU0_SET_AXIS_CLEAR_W(f, axis)` | `mfc1 $2,f; qmtc2.ni $2,vf2; vaddx.axis vf10,vf0,vf2x; vmulx.w vf10,vf10,vf0x` as one block: vf10.axis = f, w = 0 (event unit path/aim vectors; separate `VU0_SCALAR_OP` + `VU0_CLEAR_W` moves a neighbouring `daddu`) |
| `VU0_LENGTH_VF10(out)` | `vmul.xyz vf2,vf10,vf10; vaddy.x; vaddz.x; vsqrt Q,vf2x; vwaitq; cfc2.ni $2,vi22; mtc1 $2,out`: \|vf10.xyz\| as a C float (`$2` clobbered) |
| `VU0_NORMALIZE_VF10()` | `vmul.xyz vf2,vf10,vf10; vmulax.w ACC,vf0,vf2x; vmadday.w; vmaddz.w vf2; vrsqrt Q,vf0w,vf2w; vwaitq; vmulq.xyz vf10,vf10,Q` |
| `VU0_CROSS_XYZ(dst, a, b)` / `VU0_DOT_XYZ(out, a, b)` | `vopmula.xyz ACC,a,b; vopmsub.xyz dst,b,a` / `vmul.xyz vf2,a,b; vaddy.x; vaddz.x; qmfc2.ni $2,vf2; mtc1 $2,out` (`$2` clobbered) |

### Float inputs through GPR constraints

The EE compiler accepts an `f32` value as an inline-asm `"r"` input and
transfers its bits to a GPR; it does not convert the float to an integer.
`VU0_SCALE_VF` uses this convention. When a native primitive takes float bits
in a GPR, pass the actual float value through that constraint rather than
introducing a separate C integer temporary with an `mfc1` output block.

The distinction closed DDS2 `func_0032C278`, the 400-byte texture palette
blender. Explicit transfer outputs created GPR quantities whose expanded
local-allocation intervals occupied `$3` and `$4`, moving the subsequent CLUT
branch operand to `$5`. Direct float inputs remained float pseudos through
local allocation; reload inserted the native `mfc1` pair, and the branch
reused `$3`. The setup instructions can look identical while later allocation
differs. Compare the local-allocation and reload dumps, including the intervals
actually passed to register selection, before attributing a mismatch to a
register-choice tie. Keep operands and memory effects accurate; register
pinning, extra barriers and operand-order searches do not establish the source
contract.

Soft-float libcalls: the `nop` retail leaves in the delay slot of `jal` calls to the double compare helper (`func_002FC6C8`, `func_00292CE0`) comes from assembling with `as -g`; see "Assembler version and `-g`".

Uses: the colour-modulate function (`func_00151568` and copies in
`billManager`, `parManager`, `code_0018CAC8`, `code_001FF030`, dds2 twins),
`btlBlendColor`/`btlBlendColorVec` in `code_001F6110` / `code_00207A38`,
`EE_MMI_UNIT_MATRIX` in the PCP effect and `code_00151F58`/`code_00159B48`
setters, the matrix copy/transform setters in `effPCPMisc`, `effPCPScatter`,
`effMagatuhi`.

How the mostly-asm functions are marked (check_unit looks for the tag on the
line above the definition):

- `/* libvu0: sceVu0Name */`: copy of an SDK routine (the register-form
  transpose `func_002DD4D8` / `func_00336388`);
- `/* vu0 routine: <what it computes> */`: branch-free COP2 routine. Its values
  come from C operands (`"r"`/`"f"`) or, for the sdf matrix library, from the
  vf28-vf31 (primary), vf24-vf27 (second bank) and vf20-vf23 (scratch)
  register convention documented in "VU0" above; hard-coded scratch registers
  (`$2`, `$3`, and `$8`-`$15` in the transposes) are the ones the SDK code
  uses. Loops, calls and address arithmetic stay C around it.
  The sdf blend routines write that scratch (`mfc1 $2, %0; qmtc2.ni $2, vf2`)
  without declaring a `$2` clobber, as the Sony samples do; nothing is live in
  `$2` across those blocks, and declaring it moves gcc's next temporary to `$3`.

### Loaded quadword to packed XYZ stream

For an aligned four-word input and a possibly unaligned three-word output,
load the `u128` in C and pass its value to the packed-store primitive:

```c
EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
cursor += 3;
```

The compiler emits the aligned `lq`; the macro covers the upper-half MMI
extraction and packed stores. A C `u128 >> 64` is unsupported by cc1, and
union half/word extraction spills the value instead of producing `pcpyud`.
The macro does not pin the loaded value to `$3`; gcc chooses that register.

This boundary matters in short loops. `mips_r5900_lengthen_loops` counts
RTL instructions, not the hardware instructions inside an asm statement.
The combined pointer-taking `EE_MMI_STORE_VEC3_FROM_QUAD` is one opaque node
and produces two padding NOPs in F024's loop. An ordinary quadword load plus
the value-taking store exposes a real additional node and produces retail's
single padding NOP, with no artificial statement or flag change. The compact
builders (`002E27D8`/`0033B688`) and wide builders (`002E3390`/`0033C240`)
match with this form. The original SDK macro name has not been recovered.

The fixed-record builders (`002E2BB8`/`0033BA68`) also match this form.
Their first additional stream uses `2 * count` VIF elements and eight bytes
per vertex. The later streams use `4 * count` elements and 32 bytes per
vertex: keep the element count distinct from the copied byte stride.

## Float constants and strings

- Float constants are literals. ee-as puts each `li.s` constant into the
  unit's `.lit4` pool itself. Derived constants fold at compile time:
  `180.0f / 3.14f` is retail's 57.32484.
- cc1's decimal-to-float conversion can be one ULP off for long literals
  (`6.283185005f` gives 0x40C90FD9, retail has 0x40C90FDA). The retail bits
  come from the expression the programmer likely wrote:
  `3.14159265f * 2.0f`, `-3.14159265f / 2.0f`.
- For the retail 0x3C8EFA35 radians-per-degree literal, write
  `0.017453293f`; `3.14159265f / 180.0f` rounds one bit lower
  (`func_00285500`).
- Reuse one `f32 wave` across `(f32)frame / 60` and `f(wave * pi)`
  to keep both live ranges in `$f20` across the call; separate locals
  change the FPR assignment (`func_0026EEE8`).
- ee-as emits one `.lit4` word per `li.s` and never merges equal values. Two
  pool entries holding the same float therefore mean cc1 loaded the constant
  twice. `t = 0.0f; if (c) t = a / b;` is a branch around a block, so cse
  (`skip_blocks`) keeps a constant loaded before it for the code after it: one
  entry, and the register survives the next call in a callee-saved FPR.
  `t = c ? a / b : 0.0f;` expands with a jump over the then-arm, the join
  label has two uses while cse runs, the path stops there and the constant is
  reloaded: two entries (`func_0025AA20` family, `3.14159265f` twice).
- cc1 truncates decimal float literals: `0.2f` is 0x3E4CCCCC, `0.1f` is
  0x3DCCCCCC, `3.14159265f` is 0x40490FDA, and `3.1415925f` is 0x40490FD9.
  Retail's "0.19999999" / "0.099999994" are `0.2f` / `0.1f`; write what the
  programmer wrote, not the shortest decimal of the retail bits.
- A call result scaled right away, `t = sdfSinPoly(x) * 0.2f + 0.1f;`,
  keeps `t` in `$f0`; `t = sdfSinPoly(x);` followed by a use of `t` lets cse
  forward `$f0` into the multiply, and `t` then takes the preference of the
  `(x - 5.0f) / 40.0f` numerator's register (`$f1`) instead.
- Float arguments to an unprototyped callee are promoted to double and go
  through soft-float helper calls (extra `jal`s). Give the callee a
  prototype with `f32` parameters.
- Strings of 8 bytes or more (counting the NUL) are literals in `.rodata`.
  Shorter ones live in `.sdata`.
- A literal whose retail copy an asm function of the unit still uses must
  stay `extern char D_X[]; /* "text" */` until that function is C
  (check_unit reports `SHARED`).
- splat shows the zero bytes between a string and a following 8- or
  16-aligned item (often a jump table) as extra `.asciz ""` entries. They
  are alignment, not a literal: write the strings and drop the
  `INCLUDE_RODATA` line if the blob holds only them; check_unit treats the
  jump-table alignment as no PAD (`func_0021BFB0`). `.sdata` strings stay
  `extern char D_X[]` with their `INCLUDE_SDATA` line: a literal moves to
  `.rodata` and breaks the ELF even when check_unit passes.

DDS1 `func_0020B640` is a concrete negative-literal example: retail loads
`D_003B98D8` from `.lit4` as `0xBDCCCCCC`, which cc1 reproduces from `-0.1f`.
Calling the camera routine with that literal preserves the float-constant
scheduling and matches its 304-byte dispatch and 40-entry jump table.
An external float declaration instead moves the pool load and the command
argument setup. The command owns the output pose at offset zero, so the
typed call passes `&command->camera`, not a second object-prefix view.

## 128-bit data

`int __attribute__((mode(TI)))` (u128) copies give `lq`/`sq`. gcc fills the
return delay slot with the last `sq`. If retail has `sq; jr; nop` instead,
the copy was not plain C: it was either an lqc2/sqc2 VU copy (see above) or
hand-written.

## Mixed integer and floating parameter order

For the observed register-passed EE helpers, integer and floating parameters
use separate register sequences. Moving an integer parameter across floating
parameters can leave the callee's incoming registers unchanged, yet change
the caller's argument expansion order and the instruction donated to `jal`.
The callee alone therefore may not establish the order between the classes.

`kwlnDrawSetC70FloatTriple` and `kwlnDrawSetD88FloatTriple` take
`(f32 rotation, f32 scale, u32 blendControl)`. Their paired script-command
callers are exact with that contract; putting `blendControl` first changes
argument setup. Compare pass-00 call RTL, the provider and every caller,
including stack-passed cases, before changing a prototype. Preserve each
argument's semantic role and update definitions and declarations together.
Do not enumerate signatures or assume this observation extends across an
argument-register limit, varargs, or an unspecified declaration.

## Seven or more arguments

The EE ABI passes arguments 5 to 8 in `$8`–`$11` (not on the stack), so a
7-argument call just loads `$8`–`$10`. Write the full prototype.

## Frame size and dead parameters

The local area is `frame - (highest saved-register offset + 8)`; size the
function's locals from that, not from the first draft
(`fldStartMiniTitleForUnlock`: 0x40 frame, saves at 0x30/0x38, so 0x30 of
locals). A frame larger than the locals plus saves can also be an outgoing
argument area, but only if retail stores to `0x0..0xF($sp)` before a call.
A frame that is too large in our build often means a parameter retail never
reads: drop it from the definition (`func_00108CB8`).

## Address of a member vs a pointer member

`addiu $a0, $base, 0x68` before a call passes the address of an embedded
member (`f(&work->dma)`); `lw $a0, 0x68($base)` loads a pointer member
(`f(work->dma)`). An embedded array behind a header is a struct with the
array as its last member (`EffectRingBlock`, `MapRequestRing`).

## Event-unit vector-slot storage

`evtResetUnitVectorSlots` and the vector setters share the canonical
`EvtUnitVectorSlot` in `evt_unit.h`. Each native 0x40-byte record contains the
state and bound-unit ID, three vec4 at +0x08/+0x18/+0x28, and the two auxiliary
coordinates at +0x38/+0x3C. DDS1's static pool has seven entries; DDS2's has ten.
This is neither the event unit's motion object nor its world-list node.

The DDS2 progress-reward table also used the private name `EvtSlot`, but its
records are 0x48 bytes and have a different table base and meaning. A repeated
private name is not evidence that those tables share an owner.

## Terminal progress-reward table

DDS2's `MnuProgressEntry` in `mnu.h` owns the complete 0x48-byte records at
`D_003CE1A8`: an unsigned threshold, a model flag and eight 0x08-byte
`MnuProgressReward` entries. The native selector scans eight records with a
0x48 stride and compares the threshold unsigned. A reward's value is an item ID
when its kind is zero, and a currency amount otherwise.

`D_003CE1AC` is the first record's flag address, not a separate table of
flag-first records. Flag readers must use the canonical table's `flag` member;
do not retain the shifted-base padded view. The selection-label and grant
callbacks use the same reward array.

## SDK light-source descriptors

`SdfLightSources` in `sdf_draw.h` is an array of three optional pointers to
vec4 data, not a pointer followed by two scalar metadata fields. The native
light setup routine advances its descriptor pointer by four bytes three times.
For each non-NULL entry it reads the color vec4 at +0 and normalizes/negates
the direction vec4 at +0x10. The event manager, vector-slot updater and default
light-direction updater share this ABI; pass the array itself, not the address
of a private descriptor struct.

The provider remains assembly. Changing its callers' input ownership does not
authorize modifying its body, compiler flags or expected object.

## Magatuhi history and parent ownership

`EffMagatuhiValueWork` is the complete 0x38-byte history owner at the end of
one retained allocation. Its arrays hold `count * historyCount` positions and
sample values, a `historyCount` palette, per-slot colors and ring indices, and
four angle floats per slot. The angle storage is an array of four-float rows,
not a second scalar-pointer view.

The separate 0x20-byte `EffMagatuhiOwner` copies seven parameter words into
its 0x1C-byte `EffMagatuhiHistoryParams` and appends a history-owner pointer.
Its release/update callers share that owner, rather than padded handle-word
views. The factory returns the actual history pointer; the allocation handle
and address returned by the resource-retain API remain separate SDK concepts.

The clone input is seven word-aligned words beginning at the effect head's
`particleCount`; all ten callers supply that word's address. Preserve that real
word input instead of erasing its alignment with a byte-blob pointer.

## Textured-square template ownership

The event manager's former nine-word `EffTemplateBody` is the same
0x24-byte `EffResourceRectParams` consumed by `effCloneResourceTemplate`.
The live textured-square work is the canonical 0x28-byte `EffResourceRectWork`,
not a second body-plus-word layout. Its selected source handle follows `params`.
The event setup table, its whole-parameter setters and the viewer's 0x24-byte
parameter copies all use the same canonical record.

## Model-track parameter and work ownership

`EffTrackPolyParams` in `eff.h` is the complete 0x34-byte constructor input,
not a model pointer alone. `effTrackPolyCreateWork` allocates a 0x3C-byte
`EffTrackPolyWork`, copies the thirteen parameter words, installs its fixed
step count, clears `updateCount` and creates independently owned history data.
The renderer, constructors and model-viewer record conversion share these records.

`sampleInterval` is unsigned: the active update uses unsigned remainder to
gate endpoint sampling. Its borrowed `MdlCtx` supplies the SDK model through
`inner` and the frame bounds through `first->currentFrame`; the former padded
`EffTrackPolyModel` and `EffTrackPolyModelState` were prefixes of the existing
context and motion owners, not separate model allocations.

`MdlResourceItem.type` distinguishes the track pointer stored by
`mdlCreateViewerEffectPart` (type 2) from the handles installed by
`mdlBindViewerPartRecords`. Keep those alternatives on the same tagged union
in `mdl.h`, and use `part.track` for track creation, release and update.

DDS2's resource dispatcher is consumed only for its side effects. Its caller
at `0x234E24` immediately invokes the next-record helper without using `v0`.
The track creator stores the returned work pointer, then restores registers
and returns without forming a separate result. Both routines therefore use
the same void ABI as their DDS1 counterparts rather than returning incidental
callee register contents as integer handles.

## Battle records and saved-party ownership

The DDS1 battle getters return the canonical `DatPartyRecord` entries in
`datGameState->party`. The current-entry getter forwards its roster ID to
`dds3FindEntryIndex`; it does not load that ID through a pointer argument.
Use the shared record for party scans and command-power snapshots, and the
shared game-state inventory, battle-flag bank and 40-byte scene records
rather than local byte-offset views.

DDS1's 280-byte scene-stream selector at `0x001F3278` reads the same
complete `BattleAdjustmentRecord` used by encounter rolls and stat
adjustment. Its `u16 streamSelection` is at `+4`, while the four
`0x7C`-byte groups begin at `+0x1C` and each contain twenty six-byte
entries. Keep this owner in `dat_state.h`; the old stat consumer's
fourteen-entry, rebased local view was not the actual record boundary.
`BtlState.unk24A` is the existing byte selecting fallback stream 2 when
the scene record has no sound override. Selection 5 cycles through the
five-ID table using the unsigned counter modulo 5, then increments it.
The three migrated units gate clean at 83/0, 272/0 and 570/0.

DDS2's `ptyGetCombinedRecordAndSlotValue(s32 id, s32 slot)` returns an
`s32` sum of the item's base value and saved stat bonus. Its menu consumer
`mnuApplyPackedGroupValues` now passes a word-sized item ID and keeps the
result in `s32`; the old `u64` declarations were not evidence of packed
values or a wide return. The draw controller reads the canonical party
record's `itemId` at +0x1B2, not an alternate record view.

Both games now embed the complete canonical `DatPartyRecord` in `BtlUnit`
at `+0x120`. Actor stat getters, command-power snapshots and status checks
use that record directly rather than a second actor-stat prefix. The earlier
`+0x174`/`+0x178` actor-link interpretation was superseded by the shared
record layout; it is not evidence for an overlapping owner.

DDS1's escape query at `0x001A8640` instead walks the real
`BtlState.units` chain. Its unsigned halfword loads at `0x001A86B4` and
`0x001A86E0` read `BtlUnit.partyRecord.status`: the complete party record
starts at `+0x120`, and its status word is at `+0x0E` (`+0x12E` overall).
The `0x2A0F` eligibility mask belongs to that record, not a second actor-flags owner.

DDS2's command-block query at `0x001ABB10` reads the shared two-byte
`DatCommandSelector` rows: the signed resource/stat selector is byte 0,
and the signed kind is byte 1. Kind 0 and kind 2 both reach the cost-mode
restriction; treating kind 2 as an unconditional exemption changes behavior.
The slot-cost query accepts `BtlUnit *`, and the enabled-current-ID check
and `mnuGetPartyEntryCurrentId` accept `DatPartyRecord *`; the latter returns
the record's `u16 itemId`, not a word-sized address or a second record view.

DDS1's camera-selection command stores its selected actor in
`BtlLinkedCommand.selectedUnit` at `+0x100`, then reads the native `u16`
`BtlState.cameraPresetMode` at `+0x244`. Keep these fields on their existing
complete owners; the adjacent store/load is not evidence for a new command
or state-prefix view. DDS2's corresponding command uses its existing
selected-actor field at `+0x120` and mode field at `+0x268`.

DDS1's actor-presentation bridge at `0x001B83D8` receives a complete
`BtlTask *`: `task->unit` at `+0x18` supplies the native 64-bit actor
identity compared against `BtlState.units`. Its signed mode/value bytes
update the selected `BattleActorPanelWork.activeEntries` record rather
than an overlapping panel-row prefix. The command scene borrows the task
through `BattleSceneObject.owner`; the reset callback at `0x001C9E20`
passes that same task type and sets its existing `flags` member.

DDS2's special-actor chunk update at `0x0021A778` walks
`BtlState.units` and compares the signed `EvtUnit.slotC` frame threshold
against the model's current frame. The unsigned ID is
`BtlUnit.partyRecord.unitId` at `+0x124`, not an independent mode field.
Promote it to a meaningful `s32` local for the ID-range tests; the retail
code uses `lhu` followed by signed `slti`. The named-chunk fade/reset helpers take
actual `const char *` names, not integer prototypes requiring pointer casts.

DDS1's actor-effect task update at `0x001F1470` transfers the already-matched
DDS2 counterpart at `0x00202100` through the same complete task-argument owner.
Its `ActorEffectOwner` union has two real uses: a dereferenced actor pointer
and the encoded word stored by the field-color selector API. The effect
factory at `0x00160958` allocates a `0x128`-byte block and returns its address;
the event and battle callers agree on an opaque pointer return and pointer
owner argument. The selected-unit task still transports that return in its
native word-array argument slot; typed actor-effect callers store it without casts.


## SDK packet words versus GS payload words

SDF's packet allocator and cursor helpers transport EE addresses as 32-bit
words. The DDS1 `code_002D33C8` / DDS2 `code_0032C278` providers return
`s32` from `sdfAllocPacketAligned`; `sdfAppendPacket` takes
`(SdfListHead *, u32)`. The corresponding `code_002DDC98` / `code_00336B48`
constructors take a real `SdfDrawPacket *` in `sdfConsInitPacketHeader`,
return `u32` from `sdfConsFinalizePacketHeader`, and return `s32` from
`sdfConsMeasurePacketWithHeader`. Decode a transported address at the
SDK buffer boundary, rather than changing these providers to invented
pointer or 64-bit signatures.

Packet-list owners passed between draw helpers are `SdfListHead *`, and
texture-bound helpers take `SdfTex *`. These are not GS register values.
Keep the actual GIF register list and GS A+D payload entries as `s64` /
`u64`: narrowing the address interface must not narrow those packed
hardware words. Both games' eleven interface emitters, border wrapper,
panel renderers, and picture submission wrappers remained byte-exact with
these distinctions.

Blur drawing shares the `0x28`-byte `EffBlurQuad` payload in `eff_blur.h`.
Event callbacks and event-viewer serialized records embed that same payload;
a four-byte color array does not establish a separate `BlurSource` owner.
Keep the surrounding serialized records distinct from allocated blur work:
the scatter and scale parameter prefixes are both `0x2C` bytes, but their
fields differ and neither includes the live owner's allocation or slot tail.

The interface sprite constructor is a different boundary: its optional
argument is a kind-selected `u32` payload word, not a universally typed
texture parameter. Texture callers encode their `SdfTex *` once when
passing that word. The sprite's allocation descriptors remain genuine
`SdfMemBlock *`, and its payload remains the generic `u32` buffer. Do not
reinterpret an allocation descriptor as a texture or impose a second
structure view merely because kinds 6--9 initialize the first payload word.

The kernel's 98-entry draw pool uses complete `SdfPoolNode` owners of size
`0x20` for its draw protocol, not module-specific callback prefixes.
The SDF providers initialize the next link, transported first/last words, and
native `void append(SdfListHead *, SdfListHead *)` / three-argument
`s32 prepend` callbacks. The SDK itself accepts the owner through its common
packet-list prefix at the call boundary; keep the full owner for array
indexing and callback access. `D_003255A8` / `D_003805A8` are surface 83,
`D_00325708` / `D_00380708` are surface 94, and
`kwlnPositionedTextSurface` is surface 96. Submission has no status result:
`itfBuildAndSubmitPanelPacket` returns `void`.
The SDK's float-key-pool payload is the separate 16-byte `SdfKeyPoolNode`;
do not conflate its kind at `+0x0C` with the 32-byte callback owner.

The message initializer is a distinct native SDK protocol boundary.
`sdfStoreMessageWordsAndNotifyConsumer` transports four 32-bit words;
field setup passes physical entries 34, 40, 42 and 41. Its work word selects
entry 40, where `sdfInstallPoolNodeReleaseCallbacks` installs
`sdfReleasePoolNode(SdfPool *, SdfKeyPoolNode *)` and
`sdfUpdatePoolFreeListByMode(SdfPool *, s32, SdfKeyPoolNode *)` in slots
`+0x10` / `+0x14`. These are not the draw append/prepend prototypes.
Keep this genuine native word transport as an explicit residual: do not
force the pool callbacks through draw signatures, add an owner union, or
introduce a second structure view. The model aliases `D_00325048` /
`D_00380048` select this same work entry.
The thirteen-row packet-group submission bridge reads the same four-word
groups; row seven can contain these work callbacks. Its generic owner
interface is therefore a separate boundary, not a draw-only owner array.
Do not force `sdfSubmitDrawPacketGroups` through draw append/prepend types.

The flag-list geometry packet wrappers `func_002EF2B0` / `func_00348158`
still have legacy caller/provider declarations. Recover their renderer
return and argument-forwarding ABI independently; canonical draw owners
do not establish that wrapper ABI.

`sdfAllocatePacketList` and its optional allocation callback return native
`s32` allocation words. Decode each newly allocated list once into
`SdfListHead *`; keep the full `SdfPoolNode` owner and typed list thereafter.
The diagnostic draw leaves similarly decode each 20-byte
`sdfAllocAligned` result once at the SDK boundary. These are allocation
transport conversions, not permission to reinterpret a game-owned record.


## Call arguments already live in registers

A call need not write every argument register immediately before the jump.
Check incoming arguments and earlier producers before inferring an absent
argument or an old-style declaration. DDS1 `mnuDrawBackdrop` already has its
surface selector in `$a1`; explicitly passing `option` to
`sdfSubmitGsTestOneRegisterPacket` preserves the native instructions and
expresses the complete interface. The legacy `fileSetRecordSecondVector`
call boundary needs its own producer/callee analysis; an unprototyped call
alone does not establish the original declaration.

GS TEST packets have a genuine 64-bit payload. In both titles, the forwarding
wrapper preserves that value, the constructor retains it with a 64-bit move,
and the GIF A+D descriptor receives it with `SD`. Use
`sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind)` and a matching `u64`
constructor input. Ordinary integer literals then reach argument expansion
as `DImode` through the actual prototype. A false 32-bit declaration converts
them to `SImode` and can change constant setup and call-delay scheduling.
An `L` suffix or a K&R declaration may hide that mismatch; neither replaces
recovering the real input contract. This width belongs to the TEST payload,
not automatically to every GS writer's source input.

## Assembler version and `-g`

The build uses the ee-as shipped with ee-gcc 2.96, invoked with `-g` (the
build and `tools/cc.sh`). With `-g` GNU as does not move instructions into
delay slots in `.set reorder` code, so every slot cc1 left unfilled keeps its
`nop`. This is the retail rule:

- call argument moves stay before the `jal` (`move $4,$18; jal cos; nop`), the
  soft-double/libm sequences (`dpcmp`, `dpsub`, DDS2 `sdfRotateMatrixBasisX/Y/Z`);
- the instruction reading an FPR the preceding `mtc1`/`lwc1`/`cvt.w.s` wrote
  stays out of the branch slot (`mtc1 $4,$f1; cvt.s.w $f1,$f1; b; nop`), and
  `li.s $f12,K; jal f; nop`;
- the closing `addu` of an indexed `la $rd,sym($rs)` or of a large-offset memory
  macro stays out of a following `jr`'s slot.

Building both titles with and without `-g` gives the same bytes for all code
that already matched (the earlier `as_coproc_delay.py` pre-pass emulated a
subset of this rule and is no longer run), so an unfilled call slot in retail
is not a source-shape problem. The older `ee-gcc2.9-991111` as is not the
retail assembler: building everything with it changes both ELFs in thousands
of places. Don't switch assemblers or flags per file.

## Loops, tail calls and register priority (gcc 2.95 internals)

Before reload, sched1's `rank_for_schedule` compares priority, then register weight,
before region/dependence, fanout and source order. Weight is register outputs minus
`REG_DEAD`/`REG_UNUSED` notes (`find_insn_reg_weight`): a copy whose source dies has
weight 0 rather than 1 and schedules **earlier**, not later, than equal-priority
independent constants (DDS1 `func_002AA748` loop preheader; still non-matching).

### Saved-register homes: what global allocation actually sorts by

Mirrored `$s0`/`$s1` homes are not a declaration-order effect. In this cc1,
`allocno_compare` sorts first on `REG_N_RANGE_COPY_P` (a Cygnus live-range
splitting copy, not a pointer or user-variable flag), then by descending
`floor_log2(refs) * refs / live_length * 10000 * width`, and only then by
ascending pseudo number. Pseudos local to one basic block with a single death
are allocated earlier by local-alloc, with copy suggestions tried first.
Controls on DDS1 `func_00104168`, `func_002E69F0` and others: plain versus
`register` storage class, prototyped versus unprototyped integer calls, and
visible/static/opaque out-of-line callees all left the priorities and homes
unchanged. The ordering follows from what is referenced and for how long:
for example, testing the other operand of a predicate swaps the homes. So look
for the real data flow (which value is reloaded, cached or recomputed) and not
for a declaration or prototype switch. Some "mirrored" parks are caller-saved
`$v0`/`$v1` problems instead (`func_00244658`, `btlSelectLowestRankTarget`).

Two exact cross-title examples recover the meaningful data flow rather than
choosing register homes. The selection-trail constructors `func_001B3DC8`
(DDS1) and `func_001BE9E8` (DDS2) initialize their real `positions[3]` origin
endpoint and derive the other three positions directly from that member.
Computing a separate scalar origin and copying it into the fourth lane later
leaves different temporary allocation. The producer, four-lane updater and
40-byte allocation establish the actual array; no storage view is invented.

The reserve pulse renderers `func_001B7C90` (DDS1) and `func_001C2EA8` (DDS2)
follow the existing active-panel renderer's initialization phases: initialize
level/geometry defaults, then select the neutral `baseColor` before the signed
state guards. In this pair, an early declaration initializer leaves 19 words
different; explicit color selection after the default initializers restores
the native constant and branch-delay scheduling. Moving color selection past
the guards exceeds the native extent. These are bounded source examples, not
permission for declaration-order enumeration or artificial lifetimes.

### Non-rotated loops: `b` to the top-of-body test (stmt.c `expand_end_loop`)

`expand_end_loop` "rolls" the loop-top test to the bottom. It scans from the top
of the loop for every jump whose target is the loop's exit label (the loop test
*and every `break`*). It counts INSN and JUMP_INSN only (CALL_INSN not counted)
and stops looking once more than 30 have been counted after the first exit
found. Everything from the loop top up to and including the **last** such jump
is moved after the rest of the loop (the `continue`/increment block) and a
`b start` is put in front:

- last exit jump inside the 30-insn window: `[b top][incr][top: test + body up
  to the last break][remaining tail]`. This is retail's non-rotated loop
  (for `for` loops the increment sits before the test). The tail block that
  follows the last `break` ends up physically before the increment.
- last exit farther away: only the test is rolled, and jump.c
  `duplicate_loop_exit_test` (no call or label in the exit code) rotates it into
  guard + do-while. This is what plain C usually gives.

So a loop that keeps `b` + test at the top needs its last `break` within the
first ~30 insns of the loop. Example (DDS2 `func_0025FE70`): a `continue` chain
followed by two separate `{ result = 1; break; }` exits; one merged
`if (a || b) continue; result = 1; break;` rotates differently.

### Loops that loop.c never optimises: a branch from outside into the test

A loop that retail enters by a branch from outside straight into its bottom test,
and that keeps loop invariants inside (`lui %hi(sym)` recomputed every iteration,
constants re-materialised, `sll i,2; addu base` per iteration instead of a pointer
giv), was skipped by loop.c entirely. jump.c redirected a branch that lies outside
the loop notes into the loop's test, so loop.c saw a jump into the loop. Sources that
do this:

- the loop opens a `switch`'s `default:` arm, or an arm reached by the compare
  chain of a non-jump-table switch: `switch (mode) { case 1: ...; break; default:
  for (i = 0; i < 4; i++) ... }` (DDS2 `func_002D1058`, DDS1 `func_00290FE0`, DDS2
  `func_002C1FF0` with the same loop in both arms, only the default copy
  unoptimised). The jump-table cases and `if`/`else` arms are optimised normally.
- the loop directly follows an early-return guard written as one `||` chain,
  `if (a <= 0 || b < 0 || c < 0) return -1; for (...)`: the last compare branches
  into the loop test with the counter init in its slot.

Reusing one counter `i` across all loops of the function is ordinary C89. In
`func_002D1058` it also gave retail's register for the index.

`*out` as the loop cursor (DDS2 `func_0025E6F0`): `*out2 = track->first;
while (*out2 != 0) { if (v < (*out2)->frame + base) break; *out2 = (*out2)->next; }`,
then `if (*out2 != 0) *out1 = (*out2)->alt; else *out1 = track->fallback;` (a
store in each arm; a ternary puts the temp in `$7`).

List walks with a shared increment block (`b test; L: lw next; test: beqz node;
... bnez flags, L`): load the second field into its own local first
(`u32 key = node->sortKey;` before the `flags & 1` test) so reorg fills the
`bnez` slot with that load; otherwise both slots take a copy of the increment.

A `b` into the middle of a loop that lands on a *call* in the body (one
`jal work` for the first item and the rest, `nop` in its slot) is a written-out
first iteration plus a priming-read `while`, merged by the final jump pass's
cross-jump: `work(q, first, first); next = find(q, id); while (next) {
work(q, next, first); next = find(q, id); }` (DDS2 `func_002D4CF0`/`E60`/`F10`
and DDS1 twins). A call in the loop condition blocks `duplicate_loop_exit_test`
and gives a `b` onto the `find` call instead.

### Leading exits can make loop.c report a loop as "phony"

DDS2 `func_001C9BE8` has two independent bounds at the loop head:

```c
for (;;) {
    if (row >= shown || index >= total) break;
    /* Draw this row, then advance row, index and y. */
}
```

This natural form keeps the retail constants and indexed addresses inside the
loop. A combined `for (; row < shown && index < total; ...)` instead receives
normal loop optimisation. Two consecutive leading `if (...) break` tests can
produce the first form too, but the syntax alone is not sufficient evidence.

The decisive diagnostic is `is phony` in `09.loop`: scan_loop did not find the
expected loop label immediately after `NOTE_INSN_LOOP_BEG`. In the confirmed
case, GCSE inserted work on the entry edge with a block-begin note between
the loop-begin note and label. Other two-bound loops are ordinary loops and
must not be forced into this class. If the dump has a normal loop, investigate
its cost decisions instead.

### An optimised loop can still retain invariant constants

Missing hoists do not always mean loop.c skipped the loop. Its movable-value
decision compares `threshold * savings * lifetime` with the loop's instruction
count before strength reduction. Calls change the threshold; the relevant
counts are RTL instructions, not the final assembly length. Related cost
tests also decide whether a general induction variable is worth reducing.

DDS1 `func_0014B688` is exact with direct
`fldSparkObjectEntries[i].field` accesses. Their larger initial address RTL
keeps constants in the loop while pointer induction variables are reduced.
Caching and advancing an entry pointer shrinks that RTL and hoists the
constants, as in its DDS2 counterpart. Compare the actual `savings`, movement
and `not worth while` records in `09.loop`. Do not pad the loop with unused
work to cross a threshold, or infer a per-file option from one missed hoist.

### Geometry vertex counts define the reduced pointer loops

DDS2 `func_002F8648` builds a cylinder from 32 XZ samples and 31 intervals.
Its real scratch buffers hold 18 wall vertices and 48 cap vertices, each a
16-byte row whose XYZ components are serialized. Index the wall rows by
`vertexCount` and `vertexCount + 1`, then advance once by two. Index each fan
triangle by that count and its next two rows, then advance once by three.
The wall's full-buffer path keeps the sample index unchanged to repeat a
seam; the top fan's remaining 45 vertices carry into the bottom fan.

These count transactions determine the address induction variables in
`09.loop`. Indexed cap construction recovers the independent top-cap origin
and the native `0x5D0` frame. In the measured pointer-wall control, the circle
origin has five references over 202 instructions, giving allocator priority
495; the top-cap origin has three over 62, giving 483. Indexed wall generation
extends the circle lifetime to 208, reducing its priority to 480 and restoring
the native top-cap/circle register ordering. Trace the actual index units and
reduced addresses before proposing another owner or lifetime.

The angular step is also a real `.lit4` literal. The same-unit radial
constructor supplies the full-turn expression: `6.2831852f / 31.0f` emits
`0x3E4F8C3B` under EE GCC 2.96. Use it directly in the loop update. Loop
invariant extraction places its load as retail does; pre-caching the constant
leaves two setup instructions swapped. Check the target compiler's pool bits
and earliest changed pass when applying this lesson to another function.

### Tail call kept as `jal` + epilogue: loop notes

Any loop construct around the last call leaves NOTE_INSN_LOOP notes and the call
stops being a sibcall (`do { } while (0)`, `for (;;) { ...; break; }`, a
one-iteration `for`); `switch (0)`, statement expressions, `if (1)` and plain
blocks do not. A real loop around the call is therefore one natural cause of a
`jal` tail. A dummy `do { ... } while (0)` wrapped around a whole function body
only for this is a lever and is not accepted (DDS2 `func_002A5F80`,
`func_002A5890`, DDS1 `func_002C1548` match that way and stay INCLUDE_ASM).
Other causes: varargs, struct return, converted return, address-taken locals,
stack arguments, nested `return;`.

At expansion time, a statement after the final call can also prevent a
sibling call even if later passes delete that statement. DDS2
`func_00286A58` has the release-and-clear form
`evtReleaseMantraSelectionWork(work); work = NULL;` and retains `jal`.
This is a compiler observation, not evidence that every unexplained forwarder
cleared a local. Require a genuine source-level reset or recurring macro
contract; adding `unused = 0`, an empty loop, or a local function pointer
solely to block the sibling call is not acceptable.

### Per-TU `-fno-optimize-sibling-calls`: boundary evidence and its limits

The flag does more than turn `j` into `jal`: it also changes basic-block order
(`cleanup_cfg`), so a function whose source was found under -O2 may stop matching
under it and the other way round. Check every function of a candidate run
under both flag sets (`check_unit --cflags=-fno-optimize-sibling-calls`) before
calling it a run. Examples: DDS1/DDS2 `btlRollAiBucket` matches under both only as
`if (prev >= slot - 3 && prev <= slot + 3) { reroll; return reroll / 0x29; }
prev = slot; return slot;` (the `<`/`||`/`else` spelling is -O2-only);
`btlUnitBlocksElementQueryForGroup` (both games) needs -O2 under any shape tried.
A TU that starts with a looping function can also change how a LATER function
compiles (DDS1 `func_0020A780` came out 232 instead of 224 bytes when
`btlUnitBlocksElementQueryForGroup` was the first C function of its TU; any
earlier C function cured it). If a proposed boundary fails only that way, the
boundary is wrong: retail has a C function before the first one tested.
A fully-C unit can still own an `INCLUDE_RODATA` leaf used through an external
declaration. `include_rodata.py` must retain those leaves even when no
`INCLUDE_ASM` remains. A missing generated include for data verified to belong
to the unit is an attribution/tooling issue; it does not establish a new source
boundary. DDS2 `func_0031BA28` exposed this case.
Compile an owned string as a literal or named constant only when its placement,
sharing and padding agree with retail; do not change the representation just
to hide a missing generated include.

### Saved-register order: global-alloc priority

This compiler's `allocno_compare` uses `floor_log2(n_refs) * n_refs *
hard_register_width * 10000 / live_length`, truncated to an integer. Higher
priority allocates first and takes the first legal hard register in target
order; ties go to the lower allocno number. The width is the number of
consecutive hard registers, normally one, not the pseudo's byte size.
`calls_crossed`, pointer and user-variable flags are not part of this score.
`cc.sh -dl` prints `used N times across L insns` per pseudo, so the order can be
computed with `tools/ee_gcc_allocations.py`. Natural source shapes that flip it:

1. **End a live range earlier.** DDS1 `func_00276B38`: `changed = 1;` after the
   calls in each `if` block (not before them) shortens `input`'s live length and
   fixes a `$17`/`$18` swap.
2. **Drop a reference.** DDS2 `func_002B4180`: one `window` local for paired
   stores removes a use of `party`, which lets `arg1` outrank it.
3. **Order of the definition.** DDS2 `func_002A7260`: `u32 flag = keep != 0;`
   first and the base pointer assigned after the `if` puts flag in `$20` and the
   table pointer in `$19`.
4. **Advance a cursor in place.** `entries = node + headerSize` ties the sum to
   `headerSize` (the first operand that dies there), raising its priority.
   Retail's effect allocators copy the header pointer first and then advance:
   `u8 *node = body; body += headerSize;` (DDS2 `func_002E2E28` and 15 more,
   `move $3,$2; move $4,$3; addu $3,$3,$17`). Where retail does give
   `headerSize` the higher register (DDS2 `func_002E24A8`) the sum form is right.
5. **Straight-line setters: derive the store order.** In a function with no
   branch every parameter is a local qty, each with 2 refs, so the shortest
   live range (entry copy to its store) gets the lowest saved register. sched1
   then reorders the independent stores; on toys the emitted order is: the
   object store with the higher source position, the work store with the
   highest source position, the other object store, then the remaining work
   stores ascending. Work out the live lengths retail's register order needs,
   solve for the store order, and write the source in the order that produces
   it (DDS1 `func_001168F0` / DDS2 `func_00116B58`: predicted, matched first
   try). This is a derivation from the allocator, not a search.
6. **Don't name a pointer the original only dereferenced.** A `next = block->block;`
   local is a user-variable pseudo (`user var` in the `-dl` listing) allocated
   in global-alloc with its own priority, so every other counter shifts one
   register. Writing the loop as `for (; block->busy != 2; block = block->block)`
   with `size = block->block->address - block->address;` leaves the loaded
   pointer to cse (an expression temp the allocator places next to `size`) and
   gives retail's counters `$12/$13/$9/$7/$11/$10`. DDS1 `func_002D0B50` / DDS2
   `func_00329A00` (heap statistics; 28 of 41 words differed with `next`,
   0 without). Before trying declaration orders on a register-numbering-only
   miss, check whether a named pointer local is one retail would have left as
   an expression.
7. **Cursor iteration is a `for` over the cursor's own return value.** DDS2
   `dds3VisitWorldObjectValues` / DDS1 twin: `for (more = Reset(object);
   more != 0; more = Advance(object)) { if (callback(Read(object)) == 0)
   return 0; } return 1;` matches. The same logic as `if (Reset() == 0)
   return 1; do {...} while (Advance())` leaves the empty-list exit jumping
   into the loop test (3 words off) because jump2 merges the `return 1`
   blocks differently.
8. **A value assigned in both arms is not the same as preset-then-override.**
   DDS2 `func_001A7A08` / DDS1 `func_0019F9E0`: `if (a & 2) step = -1; else
   step = (b & 2) > 0;` matches; `step = -1; if (!(a & 2)) step = ...` gives
   the same branches but loses retail's re-materialised base address
   (`addiu $3,$6,%lo(sym)` after the first test) and the delay-slot fill.
   `(x & 2) > 0` is also how retail's `sltu $5,$0,$2` arises; `!= 0` gives
   `srl/andi`.
9. **Fixed-stride arrays in a global: index the typed array twice.** DDS2
   `func_0026D4C8`: retail computes the entry's flag word from `state +
   i * stride` with the array offset folded into the displacement
   (`lhu 0xA60($2)`) and the entry pointer separately (`addiu $17,$4,0xA60`).
   `((State *)g)->party[i].flags & 1` in the test and `slot =
   &((State *)g)->party[i]` inside the branch reproduces both; taking
   `slot` first and reading `slot->flags` gives one address and 37 words of
   difference. (`func_0026BC80` shows the plain `(Slot *)(g + i * stride +
   off)` form matches when only one address is used.)
10. **Declare an ignored-result call with the callee's real return type.** A
    call to a function that returns a value (`s32 itfEnqueueMemNode(void *,
    MemNode *)`) declared `void` in the caller's unit changes the temp
    registers around it: DDS2 `func_0019C4D0` / DDS1 `func_00194840` (frFont
    glyph chain release) had every structure right and the two counter
    decrements in `$2` (6 words); `extern s32 itfEnqueueMemNode(void *node,
    s32 pool);` gives retail's `$3` and a full match. Before touching
    registers, check each callee's definition (the `grep` for its real
    definition is one command) and copy its return type.
    More cases found by re-running every park with each callee's declaration
    corrected one at a time: the debug printers (`func_003003F0` in DDS1,
    `func_0035B6E0` / `func_0035C860` in DDS2) return `s32`; declaring them
    `void` cost DDS1 `func_002241D0` / DDS2 `func_0023EE08` (a result-ignoring
    printf tail after an arg-fetch call) their full match and made DDS2
    `sndPollAtrac3SELoadTask` a CONTEXT flip. Correct only the callee the
    failing function calls: correcting every `void` prototype of a unit at
    once broke functions that had matched.
    This can also explain a one-register shift *after* an ignored-result call.
    A wrongly value-returning declaration can create a dead `$v0` result whose
    live range overlaps the next local-allocation quantity. The allocator then
    skips `$v0` and first-fits that quantity in `$v1`. In DDS1
    `prfBuildRawSkillList`, an implicit-`int` declaration of `memset` keeps
    `$v0` live across the row-index quantity; the unit's `extern void memset()`
    removes that result and reproduces retail's four-word `$v0` index chain.
    In DDS1 `btlUnitFadeInTask`, declaring the genuinely `void`
    `func_00221EF0` and `mdlBroadcastMasked` as value-returning similarly swaps
    the `$v0`/`$v1` quantities used by the flag update; their real `void`
    declarations restore the retail assignment.
    Therefore, when a near-match shifts one temporary from `$v0` to `$v1`
    immediately after an ignored-result call, verify that callee's definition
    before trying declaration-order permutations. This is a targeted check,
    not a reason to rewrite unrelated declarations.
    Three more independent DDS1 examples are `func_0018AB40` (effMagatuhi),
    `func_00280E08` (code_0027BF00), and `func_00288E70` (code_00288E70).
    Their genuine `s32` callees are `effMathStepBezierSlot`,
    `frFontQueueGlyphInSelectedSlot`, and SDK `WaitSema`, respectively.
    Each `void` declaration leaves exactly two incorrect words: the post-call
    comparison/load and branch use `$v0` instead of retail's `$v1`. Correcting
    that one return declaration gives whole-unit checks of **35 match, 0 differ**,
    **110 match, 0 differ**, and **18 match, 0 differ**. The first two callees
    have matching C definitions; `WaitSema` is also declared `s32` by the
    kernel SDK and the file-manager units.
    An RTL probe of `func_0018AB40` isolates the first difference to the call:
    the corrected declaration adds `set (reg:SI 2 v0)` around its call result.
    Allocation order stays unchanged, while comparison pseudo `r101` moves
    from `$v0` to `$v1`. This is another application of this existing idiom,
    not evidence for declaration-order shuffling or an invented return type.
    The same error can change late tail merging, not just two register words.
    DDS1 `func_00232E20` / DDS2 `func_0024DBB8` ignore the genuine
    `PolyMovieWork *` result of `evtPolygonMovieInitWork` before `flags |= 8`.
    With a false `void` declaration, the final `lw/ori/sw` uses `$v0` and merges
    its store with the three asset-cleanup arms. The correct pointer result
    produces retail's separate `$v1` update; the polling function itself remains
    `void`. Correcting only that initializer result fixes the DDS1 **52/90-word**
    residual. Reusing the existing `EvtWindowContext` asset prefix gives
    **47 match, 0 differ** and **50 match, 0 differ** for the two current units.
    `filePollEntryCleanup` also has its genuine `s32` declaration, but an
    independent control with that call still declared `void` remains exact:
    the initializer return is the decisive change.
    A currently matching `void` C definition is not conclusive negative evidence.
    Both games' `sdfAllocAndClearQuadwords` actually forward the buffer returned
    by `sdfClearQuadwords`; both formatted-packet wrappers return the cursor
    saved by their assembly helper and consumed by real packet-append callers.
    Giving those producers genuine `void *` results preserves **4 match, 0 differ**
    for each allocator unit and **78 match, 0 differ** for each packet unit.
    Do not drop an evidenced pointer result, or invent a wide wrapper result,
    merely because either declaration can reproduce the same bytes.
11. **`x / 5` always compiles with the zero check, and a source-order trap.**
    ee-gcc emits `addiu $2,$0,5; div; beql $2,$0,1f; break 7` even for a
    constant divisor, so the check is not evidence of a variable divisor
    (checked on a scratch file). In DDS1 `func_002E41B8` (GIF tag closer) the
    stores that depend on the quotient must follow the division expression
    in source order (tag header store first, then the buffer fields):
    putting the buffer stores first makes sched1 issue them between `div` and
    its check (14/28 words), the tag-first order gets to 5/28.
    Independent work is different. In the paired title-stream setup helpers
    `func_0026ABA8` / `func_002A27A8`, the shapes `q = n / d; zero = 0;
    result = q;` and `result = n / d; zero = 0;` differ at pass 00, but sched1
    converges both forms: the zero store fills divider latency and the quotient
    store waits for the result.
    Both forms produce the same exact assembly. If a controlled source-order
    or temporary-local test converges by pass 17, stop permuting those forms;
    only a real dependency can constrain the schedule.
12. **`bne` with a filled slot vs annulled `bnel`: the callee must be C-defined
    earlier in the same unit.** `if (a >= 200) return; if (b == 1) f();` (jal
    tail, `ld $31` slot) compiles to `bnel`/`ld ra` when `f` is only declared
    or is an `INCLUDE_ASM` function, and to retail's plain `bne` + `ld ra` slot
    when `f` is a C definition earlier in the same translation unit (scratch
    test: declared `void`/`s32`/K&R, defined later, or any other function
    defined earlier: all `bnel`; only the callee defined above the caller
    gives `bne`). DDS1 `fldCheckSceneReady` (1 of 15 words) and the
    effEvent `func_00190708` residual are this: they stay one word short until
    the callee (`func_001462D8`, 429 asm lines) is itself decompiled. Do not
    fight it with source shapes; park it as "blocked by callee" and revisit
    when the callee lands. Not every plain-`bne` residual is this: DDS2
    `func_0021B4C0` calls `btlGetRuntime`, which lives in another unit.
13. **Give accumulator locals and fields their real pointer type, not `void *`
    or `s32`.** Load/store ordering in sched1 depends on type-based aliasing.
    DDS1 `func_002EFC68` / DDS2 `func_00348B10` (pool chain rebuild) sat at
    2 of 25 words ("`lw $4,0xC(pool)` after `sw $0,0($sp)`") with the two
    accumulators written as `s32` and then `void *`; typing them
    `SdfPoolNode *` (the chain really is pool nodes: the callee follows
    `node->next`, the pool fields `head/tail` become `SdfPoolNode *`) gave a
    full match, and the callees `func_002EFB88` / `func_00348A30` matched at
    the same time. Also unify duplicate views of one structure: the key-tree
    walker used a private `SdfKeyTreeNode` while the unit already has
    `SdfTreeNode`; `pool->sub` became `SdfTreeNode *sub[1]`.
    A 1-3 word residual in a function whose locals or fields are typed
    `s32`/`void *` is a type problem before it is a scheduling problem.
14. **A symbol address used before and after a join (loop, `if`/`else`): three
    shapes, and which source form gives which.** Scratch matrix (symbol `A` as a
    call argument, then a loop that walks it; relocations counted):
    - Only calls between the two uses, or a one-armed `if`: one `lui`/`addiu`
      pair held in a callee-saved register (whole address shared).
    - A loop or an `if`/`else` between them and the second use written through a
      **pointer local** (`p = (T *)A; ... p->x`): `lui` shared in a saved
      register and `addiu %lo` re-derived at the second use (`hi lo lo`). This is
      retail's "`lui $19` kept across the loop, `addiu $3,$19,lo` for the remainder"
      (DDS1 `func_00101368`, `func_002878D8`, `func_0016FC58`).
    - The same join with the second use written as an **indexed access of the
      symbol** (`((T *)A)[i].x` in the loop body, no pointer local): the whole
      pair is rematerialised (`hi lo hi lo`), nothing held in a saved register.
    So match the retail shape by choosing the second use's form. DDS2
    `func_00122828` (field table loader; the seventeenth read passes `D_00389170`,
    a loop later walks it): `record = (FieldStageCoordinate *)D_00389170` kept the
    address in a saved register (0x20 frame instead of 0x10);
    `((FieldStageCoordinate *)D_00389170)[i].x` gives retail's frame and a full
    match (with two separate `if (id > 0) { if (id < 0x1F)` tests, since
    `id > 0 && id < 0x1F` folds to one unsigned range compare). A saved register
    can also be absent for another reason: if retail keeps other values in the
    callee-saved registers (slot addresses instead of re-read handles) the
    address pseudo loses the allocation and is rematerialised; that is a
    liveness question, not an address-form one. Volatile `asm` between the uses
    does not break the sharing (tested).
    **Switch with the symbol used in every arm** (4 arms, field accesses and a
    call each): `W *w = &G;` initialised before the switch and used in every arm
    gives ONE `lui`/`addiu` in the prologue held in a saved register for the
    whole function (retail DDS1 `func_002890B8`: `lui/addiu &fileManagerWork` in
    $19 before the switch). Writing `G.a`, `G.b` (direct field accesses) or
    assigning `w = &G` inside each arm rematerialises per arm (4 pairs). A
    self-assigned `FileManWork *work = work;` placeholder, as in the old
    `func_002890B8` park, gives neither: it never reads the symbol.
15. **`j callee` (tail call) is lost when an early `return;` skips the call; and a
    "both or neither" test is spelled out.** A void function whose last
    statement is a call is compiled to `ld regs; j callee` (sibling call) only if
    no `return;` jumps over that call to the epilogue (the early `return;` gives the
    epilogue a second incoming path and the call stays `jal`; tested with a
    one-call body plus one early `return;`; a `return;` AFTER a call, as in switch
    arms, is harmless, see the switch matrix below). Retail's
    `j btlAppendIndexListEntry` in DDS2 `btlQueueLoneFreeTeamHandle` (`0x00220B20`) therefore means the whole
    body is nested in `if`s, never `if (...) return;`. Same function: "exactly one
    of two flags is clear" matched only as
    `if (!((a == 0 && b == 0) || (a != 0 && b != 0))) { x = (a == 0 ? first : second); ... }`
    (the select is emitted once after the join, as `movz`); `(a == 0) != (b == 0)`,
    `a ? b == 0 : b != 0` and the positive `(a == 0 && b != 0) || (a != 0 && b == 0)`
    each differ by 3-40 words. Also: a local that is initialised only inside the
    guarding `if` (not at its declaration) lets gcc fill the `beqz` delay slot with
    the first initialisation, as retail does.
    The current complete-owner source uses `BattleLinkedEffectState.actor`,
    `BtlUnit.partyRecord.unitId`, and `ActionStateLink.indexWork`; selecting
    the first unit using its own unavailable flag gives the native `movz`.
    The 280-byte target is exact and its whole action unit gates **318 match,
    0 differ**, without a second unit view or raw state offsets.
    Switch matrix (5 arms + default, scratch): arms written `f(); return;` give `j`
    for every arm including a fall-off default, whatever the caller's and callees'
    void/int types; with `break` arms an `int` or implicit-int caller makes EVERY
    call `jal` (a void caller still `j`); a `default:` written FIRST (source order
    first) with `break` and the others `return;` gives `jal` for that arm only
    (`default_first`: jal, j, j, j, j), but retail emits the default last, so that
    does not reproduce DDS2 `func_001525F0` (jump-table, cases `j`, default `jal`);
    default last, `case 3` sharing the default, or `return;` after it all give `j`.
    Still open: how retail gets a trailing `jal` default after `j` cases.
16. **`addu` operand order follows how the element is reached.** A table element
    taken as a pointer, `entry = &entries[i]; entry->a ...`, is emitted
    `addu base, idx<<3` (base first); the same element accessed directly,
    `entries[i].a`, gives `addu idx<<3, base` (index first), which is what retail
    shows when every `addu` in the function has the index first (DDS2
    `func_00110240`: 20 differing words -> 7 after replacing the `entry` pointer
    by direct `entries[arg->unk4].field` accesses). Same function: the two-way
    free-list push matched as `if (tail < 0) { head = x; } else { ... }`
    (`bgezl` with the else block first), not as `if (tail >= 0) {...} else {...}`.

Unresolved: a saved register initialised as a copy of another holding the same
constant (`move $16,$19` for `i` from `bestIndex = 0`, DDS1 `func_00202F90`,
`func_00203BA8`, DDS2 `func_00216888`, `func_00215C70`, `func_002CBA90`). cse
always folds a plain `i = best` to the constant (a CONST_INT costs 0). The copy
survives only when `fold_rtx` reduces an if-converted select with a known
condition to one register arm; the matching test shapes all had a dead second
arm, which is a lever, so these stay asm until the real construct is found.

Two related mechanisms are known:

- **A `move $3,$2` right after a call, with no `move $2,$3` before the
  return, means the function returns the call result.** cse deletes the final
  `v0 = slot`, but the `(use v0)` at the end keeps `$2` live through the whole
  function, so the result variable conflicts with `$2` and gets `$3`. A
  function typed `void` that shows this is really `return slot;` (DDS2
  `func_0031DFB8`).
- **A plain copy never survives inside one extended basic block.** When a
  pseudo is copied from another, cse makes the one that lives longer the
  canonical register and rewrites the other's later reads to it, so
  `u32 result = param;` in any placement collapses into one register. A
  retail copy needs the two values' uses separated by a block boundary (a
  label with two predecessors) or the older value used later than the copy.
- **`addu $5,$3,$2; daddu $3,$5,$0` (one address in two registers) comes from
  an array embedded in a pointed-to struct.** For `table->entries[i].a` and
  `table->entries[i].b` the front end adds each field offset to the base
  first, so cse1 sees two different sums; combine later folds the offsets into
  the displacements and `reload_cse_regs` turns the duplicate sum into a move.
  A pointer member (`Rec *records; records[i].a`) gives one register instead.
  DDS2 `func_00231618` / DDS1 `func_00216B00` (also `btlClearActorEntrySlot`,
  `btlActorEntryIsExpired`).

### FP registers: local-alloc before global-alloc

A pseudo born and dying once inside one basic block is a local qty, allocated
by local-alloc before global-alloc (even across calls); local priority is the
same formula, each qty takes the lowest free FPR. `t = a * b` ties `t` to the
first dying input's qty. sched1 runs first, so an independent `mtc1`/`cvt`
is born early (long life, low priority). A value that must land *above* a
competitor needs the competitor to be a function-scope multi-block variable.
Dumps: `cc.sh -dl -dg` on a scratch copy; `.greg` shows `regs to allocate: ...`.

1. **Global scale, local temps.** DDS2 `func_0027C2F0` (+4 twins): function-scope
   `f32 scale;`, each arm `scale = field * 0.25f;` in one statement, then
   `scale = 1.0f - scale;` separately. Not `f32 scale = field; scale *= k;`.
2. **One variable per role.** DDS1 `func_00190D18`: a variable reused for two
   values is one global pseudo; `scale` and `range` as two locals each get `$f20`.
3. **Reload, don't cache across a call** when retail reloads the field after the
   `jal` (a cached copy costs one more callee-saved FPR).
4. **Repeat the constant per arm.** DDS1 `func_00162088`: `height = -1.0f` on
   both sides of the `||` (retail loads it twice) gives the extra refs that order
   `length`/`height`.

### Float literals: spelling decides the last mantissa bit

A float read as `lwc1 %gp_rel(D_xxxxxxxx)` is a `.lit4` pool constant (check_unit says
"is a .lit4 pool constant: write the float literal" and prints the expected bits).
cc1 2.96 rounds decimal literals so that adjacent spellings differ by one ulp:
`1.5707962f` is 0x3FC90FD9 but `1.5707963f` is 0x3FC90FDA; `6.283185f` is
0x40C90FD9, `6.28318548f` is 0x40C90FDA, `6.2831855f` is 0x40C90FDB;
`0.7853981f` is 0x3F490FD9; `0.08726646f` is 0x3DB2B8C1. Do not guess from the value:
compile a scratch file with several spellings (`tools/cc.sh -DSKIP_ASM x.c -o x.o`)
and read `.lit4` with `objdump -s -j .lit4 x.o` (DDS1 `func_001725F0`, effPCPScatter).

### FP register numbers in call-free blocks follow the sched1 order

When a block without calls differs from retail only in FP register numbers
($f1/$f2 swapped, a constant in a different register), do not hunt for a
declaration order or a local that renumbers: local-alloc orders the pseudos by
priority (refs / live length, shorter lives first, ties by pseudo number), and the
live lengths come from where sched1 put the insns. DDS1 `func_0010F9A8`: retail has
`lwc1 limit; abs.s; mtc1 0.0` and gives the zero `$f1`, limit `$f2` (zero's live
range is shorter, so it is allocated first); ours has `abs.s; lwc1 limit; mtc1 0.0`
and the tie goes to the lower pseudo (limit `$f1`, zero `$f2`). The root cause is the
insn order, and that order is `prio` in the `-dS -fsched-verbose=5` dependence table
(`prio` = longest latency path to the block end: `abs.s` 13 against `lwc1` 11, so
`abs.s` issues first; the higher prio is picked first, and only an equal prio falls
back to source order). Reordering or renaming declarations never changes a prio, so
such residuals (DDS1 `func_0010F9A8`, `func_00152560`, `func_0017FD30`,
`func_001725F0`, `func_00205730`) only match when the expression shape changes the
latency chain, e.g. one more `add.s` in the limit's path that lifts the load's prio
above the abs (the sibling `dds3TestObjectSphereOverlap` has such an add and gets
retail's `lwc1, abs.s, lwc1` order). Check the `prio` column before trying variants.

### `la $rd,sym($rs)` (`lui $1; addiu $1; addu`) and constant register order

- cc1 always prints an indexed table address as the macro `la $rd,sym($rs)`;
  the assembler expands it to `lui $rd,..; addiu $rd,..; addu $rd,$rd,$rs`,
  but when `rd == rs` it needs the temporary and prints `lui $1; addiu $1; addu
  $2,$1,$2` (retail DDS1 `func_002D0E30`). The pointer therefore has to be
  allocated into the offset register, which happens when it is live across a
  branch and the field loads go into locals assigned after the branch (so the
  loaded temps do not need `$2`): `extern u8 tbl[]; entry = (E *)(tbl + i * 12);`
  first, then `if (c) { ...clear... }`, then `x = entry->x; ... result->x = x;`.
- Hoisted constant stores (packet words `packet[k] = 0x...`) take callee-saved
  registers in order of local-alloc priority, and priority falls with live
  length: the constant with the longest live range gets the highest register.
  Writing its store first in the first block and last in the last block makes it
  `$22` instead of `$18` (DDS1 `func_0014F860`).
- `xori $r,$r,0` ahead of `movz/movn` is cc1's `x != y` with `y` folded to 0 only
  after expansion. `(m & 0x2000) != 0`, `?:`, `if`, `== 0`, u64/u16/s16 params
  never produce it (15 scratch spellings, `btlLowestSetPairIndex` tail).

### Alias sets stop gcse merging a reload

gcse refuses to merge two MEMs with different alias sets, so a typed field access
and a differently typed access to the same location stay two loads; two typed or
two raw accesses are merged even across a loop. A retail reload after a loop
therefore means the two accesses had different types. Acceptable only when the
codebase already has a second struct view of the object (DDS1 `func_00277CB8`:
`MenuSelectionState->list` for the walk, `((MenuInputNode *)state)->flags` for
the seek argument). A raw cast next to a typed access of the same field is a
lever (DDS2 `func_002B40F8` stays INCLUDE_ASM).

### Declared types change scheduling dependencies (alias sets, readonly)

sched1/sched2 add dependencies between MEMs from their alias sets and
`/u` (unchanging) flags, so a wrong declared type can swap two independent
loads or stores even when every instruction is right:

- A table that really lives in `.rodata` is `const`: the `const` extern makes
  its loads unchanging, the false anti-dependence on a later store goes away,
  and sched2 picks the other load first (DDS1/DDS2 `bfOpWaitDispatch`:
  `extern const ScrCommand D_0039E288[]`; mutable spelling swaps two loads
  around the `bnel` slot).
- A flag word that is really a bitfield in a struct with callback-pointer
  members takes the aggregate's alias set, which orders the flag store
  before the callback-table load (DDS2 `func_00317FE0`); a scalar `u32 flags`
  lets the load move up.

Only fix types the data really has (rodata placement, a bitfield the code
tests bit-by-bit). Adding views to steer alias sets is the lever above.

### Access through a union versus its actual member type

An access whose `COMPONENT_REF` chain passes through a union member receives
alias set zero in this compiler. That conservative view can prevent PRE from
merging loads and add memory dependencies in sched1/sched2. Accessing the
same active payload through its actual struct type can give its fields their
ordinary scalar alias sets.

The DDS2 linked-effect callbacks provide a confirmed example:

```c
BattleLinkedEffectState *linked = &ctx->effect->linked;
linked->actor = unit;
```

This uses the existing member's address with no cast or alternate layout.
`btlBindEffectUnitAndClearStateFlags` and `func_00226C98` match with that
payload access. A direct `ctx->effect->linked.actor` access has a different
dependency graph. Check the active mode, allocation and other field users
before taking such a pointer; the union remains the owner of heterogeneous
payloads.

Alias set zero does not force dependencies between all accesses: known
disjoint locations can still be separated by address analysis. Conversely,
an integer placeholder for a real pointer can manufacture a dependency even
without any union. Recover the field's producer/consumer contract rather
than adding raw cast views, unions, or `volatile` to request a schedule.

### Float constants never in a delay slot

Neither ELF has `mtc1 $1,$fN` or `mtc1 $0,$fN` in a branch slot: `li.s` is a
`.set reorder` macro and `as -g` keeps it where cc1 put it
(retail `li.s $f12,K; jal f; nop`, DDS2 `func_001ECC18`).

### Preserve the vector type of a local coordinate result

When related vector helpers reserve a 16-byte local area and save the first
computed coordinate at its component offset (`X` at `sp+0`, `Y` at `sp+4`),
use the existing vector type for the result. Do not flatten that object into
independent float locals just because only two coordinates are needed:

```c
/* Scalar reconstruction: the first result stays live in an FP register. */
f32 y, z;
y = vector[1] * cos(angle) + vector[2] * sin(angle);
z = vector[1] * -sin(angle) + vector[2] * cos(angle);
vector[2] = z;
vector[1] = y;

/* Existing canonical type preserves the coordinate object's storage. */
SdfVec4 result;
result.y = vector[1] * cos(angle) + vector[2] * sin(angle);
result.z = vector[1] * -sin(angle) + vector[2] * cos(angle);
vector[2] = result.z;
vector[1] = result.y;
```

Controlled scalar-versus-vector copies of DDS2 `func_00325EC8`,
`func_00326018`, and `func_00326158` differ in 18/83, 18/79, and 18/83
compared words respectively with scalar locals (`0x30` frame, first result
in `$f20`). All three are exact with the canonical `SdfVec4` local (`0x40`
frame and the retail coordinate stack slot); the whole unit ends
`26 match, 0 differ`. Standard `double sin(double)`/`cos(double)` and
ordinary expressions are sufficient; explicit compiler-helper calls or
invented integer libm prototypes are not the source idiom.

The original local declaration is inferred from those component offsets,
the surrounding helpers' existing vector type, and the controlled result.
This is not permission to invent an alternate view, add unused padding
variables, or replace every scalar temporary with an arbitrary aggregate.

## Not allowed

These are fakes, and check_unit reports them as `TRICK`:

- computed gotos and label-address tables standing in for a switch;
- `register x asm("$n")`;
- asm used for anything but COP2/MMI under the rules in "Inline asm: COP2 and
  MMI" (check_unit reports mostly-asm functions as `ASMBODY`);
- dummy variables or `volatile` added only to steer codegen.

Reusing one local for several values is fine when a single type and a neutral
name fit every use (`i` across loops, `t`/`factor`/`n` for successive
calculations), as C89 code with all declarations at the top often does. It is a
fake when the reuse only reads correctly under a misleading name (a `column`
counter holding a row bound), or when the variable exists only to steer
register allocation.

## Pure two-word swaps: identify the decisive scheduler key

A residual consisting of two exchanged instructions is a phenotype, not a
single source idiom. The same final diff can originate in readonly memory,
native pointer fields, or a real return-value lifetime. It does not justify
declaration-order searches or adding a value that the function does not need.

The following corpus comes from existing parked notes. Some entries have
already been resolved in production; an old park is not evidence that its
function still needs decompilation. Paths below are relative to `build/parked`.

| Game | Function(s) | Historical exchanged pair | Park |
| --- | --- | --- | --- |
| DDS2 | `func_001552E0`, `func_00155690` | `a0 = sp` / `a2 = v0`, `+0x70/+0x7C` | `dds2/game/code_00154558/func_001552E0_Fam_Mid.c`, sibling `_Fam_Mid.c` |
| DDS1 | `bfOpWaitDispatch` | command parameter count / VM stack pointer, `+0x54/+0x60` | `dds1/script/scrTraceCode/bfOpWaitDispatch_P3_Med3.c` |
| DDS2 | `bfOpWaitDispatch` | same two loads as DDS1 | `dds2/script/scrTraceCode/bfOpWaitDispatch_P3_Med3.c` |
| DDS1 | `func_00199828` | allocation-pointer store / kind-byte store, `+0x3C/+0x40` | `dds1/game/code_00196478/func_00199828_W16_Snd.c` |
| DDS2 | `func_001A1858` | corresponding sprite-constructor stores | `dds2/game/code_0019E138/func_001A1858_P8_FreshA.c` |
| DDS1 | `func_002BA058`, `func_002BA0C0`, `func_002BA170`, `func_002BA230`, `func_002BA2D0`, `func_002BA3C8`, `func_002BA498` | epilogue `ld s0` / `ld s1` | `dds1/game/code_0029A840/func_002BA058_family_W16_Snd.c` |
| DDS1 | `func_002D5CD0` | resource-header / packet-source loads, `+0xA8/+0xB0` | `dds1/game/code_002D33C8/func_002D5CD0_P2_Med2.c` |
| DDS1 | `func_0013D650` | return-address save / actor-task flag store | `dds1/game/code_00126A30/func_0013D650.c` |
| DDS2 | `func_00140238` | same pair, `+0x18/+0x1C` | `dds2/game/code_00136EF8/func_00140238_P1_Med0.c` |
| DDS2 | `func_00219BD0` | flags / motion-parameter stores, `+0xBC/+0xCC` with native types | `dds2/game/code_002112C8/func_00219BD0_M86.c` |
| DDS2 | `func_002F6A80` | entry argument / path-work pointer loads, `+0x88/+0x90` | `dds2/game/code_002DE248/func_002F6A80_Fam_Tail.c` |
| DDS2 | `func_002DFAB0` | address high half / stack-address setup, `+0x114/+0x118` | `dds2/game/code_002DE248/func_002DFAB0_Fam_Tail.c` |

This is nineteen functions, not nineteen independent fixes. In particular,
the seven mapping wrappers now live in `src/dds1/game/code_0029C530.c`;
the historical park directory predates the current TU boundary.

### Use the actual gcc 2.96 comparator

The shipped `cc1` has `rank_for_schedule` at host address
`0x0816C6AC`. Its descending preference order is:

1. Greater critical-path priority.
2. Before reload only, smaller register weight.
3. Interblock target/speculation/probability preferences, where applicable.
4. Dependency class relative to the last-issued instruction: independent
   (or cost-one) before costly anti/output dependence before costly true
   dependence.
5. More forward dependents.
6. Earlier logical instruction UID.

Thus sched2 does not use the register-weight tier. A different load/store unit
or floating-point register class is not an additional comparator key. Consult
the actual ready list and dependency edges before blaming the final UID tie.
`tools/ee_gcc_probe.py --cflag=-fsched-verbose=5` exposes these in `17.sched`
and `25.sched2`; `29.dbr` shows subsequent delay-slot donation.

Register weight is read from the scheduler's input RTL: register outputs add
one, `REG_DEAD`/`REG_UNUSED` notes subtract one. The printed dependency table
does not contain that weight. Its fields alone cannot replay the comparator;
`ee_gcc_schedules.py` deliberately calls that partial view a heuristic.
Also distinguish logical input order from printed table order and account
for interblock preferences before attributing a choice to a final tie.

Basic-block boundaries matter even when final assembly has no branch there.
Cross-jumping runs after sched2; genuine duplicated branch tails can be
scheduled as separate blocks and merged afterwards. The paired
`evtViewerPickNextHandler` is a reference for the resulting constant-address
versus epilogue-load order. Recover actual control flow; do not insert an
empty condition or identical-outcome branch solely to reset the scheduler.

### Two readonly-table controls

Both games' existing `bfOpWaitDispatch` are exact with an `extern const
ScrCommand` table. Removing only that `const` in private snapshots reproduces
exactly the two `+0x54/+0x60` loads, leaving the other 37 words identical.
The unmodified production script units each pass `41 match, 0 differ`.

In the DDS2 sched2 success block, both loads have priority 4. With the mutable
table, instruction 70 (parameter count) and instruction 68 (VM stack pointer)
each have three forward dependents, so the earlier UID wins. The readonly
`mem/s/u:SI` removes 70's anti-dependence to instruction 74's stack-pointer
store: 70 then has two dependents while 68 still has three. The forward-count
key now chooses the VM load first. Delay-slot donation turns this into the
retail success-branch slot. This is a real rodata/alias fact, not an instruction
order requirement to encode in C.

### Three return-liveness controls

`func_002BA170`, `func_002BA230`, and `func_002BA498` have ordinary `s32`
returns that forward `func_002B9320`'s result after restoring the mapping
table. Private controls changing just those wrappers to `void` and ignoring
the result each reproduce exactly two swapped epilogue loads: `+0x4C/+0x50`,
22 of 24 words identical. The combined negative control gives
`516 match, 3 differ`; after a stable resplit the positive snapshot passes
`519 match, 0 differ`, with no PAD or other diagnostics. These wrappers were
already C before this experiment; this validates a cause, not a new landing.

For `func_002BA170`, the positive sched2 has a legitimate `USE v0`
(instruction 85) after the last table-pointer store (66). It emits no extra
machine word, but becomes the last-issued instruction before restoring
callee-saved registers. The `s0` and `s1` loads have equal priority 4 and two
forward dependents; relative to that USE they tie, and the earlier `s0` UID
wins. In the void control, the last-issued instruction is the table-pointer
store (61). The `s0` restore has an anti-dependence on it, while the `s1`
restore does not, so the dependency-class tier chooses `s1` first.

The source fact is the wrapper's real result contract, not a dummy local or
an epilogue-order rule. Recover it from the provider and callers; do not
change a genuinely void function to return an unused value merely to move
restores.

### The numeric-room reference remains unresolved

DDS2 `func_001552E0` and `func_00155690` are each 440 bytes without padding.
The honest candidates still have 108/110 exact words, and their combined
private TU passes `95 match, 2 differ` with no other diagnostics. In
`func_00155690`'s sched2 formatter block, instruction 108 (`a0 = sp`) has
priority 10 and **five** forward dependents, while instruction 112
(`a2 = v0`) has priority 10 and **four**. The former wins before the logical
UID tie, despite its later position after sched1. The last `a2` move is then
donated into the formatter's `jal` slot. Retail instead donates `a0`.

Separating the numeric input from the parsed room's lifetime, combining the
parse-and-apply expression, and using the SDK's actual `sprintf` spelling
with its `int (char *, const char *, ...)` prototype all retain that pair.
The immutable format declaration was already present. None supplies the
missing original-source fact that would change the dependency graph.
Do not generalize the readonly or hidden-return fixes to this stack buffer:
both reference functions remain `INCLUDE_ASM`. No declaration enumeration,
invented wide return, compiler-flag change, or artificial dependency was
used to turn this unresolved case green.

## Branch-likely

### Result helpers and controllers share compilation ownership

DDS1 `func_00268AB8` (644 bytes) and DDS2 `func_002A05C0` (788 bytes)
each had a sole BEQL/BEQ difference. Their real preceding increment helpers,
`func_002687C0` and `func_002A0278`, supply the same-unit nothrow information
when retained with their controllers in `code_002665E0` and `code_0029DF18`.
Restoring that contiguous ownership matches both complete controllers.

The earlier no-sibling-call boundary inference was refuted independently.
All four detected DDS2 late-call tails pass live local palette buffers to the
draw provider. Those escaped stack addresses themselves prevent a sibling
jump; a sequence of JAL tails does not establish a compiler option. The eight
previous DDS2 result bodies remain exact with sibling optimization enabled.
Complete merged source/data checks are DDS1 73/0 and DDS2 82/0, and both
retail images remain byte-identical after removing the unnecessary flags.

Use the actual helper definition and audit every affected body and section.
A visibility stub or isolated exact flag probe cannot establish ownership.
Verify the effective compiler options, too: `tools/cc.sh` appends unit options
after extra options, so an extra option can be overridden by the unit's flag.

The `L` bit describes delay-slot execution, not the source comparison:
`beq`/`bne`/`bc1f` execute their slot on both paths, whereas their likely
forms annul it when the branch is not taken. Do not infer `==` versus `!=`,
or a ternary versus an `if`, from that bit alone.

Use `tools/ee_gcc_probe.py` with the unit's canonical `--as-unit`, then
`tools/ee_gcc_delay_slots.py` on the probe directory. First establish that
the diagnostic assembly reproduces the ordinary `check_unit.py` result.
In `29.dbr`, `/u` on the jump marks annulment; `/s` on its donor marks a
donation from the branch target. The tool reports the donor UID and whether
it executes always, on the taken path, or on the not-taken path. Compare
`28.mach` before attributing a difference to delay-slot filling: a different
CFG or register assignment is a different experiment.

A matched nullable-pointer example is DDS1 `func_002BE4B8` (624 bytes,
`game/code_002BC8F0`) and its DDS2 twin `func_00305C40`. Their natural
`if (color == NULL) { defaults; } else { color->rgba; ... }` becomes a
`bnezl` with the first color-word load in its taken-only slot. That load
must not execute for the null/default arm. Both complete units passed
`check_unit.py` after their C ports; no assembly or branch annotation was
needed.

The compiler's `fill_slots_from_thread` also tests the candidate's effects
against the opposite thread's needed registers and whether it may trap.
Thus an ordinary target donation can be legal when the other path
overwrites its result before any use. The same-TU `TREE_NOTHROW` /
`REG_EH_REGION 0` mechanism described above can change that liveness scan;
it is evidence to recover the real TU, not permission to hide a callee or
invent a nothrow attribute.

An unresolved counterexample is DDS2 `func_0031CBC8` (544 bytes). Retail
has `bc1fl` at `+0xD8`, with `andi a1,a1,255` in the slot; the honest
candidate has `bc1f` with that same slot and destination (135/136 exact
words, not an objdiff percentage). The branch is generated by the unsigned
float-to-integer conversion, not by a literal game-code `if`. In the
candidate's `28.mach`, the opposite large-value conversion overwrites
`a1` before using it; `29.dbr` donates the byte mask from the target
(jump UID 100, donor UID 122), without annulment. Branch-local const
calculations introduce four additional FPR differences; combining the
real eligibility guards, or using a loop `continue`, preserves the same
one-word residual. Arm spelling alone did not recover the missing fact.

A separate three-function conversion control at the normal `-O2 -G8`
showed that direct `f32`-to-`u8` and `(u32)value`-then-byte conversion
both emit the unsigned 2^31 correction path. A signed-word intermediate
removes it, but changes the conversion's defined range. Do not substitute
a signed conversion, manually spell the compiler's correction algorithm,
or change an API/return width merely to obtain the desired annulment.
Keep a one-word miss parked until a genuine type, liveness, inline, or TU
fact explains it.

A real-body visibility control makes the TU mechanism concrete for DDS2
`fldDestroySceneTasksAndBuffers` (`0x001CFCA8`, 592 bytes,
now `game/code_001B2AF8`). The normal twin-port candidate has just
`beql` versus retail `beq` at `+0x1B8` (147/148 words; `51 match, 1 differ`).
Putting the actual existing `btlDestroyTaskD` body before that caller in a
private diagnostic TU yields `53 match, 0 differ`, including the provider
and all surrounding C. After normalizing block addresses and label
ordinals, `28.mach` differs only by `REG_EH_REGION 0` on call UID 443.
Both `29.dbr` files donate the same global-panel load, UID 449, into jump
UID 435: taken-only for the opaque provider, always for the visible one.
The final assembly differs only in that branch mnemonic. The TU was then
authenticated: the short string literals `"%3d"` and `"%s"` are each
emitted once and used on both sides of the former `code_001C35F0` and
`code_001C7FF8` splits, so both belong to `code_001B2AF8`'s file. With the
units rejoined, the function matches as written.

Check API contracts before treating a park as one-word-close. For DDS2
`func_0024D430`, combining the genuine glyph-increment eligibility in the
completion-first layout removes the old `+0x21C` branch residual. Using
the existing producer's `u8 func_002A8028(void)` contract leaves three
different words at `+0x258/+0x260/+0x264`, not the older park's one word
with its `s32 (s32)` declaration. A global-byte getter's native body alone
does not prove its original return width or distinguish zero parameters
from an unused parameter. Recover independent API evidence rather than
choosing the declaration with the prettier score.

DDS2 `func_0014D0E8` (660 bytes) is an exact example of branch-local
complete actions. Its stage-29/30 and default branches select different
effect descriptors, but each creates the node, publishes it, and restarts
the base instance. Keeping that complete action in each branch lets GCC
schedule the restart argument load before the new-node store; reorg then
uses the store in the restart call's delay slot and merges the common tails.
Factoring only the restart after the branch instead left the store and
argument load in different scheduling blocks, producing three differing
words out of 165: `+0x124` annulment and `+0x138/+0x140` store/load order.

The controlled probes keep the same stage condition and pass-28
`REG_BR_PROB 5000`. They differ from pass 00 because the real call sequence
belongs to each branch. The exact form's pass-29 default-descriptor donor
executes on both paths, instead of only the taken path. This is an earlier
CFG and scheduling change, not a same-TU nothrow fix or an inferred change
of branch probability. Correct effect pointer APIs and the producer's
`u8 fldTestSceneControlFlags(u32)` contract retain the match. The model
context and node primaries are shared with their existing owner through
`mdl_context.h`, rather than duplicated as private prefixes.

Treat this as a source-shape lead when each arm performs a real complete
action and the mismatch involves its common tail. Do not duplicate calls
that would execute twice, invent work or dependencies, or generalize it to
every branch-likely residual. Selecting one descriptor before one call was
a distinct failed shape here: it produced `movn` and a shorter function.

## Nested rectangle draw records and void submission

The resource-rectangle family has two real parameter records. The `0x24`
`EffResourceRectParams` stores extent and center in its first three words,
then embeds an `0x18` `EffResourceRectDrawParams` at `+0xC`. That inner
record contains four color bytes, signed blend control, and four signed
bounds. The `0x28` owner appends its source handle at `+0x24`.

DDS1 `effResourceQuadDraw` (`0x00187DB0`) and DDS2's twin (`0x0018F9E8`)
receive the inner record directly. Their byte color loads start at input
`+0`, blend control is at `+4`, and bounds begin at `+8`. An old call that
passes the outer record's color-array address does not justify introducing
a second struct view. Embed the actual draw record in its existing owner
and pass `&work->params.draw`.

These renderers finish with the canonical `SdfPoolNode.append` callback,
which returns `void`; the four effect dispatch consumers also ignore any
result. The pixel-bound generator therefore has a real
`void (EffResourceRectWork *)` contract, not an invented scalar result.
Changing the nested layout and provider contract preserves the native
nine-word clone and both bound generators.

This ownership/ABI cleanup is separate from matching the quad renderer.
The canonical DDS2 draft still differs in 56 of 129 emitted words against
131 retail words, beginning with the branch distance at `+0xF0` and the
left-column store order at `+0x104`. Chaining the paired column assignments
does not change that result. Keep the renderer as assembly rather than
adding an interior shadow, widened parameter, or store-order lever.

## Party page rows and embedded staff resource banks

`MenuPageWindow.records` borrows the existing `PartyPanel`: its two counts
are at `+0/+4`, and five `0x34`-byte rows start at `+8`, for a total `0x10C`.
Each row's party index is at `+4` and resource index at `+8`. Use the real
`slots[index]` row; a record starting at `+0xC` is a biased projection, not
another layout. Both games' page-resource and count consumers match with
this primary owner.

The primary, secondary and alternate page resources at `+0x0C/+0x14/+0x1C`
are `EffectSlotSet *` in both games; `+0x10/+0x18/+0x20` remain signed slot
indices. The count/list APIs consume the real `PartyPanel *` and its two
count fields. Do not invent a separate count-prefix owner or transport these
resolved resources through integer words at the page constructor boundary.

`BrsSkillPackageWork` embeds `StaffSlots` at `+0x4F8` in DDS1 and `+0x51C`
in DDS2. Pass its address to staff-bank APIs instead of treating adjacent
scalar fields as an array. Its resolved base, pair, main and extra banks
retain `EffectSlotSet *` owners. Pass them directly to the page constructors;
their resource formals are pointers, not serialized handle words.

Keep page-selection clearing on the real `MenuPageWindow.slots` array.
Direct indexing of the selected row's HP/MP animation states gives the
native DDS2 helper without a second `MenuWindowSet`/`MenuSlotWindow` view.
That earlier shared-header closure was 58 actual CPP consumers, all clean in
serial `check_unit` runs (2200 matching functions, zero differences).

DDS1's general `MenuWindowContainer` keeps frame, base-sprite and overlay
`EffectSlotSet *` owners at `+0x2C/+0x34/+0x3C`, with their slot indices at
`+0x30/+0x38/+0x40`. The grid setter dereferences these resource sets; they
are not XY coordinates or color words. The signed decoration fades at
`+0x44/+0x48` are distinct from the window-wide `fadeScale` at `+0x88`.
Keep the native pre-update alpha snapshots and per-layer clamp behavior.
The forwarding API now takes the real resource pointers; unresolved outer
saved words cross that boundary once rather than creating another window
view. `entryX` at `+0x1C` remains a signed word: its staff resource, camp
option and zero producers do not establish one universal pointer type.


## Profile panels retain sprite-slot pairs

DDS1's `0x3C` `MenuProfilePanel` owns three `MenuProfileSlot` records at
`+0x18`, `+0x20` and `+0x28`. Each record holds an `EffectSlotSet *` and a
signed entry index, not XY coordinates. The draw routine dereferences the
set's work-entry bank and reads the selected `BdWork.sourceWidth`; its
phase and opacity are the words at `+0x34` and `+0x38`.

DDS1's scaled variant draw at `0x0024F338` uses the same complete
`EffectSlotSet`/`BdWork` pair as its exact `0x0024EF68` neighbor. It temporarily
scales the selected entry's width and height, draws through resource-table
slot 5, then reloads the placement-selected resource before restoring its
native dimensions. Retail re-resolves the resource across the drawing callback;
do not replace these owners with a sprite-resource prefix or cache the
pre-call work-bank pointer for the restore.

The generic two-word `itfGridStorePosition` setter's name does not prove
that these cached words are positions. Pass the actual slot record and
the genuine grid/fill-index inputs explicitly. The native first call
needs no incoming-argument copies; that is not evidence for omitting its
second and third arguments. Constructor, release, cache and phase advance
borrow the same primary panel, and DDS1's progress host retains its pointer.

This owner cleanup does not match `func_00285208`. After three natural
loop forms, the best canonical draft remains 556 bytes against 568 retail
bytes, with 94 of 139 emitted words differing. The live draw function stays
assembly. Main intentionally retains the protected PR547 integer-word draw
bridge; do not widen or reshape it as part of this cleanup.

## Terminal commands belong to the list's window state

DDS2's four command setters at `0x002971C0`–`0x00297220` all receive a
`MenuList *`, not the enclosing terminal scene. The dispatch caller at
`0x00261F48` passes the wrapper's `+0x18` list to the first pair, and the
window constructors allocate the same `0x14`-byte context for that list.
Its first two words are the embedded `MenuAction` command value and mode;
its selected slot is the halfword at `+0x12`. Use
`((MenuTerminalWindowState *)list->context)->command` through a typed local,
without a second action-owner projection.

The command value is signed `s32`: the row and window fades load it into
`cvt.s.w` directly, with no unsigned-conversion branch. The same primary
context has a signed pulse counter at `+0x08`; the selected-row callback
increments it through 60 and converts it directly to float for its sine.
The matching window fade at `0x002960F0` borrows this context directly,
instead of projecting its command through a separate `s32 *` array.
Its complete consumer unit is clean at 13 matching functions. The larger
row callback at `0x002958B0` remains assembly after three honest source
shapes, with its best bounded candidate parked rather than enabled.

The separate `0x38C` `MenuTerminalContext` owns the terminal's phase at
`+0xC0` and frame counter at `+0xBC`. The phase is signed: the wait helper
uses `slti`, not `sltiu`. Its slot-advance count at `+0xCC` is also signed
byte storage, as the text consumer uses `lb`. Scene consumers borrow the
real `MenuWindowContainer`, `MenuList`, `MenuListNode.camp` and window-state
owners rather than shadow prefix records. Existing SDK address-word and
byte-pointer interfaces remain explicit boundaries; member addresses do
not require integer offset arithmetic.

This scene embeds the complete `0x168`-byte `MenuCampEffect` at `+0x210`,
not just its `0x60`-byte resource prefix: the sixteen spark records and badge
fade continue through `+0x377`, before `windowResource` at `+0x378`.
DDS2 `0x0025FD78` draws effect slots 1, 2 and 3 for scene types 0 and 2,
setting slot 3's primary `BdWork.geometry.angleDegrees` to 90 degrees. Types 1 and 3
pass `&scene->campEffect` to `mnuDrawCampIconBackdrop`; resource loading and
teardown borrow `&scene->campEffect.resources`. The renderer and every live
caller use `MenuTerminalContext *` and a void return contract.

The shop transaction at DDS2 `0x00263FB0` borrows the selected row
through `window->list->cursor->camp`, not an event-object prefix.
Its full-width value times `MenuTerminalContext.multiplier` determines
the currency delta; `DatGameState.inventory.counts` consumes only the
multiplier's low byte. A purchase increments ordinary inventory only
when the listed-item lookup is negative, but the allowed-ID progress
test is independent of that lookup. Both purchase and sale restore
the row's displayed value from its preserved `camp.price`.

The provider unit is clean at 38 matching functions, and the complete
event consumer unit at 75, with zero differences and no checker flags.

The generic `0x74`-byte list node's value at `+0x04` is an opaque
`const void *`, passed unchanged by both games' append and insertion APIs.
DDS1 `0x00245A40` passes a 25-byte caption-table row and DDS2
`0x002B0FA0` passes a party-caption entry; null entries do not establish a
separate scalar variant. Actual item/command IDs occupy the distinct
`camp.value` payload at `+0x60`. The DDS1 item-list constructor uses the
price record's byte flags at `+0x00` and word price at `+0x04`, excludes
IDs `0xA0`–`0xBF`, and otherwise selects `0x60`–`0x7F` prices by solar phase.
It releases the prior list context before destroying the old window,
then attaches the new draw callback and zeroed context to the new list.


## Camera-color keys borrow the complete camera setting

The polygon-movie interpolators and camp timeline's `blendData` share the
`0x38`-byte `EvtBlendKey` in `fld.h`. Its two three-value banks are separate
from the camera setting's destination rows; pass the key itself, not an
integer-word projection of its stack address.

`fldLoadBattleSkyAndFilter` allocates complete `0x54`-byte
`FldCameraSetting` records and copies the complete default into each slot.
The first word precedes the embedded `0x20`-byte `FldColorParams`; the tail
is three four-word rows starting at `+0x24`. The native key writer updates
columns 1 and 2, at `+0x28/+0x2C`, then advances by `0x10` for each row.
Do not collapse that tail to three two-word pairs or declare all of it
padding. The other columns retain their previous values through the
complete setting copy.

The copy/update APIs borrow this primary setting directly. DDS2's former
`FldSaveHeader` word array was a second view of the same storage, not a
separate save-file allocation.

The two key-application helpers remain assembly, with honest typed candidates
parked in `build/parked`. The best measured DDS1 form emits `0xE0` bytes
against `0xF0` retail and differs in 43 of 56 emitted words, beginning at
`+0x20`; the paired DDS2 baseline differs in 44 of 56. Do not restore the
old stack-word casts and byte-offset copies merely to recover a C match.
This is a complete ownership/API cutover, not a claim that either helper
has been decompiled.

## Staff description state is a word, not an address-valued SDK handle

DDS2's `func_002C1B68` is an ordinary game function with the real
`void (u32 *out, u32 value)` contract: its body is `*out = value`.
The staff callbacks pass the word at `MenuStaffContext +0xAA50`.
Expose that single `u32` in the primary owner and pass its address;
the neighboring padding does not establish a larger workspace allocation.
Eight existing staff-page callers now use the correct pointer prototype;
their complete units are clean at 43 and 24 matches. The terminal's two
gradient-control callers borrow `&state->active` under the same contract
(83 matches). `MenuGradientFade.active` is a `u32` word: both games only
store zero or test it for zero/nonzero, with no sign-dependent use.
The PR534/547 word-setter callers remain intentionally protected residuals,
as does the separately deferred dispatcher ABI.

The staff-value callback at `0x002AFE18` matches all 856 native bytes using
the primary staff, window, list and party-record owners. Its final item-use
arm is a normal nested `if (operation == 0) { failure } else { reset }`,
not an inverted `else if` chain. Keep the actual `u16` current-ID getter
return type and its existing integer-address parameter contract; the two
older label callers and the new callback all preserve their native code.


## Flag lists own their complete embedded color parameters

The DDS1 `0x002CEAE8` and DDS2 `0x00316528` factories allocate
`0x58 + 0x30 * count` bytes. The `SdfFlagListWork` is appended after
three separate arrays: two four-float vertices (`0x20` bytes), two packed
colors (`0x08` bytes), and one `SdfFlagListMark` (`0x08` bytes) per entry.
The work owns those array pointers and the SDK allocation handle at `+0x54`.
The factory copies the entire `0x40`-byte `SdfFlagListParams` to `+0x14`;
neither the old `RgbAlpha` prefix nor a separate selection-work view is a
complete owner.

The parameter's `0x24`-byte color track contains packed color words, not
floats at every word. Its following `0x10`-byte alpha track carries the
alpha value, draw-surface index, and floating fade-in/fade-out fractions.
The final three parameter words are the signed frame limit, entry count,
and speed. Field color defaults and updates borrow this same parameter
record, and color-copy APIs write directly into the embedded record.

Rain and field effect slots store `SdfFlagListWork *`, not game-pointer
words. DDS1's battle slot is `+0x58C`; DDS2's is `+0x5C0`. The existing
singleton battle getter's address-word ABI is unchanged. SDK retained
address conversion and allocation partitioning occur only at the factory
boundary; callers do not reconstruct this owner's fields through offsets.


## Runtime task handles retain the unsigned sequence counter

`btlStartTask` returns the runtime task's `u64 handle` at `+0x38`.
DDS1's `0x001D4860` stores the fresh 64-bit sequence counter with `sd`
and rereads that same member with `ld v0` at `0x001D48A8` before returning.
The shared `BtlRuntimeTask` and DDS2 provider use the same member.
Use `u64`, not a widened signed return invented to obtain code generation.
Both complete provider units and the unprotected local-extern consumers
retain their native code after this return-only correction. The protected
PR530 consumer in DDS2 `code_00227288` intentionally retains its local
`s64` declaration; there is no conflicting shared-header declaration.

The effect creator's scheduler predicate at `+0x08` belongs to
`startCondition.value.taskKind`; it is not the runtime `taskId` at `+0x20`.
The DDS1 scene consumer borrows the existing `BtlRuntimeTask` rather than
a misleading short request prefix. Its readiness function remains assembly:
three natural forms failed, and the best candidate is parked, not enabled.
The separate task-argument getter ABI and its 180 declaration/call/definition
sites are unchanged; a provider-local pointer prototype is not a substitute
for an actual getter/consumer cutover.

## Party pages borrow complete stat rows and texture/effect bars

`PartyPanelEntry` is a real `0x34`-byte row, not three words followed by
padding. DDS2's producer at `0x002C4328` copies the party record's level,
current HP/MP and maxima into `+0x0C` through `+0x1C`, then sign-extends
the five base-stat bytes into the five words at `+0x20` through `+0x30`.
The natural indexed five-stat `for` loop matches all 260 native bytes;
the caller passes the actual `DatPartyRecord *` without a byte-view cast.
The complete producer unit remains clean at 172 matching functions.

The HP/MP subrecords in each DDS2 page bank are the same complete
`0x50`-byte texture/effect owners used by the ratio-panel helpers.
`MenuPageBar` owns the settings, seven textures, two effect nodes, fade
and effect-update state. The former local `MenuEffectPair` was a second
view of this storage, not another owner; its consumers now borrow the
shared primary type. Fade/reset callers use `fade` and `fadeOut` directly,
and the complete companion unit remains clean at 109 matching functions.

The larger page renderer at `0x002BFEA0` stays assembly after three honest
source shapes. Its best `MenuPoint`-based candidate differs in 22 of 291
instruction words and still has constant-pool/shared-data differences.
Keep that partial parked rather than enabling it or declaring a match.

## SDK callback lists own both teardown hooks

`sdfCreateTaskHeader` allocates and clears the complete `0x1C`-byte
`SdfList`, including its allocation handle, count, endpoints, userdata
and both hooks. `sdfListAppend` and `sdfListInsertAfter` allocate and clear
the complete `0x14`-byte `SdfListNode`; its value is a borrowed payload
pointer. Movie task slots and their cue links borrow these owners, not
separate short headers or offset-based link views.

Userdata is a real game-owned pointer: the menu constructors pass their
resource work through the list to its teardown callback. Setter arguments
are function pointers, not signed address words. The SDK's generic
unprototyped hook convention accommodates the shared no-op also used by
scalar task entries and the specialized ordinal/sentinel-plus-payload
callbacks. Do not add function-pointer casts or matching-only adapters.

DDS3's indexed heap list is different: its count, head and tail begin at
`+0`, `+4` and `+8`, and its node is released through the ordinary heap
free routine rather than the SDK size-class allocator. Its identical
`0x14`-byte node geometry alone does not prove shared producer identity.

The allocator's unlink helper instead borrows the existing `0x10`-byte
`SdfMemBlock`. Its previous/next links are at `+0` and `+4`; an eight-byte
`SdfListNode` projection or byte-offset view is not a second list owner.
Both titles pass the actual allocation and its predecessor directly.


## World nodes own separate transforms and kind-specific work

`dds3CreateWorldNodeForKind` (DDS1 `0x0010F418`, DDS2 `0x0010F640`)
requests `0x44` bytes: **68 decimal**, not `0x68`. `EffWorldNode` is this
primary owner. Camera, action, script and event-slot nodes share its
metadata, callbacks, payload pointer and links; they are not independent
short object headers. Its high tag byte selects the kind. The payload at
`+0x18` remains opaque in the shared owner, with each kind using its own
existing work type at the consumer boundary.

The separate `ObjectTransform` allocation is `0xD0` bytes
(`effObjInnerCreate`, DDS1 `0x0010F570`, DDS2 `0x0010F798`). The world node's
`inner` at `+0x1C` points to its matrix, position/rotation/scale, saved and
smoothed vectors, and flags at `+0xC0`. Vector setters accept the world
node and select its inner allocation; transform flag/backup helpers
accept the transform itself. Do not confuse the kind-specific payload
with this inner owner.

The kind-dispatch getter at DDS1 `0x00112888` / DDS2 `0x00112AB0`
also receives this complete world node. Reading `kindTag >> 24` keeps
the native byte load at `+0x0F` without a second short owner view.
`dds3CreateWorldInnerState` (`0x00112958` / `0x00112B80`) separately
allocates `0x90` bytes of kind-specific work and stores that pointer at
`+0x18`; its historical "inner" name does not mean `ObjectTransform`.
That work owns an `ObjBase *` at `+0x80` and a state word at `+0x88`.

The model viewer's attachment routine (DDS1 `0x0021FD50`, DDS2
`0x0023A8C0`) looks up two such nodes: kind 5 is the destination and
kind `0x11` supplies two adjacent float vectors through `data`.
The source payload is borrowed as float vectors, not another short
world header. Its final VU0 store addresses the destination's separate
transform vector at `+0x70` through `ObjectTransform`.

The kind directory `D_003299C0` points at complete six-word (`0x18`-byte)
`EffWorldOps` tables. Their first four entries are create, destroy, update
and draw callbacks; both remaining words are zero. Kind 1 selects
`D_00329A20` and its initializer at DDS1 `0x00110578`, which allocates a
`0x40`-byte `EvtWorldTable` and eighteen `0x0C`-byte slots. Each slot's
endpoints are borrowed `EffWorldNode *` values.

Kind 1 also uses the first eight metadata bytes as `WorldValueIndices`:
the head, tail and cursor indices followed by the entry count. The cursor
helper at DDS1 `0x00110490` demonstrates this interpretation. The same
index record occurs in a separate `0x10`-byte allocation at `0x0010FEC8`,
so its accessors retain the index-record type rather than pretending
every index record is an entire world node. Kind 2 instead stores its
object key in the second metadata word (`evtSpawnActionObj2`,
DDS1 `0x00111188`). The value word can hold either a scalar or a native
caption address; caption consumers interpret that word as a string
pointer at the use site.

A shared tagged union was tested and rejected: its aliasing changed the
already-matched world and script constructor schedules. The exact shared
representation therefore remains a plain node with opaque payload and
kind-specific interpretation at the consumer boundary. No matched C
constructor is replaced with assembly to accommodate a type cutover.

## Reload caller-owned coordinates across viewport updates

DDS1 `func_00250E88` (`0x00250E88`, 216 bytes) scrolls a viewport toward
the signed X/Y entries in `D_0036B7F0`. Reuse the existing
`MnuVariantSpritePlacement` six-halfword row contract from
`code_0024E1C8` and `code_00259498`, rather than inventing another struct
view of the same table.

The initial X value is retained for its upper-bound check. After the Y
updates, however, the lower X adjustment reloads `*xCoordinate`: the
caller's X/Y pointers may alias. Keeping the initial X value across those
Y writes changes the function's behavior. The natural write-first update
`*xCoordinate += deltaX - 0x13C; x = *xCoordinate;` also reproduces the
retail store followed by the cached bound-check value. This source was
text-exact under `check_unit.py`; no scheduling-only temporary is needed.

## Resolve actual predicate contracts before scheduling work

DDS2 `func_0028A018` (`0x0028A018`, 220 bytes plus four bytes of padding)
prints `mtrChkMantraCompleteMaster!!` and checks the six mastery flags
through a nine-entry unit-index map. Its owner is `DatPartyRecord`, not
an integer with an ad hoc `+4` access. The actual providers are
`s32 mdlFlagTest(s32)`, `s32 prfAreAllRequiredProfileFlagsSet(DatPartyRecord *)`,
and the existing variadic `void evtPrintDeveloperConsoleMessage`.

With these contracts, ordinary automatic array initializers and early
returns are text-exact. The arrays replace their two source
`INCLUDE_RODATA` markers; the list caller converts its existing opaque
word into the party-record pointer at the consumer boundary. The full
unit gate preserves all 23 compiled functions without instruction or
delay-slot forcing.

## Timeline registration slots belong to script storage

DDS1 `func_00243F48` (`0x00243F48`, 268 bytes plus four bytes of padding)
selects the first type-4 `CampKeyTrack` and checks its keys against the
scene's current frame. The ten registration slots at game-state
`+0x360..+0x384` are existing `DatScriptGlobals.ints[200..209]`, not a
second game-state layout or `CampScene.registeredIds`.

Reuse the primary track/key records and `mnuUnpackNibbleFields`. Its two
output parameters are real work even though only the high nibble is used
here. An absent eligible key clears the corresponding script register
only if it is not already `-1`. This ordinary nested traversal is
text-exact; the whole current DDS1 unit gate preserved all 66 compiled
functions with no context, data, or undefined-symbol diagnostics.

DDS2 `func_0025F330` is the 268-byte instruction twin and reuses the
current DDS1 traversal unchanged. Its script-slot count remains ten even
though DDS2's separate `CampScene.registeredIds` registry holds twenty;
keep those two limits distinct. The full DDS2 unit gate preserves all
86 compiled functions with no context, data, or undefined-symbol diagnostics.

## Sparse resource cases preserve motion-selector range trees

DDS2 `func_00218520` (`0x00218520`, 268 bytes plus four bytes of padding)
uses selectors 16/17 to restore saved motion parameters for resource IDs
`0x104..0x106` and `0x138..0x13A`. A single six-case switch reproduces
the native range tree; a compound pair of range expressions does not.
The resource-ID snapshot has the unsigned domain shown by all four
native `sltiu` comparisons. Retain the shared actor record and its
existing resource-index field rather than introducing an alternate view.

The motion state belongs to `BtlUnit.ext`, its model to `EvtUnit.owner`,
and its sampled speed to `Motion.frameStep`. The API accepts five integer
arguments followed by a float; treating the fifth integer mode as absent
loses the override carried in `$8`. Its actual callees are the void
`evtPrepareUnitMotionState`, `mdlAddEntryFlagged`, and
`sdfMotionSampleAtFrame` providers. The stable whole-unit overlay gate
preserved all 316 current C functions with no data or context diagnostics.

## Ready-first battle light callbacks preserve the wait counter

DDS1 `btlUnitBaseLightTask` (`0x001D9820`, 280 bytes) runs the lighting
work only for an active unit whose signed delay has reached two. The
ready arm returns completion immediately; otherwise the active wait arm
increments its counter and returns zero. This ordinary ready-first
control flow reproduces the native branch-likely increment-store delay
slot. Early-return wait guards instead overrun the next function.

Use `BtlUnit.ext` and the primary `EffWorldNode` payload. The existing
`EvtUnit.currentTransitionValue` word carries a slot-9 node address when
flag `0x40000` is set, as also shown by `evtUnitManager.c`. Its light
payload uses the shared `EvtTargetInfo` in `evt_unit.h`: first colour at
zero, direction at `0x10`, and second colour at `0x40`. The six copies use the
existing Sony-compatible `PCP_COPY_VECTOR` primitive, not new game-code
assembly. Retain both target-owned diagnostic strings as literals so
the complete unit's data remains exact.

The creator's eight-byte argument block contains the unit pointer and
signed delay. Its owner identity comes from `BtlUnit.identity`; no second
owner view is needed. The complete current DDS1 unit gate preserved all
569 C functions with no context, data, or undefined-symbol diagnostics.
The 280-byte DDS2 instruction twin is `0x001E67E0`.

DDS2 now uses the same ready-first callback and a dedicated unit/delay
argument tuple rather than the unrelated sound-task argument alias. Its
complete `game/code_001DD390.c` gate is **520 match, 0 differ**, including
the 280-byte callback, the unchanged 120-byte creator, and literal data.
The unchanged 0x68-byte `EvtTargetInfo` was promoted from the identical
event-manager and DDS1 battle definitions; all three local copies were
removed rather than introducing another payload view.
All 27 transitive `evt_unit.h` importers (12 DDS1, 15 DDS2) passed the
combined serial gate with zero differences or diagnostics; the independently
resolved inventory and current source/header fingerprints agree with it.


The scene-light propagation callbacks (`001F0430` / `00200FB8`, 332 bytes
per title) consume a 0x34-byte request: direction, light color and ambient
color as complete four-float vectors, then the transition value. Keep the
SDK vector-copy boundary and the two separate unit-status guards. Combining
the flag predicates lets EE GCC replace the guarded word reads with a joint
64-bit mask test.

After the unit callbacks, capture the transition count before publishing the
three output direction components, then store the captured count plus one.
Retail reads the count before those float stores; an increment expression
after them delays the load and changes scheduling. This is the observed
memory-read epoch, not an extra use or a synthetic dependency.


## Camp option availability uses signed halfword flags

DDS2 `func_00260020` (`0x00260020`, 280 bytes) copies a seven-entry,
28-byte enable configuration, queries availability modes one and three,
and enables additional options from the shop's message-set ID and
`DatGameState.progressTotal`. The query provider takes only the scene
and mode; the remaining caller registers are not extra arguments.

The primary `MenuTerminalContext.unkA0`, `unkA2`, and `unkA4` fields
are signed halfwords: the native five boolean loads use `lh`, not
`lhu`. Recover their types rather than casting each predicate. The
unit-local `CampFlagRow.messageSet` is an unsigned resource ID; grouped
switch cases one through three reproduce its `sltiu` and zero exclusion.

The resulting window belongs to `ownedWindows[0]` at `+0x7C`, not the
separate optional `window` at `+0x80`. Its destructor already distinguishes
those owners. The target and complete DDS2 unit are text/data exact
(`87 match, 0 differ`); a serialized gate of all 66 direct and indirect
`mnu.h` source includers preserved every compiled C function with no
context, data, or undefined-symbol diagnostics.


## DDS2 staff item use owns the complete party page and fade state

DDS2 `func_002ADDA0` (`0x002ADDA0`, 492 bytes excluding padding) uses
`MenuStaffContext.partyWindow` at `0x284`, not a flags word projected
as a page. The retained selection list is `partyWindow.lists[0]` at
`0xA914`. The same allocation owns `PartyPanel` at `0xA928` and
`MenuFadeFields` at `0xB10C`; the creator allocates and clears `0xB1E0`
bytes. Keep its remaining `0x10` bytes opaque. The field-skill window
at `0x104` is also a real `MenuWindowContainer *`: `002AAEA0` accesses
its list and last node.

The selected item ID comes from `MenuListNode.sortKeySecondary`;
`sortKeyPrimary` is its quantity. Item use replaces the input word
with zero on success or `0x8000` on failure. The readiness helper can
remove an exhausted cursor, so reload the current cursor and its
quantity after that call. The failure path precedes the ready path;
cancel/empty selection does not start another fade.

`mnu_staff.h` is the DDS2 interface used by the actual staff/page/fade
consumers. Item/effect entry points transport page and party-record
pointers, not integer-address adapters. The fade starter retains a
generic resource/task boundary (`void *` arguments) and recovers the
window and fade owners inside. That natural interface preserves the
native runtime-alignment copy sequence; direct record-pointer formals
and a typed-window/opaque-work form changed 77 of 95 emitted words.
The two recovery locals are necessary object conversions, not
register-allocation aliases.

The item-use callback and its complete unit are text/data exact
(`45 match, 0 differ`). All existing control flow, including the
field-effect provider's pre-existing sound arms and development hooks,
remains unchanged by the pointer-contract cutover.


## Motion-sound callbacks retain the pack's native format records

The 448-byte DDS1/DDS2 `evtUpdateMotionSeTask` twins use the existing
`EvtMotionSeTaskParams`, a kind-5 payload's `EvtUnit *` at `+0x08`,
and `MdlCtx`'s first motion. Convert its floating current frame to a
signed word before comparing the type-6 pack's 16-byte cue records.
The pack entry's cue count and motion ID are signed words at `+0x08`
and `+0x1C`. A missing object, pack, or unit returns `-1`; the ordinary
continuation and event-ID exclusion return zero.

Replacing the last assembly body also removes format records carried
with that body's data. Own the existing `"mse_%d_%d"` and script-path
records in C at their native order, preserving their 16/32-byte extents,
eight-byte alignment, and trailing zeros. Do not drop the pack's three
small-data includes or literalize their strings.

Both normal objects retain exactly 24 bytes of small data and contain
no `.lit4` or `.lit8`. Losing the format records shrinks `.rodata` from
144 to 95 bytes; its following alignment shifts the later banks by
48 bytes and can push unrelated literal relocations outside the fixed
retail gp window. Restoring the actual data owners, not changing flags
or adding a fake float symbol, produces both retail-SHA-exact executables
in the isolated full build.

## DDS2 terminal panel fade uses a mode switch and early disable guard

`func_00268838` (`0x00268838`, 364 bytes) uses the existing `MenuSlotState`
owner. Mode zero draws two overlays and decrements a positive hold counter;
modes one and two clear that counter. Negative modes and modes above two
preserve it, so replacing the switch with a nonzero-mode test is incorrect.
Reduced mode also clears the hold counter.

The bank's `+0x18` pointer is `EffectSlotSet.workEntries`; the `+0xC4`
angle write is `workEntries[1].geometry.angleDegrees`, not an offset in the bank.
Fade-in adds twelve and clamps to 256; fade-out subtracts seventeen and
clamps to zero. The two final draws use the scene's owned
`DspScrollingStripState` records.

An early disabled-state return leaves the last panel draw as an unconditional
terminal statement, reproducing the native sibling tail call. The enclosing
conditional form was four words shorter. The typed implementation and all
eight task-address caller conversions pass the normal whole-unit checker:
`114 match, 0 differ`, with no context or data diagnostics.

## Battle camera state embeds the linked-command payload

The battle singleton's `+0x70` address is passed to the same camera-motion
helpers as an ordinary `BtlLinkedCommand`. Its active, start and end poses,
flags, progress and target list therefore belong to one embedded
`cameraCommand`, not a second camera-only view. The member ends at `+0x1A4`
in DDS1 and `+0x1C8` in DDS2, preserving the following event fields.
DDS2 `func_001E9130` additionally loads the command's quaternion at `+0x100`
and translation at `+0xF0`; the singleton's `+0x6B0` predicate receives
the entire command before its selected camera handler runs.

## Resource-slot geometry has one contiguous bounds array

`BdWork.geometry` owns four signed bounds (x, y, width, height), four packed
corner colors, and the rotation angle at the unchanged native `+0x04..+0x24`
offsets. Grid easing walks the first two bound coordinates and then the
geometry's actual `cornerColors` member; it does not walk beyond a scalar
or reach colors through a second record view.

Easing parameters come from `EffTimedState.source->status`: the full mapped
record retains the address of its allocated, decoded table bytes. Reading
that buffer as the existing table does not require a `GridAngleSlot` prefix
projection or a redundant cast of the already-typed source record.

DDS2 `itfGridApplySqrtBoundsAndColorScale` (`0x003075D8`, 312 bytes) and
`func_00307EF8` (`0x00307EF8`, 296 bytes) match using this grouped owner and
the mapped record's real `status` pointer. `func_00307A68` (`0x00307A68`,
320 bytes) ports the same owner's threshold-alpha easing from the matched
DDS1 `func_002C0038`, including its destination/source palette cursors.
The complete grid units check DDS1 `62 match, 0 differ` and DDS2 `70 match, 0 differ`.

## Battle ability queries receive the party record

`btlCheckSpecialAbility` forwards its first argument to `datUnitHasSkill`,
so that argument is a `DatPartyRecord *`, not an integer actor address.
Actor-facing slot and recovery helpers receive a `BtlUnit *` and pass
`&unit->partyRecord`, the native subobject at `+0x120`.
`btlApplyCommandAbilityMultiplier` also receives a party record: the
combined-party caller passes a local record snapshot, not a whole actor.

The display identifier at `+0xE0` is unsigned: DDS1
`btlIsActorModeActionCodeAllowed` compares it with `sltiu` at `0x001AAAF8`.
The canonical members are `displaySpecies` in DDS1 and `combatantKind` in
DDS2; their other current consumers use equality or forward the identifier
to model-loading helpers, not signed ordering or arithmetic shifts.


## Battle entry tasks retain their native pointer owners

DDS1 `func_001D4E98` obtains model resources through `BtlUnit.resourceLink`
and `effectObject`; the battle callbacks at `+0x5C4`, `+0x5D8` and `+0x5E0`
receive the unit itself. DDS2 `func_001D08A8` sequences the native 64-bit
task dependencies through `beginBattleEntryTasks` and `finishEnemyEntryTasks`,
with `selectEntryModelVariant` receiving the current unit.
Actor-transparency payloads contain a `BtlUnit *`; actor-effect payloads
contain the allocated 32-byte `SoundResourceNode *`, not its distinct
24-byte `SoundEffectNode` prefix.

## Paired billboard packets interleave both child streams

DDS1 `func_00150EB0` and DDS2 `func_00158AA0` build seven-entry local color
and UV arrays from both `BillPacketWork` children. Each vertex's first
two UV components come from child zero and its last two from child one;
the paired packet builder consumes those arrays before both counts reset.
The paired builder is DDS1 `func_002E2BB8` / DDS2 `func_0033BA68`, distinct
from the compact single-stream vertex builder.

## Party vitals use one paired-effect owner

DDS1 `func_00275B40` writes numeric-bar opacity at `MenuEffectPair +0x4C`;
the existing paired-effect helpers own its position nodes at `+0x40/+0x44`.
Those types now live once in `mnu.h`. The draw banks and cost-icon arguments
are `EffectSlotSet *`, while the font builder retains its existing integer
glyph-handle API. The 1028-byte target and all 66 menu-header includers match.

## Debug FOV and battle presentation preserve real call contracts

DDS1 `func_0019FCC8` builds a complete `SifCommand`, appends the pointer returned
by the variadic SIF formatter, and edits `SdfProjectionRecord.camera.fov`.
Its native format literal is function-owned; the complete unit checks 35/0.
All six native callers of `btlResetActorSlotPresentationValue` deliberately
pass a `BattleSceneObject *` in the second argument, even though the callee
does not use it. DDS2's `+0x6A4` battle callback likewise receives a `BtlUnit *`,
now represented by `BtlState.actorEligibilityOverride`.

## Resource-progress records retain their array owner

DDS2 `func_00321340` indexes 28-byte `MenuResourceRecord` entries, reloads
the table after registry lookup, and compares its signed `progress` member
with the unsigned halfword progress counter. The constructor at `00322E18`
receives a `MenuWorkEntry *`, not a numeric node identifier; the animated
effect factory likewise returns the allocated work entry. The canonical
record and these pointer contracts live in `mnu_work.h`.

## DDS1 staff pages retain one embedded window owner

DDS1 `func_00276898` receives a `KwlnTask *` and explicitly forwards it to
`kwlnTaskGetUserValue`; the native accessor loads the task's `+0x38` word.
`CampMenuContext.partyWindow` starts at `+0x15C`, with native slots at window
`+0x78` and a `0x134` stride. Its `lists[0]` is at window `+0x67C`, hence
context `+0x7D8`. `mnuDrawSlotIcons` receives the window itself: the native
staff scene callback passes `context + 0x15C` through the party-status draw
to that icon routine. No additional slot field or overlapping selection-list
view is needed. The following `PartyPanel` starts at `+0x7EC`. Slot flags
and sprite `profileFade` are accessed through their existing owners.

The scene initializer resets one `EffectSlotSet *`: the reset producer
accepts no second argument, regardless of an incidental native `$5` value.
The 380-byte target and all other C in its unit check `65 match, 0 differ`;
all 66 current direct/indirect `mnu.h` and `mnu_staff.h` includers also
check with zero differences or context/data diagnostics.

## Rain shares the primary field-area state

DDS1 `func_00134348` and DDS2 `func_00136EF8` load a signed halfword
at primary area-state `+0x12A` before advancing the camera-color fade or
drawing the rain layers. A nonzero value suppresses those effects.
The heading source at `+0x64` is a float. These fields belong to the
existing `FldAreaWork` / `FldAreaState` owner, not a second rain-only view.
Their retail globals are respectively `0032E4DA` / `0038989A` and
`0032E414` / `003897D4`. Completing the primary types leaves both
non-matching rain functions as `INCLUDE_ASM`.


## Relocated resource groups retain their two-pass serialized layout

DDS2 `func_00325CC8` allocates a `0x14`-byte result through its real
owner-and-size allocator interface. It relocates the data block, then copies
all eight-byte `DdsCountedPayload` descriptors before appending their
eight-byte serialized entries and rebasing each descriptor's data pointer.
The existing `ResourceList.count.packed` supplies both halfword counts.
`ResourceNode.handle` is a generic address-valued word in this interface;
its conversion to the existing payload type does not change a callee's
pointer contract. Ordinary fixed-size `memcpy` emits the native unaligned
descriptor copy without an invented 64-bit field or inline assembly.

The 508-byte target matches all 127 words on the first natural form.
Its complete live unit reports `37 match, 0 differ` without context,
rodata, undefined-symbol or shared-data diagnostics.


## Mantra grid and frame share one display-work owner

DDS1 `func_00257BD8` advances the grid at `+0x484` and the wrapping frame
at `+0x490` in the same allocation. Both fields belong to the existing
`MantraPulseDisplayWork`, along with scroll coordinates at `+0x5A0/+0x5A2`
and flags at `+0x5AC`; a shorter `DisplayGridWork` view is unnecessary.
Folding the frame into the primary owner and deleting that duplicate view
preserves all offsets and leaves `code_00257200` at `9 match, 0 differ`.
This layout closure does not change the separate profile-progress API.

## Retained battle operands use one copied argument packet

DDS2 `btlCreateActorParameterDeltaTask`, `btlCreateDeferredActorStatsTask`,
`btlCreateMaskedActorEntryUpdateTask` and `btlCreateQueuedActorEntrySelectionTask`
allocate `0x30` argument bytes: one `BtlUnit *` followed by the complete
`0x2C` `BtlOperandEntry`. The deferred-stat and entry-selection callbacks
read that same packet, not shorter overlapping views. A fixed-size `memcpy`
copies the operand with the native unaligned-copy sequence after the actor
pointer is stored. Their provider unit checks `521 match, 0 differ`.

`BattleIndexWork.skillId` remains a signed word. A consumer's halfword load
can result from narrowing that word to a 16-bit selection ID; it does not
by itself establish a word/halfword union. Keeping the word restores the
unchanged DDS1 `btlRunAiAction` and its unit checks `207 match, 0 differ`.

The group's dispatch kind retains its game-specific signedness: DDS1 uses
`s32`, DDS2 `u32`. DDS2 `btlGetCommandEffectId` bounds its low dispatch cases
with `sltiu`; restoring the unsigned owner member leaves `code_0020E850`
at `60 match, 0 differ`.

The HP/MP forwarding entry points take a `DatPartyRecord *` and a signed
delta in both games. DDS2's retained operand callback passes the actor's
complete party record and its HP/MP deltas in the two argument registers;
zero-parameter forwarding declarations obscure that native contract.
The providers and their battle callers use the same record owner.

The retained-operand hook is a primary `BtlState` member at DDS1 `+0x5AC`
and DDS2 `+0x5E0`. Both native callbacks pass the unit and the copied
operand's first word, rather than a separate short argument view.


## Actor overlay consumers read the low effect flags

DDS1 `effApplyOverlaySpecs` (`002B3AC0`) loads a halfword at actor
`+0x310`; DDS2 `effApplySelectedActorEffects` (`002F7128`) loads the
corresponding halfword at `+0x330`. Both test `0x10` before applying an
overlay. DDS2 additionally tests `0x40` and skips overlay IDs `0`, `2`
and `0xA` when it is set. These are reads of `BtlUnit.effectLink.flags`,
not the packed eight-byte flags/reference state or its reference count.


## Integer VM setter return contract remains prototype debt

`scrSetIntegerReturnValue` has a void C provider in both games. The retail
leaf happens to leave `scrCurrentContext` in `$2`, and the already-matched
model-return commands (`00225620`/`00225880` in DDS1 and
`00240280`/`002404E0` in DDS2) retain a local `s32` declaration that propagates
that word. This is an unresolved wrong-prototype dependency, not evidence
that the setter's public API returns a context pointer.

An explicit `ScrData *` return preserves the setter's own instructions, but
changes unchanged flag-query consumers from `$2` to `$3` for their load,
mask and store sequence. Without independent evidence for a return value,
the typed-return closure is parked; its provider and existing consumers
remain as they were. Do not promote the incidental register value into a
new public return contract merely to make those commands match.

## Result animation level arguments are promoted integers

DDS2 `func_00299988` receives the level in a 32-bit integer argument and
stores its low halfword into both `BrsProgressAnimation.level` fields.
Both retail callers in `func_00299018` deliberately load `$6` with
`lhu` from `DatPartyRecord.level`; the callee does not normalize `$6`
to a 16-bit formal. Its declaration therefore uses `s32`, even though
the destination members are 16-bit. The current caller unit checks
`40 match, 0 differ`; the setter body remains `INCLUDE_ASM`.

DDS1 twin `func_00262A30` has the same promoted level contract: the
two calls in `brsApplyPartyRewards` deliberately `lhu` into `$6`,
and the setter stores `$6` directly into the two halfword levels.
Its source-local declaration also uses `s32`; the complete twin unit
checks `42 match, 0 differ`, with its setter body still `INCLUDE_ASM`.


## Field area and actor-row owner fields

DDS1 `code_00126A30` uses its existing `FldAreaWork` for the floor at
`+0x14` and signed event-state halfword at `+0x104`; the shorter local
`FldAreaState` view is removed. Both games' field-event dispatchers store
the command's value at area-work `+0x24`; this write-only word stays
`unk24`, without inventing a stronger meaning.

The DDS1 actor row loads `+0x36` with `lh`, passing it either as a flag ID
or a deferred-field selector. `sequenceValue` keeps those roles neutral.
DDS2 copies the row's string at `+0x55` with `strcpy`, matching DDS1's
existing `taskName[0xF]`; neither change alters the `0x6C` wire stride.

## Battle-state clocks and actor-returning sound selectors

DDS1's model-update clock at `BtlState +0x1F0` is distinct from the
scene-phase counter at `+0x210`; keep both fields without shifting the
`0xE10` owner. DDS2 `00201FD8` calls the `+0x6E0` selector with an actor
and conditionally substitutes its returned actor pointer. Its contract is
`BtlUnit *(*selectSoundEffectTarget)(BtlUnit *)`, not an integer status.
Numbered callbacks whose purpose remains unknown use `unkNNN`, rather
than assigning an unsupported meaning to their return values.

## Serialized billboard and file-emitter record owners

Billboard operation tables select genuinely different copied records, not
alternate views of one large configuration. DDS2 `003E9D00` class rows copy
`0x88`, `0x8C`, `0xAC`, and `0x88` bytes. Resource rows at `003E9DF4`,
`003E9E10`, and `003E9E2C` copy vortex/column/spiral records of `0xF8`,
`0xF4`, and `0xD8` bytes; animation row `003E9E7C` copies the `0x10C` flame
record. Kind is local to its operation table. The frame table at DDS2
`003E9950` / DDS1 `0037E8A0` has lengths `80,8C,9C,80,88,A4,A8,98`.
Keep the point record embedded in its class work at its own `0x88` size.

`FileSlot` and `FileSlotTable` are the primary `0x20` cell and `0x2C`
runtime owner shared by the file providers and billboard consumers.
The serialized `FileKeyBlock` owns color/alpha tracks at `+0x2C/+0x50`,
including the surface-index word at `+0x54`; billboard openers consume its
signed low half, while the strip output copies the entire word. This
narrow consumer does not establish a second mode-record view.

`FileKeyBlock.spawnRate` at `+0x24` remains unsigned: native emitter
samplers use unsigned-to-float conversion. DDS1 capacity helpers instead
interpret their factors as signed words (`LW` and `MULT` at `002A5A58`,
also `002AF560`), so those products explicitly convert the rate to `s32`
before applying their unsigned capacity cap.

Native ribbon allocation uses `repeat * 4 + 4` position cells, each
16 bytes, and UV cells of 8 bytes. An inactive particle advances its
position pointer by `rowStride` and its float UV pointer by
`rowStride * 2`, not four times those distances. Earlier emitter parks
also omitted real position-vector stores: vortex normal Y is
`climb / segmentCount`, column vectors contain their current/next vertical
positions, and flame vectors contain the updated vertical position.
Retail COP2 loads consume these lanes; restore them before comparing any
parked source, rather than treating the remaining differences as regalloc.

## Actor motion duration metadata

DDS1 `001D6050` reads an unsigned frame count at status-record
`+0x2E + index * 0x14`, which is `BtlActorMotionSlot.frameCount` at
`+0x02` in the existing `+0x2C` motion array. It divides that count by
the slot's `alphaFrameScale` and the singleton's `modelFrameScale`,
then converts the result to a signed frame duration. Reuse that primary
motion owner rather than introducing another status-table view.
DDS2 `001E2E58` uses the same slot metadata and calculation, with the
singleton scale at `+0x4C8` instead of DDS1's `+0x494`.


## Actor-panel reserve initializer fields

DDS2 `001C2450` and DDS1 `001B7238` initialize the four reserve entries
at `BattleActorPanelWork + 0x7C0`, not the three active entries at `+0x0C`.
The presentation words at `+0x08` and `+0x0C` are cleared and set to 180
respectively. Their meaning is not established, so they remain `unk08`
and `unk0C` in the existing presentation owner.

The selected reserve index at work `+0x7BC` is a signed halfword:
DDS2 `001C269C` loads it with `LH` before indexing the reserve row.
Keep that cursor distinct from the party-record index at `+0x7BE`.

## Battle effect allocation and shared sound ownership

DDS1 `00160958` and DDS2 `00168548` allocate and clear `0x128` bytes.
Their object is the same owner that `sndUnlinkVoice` follows through
`next` at `+0x124` and that the frame helpers update at `+0x10`:
one `BattleEffect`, not a sound-list prefix cast to a separate effect.
The pointer at `+0x04` is its real `SoundMixer`; release decrements that
mixer's active count at `+0xC3C`. The allocator narrows its second
argument with `ANDI 0xFFFF` before bank selection and the `SH +0x1C`,
so its formal is `u16 kind` in both games.

`btlReleaseEventData` legitimately reuses one `void *` temporary for
two different owned allocations, the effect and then its mixer. Keep
that generic release local rather than adding casts between unrelated
owners; the public release prototypes retain their concrete types.

## Battle-group extent output contract

DDS1 `001F66D8` and DDS2 `00208000` take an `s32` group mask and two
nullable `f32 *` outputs, and return the extent in `$f0`. DDS2 `+0x248`
and `+0x250` store the float outputs with `SWC1`; integer-address formals
at these positions obscure the real API.

## Font glyphs, draw controllers, and message-window ownership

The font parent pool allocates `0x44`-byte `FrFontGlyph` nodes. Their
`previous` link is at `+0x24`, `next` at `+0x28`, and child chain at
`+0x1C`. The packed word/halfword/byte members are the existing font
owner's actual representations, not alternative controller views.
`FrFontCtx` is a separate `0x20`-byte encoded-text draw controller:
input bytes at `+0x10`, glyph chain at `+0x14`, and cursor at `+0x18`.
Glyph-only setters must receive the glyph, not a cast to that controller.

Message-window text status is the signed byte at state `+0x34`.
The three resource fields at `+0xA4` are `UiSprite *` objects; their
release helper owns the sprite's payload and record allocations.
Replacement-text slots pair relocated byte addresses with `SdfMemBlock *`
allocation handles. Retain/release use the memory-block owner, not the
unrelated linked named-resource `SdfResource`; the paired-slot release
helper receives the same `ItfMesTextSlots` used by its allocation producer.
The default/shared-flags glyph draw wrappers forward the renderer's
real measured-advance result, which the sound-selector UI tests before
advancing its message and fade state. Both games' initial 16 `itf.h`
includers gate with zero differences after this owner/type closure.

`frFontMeasureGlyphChain` receives the parent `FrFontGlyph *` and returns
the existing unsigned sum of child advances and signed spacing. Utility
draw helpers carry glyph pointers, not integer handles or controller views.
The colored-text resource itself is one serialized `FrFontTextBank`; its
getter returns the bank's address, which the layout helpers consume directly.

DDS1 `0019EE58` borrows the same embedded sound selection and fade owners
as the matched DDS2 `001A6E88` renderer. Its two-point `DrawVertex` bounds
and four-word `DrawColorRec` values use the existing packet interfaces.
DDS1's textured row ends at `y + 0x88`, but its outline ends at
`y + 0x78`; unlike DDS2, those corners do not share a cached bottom.

DDS1 `0019E4F8` reads the selection phase as a signed halfword and stores
`-1` when the closing motion finishes. Its entry Y coordinate is also
signed: the closing arm compares it with `0xAF8` before scrolling.
The current entry bottom uses `entryBlock.unk16`, not the table count
at `+0x1A`; the selection countdown is initialized before phase 3.
All 30 current `itf.h` includers remain exact with these two signed fields.

## Solar overlay state and kernel value boundary

DDS1 `00228A00` and DDS2 `002436A8` allocate `0x104` bytes: a sprite
handle at `+0`, followed by the `0x100`-byte animation state at `+4`.
Its eight six-byte points start at work `+0x0C`; the two `0x60`-byte
noise states start at `+0x3C` and `+0x9C`, and the cached phase is at
`+0xFC`. These are one `SolarOverlayWork`, not separate visual-table
and point-owner views.

The phase getter returns `s32` even though it loads a byte field.
The transition caller's `ANDI 0xFF` establishes the wider return
contract; the provider's `LBU` alone does not establish a `u8` return.
The work initializer only writes the two noise geometries, their
intervals and `unk0E`, plus the cached phase; it does not reset all
animation fields.

Solar scheduler callbacks use `KwlnTask *` and the kernel's `s32`
update result. The work pointer crosses the genuine `u32` user-value
API once on set/get, and a returned next-update address crosses the
same encoded-word scheduler boundary.

## Battle mode payloads and actor contracts

DDS2 battle work `+0x718` owns a mode-specific allocation, not always a
linked actor state. The allocator at `00229728` uses 12 bytes for mode
779, eight for 782, four for 785, 24 for 786, and two for 787. Its mode
795 branch allocates only one byte for the statistic selector.
`BattleEffectPayload` keeps the observed payloads distinct; only the
mode-786 linked state treats its first two words as actor pointers.

The linked state's `+8` word is read by `btlGetEffectValue`, while boss
routing at `0021C17C` and `0021AC5C` reads signed bytes at `+8` and `+A`.
These are genuine word/byte representations of the same storage.
Both games' `btlGetEffectActor` returns `BtlUnit *`, and the actor
predicate and target comparisons use that pointer contract directly.


## Event-viewer pointer and key storage

DDS1 `evtViewerHasUpdateFlag` and `evtViewerDispatchFlagMode` take the
primary `EventViewerState *`; their retail bodies read its flags and
glyph position directly. Kind-18 dispatch at `0022CED0` reads the
track's signed resource argument at `+0x14`; voice-conflict handling
saves the current glyph position in the viewer's signed word at
`+0x2480`. The existing primary owners expose those fields without
additional views. The dispatcher's calls into claimed `0022FB30`
retain its existing address-word interface at that single boundary.

## Source-effect frame counters and category-camera overrides

The source-effect task's `+0x18` word is a signed elapsed-frame counter,
not an unsigned duration. DDS1 `001F1CC8` and DDS2 `00202958` use signed
`slti < 12` and direct `cvt.s.w`, then increment it every update. The
signed word at `+0x1C` counts the twelve fade-out updates. Its owner
at `+8` is both a unit pointer and the encoded field-color selector key,
so it uses the existing documented `ActorEffectOwner` union.

The category-camera override at battle state `+0x630` (DDS1) /
`+0x668` (DDS2) receives the linked command and two presence flags.
Native `001DDF20` / `001EB5B0` derives them from the linked index
list's actors (`flags & 0x200`, `flags & 0x400`); a nonzero callback
result handles the camera selection and skips its fallback.


## Mantra-scene scroll coordinates

DDS1 `00251260` clamps the selected scene's scroll position through
`00250E88`. The primary `MenuSceneWork` owns signed halfword coordinates
at `+0x5A4` / `+0x5A6` and an unsigned bounds-flag byte at `+0x5AC`;
the completed tail still fits the existing `0x5B0` allocation.
The two widened coordinates form a local `s32 position[2]` passed by
axis to the clamp routine, then stored back to the signed halfwords.
Bounds flags select maximum X in priority order 4, 2, 1; maximum Y is
`0x38E`. The grid cell's generic value word supplies the selected
`MenuSceneEntry *`, whose unsigned scene ID indexes the placement table.

## Mantra-scene glow and particle ownership

DDS1 `00255E08` uses the primary `MenuSceneMetadata.sparkles` at `+0x1CC`.
`include/mnu_scene.h` owns this `0x6C` state and its eight `0x0C` particles;
the complete scene remains `0x248` bytes, with currency still at `+0x23C`.
Normalize the countdown first, then subtract that fraction from `1.0f`,
as in the matched DDS2 `00285788` timer calculation. This preserves the
retail float register/scheduling shape without artificial temporaries.
The sine phase and base opacity use `3.14159265f` and `0.2f` literals,
not extern globals for their `.lit4` pool entries. Scale the glow opacity
by the incoming alpha before the integer sprite-renderer argument.

## Face-body task pointers and runtime sequence handles

DDS2 `001E74A0` allocates an eight-byte task packet. Its callback
`001E7438` reads both words as `BtlUnit *` to obtain their body positions;
the second word is a target actor, not an integer option. The dedicated
`BtlFaceBodyTaskArgs` owns those two pointers.

`btlAdvanceRuntimeSequenceCounter` returns `u64` in both providers.
Its internal signed overflow test keeps the returned handle positive,
but callers retain the full eight-byte value: `0021C1FC` stores it with
`sd` into the runtime task's `ownerId`. Extern declarations follow the
provider rather than narrowing or adapting this opaque handle.

## Retained actor task constructors use real pointers

`btl_unit_tasks.h` is the shared constructor interface: `btlCreateUnit`
returns `BtlUnit *`, and model-load, base-light, position and rotation
constructors return `BtlRuntimeTask *`. Their actor operands are pointers,
not serialized integer helper arguments. DDS2's rotation constructor has
an additional byte-sized mode argument; DDS1's constructor has three
arguments.

DDS1 `001D7B60` and `001D7CF8` build the existing position/rotation
argument packets through their `from`, `to`, `rate`, `t`, `count` and
`unit` fields. The scheduler's retained actor identity is the native
eight-byte `ownerId` at `+0x40`.

DDS2's dispatcher at `00229728`, mode 779, installs the hero-load and
subtask hooks and allocates a zeroed twelve-byte actor-selection payload.
Those helpers use `effect->selection.unit`. Mode 786 instead owns the
linked-effect payload and uses `effect->linked.actor`; neither helper
needs a mode-blind reinterpretation of the runtime payload.

## Referenced sound effects share the field-selector interface

`eff_field_color.h` preserves the selector provider's four argument types.
DDS2 `effBTLFieldColorTestFlags` returns `u8`, matching its provider; it is
not an integer-return adapter. Both field-selector providers and their
actor-effect users include this interface.

DDS2 `00201C98` uses `SoundEffectReferenceArgs` and the primary actor's
`effectLink` fields. Its two retained owner words also serve as selector
keys, as documented by `ActorEffectOwner`; no additional actor prefix
view is needed. The sound duration can be forced to 35 frames by the
actor's packed effect state, then updates the effect and task limits.

## Indexed retained billboards return their SDK address word

`effRetainResource` creates a billboard, binds the indexed owner's
`entryList`, and increments that resource's reference count. Its returned
`u32` is the established billboard handle/address-word interface, not a
new pointer-argument adaptation. DDS2's provider already returns this
value explicitly; DDS1's equivalent provider must return it too.

The particle shared-resource constructors (`002B4798` / `002F99E8`)
store the return register in their native `billHandle` word at `+0xA4`.
The DDS1 provider's void definition omitted a real return contract even
though its existing consumers use that value.




## Font root, retained UVs and original prototype scope

`frFontWork` has one `FrFontSystem` owner in both games: nine resource
entries, cache/pool controls, the atlas at `+0x160`, six image-buffer words
at `+0x178`, and two glyph queues at `+0x194`. The retained resource header
owns the dimensions and glyph-count bound; DDS2 places its lookup offset
eight bytes later than DDS1.
The six image-buffer words are unsigned: the font sprite renderer loads its
selected CLUT buffer with `lwu` before shifting it into the 64-bit TEX0 word.

`FrFontGlyph` genuinely gives its `+0x1C` and `+0x20` pointer words two
roles. A parent stores its first/last child, while a drawable item stores
a retained `FrFontRecord` or a borrowed source record. The retained
record's `FntNode` owns four UV words at `+4`. Native `00193D70` /
`0019BA00` receive their address as the ninth argument, not a glyph Y
coordinate; their width and half-height arguments are full signed words.

The interface font TUs originally call `frFontListInsert` without a
prototype in scope. Preserve that C89 implicit-call boundary: the
callee is defined as `void` in the game font TU, but publishing its
prototype to the interface TUs changes the retail caller's post-call
counter register. This is original prototype scope, not a false return
declaration.

The selected queue can use direct indexed load/store through
`frFontWork.glyphSlots`; no byte-offset view is needed. Glyph construction
uses a named `FrFontEntry *` for the selected resource's glyph-count bound.


## Mantra tutorial fields

DDS2 `002933F0` uses the existing `MenuPanelState`'s signed timer at
`+6`, unsigned resume state at `+7`, signed message dimensions at
`+0x558/+0x55A`, and selected-node pointer at `+0x5DC`. The enclosing
object's message-window word is at `+0x44`; the state remains at
`+0x240`. These are primary-owner fields, not a second tutorial view.
`00292B90` and `002917C0` are void transition constructors; the
tutorial's actual status-polling helper is `00292CF0`.


## Actor-panel secondary transition state

`BattleActorPanelPresentation.transitionState` at presentation `+0x70`
is signed. DDS1 `001BA660+0x28` reads the byte with `lb`, not `lbu`,
before dispatching the secondary pulse phases. The existing C consumers
only store the closing phase value `4`; the signed type preserves that
work without changing the allocation or introducing another panel view.


## Camera-command scale and corner-frame work

The corner-frame task passes the battle state's embedded camera command at
`+0x70`, not a separate scene object. Its expansion reads
`BtlLinkedCommand.exponentialRange.end` at command `+0x138` in DDS1
and `+0x15C` in DDS2. This is the exponential updater's progress, not
a separate panel scalar. The eight-byte exponential work is followed by
the twenty-byte quadratic accumulator at `+0x13C` / `+0x160`. The latter
consumes the remaining twenty bytes of the enclosing state's opaque gap;
every later state offset is unchanged. These are the only by-value
command embeddings, and neither command has a `sizeof` consumer.

The existing unit-local `BattlePanelEdgeWork` owns both signed phase bytes
at `+0x30/+0x31`, the frame timer at `+0x34`, the anchor vector at `+0x40`,
four corner vectors at `+0x50`, and the signed alpha at `+0x90`. The corner
advance helper's first argument is the same camera-command pointer even
though that helper does not need to read it.


## Event-viewer group window row limit

The type-`0x18` group viewer counts two fixed rows separately from the
group chain; the type-`1` viewer counts three. Initialize the visible-row
limit to the window's capacity, then reduce it when the full count is
smaller. DDS1 `0023BB20`/`0023A688` and DDS2 `002569D0`/`00255538`
use capacity 15 and the same cap-first clamp as the matched neighboring
world-node viewer. Keep the full count for `kwlnStepTwoListCursors`; pass
the real `EvtRuntime.groupFirst` and `groupCursor` addresses to its pointer API.

## Serialized waypoint and actor block

`fld_waypoint.h` owns the complete WAP payload: five DDS1 or eight DDS2
`0x20`-byte headers followed by 256 canonical `FldActorEntry` records.
The loader/copy lengths are `0x6CA0` and `0x6D00`, and the actor arrays
start at `+0xA0` and `+0x100`. Header `sequenceArg` is a signed word:
the actor-trigger sequence constructor reads it with `LW`; the short
getters begin at `+4`, so no dual-use union is warranted.
Use the block's `headers` and `actors` members, including the selected
actor's motion, rather than separate interior-address aliases or a
second word-array view of the copied allocation.

`fldApplySceneRoomSelection` takes the same `FldActorEntry *` in both
games: its signed-byte reads are `selectedRoom` at `+0x53` and `rowIndex`
at `+0x45`. DDS1 stores these into the primary `FldAreaWork` fields at
`+0x88` and `+0x58`, not into a separate word-array view.

## Result fade rows and profile icons

`BrsSkillPackageWork` owns five `0x28`-byte fade rows at `+0xDA0` in DDS1
and `+0xAF20` in DDS2. Their background state and opacity fields are at
row `+0x14/+0x1C` and `+0/+8`, respectively. DDS2's profile-icon
renderer reads the existing `0x68`-byte `profileAnimation` rows at
`+0x4C/+0x50`; it does not need a second overlapping title-menu view.
The fade updates remain ASM: direct canonical accesses fold the row base
by `0x10` differently from retail, although the effective accesses agree.

## Frame/depth packet builder callback

`SdfPacketBuilder.prepare` receives the builder and the selected buffer
index: both games' indirect callers deliberately set `$a0/$a1`.
Its `source` is the canonical `SdfGraphObj *`, and `frameMask` is the
upper FRAME register word at builder `+0x54`. The initializer's formals
follow those real field types. The callback itself remains ASM: honest
64-bit GS packing with the matched neighboring convention differs in the
width rounding shift (`dsra32` versus retail `sra`).


## Mantra node tables and profile requirements

DDS2 `func_00315950` and `mnuIsResourceCategoryAvailable` use the canonical
`MantraNodePos` from `mnu_mantra.h`. Their whole-word tag reads belong to
`selector.packed`, and their six links belong to `neighbors`; a separate
header/adjacency view is unnecessary. This owner fold leaves
`code_00313BB8` at `94 match, 0 differ`.

The menu's eight-byte unit entries contain an eight-bit `nodeId`, not a
13-bit ID. `00289CA0` and `00289CE8` store that byte; `00289CB0..00289CF8`
separately update the four-bit kind and marked bit with `LHU`/`SH`.
Keep these fields in the same `u16` bitfield container, with the ID occupying
its aligned low byte. No union or alternate entry view is needed.


## DDS1 HARI form counter

`BattleEffectState`'s first word is mode-dependent. HARI1 uses its
`formCount` halfword, not the actor-pointer or stat-selector view.
`btlQueueHariFormChangeOrPartyCommand` checks the three-change limit and
increments it; `btlGetEventEffectValue` and the special-enemy phase gate
read the same halfword. The union preserves the existing actor/timing
layout without treating every mode's payload as an actor.

## Resource-backed task names and script lookup

The field task registries contain `FldFileResource *` entries in both games:
their ID, name, transform and payload are at `+0`, `+8`, `+0x10` and
`+0x20`. `fldGetTaskRecordValue` forwards the name pointer, not an integer
attribute; actor lookup and script commands pass it to name-based consumers.
Use the primary record's members rather than word indexing beside that owner.

The enabled `scrScriptProcess.c` definitions return `ScrData *` from
`scrFindNamedProcessNode(char *)`, while `dds3SceneBasic.c` defines
`evtDestroyNamedTask(void *, const char *)`. Their field callers must keep
these pointer contracts. Clients that only compare the lookup result with
NULL can use the canonical incomplete `struct ScrData *` tag without adding
a second script-data definition or importing the complete script header.


## Reward-panel glyph pointer contracts

DDS1 `code_002649B0`'s formatted reward text retains the actual
`FrFontGlyph *` returned by `func_001979C8`. The font measurement provider
returns `u32`, and `frFontSetContextPair` receives that glyph pointer with
two `u32` coordinates. Importing the canonical interface declaration removes
the caller's old integer-handle prototypes and pointer casts without changing
either enabled text routine. The three-row reward renderer remains ASM:
the true-pointer candidate still differs in five instruction words, involving
the final color-array store and the second text call's argument scheduling.

## Action-camera helper contracts and legacy arity

The camera core (`001DEFE0` / `001EC868`) receives a
`BtlLinkedCommand *`, a `BtlCamState *`, and an `f32` framing parameter.
Its fallback wrappers pass `27.5f`; the tilt wrappers pass `20.0f` and
operate on the pose's direction vector. Their callers pass the actual
embedded camera members, not integer handles or alternate effect views.

The category predicates also receive the linked command, not a `BtlUnit`:
their first load is command `actionCode` at `+0x114` in DDS1 or `+0x134`
in DDS2. The flag-0x100, type-two and flag-0x40 providers are
`001DD198`/`001EA620`, `001DD1C8`/`001EA650` and
`001DD348`/`001EA800`; the two byte-result comparisons at
`001DD488`/`001EA940` and `001DD498`/`001EAA08` use the same owner.
Keep these pointer contracts at every call, without integer transports.

DDS1's preset constructor `001E6BB0` sign-extends its two table coordinates
at `001E6BB4/001E6BD8` and `001E6BF4/001E6C40`: use `s16` formals.
The adjacent entry constructor `001E6668` multiplies the full incoming
index and compares the full kind instead; its coordinates remain `s32`.
Both receive the primary command and camera pointers. The height clamp
`001EB1B0` retains the unused command cookie and its two `s8` bypass flags.
Transitively typed cursor callers pass `&command->camera`; the linked-unit
initializer retains the actual state-owned command at `state + 0x70`,
not an integer address or a second command view.

The corresponding DDS2 constructors `001F5868` and `001F5320` retain the
same distinct `s16` and `s32` coordinate contracts. The single-resource,
effect-trigger and motion-reset helpers likewise take the command owner.
DDS1 `001D2A10` stores the full action-classification result at index-work
`+0x24`; that scalar is `BattleIndexWork.slot`, read through its task owner
by the trigger helper. The side-indexed status-table API is unchanged.
DDS2's reset-cursor caller `001B9A10` leaves its status-test link in `$a2`,
but `001FFF68` overwrites `$a2` before use: the initializer consumes only
the command and its camera, not a third actor argument.

DDS1's small camera callbacks `001E5460` and `001E5700` retain the
command/camera signature even though they do not consume the camera:
`001DD890` explicitly sets `$a1` in the calls at `001DDAEC/001DDACC`.
The blend selector `001E5730` similarly retains both pose arguments;
`001DDC38/001DDC40` explicitly supplies the front and back cameras.
These are unused formals in the real callback family, not integer adapters.

DDS2 `btlUpdateActionPoseForLinkedTarget` deliberately leaves the target
unit in `$a2`: native `001EBD6C` loads it there before the tail call at
`001EBD88`. The fallback `001ECBF8` only consumes `$a0/$a1`, forwarding
those two pointers to the camera core with `27.5f`. Keep an unprototyped
declaration ahead of that genuine three-argument legacy caller and the
typed two-parameter definition afterwards; do not invent a third formal.

The three default blend handlers (`001DB358/360/368` in DDS1 and
`001E8568/70/78` in DDS2) retain an unused `BtlLinkedCommand *` formal.
Each has one retail caller: the call delay slots at
`001DBF60/001DBF9C/001DBFCC` and `001E9228/001E9264/001E9294`
pass the state-owned command at `state + 0x70`. Their native bodies
simply return one; the unused context is part of the common handler
contract, not an omitted argument or an invented body.

Both quadratic range providers receive a `BtlScalarRange *` and an `f32`
time step. DDS2's half-blend helper computes the range address at
`001E8670` in the initial-state branch's delay slot; that same `$a0`
reaches the quadratic step call at `001E86B0` on the other branch.
Do not keep the former one-float declaration that matched accidentally
because this native range address was already present. The four blend
helpers receive the command and pass its actual embedded interpolation
work. DDS1's `+0x128` progress word has the same bits-zero initialization
and float-interpolation use as DDS2's documented `+0x14C` union.

## Local-map request rings

Both request-ring constructors allocate a 0x44-byte prefix followed by
0x20-byte nodes. `MapRequestState` owns the handle, three node cursors,
halfword counters, and callback; `MapRequestRing` supplies the allocation
layout. Share these owners from `fld_lmap_task.h` rather than redefining
the DDS2 queue as an integer array beside its node view.

The progress callback receives the node's three payload words, the state
pointer, the node pointer, and an `f32` progress value. Native callbacks
and the indirect call agree on all six arguments. Creation returns the
state pointer, release uses its handle, and interval/producer calls keep
the state pointer end-to-end.

## Viewer paired-file export

DDS1 `0023E7F8` saves the PM2 and PM3 files with separate 64-byte path
buffers. PFS debug mode passes open flags `0x602` and mode `0666`;
the PC path uses the same flags without the optional mode argument.
Both descriptors are checked before the common headers are emitted.
The native pre-save walk follows every group `next` link without any
per-group operation; do not add a synthetic counter or discard that walk.

The runtime's keyboard name at `+0x22D4` and entry-list cursors at
`+0x22F4/+0x22F8` overlap the command's 20-byte event name at `+0x22E8`.
Model these mode-specific uses with an embedded union and migrate the
keyboard/list consumers to its members, not a cast-based second view.
The PM2 section switch physically places section 24's type-27 writer
before section 21's metadata writer in retail (`0023EC08`/`0023EC20`).
PM3 still scans all 26 section numbers but writes only section 4.

## Actor effect world-node ownership

DDS2 `BtlUnit.effectObject` at `+0x33C`, like DDS1's member at `+0x31C`,
is an `EffWorldNode *`. The actor-model creator returns that node; effect
origin consumers pass it to the SDK vector helpers, and the scale updater
uses its `inner` transform. Keep it pointer-typed through construction,
removal, flag operations and vector calls, without integer transports.
DDS2's actual flag providers take `(void *, u32)`; the node-removal
provider takes `EffWorldNode *`.

## Primary actor status is a complete two-word record

Both `BtlUnit` owners embed `BtlUnitStatus` at `+0x110`: signed `s32 flags`
followed by unsigned `u32 stateFlags`. Scalar consumers use those primary
members. `btlUnitStatusPair` observes the complete eight-byte record by
ordinary `memcpy` into a local `u64`; it never widens a scalar subobject or
casts its address to another record or union. This replaces the inherited
`BtlUnitFlagPair *` second view and the former union-only recommendation.
A complete-record copy is an alternative to a genuinely embedded union,
not permission to introduce a cast-based union view.

The record has size eight and alignment four; the actors retain their
original alignment eight, sizes `0x348` / `0x368`, and every existing field
offset. The low word stays signed. A scalar record write keeps the ordinary
word alias behavior, while the compiler lowers the complete-record copy to
the native `LD` observation. Do not replace the ordinary copy with an alias
attribute, special builtin, memory barrier or per-file flag. Qualification
requires complete owner, projection and provider/caller review, alias and
layout checks, all affected whole units and both linked images.

The stiffen-damage callbacks in both games explicitly borrow the actor for
each status test and its selected position provider. Capture it immediately
before that test, after any earlier random/provider calls. Only that test
and its provider use the captured actor; later `task->unit` observations
remain fresh after the provider. This expresses the native pre-provider
reuse, not an extended snapshot lifetime or a synthetic callback barrier.

DDS2's resource/sound queue is a separate nonactor link boundary. Its
verbatim inherited `ActionUnit` declaration remains solely for link flags
at `+0x0C` and an actor address at `+0x18`; the selected actor and actor child
use `BtlUnit`. No actor-status access uses that private declaration. The
canonical `ActionStateLink` union and its inherited callback/projection
type debt are unchanged; primary actor-status closure does not claim
ownership or callback-type closure for that separate allocation.

## System-effect flags and reference count

DDS1 `001F1110` loads the complete `BtlUnit.effectLink` pair at `+0x310`
with `LD` (`001F1194`), then tests its low flag bits. Other effect callbacks
access the same pair with `SH` flags and `SW` reference counts. Use the
existing embedded `BtlEffectLinkState.packed` member; do not widen the
scalar flags or introduce a second actor view. This genuine owner is
distinct from the complete primary actor-status record above.


## GS graphics transfer-worker control

DDS2 `0032A230` uses the separate control bytes `D_00438A1C` and
`D_00438A1D`, not `sdfBusyBufferIndex` or the `sdfTextureUpdateQueue`
object. The transfer drain rechecks `D_00438A1C` after `SleepThread`;
`sdfWaitSlotReady` polls volatile `D_00438A1D`. Keep both control bytes
volatile, clear the active byte before testing the reset byte, and retain
the existing volatile GS image-upload semaphore contract.

DDS1 `002D1380` has the same protocol with `D_003BD32C` and
`D_003BD32D`. Retail `002D51B0`/`002D5218`/`002D5258` repeatedly
checks the reset byte while polling hardware; `002D52DC`/`002D5338`
reloads it after sleeping, and `sdfWaitSlotReady` polls the active byte
at `002D54B0`. These real shared controls justify the volatile byte
contracts; they are not scheduling-only qualifiers. Its GS `BUSDIR`
write at `002D148C` is a genuine 64-bit MMIO access.

The worker acknowledges both graph requests before calling their handlers.
Its reset path snapshots DMAC `D_ENABLER` before writing `D_ENABLEW` and
stopping VIF1, then restores that enable state. Reading the enable register
after those writes is a different hardware protocol.

## Inclusive lowest-HP target selection

DDS2 `btlSelectLowestHealthElementBlockTarget` uses `sltu best,current`
at `00215E38` and skips the update when that comparison succeeds.
The condition is therefore `current <= best`, not a strict comparison:
an equally healthy later eligible unit replaces the earlier candidate.
Keep the unsigned 32-bit bound and the 16-bit current HP; exchanging the
operands of the equivalent inclusive comparison does not change tie-breaking.

## Inner-transform SDK vector boundary

The matched `effObjSetInnerFirstVec` and `effObjSetInnerSecondVec` providers
take `(EffWorldNode *, u128 *)` (DDS1 `0010F6E0`/`0010F710`, DDS2
`0010F908`/`0010F938`). They copy the packed vector through `PCP_COPY_VECTOR`
into the primary `ObjectTransform.position`/`rotation` float arrays.
The battle callers' float-vector scratch arrays remain float arrays; the
`u128 *` conversion belongs at this SDK boundary, not in a second owner view.

## General-heap descriptor and represented-address boundary

The matched `sdfMemory.c` providers in both games distinguish the descriptor
from its represented data address: `sdfAllocGeneralBlock(s32)` returns a
`SdfMemBlock *`, while `sdfMemoryGetBlockAddress(SdfMemBlock *)` returns a
`u32` address word. Task-resource and grid `allocation` fields retain the
descriptor pointer for destruction; they are not integer allocation indices.
Convert the returned address word once at the accessor boundary to the
appropriate work pointer. Do not declare the accessor as returning `void *`
or the allocator as returning `s32` to suppress that real SDK boundary.


## Mantra icon-pool allocation boundary

DDS2 `mnuAllocateMantraIconPool` (`00275510`) allocates a `0x14`-byte
`MantraIconPool` header followed by `count` twelve-byte `MantraIconEntry`
records, clears that entire allocation, and returns the represented pool
pointer. Keep the heap descriptor pointer until the SDK address accessor,
then convert that address word once. Express the allocation and first-entry
address using the actual owner types; the three icon-list constructors consume
the returned `MantraIconPool *` without integer-to-pointer casts.

## Mantra asynchronous resource-loader ownership

DDS2 `00287078` passes the stored `MtrResourceLoadState.fileEntry` directly
to the file-request readiness, data, size, handle and cleanup APIs. Keep
that work pointer opaque rather than transporting it through an integer.
The same loader addresses twelve resource-handle words at
`MnuStatusResource + 0x08`; these are `resourceSlots`, not padding or a
second view of the status owner. The descriptor-table index remains `u16`:
retail uses both `LHU` and explicit signed-halfword extension for table access.

## VU clipped-vertex owner

DDS2 `003379F0` and `00337FD8` index the existing `VuBlendNode` records
with a `0x60`-byte stride. Their position vector starts at zero; category
and clipping bytes are at `0x50`/`0x51`. The existing blend providers load
the node's `0x40` quadword and use its W lane, the `weight` at `0x4C`;
the other three words are the linked-list and source pointers.
Complete this primary owner rather than introducing a second clip-vertex
view. The triangle classifiers remain assembly until their C matches.

## Timed battle-camera instruction banks

DDS1 `001ECCA8` and DDS2 `001FD400` consume `0x10`-byte instructions
and `0x80`-byte parameter records; the latter have a signed terminal byte
at `0x70`. The real progress loop advances the parameter index only when
the time window has expired and that record is not terminal.

The DDS2 cursor keeps one primary `0x130`-byte owner: `pathEnd`/`pathStart`
at `0x20`/`0x30`, captured direction at `0x80`, and distance at `0x90`.
The input motion factor is stored separately from the stepped exponential
range; path distance is the measured length times the stepped factor.
Use the existing VU0 macros for the native COP2 vector work. The target's
own assembly file contains its fourteen-entry jump table, not another
function's data; the natural C switch reproduces those entries.

## Polygon-movie loader word arguments

DDS1 `00234DA8` and DDS2 `0024FB48` preserve the event and scene arguments
as full words, pass them to `evtFormatPolygonMoviePaths`, and store them in
the primary `PolyMovieWork.eventId`/`sceneId` signed-word fields. Their shared
declarations therefore use `s32` arguments, not halfwords.

The resource loader returns a `SdfMemBlock *` descriptor but writes a `u32`
represented address. The synthesized PMD2/PMD3 header providers instead
write `void *`/`u8 *` outputs and return the descriptor as an `s32` address
word. Preserve those actual boundaries rather than inventing narrower loader
prototypes. The two movie loaders remain assembly: the current honest DDS2
candidate still swaps the two argument-setup instructions at `+0x80/+0x84`.

## Callback-list allocation descriptors

`SdfList.allocation` is a `SdfMemBlock *`, not a numeric list index or the
represented data address. A forward declaration keeps that canonical pointer
owner available without relocating the full general-heap descriptor definition.
Its layout remains a `0x1C`-byte callback-list header.

Both `sdfCreateTaskHeader` providers retain the descriptor, convert the getter's
real `u32` represented address once to the list pointer, and release the retained
descriptor directly. Task-work and grid allocation fields follow the same
ownership rule; no pointer-to-word-to-pointer release adapters remain there.


## Serialized field-sky gradient records

DDS1 `00132010` and DDS2 `00134A18` read `0x124`-byte sky records:
a signed mode word followed by an eight-row, nine-column packed-color grid.
The DDS1 loader allocates/reads `0x12400` bytes, or 256 records, and
`fldSetFadeTarget` writes the selected record's mode rather than a flat
`area * 73` word offset. The signed sky-alpha adjustment is the separate
`D_0032E570 + 0x38` word (`D_0032E5A8`), not `FldAreaWork + 0x38`:
the real `fldAreaState` base is `0x0032E3B0`.
The renderers clamp three corners' adjusted alpha but leave the bottom-left
corner unclamped; their ordinary float-to-int C casts emit `CVT.W.S`.
The renderer bodies remain assembly pending a genuine source-shape match.

## Lowest-stat command selection

DDS2 `btlSelectLowestStatTarget` (`002263D8`) receives the primary
`ActionStateLink`: its pending flags, command ID, owning unit and retained
target list are `pendingFlags`, `indexWork.skillId`, `unit` and
`indexWork.indices`. The callback traverses `BtlState.units` directly and
reads the mode-795 byte through `BattleEffectPayload.statIndex`; it does not
need an alternate action or battle-state view.

## Unit position and extent pointer contracts

`btlUnitGetPosVU` receives the primary `BtlUnit *` in both games; its selector
is a byte (`ANDI 0xff` in the native entry). DDS2 `001F5780` likewise reads
the unit's scale, height, reach and `unkC0`, with a byte selector and two
floating-point offsets. `btlGetUnitTargetDistance` passes an actual target
vector to `001FDD20`: that helper performs `LQC2` through both pointer
arguments, followed by two scalar radius arguments in the float registers.
The local declarations and callers use these pointer contracts directly;
no pointer-to-word bridge is needed.


## Canonical grid allocation descriptors

`sdf_grid.h` owns `SdfGrid` and `SdfGridCell` for both games. The grid's
`allocation` retains a general-heap `SdfMemBlock *`, distinct from the
represented data address returned by the memory getter. Destruction releases
that descriptor directly. The canonical header remains `0x34` bytes and each
cell remains eight bytes; the DDS2 units no longer declare separate grid views.


## Event motion completion callbacks

`EvtUnit.motionCallback` at `+0x94` is a function pointer, not a stored scalar.
The DDS1/DDS2 motion drivers call it with the unit and zero in states 1 and 4;
a return value of one completes the motion. The vector-transition providers
retain that same callback, and normal motion setup clears it.

The stored motion index at `+0xBC` and the frame fields at `+0xCA..+0xCE`
are signed halfwords. The driver compares `motionEndFrame` against the model's
current frame, saturates the moving/idle counters at 32767, and clears the
moving counter after six idle frames.


## Scoped kernel task flags

DDS1 `00101060` and DDS2 `00100F48` operate on the canonical `KwlnTask *`
and admit flag bits through `0x0FFFFFF0`. Scope zero updates the supplied
task without a null guard; scope one updates other scheduler tasks, scope two
recurses through its children, and scope three updates every scheduler task.
The three scheduler lists are selected with a signed counter and explicit
start/active/destroy cases. Their traversal uses `listNext`, not hierarchy
`next`. The DDS3 set/clear entry points preserve the task pointer and signed
scope contract directly.


## Forwarded linked-action camera state

DDS1 `001EEAE0` receives a `BtlLinkedCommand *` and a `BtlCamState *`.
The second argument is only forwarded to the two camera constructors; its
caller passes `&action->camera`, not a second command owner. The routine reads
the party ID through the command's task and primary unit owner. Its body remains
assembly because the honest C still differs in switch layout and sibling-call
selection; the truthful caller contract does not depend on that body landing.


## Selected camera-preset motion parameters

DDS2 `001F35C8` copies two 48-byte `BattlePairCameraPreset` records, then
composes their quaternions with the linked unit's rotation through the
existing VU macros. The native sequence reads the selected record's motion
parameter before committing the count reset and camera flags; a named
`motionParameter` local expresses that real read without duplicate work.
Its single forwarding caller receives a `BtlLinkedCommand *` and passes
the embedded front/back `BtlCamState` objects.

The copied 96-byte default bank is the true const `D_004183D8` owner,
containing two existing `BattlePairCameraPreset` rows. Its definition replaces
the old data include at that owner boundary; the routine copies the aggregate
before selecting a row. The retail split's eight zero alignment bytes before
the following jump table are not record fields or artificial C padding.

## Texture-viewer switch-table alignment

DDS1 `00104810` and DDS2 `00104700` display the canonical `SdfTex`
dimensions, nine native pixel-format labels, `resourceKey`, and
`battleTextureSlot`. The two format literals and 45-target switch table
are owned solely by each function's assembly split; the label-pointer
array remains a separate shared object.

The retail table's terminal zero words are alignment, not extra cases.
The natural C switch leaves an eight-byte alignment frontier in the unit
comparator. Main's clean-clone full DDS1 build rejected it with 52 differing
bytes, so the text-exact candidate is parked and production remains assembly.
Do not manufacture cases, data objects, or padding to supply the bytes.
A future landing needs corrected jump-table/rodata ownership and a passing
full retail SHA-1, not instruction equality alone.



## Battle lift/settle task workspace

DDS1 `001D9C28` and DDS2 `001E6BF8` share the 16-byte task arguments:
unit, tick, amount, and velocity. The frame displacement uses the old amount;
the next amount receives a separately calculated delta, while velocity is
updated independently. DDS2 uses `unitLiftPredicate` at `+0x6B4` and the
existing motion selector at `+0x5D4`. The matched DDS1 body is an exact source
donor after these owner and provider substitutions. The falling velocity
literal is `-0.17999998f`, not the adjacent float represented by `-0.18f`.


## Lens-flare copied parameters and vector work

DDS1 `0029C620` and DDS2 `002DE338` allocate a `0x58`-byte work object and
copy the `0x40`-byte parameter record into its `+0x18` member. The draw
callbacks (`0029C748` / `002DE460`) use the existing `SdfColorTrack` and
`SdfAlphaTrack`, load strength at work `+0x4C` as a float, compare the frame
and limit as signed words, and load the flare-set index at `+0x54` with
`LBU`. `EffLensFlareParams` and `EffFadeVectorWork` own these fields in the
narrow billboard header; the file wrapper forwards its work argument and
returns the created object. The vector operations retain the existing SDK
VU macros, with no new inline assembly.


## Strip owner and copied source header

DDS1 `002AAF70` and DDS2 `002EE348` allocate a `0x3C`-byte `EffectStripNode`.
Its file constructors copy `0x20` source bytes into the member at `+0x0C`;
the grid-record provider value is at `+0x34`, and the trailing count is a
halfword at `+0x38`. The same primary owner serves construction, cloning,
release, color/opacity updates, and record forwarding in both games.
The clone uses this owner's active record, not an `EffClassWork` view of
the same address; opacity likewise belongs to the strip owner rather than
a separate `ValPtr34` prefix. Provider-word APIs retain their existing
contracts in this owner-only cutover.


## Shared motion-SE cache owner

DDS1 `001F3E70` and DDS2 `00204B00` allocate the same `0x108`-byte
`SoundSlotOwner`: flags and category/id, the `0xF4`-byte loading work at
`+0x0C`, then previous/next links at `+0x100`/`+0x104`. The work owns the
retain count, pending packed-track key and slot, 29 file-request words,
and 29 resource-handle words. Both sound engines and the battle-model
cache consumers now use `snd_slot.h`, rather than separate flattened
declarations of the same runtime object. The release boundary takes that
owner pointer; the final retain-count transition releases both arrays
before unlinking and freeing the owner.


## Effect-work scale callback ABI

DDS1 `00161A88` and DDS2 `001696B8` return the actor-scale float. Operation
rows are `0x28` bytes; the callback at `+0x14` takes `(payload, f32 scale)`,
not merely a payload. The compact tables route kinds 4 and 6 to the
extended-work scale dispatcher (`00163108` / `0016AD60`), so both dispatch
layers forward the float through a named `setScale` member. Existing PCP
callers already supply the float; their separate legacy handle ownership
is intentionally outside this narrow ABI correction.


## Surface, ribbon, and motion packet descriptors

The `0x2C`-byte globals initialized by the billboard surface, ribbon,
scaly-strip, and DDS2 motion-resource constructors are `EffPacketParams`,
not a separate `EffMotionSetup` prefix. Native halfword stores at
`+0x00`/`+0x02` set parameter/vertex counts, `+0x04` sets the primitive,
and the pointer at `+0x0C` selects the packed parameter table. All nine
globals across the two billboard units now use the same complete packet
owner and its existing members, without a second view.


## Action-camera setup callback and legacy fallback arity

DDS1 `001DEBE0` and DDS2 `001EC418` call the state callback at `+0x61C`
and `+0x654`, respectively, with the current `BtlLinkedCommand *`; a
nonzero result handles camera setup. DDS2 `00229728` installs
`0021E778`, `btlSelectActionCameraByTableFlags`,
`btlSelectRaisedCameraFromActionFlags`, and
`btlDispatchActionByResourceFlags` in that slot. The shared owner names
this command callback `actionCameraSetupHook`, not a unit callback.

The DDS2 fallback passes the same command in both `$a0` and `$a1` to
`001F4E30`, whose matching definition consumes only one command. An
unprototyped `void` declaration before this two-argument call is the
truthful legacy-arity form; the later one-parameter definition remains
unchanged. This form still tail-calls under the unit's native flags,
whereas retail uses `JAL` and a shared epilogue. The caller therefore
remains assembly; an invented integer return contract is not a fix.


## Rotating-quad parameter owner and frame

DDS1 `002A7B68` and DDS2 `002EA120` allocate a `0xD4`-byte
`EffQuadWork`, copying a `0x98`-byte `EffQuadParams` at `+0x30`.
The signed word at `+0x2C` is its frame, not the class-dispatch kind.
The draw callbacks `002A8020` / `002EA5D8` multiply the `+0x64`
scalar-track result by the two scale fields; the `+0x90` track is
the angle. The historical rotating-quad scratch names those tracks
in reverse and is not a semantic reference for that assignment.

The native constructor and owner-duplication methods narrow the
alpha track's signed surface index at `+0x58` to `s16` for
`billSetBillboardMode`; this is an arithmetic narrowing, not a
second halfword view of the record. Billboard/reference/asset
ownership remains at `+0xC8`/`+0xCC`/`+0xD0`. Secondary file kind 7
selects the animation reference; it is distinct from the primary
source-kind word copied to `+0x28`.


## Mantra pulse frame and primary particle system

DDS1 `mnuDrawMantraPulseFrame` (`00257C10`) gets a
`MnuProfileProgress *`, advances the display's `SdfGrid` and frame, and
draws the selected grid at `(-9 - scrollX, 0x45 - scrollY)`. Its inner
grid pass uses depth 1 while preserving the caller's amount. The existing
GS state setup and restoration are the same as its matched neighbouring
grid renderer; no profile operand view is needed.

The identical DDS1/DDS2 particle allocators append a complete `0x2C`-byte
`ParSystem` to the point, colour and `ParCell` arrays. Its cells pointer is
at `+0x14`, and its pending-list link is at `+0x24`. `eff.h` now owns that
unchanged record beside `ParCell`; the allocator translation units no
longer maintain separate definitions.


## Configuration creation forwards its mode

DDS1 `mnuCreateConfigTasks` (`002912C8`) takes `s32 mode` and forwards it
to its constructor, just like DDS2 `002D1300`. Its staff-menu and event
callers deliberately pass 0 and 1. A zero-arity C definition happened to
leave `$a0` intact but omitted this real contract; the formal, constructor
call, and both caller declarations now express the native mode forwarding.
The non-matching configuration constructor bodies remain assembly.


## Action-animation effect classification

The retained-operand flag mapper (`001F0CA0` / `00201828`) reads byte
`+0x02` of each native `0x20`-byte action-animation descriptor. For
reflected/special groups, values 3 and 4 select distinct combinations of
the target/source effect flags. `BtlActionAnimationRecord.effectKind`
owns this byte; it is not padding or a second descriptor view. The
nullable companion units already belong to `BattleIndexWork`, and their
effect flags belong to the primary `BtlUnit.effectLink`.


## Panel options and the controller button bank

`ItfMesBlk40.options` contains four-byte `ItfMesOption` records with signed
`id` and `value` halfwords. DDS2 `001A6AB8` loads both with `lh`, indexes the
signed-byte controller bank at `D_0037F510 + 0x20`, and uses `value` to count
clear bits in `panelValue`. Its caller passes the existing panel selection
record, not a raw byte buffer. The unit's `SndPad` primary now documents that
indexed bank and its named controls as two arms of the same union; the named
offsets and the complete `0x40`-byte layout are unchanged. The callback remains
assembly: the honest typed iterator still differs in six prefix instructions.


## Save-flow allocation and serialized header copy

DDS2's `fileLoadSelectionWork` owns the existing `0x40`-byte `MenuWork`.
Retail `002D0498` reads both decision bytes at `+0x30/+0x31` with `lb`;
the final word retains the `SdfMemBlock *` allocation descriptor, not the
buffer address. The resource getter's `u32` address is converted once at
that API boundary. `fileCopyRecordHeader` copies the first `0x30` serialized
bytes from either directory metadata or `DatStateHeader`, whose additional
runtime words are not copied. A `const void *` source and fixed-size
`memcpy` express this byte-copy contract without inventing another struct
view of those primary owners; the provider remains text-exact.

## Battle query sibling-call option boundary

DDS1 `001FF0C8..00202178` contains 80 functions, seven native
`jal`-plus-epilogue tails and no function-directed `j`. The preceding
`btlRunWeightedAiAction` uses a sibling call; the next function,
`btlUnitBlocksElementQueryForGroup`, requires default flags in its matched C.
As with DDS2 `00211360..00214948`, this supports a separate
`-fno-optimize-sibling-calls` translation unit, not wider fabricated returns.
The larger no-`j` run ending at `00205EE0` is refuted by that default-option
function. The six short wrappers can retain the same natural `s32` contracts
as their matched DDS2 twins. Native constant ownership places the read-only
boundary at `003A5AD0`; all literal floats and small data remain in the
default-option successor. This is an evidence-inferred option boundary, not
an independently recovered Nocturne `__FILE__` boundary.


## Sound-selector manager update

DDS2 `001A76C8` traverses the existing `ItfMesGlobals.pool.activeHead` list,
passes each node's stored state address to the true `void (ItfMesState *)`
panel updater, then increments the manager's `unk8` counter. A nonzero result
from the zero-argument `001200E0` gate skips both operations. The natural
early return matches all 112 native bytes; the older gated-block candidate
had eight differing words. No alternate list owner or callback signature is
needed.

## Save confirmation restores maxima before currents

DDS2 `002D0498` walks the five canonical `DatPartyRecord` entries in
forward order and re-reads `datGameState` across the selector/stat calls.
Its natural early guards and signed index let gcc derive the native
countdown. After computing maximum MP, the source sets `maxMp` before
restoring current HP and MP; ee-gcc schedules the native stores to
`mp`, `hp`, then `maxMp`. Named `u16` locals preserve the actual occupied
flag narrowing and current maximum-HP read. No alternate record view,
artificial counter, or register-control construct is needed.


## Callback returns and skill-runtime visibility

DDS2 `0030A8A8` receives its `KwlnTask *` in `$4`, forwards it to the task
user-value getter, and propagates the controller's `-1` result. Its real
contract is `s32 (KwlnTask *)`, not a zero-argument void callback. DDS1
`0027B888` takes the primary `MenuList *` and returns its cursor pointer;
the destructor's pending value is therefore `MenuListNode *`, not `s64`.
The legacy terminal caller in `code_00248580` retains its old extern until
`MenuTerminalWork.list` and `MenuProgressOwner` receive a separate primary
list-owner cutover; no cross-type cast was added to disguise that debt.

DDS1's `0x38`-byte `SkillMenuRuntime` is declared before the skill-refresh
draft that uses it. The callback's context is `CampMenuContext *`, and its
selected party list belongs to `partyWindow.lists[0]`; neither fact requires
a second context or runtime view. These three target bodies remain assembly
until their independent instruction differences are resolved.

## Timeline clamp halves share their parameter union

DDS1 `002429F0` and its exact DDS2 twin `0025DE08` walk the existing
`EvtRuntime` groups and keys. Duration columns update
`EvtRuntimeChild.duration`; the supported end-offset kinds
use the two signed halves of the existing `p08` parameter union. Both clamp
writes therefore belong to `EvtViewParam`, rather than an independent
scalar beside a one-half union. This primary-owner closure reproduces the
native group-type reload at both clamp joins and both generated switch
tables with ordinary C. Unsupported end-offset kinds retain the previous
`end` value, as the native traversal does.

## Action-light integer-address dependency

The primary action-animation record is 0x20 bytes: byte 0 is `cameraKind`,
byte 3 is `kind`, +8 is `f32 lightColorMode`, +C holds three light-color
floats, +18 is `s32 defaultValue`, and +1C is the flag halfword. A private
`BtlActionAnimationRecord *datActionAnimationRecords` consumer cutover
leaves just one word different in both `btlBuildActionLightParameters`
twins (`001EF6B8` / `00200290`): +40 becomes `daddu`, not native `addu`.
The matched integer-address expression multiplies the index by `sizeof`
before adding the table address. All four retail callers supply the captured
signed `indexWork.skillId`; an unsigned formal is not independently proven.
This suggests an original integer-address calculation or differently typed
index. The pointer closure remains private; no steering cast, byte view,
unsupported unsigned formal, or regression of the matched builders is kept.

## Event-vector record copying

DDS2 `func_00115358` builds a 48-byte, word-aligned parameter record from
`D_00412950`, replacing its first two vectors with the established SDK
`PCP_COPY_VECTOR` primitive. Retail's two fixed-scratch-register `lq`/`sq`
pairs support that macro use. The full record copy is `memcpy`, not typed
structure assignment: the byte-copy alias contract preserves the dependency
record's vector-pointer store before the unaligned eight-byte transfers.
No packing, additional owner view, or compiler flag override is required.

The DDS1 twin `func_001150F0` uses the same
`EffectEventVectorParameters` shape and `D_0039F7D0` initializer. That
initial copy retains the last three parameters and color while the two
SDK vector copies replace only the first 32 bytes. The native mixer
dependency is the `SoundMixer *` returned by `effEventCloneSoundMixer`, whose
implementation calls `sndMixerClone`.

## Panel resources and typed pair updates

DDS2's `MenuPanelGroup.texture` is the `EffectSlotSet *` passed to the
footer draw and group constructor, not an integer texture identifier.
The existing complete `MenuPanelItem` owns the pair setter's +18/+1C
stores (`value18` and `option`); its definition must precede that setter.
`MenuStaffContext.spriteArg1` still belongs to the legacy window-resource
word API. Its one explicit word-to-pointer conversion at group creation
is the same boundary used by the neighbouring sprite constructor.
Retyping that transport field and its producer/consumer API is separate
end-to-end debt, not a reason to add pointer/integer round trips elsewhere.

## Serialized model commands and buffered reference lookup

`SdfCommandNode.payload` is packet storage for kind 1 and the serialized
command pointer for kinds 2/3. Its +C allocation descriptor is independent.
Reference commands carry a kind byte, flags at +16, a count at +18 and
halfword object indices at +1A. Their dispatcher receives the primary
`SdfModel *` and indexes `model->list->buffer` as `SdfDrawNode **`; the old
pointer-to-padded-context view incorrectly described those two loads.
Both existing dispatchers retain their native flag/count guards and real
bottom-tested loop. The model builder and reset provider consume a
`u32 *` serialized command-list stream, with the draw-node owner typed
end-to-end. These owner/API changes preserve every affected function.

## Hit and enemy-capacity override contracts

DDS2's battle root contains a signed hit-result override at +5D0 and a
floating hit-chance scale callback at +704. The calls at 001B1848 and
001B1B18 receive source actor, target actor and command; 001B1B20 consumes
the latter result directly from f0. The +70C single-target override receives
an `ActionStateLink *` at 0021552C and a nonzero result handles selection.
These are separate primary-owner members, not another padded battle view.

Both games' 0x28-byte scene records use +22 for the active-enemy limit
(zero means five, values six or greater disallow spawning) and +23 for the
cumulative spawn limit (zero means unlimited). DDS2 001B325C forwards the
actor, typed scene record and unsigned limit to the battle root's +6D4
override. The DDS1 twin 001A8CE0 confirms the same serialized scene bytes.

## Viewer motion-frame captures

DDS1 `0022EB10` shares its four-channel motion/key algorithm with matched
DDS2 `002496B0`, but lacks DDS2's duration and blend-lead adjustments.
The cache guard precedes reading the key's loop parameters. In DDS1's
nonlooping branch, `relativeFrame` captures the requested frame minus the
key frame before querying the model's frame count; the unclamped branch
still rereads the key frame after that query. Both reads follow retail,
without extra owner views or scheduling-only temporaries.

## Actor-formation high-water and completion callback

DDS1 `001F5028` and DDS2 `00205CC8` retain the largest arranged actor count
as a halfword at battle-root +246/+26A. Their stack scratch is sixteen
actor pointers; the count store is not a reason to invent a capacity check
or alter that footprint. Formation geometry reads the unit's mirrored
extent at +BC, separately from camera reach at +B4.

After both position and rotation passes, the arrangers call the battle
root's +5B8/+5EC completion hook without arguments and ignore any result.
Each game's scene initializer installs six providers; the twelve existing
provider contracts are `void (void)` (DDS2 `002218C8` remains ASM and reads
no input registers). This is `postPlacementCallback` on the primary owner.
The special DDS1 actor action clears that callback, rather than storing
an unrelated integer. Both arranger bodies remain ASM: the natural typed
twins have matching sizes/control flow but unresolved floating-register
homes, recorded in their disabled candidates.

## Field-reset call arity

DDS1 `func_00131580` is a zero-argument reset: its matched definition
clears `D_0032E538[0]` and does not consume incoming argument registers.
The area initializer `00120EC8` still has `$4 = 0x10`, `$5 = D_0032E570`
and `$6 = 1` before that call because it used them for earlier stores.
Those live register values do not establish a three-argument call.
The caller unit declares the actual `void (void)` contract; its remaining
field-store scheduling differences are independent of this repair.

## Event-viewer selected-label primary owners

Both 228-byte `evtDrawSelectedEntryLabel` providers receive an
`EvtRuntimeGroup *` and the owning `EvtRuntime *`. The signed word at
group +8 is `entryHeader.word`; the name address is the corresponding
`runtime->entryName[index]` row, not a second padded view or raw runtime
offset. These typed accesses preserve both providers and both complete
121-function units exactly. The initial list argument remains the
existing callback word API, with conversions only at the packet boundary.

## SDK interrupt-enable sequence

`EE_ENABLE_INTERRUPTS_SYNC()` in `ee_mmi.h` represents the single SDK
hardware operation `sync; ei`: complete prior stores before enabling
interrupt handlers. The sequence occurs independently in DDS2
`00345268`, `00345298`, and `00329F30`, and in DDS1 `002D2140`.
Keep the two hardware instructions in one intrinsic rather than modeling
them as independent compiler operations. Exact consumers are DDS1
`002D2140` (51-function unit), DDS2 `00329F30` (5-function unit), and
`00345268` (62-function unit), verified by whole-unit `check_unit.py`.
Their callback results and actual argument contracts remain ordinary C;
the only assembly is the evidenced SDK operation that C cannot express.

## Linked-defeat query payload

DDS2 `00229F14` requests four bytes for the mode that installs
`func_00221158`; `0022A41C` allocates and clears those four bytes.
Its payload has an enabled byte at +0 and a counter halfword at +2.
Use `BattleEffectPayload.query`, not a cast of the linked-actor member:
the separate Brahma mode allocates sixteen bytes and reads +0 as an actor
pointer. The mode-selected payload union owns both interpretations.


## Actor-extrema camera output and mode transport

DDS1 `001E0DA0` and DDS2 `001EE690` receive the owning
`BtlCamState *`: retail publishes position, direction, distance and FOV
at +0, +0x10, +0x20 and +0x24, then calls the camera finalizer.
Their four C callers pass the embedded front/back camera owners directly,
not a position-only view or a cast to `f32 *`.

Keep the mode argument's existing `s32` caller transport. The provider
decodes its low byte internally; its entry `andi 0xFF` does not by itself
prove a byte-width formal. The independently reviewed DDS2 continuation
found that a `u8` formal changes the already matched caller. The two
provider bodies remain ASM with their register/scheduling residuals.


## Actor-panel cursor and mirrored-sprite constructor

DDS2 `001C3EC0` and DDS1 `001B8CB8` use the actor panel's primary
presentation block: signed option selection at +0xDD, pending group index
at +0xDE, halfword option at +0xE0, XY offsets at +0xE4/+0xE8 and an
unsigned phase counter at +0xEC. The signed echo count at +0xF4 addresses
the four existing +0xF8 mirrored-sprite records with `count - 1`; it does
not establish a fifth record or a separate cursor view.

DDS2 `001B7208` and DDS1 `001AC5F8` initialize that same
`BattleMirroredSpriteRecord`: active byte, indexed alpha, slot halfword,
scale, XY words and frame word. Give the initializer its real record
pointer rather than an integer-address formal and raw-offset stores.

## Gradient rectangle color-array API

`uiDrawGradientColorRect` (DDS1 `002C0F88`, DDS2 `003089B8`) forwards
its sixth argument unchanged to the four-vertex strip provider. That
provider reads four packed color words at offsets 0, 4, 8 and 12; the
contract is `const u32 *`, not an encoded scalar color.
Own stack arrays are `u32[4]` and pass directly, including the paired
menu gradient-fade callers. The real leaf's finite declaration uses
the same pointer contract; its body remains ASM.

The generic `effSelectPresetAndDispatch` word API remains unchanged.
Each game's wrapper performs one explicit word-to-color-pointer
conversion at this boundary; no intermediate integer casts are
needed for callers that already own a color array.
Both providers and all nine affected source units gate with zero
differences; this contract closure credits no newly matched body bytes.

## Scene bounds flag signedness

DDS1 `00253018` reads `MenuSceneWork.boundsFlags` at +0x5AC with
`lb`, not `lbu`. Keep the primary byte field signed. The drawing
consumer `00251260` deliberately copies it into a `u8` local for
the 1/2/4 mask tests; that unsigned interpretation remains unchanged.
No second scene view or cast solely to select a load instruction is needed.
The selected-position routine still has an unresolved boolean-tail
lowering difference and remains ASM.


## Item-command scene pointer boundary

DDS2 `001C9BE8` reads the scene object's selection halfwords at
+0x0C/+0x0E, just as the landed DDS1 `btlDrawItemCommandRows`
(`001BE8A0`) does. Its declaration takes `BattleSceneObject *`.
The dispatcher retains its existing integer context transport and casts
once at that pointer boundary; the other option-count providers keep
their own existing word contracts.

## Target-side camera pose override

DDS1 `BtlState.cameraPoseBlendHook` at +0x628 and DDS2's corresponding
member at +0x660 receive a `BtlLinkedCommand *` and the two target-side
presence flags, not a `BtlUnit *`. DDS2 `001EAE88` loads +0x660 and passes
the command plus the 0x200/0x400 scan results. The existing
`btlStartLinkedActionPoseBlendIfEligible` dispatcher uses the same
interface; it passes its command directly without a second actor view.


## Uniform action-camera builder interface

DDS2 `001EAE88` calls `btlChooseActionPoseBlendFromActorCount`
(`001F34F8`) with the command and its front/back camera addresses
(+0x30/+0xC0), just as it calls the other three-input pose builders.
Keep that three-formal interface. This provider updates the embedded
cameras through the command and does not need its two explicit camera
parameters; the body remains unchanged. DDS1's corresponding
`001E5730` uses the same interface.


## Fixed-pass SDF item command dispatch

DDS1 `002D86E0` dispatches three inline command addresses or a
zero-terminated address list through two buffered passes. Fixed counters
terminate with `!= 3` and `!= 2`, matching the existing fixed-pass loop in
`sdfModelResetAndInitNodes`; range comparisons instead introduce SLTI
instructions and change the saved-register set. The list cursor reads
and advances before testing its sentinel: `while ((address = *cursor++) != 0)`.
Command addresses are serialized `u32` words converted to the existing
command-list pointer type at the SDK boundary. The canonical function is
`void`: IDA's apparent return values are incidental comparison constants.
The live DDS1 `sdfModel.c` gate reports 18 match, 0 differ.

## Packed two-vertex GS lines

DDS1 `002C10C0` uses a 32-byte packed vertex: two 64-bit color-channel
pairs, packed 32-bit X/Y coordinates, then a 64-bit zero-extended depth.
These widths come from the retail SD stores, not widened C temporaries.
The packet header's register list is `0x5151` (RGBAQ/XYZ2 twice).
Its local constructor prototype follows the C provider's `SdfDrawPacket *`
and `s64` register-list contract. Encoded allocator addresses convert to
packet/list pointers at SDK boundaries. The final pool-node-to-list-head
callback conversion reuses `sdfFlushPoolNodes`' shared first/last prefix;
embedding the list header in the pool node remains a dedicated SDK type debt.

## DDS2 progress-icon fields belong to the existing row

Retail `0029FA98` indexes the `0x68`-byte `levelAnimation` bank at
work offset `0xB060`. Its signed enable byte is row `+0x38`, followed by
32-bit opacity, X and Y at `+0x40`, `+0x44` and `+0x48`. The existing
icon state at `+0x4C` gates a separate packed-color word at `+0x54`.
Those fields complete `BrsProgressAnimation` without changing its size or
either game's work layout. A purported second bank at `0xB080` would
instead begin at this row's existing `applied` member (`+0x20`).

## Camera instruction streams are pointer banks

DDS1 `001E9DE0` and DDS2 `001FA480` receive a command, a camera pose,
and a read-only `BtlCameraTimedInstruction *`. The third input is not
an integer mode: retail reads kind at +0, a signed parameter at +4,
start/duration at +8/+0xC, and advances the stream by 0x10.
The corresponding eight bank arrays in each game's camera unit hold
instruction pointers. Their existing owners are declared before first use,
so all nine DDS1 and fifteen DDS2 C calls pass pointers directly.
No serialized data, instruction body, compiler flags, or variable declaration
order changes are required for this contract. Both interpreter bodies remain ASM.

## DDS1 motion-SE loader state has a canonical owner

The singleton `BtlState` owns the motion-SE `SoundSlotOwner *` list at
`+0x23C` and the two frame words at `+0x264` and `+0x268`. These replace
padding, preserving the complete `0xE10` layout. DDS2 already owns the
corresponding list at `+0x260` and frame words at `+0x288` and `+0x28C`.
The constructor-backed `SoundSlotOwner` in `snd_slot.h` owns the separate
29-entry request and resource-handle arrays; a new updater must not borrow
the competing local `BtlActorWork` view.

Keep the frame storage unsigned, as existing polling comparisons do.
The native updater explicitly classifies each word as signed before
incrementing it, with `-1` disabling that clock. Read track-loading flags
again after the status provider: it can clear the loading bit before the
caller decides whether a bank is still pending.

The field-only completion preserves every one of the 31 `btl_state.h`
source includers in both games. It does not claim a matching updater body.


## Defeat-camera forwarding interfaces

DDS2 `001F3228` and `001F34C8` take the command and a camera slot,
as shown by the native `001EAE88` calls and the already matched DDS1
`001E5460`/`001E5700` counterparts. The second wrapper forwards both
inputs; the first publishes defeat state through the command and does
not use its camera slot. Preserve that uniform two-input interface
instead of integer-address formals and one-input forwarding.

## Stored file-request words cross real pointer interfaces

`fileManager.c` supplies three distinct, finite interfaces:
`fileIsRequestReadyInCurrentMode(FileRequest *)`,
`fileGetResourceHandle(FileWork *)` returning `u32`, and
`filePollEntryCleanup(FileCleanup *)` returning `s32`.
The battle units `001C8890`/`001DD390` now declare these opaque provider
tags before first use, instead of relying on implicit declarations or
`s32 (s32)` prototypes.

Existing task and motion-SE records still store request addresses as words.
Convert once at each provider boundary to the corresponding opaque pointer;
do not introduce another structural view or change the stored-word layout.
The resource-handle result remains its genuine `u32` API representation.
Both complete consumer units remain byte exact, with providers and `file.h`
unchanged. This contract closure does not enable either motion-SE updater.


## Remote sound status uses a signed word and a wide cache

DDS2 `func_002A1790` is the native-identical twin of DDS1 `func_00269B80`.
The remote SDK entry returns a signed word (`0034EA00` loads with `lw`,
then `0034EA04` moves that word to `v0`), despite the stream-status cache
being read and written with `ld`/`sd` at `002A17B8`/`002A17F8`.
Declare the variadic SDK interface as `s32 (s32, s32, ...)`, promoting
its result into the real `u64` status/cache, as the matched DDS1 twin does.
Do not widen the SDK return or narrow the cache to force scheduling.

## Native SCE filesystem records share one SDK owner

`sce_io.h` owns the 0x40-byte `SceIoStat` and 0x144-byte `SceDirent`.
These are project spellings for the SCE iox field layout: the directory
record embeds its stat, followed by the 256-byte name at +0x40 and the
private pointer at +0x140. Retail completion copies 0x140 bytes and one
trailing word. Keep ordinary word alignment; do not import the modern
[PS2SDK iox alignment attributes](https://github.com/ps2dev/ps2sdk/blob/master/common/include/iox_stat.h).

The paired battle directory readers now use that complete record, rather
than a short 64-character name view. Forward both directory and record to
the SDK reader, and forward the directory to its closer. Each native
scanner deliberately passes the saved directory in `$a0` when closing;
the apparent no-argument SDK close reaches a descriptor lookup that
consumes that input. Preserve built-in iteration and partial-list failures.

The four stat consumers, two movie units, and two battle-reader units
remain exact. PiM's claimed effect-directory record copies are intentionally
unchanged until that owner scope releases; this note does not claim that
the movie scanners themselves have matching C.

## Bound-effect pose synchronization shares the native SDK vector shape

DDS2 `func_00226AB0` is native-identical to matched DDS1 `func_00205730`.
Reuse its actor reloads, actual translated X/Z/height work, and 16-byte
aligned quaternion output. The target performs native COP2 vector loads
and stores around the quaternion helper; use the existing `pcp_vu0.h`
macros, including the donor's `VU0_STORE_VF_UNCLOBBERED`, rather than
adding raw asm or a second actor view. The rotation setter's existing
quadword-pointer boundary still requires its ordinary vector-storage cast.

## Movie suffix lookup returns a character pointer

The SDK last-character search at DDS1 `00302240` / DDS2 `0035D5B0`
has the standard `char *(const char *, int)` contract: it tracks the last
matching address and also permits the terminator itself to match.
Its movie-stream constructor consumers now receive that pointer directly,
instead of declaring an integer result and casting it. Keep the current
linker symbols until the normal names pass; no alias or adapter is needed.

## A reused byte extent can preserve a constant-size `memcpy` call

ee-gcc 2.96 expands memory builtins before propagating ordinary local
constants. A literal `memcpy(dst, src, 48)` inlines at normal `-O2`, but
a named byte extent reused for clearing and copying retains `jal memcpy`,
even though the final call loads the constant 48 into `$6`.

The poly arc constructors at DDS1 `0015E760` / DDS2 `00166350` copy a
class-specific tail after a variable-sized template header. Computing
`tailBytes = sizeof(PolyArc) - sizeof(PolyRingHead)` and using it for both
the allocation extent and tail copy restores the retail 48-byte call
without changing compiler flags or library prototypes. A diagnostic
normal-flags probe produces the same 292-byte body and remaining ten-word
diff as `-fno-builtin`; the call alone does not establish a TU flag.

The already-C resource header constructor at DDS1 `00234C18` similarly
reuses `size = 0x20` for clearing, allocation, and copying: its four-byte
literal copy inlines while its final 32-byte copy calls `memcpy`.
Use a size local only when it represents a genuine reused extent, not a
dummy temporary introduced solely to inhibit builtin expansion.

## Result background and portrait fields share a primary row

DDS2 `0029DF18` forms `AF10 + index * 0x28` and accesses background
state/opacity at `+0x10/+0x18`. The portrait writer `0029E478` forms
`AF20 + index * 0x28`: its ready byte, opacity and integer coordinates
are at `+0x14/+0x1C/+0x20/+0x24`. All these real fields fit one canonical
`0x28`-byte `BrsFadeAnimation` beginning at `AF20`, with background fields
at `+0/+8`. No shifted second view, overlapping union, sixth entry or
next-row indexing is needed. Its five entries end at `AFE8`; the following
`0x78` bytes preserve the `B060` level bank and complete `B704` owner.

The native background writer's earlier biased address remains a body
matching residue, not grounds to move the primary origin or cast around
the fields. Both native fade writers stay ASM. DDS1's separate `DA0`/`DB0`
encoded origins and existing layout are unchanged by this DDS2 closure.


## DDS2 named state records own the copied initial tag

`mnuCreateNamedRecord` allocates and clears 0x22 bytes, then copies one
eight-byte initial-parameter group into `MenuStateRecord.tag` at +0x0A.
The mode byte and signed duration are members of that group, rather than
independent scalar aliases. The +0x18 halfword and direction nibble at
flags bits 5..8 are also native state-record fields. Whole-record assignment
reproduces the four unaligned eight-byte transfers and final halfword in
`003242D0`; do not replace them with invented 64-bit owner views.
Retail initializes only bytes 1..7 of the temporary initial tag in these
effect builders; its copied first byte is unused by those consumers.

## Linked-number tasks use the primary battle-unit counters and health record

DDS2 `0020F5E0` selects a twelve-entry display offset from the byte at
`BtlUnit +338` and increments it; the linked-number destruction callback
decrements that same byte. The counter-display callback owns the byte at
`+339`. These are the existing DDS1 `firstCountdown`/`secondCountdown`
fields at `+318`/`+319`, not a second unit view. The number payload's
`elapsedTicks` uses unsigned threshold tests at 5, 24 and 36 frames.

The separate 0x2C counter payload also owns an unsigned `elapsedTicks`
at +0x24 in both games. DDS2 `0020FA98` tests it with `sltiu` at
`0020FBC8`/`0020FBD0` and uses the unsigned high-bit conversion path
at `0020FBF0..0020FC00`; its constructor clears that same word.
Correcting the private owner preserves the matched constructors, but
does not close either counter callback's broad dispatch/allocator residual.

The color-threshold getter takes `BtlUnit *` and reads `partyRecord.hp`
and `partyRecord.maxHp` at `+126`/`+128`, as the already-C DDS1 getter
does. Its remaining legacy integer-address callers convert only at that
API boundary; no `UiObject` view is needed by the getter.

## DDS2 page bars retain real sprite resource owners

`MenuPageBar.textures[7]` at `+1C..+34` contains the `EffectSlotSet *`
results of `effCreateResourceSlotSet`, not numeric texture IDs. Drawing,
grid updates and release consume those same owners directly. The template
argument is also an `EffectSlotSet *`; the factory copies its resource metadata.
`002C1FF0` takes the page owner, variant, settings pointer, Y position,
quantized span, template, index array and count. Its `002C2128` wrapper
forwards the first six inputs and supplies one of two genuine seven-index
tables. The camp caller converts only its stored allocation/resource words
at these pointer boundaries. The existing default-setting wrapper still
publishes a word-address API, so its conversion remains at that call.

## The quaternion getter takes the primary actor owner

`btlCopyUnitRotationQuaternion` at DDS1 `001D66D0` / DDS2 `001E34D8`
copies the actor's `BtlUnit.orientation` at +0x70, not its world rotation
at +0x40. Its destination is a generic 16-byte SDK vector buffer.
Use the primary actor directly at every caller; the former byte-owner
and destination-quad casts do not represent separate objects.


## DDS2 overlay particles complete the existing runtime owner

DDS2 `0022D040` allocates 0x20-byte particles: a three-float direction,
rotation angle/speed, displacement/speed, and alpha/age/delay bytes.
The renderer at `0022D8A8` reads the same floating motion fields.
`BattleRuntimeState +18..34` therefore owns the row dimensions,
integer parameters and floating animation bounds; its `ownedData`
points to these particles, not a second runtime layout.

The constructor's unexplained third allocation-call register is not an
allocator argument: the existing one-input provider does not consume it.
Leave the constructor parked rather than inventing a prototype or dummy.

## Viewer range setters operate on the primary timeline runtime

`evtViewerSetMinimumFromCurrent` / `evtViewerSetMaximumFromCurrent`
(DDS1 `0022C3C8` / `0022C3E8`, DDS2 `00246D40` / `00246D60`)
update `EvtRuntime.headerFirst`, `frameRange.word` and `curFrame`.
The retired `EvtRange` was a padded projection of that same allocation.
The whole range word, rather than only its serialized low halfword, is used.

Option confirmation (`00230660` / `0024B268`) clears the real word at
`+2288` for type-12 keys and owns a frame-count dialog base at
DDS1 `+248C` / DDS2 `+24B4`. These complete the existing padding
without changing either runtime's size or inventing a second layout.

## DDS1 profile progress finishes five distinct animation fields

The profile callback at `00268AB8` indexes the existing 0x68-byte
`BrsProgressAnimation` bank at work `+1220`. It reads the signed progress
at row `+30` and publishes opacity at `+1C`; completion writes frame `+44 = 0`,
icon opacity `+48 = 0x80`, icon state `+4C = 1`, and flags `+60/+61 = 1`.
These fields complete the primary DDS1 owner without moving either bank,
changing its extent, or applying the different DDS2 counter origin.
The unexplained `+61` flag remains neutrally named.

This is an owner completion, not a callback match: the released correct
callback draft still differs in frame/register/control-flow lifetime and
remains assembly. All 17 actual `mnu_result.h` clients gate 596 match,
0 differ with no context, rodata or undefined-symbol rows after resplit.


## EffectObj and EffWorldNode merger is parked on the kind alias contract

The constructor allocates the same 0x44-byte object described by both
owners: identity at +04, kind-7 payload at +18 and `ObjectTransform` at +1C.
Retail writes the kind word at +0C (DDS1 0010F418 / DDS2 0010F640), but
effect providers read its high byte at +0F (001158B8 / 00115B20).

A real word/byte union makes both effect units exact, but regresses both
constructors by 12/54 words. Retail reads the operations-table slot before
initialization (DDS1 0010F45C..0010F464, before SW at 0010F468); the union
prevents gcc from hoisting that read. Explicitly reading it first leaves
four instruction differences; an unsigned 24/8-bit field view still leaves
the original twelve. The operations tables live in `.data`, so qualifying
their externs `const` is not an acceptable fix.

Keeping the original word field and using `kindTag >> 24` emits LBU but
reorders four stores in `effObjBindValidatedOwner` (+3C/+44/+4C/+54).
The merger and union therefore remain private, and existing matching C
is retained. No alias cast, dummy qualifier, or store-order search is used.

## DDS2 stat gauge span is signed; pulse division is still unresolved

The unit-private `MenuPanelItem.value10` at +10 is a signed pixel span.
`func_002C2AE8` combines it with signed stat differences and uses retail
`mult` followed by signed `div` at 2C2B3C/2C2B50, 2C2CEC/2C2D0C and
2C2DB0/2C2DD0. Keep that primary field `s32`, not an unsigned owner plus
a renderer-only signed view. Its existing word store and all 175 matched
functions in `code_002BE628` are unchanged.

The renderer is not thereby matched. Its pulse still has real hardware
division by constant 64, while ordinary C cancels `phase * 256 / 64`
during the first compiler pass. Division by 170 remains hardware division.
The released complete owner-based attempt already checked signed math;
there is no evidence for a dynamic divisor, fake qualifier or flag change.
A real original inline may explain the late constant, but no such helper
has been identified, so the body stays assembly.

## DDS2 local-map loader retains a real subrecord and forwards task work

`func_0030AC10` retains `LmapTaskState + 0x78` throughout the loader switch:
file request at +00, PAC request at +04, signed phase/index at +08/+0A,
and an unknown word at +0C. Keep this as the primary `LmapLoadState`, with
the shared `FilePacRequest` owning the PAC queue rather than another view.

`func_0030A8A8` forwards `kwlnTaskGetUserValue` in `$4` through the matched
`func_0030ABF0` wrapper to the loader. Both receive `LmapTaskState *`;
the wrapper compares the loader's actual `s32` result directly. The former
void/no-argument declarations and `s64` comparison local were unnecessary.
This contract correction does not match the loader body: its honest park
still differs in phase-request flow and register/scheduling details.

## Camp stock-window setup receives its actual menu owner

DDS2 `00261B98` operates on `MenuTerminalContext`; its caller at
`00262E98` transports the task's word-valued user data into that
pointer interface. Keep the conversion at this real task boundary,
not a wrong integer prototype on the setup method.

The shared eight-byte `DatItemSkillRecord` in `dat_command.h` owns flags
at zero, the quantity-consumption flag byte at one, a command index at
two, and the base price at four. Event, battle, roster and shop readers
in both games use this owner directly rather than separate index,
availability and price views. Byte one remains `unk01`: bit 2 causes
the battle consumers to decrement item quantity, but the other bits
are not yet identified.
The body remains parked: `+154..15C` still has a three-store schedule
rotation, which does not justify store-order search.

## Object-base render setup uses the complete model owner

DDS1 `func_00112100` follows `ObjBase.resourceHandle` to `MdlCtx.inner`
and temporarily changes `SdfModel.flags` at +19. The existing complete
owners replace the two render-only prefix views without changing code.
`mdlProcessContextNodesAndTransforms` receives the actual `s32` SDK
update argument; the surface-table pointer crosses that address-word
boundary explicitly, rather than through a false pointer prototype.

## Kind-5 room-mode selector is signed

The object-base dispatcher reads `EffectObjectData + 04` with `lw` and
tests it with `blez` before passing the positive selector to
`fldTestRoomObjectModeFlag(area, floor + 1, selector)`. Keep the primary
field `s32`; an unsigned owner plus a renderer-only signed view or cast
would hide the real contract. Its existing zero initialization is unchanged.

## Message-option setters return status without narrowing their arguments

DDS1 `0019C9F0` / DDS2 `001A4A10` return zero on rejected keys/capacity
and one after storing the option pair. Their unit-local forwards therefore
return `s32`, with the existing three `s32` arguments; the script wrappers
ignore the result. A narrow key formal would incorrectly admit truncated
out-of-range input. The setter bodies remain assembly pending their store
scheduling residual; this prototype closure does not claim a body landing.


## Linked defeat-candidate cameras use their primary action and pose owners

DDS1 `001E5198` takes a `BtlLinkedCommand *` and two `BtlCamState *`
outputs. Its sole C caller, `btlStartLinkedDefeatCandidateAction`, uses the
existing action task, target list, embedded front/back camera states,
motion parameter and flags directly. The task pointer is reloaded after
the candidate-clear call, preserving the native lifetime boundary.
The already-C DDS2 counterpart `001F35C8` has the same three-pointer API
but a different camera algorithm and preset data.

The callback/formal closure gates DDS1 `code_001C8890` 585 match, 0 differ;
the unchanged DDS2 twin unit gates 537 match, 0 differ. The DDS1 camera
body remains assembly: a donor-backed private bank-copy draft has 28/178
word differences, including scalar operand roles and the second muzzle
buffer-address materialization. No extra asm/barrier is added to force it.

## Camp camera-color keys use the canonical interpolation record

Track type 25 (`0x19`) captures a 0x40-byte `EvtCameraColorPayload`;
its first 0x38 bytes are the shared `EvtBlendKey` passed to
`evtBlendParamsH`. The four former `CampListLayout` definitions describe
that same record, not a separate menu owner. Both default constructors
and both key-capture callers now use the canonical type and its w/x,
flagWord and y/z members, preserving every store and opaque payload tail.
The interpolation bodies at DDS1 `00242F78` / DDS2 `0025E390` remain
assembly; retiring the duplicate views does not claim a body landing.


## DDS2 profile panels own two resource sets, not an integer-word view

`MenuProfilePanel` remains 0x48 bytes in DDS2: `capValue` and `option`
are signed words at +10/+14; `sets[2]` holds the `EffectSlotSet *` values
at +18/+1C. The five-argument `mnuSetGroupProperties` writes only those
two pointers and the +24/+28 trail/particle indices; +20 is preserved.
The five random opacities start at +2C, followed by signed phase/opacity
at +40/+44. The renderer uses signed division for the percentage and
signed comparisons/arithmetic shifts for opacity.

Factories and forwarders use this one primary owner and real resource
formals. Stored integer resource-address boundaries are converted only
at their existing calls. DDS1 keeps its different 0x3C profile layout.
Its already-matched `00285208` shows the authentic direct-indexed
`BdWork.sourceWidth` crop and `geometry.bounds[2]` update idiom, but is
not a byte-identical donor for DDS2's longer `002C33C0` renderer.

## Resource effect wrappers forward their pointer arguments and result

DDS1 `00115298` and DDS2 `00115500` are the same constructor forwarder:
the resource and two vector pointers reach the inner constructor unchanged,
and callers consume its returned object pointer. A `void(void)` declaration
only happened to preserve those registers in the old wrapper's machine code;
the real three-pointer, pointer-returning contract is now explicit in both.
This closure does not change the separately parked script-setter return ABI.

## Viewer window shading uses signed byte state

DDS1 `00230140` and DDS2 `0024AD48` load both `EvtRuntime` bytes
at `+0x23C4/+0x23C5` with `lb`. The first is `windowShadeFade`:
inactive windows subtract nine down to zero, active windows add three
up to 94, and the overlay alpha is `128 - windowShadeFade`.
The arithmetic assignments also reload the stored byte with `lbu`;
that does not make the primary field unsigned. The cleanup functions
reset the same signed owner field, and the active/inactive setters retain
their zero/one stores. The packet-building bodies remain ASM: completing
these primary fields does not resolve their packet-address scheduling.

## Adjustment script wrappers return the script result

DDS1 `00118558/00118620/00118648/00118670` and DDS2
`00118B90/00118C58/00118C80/00118CA8` are true tail forwards
to the selected unit script, `evtRunContext`, or another such forwarder.
Their final instructions jump directly to the callee after restoring the
frame; the surviving `v0` is the callee's result, not fall-through garbage.
The adjustment evaluators consume that result as a signed word.
These wrappers therefore return `s32` with an explicit `return` expression;
the existing byte-mode conversion is unchanged. Both whole units still
match, while the separately parked evaluators remain ASM.

## Field controller, camera state and resource banks share one owner

`fld_area_work.h` owns the complete `FldAreaWork`; the field, camera,
script and billboard consumers no longer keep prefix views or word-array
aliases. `area/floor` are at +10/+14, with `floor` zero-based. The separate
resource-load state at +78/+7C/+80 is `resourceFlag/resourceArea/resourceFloor`,
not another controller's area/floor.

DDS2 inserts twelve bytes before the player-model/XYZ history tail.
The retained map bank starts at +19C with four records in DDS1, and at
+1A8 with eight records in DDS2; DDS2's four texture records follow at +1E8.
The five records once named `fldmix` by DDS2's script unit are the same
map-bank entries 3 through 7, not a second layout. XYZ remains three
coordinates followed immediately by saved XYZ history. Compile-time size
and offset checks preserve both games' actual complete owner extents.

## The kind-5 entry-blend yaw is a float payload field

DDS1 `001122F0` loads `EffectObjectData +34` with `lwc1` directly
into the yaw argument of `mdlBlendEntryPitchYawAndUpdate`.
The primary `word34` field is therefore `f32`, not an integer word
requiring a reinterpretation view. Its six existing initialization stores
remain `= 0`; both complete initialization units retain their native bytes.

## Actor-camera dispatch receives the actual camera payload

DDS1 `001E3E58` and its matching DDS2 counterpart `001F17C8`
receive a `BtlCamState *` as their second argument. In the canonical
`BtlLinkedCommand`, the primary `camera` starts at +0, `frontCamera`
at +30, and `backCamera` at +C0. Passing the command base therefore
means `&action->camera`, not `&action->frontCamera`; the alternate
caller passes `&action->backCamera`. Neither path needs a byte-pointer
cast or an address-shaped second view.

## DDS2 progress rows retain a signed ramp counter

`002A0278` accesses the canonical 0x68-byte `BrsProgressAnimation`
with signed `lw`/`sw` at +28 and `lb` at +2C. The `frames` counter
increments and clamps to 0..120 before forming `150 - frames`.
A nonzero `skipRamp` bypasses that ramp and uses the fixed fast
step of 10000. These fields occupy the original padding; the
level/profile bank origins, other fields, and DDS1 layout are unchanged.

## DDS2 mantra navigation consumes the whole menu owner

`0028D7C8` loads the flag resource from owner+7AC and the animation pool
from owner+BEC. These are `MnuStatusResource.menu.slots` and
`MnuStatusResource.menu.resource`, not the selection controller at C00.
The reveal and neighbour-selection helpers take the whole typed owner.
The unchanged rank-pass address-word interface decodes its argument at
the three calls into these helpers; no alternate workspace layout is needed.

## Model update arguments are draw-surface tables, not frame counters

DDS2 `0033251C..00332544` reloads the original renderer argument, indexes
four `SdfPoolNode *` entries, and calls each selected pool's `append` at +10.
`mdlProcessContextNodesAndTransforms`, `mdlBlendEntryPitchYawAndUpdate`, and
`evtStageTestUpdate` therefore forward `SdfPoolNode **surfaces` unchanged.
The stage viewer's +08 model is `MdlCtx *`; its motion-state check is
`model->first->state`, not an address-word view. Existing encoded-word
transport and the SDK pool-to-list-head callback boundary are unchanged.

## Basic object drawing uses the complete model owners

DDS1 `00112100` and DDS2 `00112328` obtain `MdlCtx *` from the basic
object's resource handle and operate on `context->inner->flags` in the
complete `SdfModel`. Their old context/inner prefix typedefs are unnecessary.
The draw-surface pointer table is forwarded unchanged through the model
update API; no auxiliary flags view or SDK address-word conversion is needed.


## Marked-actor camera dispatch forwards two owned camera records

DDS1 `001DEE18` forwards its linked-command pointer unchanged, then forms
the front/back camera arguments at +30/+C0 for `001E4AC0`. DDS2's
`001F2758` has the same three-pointer contract. These are `BtlCamState *`
members of `BtlLinkedCommand`, not integer-address parameters; the dispatch
callback therefore accepts the command pointer directly.


## Effect-preview selection has an unsigned count and status result

DDS2 `002FE5B8` iterates its source records with `sltu` and transports an
unsigned source index through the menu's floating-point value. Its shared
exit explicitly copies the status word to `$v0`; the result is `u32`, not
`void`. Creation-menu wrappers intentionally discard that result.


## Actor-specific camera timing uses an owned signed frame counter

DDS2 `001F3C30` initializes linked-command +140 to zero; `001F3E48`
compares that signed word with the actor's motion frame and increments it.
It is `BtlLinkedCommand.cameraFrame`, not unused byte padding. The update
routine receives the same command pointer that the cursor dispatcher owns,
so its formal and callers do not need integer-address conversions.

## Removed debug text retains its fixed four-argument interface

DDS1 `001FC924` supplies x, y, style and a text pointer to `001FB130`,
even though retail's provider discards the output. Its unused formals
match the recovered DDS2 `0020D1B0` interface; the empty body does not
make it a zero-argument function or a variadic formatter.


## Camera endpoint setup follows the linked-command index append

DDS2 `001EC688` appends `action->link->unit` to the command's target
index list, then forwards the unchanged command and its front/back camera
members to `001F2E30`. The wrapper and constructor use a command pointer
and two camera pointers; a second integer-address alias has no ownership
or transport role.


## Defeat queries borrow the complete battle unit

DDS2 `001B2430` reads the unit's party-record status and HP with LHU at
`+0x12E` and `+0x126`, and the battle work's flags at `+0x218`.
Its exemption query `001B47E0` uses the same unit and its party-record
unit ID at `+0x124`. These are `BtlUnit *` interfaces, not byte-buffer
interfaces: their callers need neither byte-pointer casts nor a second
UI-record projection of the party data.


## Ring pulse rendering narrows the timer, not the unsigned mode

DDS1 `00258FD0` guards its `u32` pulse mode with `sltiu ...,6`; no second
unsigned cast is needed. The same renderer reads the stored `u16` timer
with `lh` before progress conversion and the mode-five threshold test.
Its explicit `s16` narrowing therefore preserves real signed consumption,
while duration is already an `s16` field.

## Solar indexed packets borrow point and packed-color buffers

DDS2 `00311F20` reads `s32` XY pairs and `u32` RGBA words; its second
argument is an unsigned XYZ2 tail word, followed by count, first-color
selection and surface index. The four C callers in `002437F0` now use
that real pointer/color contract rather than an integer point prototype.
The `003C90DC` interior table origin remains unchanged: its preceding
word holds the point-buffer address, decoded at the provider boundary.
Changing that iterator to the primary table origin is separately parked
because the equivalent DDS1 change regresses two existing exact bodies.


## Room resource registration returns its slot index

DDS1 `00138ED0` and DDS2 `0013BAB8` return the old `fldTaskSlotCount`
before incrementing it (`0013A6A8` / `0013D290` move that value to `v0`).
The paired action constructors immediately multiply the result by `0x140`
to update the corresponding room record. Their `FldFileResource *`,
`EffWorldNode *` interface therefore returns `s32`, even though the general
file-resource dispatchers ignore it. Correcting the caller declarations
does not change either still-ASM provider's body.


## Linked-defeat camera dispatch has its own override

DDS1 `001E4180` and DDS2 `001F1B00` read runtime callback slots at
`+60C` and `+644`, respectively. Both deliberately pass the linked command,
the destination camera pose, and integer `1`, then skip their default
framing when the callback returns nonzero. `BtlState.defeatCameraHook`
therefore has the shared `s32 (BtlLinkedCommand *, BtlCamState *, s32)`
contract; it is distinct from the adjacent ordinary arrangement hook.
The two dispatch bodies remain ASM.

## Terminal glyph constructors preserve chain pointers

DDS2 `0019F5E8` and `0019F6C8` return `FrFontGlyph *` and take a previous
`FrFontGlyph *` as their sixth argument. Both native-matching definitions
append text, update the resulting chain and return it. The terminal
numeric/currency callers in `002665B0` now use that actual contract,
with null previous chains and no integer-return reconstruction casts.
The existing glyph typedef is declared before the first pointer import.


## Battle effect payloads follow the mode's allocation

DDS2 constructor `00229728` allocates and clears twelve bytes for mode
779's guard state and eight bytes for mode 782's marked-scene/summon
state. The former owns the temporary unit at `+0`, selected guard at
`+4` and result bytes at `+8/+9/+A`; the latter owns the active byte and
the full-word selected party ID at `+4`. They are distinct members of
`BattleEffectPayload`, not prefix views of mode 786's larger linked
effect record. Unit creation, cancellation and the marked-state getter
consume their actual variant; destruction borrows `BtlUnit *`, not a
scalar address word.

Mode 789 instead allocates sixteen bytes: its actor is at `+0`, Brahma
ratio at `+4`, and pending replacement byte at `+8`. Its registered actor
creation/destruction, ratio methods and `00222100` use that variant rather
than mode 786's linked-effect record. The replacement routine retains the
script task while reusing one scheduler-task local for its dependent effect
and sound, whose conditions freshly read the script handle before starting.

## Template clones retain the common particle-kind owner

The radius updater and template-clone helpers in DDS1 `00151F58` and
DDS2 `00159B48` address the same 0x18-byte `ParKindState` at template
offset `+0x30`. Its `+0x04` halfword is the template entry count:
the clone reads it for each supported resource kind when sizing the
per-particle subrecords. Embed this owner rather than casting the
address of a standalone kind halfword to `ParKindState *`; the count
remains at template offset `+0x34` and the template stays 0x180 bytes.


## Quantized strip preparation retains its configuration pointer

DDS1 `002AFE68` and DDS2 `002F3258` consume the existing
`EffBillQuantizedConfig` as their second argument. Both the shared-texture
and retained-frame constructors pass that owner directly; an owned class's
payload is the same configuration, not an integer argument or a second
record view. The first argument remains the existing three-word buffer
API. The two large preparation bodies remain assembly while their
placement and scheduling differences are unresolved.

## Kind-3 slot handlers own the complete mover payload

DDS1's operation table `003299C0[3]` selects `00329A50`, whose constructor
`00111400` allocates and clears sixteen bytes. `Dds3SlotResource` is that
complete owner: transform target at `+0`, retained path work at `+4`,
relative-transform callback at `+8`, and kind-16 source node at `+C`.
The slot accessors in `00111610`/`00111838`, mover update, and destruction
use the same payload, not separate resource/mover prefixes. The getter
returns the retained path; the conditional attachment tests the source node.

`dds3InvokeSlot1Handler` installs rather than invokes the callback. Mover
update supplies `ObjectTransform *` and `EffWorldNode *`, and applies the
relative transform only for result 1. The constant-zero callbacks
`001243C0`/`001266D8` retain those unused family formals. Ring-entry addresses
and room-name addresses still use the world node's existing encoded `value`
word; generic slot APIs and the float-counter SDK word boundary are unchanged.

## Packed command queries borrow the complete party record

DDS1 `00117438` and DDS2 `001176A0` use `DatPartyRecord` for packed-status
query/apply/raise helpers and DDS2's HP-bracket helper. HP/max-HP at `+6/+8`, MP/max-MP at `+A/+C`
and status at `+E` belong to one record, not separate short party views.
The affinity and skill providers already accept that same owner.

The HP/MP mutators in `datCalc.c` really return `void`, but these caller TUs
keep them unprototyped rather than publishing false `s32` declarations.
Native DDS2 `sdfApplyCommandResults` reloads HP into `v1` and branches on it at
`+E0/+E4`; exposing the void prototype changes those two register operands.
The inferred original implicit-int call boundary preserves the observed
caller without inventing a provider result; neither call consumes a result.


## Camera preset outputs are embedded pose records

DDS2 `0021C5E0` and `00224598` initialize a `BtlCamState` and rotate its
direction vector at `+0x10`. The output is not a second command-shaped
`BtlEffect` view. Caller `00224DF0` deliberately supplies the command's
leading `camera` member at offset zero, while its action/state/link fields
belong to the complete `BtlLinkedCommand`. Boss preset selection at
`0021C428` likewise borrows that command's link to its unit.


## Replacement-text sources are pointers, not slot identifiers

`evtCopyEntryStringToActiveWindow` (`0024DD90` / `0026C918`) and
`itfMesCopyStringToWindowTableSlot` (`0019C838` / `001A4858`) forward
encoded byte/halfword text buffers through a `const void *` source.
Their window and replacement-slot inputs remain numeric identifiers.
The existing K&R copiers `0019D460` / `001A5480` already take a
`const char *` source and use it in `strlen`/`memcpy`; keep their
unprototyped declarations because the legacy one-argument bridges remain.
Stored script/name-bank address words are decoded at their actual uses;
do not retype the dual-use DDS1 `D_0032ACA8` flag/text bank as pointers.

Pending Prism's profile-owner release, DDS1 `00250E88` and `00254B30`
retain their old TU-local imports. The former still has two pointer-to-word
text arguments. These two excluded consumers are explicit follow-up debt,
not a completed all-caller migration.

## Scene input feedback is a void dispatcher

DDS1 `00250B60` samples the low byte of the word-sized `cursorInputMask`
into a signed direction local and reads `cursorMoving` with `lb`.
The primary scene-work flag is therefore `s8`; its existing cursor-position
producer still stores the real 0/1 movement result.
The generic up/down navigation helpers return the selected `SdfGridCell *`
or NULL; the scene's custom left/right helpers instead normalize success to 0/1.

The only retail caller (`00251A38`) ignores this dispatcher's result, and
the sound provider `sndSetSequenceVolumePan` returns `void`. The four
feedback cases use ordinary terminal sound returns for cases 1–3 and
the final case's implicit function exit. This preserves the native three
tail jumps and final call/epilogue without inventing an unused integer
return value. DDS2 `0028B318` is a different navigation updater, not its twin.


## Local-map loader retains opaque file requests

`LmapLoadState.file` is the request passed to the loaded-address, size,
resource-handle, readiness and cleanup APIs. Its archive request comes
from `fileQueuePlainDispatchRequest`, whose matched provider also returns
`struct FileRequest *`. Both fields therefore retain that canonical
opaque owner, rather than `void *` or an invented `FilePacRequest` view.
This does not expose the provider's private PAC layout; the sixteen-byte
load record and its offset `+0x78` inside the `0x88`-byte task stay unchanged.

## Selected-item category queries borrow the actor

`btlTestSelectedItemCategoryMask` (`001A8448` / `001B2900`) reads the
actor's `selectedEntryIndex` and indexes the canonical command record.
Its first input is `BtlUnit *`, not an integer unit address. DDS1 now
uses the same provider contract as DDS2, including both split battle
caller units. The action/category index and returned predicate stay `s32`;
the caller's existing generic pointer forwarding needs no integer adapter.


## Quarter-threshold queries consume the copied party record

`btlIsCurrentValueBelowQuarterThreshold` takes `BtlUnit *`. Retail reads
the HP/max-HP halfwords at `+0x126/+0x128`, which are
`partyRecord.hp` and `partyRecord.maxHp`, not a separate UI-object owner.
The DDS2 provider (`001B24C0`) keeps its `< 26` percentage test; the
DDS1 counterpart (`001A8018`) keeps `< 25`. Their common owner does not
make those thresholds interchangeable. Local callers use the actor
pointer directly, without a byte-pointer adapter.

## Rotated rectangles borrow their UV and corner-color words

The sixth and seventh inputs to `itfDrawRotatedTexturedRect` and its
DDS2 primitive (`00306030`) are read-only four-word arrays. The matched
border renderer (`00306678`) temporarily edits its UV words and supplies
its local corner-color words directly, just as the DDS1 counterpart
(`002BEEA0`) does. Keep these inputs as `const u32 *` through the wrapper,
without pointer-to-word transport casts. DDS2's additional rotation-mode
input remains in its original forwarding position; this contract change
does not replace either primitive's assembly body.


## Packed triangle coordinate inputs are arrays, not coordinate scalars

DDS2 `00308650` reads three signed coordinate words from each of its first
two inputs and three packed color words from its fourth input. Keep the
primitive's coordinate parameters typed, with one decode of each existing
address word at `uiDrawUniformRgbRange`. Its caller still forwards eight
inputs; the final three are unused by the primitive. The line and triangle
packets share the same 0x20-byte channel/XY/depth record: the low X word is
zero-extended, Y occupies the high word, and the hardware depth slot is a
64-bit serialized field fed by the true 32-bit input. This contract closure
does not imply that the assembly triangle body has matched.

## Camera arrangement reads the mode/count both separately and together

The DDS1 camera arrangement path (`001E3E58`) reads `+0x244` as a word
equal to `0x10003`, while its loops read the mode halfword at `+0x244`
and update the actor high-water halfword at `+0x246`. The canonical
`BtlState` therefore exposes these genuine word/half uses in one union,
as DDS2 already does at `+0x268`. Its `+0x608` hook receives the linked
command, camera pose and mode and returns a handled predicate, matching
the DDS2 `+0x640` hook. These owner declarations do not claim that the
DDS1 arrangement body has matched.

## Signed command selectors borrow the actor

`btlGetActorIndexedSignedValue` (`001A2F00` / `001ABF00`) takes
`BtlUnit *` and returns `s8`. Only selector zero on an actor with flag
`0x400` reads the enemy record, using `partyRecord.unitId`; other inputs
read the canonical two-byte command-selector table. The linked special
actor callback (`0020D2E0`) passes `task->unit` without an address cast.
Its signed selector-to-`u32` mapper conversion is a value boundary, not
pointer transport. Both actor inputs of `btlSelectedEntryHitsElement` now
use `BtlUnit *`; its selector provider consumes the first actor and its
status test consumes the second. Generic effect-dispatch words are decoded
at their existing boundary rather than propagated through actor providers.

## Model afterimage slots own models and signed countdowns

The paired afterimage updaters (`002174C0` / `00231FD8`) allocate `0xC`
bytes per `MdlDevSlot`: next at `+0`, the actual `SdfModel *` at `+4`,
and a signed word countdown at `+8`. The destructor releases that model
before the slot allocation. Keep the complete owner in `mdl.h`, not an
eight-byte prefix or another private slot view.

`SdfModel.unk94` is one four-byte value with genuinely different uses.
DDS1 `0021755C` and DDS2 `00232074` store the float `1000.0f`;
DDS1 renderer `002D9530` loads its unchanged word and `002D953C`
forwards those bits into a VIF packet at `+0x2C`. The Nocturne debug
renderer independently reads the same member as a float at `002B07D4`
and as a packet word at `002B089C` / `002B08A8`. Use the primary
owner's documented `scalar` / `word` union, preserving its `0x9C` extent.
These type completions do not match the still-assembly afterimage body:
its unobserved local packed-color store needs genuine source/API evidence,
not a padded temporary or fabricated memory-output packing primitive.


## DDS2 scene-slot flags and the retained fade-count argument

`ActorSlotOrder` keeps the DDS2 slot-mask/mode flags at `+2`, separate
from the signed state bytes at `+4`. The controller `001CF800` uses
`LHU`/`SH` there, setting mode bit `0x1000` and the individual slot bits;
the DDS1 layout is unchanged. Its stale-slot call at `001CF840` deliberately
loads the signed last fade index from `sp+0` into `a0` at `001CF844`, after
the count provider wrote that output. `btlFadeStaleSceneSlots` retains this
unused formal even though its retail body does not read it. These contracts
do not claim that the controller body matches.


## PM paired-file exporters return a real status

The DDS1 `0023E7F8` and DDS2 `00259AE8` providers return an explicit
`s32`: zero when either file open fails, one after the completed PM2/PM3
write, close, and device-sync sequence. Their real inputs are a signed
mode and the canonical `EvtRuntime *`. The viewer's mode-selection
client deliberately discards the result, but its declaration must still
describe that status API rather than `void` or an unprototyped call.
The upstream exporter bodies are unchanged by this contract correction.


## Battle input snapshots retain the complete kernel extent

DDS1's local `SndPad` owner in `code_001A1960` covers all `0x40` bytes,
not just the known button fields through `+27`. The kernel initializer
`001039E0` clears two banks of two ports with sixteen input bytes each;
the cursor provider indexes that same 64-byte snapshot by 16-byte set.
An unknown tail completes the existing owner without moving or changing
any named field. This type completion does not match the still-assembly
selection controller `001C08B8`.


## Field scatter effects retain their actual work pointer

DDS2 `code_00124040` stores `effBlurCreateScatterWork`'s returned
`EffBlurScatterWork *` in `D_00435F74` and later passes it to
`effBlurReleaseFirstResource`. The retained global is a work pointer,
not an arithmetic handle; the two pointer-to-integer stores and the
integer-to-pointer destruction cast are unnecessary. The serialized
small-data slot remains four bytes. This owner closure does not claim
the still-assembly `fldProcDraw` callback as matching.


## Scene target cursors use a word and two coordinate halves

The DDS1 selector `001C0650` compares `SceneAiWork`'s cursor at `+18`
with a word load (`001C0724`, `001C0760`) and moves its two halves with
`SH` at `+18/+1A`. The primary owner embeds those genuine word/half uses
in one union. Existing consumers retain unsigned halfword storage;
the selector's signed logical coordinates account for its `LH` indexing
and signed 16-bit wrap checks. The constructor's row `2`, column `1`
pair produces the packed sentinel `0x00010002`. This owner completion
does not match the still-assembly selector body.


## Positioned field strings retain their format pointer

Both `fldDrawFloorQuad` providers (`0012BAD8` / `0012E008`) forward
their third argument directly as the `const char *` format of
`sdfFormatSifPacket`. It is not a numeric draw value; the pointer
formal removes the integer-to-pointer conversion without changing
the emitted provider instructions. There were no existing C clients
at this cutover. The diagnostic grid and field draw callback remain
assembly until their independent control/coordinate differences match.


## Result backgrounds and portraits share one complete row

DDS1 has five `0x14`-byte skill-icon rows at `+D50..+DB4`, not two:
`00264B08` handles the first row and then four following rows, including
the final state byte at `+DA0`. The following five `0x28`-byte fade rows
start at `+DB4`; backgrounds use row `+0/+8` and portraits use
`+14/+1C/+20/+24`. The portrait producer `00266B10` and renderer
`00266BC0` agree on absolute `DC8/DD0/DD4/DD8`. This is the same primary
row layout already proved at DDS2 `+AF20`, not a second overlapping view.
The DDS1 next section stays at `+EE0`, and the complete work stays `0x1590`.
Completing this owner does not match its remaining assembly renderers.

## Blur controllers keep the actual packet-list pointer

The matched `0019FF60` / `001A8BD0` controllers retain
`sdfCreateResetPacketList`'s `SdfListHead *` directly. No intermediate
signed address word or repeated conversion is needed for its typed list
consumers. Packet payloads still cross the real `u32 packetAddress` API
explicitly. Their unchanged surface callback's legacy first-formal
contract is separate SDF primary-owner debt; this local cutover does not
introduce or certify a new surface view or claim new matching bodies.


## Panel selection opacity has signed clamp arithmetic

DDS1 `00284C48` reads `MenuPanelItem.selectionRamp` at `+0x8C` after
its draw callback. The native `BLEZ` at `00284D80` and `BLTZL` at
`00284D90` implement positive decay followed by a negative clamp to zero;
the rolling branch also uses signed `SLTI` at `00284DC8`. The primary
unit-local field is therefore `s32`, not an unsigned color word. Its
constructor and setter still store the same nonnegative initial values.
This owner correction does not certify the still-nonmatching renderer.


## Camera support selection is a signed primary-owner field

`0012C880` / `0012EDB0` read the signed word at `FldAreaWork +0xA8`
first; only `-1` selects the default record at `+0xA4`, whose negative
values suppress the height probes. Both routines reload the override
after their two triangle-height calls. This completes the existing
primary owner without an address-named scalar alias or another view.
The support-height routines themselves remain assembly: selector
lifetimes, control scheduling and triangle-call setup are not matched.


## Scene count scratch is a complete shared pointer owner

`001B2AC8` / `001BD6E8` both allocate and clear `0x38` bytes for
`SceneKindTable`. Its unsigned pulse phase, signed halfword alpha,
cached entry/cursor/relative row and eight count words occupy `+0`,
`+4`, `+8/+C/+10` and `+14..+30`; `+34..+37` remain opaque.
The canonical owner replaces both partial `0x34`-byte DDS2 copies.
The retained globals are pointers in both games, and the DDS1 count
provider indexes the same typed `value[]` rather than a parallel raw
offset view. The shared-header cutover does not match the still-assembly
selection controller or close the separate legacy scene-context API.


## Field vector fetches retain their real node contract

`effObjFetchInnerFirstVec` takes an `EffWorldNode *` and leaves the
position in `vf10`; it has no C return value. The field controller
converts its retained address word once at that provider boundary.
The vertical-step callback at `001321F8` returns zero on both exits,
so its local forward declaration is `s32(void)`, not `void(void)`.
It remains assembly: no redundant Z self-copy or one-off coordinate
inline is introduced to imitate its still-unmatched join stores.


## Hit-result overrides retain both real actor arguments

DDS1 `0020ED90` installs `btlClassifyLinkedSkillRequest` at
`BtlState +0x59C`. The hit classifier at `001A7548` explicitly supplies
source, target and command index and returns a nonzero override result;
the provider legitimately ignores the source but retains its real
`BtlUnit *` formal. Its target is a pointer, not an address-word API.
The shared primary member uses the already established DDS2
`hitResultOverride` name (DDS2 `+0x5D0`); neighboring offsets and both
extents are unchanged. Restoring this hook and the existing provider
types does not certify either still-assembly hit classifier.


## Viewer mode providers retain the real runtime pointer

`0022FB30` / `0024A738` consume `EvtRuntime *` directly; their mode
dispatchers already have that same allocation. Pointer formals replace
the address-word inputs and the local cast aliases, with no change to
the playback or movie-track work. Every real caller supplies the pointer.

Keep deliberate extra-argument calls through the existing unprototyped
declarations: for example DDS2 `evtViewerFrameChangeUpdate` also supplies
its step and input-table address. Likewise `0024C650` sets `a0=viewer`,
`a1=value` and `a2=curFrame` for `mnuFxWorldScrollDelta`; do not invent its
three additional incoming scratch values. The timeline provider's
pre-existing six-formal contract is separate debt, not changed here.


## Room probes use their primary descriptor and geometry

The DDS2 `0013DDC0` controller follows the existing task descriptor's
shape pointer at `+0x0C`; its kind and dimensions belong to that serialized
shape, not a second descriptor view. The three facing probes now read
the same `FldTaskInfo.shape`. Room planes and limits are already members
of the complete `0x140`-byte `FldRoomState` at `+0x90` and `+0xF0`.
Both plane predicates use that owner rather than the `D_00444BC0` alias
and its parallel padded type. This type closure does not match the
controller's separate constant-lifetime and branch differences.


## Disabled diagnostic drawers retain finite vector inputs

DDS1 `001FB168` / `001FB178` / `001FB198` and DDS2
`0020D1E8` / `0020D1F8` / `0020D218` are empty retail drawers, not
zero-argument interfaces. Their camera-diagnostic callers deliberately
form a vector pointer; a vector pointer, packed color, integer flag and
float radius; or two vector pointers and a packed color, respectively.
For example `002B800C` / `002FF264` pass the computed horizontal radius
in `f12`, with the point/color/zero flag in `a0`/`a1`/`a2`.
Retain those unused formals while leaving the genuine empty bodies alone.
This finite caller-evidenced closure does not match the still-assembly
diagnostic controllers or claim that the disabled drawers consume input.


## Field trigger shapes are variable-sized tagged records

The `0013EB30` action constructor places an eight-byte kind-0 sphere
record (kind and radius) immediately before its sixteen-byte task
descriptor. The descriptor's shape pointer refers back to that prefix.
The room binder reads two dimensions for kind 1 and three for kind 2;
those plane and box records occupy twelve and sixteen bytes respectively.
`FldTriggerShape` therefore names their real serialized variants in a
kind-tagged union, rather than treating every shape as an oversized box.
The existing three facing probes consume its common kind word exactly.
The complete constructor candidate remains private and non-matching;
its separate `0x30`-byte transform and two primary `FldFileResource`
descriptors are not replaced with partial descriptor views.

DDS1's slot reset and three facing probes share the same complete
sixteen-byte `FldTaskInfo` descriptor as DDS2; its shape pointer is at
`+0C`. Keep the slot and tagged-shape accesses on that primary descriptor,
not separate padded prefix views. The named scene-task provider takes a
world-node pointer and a task-name string and returns a signed task ID.
Its two existing DDS1 field callers retain their exact bytes after that
declaration is corrected; the room/task controller remains assembly.

## Result icon ramps distinguish storage from conversion

DDS1 `func_00267850` uses signed lower/upper clamps for
`BrsProgressAnimation.iconColor`, `completionColor` and `auxiliaryColor` at
`+40/+50/+54`. Their storage is `s32`; the constructor values alone do not
establish unsigned storage. The sine-derived icon value still uses a `(u32)`
conversion before assignment to that signed field, preserving the native
conversion path. Storage signedness and expression conversion are separate
contracts. Its two coordinate pairs at `+44/+58` are actual two-element
arrays, with the complete `0x68` row and all shared-header users checked.

## Result EXP flash counters retain signed scalar storage

`00266E28` and `0029E820` increment the level-up flash frame through
`0..15`, subtract `0x10` from its opacity, and add eight to the row
opacity. Their lower clamps use `BLEZ` and their upper clamps use signed
`SLTI`; these are signed integer counters, not packed color words.
The DDS1 primary `BrsProgressAnimation.alpha` at `+1C` and
`iconOpacity` at `+48` are therefore `s32`. Its byte at `+41` is
set during level-up and cleared when the flash fades out.
The DDS2 counterparts occupy `+2D`, `+30` and `+34` inside the same
complete `0x68`-byte progress row.

DDS2's native `B050 + index * 68` base followed by a `+10` byte access
reaches the canonical `B060` row's `drawPhase`, not its separate `state`
at `+10`. Keep the established `B060` bank origin instead of inventing
a biased record view. These owner completions do not claim that either
still-assembly EXP state controller has been matched.


## Mantra grid custom data is the scene-owned node array

DDS1's grid factory passes `MenuSceneWork.nodes` at `+004` as the
SDK grid's word-backed `userData`. The initializer at `00253208` clears
`0x480` bytes (96 twelve-byte records), fills IDs 1 through 88, and writes
signed halfword X/Y, their decimal remainders, and the state word at `+08`
(`002532E8`, `00253354`, `0025335C`, `00253360`, `00253368`).
The same primary `MnuMantraNodeState` replaces the separate coordinate,
neighbor-status and prerequisite-status prefixes. Grid and scene extents
remain `0x34` and `0x5B0`; the factory no longer constructs a raw `+4` view.

The connector at `00258258` receives this array as its ninth argument,
not the independent `D_0036B7F0` sprite-placement table. Its `LBU +2`
reads the low byte of node Y, not a newly invented sprite rank field.
The initializer and reset interfaces retain `MenuSceneWork *`, matching
the real pointers supplied by all callers; SDK word getters still decode
their own generic retained values at the existing boundaries. Completing
this owner does not certify the still-assembly connector or initializer.

## Model viewer position-draw flag retains signed byte storage

DDS1 `mdlViewerEnd` reads the primary `MdlViewState.unk0E` flag with
`LB` at `0021DCB8` and `0021DD18`. Keep this unit-local field as `s8`;
its menu consumer and all other existing C functions remain byte-exact.
This type correction does not claim that the viewer callback itself
has been matched: its current honest C candidate still differs.


## Shooting input tilt is integer storage with floating-point arithmetic

DDS2's `00318660` controller reads shooting work `+90` as a signed word,
converts it to float, changes it by `1.0f` against the `+/-20.0f` limits,
then converts back to a word. Keep that field as `s32`, not `f32`.
The adjacent `+94` selector is a signed halfword populated from the menu
work flag word's mode bits; the existing round stays at `+96`.

The same controller both extracts bits and performs full-word `OR 4` and
`OR 0x20` at `+70`. A documented union exposes that flag word alongside
the existing pause/initialized bitfields without changing their names or
the `0x1E0` allocation. This completes the canonical owner, not the
still-assembly controller. Credit PiM's released `6017625724` investigation
for the earlier private identification of these fields.

## Pending spatial sounds retain radius before XYZ

DDS2's `FldClear18` is one `0x18`-byte owner: flags and sound ID at
`+0/+4`, radius at `+8`, and world XYZ at `+C/+10/+14`.
The `001445D0` producer stores its first float at `+8`; the placement
caller supplies a radius followed by the actor's three coordinates.
The `001447A0` consumer uses `+8` for both range and sound scaling.
Rename the existing primary members and constructor parameters together;
do not add a parallel position/radius view. This closure does not claim
that the spatial-source update itself has been matched.

## R5900 square-root operands use FT, not standard-MIPS FS

The original EE assembler emits `0x46020044` for `sqrt.s $f1,$f2`
and `0x46010084` for `sqrt.s $f2,$f1`. R5900 `SQRT.S` therefore reads
the FT field. A generic MIPS decoder can incorrectly report F0 as the
source for both words. In `001447A0`/`001415F8`, the two 500-unit checks
really take the square root of the Y delta squared; do not rewrite them
as full-distance or X-only checks based on that decoder output.

## Field label tables retain coordinate headers before encoded text

DDS2's `FLDALL.TBL` loader reads `0x3C00` and `0x4400` bytes for
512 records of 30 and 34 bytes. The matched `001237B0`/`00123808`
lookups read signed coordinate halfwords at `+0/+2`; encoded label text
starts at `+4` and occupies 26/30 bytes. Share those complete records
between the loader/lookups and label-length cache, rather than inventing
stride-sized string objects based at the interior `+4` addresses.
The banner's primary/alternate selection indices are signed words at
`FldAreaWork +C4/+C8`; the initializer and both retail banner consumers
use `-1` as the no-selection sentinel. These owner completions do not
claim that the remaining banner controller has matched.


## Battle text handles cross the real glyph draw boundary

DDS2's command-list text helpers receive a 32-bit handle from
`itfCreateConvertedTextGlyph`, then pass its glyph pointer to
`frFontDrawGlyphWithSharedFlags(FrFontGlyph *, s8)`. The provider in
`interface/frFont.c` forwards that pointer and mode to the chain renderer.
Use the same explicit word-to-pointer boundary as the adjacent
`frFontQueueGlyphForCurrentDrawBuffer` call, rather than relying on an
implicit integer argument. All three callers (now in `game/code_001B2AF8`)
retain their retail instructions. This contract correction does not
claim that the still-ASM command-list renderer has matched.

## PCP block sets retain full vector and parameter owners

The paired `0017DCF8`/`00185950` updaters copy the quadword at work `+50`
to `+40`, then store the model's returned position at `+50`. These are
`currentPosition` and `previousPosition`, not an opaque 0x20-byte pad.
Their shared 0x50-byte parameter record starts at work `+60`: position
at `+0`, signed fade durations at `+10/+14`, float scale at `+18`,
three group sizes at `+1C`, float group scales/random scales at `+28/+34`,
scatter factors at `+40`, and the signed tail-start frame at `+4C`.
Complete those fields in each game's existing primary owner; do not add
a second float view beside word arrays. Constructor/clone whole-record
copies, the 0x10C work allocation, and existing `u32` frame/count fields
at work `+B0/+B4` remain unchanged. The updater bodies are still ASM;
their native modes 1/2 omit local matrix initialization, so a future C
body must not invent an identity-matrix fallback.

## DDS2 spark scripts belong to their primary resource rows

`func_00150800` finds a row through `fldFindResourceRecordIndex`, multiplies
its index by `0xE0`, and stores that same `FieldResourceRecord *` at
`fldSparkControlState +4`. Its signed halfwords at `+4/+8` supply the slot
count and duration; `func_00151498` returns the signed parameter at `+6`.
The collector addresses sixteen 12-byte sequences starting at row `+20`,
each with a signed word count and eight signed-byte slot IDs. Complete
`FieldResourceRecord` itself rather than introducing a second asset view.

The DDS2 controller's actual payload ends at `+48` (size `0x4C`). Its
inserted `+8` word and pickup/scheduled-sound bookkeeping differ from
DDS1's `0x40` controller, so do not homogenize their offsets. In DDS2,
`func_00150F20` uses signed countdown comparisons at `+1C`; the collection
window separately interprets `countdown - 10` as unsigned before testing
it against six. Field-area pickup count/score at `+106/+108` are cleared
by the initializer and incremented by the collector. Existing field
getters and controller clients use their primary members, not interior
global aliases. The precise PRNG contract is
`u32 effMiscRand(struct EffRandState *)`; null selects the default state
in the real `00340AC8` provider.

## DDS2 HP/MP command factors use the primary battle owners

The two amount resolvers call `BtlState.commandAmountScaleHook` at `+708`
with source, target, command ID and operation, and multiply its `f32` return.
The solar factor is selected from two adjacent nine-float rows at battle
parameter `+A98/+ABC`, covering the mirrored phase provider's `0..8` range;
source flag `0x200` selects the first row. Native `LWC1` accesses also establish
the independent parameter scalars at `+B64/+B90/+B94`. Complete the existing
owners without changing their layouts or adding alternate raw views.

Both retail callers of `btlGetClampedBattleTableValue` deliberately pass
the source actor in the call delay slot, even though the 60-byte provider
ignores it. Its index reads the low halfword of the existing `activeGroupCount`
at `+47C` and clamps it to four. A natural `u16` local assigned from that
primary word member still emits the native `LHU`; neither an interior cast
nor a second count view is needed.

## Solar sprite ownership and stalled view retirement

`SolarOverlayWork.noiseSprite` is the `EffectSlotSet *` returned by
`effLoadIndexedResource`, not an encoded numeric handle. The paired load/free
helpers accept its output slot as `EffectSlotSet **`. Their `/itf/` base paths
are character arrays in `.sdata`. The frame renderer also increments, tests
and resets the signed word at overlay `+0x100` (state `+0xFC`), now
`pulseFrame`; the state and whole-work extents remain `0x100` and `0x104`.

The legacy solar layer/context projections still need retirement. Their
`0xA0` layer width/height at `+0x0C/+0x10`, saved dimensions at `+0x7C/+0x80`,
and context array at `+0x18` correspond to canonical `BdWork` and
`EffectSlotSet`. A direct canonical rewrite retains the algorithm but changes
26 of 95 and 24 of 99 words in the already-matched layer providers, beginning
at `+4` with table/register allocation. Full-work versus resource-slot
pointer formals do not close it; a three-word placement record also fails.
Keep those projections as existing debt rather than publishing regressions
or restoring byte-address arithmetic alongside the typed owners.


## Model selection and filename tables

`mdl_resource_table.h` owns both games' eight-byte selection records and
twelve-byte filename triples. The viewer writes the selection suffix with
two separate `sh` stores, so the formerly unused word is two halfwords.
The triples hold resource-list, model and motion paths, not numeric handles;
the model manager and viewer now share their types. The separate selection
and path tables use the existing generic pointer/count descriptor.
DDS2's slot setter takes three filename pointers: its battle callers pass
the `.data` strings `human/pc001_00.PB` and `human/pc001_01.PB`.
DDS1's `func_00218BE8`, like DDS2's `func_00233700`, receives the allocated
chip-cell pointer directly and forwards it to `sdfReleaseChipBlock`.

## Scene AI sprite angles and scale-call boundaries

`SceneAiWork.panelAngleDegrees[4]` owns the four floats at `+0x30`;
the `0xA4` allocation and later members are unchanged in both games.
The paired renderers copy one angle to `BdWork.geometry.angleDegrees`
for each panel draw, then reset the resource angle to zero.

The paired scene scale routines receive an `EffectSlotSet *`, slot index,
two integer output pointers and an `f32` percentage. The first float
argument is passed in `f12`; declaring that fifth formal as an integer
loses the native ABI. The routines update the canonical sprite bounds
and write the signed half-size adjustments to the two outputs.

## Battle entry providers use the canonical unit

DDS2's entry-code getter, setter, expiry query, match query and multiplier
take `BtlUnit *`, as their DDS1 counterparts already do. The seven signed
six-byte records are `BtlUnit.entrySlots` at `+0x2E6`, and the multiplier
reads the same unit's flags at `+0x110`. These providers are not methods of
the old local `UiObject` view. The provider unit and the existing entry-query
users in both games remain text- and data-exact after the parameter cutover.

## Field spark dialog twins

DDS1 `func_0014C648` and DDS2 `func_00150A60` share the twelve-phase
spark/message transition controller. The dialog timer is the same physical
member at DDS1 `FldSparkController.unk2C` and DDS2 `unk30`; the sequence
resource is title-specific (`0x670010` versus `0x680010`). DDS1's retained
request `D_003BAFE8` is a `FileRequest *`, while `D_003BAFEC` and
`D_003BAFF0` are the resource-handle and loaded-data words. Keeping that
request typed removes the integer/pointer adapters in both controller paths.


## Paired-actor command roster records

DDS2 `func_001AC750` clears a halfword count and writes twelve-byte rows
starting at `+4`: a skill ID and two actor pointers. `BattleRosterTable`
describes that variable-length record without guessing its backing capacity.
The shared record is in retail `.data`, so its extern retains that section
rather than letting the four-byte flexible header imply GP-relative storage.
The scene counter's scratch genuinely holds signed IDs or an unsigned count;
its subsequent signed first-halfword read is preserved by the documented union.


## Secondary phase work and mirrored-sprite actor arguments

DDS2 `func_001BC8A8` shares the existing `0x138`-byte phase-panel allocation:
the signed secondary phase is at `+9`, its counter at `+0x24`, and its
integer angle at `+0x28`; the arrays still begin at `+0x38/+0x78/+0xB8`.
The mirrored-sprite helpers `func_001B6FC0` and `func_001B70B8` do not read
their first argument. Their sole retail caller passes the current `BtlUnit *`,
not an integer ID, so retaining that pointer formal avoids false scalar calls.


## Converted scene text retains its glyph pointer

`itfCreateConvertedTextGlyph` returns `struct FrFontGlyph *` and accepts a
glyph parent of that same type. DDS2's scene text helpers retain this pointer
through draw and queue calls instead of transporting it through a scalar ID.
The provider's existing definition and both whole-unit helper gates establish
the contract; the local declaration does not change the call ABI.


## Signed mantra IDs and promoted panel status

DDS2 `MenuPanelPositionRecord` contains a signed ID and an unsigned state
halfword. Both the reset `func_00291338` and lookup `func_00291400` read
IDs with `lh`; the flags use `lhu`. The three banks each contain 37
four-byte records. A supposed “unknown upper halfword after lhu” is not
a missing owner: unsigned halfwords still promote to signed `int`.

The low-nibble update `(flags & ~15) | status` recurs in the reset,
`func_002917C0` and `func_00292998`. The common signed-word status builder
preserves its promoted arithmetic before assignment back to the halfword;
it also constructs the reset value from zero flags and status 2. The two
existing C callers remain exact. The reset remains assembly: its truthful
196-byte replay is eight words away, chiefly the mask/reset register swap.


## Scene actor-limit query's unused owner

DDS2 `btlIsSceneActorCountWithinLimit` ignores its first argument, but both
retail call sites (`0x1C8EC4`, `0x1C9ACC`) pass the unit at `owner->unit`
in their delay slots. Its first formal is consequently `BtlUnit *`, not an
integer ID. This local signature completion leaves the query's text unchanged.


## Descriptor-bound effects retain their second vector pointer

`effObjSpawnDescriptorBoundEffect` and `effObjCreateWithBoundBill` pass
both vectors to `effObjCreateWithVectors` as pointers. The paired event
viewer callers supply real four-float arrays; the named-resource wrapper
already receives a pointer. Keeping the second vector typed through that
chain removes the address-word conversions without changing instructions.
DDS2 retains its existing K&R constructor definition.

This closes the descriptor/bound-bill argument path only. The integer VM
setter return debt remains unchanged, and does not license a false pointer
return, a new object view, or a wrong-prototype target landing.


## Synthesized polygon-movie headers return allocation handles

The paired PMD2 `evtPolygonMovieCreateHeader` and PMD3
`func_00234C18`/`func_0024F9B8` constructors return the `SdfMemBlock *`
obtained from `sdfAllocGeneralBlock`; their output arguments separately
receive the retained data address. Returning the handle directly retires
the pointer-to-integer adapters without changing any constructor text.
No existing C caller or shared declaration uses these four providers.
The three-resource movie loaders remain assembly: natural grouped paths
and these pointer contracts retain the two-word argument-setup delay-slot
swap at `+0x80`/`+0x84`, so this closure earns no new matched-body credit.


## Model-node alpha update: visibility proves text, not a TU cutover

For DDS2 `func_0031CBC8`, an identical-header control leaves `bc1f` instead
of retail `bc1fl` at `+0xD8` (135/136 words). Compiling the authentic
`mnuSetNodeModelBroadcastByte` definition before the unchanged caller makes
all 136 words exact, confirming a real callee-visibility dependency.

Moving the boundary to `0x31C8B0` is not qualified: that setter needs its
existing no-sibcall setting to retain the retail `jal mdlBroadcastMasked`
and epilogue, whereas `func_0031C940` has a native sibling jump at
`0x31C9E4`, and `itfDrawModelInstanceImage` has two at `0x31D750` and
`0x31D7C0`. The setter's post-call instructions only restore registers;
there is no `$v0` extension or result transformation. Its real callee
returns `void`, and no evidence supports inventing a different setter
return contract. Keep the caller in assembly until independent source or
contract evidence explains both the visibility and flag boundaries.


## Model billboard parts forward opaque descriptor addresses

The paired `mdlAddBillboardPart` providers at DDS1 `0x219AF8` and DDS2
`0x234668` receive the PAC node's `dataCursor`, not a descriptor-table
index. They do not dereference that descriptor: they forward its address
to kind 1 of `billCreateIndexed` and store the returned billboard.
Consequently the part helper uses `void *descriptor`, while the existing
kind-dispatched `billCreateIndexed(s32, u32)` interface retains its opaque
payload word. The conversion to that word is an actual dispatch boundary,
not an integer prototype used to influence allocation.

Both 88-byte provider bodies stay exact. The native parsers supply the
address with `lw` from `PacWork.dataCursor`; no matching C caller needs an
integer adapter, and this closure does not claim a parser-body match.

## Terminal text uses the existing glyph pointer contracts

DDS1 `code_00248580`'s atlas-slot and number-sprite clients return and queue
`struct FrFontGlyph *` from the existing text providers; their handles are
not integer IDs. The local declarations now agree with the providers,
including `frFontDrawGlyphChain`'s `s8` option and `u32` priority. No provider
or shared header changes are needed. The selected-row candidate at
`0x248E68` remains ASM: its two independent instructions at `+0x9C/+0xA0`
are still exchanged, so this adapter retirement claims no new body match.

## Viewer command history retains its runtime pointer

`evtViewerPushCommandHistory` receives the same `EvtRuntime *` as its
native option-handler callers. DDS1 `0x22FF30` and DDS2 `0x24AB38` immediately
read that object's history count, update its action mode and append the
three history halfwords; the argument is not an encoded task-user word.
The paired declarations, nine callers in each game and the 44-byte
forwarders (`0x230A38`/`0x24B678`) therefore pass the pointer directly.
Actual task-user address decoding remains at the scheduler boundary.

Both 100-byte providers and 44-byte forwarders remain exact under the
existing unit flags. This source-only contract repair does not claim an
option-confirm body match.

## Enemy tick byte and unit-local roster stat view

`DatEnemyRecord` byte `0x47` is the tick count read by the DDS2 battle
action-frame path (`func_001D5FB0`, `code_001D4438`); zero is read as one.
The header names it `tickCount`. `datRosterDetails` entries (stride `0x14`)
carry the count at `0x11` and the amount at `0x12`. The sibling
`EventRosterStat` layouts disagree (`0x0E/0x0F` in `code_001A5BB8.c`, `pad08`
in `code_0011A118.c`), so `code_001D4438` keeps a unit-local `DatRosterDetail`
view until a shared record is reconciled. This note claims no body match.

## Title-audio states own the complete decoder record

The title stream and secondary sound-buffer state are both `0x28`-byte
`TitleAudioStreamState` records, not unrelated word arrays or a sample-only
prefix view. Their compressed-data, PCM, decoder and allocation pointers
occupy `0x14`, `0x18`, `0x1C` and `0x20`; load state is at `0x24`.
The decoder remains an opaque SDK word array, while the allocation is the
real `SdfMemBlock` descriptor. Explicit address-word conversions remain only
at the SDK's word-valued address API and PCM-buffer representation boundary.

Credit Basalt's released title-stream support for the complete owner evidence.
The paired source-local cutover preserves every existing C function:
DDS1 `code_00268AB8` gates `65 match, 0 differ` and DDS2 `code_002A05C0`
gates `71 match, 0 differ` in the private proof. The condensed status snapshot
still transfers just frame count, frame index and repeat frame. This owner
closure does not claim a match for either assembly-retained stream updater.

The title-audio tick limit (`D_003BC5C8` / `D_00437A38`) is an `s32`:
retail uses signed `slt` against the incremented tick, and its C setters
store `6` or the signed load state. Keep one declaration per owning unit;
the unused declarations left in DDS1 `code_0026BD58` are retired.

## Field loaders borrow serialized texture-offset lists

`func_00128780` and DDS2's `func_0012AC90` borrow a
`const SdfTextureOffsetListHeader *` as their fifth argument. Their resource
builder already has that canonical input type, and both world-object callers
already receive the same typed list. Do not convert that pointer to an address
word merely to satisfy an obsolete local declaration.

Keep the six-word caller contract: DDS1 `0x110BA0` and DDS2 `0x110DC8`
deliberately move the wrapper's seventh argument into the loader's sixth
argument register in the call delay slot. The assembly-retained loader does
not read that final word. This input-type closure does not claim either
loader body is matching C.

## Shooting pause choice uses a complete fade-number owner

DDS2 `func_0031B080` passes `MnuShootingWork +0x14C` to both the
`func_0031ED68` display-value setter and `mnuDrawFadeSequenceThree`. Their
`FadeNumber.displayValue` access at `+0x18` identifies the work's `+0x164`
word; the resetters clear the complete `0x1C` bytes. Keep this payload as
`pauseFade`, not a byte pad cast to its fade-state prefix. This owner closure
does not solve the retained caller's retail JAL-versus-sibling-jump frontier.


## DDS1 result-animation rows start at their phase byte

The `0x68`-byte DDS1 result rows start at owner `+0xEF4` (level) and
`+0x1234` (profile), not twenty bytes earlier. `func_00268AB8` operates on
the profile phase at `0x1234 + index * 0x68`; `func_00267FF0` writes its
icon and completion fields, and `func_00268D40` writes the initialization
flag and previous progress at row `+0x60`/`+0x64`. The renderer
`func_00268590` reads auxiliary color/X/Y at row `+0x54`/`+0x58`/`+0x5C`.
These are the same record's tail, not a sixth sentinel or an overlapping
rendering view. Both arrays still contain five rows; the owner stays
`0x1590` bytes and DDS2's layout is unchanged.

All fifteen existing header consumers remain exact under this primary
owner correction. The four-icon renderer itself remains assembly:
three genuine palette-copy/representation forms leave a smaller frame,
different pointer/register retention and broad scheduling differences.

## Effect allocations share a prefix, not a complete owner

DDS1 mode `0x108` owns the `0x18`-byte timing/vertical-motion record;
mode `0x10B` owns a distinct `0x10`-byte selected-boss record.
`btlInitRandomBossSelection` (`00208D30`) and `btlSwapRandomBossSelection`
(`00209238`) access the selected ID as a full word at `+0x0C`, not as the
timer/active/phase bytes of the other allocation. Both complete owners
embed `BtlEffectHeader` first. Generic actor/count code uses that prefix;
mode-specific code recovers only its actual containing owner.

Upstream `87e2edb1`'s matching `00209528` boss-vector reseed belongs to
the latter owner. Its actor accesses migrate to `header.actor` without
changing its control flow, provider calls or vector-copy work.

DDS2's native `00229728` dispatch table assigns `00221BC8` to mode
`0x315` (789), not `0x316`. `0022A11C` through `0022A17C` allocate and
clear 16 bytes, matching the existing `BattleActionState`. Its alternate
motion byte at `+9` is signed: `00221C48` uses `LB`, and `00221CA4` uses
`SB`. This is a field of that owner, not a view of the DDS1 timing word.


## Mantra flag lookups copy their owned constant tables explicitly

DDS2 `func_0028F128` copies four ID/flag tables (14, 4, 8, 8 halfword
pairs) and nine signed party offsets into automatic arrays before its
first-hit scans. Declare the actual source tables at `004272F8`,
`00427330`, `00427340`, `00427360`, and `00427380`, then copy each with
`memcpy`; keep their existing `INCLUDE_RODATA` ownership. Synthesized
literal-array initializers instead allow gcc to reorder the table copies
and introduce new literal data. The explicit-copy form matches all 150
native words and preserves all seven surrounding C functions (8/0 unit).

## Scene-pair result checks carry actors, not integer addresses

`btlCheckScenePairResult` passes its own actor and both descriptor actors to
`func_001ABDE8`, whose four actor inputs are actual `BtlUnit *` owners.
The result-6 validation calls `func_001ABA40`, which reads that actor's
party record. Keep the wrapper's formal, both `SceneCheckArgs` members,
and every query declaration pointer-typed, including the fourth nullable
actor in the linked-command caller. The unused second local descriptor
definition and query declarations in `code_001D4438` are not another view.


## Battle overlay visibility and packet-list owners

DDS1 `btlInitVisibilityGrid` allocates exactly `gridWidth * gridHeight`
bytes; `00212998` reads each visibility value with `LBU`. Keep the
source-local runtime owner as `u8 *`, including its initializer and
release local, rather than an opaque allocation with a signed-byte view.
The overlay providers and their fade caller carry actual `SdfListHead *`
values through to the existing SDK list APIs. The textured-cell provider
forwards that same owner to `effAppendTexturedTrianglePacket`.

This does not turn SDK address-word interfaces into pointer returns:
`sdfAllocPacketAligned` still returns its native `s32` address word, and
`sdfAppendPacket` still takes a `u32` packet address. Convert only at those
documented word boundaries. Native `SD` operations justify the existing
four-`u64` `SdfPacket`; no alternate packet owner is needed. Credit
Purist6c's complete prior overlay reconstruction for the owner-gap lead;
the `00212998` body remains unlanded until an honest full match.


## Boss camera selection takes a pose, not a second command

DDS2 `00222450` saves incoming `$5` as its camera destination and `$6`
as the rotation option, independently of the command in `$4`. Its
declaration therefore takes `(BtlLinkedCommand *, BtlCamState *, s32)`.
The `00223DD8` caller supplies `&unit->camera`, the real camera prefix
at zero, rather than passing a command through an unprototyped call.
This contract closure preserves all 253 existing C functions exactly;
the 640-byte selector itself remains assembly (prior 34-word frontier).


## Display text wrappers borrow the actual character buffer

DDS1 `itfDrawGlyphChainWithWidthQuery` (`002CA8F0`) and
`frFontQueueFlaggedGlyphAndMeasure` (`002CA988`) take their sixth argument
as `char *` and forward it to the text-glyph constructor. The display
callers in `code_00254B30` therefore pass their formatted `char` arrays
directly; their local declarations must agree with those existing
providers. This is not an SDK address-word storage boundary, so no
pointer-to-`u32` round trip belongs at these calls.


## Reposition wrappers forward the formation-change result

`btlRepositionPartyAroundBattleCenter` (`001F53C0` in DDS1,
`00206060` in DDS2) forwards the signed `0`/`1` result of the
inner party-placement routine. Its C definition and local declarations
must therefore return `s32`, even where a caller intentionally discards
the result. DDS2 summon setup snapshots `$v0` at `001DB7D4`, in the
following call's delay slot, and uses that snapshot to decide whether to
start the command-sound tasks. This is the inner routine's real result,
not a fabricated return or a fall-through register value.


## Movie packet builders borrow the complete stream owner

DDS2 `func_002A7B28` takes `MovObj *`, as the existing
`mnuMovieDrawNextProc` caller already supplies, plus `SdfPoolNode *`.
Use the canonical nested stream-frame fields and pool append callback
when reconstructing it; an integer-address or second movie-record view
is not its contract. The 512-byte packet body remains assembly.


## Counter draw callbacks forward six ordinary arguments

DDS1 counter dispatch at `002C4AD8..002C4AF0` deliberately sets
`$4..$9`: x, y, z, the counter runtime, the channel, and the incoming
draw context. The last value is saved from incoming `$8` and forwarded
through `$9`; it is not a residual register. Use the existing six-formal
`SdfCounterDrawFn` convention from DDS1 `code_002C5FD8` and DDS2
`code_0030B838` in DDS1 `code_002C3868` too. Its label-plate provider
receives a named `s32 drawContext` formal even though that provider does
not consume it. This contract change leaves both its dispatcher and
label-plate machine code unchanged.


## Draw-packet payload getters forward opaque pointers

`billGetWorkTransformMatrix` (`0015F810` in DDS1, `00167400` in DDS2)
is the same eight-byte leaf in both games: `jr ra` with
`addiu v0,a0,32` in the delay slot. Every C consumer passes the
`void *` draw packet returned by `effCreateSizedDrawPacket` and uses
the payload after its 32-byte header. The getter therefore takes and
returns `void *`, using byte-pointer arithmetic internally; it does not
take an `EffectDispatchState *` or shuttle a software pointer through
`s32`. Blur callers interpret that opaque payload as their real quad.
This does not change the SDK allocator's address-word return or the
`u32` packet-address argument to `sdfAppendPacket`.

The existing draw builders `func_0015FE20` / `func_00167A10` also return
the allocated `void *` packet. Their effect/particle caller declarations
must agree; an integer return is not the SDK allocation API. Where a
caller immediately submits that packet to `sdfAppendPacket`, the explicit
`u32` conversion belongs only at its physical-address second argument.
The builders, descriptor layouts and genuine allocator word ABI stay
unchanged.


## CPU list wrappers preserve the list owner, not its DMA address words

`evtBuildFrameStatePacketList` returns the `SdfListHead *` created by
`sdfCreateResetPacketList`; its frame callers append that same owner to
their `SdfPoolNode`. The background resource/descriptor builders retain
the list returned by `sdfAllocatePacketList` as `SdfListHead *` too.
`sdfConsAppendProgramReferencePacket` receives that CPU list owner as
its first argument and a `DmaPacketHeader *` as its second argument.
Only the second argument is converted to the genuine packet-address
word submitted to `sdfAppendReferencePacket`. Neither the software
list wrappers nor their callers need an integer-to-pointer round trip.
Keep encoded program addresses, DMA tag words, and allocator/range
address-word interfaces unchanged.

The camera FOV editors and mirrored-unit resource/descriptor submissions
also keep factory results as `SdfListHead *` through the matching pool
callback. Their float calculations, input predicates and model-creation
control flow are independent of that software-list owner.

The image-outline and textured-quad/triangle appenders likewise receive a
`SdfListHead *`; the packet allocated inside each appender still becomes
a hardware address word in the second `sdfAppendPacket` argument. The
two `itfDrawPulsingTestOverlay` functions keep their reset-list result
as that pointer through packet appends and the surface callback. These
closures change no allocator, GS/DMA word, statement order, or callback.

The short field GS-command/frame/vector submissions, generated/composite
texture submissions, and UI sprite-outline submissions keep their
`0x20`-byte software list as `SdfListHead *` through initialization,
draw helpers and pool append. Decode the allocator's word only once;
their separate GS packet locals and physical submission words retain
their original address-word representation.

`FldQuadState.packetList` at `+0x28` is the reset-list pointer, not a
transport word. Its start/advance helpers receive the actual quad owner,
and every local draw user forwards the stored pointer directly. Particle
draw buckets likewise store CPU lists. Their draw receiver is the existing
`SdfPoolNode`, whose `+0x10` callback is `append`; the unused `ParDrawCmd`
prefix view is not a second owner. Physical payload addresses passed as
the second `sdfAppendPacket` argument retain their word ABI.


## Battle panel corner clearing retains the retail redundant test

DDS1 `func_001B05D0` retains the corner-1/3 test at `0x001B079C`
even though both arms clear the low color byte. This is native work,
not an invented scheduling branch. Its 37 x/y/texture triples and four
initial colors are function-local initializers replacing only that
function's own `D_003A2608` and `D_003A27C8` data inclusions.

## Model viewer surfaces use the canonical SDK pool owner

`D_00325048` (DDS1) and `D_00380048` (DDS2) are the native interior
symbols for `kwlnDrawSurfaces[40]`: each is `0x500` bytes after the
array base, whose canonical `SdfPoolNode` entries are `0x20` bytes.
Their callback at `+0x10` is therefore `append(SdfPoolNode *,
SdfListHead *)`, not a separate viewer-device callback taking an
integer list. Keep the typed native interior externs: using the array
origin instead changes the compiler's addressing in all three viewer
submission functions. Both games' list locals and callbacks remain
byte-exact with the canonical owner and native symbols.

The shop builders also retain their `sdfAllocatePacketList` result as
`SdfListHead *`. Frame initialization passes `&scene.link` to the
linked-payload helper, rather than casting the complete `SdfSceneNode`.

## Viewer object creators consume the canonical timeline owners

`evtViewerCreateObjectInFreeSlot` at DDS1 `0x0022C7F0` and DDS2
`0x00247168` consumes an `EvtRuntimeGroup` and its `EvtRuntimeChild`,
not separate command/parameter prefix views. Track type, resource and
plain-mode byte are `type`, `resourceData` and `metadata.extra1`;
the kind-specific byte/halfword parameters use the existing `p08`/`p0C`
union members. Only the pre-existing integer-address SDK boundary
(`func_001150B0` / `func_00115318`) converts `resourceData` to a word.
The owner migration leaves both 600-byte creator bodies byte-exact.

## DMA formatter output buffers are CPU pointers

DDS1 `func_002D4C80` / `func_002D4CC8` and DDS2 `func_0032DB30` /
`func_0032DB78` build reference-chain tags in a caller-provided
`SdfDmaReferenceChainPacket *`. Their first argument is different:
it remains the SDK source-address word, with the native
offsets added before `sdfBuildDmaReferenceChain` encodes it. All C
callers retain the allocated output buffer as the canonical pointer;
only its eventual DMA submission converts it to a `u32` packet-address
word. The allocator's address-word ABI, source offsets, tag encoding,
and packet submission order remain unchanged.

## Viewer dispatch shares its resource providers

DDS1 `func_0022CED0` needs the preceding `evtEventViewer.c` resource
providers visible. Moving only `evtEventViewerFreeBuffer` before it does not
fix the branch: both the release and constructor calls need their genuine
same-TU `REG_EH_REGION 0` notes. Controlled probes keep pass-28 branch UID
276, target and epilogue donor UID 1658 identical; reorg alone changes the
donor from taken-only to unconditional (`bnel` to retail `bne` at +0x124).

Independent ownership evidence is the adjacent Nocturne-mapped release
at `0022CB68` and the preceding voice-conflict diagnostic `D_003ACFE8`,
whose consumer is this dispatcher. DDS2 mirrors that ownership with
`D_004224A8`. The recovered DDS1 leading region ends at `0022D420`;
the following jump table starts at `003AD0C0`. The recovered DDS2 region
ends at `00247DE0`; its dispatcher's two tables end at `00422580`, where
the following unit's rodata begins. Both complete dispatchers match with
their resource providers in the viewer unit. No flags or attributes change.

Declare the nibble unpacker's actual `void` return before calling it. In
the DDS2 combined unit, an implicit integer-return declaration creates a
`call_value` RTL and assigns the following key-frame load to `v1`, rather
than retail's `v0`. The existing provider writes two signed 32-bit outputs.
Native zeroes and passes adjacent slots at `sp+0` and `sp+4`; a two-element
`s32 nibbles[2]` array expresses those outputs directly. Keep the preceding
voice-conflict diagnostic before the generated switch tables in rodata.

Writing the source fields in pending/message/frame order emits the native
loads and pending/frame/message stores without artificial temporaries.

## Blur and rectangle sources retain CPU texture pointers

`EffBlurTemplate`, `EffBlurScatterWork` and `EffBlurScaleWork` store the
selected `SdfTex *` at `+0x2C`; `EffResourceRectWork` stores it at `+0x24`.
The four viewer channel caches carry the same pointers selected from
`EvtRuntimeGroup.texture`, not resource indices or DMA address words.
DDS1 kind work retains a direct texture. DDS2 retains the existing
three-word `EffKindAssetHolder` (`kind`, reference count, texture); its
create/retain/release users share that primary owner rather than array views.
The native kind tables contain exactly four non-null texture initializers
per game, each receiving the kind-work pointer and its selected texture.

The blur and rectangle constructors copy only their `0x2C`/`0x24` parameter
prefix with `memcpy`, leaving the selected texture outside that copy.
With the truthful pointer field, typed aggregate assignment changes the
texture-store/copy scheduling from `+0x1C`; the byte-prefix copy preserves
the native store-before-copy sequence. Do not restore an integer texture
field to conceal that source-level distinction. Physical packet submission
addresses and the allocator's genuine address-word interface stay unchanged.


## DDS2 kind clones return their allocated work

`effCloneKindWork` and `effCloneAlternateKindWork` return `EffKindWork *`,
not an encoded integer. Their native `v0` is the allocated copy, including
after the optional secondary-asset retain and texture initialization.
The `fileJobTypeOperations` child callbacks at `0x003E9214` and `0x003E923C`
feed `fileJobCreateChild`, which stores the result in `FileJobPayload.data`.
That caller already uses the truthful `void *` callback return; no extra
pointer decode, callback cast, or shared-header cutover is necessary.

## Rebase annulment parks onto the current primary context

DDS2 `func_001E72B0` (`0x001E72B0`, 200 bytes) uses the existing
`BtlActorModelBlendArgs` allocation and `BtlUnit` owner. Its historical
49/50-word draft had BEQ instead of retail BEQL at `+0x88`. Splicing the
natural body into the current unit with the canonical status/timer fields,
`mdlGetNodeMotionIndex`, and the actual typed effect provider closes all
50 words and gates `game/code_001DD390` at 287 match, 0 differ.
The obsolete following `u32 *` callback declaration is removed; the creator
uses the same real argument packet. No TU move, visibility attribute,
declaration-order change, or compiler-flag override is involved.


## Frame-reference submission retains its list owner

The paired frame-reference submissions (`00108E60`/`00108D80` and
`00108F00`/`00108E20`) allocate a `0x20`-byte `SdfListHead`, initialize it,
append a separate `SdfDmaReferenceChainPacket`, then pass that same list
to the selected surface. Declare the local as `SdfListHead *` instead of
re-decoding `void *` at the tag append. The frame packet's `u32` conversion
is still required by the physical DMA tag append interface.


## Interface draw packets keep the typed header owner

The nine triangle, quad, sprite and indexed-list writers in DDS1
`code_00196478` and DDS2 `code_0019E138` retain `SdfDrawPacket *` from
allocation through header initialization. Their variable payload cursor
remains a `u64 *` returned by the existing SDK address-word offset helper.
`sdfConsMeasurePacketWithHeader` is the real scalar `+0x20` operation, so
its address-word boundary stays explicit; packet submission separately
converts the completed packet to the physical `u32` append argument.
Do not invent another packet view, alter the SDK helper's scalar contract,
or collapse the genuinely consumed header and payload locals.


The field sprite submission family similarly keeps the actual `void *`
returned by `sdfConsAllocateColumnPacket`. Only the separate
`FldSpriteVertex *` payload is dereferenced; the packet itself is forwarded
opaquely to the SDK offset helper and physical append. Keep those two real
locals and the existing payload owner, without encoding the allocated
packet in `s32` or inventing a second sprite/header view.


## Model viewer lists retain their primary work owners

The paired viewer option callbacks keep the reset-list factory result in
`MdlOptionState.packetList` at `+0x30`, then pass that same list to packet
and surface append routines. The separate `MdlCtrlState.packetList` at
`+0x08` feeds the packed-color label renderer. Both are `SdfListHead *`;
the two work structures remain distinct, with their original sizes
(`0x38` and `0x0C`) and offsets. No integer encoding or alternate work
view is needed at either CPU list boundary.


The depth-selected cell helper `kwlnDrawSpriteCellZ` likewise accepts a
`SdfListHead *` in both games. It forwards that CPU list unchanged to
`sdfAppendPacket`; only the generated rectangle packet is the physical
second argument. The depth, cell-span arithmetic and packet constructor
retain their existing contracts.


## Battle texture preview retains its allocated CPU packet

`btlDrawResourcePreview` keeps the column factory's true `void *` result
beside the existing `KwlnSpriteVertex *` payload cursor. Only the latter
owns the vertex fields. The SDK `+0x20` helper still takes and returns a
scalar address/extent word, and append still consumes a physical `u32`;
those are the two explicit word boundaries, not alternate CPU views.
Local declarations must agree with those actual provider contracts in
both games, even when an older wrong pointer return happened to match.


## Event viewer software lists and claim-held import debt

The menu frame, available row/header callbacks and their reset-list
locals use the existing `SdfListHead *` owner through formatting and
surface append. The header and row callback typedefs stay unprototyped:
their existing call convention genuinely supplies different arities.
`kwlnDrawSpriteCell` now exposes the same CPU list owner in both provider
definitions; its generated packet still occupies append's physical
second argument.

Existing viewer sprite-import declarations/casts and four claimed
functions are deliberately unchanged, not evidence of a word-based CPU
API. Finish the script-test import at `0022AB90` after its claim releases
or 2026-10-10 21:47Z; finish the camera-editor pair `0023CA60`/`00257910`
and the viewer sprite imports after release or 2026-10-11 04:22Z.
The DDS2 row callback `00251ED0` similarly waits until release or
2026-10-12 05:19Z. No new compatibility casts or adapters are introduced.


## Model-blend tasks share one complete argument packet

DDS1 `001DA3A8` and DDS2 `001E7378` allocate the same `0x1C`-byte
`BtlActorModelBlendArgs`. The callbacks' initial `BLTZ` proves the signed
index; unit and target are the real `BtlUnit *` owners. Their completion
result follows the scheduler's `s32` callback contract. Fresh full-unit
checks make both 200-byte callbacks exact without a TU or flag change;
the older one-word annulment parks are not a reason to add call hints.

The blend and stationed-SE factories return the common `0x70`-byte
`BtlRuntimeTask` header. Stationed SE adds only a four-byte argument word,
and its callers use scheduler fields, not a derived sound owner. Preserve
the defeat transition's genuine reused task local with that primary type;
do not cast a blend allocation through the old `SoundTask` view.
Clients needing only the opaque factory retain truthful local declarations
rather than importing unrelated sound-resource layouts and contracts.


## Single-register GS writers retain the packet header

The paired framebuffer flag, TEST, ALPHA, PABE and texture-register writers
retain `SdfDrawPacket *` from allocation through header initialization.
Their existing `u64 *` descriptor cursor owns the actual two-word GS
payload, not another packet view. Preserve the scalar SDK offset helper
and physical append conversions; the register values, branches and store
order are independent of the CPU header's pointer representation.


The paired colored triangle, rectangle-strip and gradient-line writers
use the same primary header lifetime. Their existing named vertex
structures describe only the payload after the SDK's `0x20` header;
keep those real cursors and their packed GS fields. Header typing does
not justify merging vertex formats or changing coordinate packing,
color unpacking, or store order.


Field GS primitive writers also keep two distinct primary allocations:
`SdfListHead *command` for the CPU list, and `SdfDrawPacket *packet` for
the initialized draw header. Their existing `u64 *` payload cursor and
coordinate/color work arrays remain unchanged. Do not apply this header
type to SDK routines that only transport address words through the
scalar finalize interface; that is a different, genuine word contract.


The counted RGBA/XYZ2 strip pair (`002CAAC8`/`00311F20`) also retains
the primary `SdfDrawPacket *` independently of its real `u64 *` vertex
cursor. Header typing leaves count, first-color selection and coordinate
packing unchanged; it is not a reason to rewrite the loop or input APIs.


`evtSubmitGradientRectAtDepth` in both games keeps its command-list and
draw-header allocations under those same two primary pointer types.
Its four real color-channel captures, coordinate array and color-selection
loop remain the original consumed work, independent of list encoding.

## Field model packets use their producer's complete input

`fldSubmitModelPacket` consumes the same `FldMarkerPacket` that its two
marker producers fill. Its position, canonical `BillTextureQuad`, corner
array, packed color and final float cover the real `0x48` input; a second
`FldModelPacketInput` prefix is unnecessary. The separate command-list,
DMA header and opaque geometry allocations retain `SdfListHead *`,
`DmaPacketHeader *` and `u8 *` until their physical-address submission.
The SDK model formatter agrees with the existing billboard callers.

The sound-selector fade/message helpers, indexed field triangle helper
and optional frame vignette likewise retain real command-list pointers.
The vignette's column buffer remains opaque, with explicit address-word
conversion at the scalar SDK header-offset operation.

The released field and event gradient-triangle bodies retain their
existing coordinate workspaces, color captures and packing loops when
the command-list and draw-header locals become their primary pointers.
Owner recovery does not justify changing the published matching loops.

The TEST/ALPHA state writers and panel-state writer are a different case:
their allocation word goes directly through the SDK's scalar finalize
operation and physical append, with a real `u64 *` or state-tail write
cursor returned by that operation. Keep this genuine SDK word transport.

## Menu model helpers share their complete producer owners

The DDS2 `0x50` model-node pool is not the `0x34` image-instance pool.
Its allocator publishes `MnuSectionModelWork`, `MnuNodeList` descriptors
and `MnuModelNode` records from `mnu_shooting.h`. Deactivation, translation
and high-byte broadcast helpers take those actual nodes; the all-node
deactivation walker takes the actual list descriptor rather than a word
array. Preserve the broadcast provider's true `u8` result boundary.

The `0031CBC8` current-owner candidate still differs only at `+0xD8`:
the compiler emits `BC1F` where retail has `BC1FL`, with the same byte-mask
delay instruction. Three genuine result-width/counter-lifetime forms
preserve that residual. Shared-owner recovery does not justify forcing
annulment with a branch, flag, assembly or ordering lever; keep retail
ASM enabled and the reviewed C candidate parked.



## Camp title resources share the complete staff context

DDS2's camp constructor allocates `0xB1E0` bytes. `MenuStaffContext` owns
the retained allocation, request list at `+0x5C`, `StaffSlots` at `+0x60`,
two title frames at `+0xC4`, nine party titles at `+0xCC`, and the context
title at `+0xF0`. The former `CampVisualWork` and `TitleEffectHandles`
prefix views are unnecessary. The background value at `+0x288` is the
existing page window's `transitionValue`, not a second scalar copy.

The backdrop's apparent `+0xE6C`/`+0xEDC` position view is slot `0x17`
of the canonical `BdWork` array: `geometry.bounds[2]` and `sourceWidth`.
Access through the array base preserves retail's immediate displacements;
an interior slot pointer introduced an extra address calculation.

The resource-list builder remains ASM: three honest cursor/index forms
still sibling-call the final append, whereas retail calls and restores
its frame. Owner recovery alone does not justify an ABI or flag change.


## Particle templates embed their actual emitter prefix

DDS2's `EffTemplatePacketList` owns an `EffEmitterHead` at offset zero;
the common runtime prefix is `0x150` bytes and the complete template is
`0x180`. Pass its embedded head to initialization and resource release,
not a competing prefix view. The native scale vector starts at `+0x10`,
restart-step counts occupy `+0xA4/+0xFC`, and the backup matrix starts
at `+0x100`. Lifetime is signed: `0015C3C8 +0x6C4` and
`0015F918 +0x654` use `SLT`, and the spawn routines convert it to float.
`ParKindState +0x0C/+0x10` hold dispatch arguments/drawing-system pointers
for cell kinds but gradient endpoint words for kind four; select the
documented union member by kind rather than reinterpreting the owner.

## Field color vectors and packed words share one record

`EffFieldColorRecord` is the `0x14`-byte primary table element in both
games: four floats followed by the packed color at `+0x10`. The base-color
lookup reads scale lanes one through three, and the entry-word getter
reads that same record's packed word. The old `Entry20B` views started at
the interior word (`D_0034E740`/`D_003AB070`); use the primary records at
`D_0034E730`/`D_003AB060` instead. The lookup and narrow-selector entry
return the record pointer and write through an actual `f32 *`.

The lookup's retail LQC2/VMUL/SQC2 pipeline supports the existing VU0
macros. Its unused stack-vector lane is not an invented initialization.
Three honest DDS1 forms still differ in the default-vector register and
record/FPU scheduling; independent primary-record accesses reduce the
checker residual to 20 of 66 words, first at `+0x5C`. Keep the retail body
enabled rather than adding pointer copies or a one-off inline helper.


## Terminal dispatch uses its complete scene allocation

The terminal constructors allocate `MenuTerminalWork` (`0x164`, DDS1)
and `MenuSlotState` (`0x3F8`, DDS2). Their event dispatch, callback slots,
fade/BGM work and menu lists belong to those owners, not overlapping
event/transition/progress views. The callback pairs are at `+0xC4/+0xC8`
and `+0xCC/+0xD0`, respectively. Other units need only opaque pointers
and the providers' real void-return contracts.

The entry updater candidates remain ASM after three honest forms each:
both differ at four of 49 words, exchanging the mode/list-count loads
and the associated store/delay-slot scheduling. Capturing a native
input snapshot or committing the mirrored state first does not fix it.

## Actor-slot bit-mask signedness remains an open contract

DDS2 `001CCEB8 +0x2E4/+0x2F4` uses `LH` for the bank's mask at
`ActorSlotOrder +0x02`, then `SRAV` and a low-bit test. The independent
`001CF800` accesses the same bank member with `LHU` at `+0xC4/+0x120`
when setting `0x1000` or masking with `0x0FFF`. Those masked operations
do not distinguish the original signedness. There is no independent
sign-sensitive consumer in the remaining native bank-access census;
do not change the canonical `u16` to force the updater's two loads.

The primary-owner candidate uses the evidenced signed timer and word
arrays without byte-padding casts. Its best measurement is 14/522
different words: the two mask loads plus the known twelve-word phase-three
register/scheduling residual beginning at `+0x460`. Three honest forms
were measured; the public owner and retail updater remain unchanged.

## Queued menu draws own a typed six-word context

DDS2 `002BE628` produces six words for `002BF660` and its kind-0/1
renderers: the panel index, a true `MenuPageWindow *`, two origin
coordinates and two extent coordinates. The unit-local
`MenuQueuedRenderContext` owns those fields and asserts size `0x18`;
the producer no longer encodes the page pointer in an integer array.
The independent context/formal migration is byte-exact at 180/0.

The dispatcher remains ASM. Three own structured switch/equality/phase
forms exceed its next-function bound; the native body is 460 bytes.
Its clear guard reads the original first command's marker, not the
moving command, and drawing callbacks precede the live fade-field reads.

## Emitter templates embed one canonical runtime prefix

`EffEmitterHead` (`0x150`) and `EffTemplatePacketList` (`0x180`) now live
in `eff.h`; both games retire their local definitions. DDS1's former
flat template members migrate to the same `head` embedding used by
DDS2. All 161 typed prefix access sites preserve the native offsets,
and both affected units gate 118/0. Initializer call boundaries use
true pointers; existing stored-address return contracts remain separate
debt rather than being changed as a matching lever.

The DDS1 initializer `00153740` remains ASM at 118/120 equal instruction
words after three own forms. Its capacity `LHU` and particle-count `LW`
exchange at `+0x154/+0x15C`. The single signed `drawKind` naturally
supplies the billboard's `LH`; the track's unsigned 16-bit bucket
interpretation is widened explicitly before its 32-bit parameter store.
Existing SDK matrix macros cover the native `vf28`–`vf31` copy.

Kind 4 initializes only `EffTrackPolyParams +0x14..+0x30`. Its manual
history lifecycle dispatches reset, cross-endpoint insertion, coloring
and drawing, not the separate model-sampling updater that reads the
prefix. Preserve the original partial initialization; fabricated prefix
stores would change both the work and the native stack contents.


## Diagnose the first divergent compiler pass, not just the final register

The bounded `near_sweep2` investigation used the configured `cc.sh` wrapper
with `-da -fsched-verbose=5`, keeping source snapshots, objects and dumps
outside the checkout. Diagnostic flags were not added to production builds.
The following cases distinguish three different causes of small residuals.

### A fade-out phase must live across its interval tests

DDS2 `func_001542D8` initially differed only at `+0x13C/+0x140`: the
fade-out subtraction wrote `$f0`, and the next subtraction read `$f0`,
where retail uses `$f1` for both. Pass 19 assigned the old `elapsed`
pseudo `r87` locally to `$f0` (four references, live length three).
The common opacity factor `r88` was a separate global pseudo assigned
to `$f0`; this was not an arbitrary swap of two global allocation scores.

The matching source caches the fade-out phase for both bounds and then
updates that same meaningful value:

```c
} else if ((elapsed = markers[i].progress) > 75.0f &&
           elapsed < 90.5f) {
    elapsed -= 75.0f;
    factor = 15.0f - elapsed;
    /* The existing opacity calculation follows. */
}
```

Now `r87` spans the conditional blocks and participates in global
allocation: twelve references, live length ten, priority `36000`,
allocation order one, selected `$f1`. The factor remains `$f0` at order
zero with priority `73846`. Caching only this interval preserves retail's
other phase reloads. Caching the entire cycle fixes these two arithmetic
homes but removes other retail reloads and is not a match. Branch-local
arithmetic workspaces alone leave the original two-word residual.
The complete live `code_001442D0` unit gates `170 match, 0 differ`.

### An opaque provider can manufacture a branch-likely residual

DDS1 `evtViewCmdResolveSlot` differs at `+0x94` as `BNEL` versus retail
`BNE`. In pass 29, branch UID 89 takes target-thread donor UID 110,
the ordinal increment in `$s1`; the opaque-provider version marks the
branch `/u`, so that donor executes only on the taken edge.

Its successful-selection continuation calls `func_0022E5A0`, which is
still `INCLUDE_ASM` and has no `REG_EH_REGION 0` call note. A private
control compiling the authentic, still nonmatching provider candidate
before the caller establishes that note. All 45 real instruction patterns
in pass 28 remain identical, including the CFG and register assignments.
Pass 29 then retains the same donor but removes `/u`, and the caller
alone gates `1 match, 0 differ` at 212 bytes.

This is the concrete `fill_slots_from_thread`/opposite-thread-liveness
case described above: a possibly throwing opaque call stops the
fall-through scan, whereas a known-nothrow same-TU call lets it continue.
It is a compiler-mechanism proof, not independent TU-boundary evidence.
The provider control still differs at 182 of 271 compared words and is
not publishable; therefore the caller remains ASM pending a genuine
provider closure. Do not publish a stub, add a nothrow annotation, or
change a real callee's ownership merely to select `BNE`.

### Fixed argument copies can lose in sched2 after allocation is correct

Both DDS2 numeric room-mode setters have their only two residual words
at `+0x70/+0x7C`, exchanging `a0 = sp` and `a2 = v0`. These destinations
are fixed ABI argument registers, not competing saved-register allocnos.
In their pass-25 block 5, both copies are ready at clock three with
priority ten. UID 103 (`a0 = sp`) has five printed forward dependents;
UID 107 (`a2 = v0`) has four, and the scheduler selects UID 103.
These observed fields are consistent with the fanout heuristic, but the
printed table does not expose every comparator criterion and is not a
complete comparator replay. The present residual is two of 110 words
in each function; moving the numeric value between ordinary real locals
did not change it.

The paired queued-file providers, DDS1 `002B6778` and DDS2 `002FD900`,
instead exchange a metadata `LHU` and `a0 = v1` at `+0x228/+0x22C`.
In their pass-25 block 14, all competing instructions are ready at
clock nine. The halfword load's printed priority exceeds the argument
copy's: `12` versus `9` in DDS1, `14` versus `11` in DDS2. It is not a
source-order tie or an unidentified saved-register allocation. Publishing
the retained output pointer after its metadata leaves the same two-word
residual. Caching one transfer length for both metadata and copying
extends its real lifetime across a call and substantially worsens the
match; no register-pinning or extra copies are justified.

DDS2 `002CBA90` is different again. Its loop index's literal-zero
initializer is already present in pass 0 and survives CSE, regmove and
reload; pass 20 correctly assigns it to `$s0`, while the found flag is
`$s1`. Retail's `s0 = s1` at `+0x54` is not explained by swapping those
allocation homes. Initializing the real counter at function entry
changes its lifetime but worsens the residual from one to sixteen of
78 words. The original initializer/value-availability shape remains
unproved; do not invent `i = found` solely to copy a known zero.


## Surface ring buffers have one typed owner

`EffSurfaceGridNode` owns the ring counters and vertex/color regions in both
games (DDS1 `code_0029C530`, DDS2 `code_002DE248`). The partial
`EffRingFrameState` view is retired. Signed comparisons and wrap calculations
in DDS1 `002A78E0` and its sampler establish the signed counters; the DDS2
counter group has the corresponding 0x10-byte prefix shift.

The producer allocates consecutive main vertices, eight cap vertices, eight
cap colors, and main colors. The canonical fields therefore use `u128 *` for
vertex regions and `u32 *` for RGBA regions, with conversions only at the
byte-allocation cursor. Samplers and the color-gradient writer read the
primary owner directly. The 0x3C/0x4C allocations and existing layout stay
unchanged. Both owner-only whole-unit gates are exact: DDS1 533/0, DDS2 677/0.

The DDS1 renderer remains ASM. Its best defined candidate differs in 30 of
147 aligned words. One explicit difference is the empty second span's start:
retail omits its initialization on the non-wrapping path but still loads the
slot while preparing a zero-length span. The retained C initializes that
start and emits an additional store, so it is not a matching landing. Do not
remove the initialization merely to reproduce an undefined local read.

## Actor triangle bands snapshot unsigned alpha values

DDS2 `func_001C5610` reads the canonical actor-panel work, rather than a
second row projection. Position comes from `activeEntries[index].position`
after GS setup; all eight `presentation.triangleAlpha` bytes are cached
before the first triangle callback. The two four-corner bands share their
literal triangle coordinates, then unconditionally restore alpha mode zero.

The cached alpha values are `u32`: packing them with `alpha << 24` is unsigned
RGBA arithmetic, not a signed overflowing shift. This real type correction,
the current position-array owner, and the provider's actual nine `s32`
arguments produce exact 600-byte retail text. Whole-unit private and live
checks are DDS2 228 match, 0 differ; the identical honest body ports to DDS1
`func_001BA408` in `code_001A9780`, which gates 189 match, 0 differ.
The retained R1 owner draft and Opal/Obsidian
reconstruction/review supplied the baseline; no ABI or scheduling controls
were added.

## Viewer timeline filters share a forward frame boundary

DDS1 `func_0022E5A0` walks the canonical runtime's tracks and keys. The default
interval is the key's `duration` at +2, not a parameter halfword. The second
pass skips condition-disabled keys, stopping at the first enabled future
frame; the predecessor is then searched backward for an enabled selection.
Kind 2 reads condition word 1; kinds 12/16/17 read word 3, using the signed
low half of the existing parameter-word union.

Use the natural `for` cursor increment for the forward pass, with `continue`
in the two condition-filter branches and one shared frame comparison. The
nested other-kind guard preserves retail's two forward predicate call sites
without duplicating the frame comparison. This also reproduces the native
frame lifetime; no variable ordering or allocation controls are needed.

The 1144-byte provider and its 212-byte `evtViewCmdResolveSlot` consumer land
together: the unit gates 59 match, 0 differ. The truthful `EvtRuntime *`
declaration closure gates the event-viewer unit 31/0 and `code_00235270`
128/0. Credit Purist6c's corrected reconstruction and RTL lifetime analysis.

DDS2 `func_00249088` additionally calls `func_00247DE0` for each active
interval, after `func_00247858`; this real second call keeps the interval
length live. Porting the same cursor loop with that native extra work matches
the 1168-byte provider and the 212-byte resolver on the first form: 63 match,
0 differ. Consolidating the two provider declarations in `code_00250010`
into one truthful `EvtRuntime *` declaration gates 128 match, 0 differ.
Both games' nearest-key helpers read and write only word zero of the incoming
distance pointer, so `bestDistance` is a scalar, not a second parameter view.


## Movie parameter blends use the renderer's primary records

Both polygon-movie units now use `EffBlurQuad`, `EffBlurTemplateBody`,
`EffResourceRectParams`, `EffBlurScatterParams`, `EffBlurScaleParams` and
`EffSolidRectParams` directly; the local `EvtBlendA..G` projections are gone.
The native two-coordinate loops establish `position[2]` / `center[2]`, and
the nested two-by-two loops establish `corners[2][2]`. Named renderer accesses
migrate to those same arrays, with sizes and offsets unchanged.

`evtBlendParamsD` writes only the resource rectangle's 0x14-byte prefix,
leaving its existing bounds untouched. Its packed RGBA word and the
renderer's four channel reads justify the documented color union in
`EffResourceRectDrawParams`. Other packed colors remain plain `u32`.
The blender units are exact at 32/0 each; the private owner proof also gates
all 147 existing header consumers with no changed retail words.

## Kernel parameter snapshots retain their legacy views

The local `Kwln*Params` records in both `dds3KernelDraw.c` units remain
recorded owner debt, not newly introduced views. Replacing them with the
canonical Eff records privately regresses six already-matched functions
per game. Five fade-setup helpers move the counter `SH` before the alpha
`LBU` (four words each); the per-frame updater overruns its native boundary.
The legacy `KwlnScaleBlurParams` also has `aligned(8)` and size 0x30,
whereas `EffBlurScaleParams` is 0x2C. Do not force the primary record's
size/alignment or change statement order to conceal those snapshot/alias
differences. The kernel sources remain unchanged while the independent
movie-blender owner fold lands.

## Explicit fixed-step and ramped-step assignments preserve local allocation

DDS1 `game/code_002665E0::func_002687C0` advances either the level or
profile reward row. A positive pending amount selects a fixed increment of
10000 when `skipRamp` is set; otherwise it increments/clamps the frame
counter and computes the ramped increment. Keep these two assignments in
their actual `if/else` arms, followed by the shared fast-completion override
and pending subtraction/clamp.

Preinitializing `step = 10000` before the ramp test is behaviorally equivalent,
but leaves six differing instruction words: two mirrored address/byte
register swaps. Before local allocation, its constant assignment falls
between the row-address addition and signed `skipRamp` load. The address
quantity has two references and a live interval of four allocation positions
(priority 5000); the byte quantity has two references over two positions
(priority 10000). The byte therefore takes `$2` and the address takes `$3`.

With the explicit fixed-step `else` assignment, both intervals have length
two and priority 10000. Local allocation selects the address first: `$2` for
the address and `$3` for the byte, matching both native paths. Native compiler
entry/return observation reproduces the uninstrumented assembly exactly;
the complete 756-byte body and all five C functions in its owner match.

This is a control-flow reconstruction backed by the actual fixed/ramped
calculation, not a reason to reorder statements or extend unrelated values.
Inspect the pre-allocation stream and real quantity priorities before applying
it elsewhere. Preserve Opal's full reconstruction and Azure's owner/compiler
admission credit.

## Viewer contracts preserve the native word inputs

The nearest-key dispatchers receive a full `s32` offset: retail uses `$10`
directly in its frame addition, without a callee halfword truncation. Their
existing caller carries a signed halfword, so correcting the two declarations
does not change the already-matched caller bytes.

DDS2 `evtViewerHasUpdateFlag` takes the canonical `EvtRuntime *`; all its C
callers now pass that owner directly. The query snapshots the native loaded
flag word as `s32`, then tests bit 0x10. This retains retail's `ANDI`/`SLTU`;
testing the unsigned owner field directly instead emits `SRA`/`ANDI`. The
16-byte query and both complete consumer units remain exact (63/0, 31/0).


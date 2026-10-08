# Confirmed source idioms (ee-gcc 2.96, -O2 -G8)

Each idiom was checked by compiling it and comparing with retail. When retail
shows one of these shapes, write the C given here. None of them is a trick:
all are ordinary source that the original compiler turns into exactly the
retail code.

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


## Calls with fewer arguments than the callee reads

When retail calls a function with fewer registers set than its body reads
(a sibling `j` with `$5` never written), the original source had no
prototype in scope. Declare it unprototyped (`void f();`) and write the
definition K&R; a full prototype turns the call into an error
(`fileSetRecordSecondVector`, DDS2).

An old-style declaration can also preserve an argument's width when the
argument count is correct. With `void f();`, an `L`-suffixed integer argument
keeps its `long` type and reaches pass 00 as `DImode`; a fixed `f(s32, s32)`
prototype converts it to `SImode`. For the GS TEST packet calls this is the
difference between retail's `dli $a0,0x5100d` with the other argument in the
call delay slot and `li $a0,0x50000` followed by `ori $a0,$a0,0x100d` in that
slot (`evtPrepareSolarOverlayTestState` in both games, `func_0026C350`). Use
this only when the callee really has a K&R definition and retail supports the
wide literal; if the argument mode is unchanged in `.00.rtl`, the call
contract is not the cause.

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
A fully-C unit has no `INCLUDE_RODATA` step: a string a function names through
`extern char D_X[]` is lost from the link; write the literal in the C.

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
`game/code_001C7FF8`). The normal twin-port candidate has just
`beql` versus retail `beq` at `+0x1B8` (147/148 words; `51 match, 1 differ`).
Putting the actual existing `btlDestroyTaskD` body before that caller in a
private diagnostic TU yields `53 match, 0 differ`, including the provider
and all surrounding C. After normalizing block addresses and label
ordinals, `28.mach` differs only by `REG_EH_REGION 0` on call UID 443.
Both `29.dbr` files donate the same global-panel load, UID 449, into jump
UID 435: taken-only for the opaque provider, always for the visible one.
The final assembly differs only in that branch mnemonic. This is a causal
control, **not a landed match**: the original DDS2 TU boundary has not been
authenticated. The Nocturne prototype catalog has no matching row, and
the US catalog's weak call-graph correspondence is not TU evidence.

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

`BrsSkillPackageWork` embeds `StaffSlots` at `+0x4F8` in DDS1 and `+0x51C`
in DDS2. Pass its address to staff-bank APIs instead of treating adjacent
scalar fields as an array. The bank retains genuine `u32` SDK handles:
DDS2 `effLoadIndexedResource` returns that type, and
`mnuInitializeCampPanelResources` decodes its first resolved handle only
at the `EffectSlotSet *` constructor boundary.

Keep page-selection clearing on the real `MenuPageWindow.slots` array.
Direct indexing of the selected row's HP/MP animation states gives the
native DDS2 helper without a second `MenuWindowSet`/`MenuSlotWindow` view.
The final shared-header closure is 58 actual CPP consumers, all clean in
serial `check_unit` runs (2200 matching functions, zero differences).


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

DDS2 `itfGridApplySqrtBoundsAndColorScale` (`0x003075D8`, 312 bytes) and
`func_00307EF8` (`0x00307EF8`, 296 bytes) match using this grouped owner and
the mapped record's real `status` pointer. `func_00307A68` (`0x00307A68`,
320 bytes) ports the same owner's threshold-alpha easing from the matched
DDS1 `func_002C0038`, including its destination/source palette cursors.
The complete DDS2 grid unit now checks `68 match, 0 differ`.

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
`BtlLinkedCommand.panelScale` at command `+0x138` in DDS1 and `+0x15C` in
DDS2. Completing this tail consumes eight bytes of the enclosing state's
opaque gap; every later state offset is unchanged. These are the only
by-value command embeddings, and neither command has a `sizeof` consumer.

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
and `+0xAF10` in DDS2. Their state and opacity fields are at row
`+0x14/+0x1C` and `+0x10/+0x18`, respectively. DDS2's profile-icon
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

DDS2 `btlUpdateActionPoseForLinkedTarget` deliberately leaves the target
unit in `$a2`: native `001EBD6C` loads it there before the tail call at
`001EBD88`. The fallback `001ECBF8` only consumes `$a0/$a1`, forwarding
those two pointers to the camera core with `27.5f`. Keep an unprototyped
declaration ahead of that genuine three-argument legacy caller and the
typed two-parameter definition afterwards; do not invent a third formal.

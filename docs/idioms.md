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
- Declaring an INCLUDE_ASM callee `s32` instead of `void` moves values loaded
  after the call from `$2` to `$3`. Try this on near-misses that only differ
  in `v0`/`v1`.
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
- A call to a function already compiled earlier in the same file differs from
  a call to an extern. Without `-fexceptions`, `rest_of_compilation` marks
  every compiled function `TREE_NOTHROW`, and later calls to it get a
  `REG_EH_REGION 0` note. In reorg (`find_dead_or_set_registers`), the scan of
  a branch target's live registers stops at a call that can throw (an extern)
  but runs past a nothrow call. That changes which delay slots get filled:
  `bne` against `bnel`, or a lone `nop`. So when a caller matches only with its
  callee opaque, retail compiled the callee in a *different* file: a
  translation-unit boundary lies between the callee and the caller. When a
  caller matches only with the callee visible, both were in one file. Record
  such cases as boundary evidence and split the unit; a block-scope `extern`
  in the caller reproduces the bytes, but it is a codegen lever, not the
  source. The definition form of the callee (`static`, return type,
  prototype) makes no difference.
  Mechanism (gdb on cc1, `fill_slots_from_thread`): when the delay slot comes
  from the branch target, the insn stays unannulled only if it sets nothing
  in `opposite_needed`, the registers `mark_target_live_regs` reports live at
  the fall-through. That set is a forward simulation from the start of the
  extended block (REG_DEAD regs only die at the next label) and
  `find_dead_or_set_registers` then stops at the first throwing call. So a
  fall-through that starts with an extern call keeps the slot register
  "live" and the branch becomes `beqzl`/`bnel`; with the callee compiled
  earlier in the unit the scan runs on, sees the call clobber the register and
  the branch stays plain. Toy-verified on `func_00190708` (callee
  `func_00190118`, defined in the unit before `effEvent`), DDS2
  `func_00224500` (callee `func_00217470`) and `func_00183EE0` (callee
  `effPcpEventWorkInitEntries`, same unit: the branch matches once the C
  function is visible, no unit change needed). For `func_00190708` and
  `func_00224500` the callee sits in another unit, i.e. those units are one
  translation unit in retail.
  Quick test without touching the repo: copy the unit to a scratch file, put
  a K&R stub (`void callee() { }`, or `s32 callee() { return 0; }` when the
  result is used) before the caller, and run
  `check_unit.py <unit> --source <copy>`. If the caller then reports OK the
  callee must be compiled earlier in the same TU (merge the units or match
  the callee first); the reverse test (replace an earlier C callee by an
  `extern` prototype) finds the opposite evidence, a unit that has to be
  split (`func_00280978`: `mnuClearListFlagsOneAndTwo`/`mnuTestListFlagTwo`
  must be opaque). A ternary `t = c ? a : b;` also adds a jump to the join
  label, which stops cse's skip-block path (see "Float constants").
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

- the caller or callee is varargs, or the callee returns a struct;
- the caller returns the callee's value and their return modes differ: an
  `s64`/`u64` function returning an `s32` call, or a narrow (`s8`/`u8`/`s16`/
  `u16`) callee returned from a wider caller (`return (u8)f();` too).
  `void`→`void`, `s32`→`s32` and `u32`→`u32` always sibcall. A shared wrapper
  that returns `u64`, written as `return menuRunPanel(...)` from an `s64`
  function, is why whole menu units have no `j` tails (no file flag needed);
  use the callee's real return type from its matched definition, never a
  made-up one;
- the address of a local or parameter is taken, or arguments go on the stack;
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

Plain `void w(void) { f(0); }` always sibcalls, whatever f returns. Don't try
to "fix" a jal tail in a normal file with dummy code; park it
(`build/parked/`) for `tools/flag_probe.py`.

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
| `VU0_NEGATE_XYZ(vf)` | `vsub.xyz vf,vf0,vf`: negate xyz, w kept (reversed direction after a matrix apply; 95 sites) |
| `VU0_CLEAR_W(vf)` / `VU0_SET_W_ONE(vf)` | `vmulx.w vf,vf,vf0x` (w = 0) / `vmove.w vf,vf0` (w = 1): the w fix-up before a colour pack or point store |
| `VU0_SET_AXIS_CLEAR_W(f, axis)` | `mfc1 $2,f; qmtc2.ni $2,vf2; vaddx.axis vf10,vf0,vf2x; vmulx.w vf10,vf10,vf0x` as one block: vf10.axis = f, w = 0 (event unit path/aim vectors; separate `VU0_SCALAR_OP` + `VU0_CLEAR_W` moves a neighbouring `daddu`) |
| `VU0_LENGTH_VF10(out)` | `vmul.xyz vf2,vf10,vf10; vaddy.x; vaddz.x; vsqrt Q,vf2x; vwaitq; cfc2.ni $2,vi22; mtc1 $2,out`: \|vf10.xyz\| as a C float (`$2` clobbered) |
| `VU0_NORMALIZE_VF10()` | `vmul.xyz vf2,vf10,vf10; vmulax.w ACC,vf0,vf2x; vmadday.w; vmaddz.w vf2; vrsqrt Q,vf0w,vf2w; vwaitq; vmulq.xyz vf10,vf10,Q` |
| `VU0_CROSS_XYZ(dst, a, b)` / `VU0_DOT_XYZ(out, a, b)` | `vopmula.xyz ACC,a,b; vopmsub.xyz dst,b,a` / `vmul.xyz vf2,a,b; vaddy.x; vaddz.x; qmfc2.ni $2,vf2; mtc1 $2,out` (`$2` clobbered) |

Soft-float libcalls: retail leaves a `nop` in the delay slot of some `jal` calls to the double compare helper (`func_002FC6C8`) where our cc1 fills it with the argument move `daddu $4,$2,$0` (`func_00292CE0`). Cause unknown (not a source shape so far); those functions stay asm.

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

## Calls with fewer arguments than the callee reads

When retail calls a function with fewer registers set than its body reads
(a sibling `j` with `$5` never written), the original source had no
prototype in scope. Declare it unprototyped (`void f();`) and write the
definition K&R; a full prototype turns the call into an error
(`fileSetRecordSecondVector`, DDS2).

## Assembler version

The build uses the ee-as shipped with ee-gcc 2.96. The older
`ee-gcc2.9-991111` as keeps a `nop` in a `jal` delay slot after a
large-offset macro, which is what a few retail functions show. It is still not
the retail assembler: building everything with it changes both ELFs in
thousands of places, and the text size too. Treat those functions as
unmatched. Don't switch assemblers per file.

Retail's assembler also never fills a branch delay slot with the instruction
that reads the FPR the preceding `mtc1` wrote: `mtc1 $4,$f1; cvt.s.w $f1,$f1;
b; nop`, never `b; cvt.s.w`. The 2.96 as swaps it, and no as option or other
ee assembler reproduces the retail rule without changing other code, so
`tools/as_coproc_delay.py` applies it to cc1 output before `as` (in the build
and in `tools/cc.sh`). It changes nothing that already matched.

The same holds after `cvt.w.s`/`trunc.w.s` (no `cvt.w.s $fN; jr; swc1 $fN` in
either retail ELF), and retail never moves the closing `addu` of an indexed
`la $rd,sym($rs)` into a following `jr`'s slot; the pre-pass handles both.

## Loops, tail calls and register priority (gcc 2.95 internals)

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

### Saved-register order: global-alloc priority

global.c `allocno_compare`: priority = `floor_log2(n_refs) * n_refs / live_length`.
Higher priority allocates first and takes the lowest free callee-saved register;
ties go to the lower pseudo number (parameters in declaration order).
`cc.sh -dl` prints `used N times across L insns` per pseudo, so the order can be
computed. Natural source shapes that flip it:

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

### Alias sets stop gcse merging a reload

gcse refuses to merge two MEMs with different alias sets, so a typed field access
and a differently typed access to the same location stay two loads; two typed or
two raw accesses are merged even across a loop. A retail reload after a loop
therefore means the two accesses had different types. Acceptable only when the
codebase already has a second struct view of the object (DDS1 `func_00277CB8`:
`MenuSelectionState->list` for the walk, `((MenuInputNode *)state)->flags` for
the seek argument). A raw cast next to a typed access of the same field is a
lever (DDS2 `func_002B40F8` stays INCLUDE_ASM).

### Float constants never in a delay slot

Neither ELF has `mtc1 $1,$fN` or `mtc1 $0,$fN` in a branch slot, so
`tools/as_coproc_delay.py` keeps every `li.s` out of the following slot
(retail `li.s $f12,K; jal f; nop`, DDS2 `func_001ECC18`).

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

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

## `slt; sltiu 1` vs `slt; xori 1`

A negated comparison written as an expression (`return !(x < 2);`,
`return x >= 2;`, `x < 2 ? 0 : 1`) gives `slti; xori $2,$2,1`. Retail's
`slti; sltiu $2,$2,1` comes from early returns:

```c
if (x < 2) return 0;
return 1;
```

(`if/else` with two returns gives the same code.) Example: `func_0014D0D0`.

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
- Loops over parallel per-slot arrays (`set->handle[i]`, `set->active[i]`)
  keep retail's count-up loop. A pointer walk becomes a count-down loop.
- To keep two `slti` where C would fold a range test into `sltiu`, nest the
  tests: `if (m < 0xE) { if (m >= 0xC) ... }`.
- Adjacent independent stores come out roughly reversed. Retail's order
  usually comes from writing the fields in ascending offset order.

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
the aligned/unaligned dual loop. `memcpy` with a literal size gives
`lwl/lwr` on the tail word, so it only matches when the size is a multiple
of 8. Try the struct assignment first when the tail differs.

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
- the return value is converted (`return (u8)f();`, s64 callee in an s32 function);
- the address of a local or parameter is taken, or arguments go on the stack;
- the whole file was built with `-fno-optimize-sibling-calls` (see
  `config/dds1/cflags.txt`; `tools/find_nosibcall.py` finds such files).

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

That gives retail's `lqc2; jr $31; nop` (`func_00217F70`). Only COP2 (and
MMI, where a VU function needs it) goes inside the asm; everything around it
is C. Vector copies retail does with lqc2/sqc2 are written the same way (see
`EFF_COPY64` in `effect/effMagatuhi.c`). Values passed in vf registers across
calls: `void f(void)` using the registers directly
(`game/code_002E7C20.c`).

## Float constants and strings

- Float constants are literals. ee-as puts each `li.s` constant into the
  unit's `.lit4` pool itself. Derived constants fold at compile time:
  `180.0f / 3.14f` is retail's 57.32484.
- cc1's decimal-to-float conversion can be one ULP off for long literals
  (`6.283185005f` gives 0x40C90FD9, retail has 0x40C90FDA). The retail bits
  come from the expression the programmer likely wrote:
  `3.14159265f * 2.0f`, `-3.14159265f / 2.0f`.
- Float arguments to an unprototyped callee are promoted to double and go
  through soft-float helper calls (extra `jal`s). Give the callee a
  prototype with `f32` parameters.
- Strings of 8 bytes or more (counting the NUL) are literals in `.rodata`.
  Shorter ones live in `.sdata`.
- A literal whose retail copy an asm function of the unit still uses must
  stay `extern char D_X[]; /* "text" */` until that function is C
  (check_unit reports `SHARED`).

## 128-bit data

`int __attribute__((mode(TI)))` (u128) copies give `lq`/`sq`. gcc fills the
return delay slot with the last `sq`. If retail has `sq; jr; nop` instead,
the copy was not plain C: it was either an lqc2/sqc2 VU copy (see above) or
hand-written.

## Seven or more arguments

The EE ABI passes arguments 5 to 8 in `$8`–`$11` (not on the stack), so a
7-argument call just loads `$8`–`$10`. Write the full prototype.

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

## Not allowed

These are fakes, and check_unit reports them as `TRICK`:

- computed gotos and label-address tables standing in for a switch;
- `register x asm("$n")`;
- asm used for anything but COP2 VU0 code;
- dummy variables or `volatile` added only to steer codegen.

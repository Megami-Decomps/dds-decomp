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

## Not allowed

These are fakes, and check_unit reports them as `TRICK`:

- computed gotos and label-address tables standing in for a switch;
- `register x asm("$n")`;
- asm used for anything but COP2 VU0 code;
- dummy variables or `volatile` added only to steer codegen.

# Ratio-panel contents: texture-owner allocation

DDS2 `mnuDrawRatioPanelContents`, in `src/dds2/game/code_002BE628.c`,
occupies 532 native bytes at `0x002C22D0`. The reconstruction preserves both
color blends, the signed value/total tests, texture slots 5 and 6, and the
format/create/draw/destroy sequence for each displayed number.

## Ordinary source and compiler cause

Ordinary C emits the complete 532-byte extent but leaves 22 of 133 instruction
words different. Y occupies `s2`, while the texture owner and subsequent glyph
requests reuse `s1`; retail has the opposite allocation. Separate or scoped
glyph locals and plain Y snapshots did not remove this residual.

The final source adds one initialized local binding immediately before its
real texture loads, inside the `value >= 0` block and after both blend calls:

```c
register MenuPageBar *style __asm__("$18") = pair;
```

The qualified `19.lreg`/`20.greg` records establish the actual dependency:

- Pseudo 85 is Y, pseudo 91 is the incoming `MenuPageBar *`, and pseudo 94
  holds each real glyph result. Definitions and consumers, not register
  numbers alone, establish these identities.
- In both sources, glyph pseudo 94 is the first global allocation candidate:
  9 references over 18 live instructions, priority 15000. This is **not** a
  Y/owner priority swap. Local allocation assigns only pseudos 98, 114, 115
  and 133, identically in both sources.
- With the binding, pass 19 retains UID 90, the copy from pseudo 91 to hard
  register 18, with `REG_DEAD` on pseudo 91. The actual texture loads use
  hard register 18 and mark it dead at the final slot load on each path.
  The incoming pointer's statistics become 2 references/13 live instructions,
  compared with 4/25 in the ordinary control.
- Pass 20 records preference 18 for pseudo 91 and hard-register-18 conflicts
  for Y and the other values overlapping the texture-owner phase. Glyph 94
  has no such conflict because its definitions follow the last texture load.
- The pinned compiler's `global_init` seeds `regs_used_so_far` from
  `regs_ever_live`, call-used registers and local allocations
  (`0x08140540`–`0x081405E9`). `find_reg` first restricts its search to that
  already-used set (`0x081427BA`–`0x081428F9`). Static inspection of the same
  compiler binary corroborates this mechanism; it is not a hooked execution
  log. The real hard-register-18 interval makes `s2` available for glyph 94's
  disjoint lifetime without introducing another saved register. The ordinary
  control has no such interval and gives the first glyph allocation `s1`.
- Final dispositions are Y=17, owner=18, glyph=18 with the binding, versus
  Y=18, owner=17, glyph=17 without it. The earlier pointer copy coalesces;
  there is no additional runtime move or operation in the exact result.

This documents an allocation reconstruction, not original source spelling or
a hardware requirement. There is no empty asm template, glyph binding, fake
consumer, or forced relationship between differently typed source objects.

## Independent semantic and ABI review

`style` is initialized from the actual pointer argument on every reaching
path. `MenuPageBar::textures` begins at `+0x1C`; pointer elements 5 and 6
therefore load the native `+0x30` and `+0x34` fields. The binding changes no
object access, qualification, arithmetic, call order, or memory effect.
X, Y, depth, value and total retain their signed types. The text buffer remains
16 bytes, sufficient for every nonnegative 32-bit decimal value and terminator.

The bound pointer crosses only the first texture call on the two-mark path.
It is dead after the second slot load, before the second texture call; on the
one-mark path it is dead before the sole texture call. The actual native
`effDrawTextureSlotWithBlendedCornerColors` saves `s2` at `0x00306CFC` and
restores it at `0x00306D9C`, both at stack offset `+0x40`. Its use of `s2` as
a loop counter consequently does not clobber the caller's live pointer.
The earlier `fclBlendColor` calls precede the source binding; their native
leaf body also does not touch `s2`. No artificial lifetime extends into the
font phase, where ordinary allocation reuses the register for `_FRQ *` values.

Independent review approves this specific binding under the localized
real-value constraint rules. The approval does not extend to the parked
glyph constraints or any broader binding.

## Literal ownership and complete-owner qualification

The C `"%3d"` literal replaces the old exclusive `D_00437C78` data include.
The existing C definition of `D_00437C70[2]` remains after its earlier consumer
but moves before the ratio function. The generated placements put the retained
`D_00437C60` and `D_00437C68` includes before that definition. The production
owner's `.sdata` order is consequently native: offsets 8, 16 and 24 for those
three objects, the C literal at offset 32 (`0x00437C78`), then `D_00437C80`
at offset 40. The entire section is 104 bytes. There is no duplicate string
include or retained external-string exception.

Moving the table definition before its own consumer was rejected: it exposed
the initializer to constant folding and changed another function. Leaving the
definition at the owner's end was also rejected because the new literal
preceded the table and broke `.sdata` order. The final placement preserves
both complete-owner code and native data layout.

Compiler SHA-256:
`d11ca9e2086edf122df8580c00fd9024f036d0b6c9d782fe986ad1d881d0c8f1`.
The final candidate and the ordinary-local removal control were each compiled
normally and with `-da` through the canonical owner wrapper. Each pair agrees
in normalized **whole-owner** assembly; all six allocated sections' bytes and
semantic headers; all 963 symbols and 924 resolved relocation contracts;
non-debug auxiliary sections, including `.pdr`; and ELF ABI metadata.
Normalization changes only the wrapper's random canonical-length directory
names. Section file positions and string-table name offsets are excluded,
while resolved section/symbol names, extents, flags, alignment, targets and
relocation types are compared.

The complete candidate owner, including owned data, passes 191 matches and
zero differences. Removing only the register declaration constraint leaves
190 matches and one difference: the same 532-byte ratio function with 22
differing words. This removal test is separate from ordinary/probed parity
and from the final retail link proof. No private native files or compiler
dumps belong in the source change.

## Full validation

Both complete retail builds pass their exact SHA-1 gates:

- DDS1: `6d18898e2724bf1d145392766e8ba1e678487419`.
- DDS2: `9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`.

The repository suite passes all 442 tests in 117 batches.

All 81 remaining retained-C units compile, and `git diff --check` passes.

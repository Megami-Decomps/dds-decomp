# Contributing

Anyone can contribute. This guide covers the whole loop for one function: pick
it, decompile it, verify it, name it, and share it with the other game.

## Matching rules

A function counts as decompiled only when it compiles to exactly the retail
bytes, and it has to reach that the way the original developers would have.

- **No fakematches.** Inline asm is allowed only for COP2/VU0 macro-mode
  blocks (wrapped in `.set noreorder`), the `fsqrtf` helper in
  `include/fpu.h` and the `PCP_COPY_VECTOR` quadword copy in
  `include/pcp_vu0.h` (see docs/idioms.md). Not allowed:
  - register pinning (`register x asm("$n")`)
  - computed gotos or label tables standing in for a `switch`
  - dummy variables or `volatile` added to steer codegen
  - permuter output that doesn't read like real code
- **Plausible source.** Use structs for recurring layouts, real types
  (no `u64` unless the value is 64-bit), meaningful parameter and local
  names, and natural loops and switches.
- **Compiler flags need evidence.** Per-file cc1 options live in
  `config/<v>/cflags.txt`. An option goes there only when a contiguous run of
  functions in one file needs it (so far only `-fno-optimize-sibling-calls`,
  found by `tools/find_nosibcall.py` and confirmed with `tools/flag_probe.py`).
  One function that matches under some option is not evidence.
- **Close is not matched.** A near miss stays `INCLUDE_ASM`. Save its best C
  somewhere outside the build with a note on the remaining difference.

`tools/check_unit.py` enforces most of this. `docs/idioms.md` lists the source
shapes that are confirmed to produce specific retail code sequences. Read it
before you start.

## 1. Pick a function

`python tools/progress.py` shows per-game coverage. Every retail function
belongs to a C unit in `src/<v>/<dir>/<unit>.c`. A function that isn't done
yet is an `INCLUDE_ASM(const s32, "<dir>/<unit>", NAME);` line, and its
assembly is in `asm/<v>/nonmatchings/<dir>/<unit>/NAME.s`.

Units are named after the original source files where the Nocturne debug
build proves the name (`kernel/dds3KernelCore`, `effect/effPCPMisc`, ...,
see `docs/tu-names.md`). Everything else is `game/code_<vram>`, split at
proven file boundaries. The Sony SDK libraries (`sdk/lib*`) are prebuilt
archives and stay assembly.

For DDS2, work on functions listed in `build/dds2/dds2_only.txt` first
(`tools/shared_funcs.py` writes it). Functions identical to DDS1 arrive
automatically when their DDS1 twin is decompiled (step 5).

## 2. Decompile

```sh
python tools/decompile.py func_XXXXXXXX [-v dds2]   # m2c draft with the unit's context
```

Replace the `INCLUDE_ASM` line with your C. Keep functions in retail order,
and leave `INCLUDE_RODATA` and `INCLUDE_SDATA` lines where they are:
`configure.py` recomputes their placement.

- Float constants are literals. ee-as builds the unit's `.lit4` pool.
- String literals are compiled into the unit's own `.rodata`/`.sdata`. Keep
  `extern char D_X[]; /* "text" */` only when check_unit says `SHARED`, `PAD`
  or `MERGED`.
- `switch` statements compile their own jump tables, and every entry is
  checked.

For a first pass over a whole unit, `python tools/rw_bulk.py <v> <dir/unit>`
tries a romwright draft for every `INCLUDE_ASM` function and keeps only the
ones that match with the unit still clean. romwright is optional; set
`ROMWRIGHT=/path/to/romwright-cli`. Kept drafts still read like decompiler
output, so type and name them afterwards.

## 3. Verify

```sh
python tools/check_unit.py src/<v>/<dir>/<unit>.c [-v] [--func NAME]
```

The unit is clean when this ends in `N match, 0 differ` with none of these
lines:

| Line | Meaning |
|---|---|
| `DIFF` | instruction words differ (`-v` lists them) |
| `OVER` | the function runs into the next retail function |
| `CONTEXT` | the function compiles differently inside the full unit, as the build compiles it (see idioms.md) |
| `SHARED` / `PAD` / `MERGED` | a literal can't be compiled from C yet; keep the `extern` |
| `DATA` | the unit emits data nothing accounts for |
| `MISSING` / `ORDER` / `TWICE` | a function was dropped, moved, or defined twice |
| `STALE` | the unit uses an old `func_` name that symbol_addrs has since renamed; use the new name |
| `TRICK` | computed goto, label table, or pinned register |

Always check the whole unit after an edit. A changed declaration, or even a
new name, can change how other functions compile. Then build:

```sh
ninja            # both games; fails unless every ELF is byte-identical
```

Other diff tools:

- objdiff (`objdiff.json` is generated; bases are built with `-DSKIP_ASM`)
- asm-differ (`diff_settings.py`; `DDS_VERSION=dds2` for the sequel)
- decomp.me (compiler `ee-gcc2.96`, flags `-O2`; `tools/m2ctx.py` writes the context)

## 4. Name

```sh
python tools/names.py apply <v> names.tsv
```

Each row is `<old name|0xADDR> <new> <evidence|inferred> <note>`. The tool
writes a curated row in `config/<v>/symbol_addrs.txt`, records provenance in
`config/<v>/name_sources.txt`, and renames references in C. Run
`configure.py --force-split` afterwards so the assembly follows, and re-check
the units you renamed in (`CONTEXT`).

- Names follow the Atlus convention: a lowercase module prefix plus CamelCase
  (`sdfAddHandler`, `btlResetRuntime`), with no numbers or clone letters.
- `evidence` is only for a name the binary carries for that function itself,
  e.g. in its own debug message. Everything else, including names from the
  Persona 3/4 decomps and task labels, is `inferred`.
- `python tools/names.py harvest <v>` lists the naming strings and the
  functions that reference them.

## 5. Share with the other game

About 8,600 DDS1 functions are byte-identical in DDS2 once relocations are
masked.

```sh
python tools/shared_funcs.py names         # curated names -> the other game
python tools/shared_funcs.py port          # decompiled C -> the other game (--from dds2 for the reverse)
python tools/shared_funcs.py clones <v>    # C for identical functions within one game
```

A port translates every symbol and string literal through the pair's
relocations (falling back to the full identical-pair map for callees and
globals the function's own relocations do not name), and leaves the operands of
`__asm__` statements alone. It is compile-checked and then check_unit-checked,
and anything that doesn't match is reverted.

```sh
python tools/shared_funcs.py port --units code_00207A38 --keep-types --fix-immediates
```

`--units` writes only the named destination units (safe next to other people's
edits). `--keep-types` keeps a type the destination unit already defines
(layouts differ between the games). `--fix-immediates` rewrites the struct
offsets and constants that the relocation-masked pairing cannot see: for each
ported function it reads the `mine X retail Y` words check_unit reports and
replaces the matching hex literals. Functions that still differ are reverted to
INCLUDE_ASM; if a ported declaration changes how other functions in the unit
compile, all of the unit's ports are undone (fix the layout by hand, then
re-run).

## Build details

`configure.py` automates all of these:

- splat splits each ELF per `config/<v>/SLUS_*.yaml`. Each C unit has its own
  `.text`, `.rodata`, `.lit4` and `.sdata` subsegments. `tools/split_rodata.py`
  computes them (`--section lit4|sdata` for the others), and
  `tools/include_rodata.py` and `tools/include_sdata.py` place the data that C
  doesn't produce yet.
- `INCLUDE_ASM` bodies are rewritten for the original ee-as by
  `tools/eeas_compat.py`. C is compiled by ee-gcc 2.96 at `-O2`, plus the
  unit's `cflags.txt` options, and assembled by ee-as with `-G8`.
- Jump-table entries in assembly rodata become absolute words
  (`tools/resolve_jtbl_targets.py`).
- `.sbss`/`.bss` are aligned to 128 bytes, as in Sony's `app.cmd`.
- False function starts (code that uses its caller's frame) are listed in
  `config/<v>/not_functions.txt` (`tools/find_fragments.py`).
- `tools/split_unit.py` splits a unit at a proven boundary, for example a run
  of functions built with different options.

## romwright (optional)

Function discovery, SDK signature naming and the DDS1/DDS2 pairing come from
[romwright](https://github.com/Raikaru/ventris):

```sh
# SDK signatures from archives you own (not committed):
romwright-cli signatures build --archive ee/lib/libkernl.a --package ps2sdk:libkernl \
    --license proprietary-local --out sigs/libkernl.db
ROMWRIGHT=/path/to/romwright-cli DDS_SDK_SIGS=sigs python tools/romwright_sync.py --reimport
romwright-cli diff dds2 --reference build/romwright --reference-name dds1 --json \
    --project build/romwright > build/dds1_diff_dds2.json
```

`romwright_sync.py` takes a signature name only when it lands on exactly one
function. Rows above the generated block of `symbol_addrs.txt` are curated
and win at the same address.

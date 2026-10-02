# DDS development ELFs

The `dds1-dev` and `dds2-dev` targets build separate executables whose layouts
can grow without weakening the byte-identical retail builds. Each target
recompiles and replaces selected code, read-only data, and initialized
small-data sections from three paired code units, and moves one source-owned
zero-initialized state object. One replacement retains two assembly fallback
functions. The targets also link development-only C code and data into an
appended loadable segment:

```sh
ninja dds1-dev dds2-dev
```

The results are `build/dds1/SLUS_209.74.dev` and
`build/dds2/SLUS_211.52.dev`. The ordinary `dds1` and `dds2` targets are
unchanged and remain SHA-1 checked against retail.

## What the target verifies

The development link uses the complete object graph and retains the linker's
relocation records. `tools/dev_elf.py` then rejects the output unless all of
the following hold:

- the input executable has the expected retail SHA-1;
- each retired section leaves an exactly sized zero-filled retail slot;
- replacement section addresses and sizes come from linker boundary symbols,
  rather than fixed offsets in the descriptor;
- every nonempty global or weak `FUNC` or `OBJECT` definition in a replaced
  span has exactly one development definition with the same name, binding,
  type, visibility, and logical move, wholly contained in that move;
- relocations against named replacement symbols keep that exact identity and
  addend; address-containment mapping is limited to ELF section-symbol
  relocations;
- every declared assembly fallback has the same function contract, exact
  bytes, and relative relocation signature as its retail object definition;
- every changed word outside the declared old slots, ELF metadata, and heap
  patches is explained by a relocation to the corresponding replacement
  symbol and addend;
- every `R_MIPS_GPREL16` addend is resolved against one unique `_gp`, which
  must be identical in the retail and development links;
- direct jumps, word-aligned absolute addresses, and common MIPS address
  constructions do not still refer to the abandoned slot;
- the appended `PT_LOAD` does not overlap an existing segment and fits in an
  unused program-header slot;
- file-backed moves come from file-backed input sections, while NOBITS moves
  come from actual `SHT_NOBITS` input sections and occupy only the appended
  segment's memory tail;
- linker boundary symbols determine distinct `p_filesz` and `p_memsz` values;
  the development heap begins after `p_memsz`, while the retail BSS clear
  boundary remains unchanged;
- each moved section contains the exact descriptor-asserted number of
  relocation entries at sites within its span;
- each NOBITS move asserts the number of relocation records whose effective
  targets land in its new range;
- every allocated replacement-object section is explicitly moved or retained;
  retained data and BSS sections must remain empty, and COMMON storage is
  rejected;
- every moved writable section that overlaps the signed retail `_gp` window
  declares exact retail and development GP-reference counts; no development
  reference may still address its abandoned range, and any surviving sites
  must retain their named function and intra-function offsets;
- each development-only addition occupies its linker-derived span and retains
  its expected number of relocation entries; and
- each static linker redirect starts at its asserted retail target and resolves
  to its named wrapper inside the development segment.

Each `replacements` entry substitutes a separately compiled object for the
object named by its `retail_object`; its moved sections are declared in
`moves`, while `retained_sections` accounts for allocated sections that stay
at their retail placements. The current policy requires every retained section
to have size zero. A replacement source may contain `INCLUDE_ASM` only when its
symbols are explicitly listed as fallbacks in source order. `INCLUDE_RODATA`
and `INCLUDE_SDATA` remain unsupported for replacement objects.

A writable move whose retail range overlaps the signed `_gp` window must set
both `expected_retail_gp_references` and `expected_gp_references`. These counts
cover physical GP-based address forms across the complete main text and all
moved or development-only text, rather than only relocation records emitted by
replacement code.

The descriptors for the current moves and their asserted binary sites are
`config/dds1/devbuild.json` and `config/dds2/devbuild.json`. The checks
intentionally fail closed when either retail layout or relocation closure
changes.

## Development entry hook

Each `src/dds*/dev/devbuild.c` is compiled only for its corresponding
development target, with small-data addressing disabled so it does not consume
the retail `$gp` window. Each link uses `--wrap` to redirect the single startup
call through a version-specific wrapper:

| Version | Original | Wrapper |
|---|---|---|
| DDS1 | `func_00101BD8` | `__wrap_func_00101BD8` |
| DDS2 | `func_00101AC0` | `__wrap_func_00101AC0` |

The wrapper increments `devBuildState.entryCount`, then passes the original
arguments and return value through unchanged.

The appended segment also exposes `devBuildIdentifier` and an initialized
`devBuildState.magic` marker. Together these provide concrete code, read-only
data, writable data, and an execution path for development additions without
placing any of them in the retail link.

## Current scope

These are relocatable development builds, not general mod loaders. Each
version replaces three paired units, then links one development entry object.
The first smaller replacement is compiled separately with `-G0`: its `.text`
is `0x3D0` bytes rather than the retail `0x3C8`, so two later function
definitions move by eight bytes. The mixed-source replacement grows from
`0x1230` to `0x1280` bytes of text while preserving its two assembly fallbacks
exactly.

The third replacement recovers an eight-byte writable font-node callback table
as typed source and moves it with the code that reads and initializes it. Its
`-G0` build expands GP-relative accesses into HI16/LO16 pairs, growing DDS1 text
from `0x3D4` to `0x40C` and DDS2 text from `0x464` to `0x49C`. The table's two
function pointers are relocated to the shifted callbacks in the same object;
the old code and small-data slots are zero-filled.

The first NOBITS move recovers `fileManagerWork`, a `0x40`-byte asynchronous
file-manager state object, as an explicit source definition in each title. Its
four slots begin at `+0x20` and consist of a value followed by a request
pointer; this corrects the previous nominal C layout, which was `0x44` bytes
and would overlap the next retail object if instantiated. The development
link moves the object to a `NOLOAD` tail, follows all 26 incoming HI16/LO16
relocation records in each title, and leaves the executable file payload
unchanged while increasing the appended segment's memory size by exactly
`0x40`.

Together the replacements pair 73 DDS1 and 74 DDS2 exported symbols, 50 of
which change their offset within their replacement. The verifier follows
1,002 DDS1 and 1,025 DDS2 external relocations to shifted definitions, plus 45
and 47 relocations between replacement objects. It retains 223 DDS1 and 231
DDS2 relocation entries within the file-backed moved sections, and follows 26
additional records into the NOBITS object in each title. All counts are
asserted by the version descriptors.

Replacement-owned code and read-only data may change size and contents. A
declared, file-backed initialized small-data section may also move when all of
its references and relocatable initializers pass the same closure audit. A
declared NOBITS object may move only when its source-owned input section, exact
retail and development extents, storage class, and complete incoming target
count are proven. Unaccounted allocated data, other nonempty BSS, and COMMON
storage remain rejected. Extending the replacement set is therefore a
deliberate per-object operation with a fail-closed link audit, not a claim that
arbitrary assembly, DMA data, or physical-address payloads are already
movable. Emulator and hardware execution remain an independent validation
step rather than a requirement for the static build capability.

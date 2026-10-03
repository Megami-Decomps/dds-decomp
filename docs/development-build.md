# DDS development ELFs

The `dds1-dev` and `dds2-dev` targets build separate executables whose layouts
can grow without weakening the byte-identical retail builds. Each target
recompiles and replaces selected code, read-only data, and initialized
small-data sections from eleven paired source objects, and moves three
source-owned zero-initialized state objects. The replacements retain ten
declared assembly fallback functions in DDS1 and nine in DDS2. The targets also link
development-only C code and data into an appended loadable segment:

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
- every declared assembly fallback has the same function contract, relative
  relocation signature, and instruction bytes as its retail object definition;
  changed section-symbol `R_MIPS_26` fields and canonical `HI16`/`LO16` address
  pairs are accepted only when both objects resolve them to the same named
  function and intra-function offset, while named-symbol relocations must
  preserve their encoded addends;
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
  nonempty retained sections require exact file-backed object bytes, flags,
  alignment, named symbols, and relocations, while COMMON storage is rejected;
- every nonempty retained section stays at its asserted retail address with
  exact linked bytes and exported symbols, preserves the retail `_gp`, and has
  exactly the declared retail and development physical GP-based address forms;
  development sites normally remain a subset of the same named retail function
  offsets, while a narrowly declared shifted-offset contract instead preserves
  their containing functions, opcodes, registers, target offsets, and
  multiplicities; source rebuilt with `-G0` may replace declared retail sites
  with audited `HI16`/`LO16` relocations;
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
at their retail placements. Empty retained sections may be absent. A nonempty
entry must declare its retail address, size, alignment, file storage, named
symbol count, and development GP-reference count. If the retail count differs,
`expected_retail_gp_references` declares it explicitly; otherwise it defaults
to the development count. Nonempty retained NOBITS is unsupported. A
replacement source may contain `INCLUDE_ASM` only when its symbols are
explicitly listed as fallbacks in source order. `INCLUDE_SDATA` is accepted
only when the object declares the corresponding nonempty `.sdata` section as
retained and the retained-section audit succeeds. `INCLUDE_RODATA` remains
unsupported for replacement objects.

A writable move whose retail range overlaps the signed `_gp` window must set
both `expected_retail_gp_references` and `expected_gp_references`. These counts
cover physical GP-based address forms across the complete main text and all
moved or development-only text, rather than only relocation records emitted by
replacement code.

The optional `allow_shifted_gp_reference_offsets` is limited to retained state
whose access instructions move within expanded replacement functions. It still
requires the same containing functions, opcodes, registers, target offsets,
and multiplicities; without it, intra-function offsets must match exactly.

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
version replaces eleven paired source objects, then links one development
entry object.
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

The FileManager NOBITS move recovers `fileManagerWork`, a `0x40`-byte asynchronous
file-manager state object, as an explicit source definition in each title. Its
four slots begin at `+0x20` and consist of a value followed by a request
pointer; this corrects the previous nominal C layout, which was `0x44` bytes
and would overlap the next retail object if instantiated. The development
link moves the object to a `NOLOAD` tail, follows all 26 incoming HI16/LO16
relocation records in each title, and leaves the executable file payload
unchanged while increasing the appended segment's memory size by exactly
`0x40`.

The complete FileManager subsystem is the first stateful replacement. Its
manager and callback-driver text, read-only strings, and `fileManagerWork`
state move together. The driver retains a source-built `0x10` small-data
section at its retail address: an eight-byte reentrancy guard record followed
by the eight-byte task name. Both source-recovered leading callbacks rebuild
their four guard accesses as audited `HI16`/`LO16` relocations under `-G0`, and
both drivers grow from `0xEF8` to `0xF10`. The remaining assembly callbacks
contain six GP-relative accesses in each title. The retained-section audit
proves those physical references at the same fallback-function offsets, exact
retained objects, and an unchanged `_gp` instead of silently splitting the
live state.

The paired SDF thread registry is a moved small-BSS state. Three source
units per title own and consume its semaphore and linked-list head. Their
retail text totals `0x698` bytes; the development text grows to `0x728` in DDS1
and `0x730` in DDS2. All 15 retail GP-relative accesses to the eight-byte
registry become 23 audited HI16/LO16 relocation records, and no physical GP
reference remains to either the moved state or its abandoned retail range.
The adjacent tick callback state stays at its retail address with exact bytes,
symbols, and access signatures.

The paired general SDF heap is another complete mutable lifecycle. Three
source units per title move `0xD88` bytes of retail text, growing to `0xDA8`,
along with the source-owned `0x28`-byte NOBITS heap root. Its two embedded
sentinels, base, and size have one canonical typed owner. Eight address
materializations into that root become 16 audited target relocation records;
the abandoned range has no surviving reference. Each leading unit retains an
exact `0xC` `.sdata` island containing three symbols. Its three retail GP
references become ordinary relocated address pairs in development, leaving
zero physical GP references to the moved heap state. The three units retain
three declared assembly fallbacks per title.

Together the replacements cover 168 DDS1 and 169 DDS2 exported symbols; 90
definitions per title change their relative offsets. The verifier follows
1,414/1,453 external relocation sites targeting shifted definitions, 126/128
relocations between replacement objects, and 2,776/2,972 total relocation
targets into moved content. The moved sections retain 764/780 relocation
entries. All counts are asserted by the version descriptors.

Replacement-owned code and read-only data may change size and contents. A
declared, file-backed initialized small-data section may also move when all of
its references and relocatable initializers pass the same closure audit. A
declared initialized section may remain at its retail address only under the
exact retained-section contract above. A declared NOBITS object may move only
when its source-owned input section, exact retail and development extents,
storage class, and complete incoming target count are proven. Unaccounted
allocated data, other nonempty BSS, and COMMON
storage remain rejected. Extending the replacement set is therefore a
deliberate per-object operation with a fail-closed link audit, not a claim that
arbitrary assembly, DMA data, or physical-address payloads are already
movable. Emulator and hardware execution remain an independent validation
step rather than a requirement for the static build capability.

## Portability boundary

Relocating a complete stateful subsystem proves that its code, owned state,
callbacks, and aliases are represented by the source and linker model rather
than by accidental retail addresses. It does not make that source host-native.
The development ELFs still use the 32-bit Emotion Engine ABI, MIPS calling
conventions, PS2 kernel handles, and the original SDF/IOP services. DMA-facing
addresses and several callback values are carried in 32-bit integer fields,
and the retained FileManager small-data section is intentionally an ABI island
inside the retail `$gp` window.

The useful porting boundary is therefore explicit: the recovered state layout
and lifecycle can remain game code, while allocation, asynchronous I/O, task
scheduling, DMA/address translation, and platform callback transport require
target-specific adapters. A native host build must give those concepts typed
handles and address representations before changing pointer width or structure
layout. The relocation verifier supplies evidence about ownership and hidden
address dependencies; it is not a substitute for that interface work.

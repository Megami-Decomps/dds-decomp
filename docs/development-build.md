# DDS development ELFs

The `dds1-dev` and `dds2-dev` targets build separate executables whose layouts
can grow without weakening the byte-identical retail builds. Each target
recompiles and replaces the complete `.text` and `.rodata` sections of two
paired code units, one of which retains two assembly fallback functions, and
links development-only C code and data into an appended loadable segment:

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
- direct jumps, word-aligned absolute addresses, and common MIPS address
  constructions do not still refer to the abandoned slot;
- the appended `PT_LOAD` does not overlap an existing segment and fits in an
  unused program-header slot;
- the development heap begins after the appended segment while the retail BSS
  clear boundary remains unchanged;
- each moved section contains the exact expected number of relocation entries
  at sites within its span (21 and 101 for the two text sections, and 10 and
  49 for their read-only data, in each current build);
- every allocated replacement-object section is explicitly moved or retained;
  currently retained data and BSS sections must remain empty, and COMMON
  storage is rejected;
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
version replaces two paired units' code and read-only data, then links one
development entry object. The smaller replacement is compiled separately with
`-G0`: its `.text` is `0x3D0` bytes rather than the retail `0x3C8`, so two later
function definitions move by eight bytes. The mixed-source replacement grows
from `0x1230` to `0x1280` bytes of text while preserving its two assembly
fallbacks exactly.

Together the replacements pair 55 exported symbols, 40 of which change their
offset within their replacement. The verifier follows 1,011 DDS1 and 1,036
DDS2 external relocations to shifted definitions, plus 35 relocations between
the two replacement objects in each game. It retains 181 relocation entries
within the replacement text and read-only data. All counts are asserted by the
version descriptors.

Replacement-owned code and read-only data may change size and contents. New
allocated data, small-data, or BSS sections are rejected until their
initialization and lifetime rules are modeled. Extending the replacement set
is therefore a deliberate per-object operation with a fail-closed link audit,
not a claim that arbitrary assembly, DMA data, or physical-address payloads
are already movable. Emulator and hardware execution remain an independent
validation step rather than a requirement for the static build capability.

# DDS development ELFs

The `dds1-dev` and `dds2-dev` targets build separate executables whose layouts
can grow without weakening the byte-identical retail builds. Each target
relocates the complete `.text` and `.rodata` sections of one paired code unit
and links development-only C code and data into an appended loadable segment:

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
- each moved section has exactly its declared size and its old slot is zero;
- every changed word in the retail-loaded prefix is explained by a relocation
  to the moved address interval or by a declared heap patch;
- direct jumps, absolute words, and common MIPS address constructions do not
  still refer to the abandoned slot;
- the appended `PT_LOAD` does not overlap an existing segment and fits in an
  unused program-header slot;
- the development heap begins after the appended segment while the retail BSS
  clear boundary remains unchanged;
- each moved section retains the expected number of relocation entries at
  sites within its span;
- each development-only addition occupies its declared span and retains its
  expected number of relocation entries; and
- each static linker redirect starts at its asserted retail target and resolves
  to its named wrapper inside the development segment.

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

These are early relocatable development builds, not general mod loaders. Each
version moves one paired unit's code and both of its jump tables, then links
one development entry object. Changed-size replacement units and broader data
relocation are future work. The builds perform structural and link-closure
validation, but emulator and hardware execution remain a separate validation
step.

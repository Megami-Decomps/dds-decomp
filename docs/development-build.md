# DDS1 development ELF

The `dds1-dev` target builds a separate DDS1 executable whose layout can grow
without weakening the byte-identical retail build. It relocates the complete
`.text` section of one code unit and links development-only C code and data
into an appended loadable segment:

```sh
ninja dds1-dev
```

The result is `build/dds1/SLUS_209.74.dev`. The ordinary `dds1` and `dds2`
targets are unchanged and remain SHA-1 checked against retail.

## What the target verifies

The development link uses the complete object graph and retains the linker's
relocation records. `tools/dev_elf.py` then rejects the output unless all of
the following hold:

- the input executable has the expected retail SHA-1;
- the moved section has exactly its declared size and its old slot is zero;
- every changed word in the retail-loaded prefix is explained by a relocation
  to the moved address interval or by a declared heap patch;
- direct jumps, absolute words, and common MIPS address constructions do not
  still refer to the abandoned slot;
- the appended `PT_LOAD` does not overlap an existing segment and fits in an
  unused program-header slot;
- the development heap begins after the appended segment while the retail BSS
  clear boundary remains unchanged;
- the moved section retains the expected number of outbound relocations;
- each development-only addition occupies its declared span and retains its
  expected outbound relocations; and
- each static linker redirect starts at its asserted retail target and resolves
  to its named wrapper inside the development segment.

The descriptor for the current move and its asserted binary sites is
`config/dds1/devbuild.json`. The checks intentionally fail closed when the
retail layout or relocation closure changes.

## Development entry hook

`src/dds1/dev/devbuild.c` is compiled only for `dds1-dev`, with small-data
addressing disabled so it does not consume the retail `$gp` window. The link
uses `--wrap=func_00101BD8` to redirect the single startup call through
`__wrap_func_00101BD8`. The wrapper increments `devBuildState.entryCount`,
then passes the original arguments and return value through unchanged.

The appended segment also exposes `devBuildIdentifier` and an initialized
`devBuildState.magic` marker. Together these provide concrete code, read-only
data, writable data, and an execution path for development additions without
placing any of them in the retail link.

## Current scope

This is an early relocatable development build, not a general mod loader. It
supports DDS1, moves one existing unit's code, and links one development entry
object. DDS2, changed-size replacement units, and broader data relocation are
future work. The build performs structural and link-closure validation, but
emulator and hardware execution remain a separate validation step.

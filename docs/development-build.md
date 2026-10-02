# DDS1 development ELF

The `dds1-dev` target builds a separate DDS1 executable whose layout can grow
without weakening the byte-identical retail build. It currently relocates the
complete `.text` section of one code unit from its retail address into an
appended loadable segment:

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
  clear boundary remains unchanged; and
- the moved section retains the expected number of outbound relocations.

The descriptor for the current move and its asserted binary sites is
`config/dds1/devbuild.json`. The checks intentionally fail closed when the
retail layout or relocation closure changes.

## Current scope

This is the first relocatable development-build milestone, not a general mod
loader. It supports DDS1 and moves one existing unit's code; DDS2, convenient
added source units, and broader data relocation are future work. The build
performs structural and link-closure validation, but emulator and hardware
execution remain a separate validation step.

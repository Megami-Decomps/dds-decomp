# Data symbol names carried from SMT3

<!-- rename_refs: keep -->

The companion to [SMT3 names](smt3-names.md), which carried function names from
the Shin Megami Tensei III: Nocturne HD symbol recovery. This page covers the
globals: objects in `.data`, `.bss`, `.sdata`, `.sbss` and `.rodata`.

Every name applied this way is `inferred` in `config/<v>/name_sources.txt`, with
a note of the form `SMT3HD data name via SLUS_209.11 0x<addr>, <n> agreeing
references; was <old name>`. The old name is kept there so a rename can be
audited or undone.

## How a global is identified

The function pass left a by-product: thousands of DDS functions whose masked
instruction stream is identical to a Nocturne function's. In such a pair the
instruction at a given index is the same instruction with the same registers,
and only the relocated immediate differs. So whatever object Nocturne's
instruction addresses, the DDS instruction at that index addresses the
corresponding object. Each such reference is one vote for a mapping from a
Nocturne data address to a DDS data address.

`smt3-decomp/tools/dds_data_names.py` collects those votes over every identical
pair, resolving `lui`/`%lo` pairs and `$gp`-relative accesses on both sides, and
accepts a mapping only when:

1. Every reference agrees. A Nocturne symbol whose references point at two
   different DDS addresses is rejected rather than decided by majority.
2. The Nocturne side is a real name, not a `D_`, `jpt_` or `jtbl_` placeholder.
3. The DDS address is the start of a symbol this repo already declares. A vote
   landing inside a larger object, or on an address with no symbol, is reported
   and dropped: this pass renames objects, it does not create them.
4. Both sides' declared extents agree, where both are known. A `size:` on one
   side and silence on the other is allowed; two different sizes are not.
5. The DDS name is not `evidence`, and not an all-lowercase runtime or libc
   name the linker resolves by name.
6. No two Nocturne symbols claim one DDS address, and no name lands on two.
7. A DDS1 and DDS2 object that map from the same Nocturne symbol must take the
   same name, since `include/` is shared.
8. The address is not listed in `config/smt3_data_exclude.txt`.

## What landed

```
dds1: 4343 identical pairs  accept=351  reject:conflicting-votes=2  reject:excluded=4  reject:extent-differs=8  reject:not-a-dds-symbol=261  reject:smt3-placeholder=3  same=1

dds2: 4336 identical pairs  accept=355  reject:conflicting-votes=1  reject:excluded=4  reject:extent-differs=8  reject:not-a-dds-symbol=256  reject:smt3-placeholder=3  same=1
```

The largest rejected group by far is `not-a-dds-symbol`: about 260 Nocturne
globals per game land on DDS addresses this repo has not symbolised, usually
inside a `D_` blob that splat emits as one object. Giving those their own
symbols is separate work, and worth doing, since it would roughly double what
this pass can carry.

Of the accepted names, 69 are a bare lowercase word rather than the usual
module-prefix camelCase: `startinglistf`, `runninglistcnt`, `fadergba`,
`codefunc`, `skyanime`, `fldbokasi`. Those are the original spellings, and the
kernel's three task queues read the way the engine's author wrote them.

## Names that could not be carried

ee-gcc 2.96's GCSE hashes a `SYMBOL_REF` by the symbol's name, so renaming a
global changes the expression table's traversal order and can change a register
allocation tie in a function that reads it. `docs/idioms.md` records this, and
it is the same effect that forced exclusions in the function pass. Four globals had to keep
their current names:

| Game | Function it tips | Unit | Held back |
|---|---|---|---|
| DDS1 | `dds3Particle_SparkOffsetBuffInit` | `game/code_00151F58` | `dds3EffectBase` |
| DDS1 | `dds3Particle_SparkBuffInit` | `game/code_00151F58` | `ParRand` |
| DDS1 | `fldInitializeActorMotionSlots` | `game/code_00138C30` | `fldDoorParamTable` |
| DDS2 | `sdfDrawTexturedQuad` | `game/code_002DE248` | `aViewEye`, whose name also reads as an IDA auto-name rather than a recovered original |

The first is the same object `docs/idioms.md` describes: the two Spark
generators became exact when it was named `effDefaultRandomState`, so that
spelling is load-bearing and the recovered name has to wait for a
reformulation. All three addresses are in `config/smt3_data_exclude.txt` with
their reason, and the generator skips them on a rerun.

## What a data rename touches

`tools/names.py` rewrites `config/<v>/symbol_addrs.txt`, records provenance, and
renames references in `src/<v>/**/*.c`. `tools/rename_refs.py` covers the rest:
headers, docs, and the config text. Two of those need naming here because they
are easy to miss:

- `config/<v>/symbol_aliases.json` holds interior aliases as a name, an owner
  and an offset, and `configure.py` validates that the name equals the owner
  plus the offset by looking both up by name. Renaming either end without the
  other stops the build at configure time, so `rename_refs.py` now covers that
  file.
- `tools/test_check_unit.py` pins four real symbol names as fixtures. Those were
  updated by hand in step; a rename that moves one of them makes the test fail
  loudly, which is the behaviour we want.

The generated `INCLUDE_RODATA` and `INCLUDE_SDATA` lines name each symbol, and
the split emits one assembly file per symbol under that name, so
`configure.py --force-split` has to run after a rename. It regenerates both.

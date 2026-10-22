<!-- rename_refs: keep -->
# SMT3HD names for DDS1 and DDS2

Shin Megami Tensei III: Nocturne HD Remaster ships as an il2cpp Unity build, and
il2cpp metadata keeps the original function names. The smt3-decomp project
(`SLUS_209.11`) carries those names in its IDA database at PS2 addresses: 9470 of
9571 functions. Digital Devil Saga shares Nocturne's engine, so where a DDS
function is the same code as a Nocturne function it is the same original
function, and the Nocturne name is the real name, not a description.

Every name this applied is marked `inferred` in `config/<v>/name_sources.txt`,
with a note of the form:

```
SMT3HD name via SLUS_209.11 0x<addr>, <evidence>; was <old name>
```

`inferred` rather than `evidence` because the name comes from a paired binary,
not from the DDS binary's own text; the distinction and the rest of the naming
convention are in [CONTRIBUTING.md](CONTRIBUTING.md) section 4.

## Pipeline

Three `tools/pair_build.py` runs in smt3-decomp pair functions by masked code
between two Nocturne-engine builds (method and accuracy measured on the Maniax
builds in smt3-decomp's `docs/maniax-build.md`):

```sh
# SMT3 (SLUS_209.11) to DDS1 (SLUS_209.74)
python3 tools/pair_build.py --from-instance SLUS_209.11 --to-instance SLUS_209.74 \
    --to-elf /home/wsl/projects/dds-decomp/orig/dds1/SLUS_209.74 --to-text-end 0x00315BA0 \
    --cache-dir build/dds_names/cache_smt3_dds1 --output build/dds_names/smt3_dds1.tsv

# SMT3 to DDS2 (SLUS_211.52)
python3 tools/pair_build.py --from-instance SLUS_209.11 --to-instance SLUS_211.52 \
    --to-elf /home/wsl/projects/dds-decomp/orig/dds2/SLUS_211.52 --to-text-end 0x00370B80 \
    --cache-dir build/dds_names/cache_smt3_dds2 --output build/dds_names/smt3_dds2.tsv

# DDS1 to DDS2, needed so a name carries to both games' shared include/
python3 tools/pair_build.py --from-instance SLUS_209.74 --to-instance SLUS_211.52 \
    --from-elf /home/wsl/projects/dds-decomp/orig/dds1/SLUS_209.74 --from-text-end 0x00315BA0 \
    --to-elf /home/wsl/projects/dds-decomp/orig/dds2/SLUS_211.52 --to-text-end 0x00370B80 \
    --cache-dir build/dds_names/cache_dds1_dds2 --output build/dds_names/dds1_dds2.tsv
```

`tools/dds_names.py` then turns the three TSVs into tables dds-decomp's
`tools/names.py` can apply, in smt3-decomp:

```sh
python3 tools/dds_names.py \
    --dds1 build/dds_names/smt3_dds1.tsv build/dds_names/cache_smt3_dds1 \
    --dds2 build/dds_names/smt3_dds2.tsv build/dds_names/cache_smt3_dds2 \
    --dds1-dds2 build/dds_names/dds1_dds2.tsv \
    --dds-decomp /home/wsl/projects/dds-decomp \
    --out-dir build/dds_names/out
```

and in dds-decomp:

```sh
python3 tools/names.py apply dds1 build/dds_names/out/names_dds1.tsv
python3 tools/names.py apply dds2 build/dds_names/out/names_dds2.tsv
python3 tools/rename_refs.py build/dds_names/out/renames_dds1.tsv build/dds_names/out/renames_dds2.tsv
.venv/bin/python configure.py --force-split
```

## Acceptance rules

`dds_names.py` decides one SMT3 function at a time (`docs/dds_names.py`'s own
docstrings and `decide()`/`candidates()` carry the detail; this is the order):

1. **Byte-identical.** The SMT3 function's masked-instruction hash and its raw
   (unmasked) hash are both unique on both sides: forced regardless of tier.
2. **Relocation-identical.** Failing that, a hash that keeps every constant
   except the fields that form addresses (`lui` halves, the immediate that
   completes them, `j`/`jal` targets) is unique on both sides.
3. **Shared-hash pairs only inside a constant-delta run.** A masked hash shared
   by more than one function on either side is kept only when the aligned pair
   immediately before or after it in SMT3 address order has the same
   `dds_addr - smt3_addr` delta, i.e. its unit linked as one block with its
   neighbor.
4. Medium- and low-tier `pair_build.py` alignments (no hash match, position
   inferred from the surrounding gap) are rejected outright.
5. **HD address tags, `gcc2_compiled` markers, IDA `_N` suffixes rejected.**
   An SMT3 name like `fcl_213998` or `moviePlayFile_27041744` is an address
   tag, not a name; `gcc2_compiled__7` is a compiler marker IDA turned into a
   symbol; a trailing `_N` is IDA's suffix for a repeated name. None of these
   replace a DDS name.
6. **Evidence names and their twins are never replaced.** A DDS name the
   binary's own text names (`config/<v>/name_sources.txt` kind `evidence`,
   e.g. `sdfAddHandler`) stays, and so does its twin in the other game.
7. **All-lowercase DDS names are never replaced.** A name with no capital
   letter is compiler runtime or libc (`dpcmp`, `memcpy`, ...): the SDK named
   it and the linker resolves it by that name, so the game's own convention
   (lowercase prefix, then CamelCase) does not apply.
8. **Transitivity check through DDS1 to DDS2.** When the SMT3-to-DDS1 pairing
   and the SMT3-to-DDS2 pairing disagree with the independent DDS1-to-DDS2
   pairing for the same SMT3 function, the name is dropped on both sides
   rather than trusted on a 2-out-of-3 vote.
9. **Twins must share a name.** `include/` is shared, so a DDS1 function and
   its DDS2 twin (by the DDS1-to-DDS2 pairing, high tier, strong hash) take
   the same new name; a case where the two sides would get different names is
   dropped on both sides instead of resolved by preference.
10. **Same-old-name propagation.** When only one game's function paired, but
    the other game has a function under the same old name (true of most
    `include/`-declared functions), the unpaired side takes the name too.
11. **`config/smt3_names_exclude.txt`.** Listed `<version> <address>` pairs
    keep their current name regardless of how strong the pairing is: the
    mechanism for a CONTEXT exclusion (next section).

## Spelling-sensitive exclusions

ee-gcc 2.96 ties two codegen decisions to identifier spelling. Its CSE hashes
the addresses of symbol-name strings, and its GCSE expression table hashes a
`SYMBOL_REF` by the symbol's name, which can change PRE pseudo creation order
and a later register-allocation tie. `docs/idioms.md` describes both. A rename
can therefore change the code emitted for an unrelated function in the same
unit, and because `include/` is shared, a rename made for one game reaches the
other game's units too.

Three functions had to be protected, each found by delta-debugging the renames
visible to its unit:

| Game | Function | Unit | Held back |
|---|---|---|---|
| DDS2 | `btlSortActorMuzzleDirections` | `game/code_001AF4A0` | `evtGetAgeOfMoon` |
| DDS2 | `sdfDrawTexturedQuad` | `game/code_002DE248` | `sdfGrOtClose`, `sdfItemArrayNew`, `sdfMatSetRefMap`, `sdfMatSetTex`, `sdfMatSetTexUvw`, `Mult`, `MultMatrixNoArg`, `MultMatrixRev`, `SetRotateMatrixY_B`, `ToMatrix`, `dblurGetParam2`, `dblurSetParam2` |
| DDS1 | `btlInitializeRandomCameraCursor` | `game/code_001DEFE0` | `FromEuler` |
| DDS2 | `sdfDrawTexturedQuad`, again | `game/code_002DE248` | `dds3BillboardSetPosition`, after main's field and effect names landed |

The second row needs its names as a set: reverting any eleven of the twelve
leaves the function differing. The first five were enough until main changed
the headers that unit sees, at which point the other seven had to join them,
and holding those back then tipped the DDS1 function in the third row. DDS1's
own `sdfDrawTexturedQuad` (`game/code_002BC8F0`) needs no exclusion, but it
does break if a header gains a second declaration to cover a one-sided rename,
which is why `tools/rename_refs.py` never adds one.

Each address sits in `config/smt3_names_exclude.txt` with its reason, and
`tools/dds_names.py` skips them on a rerun, so both games keep one name per
function. Holding names back is the cheap fix, not the right one, and it is
fragile: a later edit to the headers these units see can tip them again and
change which set works. Reformulating the three functions so they match with
the recovered spellings would free all fourteen names.

## Final counts

```
dds1: accept=3762  reject:address-tag=116  reject:collision-with-0x002FC7A0=1  reject:dds-evidence=2  reject:dds-runtime-name=6  reject:excluded=15  reject:not-in-dds-symbols=5  reject:shared-hash-off-run=174  reject:smt3-placeholder=22  reject:tier-low=268  reject:tier-medium=235  reject:transitivity=1  same=406  (carried from twin: 7)

dds2: accept=3761  reject:address-tag=114  reject:collision-with-0x003578E8=1  reject:dds-runtime-name=7  reject:excluded=15  reject:not-in-dds-symbols=9  reject:shared-hash-off-run=170  reject:smt3-placeholder=22  reject:tier-low=321  reject:tier-medium=276  reject:transitivity=1  reject:twin-evidence=2  same=422  (carried from twin: 17)
```

## Two pairs of names that were swapped between the games

`tools/dds_names.py`'s DDS1-to-DDS2 twin check caught two cases where the two
games' own `inferred` names had been assigned to the wrong member of a pair:

- DDS1's `fldCmdSetFloorFlag` was the same code as DDS2's `fldCmdClearFloorFlag`
  (and vice versa). The SMT3HD names correct this: both games now call that
  function `fldCommand_FLD_MAP_ON` or `fldCommand_FLD_MAP_OFF` by what the code
  actually does, not by which game had guessed which name.
- Likewise `sdfVuLoadRotationMatrixY` and `sdfVuLoadRotationMatrixZ` in DDS1
  were swapped relative to DDS2. Both games now use `SetRotateMatrixY` and
  `SetRotateMatrixZ` consistently.

## A tools/names.py fix this relied on

An older `tools/names.py` matched identifiers inside string literals, so
renaming a function could rename a task name inside a message string with it
(a task named `"fileMan"` became `"fileManagerProc"` when the function
`fileMan` was renamed `fileManagerProc`). `tools/names.py` now matches string
literals first and leaves them alone, which this rename depended on: 3,780
renames per game touch enough string content that the old behavior would have
corrupted debug text throughout.

## What this does not cover

- Unpaired DDS-only code (no SMT3 counterpart; most of the field, battle AI
  and camp-grid systems written for DDS).
- Data symbols. The same pairing reaches about 625 named SMT3 globals per game
  through functions that paired identically, but applying them needs the same
  provenance treatment as the functions plus a check that the DDS object has
  the same extent as its SMT3 counterpart. Not applied here (see
  smt3-decomp's `docs/dds-names.md`, "Second stage: globals").
- Types and struct members.

# DDS field shadow renderer: 7,328-byte reconstruction

Both functions remain `INCLUDE_ASM`. Complete typed research candidates compile
with the native EE GCC 2.96 toolchain, but neither passes the retail comparison.
This record contributes compiler and source evidence, not additional C coverage.

| Game | Function | Source unit | Retail bytes | Candidate bytes |
|---|---|---|---:|---:|
| DDS1 | `func_0011ECC8` | `src/dds1/game/code_0011D3A0.c` | 7,328 | 7,120 |
| DDS2 | `func_00120B88` | `src/dds2/game/code_0011F208.c` | 7,328 | 7,120 |

## Provenance and retained work

The isolated worktree is `build/worktrees/field-large-worker20-20261011`, branch
`decomp/field-large-worker20-20261011`, based on
`90049a9025766ef26933a4f097de43180e4389c8`. Production sources, shared headers,
configuration, compiler wrappers, and the original dirty worktree were not edited.

The starting draft was read from the primary decomp worktree's ignored
`build/field-large-7328-m2c.c` under `build/worktrees/largest-20261011`.
Its unchanged SHA-256 is
`ee92150dafdc63ab80fcd5f670a3acddfa281fe40e71cf25dc3f6be5c2f9582b`.
It contains 141 `M2C_ERROR` markers. The repaired candidate contains none and
produces no native compiler diagnostics; this does not establish behavioral or
byte equivalence.

The [machine-readable evidence](field-shadow-7328-evidence.json) records the
complete symbol translation, object sizes and hashes, native tool hashes,
compiler exit codes, retail input identities, and the exact checker summaries.

The ignored archive, relative to the worker worktree, is
`build/field7328-worker20-evidence.zip` (466,409 bytes, 51 entries). Its SHA-256 is
`f40a5e75d9a87c8b704b0e080e77e813f908ed0293be1a048d56afa9b7a26350`.
The archive and every manifest entry were read back and verified. Retain this
archive before removing the worktree: the documentation commit deliberately
does not contain nonmatching C, generated assembly, objects, or retail inputs.

Useful archive entries are:

- `field7328-dds1-body.c`: the repaired typed body, SHA-256
  `bb0a44ee5f42d5f610ff708fc8172e38963369650066c03da9829c71ed9cd416`.
- `field7328-dds1-unit.c` and `field7328-dds2-unit.c`: full source contexts with
  only the selected assembly inclusion replaced for the experiment.
- `field7328-<game>.s`, `.o`, `-full.s`, `-full.o`, `-check.log`, and
  `-receipt.json`: native compiler output, full-unit output, retail differences,
  and hashes binding the checked source to its toolchain.
- `field7328-c-copy/`: the earlier ordinary-`u128` C trial and its native output.
  `reference/` retains the original m2c draft and both target assembly listings.
- `field7328_prepare.py`, `field7328_native.sh`, and `field7328_evidence.py`:
  preparation, native compilation/comparison, and ELF-symbol inspection.

## Retail pair correspondence

Both retail bodies contain 1,832 instruction words; 1,572 words are identical
before any normalization. An instruction-by-instruction audit translates 94
referenced symbols and expresses branch labels relative to each function start.
Exactly two other instruction differences remain:

| Function-relative offset | DDS1 operand | DDS2 operand | Existing owner |
|---|---|---|---|
| `+0x46C` | `lwc1 $f1,0x140($17)` | `lwc1 $f1,0x14C($17)` | `fldAreaState.x` |
| `+0x478` | `lwc1 $f2,0x148($17)` | `lwc1 $f2,0x154($17)` | `fldAreaState.z` |

The existing conditional `FldAreaWork` layout expresses both differences.
The sequel candidate therefore comes from the same typed body plus the audited
symbol translation. No guessed address substitutions are needed.

The rendering purpose is inferred from the triangle planes, silhouette-edge
counts, extruded faces, two packet passes, and projected bounding rectangle.
No new function names were applied.

## Recovered source contracts

The object path is the existing `ObjBase.resourceSlots` array into
`SdfModel.list->buffer`, whose entries are `SdfDrawNode *`. Each selected node's
matrix is its `worldMatrix` at `+0xC0`. The base-resource getter supplies a
`MdlCtx *`, and its `inner` field supplies the model. This replaces the draft's
incorrect pointer arithmetic through `ObjBase.resourceHandle`.

The transition work uses `EvtUnit.owner->inner`, `EvtUnit.vec10`, the `0x40000`
flag, and the unsigned byte at `+0xD3`. The light target is an `EffWorldNode`
whose `inner` is the existing `ObjectTransform`. `dds3GetWorldObject` takes no
argument. The field-position fallback intentionally uses field Z for both its
Y and Z differences; that observed behavior was retained.

The global scratch layouts are supported by the indexed accesses and adjacent
symbol boundaries:

| DDS1 symbol | Research view | Access evidence |
|---|---|---|
| `D_003C2E90` | Four-float light direction | Aligned VU load/store |
| `D_003C2EA0` | 50 rows of three four-float vertices | Triangle stride `0x30` |
| `D_003C3800` | `26 x 26` two-byte edge records | Row stride `0x34`; signed count/orientation reads |
| `D_003C3D50` | `26 x 26` four-float endpoints | Element stride `0x10` |
| `D_003C6790` | Second endpoint array with the same shape | Same indexing |

The two packet descriptors are existing `SdfPrimitiveRequest` records, each
cleared for `0x2C` bytes. Their counts are 16-bit fields, their colors are packed
32-bit words, and their position/strip fields are pointers. `func_002E21A0`
takes one descriptor pointer. Each surface append call passes only the surface
and its packet list, using `SdfPoolNode.append`. The draft's extra inferred call
arguments were not retained.

Both missing scalar square roots use the existing `fsqrtf` primitive. The
direction normalization, matrix transforms, dot product, VU return-value store,
and perspective projections use existing `pcp_vu0.h` primitives. The camera
direction and otherwise unused long-offset stores observed in retail are
retained.

For the target-light branch, let `h` be the horizontal distance before direction
normalization. The opacity multiplier is
`100 - (clamp(h, 100, 300) - 100) / 2.6`. The rectangle's final alpha is
`max(0, (s32)((f32)D_003BAB28 * multiplier / 100))`. The final rectangle call
receives eight integer arguments, including bounds converted after multiplying
each floating coordinate by 16.

Ordinary C initially expressed the aligned full-vector copies through a `u128`
union member. Native GCC merged adjacent copies and reused loaded values; the
trial emitted 30 `lq` and 37 `sq` instructions, versus retail's 53 of each.
The retained typed-float candidate uses the existing `PCP_COPY_VECTOR_F32`
contract for all 41 vector-copy sites. Retail retains unrelated triangle-count
state across the copies at DDS1 `0x0011FDC0..0x0011FDE4`; the narrower contract
allows that behavior. The exact original SDK declaration remains unresolved.

The triangle plane input is filled through twelve direct scalar assignments.
A C aggregate initializer introduced an additional native block copy absent
from retail. The candidate represents the aligned scratch area before the plane
as a four-row matrix, with only three initialized rows consumed by the helper.
That matrix extent is an inference from the stack layout, not a recovered
original declaration.

Projected bounds are updated immediately after each projected point, as in
DDS1 `0x00120380..0x0012059C`. Extruded triangle writes modify XYZ only; their
existing fourth lanes are left untouched. Both initial face-vector W stores at
`0x0011EFC0` and `0x0011EFD0` are retained.

## Native validation and remaining differences

The native runner copies the baseline tools and source context onto WSL Debian's
native filesystem under `/tmp/dds-field7328-worker20.*`. It invokes unchanged
`tools/cc.sh` with the canonical unit paths, the repository's EE GCC 2.96 at
`-O2`, and the original EE assembler. No target-specific flag override was added.

The actual native checker inputs were independently hashed and compared with
the configured retail identities: DDS1 SHA-1
`6d18898e2724bf1d145392766e8ba1e678487419` and DDS2 SHA-1
`9be91ee1b4a535a4cb6ec89237b5a6ba41be2add`. All 1,832 annotated reference words
per function agree with their ELF bytes. `field7328-retail-input-audit.json`
records the file offsets, target hashes, and zero reference differences. This
checks reference provenance; it does not turn the candidates into matches.

Both `SKIP_ASM` and full-unit compilations return zero, with empty diagnostic
logs. The full-unit target sizes are also 7,120 bytes. Actual retail checks are:

```text
DIFF func_0011ECC8 @ 0x0011ECC8: 1510 of 1780 words differ (first at +0x0)
0 match, 1 differ

DIFF func_00120B88 @ 0x00120B88: 1510 of 1780 words differ (first at +0x0)
0 match, 1 differ
```

Both checker processes return 1. Neither reports `CONTEXT`, `NOASM`, or `UNDEF`.
The word counts are the checker's positional comparison output, not a measure
of semantic completeness.

| Measurement, each game | Retained candidate | Retail |
|---|---:|---:|
| Frame size | `0xBB0` | `0xBC0` |
| Instruction words | 1,780 | 1,832 |
| `lq` / `sq` | 49 / 49 | 53 / 53 |
| `lqc2` / `sqc2` | 16 / 11 | 16 / 11 |
| `sqrt.s` | 2 | 2 |
| `jal` | 50 | 50 |

The first failure is the frame-size instruction. After the frame and related
spill offsets, the early control-flow difference starts at relative `+0x3B8`,
in the light-target fade state. Later differences include register lifetimes,
quadword spills, triangle-output address formation, and packet/loop scheduling.
These require further source reconstruction; matching the frame with artificial
padding or forcing registers would not establish an acceptable match.

No runtime or emulator equivalence test was performed. A whole-game rebuild was
not used as evidence for the candidates. The production source diff is empty,
and both selected `INCLUDE_ASM` lines remain present.

## Reproduction in the shared workspace

From the retained worker worktree, regenerate the candidate contexts and invoke
the native checks:

```powershell
python build/field7328_prepare.py
wsl -d Debian -- sh '/mnt/d/Digital devil saga decomp/build/worktrees/field-large-worker20-20261011/build/field7328_native.sh' dds1
wsl -d Debian -- sh '/mnt/d/Digital devil saga decomp/build/worktrees/field-large-worker20-20261011/build/field7328_native.sh' dds2
python build/field7328_evidence.py
```

The native script calls `python3 tools/check_unit.py <canonical-unit> --func
<target> -v` after retaining both compiler contexts. The two nonzero checker
results above are expected for this candidate. The helper paths assume the
recorded shared-workspace layout and the original primary tool/input locations.

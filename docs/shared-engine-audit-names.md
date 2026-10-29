# Shared engine names used by the port audit

The paired implementations use the same engine names while keeping each
game's native signatures and layouts. HD spellings come from the recovered
symbol and type metadata; the other spellings below retain an existing
descriptive name from one decompilation. A matching checksum remains required.

## Functions

| Previous DDS name | Shared name | Basis |
| --- | --- | --- |
| `sdfModelBuildNodeResourcePackets` | `sdfModelLinkOt` | HD spelling; paired native role |
| `fldDrawShadowDisc` | `dds3DrawUnitObjectShadow` | Reviewed existing Nocturne name |
| `dds3UpdateEffectObjectFollowParameters` | `dds3UnitObjectBasicDrawProcedure` | HD spelling; paired native role |
| `dds3UpdateCrossfade` | `dds3CrossfadeRun` | HD spelling; paired native role |
| `effResourceQuadDraw` | `dds3EffectFilterDrawTexQuad` | Reviewed existing Nocturne name |
| `fldGetSceneReadyFlag` | `fldAutoMapChkMode` | HD spelling; paired native role |
| `mnuAcknowledgeCampState` | `campCheckExitState` | Reviewed existing Nocturne name |

Nocturne's address placeholder `cmp_239FA0` uses DDS's existing name
`uiDrawUniformRgbRange`: the relocation-identical wrapper supplies one packed
color for all three triangle vertices.

The prior kernel wave already aligned `dds3DrawFirstProcess`,
`dds3FirstProcess`, `dds3DrawProcess`, and the entry `sdfDefaultMain`
(assembler name `main`).

## Records

| Previous DDS type | Shared type | Basis |
| --- | --- | --- |
| `SdfPoolNode` | `sdfGrOt_t` | Recovered HD type spelling; native layout retained |
| `SdfDmaNode` | `sdfGrRefPacketHeader_t` | Recovered HD type spelling; native layout retained |
| `EffPacketParams` | `sdf3DPrimPolyParm_t` | Recovered HD type spelling; native layout retained |
| `EvtWorldTable` | `evtWorldTable_t` | Reviewed existing Nocturne spelling |
| `EvtWorldSlot` | `evtWorldSlot_t` | Reviewed existing Nocturne spelling |
| `FldCollisionFace` | `fldCollisionFace_t` | Reviewed existing Nocturne spelling |
| `FldAreaWork` | `fldAreaWork_t` | Reviewed existing Nocturne spelling |
| `SdfPrimitiveRequest` | `sdf3DPrimPolyParm_t` | Recovered HD type spelling; native layout retained |

The two DDS primitive-request views describe the same 0x2C record. They now
use one declaration, retaining the former effect-view pointer types. The
primitive header verifies all thirteen native member offsets and the size.
No unit boundaries or source filenames move.

## Fields

| DDS record | Previous field | Shared field |
| --- | --- | --- |
| `sdfDrawTag_t` | `nextList` | `next` |
| `sdfDrawTag_t` | `first` | `head` |
| `sdfDrawTag_t` | `last` | `tail` |
| `sdfGrOt_t` (formerly `SdfPoolNode`) | `first` | `tagHead` |
| `sdfGrOt_t` (formerly `SdfPoolNode`) | `last` | `tagTail` |
| `dds3Basic_t` | `value` | `name` |
| `dds3Basic_t` | `data` | `workdata` |
| `dds3Basic_t` | `inner` | `poseset` |
| `dds3PoseSet_t` | `position` | `pos` |
| `datUnitWork_t` | `maxHp` | `maxhp` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `parameterCount` | `faceCount` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `vertexCount` | `pointCount` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `primitive` | `attr` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `mask` | `attrInhibit` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `parameters` | `faceIndex` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `positions` | `vertex` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `normals` | `normal` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `texcoords` | `uv` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `extraTexcoords` | `multiUv` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `colors` | `vertexColor` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `allocate` | `allocator` |
| `sdf3DPrimPolyParm_t` (formerly `EffPacketParams`) | `depth` | `zBias` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `stripWordCount` | `faceCount` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `vertexCount` | `pointCount` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `primitiveFlags` | `attr` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `clipMask` | `attrInhibit` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `strip` | `faceIndex` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `positions` | `vertex` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `normals` | `normal` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `coordinates` | `uv` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `secondCoordinates` | `multiUv` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `vertexColors` | `vertexColor` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `allocate` | `allocator` |
| `sdf3DPrimPolyParm_t` (formerly `SdfPrimitiveRequest`) | `depth` | `zBias` |

Nocturne's local primitive declaration also follows `allocator` for the
callback at +0x24 and `zBias` for the float at +0x28; its previous field
names were `allocate` and `allocator`. The native layout and types stay put.

The earlier process-member changes remain `flag`, `processfunc`, `workdata`
and `next` in place of DDS's `flags`, `update`, `userValue` and `listNext`.
In particular, DDS's `workdata` remains its native u32 representation.

## Matching-sensitive exclusion

Both DDS games keep `sdfFlushPoolNodes` (DDS1 0x002D4490, DDS2
0x0032D340). Nocturne calls the same function `sdfGrOtClose`. The exclusions
remain in `config/smt3_names_exclude.txt`: the renamed declaration changes
ee-gcc 2.96's allocation in the full-unit `sdfDrawTexturedQuad`.

Natural UV-endpoint, packed-value and vertex-record forms did not restore
DDS1 matching. The quad bodies and compiler flags are unchanged. This is the
one withheld row of the port audit; the other nineteen rows are aligned.
The shared record names still apply to this row. No alias, padding, unused
declaration or alternate function status is introduced to hide the exclusion.

## Public-record names

DDS's `EvtRuntime` is `evtViewerInfo_t`, with `curFrame` named `nowframe`
at +0x18. Its native record, field types and other member names remain as
reconstructed for each game. Nocturne already has these names.

DDS's `WorldInfo` is `dds3WorldBasic_t`, with the original struct tag
and typedef name. `controlScene` is `controlscene` at +0x0C, including the
selected-node view. The complete native world record is public in `basic.h`.
The administration and viewer records are in `kernel.h` and `event.h`.
Native sizes stay 0x24 for administration, 0x28 for world state, and viewer
0x2490 in DDS1 / 0x24BC in DDS2 (Nocturne's viewer is 0x2328).

The battle action unit imports its five used basic interfaces without the
unused world payload; its bodies and expressions are unchanged. This lets
the original world name coexist with all full-unit matches. No callback or
compiler flag is added to compensate for declaration context.

# Exact field-object constructors

## Treasure initialization: one record and conversion at its uses

`FldObjectSlot.unitObject` is the actual `dds3Basic_t *` returned by
`dds3AddUnitObjectBasic`, not an integer handle. The activation updater loads
slot `+0x00` directly for `dds3ObjectBaseGetModelHandle`; both games reset it
with null and perform no integer operations on it. The pinned SMT3 HD
[`fldTakaraWork_tag`](https://github.com/smt-ret/smt3-reconstructed-sources/blob/cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1/include/fldTakaraWork_tag.h)
independently gives the field that pointer type. Its
[`fldItemBoxAdd`](https://github.com/smt-ret/smt3-reconstructed-sources/blob/cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1/src/fld_Npc.c)
assigns ID, object, then XYZ. That natural sequence and primary type reproduce
the DDS1 constructor's slot-address lifetime.

`FldActivationEntry` has one proven layout: `s32, s16, s16, s32, s32` at
`+0, +4, +6, +8, +0xC`. The bulk loaders, activation dispatcher and five
script selectors access those same fields. Earlier unions only provided
compatibility aliases (`selectedValue0/1`) and an unused opaque overlay;
there is no runtime variant. Use `kind` and `parameter` in the script reader,
not a union alias. Qualified ee-gcc traces show why this matters: the union's
alias-set-zero kind load gains false dependencies on placement stores,
raising its sched2 priority from 3 to 15. The plain record removes those
edges and restores the native orientation-copy order. All other consumers
in both games remain exact.

For DDS1 `func_001486D0`, keep the light angle in degrees and convert at each
trigonometric argument: `sdfSin(angle * 3.14f / 180.0f)` and the same expression
for `sdfCos`. This follows the paired HD routine's two `OOYRAD` uses. Do not
precompute a shared radians local merely because final assembly computes the
conversion once. Qualified plain-versus-dump compilation shows that direct
arguments keep conversion after the color call in sched1. The first green
shift then dies before the second is issued, so both anonymous temporaries
use `v0` and alpha uses `v1`. Sched2 hoists the conversion into the exact
native color block afterward. A precomputed or destructively converted
angle occupies an earlier sched1 issue slot, overlaps the green temporaries,
and leaves nine different words despite identical arithmetic. No artificial
color dependency, qualifier, mask or register binding is needed.

The DDS2 twin, `func_0014C9B0`, keeps those same owner and angle contracts.
Only areas 29/30 load the kind-0/3 effect and use model 23; elsewhere those
kinds use model 21 without writing the effect slot. Kind 1 shares one cache
but chooses the area-specific resource/model 17 or normal resource/model 22.
Read the palette floor through `fldGlobalBase.floor`, the same primary owner
as those area tests, rather than a separately declared interior word alias.

## Save/heal collision: preserve the actual mixed-argument API

DDS1 `func_00148FF0` shares the NPC record reset, quaternion placement and
palette-lighting contracts above, but appends an anonymous record (`999`),
chooses SAVE_UNIT/model 3 or HEAL_UNIT/model 4, and creates a radius-28,
height-400 collision box before its radius-160 room trigger.

The collision builder's source contract is five floats followed by the NPC
ID. The pinned SMT3 HD
[`fldHitProgramAdd` definition](https://github.com/smt-ret/smt3-reconstructed-sources/blob/cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1/src/fldHit.c#L7176-L7708)
and independent declaration/treasure and NPC calls in
[`fld_Npc.c`](https://github.com/smt-ret/smt3-reconstructed-sources/blob/cdcd3e5874d72e63afae6e771e3b2d8f85cf66b1/src/fld_Npc.c)
agree on `x, y, z, extent, height, id`. The providers share the eight-vertex,
four-face geometry, face indices/attributes, origin, generated identifier,
collision-registration arguments and count increment. The HD reconstruction
omits scalar vector-assignment operands, so DDS native remains the authority
for coordinate arithmetic.

The earlier ID-first DDS declaration preserved the runtime ABI because EE
integer and floating arguments use independent banks, but it changed the
save/heal call's evaluation schedule. Correct both provider definitions and
all callers through one shared header, not a target-only prototype. The
source-backed order puts the ID assignment in the native call delay slot;
both providers, both NPC constructors and both treasure constructors remain
exact. Do not infer a mixed-argument source order from register order alone.

The DDS2 save/heal twin `func_0014D380` uses extent 60 with the same height
400. Areas 29/30 select save/heal models 15/18; all other areas use 3/4.
Keep the palette-floor read on `fldGlobalBase.floor`, sharing its primary
owner with those area tests. The same constructors and source-backed
collision declaration reproduce all 1080 bytes without additional steering.

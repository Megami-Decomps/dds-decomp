#include "common.h"
#include "btl.h"
#include "eff_field_color.h"
#include "pcp_vu0.h"
#include "sdf_debug_api.h"

extern u32 effFieldColorFlags;

extern u32 effFieldColorOriginalSelector;

extern u32 effFieldColorVariantSelector;

extern u32 effFieldColorOverrideSelector;

extern u32 effFieldColorFinalSelector;

/* Keep the original selector and optional overrides separately for field-color lookups. */
void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId) {
    effFieldColorOverrideSelector = (u32)baseId;
    effFieldColorOriginalSelector = effFieldColorOverrideSelector;
    if (overrideId != 0) {
        effFieldColorOverrideSelector = (u32)overrideId;
    }
    if (finalId != 0) {
        baseId = finalId;
    }
    effFieldColorVariantSelector = variant;
    effFieldColorFinalSelector = (s32)baseId;
}

u32 effBTLFieldColorGetOriginalSelector(void) {
    return effFieldColorOriginalSelector;
}

u32 effBTLFieldColorGetVariantSelector(void) {
    return effFieldColorVariantSelector;
}

u32 effBTLFieldColorGetOverrideSelector(void) {
    return effFieldColorOverrideSelector;
}

u32 effBTLFieldColorGetFinalSelector(void) {
    return effFieldColorFinalSelector;
}

/* Default RGB adjustment used when no field-specific color is supplied. */
void effBTLFieldColorGetFixedVector(u32 unused, f32 *color) {
    color[0] = -0.73f;
    color[1] = 1.55f;
    color[2] = 0.24f;
}

extern f32 D_00350F60[][4];

extern f32 D_00352880[4];

extern EffFieldColorRecord D_0034E730[];

/* Scale the color record's RGB triple by the field color entry for this variant
 * and kind. The staging quadword's x element is multiplied but never read. */
void effBTLFieldColorGetBaseColor(s32 colorId, s16 variant, s16 kind, f32 *output) {
    f32 scaled[4];
    s16 index = variant - 200;

    if ((u16)index >= 40 || kind <= 0 || kind >= 11) {
        sdfDebugPrintf("effBTLFieldColorGetBaseColor:DummyColorLoad\n");
        VU0_LOAD_VF(vf10, D_00352880);
    } else {
        VU0_LOAD_VF(vf10, D_00350F60[(index * 10) + kind - 1]);
    }
    scaled[1] = D_0034E730[colorId].channelScales[1];
    scaled[2] = D_0034E730[colorId].channelScales[2];
    scaled[3] = D_0034E730[colorId].channelScales[3];
    VU0_LOAD_VF(vf11, scaled);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaled);
    output[0] = scaled[1];
    output[1] = scaled[2];
    output[2] = scaled[3];
}

/* Variant and kind are narrowed to their stored widths before the lookup. */
void effBTLFieldColorLookupNarrowSelectors(s32 colorId, s16 variant, s16 kind, f32 *output) {
    effBTLFieldColorGetBaseColor(colorId, variant, kind, output);
}

u32 effBTLFieldColorGetEntryWord(s32 index) {
    return D_0034E730[index].packedColor;
}

/* Enable actor-selector paths in effect consumers; retail always returns one. */
u32 effBTLFieldColorUsesActorSelectors(void) {
    return 1;
}

void effBTLFieldColorSetFlags(u32 flags) {
    effFieldColorFlags = effFieldColorFlags | flags;
}

void effBTLFieldColorClearFlags(u32 flags) {
    effFieldColorFlags = effFieldColorFlags & ~flags;
}

void effBTLFieldColorResetFlags(void) {
    effFieldColorFlags = 0;
}

f32 effBTLFieldColorGetActorScale(BtlUnit *unit) {
    f32 scale = unit->scale;
    f32 maximum = 3.0f;
    f32 radius = ((unit->reach * scale) + (unit->height * scale * 0.5f)) * 0.5f * (1.0f / 87.5f);

    if (maximum < radius) {
        radius = maximum;
    } else if (radius < 0.8f) {
        radius = 0.8f;
    }
    return radius;
}

f32 effGetActorScaleForParameterWork(BtlUnit *unit) {
    return effBTLFieldColorGetActorScale(unit);
}

INCLUDE_SDATA(const s32, "game/code_00161838", effFieldColorOriginalSelector);

INCLUDE_SDATA(const s32, "game/code_00161838", effFieldColorVariantSelector);

INCLUDE_SDATA(const s32, "game/code_00161838", effFieldColorOverrideSelector);

INCLUDE_SDATA(const s32, "game/code_00161838", effFieldColorFinalSelector);

INCLUDE_SDATA(const s32, "game/code_00161838", D_003BB040);


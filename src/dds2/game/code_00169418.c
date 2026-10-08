#include "common.h"
#include "btl.h"
#include "eff_field_color.h"

extern u32 effFieldColorOriginalSelector;

extern u32 effFieldColorVariantSelector;

extern u32 effFieldColorOverrideSelector;

extern u32 effFieldColorFinalSelector;

extern u32 effFieldColorFlags;

extern s32 effBTLFieldColorGetBaseColor(s32, s16, s16, s32);

/* Keep the original selector and optional overrides separately for field-color lookups. */
typedef struct Entry20B {
    u32 value;
    u8 pad_0x04[0x10];
} Entry20B;

extern Entry20B D_003AB070[];

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

INCLUDE_ASM(const s32, "game/code_00169418", effBTLFieldColorGetBaseColor);

/* Variant and kind are narrowed to their stored widths before the lookup. */
s32 effBTLFieldColorLookupNarrowSelectors(s32 colorId, s16 variant, s16 kind, s32 arg3) {
    return effBTLFieldColorGetBaseColor(colorId, variant, kind, arg3);
}

u32 effBTLFieldColorGetEntryWord(s32 index) {
    return D_003AB070[index].value;
}

u32 func_001695C8(void) {
    return 1;
}

void effBTLFieldColorSetFlags(u32 bits) {
    effFieldColorFlags = effFieldColorFlags | bits;
}

void effBTLFieldColorClearFlags(u32 bits) {
    effFieldColorFlags = effFieldColorFlags & ~bits;
}

u8 effBTLFieldColorTestFlags(u32 bits) {
    return (effFieldColorFlags & bits) != 0;
}

void effBTLFieldColorResetFlags(void) {
    effFieldColorFlags = 0;
}

f32 effBTLFieldColorGetActorScale(BtlUnit *unit) {
    f32 scale = unit->scale;
    f32 maximum = 3.0f;
    f32 radius = ((unit->reach * scale) + (unit->height * scale * 0.5f)) * 0.5f * (1.0f / 87.5f);

    switch (unit->partyRecord.unitId) {
    case 0x126:
    case 0x127:
        maximum = 4.0f;
        break;
    case 0x110:
    case 0x11D:
    case 0x11E:
    case 0x11F:
    case 0x120:
    case 0x121:
        maximum = 5.0f;
        break;
    }
    if (maximum < radius) {
        radius = maximum;
    } else if (radius < 0.8f) {
        radius = 0.8f;
    }
    return radius;
}

f32 func_001696B8(BtlUnit *unit) {
    return effBTLFieldColorGetActorScale(unit);
}

INCLUDE_SDATA(const s32, "game/code_00169418", effFieldColorFlags);

INCLUDE_SDATA(const s32, "game/code_00169418", effFieldColorOriginalSelector);

INCLUDE_SDATA(const s32, "game/code_00169418", effFieldColorVariantSelector);

INCLUDE_SDATA(const s32, "game/code_00169418", effFieldColorOverrideSelector);

INCLUDE_SDATA(const s32, "game/code_00169418", effFieldColorFinalSelector);

INCLUDE_SDATA(const s32, "game/code_00169418", D_00436430);


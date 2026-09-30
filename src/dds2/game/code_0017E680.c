#include "common.h"
#include "pcp_vu0.h"

typedef struct EffResourceEntry {
    f32 position[3];
    u8 pad0C[4];
    u32 value;
} EffResourceEntry;

typedef struct EffResourceWork {
    u8 pad00[0x40];
    EffResourceEntry *entries;
    u8 pad44[0x1C];
    u32 value60;
    u8 pad64[4];
    u32 resource68;
    u32 graphics6C;
    u32 resource70;
} EffResourceWork;

/* Billboard set allocated by func_0017ED78. */
typedef struct EffBillboardWork {
    s32 seed;   /* 0x00: copied from the parameter block */
    u8 pad04[0xC];
    u32 mode;   /* 0x10 */
    u32 color;  /* 0x14 */
    f32 scale;  /* 0x18 */
    u32 handle; /* 0x1C */
} EffBillboardWork;

/* Parameter block read by func_0017ED78. */
typedef struct Params {
    s32 seed; /* 0x00 */
} Params;

extern void *func_00328D68(s32 size);
extern u32 effRetainResource(s32 kind);
extern void billSetBillboardMode(u32 handle, s32 mode);

void func_0017E680(EffResourceWork *effect, u32 value) {
    effect->value60 = value;
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E688);

void effReleaseAttachedResources(u32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    sdfQueueAssetRelease(effect->graphics6C);
    effReleaseOptionalResource(address);
    func_003297C8(effect->resource70);
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017E7E0);

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017EA78);

void effReleaseOptionalResource(s32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    if (effect->resource68 != 0) {
        func_003297C8(effect->resource68);
        return;
    }
}

void effSetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + (s32)effect->entries);
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + (s32)effect->entries);
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(EffResourceWork *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

/* vu0 routine: copy a 0x40-byte matrix from src to dst through vf28-vf31 */
void func_0017ED50(void *dst, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(dst);
}

/* Create the shared billboard set and seed its default colour and scale. */
EffBillboardWork *func_0017ED78(Params *src) {
    EffBillboardWork *billboard = (EffBillboardWork *)func_00328D68(0x20);

    billboard->mode = src->seed;
    billboard->handle = effRetainResource(2);
    billSetBillboardMode(billboard->handle, 2);
    billboard->color = 0x80808080;
    billboard->scale = 100.0f;
    return billboard;
}

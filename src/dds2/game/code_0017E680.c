#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EffResourceEntry {
    f32 position[3];
    u8 pad0C[4];
    u32 value;
} EffResourceEntry;

typedef struct EffResourceWork {
    u8 pad00[0x40];
    EffResourceEntry *entries;
    s32 count;   /* 0x44: entry count */
    s32 unk48;   /* 0x48: 2 on creation */
    f32 unk4C;   /* 0x4C: 1.0f on creation */
    f32 unk50;   /* 0x50: 1.0f on creation */
    f32 unk54;   /* 0x54: 1.0f on creation */
    u8 pad58[8];
    u32 value60;
    u8 pad64[4];
    u32 resource68;
    u32 graphics6C;
    u32 resource70;
} EffResourceWork;

/* Billboard set allocated by effCreateBillboardResourceWork. */
typedef struct EffBillboardWork {
    u8 pad00[0x10];
    u32 mode;   /* 0x10 */
    u32 color;  /* 0x14 */
    f32 scale;  /* 0x18 */
    u32 handle; /* 0x1C */
} EffBillboardWork;

/* Creation parameters read by effCreateBillboardResourceWork. */
typedef struct EffBillboardParams {
    s32 mode; /* 0x00: billboard mode for the new set */
} EffBillboardParams;

extern void *func_00328D68(s32 size);
extern u32 effRetainResource(s32 kind);
extern void billSetBillboardMode(u32 handle, s32 mode);
extern s32 func_003292A8(s32 size);
extern s32 sdfResourceRetainAddress(s32 handle);
extern void *sdfCreateAssetWithDrawEntries(void);
extern void func_003332D0(u32 asset, f32 value);
extern u8 D_00452080[];

void func_0017E680(EffResourceWork *effect, u32 value) {
    effect->value60 = value;
}

/* Create an effect resource work with index entries, its entry list inline at +0x74. */
EffResourceWork *func_0017E688(s32 index) {
    s32 handle;
    EffResourceWork *work;
    EffResourceEntry *entries;
    u32 asset;
    u32 i;

    handle = func_003292A8(index * 20 + 0x74);
    work = (EffResourceWork *)sdfResourceRetainAddress(handle);
    work->unk48 = 2;
    work->entries = (EffResourceEntry *)(work + 1);
    work->resource70 = handle;
    work->count = index;
    work->unk4C = 1.0f;
    work->unk50 = 1.0f;
    work->unk54 = 1.0f;
    work->resource68 = 0;
    EE_MMI_UNIT_MATRIX(work);
    asset = (u32)sdfCreateAssetWithDrawEntries();
    work->graphics6C = asset;
    func_003332D0(asset, 1.0f);
    entries = work->entries;
    i = 0;
    if (index != 0) {
        do {
            i++;
            entries->value = 0x80808080;
            entries++;
        } while (i < index);
    }
    memset(D_00452080, 0, 0x2C);
    *(u16 *)(D_00452080 + 4) = 0x4000;
    return work;
}

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

/* Create the shared billboard set with its default colour and scale. */
EffBillboardWork *effCreateBillboardResourceWork(EffBillboardParams *params) {
    EffBillboardWork *billboard = (EffBillboardWork *)func_00328D68(0x20);

    billboard->mode = params->mode;
    billboard->handle = effRetainResource(2);
    billSetBillboardMode(billboard->handle, 2);
    billboard->color = 0x80808080;
    billboard->scale = 100.0f;
    return billboard;
}

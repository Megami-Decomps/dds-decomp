#include "common.h"

extern s32 D_00438930;

extern u32 D_0043891C;

extern s32 func_00317FE0(u32);

extern u32 D_00438918;

extern s32 D_00435BB0;

extern s16 D_00435BAC;

extern u8 D_0040ABF0[];

extern void mdlLoadViewerPackage(s32 source, s32 destination, s32 flags, s32 packageId, s32 variant);

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern void mnuResumeEffectQueueFrameAdvance(void);

extern void func_00317AD0(u32 handle);

extern void *func_003292A8(s32 size);

extern u32 sdfMemoryGetBlockAddress(void *block);

u32 mdlAdvanceViewerPackageTask(void);

u8 *mdlAllocateViewerPackageWork(void);

/* 0xAARRGGBB color split into RGB and alpha fields. */
typedef struct RgbAlpha {
    u8 pad_0x00[0x18]; // 0x00
    u32 rgb;           // 0x18
    u8 pad_0x1C[0x1C]; // 0x1C
    u32 alpha;         // 0x38
    u32 x3C;           // 0x3C
    u8 pad_0x40[0x10]; // 0x40
    float f50;         // 0x50
} RgbAlpha; // 0x54

/* Copy source for func_002CF3F8 (layout inferred from field accesses). */
typedef struct CfSrc {
    u8 pad_0x00[0x04]; // 0x00
    u32 rgb;           // 0x04
    u8 pad_0x08[0x1C]; // 0x08
    u32 alpha;         // 0x24
    u32 x28;           // 0x28
    u8 pad_0x2C[0x10]; // 0x2C
    float f3C;         // 0x3C
} CfSrc; // 0x40

extern char D_0042D4D0[];

extern s32 func_00101740(const char *arg0);

void func_00316FA8(u32 sprite);

void itfSetPackedRgbAlpha(RgbAlpha *entry, u32 color) {
    entry->rgb = color & 0xFFFFFF;
    entry->alpha = color >> 24;
}

u32 func_00316E28(RgbAlpha *entry) {
    return entry->x3C;
}

void func_00316E30(RgbAlpha *entry, u32 value) {
    entry->x3C = value;
}

void itfCopyColorFields(RgbAlpha *dst, CfSrc *src) {
    dst->rgb = src->rgb;
    dst->f50 = src->f3C;
    dst->alpha = src->alpha;
    dst->x3C = src->x28;
}

void func_00316E60(void) {
    D_00438918 = 1;
}

void func_00316E70(void) {
    D_00438918 = 0;
}

void mdlCreateViewerPackageTask(void) {
    u32 handle;

    D_00435BB0 = 0;
    D_00435BAC = 1;
    handle = mdlAllocateViewerPackageWork();
    D_0043891C = handle;
    func_00317AD0(handle);
    kwlnTaskCreate((s32)D_0042D4D0, 0x2AF8, 0, 0, (s32)mdlAdvanceViewerPackageTask, 0, 0);
}

u8 func_00316ED0(void) {
    return func_00101740(D_0042D4D0) != 0;
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00316EF8);

u8 *mdlAllocateViewerPackageWork(void) {
    void *block = func_003292A8(0x1E0);
    u8 *work = (u8 *)sdfMemoryGetBlockAddress(block);

    memset(work, 0, 0x1E0);
    *(u32 *)(work + 0x0) = (u32)block;
    *(u32 *)(work + 0x68) = 0;
    *(u16 *)(work + 0x1D8) = 0x80;
    *(u16 *)(work + 0x96) = 0;
    *(u32 *)(work + 0x74) = 0;
    return work;
}

void func_00316FA8(u32 sprite) {
    if (sprite != 0) {
        func_00317E48(sprite);
    }
}

u32 mdlAdvanceViewerPackageTask(void) {
    u32 result;
    s64 status;

    status = func_00317FE0(D_0043891C);
    if (status == -1) {
        func_00128658();
        result = 0xffffffff;
    }
    else {
        func_00318068(D_0043891C);
        result = 0;
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4D0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317010);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317058);

void mdlLoadViewerPackageFromWork(u8 *work) {
    mdlLoadViewerPackage(5, *(u16 *)(work + 0x12), 0x101, *(s32 *)(work + 0xC), *(s32 *)(work + 0x1C));
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317988);

u32 func_00317AC8(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D7A0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317AD0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317E48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317FE0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318068);

INCLUDE_ASM(const s32, "game/code_00316E08", func_003180B8);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318570);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318660);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318C00);

INCLUDE_ASM(const s32, "game/code_00316E08", func_003191B0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319388);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319A58);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319E48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319F48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319FF0);

void func_0031A090(void) {
    if ((D_00438930 != 0) && ((*(u32 *)(D_00438930 + 4) & 1) != 0)) {
        *(u32 *)(D_00438930 + 4) = *(u32 *)(D_00438930 + 4) | 0x800;
    }
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_0031A0B8);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043891C);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438920);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438928);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438930);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438934);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438938);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438940);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438944);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438948);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043894C);


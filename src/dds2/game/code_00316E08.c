#include "common.h"

typedef struct MenuWorkEntry {
    u8 pad00[4];
    u32 tag;
    s32 unk08;
    u8 pad0C[4];
    f32 x0;
    f32 y0;
    f32 scale0;
    u8 pad1C[4];
    f32 x1;
    f32 y1;
    f32 scale1;
    u8 pad2C[8];
    u16 unk34;
    u16 remaining;
    u8 pad38[4];
    u32 callback;
    u32 flags;
    u8 pad44[4];
} MenuWorkEntry;

typedef struct MenuProgressParameters {
    u32 word00;
    u32 word04;
    s32 x;
    s32 y;
} MenuProgressParameters;

/* Same effect-list layout produced by mnuCreateEffectWork in code_0031B188. */
typedef struct MnuEffectList {
    u8 *records;
    s32 count;
} MnuEffectList;

typedef struct MnuEffectWork {
    u32 handle;
    s32 count;
    MnuEffectList *lists;
    u32 unkC;
} MnuEffectWork;

/* Effect records have a 0x20-byte stride; bit 0 marks an occupied record. */
typedef struct MnuEffectRecord {
    u8 pad00[4];
    u32 flags;
    u8 pad08[0x18];
} MnuEffectRecord;

typedef struct SdfAllocation SdfAllocation;

typedef struct MnuSectionObjectList {
    SdfAllocation *allocation;
    s32 count;
    u8 *objects;
} MnuSectionObjectList;

typedef struct MnuSectionModelWork {
    SdfAllocation *allocation;
    s32 count;
    s32 (*groups)[4];
    u32 unkC;
} MnuSectionModelWork;

/* Package entries are walked through their first-word link by the loader. */
typedef struct MnuPackageEntry {
    struct MnuPackageEntry *next;
    u8 pad04[4];
    SdfAllocation *allocation;
    s32 unk0C;
    u8 kind;
    u8 pad11;
    u16 unk12;
    u8 pad14[8];
    s32 unk1C;
} MnuPackageEntry;

/* Shared work for the shooting task's package requests and state callbacks. */
typedef struct MnuShootingWork {
    u32 unk00; /* Written with the allocation handle; consumption not identified. */
    MnuSectionObjectList *objects;
    MnuSectionObjectList *playerObjects;
    MnuSectionObjectList *mapObjects;
    MnuSectionObjectList *drawObjects;
    u8 pad14[8];
    MnuSectionModelWork *modelWork;
    MnuEffectWork *effectWork;
    u8 pad24[0x34];
    s32 state;
    u8 pad5C[4];
    s32 (*update)(u8 *work);
    u8 pad64[4];
    u32 unk68;
    u8 pad6C[8];
    u32 unk74;
    u8 pad78[0x1E];
    u16 unk96;
    u8 pad98[0x140];
    u16 unk1D8;
    u8 pad1DA[6];
} MnuShootingWork;

extern MenuWorkEntry D_0040ABF8;
extern void mnuDeactivateWorkEntry(MenuWorkEntry *);
extern void sdfReleaseResourceAllocation(SdfAllocation *);
extern void mnuDestroyAllModelNodeContexts(s32 *);
extern u8 *mnuGetResourceProgressParameters(void);
extern f32 mnuEvaluateTimedValue(MenuWorkEntry *);
extern void func_0031CAE8(f32 *, s32, s32);
extern s32 func_0031B838(void *, void *, s32, f32, f32, f32, f32);
extern void func_00319FF0(void);

extern MnuEffectRecord *D_00438930;

extern MnuShootingWork *D_0043891C;

extern s32 func_00317FE0(MnuShootingWork *);

extern u32 D_00438918;

extern s32 D_00435BB0;

extern s16 mnuMovieTaskState;

extern s32 (*D_0040ABF0[])(u8 *work);

extern void mdlLoadViewerPackage(s32 source, s32 destination, s32 flags, s32 packageId, s32 variant);

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u32 mnuResumeEffectQueueFrameAdvance(void);

extern void func_00317AD0(MnuShootingWork *handle);

extern void *sdfAllocGeneralBlock(s32 size);

extern void *sdfMemoryGetBlockAddress(void *block);

u32 mdlAdvanceViewerPackageTask(void);

MnuShootingWork *mdlAllocateViewerPackageWork(void);

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
    MnuShootingWork *handle;

    D_00435BB0 = 0;
    mnuMovieTaskState = 1;
    handle = mdlAllocateViewerPackageWork();
    D_0043891C = handle;
    func_00317AD0(handle);
    kwlnTaskCreate((s32)D_0042D4D0, 0x2AF8, 0, 0, (s32)mdlAdvanceViewerPackageTask, 0, 0);
}

u8 func_00316ED0(void) {
    return func_00101740(D_0042D4D0) != 0;
}

u32 func_00316EF8(void) {
    mnuMovieTaskState = 2;
    D_00435BB0 = 1;
    func_00316FA8((u32)D_0043891C);
    kwlnTaskDestroyWithHierarchyByName(D_0042D4D0, 1);
    return mnuResumeEffectQueueFrameAdvance();
}

/* Return cleared 0x1E0-byte task work with its allocation handle and defaults. */
MnuShootingWork *mdlAllocateViewerPackageWork(void) {
    void *block = sdfAllocGeneralBlock(0x1E0);
    MnuShootingWork *work = (MnuShootingWork *)sdfMemoryGetBlockAddress(block);

    memset(work, 0, 0x1E0);
    work->unk00 = (u32)block;
    work->unk68 = 0;
    work->unk1D8 = 0x80;
    work->unk96 = 0;
    work->unk74 = 0;
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
        fldDispatchDeferredFieldCommand();
        result = 0xffffffff;
    }
    else {
        func_00318068(D_0043891C);
        result = 0;
    }
    return result;
}

extern char D_0042D4E0[];
extern void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern void dds3AdminSubmitModeRequest(s32 a0, s32 a1, s32 a2, s32 a3);
extern void scrDestroyAllNamedProcesses(void);

s32 evtCallShooting(void) {
    evtPrintDeveloperConsoleMessage(D_0042D4E0, 0);
    dds3AdminSubmitModeRequest(0x1D, 0, 0, 0);
    scrDestroyAllNamedProcesses();
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4D0);

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D4E0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317058);

/* Forward the stored package-request words without interpreting their roles. */
void mdlLoadViewerPackageFromWork(MnuPackageEntry *entry) {
    mdlLoadViewerPackage(5, entry->unk12, 0x101, entry->unk0C, entry->unk1C);
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317988);

u32 func_00317AC8(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00316E08", D_0042D7A0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317AD0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317E48);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00317FE0);
/* Run the current update; on a nonzero result, refresh it from the state table.
   The dispatch table keeps its original opaque byte-work callback signature. */
u32 func_00318068(MnuShootingWork *work) {
    s32 (*update)(u8 *work) = work->update;

    if (update((u8 *)work) != 0) {
        work->update = D_0040ABF0[work->state];
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_003180B8);

void func_00318570(MnuShootingWork *work) {
    s32 i;

    mnuDeactivateWorkEntry(&D_0040ABF8);
    if (work->mapObjects != NULL) {
        sdfReleaseResourceAllocation(work->mapObjects->allocation);
        work->mapObjects = NULL;
    }
    if (work->drawObjects != NULL) {
        sdfReleaseResourceAllocation(work->drawObjects->allocation);
        work->drawObjects = NULL;
    }
    if (work->objects != NULL) {
        sdfReleaseResourceAllocation(work->objects->allocation);
        work->objects = NULL;
    }
    if (work->playerObjects != NULL) {
        sdfReleaseResourceAllocation(work->playerObjects->allocation);
        work->playerObjects = NULL;
    }
    if (work->modelWork != NULL) {
        for (i = 0; i < work->modelWork->count; i++) {
            mnuDestroyAllModelNodeContexts(work->modelWork->groups[i]);
        }
        sdfReleaseResourceAllocation(work->modelWork->allocation);
        work->modelWork = NULL;
    }
    evtPrintDeveloperConsoleMessage("stgRelease_Section !!\n");
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318660);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00318C00);

INCLUDE_ASM(const s32, "game/code_00316E08", func_003191B0);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319388);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319A58);

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319E48);

/* Acquire a record from the third effect list at the timed UI position. */
void func_00319F48(void) {
    MenuProgressParameters *origin;
    s32 y, x;
    f32 value;
    f32 position[4];
    MnuEffectList *lists;

    origin = (MenuProgressParameters *)mnuGetResourceProgressParameters();
    x = origin->x;
    y = origin->y;
    value = mnuEvaluateTimedValue(&D_0040ABF8);
    func_0031CAE8(position, (s32)D_0040ABF8.x0 + x, (s32)value + y - 32);
    lists = D_0043891C->effectWork->lists;
    D_00438930 = (MnuEffectRecord *)func_0031B838(lists + 2, NULL, 0,
                                               position[0], position[1], position[2], 0.5f);
    func_00319FF0();
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_00319FF0);

/* Mark the occupied record with flag 0x800; the flag's meaning is unresolved. */
void func_0031A090(void) {
    if ((D_00438930 != 0) && ((D_00438930->flags & 1) != 0)) {
        D_00438930->flags = D_00438930->flags | 0x800;
    }
}

INCLUDE_ASM(const s32, "game/code_00316E08", func_0031A0B8);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043891C);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438920);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438928);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438930);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438934);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438938);

INCLUDE_SDATA(const s32, "game/code_00316E08", dds3SoundSlotPool);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438944);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_00438948);

INCLUDE_SDATA(const s32, "game/code_00316E08", D_0043894C);


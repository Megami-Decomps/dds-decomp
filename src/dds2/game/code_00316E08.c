#include "common.h"
#include "mnu_work.h"
#include "sdf.h"
#include "mdl.h"
#include "itf.h"
#include "mnu_shooting.h"





/* The record payload follows this complete 0x10-byte descriptor. */
typedef struct MnuSectionObjectList {
    SdfMemBlock *allocation;
    s32 count;
    u8 *objects;
    u8 pad0C[4];
} MnuSectionObjectList;


/* Package entries are walked through their first-word link by the loader. */
typedef struct MnuPackageEntry {
    struct MnuPackageEntry *next;
    u8 pad04[4];
    SdfMemBlock *allocation;
    s32 unk0C;
    u8 kind;
    u8 pad11;
    u16 unk12;
    u8 pad14[8];
    s32 unk1C;
} MnuPackageEntry;


extern MenuWorkEntry D_0040ABF8;
extern void mnuDeactivateWorkEntry(MenuWorkEntry *);
extern void sdfReleaseResourceAllocation(SdfMemBlock *);
extern void mnuDestroyAllModelNodeContexts(MnuNodeList *);
extern f32 mnuEvaluateTimedValue(MenuWorkEntry *);
extern void func_0031CAE8(f32 *, s32, s32);
extern void fileQueueSetPosition(struct FileQueue *, void *);
extern void mnuUpdateTimedEffectPosition(void);

typedef struct SoundSlot SoundSlot;
extern SoundSlot *dds3ClaimSoundSlot(u32, u32);
extern void mdlAddEntryFlaggedEx(MdlCtx *, s32, s32, f32, f32);
extern void sdfMotionSampleAtFrame(Motion *, f32);
extern s32 D_00438934;
void func_00319F48(void);

extern MnuEffectRecord *D_00438930;

extern MnuShootingWork *D_0043891C;

extern s32 func_00317FE0(MnuShootingWork *);
extern void func_00318570(MnuShootingWork *);
extern void mnuDestroyShootingWork(MnuShootingWork *);

extern u32 D_00438918;

extern s32 D_00435BB0;

extern s16 mnuMovieTaskState;

extern s32 (*D_0040ABF0[])(u8 *work);
extern s32 (*D_0040ABE0[])(u8 *work);

extern void mdlLoadViewerPackage(s32 source, s32 destination, s32 flags, s32 packageId, s32 variant);

extern void kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u32 mnuResumeEffectQueueFrameAdvance(void);

extern void func_00317AD0(MnuShootingWork *handle);

extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);

extern u32 sdfMemoryGetBlockAddress(SdfMemBlock *block);
extern void func_0031D928(struct WideSlotPool *);
extern void func_0031DF48(struct CompactSlotPool *);
extern void dds3ReleaseSoundSlotPool(void);
extern SdfMemBlock *D_00438948;
extern void mnuDestroyNodeJobQueues(s32 *);
extern void mnuUpdateHighScoreFlag(MnuShootingWork *);
/* Retail passes the task work to this otherwise empty legacy callback. */
extern void func_0031AF60();
extern void itfClearTintAndWorkBuffers(u8 *);
extern u32 effDestroyResourceSlotSet(u32);
extern u32 D_00435CBC;
extern void *dds3GetWorldSecondaryObject(void);
extern void dds3DestroyWorldNode(void *);
extern void mdlFlagSet(s32);
extern void mdlFlagClear(s32);
extern void mnuMarkTitleStreamResetPending(void);
extern void mnuResetTitleStreamLocked(void);

u32 mdlAdvanceViewerPackageTask(void);

MnuShootingWork *mdlAllocateViewerPackageWork(void);


extern char D_0042D4D0[];

extern s32 kwlnTaskGetTaskByName(const char *arg0);

void func_00316FA8(MnuShootingWork *work);

void itfSetPackedRgbAlpha(SdfFlagListWork *entry, u32 color) {
    entry->params.color.colorA = color & 0xFFFFFF;
    entry->params.alpha.alpha = color >> 24;
}

u32 func_00316E28(SdfFlagListWork *entry) {
    return entry->params.alpha.surfaceIndex;
}

void func_00316E30(SdfFlagListWork *entry, u32 value) {
    entry->params.alpha.surfaceIndex = value;
}

void itfCopyColorFields(SdfFlagListWork *dst, const SdfFlagListParams *src) {
    dst->params.color.colorA = src->color.colorA;
    dst->params.speed = src->speed;
    dst->params.alpha.alpha = src->alpha.alpha;
    dst->params.alpha.surfaceIndex = src->alpha.surfaceIndex;
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
    return kwlnTaskGetTaskByName(D_0042D4D0) != 0;
}

u32 func_00316EF8(void) {
    mnuMovieTaskState = 2;
    D_00435BB0 = 1;
    func_00316FA8(D_0043891C);
    kwlnTaskDestroyWithHierarchyByName(D_0042D4D0, 1);
    return mnuResumeEffectQueueFrameAdvance();
}

/* Return cleared 0x1E0-byte task work with its allocation handle and defaults. */
MnuShootingWork *mdlAllocateViewerPackageWork(void) {
    SdfMemBlock *block = sdfAllocGeneralBlock(0x1E0);
    MnuShootingWork *work = (MnuShootingWork *)sdfMemoryGetBlockAddress(block);

    memset(work, 0, 0x1E0);
    work->allocation = block;
    work->unk68 = 0;
    work->unk1D8 = 0x80;
    work->round = 0;
    work->currentScore = 0;
    return work;
}

void func_00316FA8(MnuShootingWork *work) {
    if (work != NULL) {
        mnuDestroyShootingWork(work);
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

/* Release the shooting task's owned lists, queues, slots and world object. */
void mnuDestroyShootingWork(MnuShootingWork *work) {
    s32 i;
    u32 *slot;

    func_0031D928(work->spriteWork);
    func_0031DF48(work->tintWork);
    dds3ReleaseSoundSlotPool();
    if (D_00438948 != NULL) {
        sdfReleaseResourceAllocation(D_00438948);
        D_00438948 = NULL;
    }
    if (work->progressWork != NULL) {
        sdfReleaseResourceAllocation(work->progressWork->allocation);
        work->progressWork = NULL;
    }
    if (work->alternateProgressWork != NULL) {
        sdfReleaseResourceAllocation(work->alternateProgressWork->allocation);
        work->alternateProgressWork = NULL;
    }
    if (work->effectWork != NULL) {
        for (i = 0; i < work->effectWork->count; i++) {
            mnuDestroyNodeJobQueues((s32 *)&work->effectWork->lists[i]);
        }
        sdfReleaseResourceAllocation((SdfMemBlock *)work->effectWork->handle);
        work->effectWork = NULL;
    }
    if (work->work24 != NULL) {
        sdfReleaseResourceAllocation(work->work24->allocation);
        work->work24 = NULL;
    }
    slot = work->resourceSlots;
    mnuUpdateHighScoreFlag(work);
    func_00318570(work);
    func_0031AF60(work);
    itfClearTintAndWorkBuffers((u8 *)work);
    for (i = 6; i >= 0; i--, slot++) {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
            *slot = 0;
        }
    }
    D_00435CBC = 0x80000000;
    dds3DestroyWorldNode(dds3GetWorldSecondaryObject());
    if (work->round >= 3) {
        mdlFlagSet(0x849);
    } else {
        mdlFlagClear(0x849);
    }
    sdfReleaseResourceAllocation(work->allocation);
    mnuMarkTitleStreamResetPending();
    mnuResetTitleStreamLocked();
}

/* Advance initialization; completing state zero releases the section resources. */
s32 func_00317FE0(MnuShootingWork *work) {
    s32 state;

    if (work->initialize((u8 *)work) != 0) {
        state = work->state;
        if (state == 0) {
            func_00318570(work);
            state = work->state;
            work->initialized = 1;
            work->update = D_0040ABF0[0];
        }
        if (state == -1) {
            return -1;
        }
        work->initialize = D_0040ABE0[state];
    }
    return 1;
}

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
            mnuDestroyAllModelNodeContexts(&work->modelWork->groups[i]);
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

void func_00319E48(MenuRuntimeRecord *record) {
    MnuModelNode *node;

    if ((record->state.bytes[1] & 0xF0) == 0x40) {
        if (record->state.bytes[1] == 0x44) {
            dds3ClaimSoundSlot(0x1E00001, 0);
        } else if ((record->state.word & 0x1E) == 4 ||
                   (record->state.word & 0x1E) == 6) {
            dds3ClaimSoundSlot(0x1E00000, 0);
        }
    }
    node = D_0040ABF8.object.modelNode;
    if (record->state.bytes[1] == 0x44) {
        node->model->first->frameStep = 1.0f;
        node->savedModelValue = 1.0f;
        mdlAddEntryFlaggedEx(node->model, 0, 2, 0.0f, 0.0f);
        sdfMotionSampleAtFrame(node->model->first, 20.0f);
        D_00438934 = node->model->first->frameCount - 20;
        evtPrintDeveloperConsoleMessage("ModelFrame [%d]\n", D_00438934);
        func_00319F48();
    }
}

/* Acquire a record from the third effect list at the timed UI position. */
void func_00319F48(void) {
    MenuProgressParameters *origin;
    s32 y, x;
    f32 value;
    f32 position[4];
    MnuEffectList *lists;

    origin = mnuGetResourceProgressParameters();
    x = origin->x;
    y = origin->y;
    value = mnuEvaluateTimedValue(&D_0040ABF8);
    func_0031CAE8(position, (s32)D_0040ABF8.x0 + x, (s32)value + y - 32);
    lists = D_0043891C->effectWork->lists;
    D_00438930 = mnuClaimPositionedEffectRecord(lists + 2, NULL, 0,
                                               position[0], position[1], position[2], 0.5f);
    mnuUpdateTimedEffectPosition();
}

void mnuUpdateTimedEffectPosition(void) {
    MenuProgressParameters *origin;
    s32 y, x;
    f32 value;
    f32 position[4];

    if (D_00438930 != NULL && (D_00438930->flags & 0x801) == 1) {
        origin = mnuGetResourceProgressParameters();
        x = origin->x;
        y = origin->y;
        value = mnuEvaluateTimedValue(&D_0040ABF8);
        func_0031CAE8(position, (s32)D_0040ABF8.x0 + x, (s32)value + y - 32);
        fileQueueSetPosition(D_00438930->queue, position);
    }
}

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


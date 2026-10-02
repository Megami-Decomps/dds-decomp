#include "common.h"
#include "pcp_vu0.h"

typedef struct SoundSlot {
    u32 remainingFrames;
    u32 sequence;
} SoundSlot;

typedef struct SoundSlotPool {
    u32 handle;
    SoundSlot *slots;
    s32 count;
} SoundSlotPool;

/* Nodes passed to the menu model helpers are 0x50-byte records. */
typedef struct MnuModelNode {
    f32 primary[4];  /* 0x00 */
    f32 secondary[4]; /* 0x10 */
    f32 tertiary[4]; /* 0x20 */
    f32 x;           /* 0x30 */
    f32 y;           /* 0x34 */
    f32 z;           /* 0x38 */
    u32 positionFlag; /* 0x3C */
    u32 *model;      /* 0x40 */
    u32 flags;       /* 0x44 */
    u16 value48;     /* 0x48 */
    u16 value4A;     /* 0x4A */
    f32 modelZ;      /* 0x4C */
} MnuModelNode;

typedef struct MnuNodeList {
    MnuModelNode *nodes; /* 0x00 */
    s32 count;           /* 0x04 */
} MnuNodeList;

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

extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void mdlBroadcastMasked();
extern void mdlStorePrimaryVectorVU(void *model);
extern void mdlStoreTertiaryVectorVU(void *model);

extern u32 *dds3SoundSlotPool;

extern u32 mdlGetBroadcastValue(u32 model);

extern void func_00328160(f32 *out);

extern void mdlUpdateContextRotationBasisFromQuaternion(u32 model);

void mnuClearNodeBroadcastFlag(u8 *node);
void dds3ReleaseSoundSlotPool(void);

void mnuDeactivateModelNode(s32 node);

extern u8 *func_00232198(s32 first, s32 second);
extern void mdlAddEntryFlaggedEx(u8 *model, s32 entry, s32 flags, f32 x, f32 y);
extern u32 func_003292A8(s32 bytes);
extern u32 *sdfMemoryGetBlockAddress(u32 handle);

extern u8 D_0040ABD0[];
extern u8 D_0040ABC0[];
extern u8 D_0040ABB0[];
extern s32 D_00438944;
extern void dds3SetCameraVector(s32 object, void *vector);
extern void effObjSetInnerFirstVec(void *node, u128 *vector);

void func_0031B188(void) {
    u8 *object;
    void (*update)(void *);
    u8 *data;

    dds3SetCameraVector(D_00438944, D_0040ABD0);
    effObjSetInnerFirstVec((void *)D_00438944, (u128 *)D_0040ABC0);
    object = (u8 *)D_00438944;
    update = *(void (**)(void *))(*(u8 **)(object + 0x10) + 8);
    update(object);
    data = *(u8 **)((u8 *)D_00438944 + 0x18);
    PCP_COPY_VECTOR(data + 0x50, D_0040ABB0);
    PCP_COPY_VECTOR(data + 0x70, D_0040ABB0);
}

void dds3InitSoundSlotPool(void) {
    u32 handle;
    SoundSlotPool *pool;
    if (dds3SoundSlotPool != 0) {
        dds3ReleaseSoundSlotPool();
    }
    handle = func_003292A8(0x32c);
    dds3SoundSlotPool = sdfMemoryGetBlockAddress(handle);
    memset(dds3SoundSlotPool, 0, 0x32c);
    pool = (SoundSlotPool *)dds3SoundSlotPool;
    pool->handle = handle;
    pool->slots = (SoundSlot *)(pool + 1);
    pool->count = 100;
}

void dds3ReleaseSoundSlotPool(void) {
    if (dds3SoundSlotPool != (u32 *)0x0) {
        func_003297C8(*dds3SoundSlotPool);
        dds3SoundSlotPool = (u32 *)0x0;
    }
}

SoundSlot *sndFindFreeSoundSlot(void) {
    SoundSlotPool *pool = (SoundSlotPool *)dds3SoundSlotPool;
    SoundSlot *slot = pool->slots;
    s32 i;

    for (i = 0; i < pool->count; i++, slot++) {
        if (slot->sequence == 0) {
            return slot;
        }
    }
    return 0;
}

SoundSlot *sndClaimFreeSoundSlot(u32 sequence, u32 frames) {
    SoundSlot *slot = sndFindFreeSoundSlot();

    if (slot != NULL) {
        slot->sequence = sequence;
        slot->remainingFrames = frames;
    }
    return slot;
}

/* Returns occupied sound slots to their default volume and pan when they expire. */
void dds3UpdateSoundSlots(void) {
    SoundSlotPool *pool = (SoundSlotPool *)dds3SoundSlotPool;
    if (pool != 0) {
        s32 index = 0;
        SoundSlot *slot = pool->slots;
        if (pool->count > 0) {
            do {
                if (slot->sequence != 0) {
                    if (slot->remainingFrames == 0 || --slot->remainingFrames == 0) {
                        sndSetSequenceVolumePan(slot->sequence, 0x7f, 0x3f);
                        slot->sequence = 0;
                    }
                }
                index++;
                slot++;
            } while (index < (s32)dds3SoundSlotPool[2]);
        }
    }
}

void func_0031B3B0(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 0;
}

void func_0031B3B8(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B3D0);

MnuEffectWork *func_0031B4F0(s32 count, s32 *counts) {
    s32 listBytes = count * 8;
    s32 size = listBytes + 16;
    s32 i;
    u32 handle;
    MnuEffectWork *work;
    MnuEffectList *list;
    u8 *records;

    for (i = 0; i < count; i++) size += counts[i] * 32;
    evtPrintDeveloperConsoleMessage("EffectWork Object Size %d\n", size);
    handle = func_003292A8(size);
    work = (MnuEffectWork *)sdfMemoryGetBlockAddress(handle);
    memset(work, 0, size);
    work->handle = handle;
    work->count = count;
    work->lists = (MnuEffectList *)(work + 1);
    list = work->lists;
    records = (u8 *)list + listBytes;
    for (i = 0; i < count; i++, list++) {
        list->records = records;
        list->count = counts[i];
        records += counts[i] * 32;
    }
    return work;
}

void mnuClearNodeRecords(s32 *list) {
    s32 record;
    u32 index;

    index = 0;
    record = *list;
    if (0 < list[1]) {
        do {
            memset(record, 0, 0x20);
            index = (index + 1) & 0xffff;
            record = record + 0x20;
        } while ((s32)index < list[1]);
    }
}

void mnuClearAllNodeBroadcastFlags(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            mnuClearNodeBroadcastFlag(node);
            node += 0x20;
            index++;
        } while (list[1] > index);
    }
}

void mnuDestroyNodeJobQueues(s32 *list) {
    u8 *node = (u8 *)list[0];
    s16 index = 0;

    if (list[1] > 0) {
        do {
            if (*(u32 *)node != 0) {
                fileQueueDestroy(*(u32 *)node);
            }
            node += 0x20;
            index++;
        } while (index < list[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B748);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B838);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B960);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BA28);

void mnuClearNodeBroadcastFlag(u8 *node) {
    *(u32 *)(node + 4) &= ~1U;
    fileQueueNotifyAllJobsComplete(*(u32 *)node);
}

/* Resolves a model from a resource and releases its temporary resource data. */

u32 mnuLoadNodeModelFromResource(u32 *owner, u32 resource) {
    u32 handle;
    u32 other;
    u32 data = sdfReadNamedResource(resource, &handle, &other);
    *owner = func_002D4138(handle);
    fileQueueNotifyAllJobsComplete(*owner);
    func_003297C8(data);
    return *owner;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BC10);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BDE8);

void mnuPauseEffectQueueFrameAdvance(void) {
    fileSetRenderFlag(2);
}


void mnuResumeEffectQueueFrameAdvance(void) {
    fileClearRenderFlag(2);
}


INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BFE0);

void mnuInitializeNodeTransforms(u32 *group, f32 x, f32 y, f32 z, f32 w) {
    u16 index = 0;
    u8 *node = (u8 *)group[0];
    if ((s32)group[1] > 0) {
        do {
            memset(node, 0, 0x50);
            ((MnuModelNode *)node)->tertiary[0] = x;
            index++;
            ((MnuModelNode *)node)->tertiary[1] = y;
            ((MnuModelNode *)node)->tertiary[2] = z;
            ((MnuModelNode *)node)->tertiary[3] = w;
            node += 0x50;
        } while (index < (s32)group[1]);
    }
}

void mnuDeactivateAllModelNodes(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            mnuDeactivateModelNode((s32)node);
            node += 0x50;
            index++;
        } while (index < list[1]);
    }
}

void mnuDestroyAllModelNodeContexts(s32 *list) {
    u8 *node = (u8 *)list[0];
    s16 index = 0;
    if (list[1] > 0) {
        do {
            u32 model = (u32)((MnuModelNode *)node)->model;
            node += 0x50;
            if (model != 0) {
                mdlDestroyContext(model);
            }
            index++;
        } while (index < list[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C280);

/* Claims the first inactive node whose model pointer is already populated. */
u8 *mnuAcquireUnusedModelNode(u32 *group) {
    s32 index = 0;
    u8 *node = (u8 *)group[0];
    if ((s32)group[1] > 0) {
        do {
            MnuModelNode *entry = (MnuModelNode *)node;
            if ((entry->flags & 1) == 0) {
                u32 *model = entry->model;
                if (model != 0) {
                    *model &= ~1U;
                    entry->value48 = 0;
                    entry->value4A = 0;
                    entry->flags = 1;
                    return node;
                }
                return 0;
            }
            node += 0x50;
            index++;
        } while (index < (s32)group[1]);
    }
    return 0;
}

void mnuSetActiveNodeModelVisibility(s32 *list, s8 value) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;

    if (list[1] > 0) {
        do {
            u32 active = ((MnuModelNode *)node)->flags & 1;

            if (active == 1) {
                mnuSetModelNodeVisibility(node, value);
            }
            index++;
            node += 0x50;
        } while (index < list[1]);
    }
}

/* Set the model Z of every active node. */
void mnuOverrideActiveNodeModelDepth(MnuNodeList *list, f32 z) {
    MnuModelNode *node = list->nodes;
    s32 i;

    for (i = 0; i < list->count; i++) {
        u32 active = node->flags & 1;

        if (active == 1) {
            *(f32 *)(*(u8 **)((u8 *)node->model + 0x1c) + 0x20) = z;
        }
        node++;
    }
}

/* Restore each active node's model Z from its saved modelZ. */
void mnuRestoreActiveNodeModelDepth(MnuNodeList *list) {
    MnuModelNode *node = list->nodes;
    s32 i;

    for (i = 0; i < list->count; i++) {
        u32 active = node->flags & 1;

        if (active == 1) {
            *(f32 *)(*(u8 **)((u8 *)node->model + 0x1c) + 0x20) = node->modelZ;
        }
        node++;
    }
}

void mnuCreateNodeModelEntry(u8 *node, s32 first, s32 second, s32 flag, f32 x, f32 y, f32 z) {
    u8 *model = func_00232198(first, second);
    ((MnuModelNode *)node)->model = (u32 *)model;
    if (flag != -1) {
        *(f32 *)(*(u8 **)(model + 0x1c) + 0x20) = z;
        ((MnuModelNode *)node)->modelZ = z;
        mdlAddEntryFlaggedEx(model, 0, flag, x, y);
    }
}

void mnuDeactivateModelNode(s32 arg0) {
    MnuModelNode *node = (MnuModelNode *)arg0;
    node->flags = 0;
    *node->model = *node->model | 1;
}

void mnuSetNodePairValue(u8 *node, s32 value) {
    value &= 0xFFFF;
    ((MnuModelNode *)node)->value48 = value;
    ((MnuModelNode *)node)->value4A = value;
}

void mnuSetNodePosition(u8 *node, f32 x, f32 y, f32 z) {
    ((MnuModelNode *)node)->x = x;
    ((MnuModelNode *)node)->y = y;
    ((MnuModelNode *)node)->z = z;
    ((MnuModelNode *)node)->positionFlag = 0;
}

/* Set the primary (0x00) vector and load it into the model. */
void mnuSetNodePrimaryVector(u8 *node, f32 x, f32 y, f32 z) {
    MnuModelNode *n = (MnuModelNode *)node;

    n->primary[0] = x;
    n->primary[1] = y;
    n->primary[2] = z;
    n->primary[3] = 0;
    VU0_LOAD_VF(vf10, n->primary);
    mdlStorePrimaryVectorVU(n->model);
}

/* Translate the primary (0x00) vector and load it into the model. */
void mnuTranslateNodePrimaryVector(u8 *node, f32 x, f32 y, f32 z) {
    MnuModelNode *n = (MnuModelNode *)node;

    n->primary[3] = 0;
    n->primary[0] += x;
    n->primary[1] += y;
    n->primary[2] += z;
    VU0_LOAD_VF(vf10, n->primary);
    mdlStorePrimaryVectorVU(n->model);
}

void mnuRefreshNodeSecondaryVector(u8 *node) {
    f32 vec[4];

    func_00328160(vec);
    ((MnuModelNode *)node)->secondary[0] = vec[0];
    ((MnuModelNode *)node)->secondary[1] = vec[1];
    ((MnuModelNode *)node)->secondary[2] = vec[2];
    ((MnuModelNode *)node)->secondary[3] = vec[3];
    VU0_LOAD_VF(vf10, node + 0x10);
    mdlUpdateContextRotationBasisFromQuaternion((u32)((MnuModelNode *)node)->model);
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C688);

/* Fill the tertiary (0x20) vector with one value and load it into the model. */
void mnuSetNodeScaleVector(u8 *node, f32 value) {
    MnuModelNode *n = (MnuModelNode *)node;

    n->tertiary[0] = value;
    n->tertiary[1] = value;
    n->tertiary[2] = value;
    n->tertiary[3] = 0;
    VU0_LOAD_VF(vf10, n->tertiary);
    mdlStoreTertiaryVectorVU(n->model);
}

void mnuBroadcastNodeModelState(u8 *node) {
    mdlBroadcastMasked((u32)((MnuModelNode *)node)->model);
}


void mnuSetModelNodeBroadcastAlpha(void) {
}

void mnuSetNodeModelBroadcastByte(u8 *node, u8 value) {
    u32 broadcast = mdlGetBroadcastValue((u32)((MnuModelNode *)node)->model) & 0xFFFFFF;

    mdlBroadcastMasked((u32)((MnuModelNode *)node)->model, broadcast | ((u32)value << 24));
}

void mnuSetModelNodeVisibility(u8 *node, s8 selector) {
    MnuModelNode *entry = (MnuModelNode *)node;
    if (selector == 1) {
        *entry->model &= ~1U;
    } else {
        *entry->model |= 1U;
    }
}

INCLUDE_SDATA(const s32, "game/code_0031B188", D_00438950);


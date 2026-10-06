#include "common.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"

typedef struct SoundSlot {
    u32 remainingFrames;
    u32 sequence;
} SoundSlot;

typedef struct SoundSlotPool {
    u32 handle;
    SoundSlot *slots;
    s32 count;
} SoundSlotPool;

/* Model fields shared with the mdlManager context and node records. */

typedef struct MdlCtx {
    u32 flags;
    u8 pad04[0x18];
    Motion *first;
} MdlCtx;

/* Nodes passed to the menu model helpers are 0x50-byte records. */
typedef struct MnuModelNode {
    f32 primary[4];  /* 0x00 */
    f32 rotationQuaternion[4]; /* 0x10; supplied to the model basis update */
    f32 tertiary[4]; /* 0x20 */
    f32 x;           /* 0x30 */
    f32 y;           /* 0x34 */
    f32 z;           /* 0x38 */
    u32 positionFlag; /* 0x3C */
    MdlCtx *model;   /* 0x40 */
    u32 flags;       /* 0x44 */
    u16 value48;     /* 0x48 */
    u16 value4A;     /* 0x4A */
    f32 savedModelValue; /* 0x4C; restored to the model entry's scalar */
} MnuModelNode;

typedef struct MnuNodeList {
    MnuModelNode *nodes; /* 0x00 */
    s32 count;           /* 0x04 */
    u8 pad08[8];
} MnuNodeList;
typedef struct ShortRecord {
    u8 kind;
    u8 pad01;
    s16 parameters[3];
} ShortRecord;

typedef struct ShortRecordList {
    s32 count;
    ShortRecord *records;
} ShortRecordList;

typedef struct MenuRegistryRecord {
    u8 pad00[4];
    u16 firstCount;
    u16 secondCount;
    ShortRecordList *lists;
    ShortRecordList *secondLists;
} MenuRegistryRecord;

typedef struct MenuWorkEntry {
    u8 pad00[0xC];
    MnuModelNode *modelNode;
    u8 pad10[0x2A];
    u16 elapsed;
    u8 pad3C[0xC];
} MenuWorkEntry;

extern u32 mnuResolveTaggedRegistryRecord(u32 taggedRecord);
extern ShortRecord *func_003225C0(ShortRecordList *list);


typedef struct FileJob FileJob;
typedef struct FileQueue {
    f32 offset[4];
    f32 axis[4];
    u8 unk20[0x20];
    f32 position[4];
    f32 quat[4];
    f32 scale;
    u32 color;
    u32 transformWord;
    u8 pad6C[8];
    f32 transformValue;
    u8 pad78[8];
    s32 count;
    u32 unk84;
    FileJob *last;
    FileJob *first;
} FileQueue;

extern FileQueue *fileCloneQueueEntries(FileQueue *queue);

typedef struct MnuEffectPositionStep {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} MnuEffectPositionStep;

typedef struct MnuEffectRecord {
    FileQueue *queue;
    u32 flags;
    s32 delay;
    u32 unkC;
    MnuEffectPositionStep positionStep;
} MnuEffectRecord;

typedef struct MnuEffectList {
    MnuEffectRecord *records;
    s32 count;
} MnuEffectList;

typedef struct MnuEffectWork {
    u32 handle;
    s32 count;
    MnuEffectList *lists;
    u32 unkC;
} MnuEffectWork;

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

extern f32 sdfViewTargetVector[4];
extern void *memset(void *, s32, u32);
extern void func_00326BC8(SdfMat4 *, f32);
extern void func_003270C8(SdfMat4 *, f32);
extern void func_003275C8(SdfMat4 *, f32);
extern void sdfMat4Transpose(SdfMat4 *, SdfMat4 *);
extern void func_00327C80(f32 *, SdfMat4 *);
extern void fileReadVector40(void *, void *);
extern void fileQueueSetRotation(FileQueue *, void *);

static inline void mnuSetBasisRow(f32 *row, f32 x, f32 y, f32 z, f32 w) {
    row[0] = x;
    row[1] = y;
    row[2] = z;
    row[3] = w;
}


extern void evtPrintDeveloperConsoleMessage(const char *, ...);

extern void mdlBroadcastMasked();
extern void mdlStorePrimaryVectorVU(void *model);
extern void mdlStoreTertiaryVectorVU(void *model);

extern u32 *dds3SoundSlotPool;
extern u32 fileClearRenderFlag(u32 mask);

extern u32 mdlGetBroadcastValue(u32 model);

extern void func_00328160(f32 *out);

extern void mdlUpdateContextRotationBasisFromQuaternion(u32 model);


void mnuClearNodeBroadcastFlag(u8 *node);
void dds3ReleaseSoundSlotPool(void);
void mnuCreateNodeModelEntry(MnuModelNode *, s32, s32, s32, f32, f32, f32);

void mnuDeactivateModelNode(s32 nodeAddress);

extern u8 *func_00232198(s32 resourceGroup, s32 resourceId);
extern void mdlAddEntryFlaggedEx(MdlCtx *model, s32 searchId, s32 motionIndex, f32 blendLeadFrames, f32 blendDurationFrames);
extern u32 sdfAllocGeneralBlock(s32 bytes);
extern u32 *sdfMemoryGetBlockAddress(u32 handle);

extern u8 D_0040ABD0[];
extern u8 D_0040ABC0[];
extern u8 D_0040ABB0[];
extern s32 D_00438944;
extern void dds3SetCameraVector(s32 object, void *vector);
extern void effObjSetInnerFirstVec(void *node, u128 *vector);

/* Reload the camera vectors, invoke its update callback, then restore both copies
 * of the final vector in the camera data. */
void func_0031B188(void) {
    u8 *cameraObject;
    void (*updateCamera)(void *);
    u8 *cameraData;

    dds3SetCameraVector(D_00438944, D_0040ABD0);
    effObjSetInnerFirstVec((void *)D_00438944, (u128 *)D_0040ABC0);
    cameraObject = (u8 *)D_00438944;
    updateCamera = *(void (**)(void *))(*(u8 **)(cameraObject + 0x10) + 8);
    updateCamera(cameraObject);
    cameraData = *(u8 **)((u8 *)D_00438944 + 0x18);
    PCP_COPY_VECTOR(cameraData + 0x50, D_0040ABB0);
    PCP_COPY_VECTOR(cameraData + 0x70, D_0040ABB0);
}

void dds3InitSoundSlotPool(void) {
    u32 handle;
    SoundSlotPool *pool;
    if (dds3SoundSlotPool != 0) {
        dds3ReleaseSoundSlotPool();
    }
    handle = sdfAllocGeneralBlock(0x32c);
    dds3SoundSlotPool = sdfMemoryGetBlockAddress(handle);
    memset(dds3SoundSlotPool, 0, 0x32c);
    pool = (SoundSlotPool *)dds3SoundSlotPool;
    pool->handle = handle;
    pool->slots = (SoundSlot *)(pool + 1);
    pool->count = 100;
}

void dds3ReleaseSoundSlotPool(void) {
    if (dds3SoundSlotPool != (u32 *)0x0) {
        sdfReleaseResourceAllocation(*dds3SoundSlotPool);
        dds3SoundSlotPool = (u32 *)0x0;
    }
}

SoundSlot *dds3FindFreeSoundSlot(void) {
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

SoundSlot *dds3ClaimSoundSlot(u32 sequence, u32 frames) {
    SoundSlot *slot = dds3FindFreeSoundSlot();

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

void func_0031B3B0(s32 objectAddress) {
    *(u16 *)(objectAddress + 0x1da) = 0;
}

void func_0031B3B8(s32 objectAddress) {
    *(u16 *)(objectAddress + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

s32 mnuApplyFrameKeyedModelMotion(MenuWorkEntry *work) {
    MenuRegistryRecord *registry;
    ShortRecordList *list;
    ShortRecord *record;
    MnuModelNode *node;
    MdlCtx *model;
    s16 frame;
    s32 i;

    registry = (MenuRegistryRecord *)mnuResolveTaggedRegistryRecord((u32)work);
    if (registry != NULL) {
        if (registry->secondLists != NULL) {
            for (i = 0, list = registry->secondLists;
                 i < registry->secondCount; i++, list++) {
                record = NULL;
                if (list->records != NULL) {
                    record = func_003225C0(list);
                }
                if (record != NULL) {
                    frame = work->elapsed;
                    if (frame == record->parameters[0]) {
                        evtPrintDeveloperConsoleMessage("%d:%d:%d:%d\n", frame,
                            record->parameters[0], record->parameters[1],
                            record->parameters[2]);
                        node = work->modelNode;
                        if (node != NULL) {
                            model = node->model;
                            model->first->frameStep = 0.5f;
                            node->savedModelValue = 0.5f;
                            mdlAddEntryFlaggedEx(model, 0, record->parameters[1],
                                                0.0f, record->parameters[2]);
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Allocate the list descriptors and their 0x20-byte records as one work block. */
MnuEffectWork *mnuCreateEffectWork(s32 listCount, s32 *recordCounts) {
    s32 listBytes = listCount * 8;
    s32 allocationSize = listBytes + 16;
    s32 i;
    u32 handle;
    MnuEffectWork *work;
    MnuEffectList *list;
    MnuEffectRecord *records;

    for (i = 0; i < listCount; i++) allocationSize += recordCounts[i] * 32;
    evtPrintDeveloperConsoleMessage("EffectWork Object Size %d\n", allocationSize);
    handle = sdfAllocGeneralBlock(allocationSize);
    work = (MnuEffectWork *)sdfMemoryGetBlockAddress(handle);
    memset(work, 0, allocationSize);
    work->handle = handle;
    work->count = listCount;
    work->lists = (MnuEffectList *)(work + 1);
    list = work->lists;
    records = (MnuEffectRecord *)((u8 *)list + listBytes);
    for (i = 0; i < listCount; i++, list++) {
        list->records = records;
        list->count = recordCounts[i];
        records += recordCounts[i];
    }
    return work;
}

/* Clear each record while preserving the traversal's 16-bit index wrap. */
void mnuClearNodeRecords(s32 *listAddress) {
    MnuEffectList *list = (MnuEffectList *)listAddress;
    MnuEffectRecord *record;
    u32 index;

    index = 0;
    record = list->records;
    if (0 < list->count) {
        do {
            memset(record, 0, sizeof(*record));
            index = (index + 1) & 0xffff;
            record++;
        } while ((s32)index < list->count);
    }
}

void mnuClearAllNodeBroadcastFlags(s32 *listAddress) {
    MnuEffectList *list = (MnuEffectList *)listAddress;
    MnuEffectRecord *node = list->records;
    s32 index = 0;
    if (list->count > 0) {
        do {
            mnuClearNodeBroadcastFlag((u8 *)node);
            node++;
            index++;
        } while (list->count > index);
    }
}

void mnuDestroyNodeJobQueues(s32 *listAddress) {
    MnuEffectList *list = (MnuEffectList *)listAddress;
    MnuEffectRecord *node = list->records;
    s16 index = 0;

    if (list->count > 0) {
        do {
            if (node->queue != NULL) {
                fileQueueDestroy(node->queue);
            }
            node++;
            index++;
        } while (index < list->count);
    }
}

extern FileQueue *fileQueueClone(FileQueue *source);
extern char D_0040AD50[][64];
extern const char D_00438950[]; /* "%d:%s\n"; shared sdata in current build. */

/* Give the first record the source queue and clone it for subsequent records. */
void func_0031B748(MnuEffectList *list, s32 resourceIndex, FileQueue **sources) {
    FileQueue *source = NULL;
    MnuEffectRecord *record = list->records;
    s32 i;

    for (i = 0; i < list->count; i++, record++) {
        if (source == NULL) {
            source = sources[resourceIndex];
            record->queue = source;
            evtPrintDeveloperConsoleMessage(D_00438950, resourceIndex, D_0040AD50[resourceIndex]);
        } else {
            record->queue = fileQueueClone(source);
        }
        record->flags = (record->flags & ~0x1FE) | ((u8)resourceIndex << 1);
    }
}

extern void fileQueueSetPosition(FileQueue *queue, void *vector);
extern void fileQueueSetScale(FileQueue *queue, f32 scale);
extern void func_0031BC10(MnuEffectRecord *record, f32 xAngle, f32 yAngle, f32 zAngle);

/* Position-step vectors are copied verbatim; a missing queue aborts the claim. */
s32 mnuClaimPositionedEffectRecord(void *listAddress, void *stepAddress, s32 delay,
                  f32 x, f32 y, f32 z, f32 scale) {
    MnuEffectList *list = (MnuEffectList *)listAddress;
    f32 position[4];
    MnuEffectRecord *record;
    s32 index = 0;
    s32 count;

    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = 0.0f;
    count = list->count;
    record = list->records;
    for (; index < count; index++, record++) {
        if ((record->flags & 1) == 0) {
            if (record->queue != NULL) {
                fileQueueSetPosition(record->queue, position);
                fileQueueSetScale(record->queue, scale);
                func_0031BC10(record, 10.0f, 0.0f, 0.0f);
                record->flags |= 1;
                if (stepAddress != NULL) {
                    record->positionStep = *(MnuEffectPositionStep *)stepAddress;
                }
                record->delay = delay;
                record->flags = (record->flags | 0x200) & ~0x800;
                return (s32)record;
            }
            return 0;
        }
    }
    return 0;
}

MnuEffectRecord *mnuStartPositionedEffectRecord(MnuEffectList *list, s32 delay, f32 x, f32 y, f32 z) {
    f32 position[4];
    MnuEffectRecord *record;
    s32 index = 0;
    s32 count;
    u32 flags;

    position[0] = x;
    position[1] = y;
    position[2] = z;
    position[3] = 0.0f;
    count = list->count;
    record = list->records;
    for (; index < count; index++, record++) {
        if ((record->flags & 1) == 0) {
            if (record->queue != NULL) {
                fileQueueSetPosition(record->queue, position);
                fileQueueSetScale(record->queue, 0.2f);
                func_0031BC10(record, 10.0f, 0.0f, 0.0f);
                flags = record->flags;
                record->delay = delay;
                record->flags = flags | 1;
                return record;
            }
            return NULL;
        }
    }
    return NULL;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BA28);

void mnuClearNodeBroadcastFlag(u8 *node) {
    MnuEffectRecord *record = (MnuEffectRecord *)node;

    record->flags &= ~1U;
    fileQueueNotifyAllJobsComplete(record->queue);
}

/* Resolves a model from a resource and releases its temporary resource data. */

u32 mnuLoadNodeModelFromResource(u32 *owner, u32 resource) {
    u32 handle;
    u32 other;
    u32 data = sdfReadNamedResource(resource, &handle, &other);
    *owner = (u32)fileCloneQueueEntries((FileQueue *)handle);
    fileQueueNotifyAllJobsComplete(*owner);
    sdfReleaseResourceAllocation(data);
    return *owner;
}

/* Only the target direction is normalized; the lateral basis stays fixed at -X. */
void func_0031BC10(MnuEffectRecord *record, f32 xAngle, f32 yAngle, f32 zAngle) {
    f32 vector[4];
    f32 left[4];
    f32 up[4];
    f32 position[4];
    SdfMat4 basis;
    f32 quaternion[4];

    fileReadVector40(record->queue, position);
    mnuSetBasisRow(vector, sdfViewTargetVector[0] - position[0],
                   sdfViewTargetVector[1] - position[1],
                   sdfViewTargetVector[2] - position[2], 0.0f);
    mnuSetBasisRow(left, -1.0f, 0.0f, 0.0f, 0.0f);
    VU0_NORMALIZE_PACKED_VECTOR(vector);
    VU0_LOAD_VF(vf10, vector);
    VU0_LOAD_VF(vf11, left);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);

    memset(&basis, 0, sizeof(basis));
    mnuSetBasisRow(basis.m, left[0], left[1], left[2], 0.0f);
    mnuSetBasisRow(basis.m + 4, up[0], up[1], up[2], 0.0f);
    mnuSetBasisRow(basis.m + 8, vector[0], vector[1], vector[2], 0.0f);
    mnuSetBasisRow(basis.m + 12, 0.0f, 0.0f, 0.0f, 1.0f);
    func_00326BC8(&basis, xAngle);
    func_003270C8(&basis, yAngle);
    func_003275C8(&basis, zAngle);
    sdfMat4Transpose(&basis, &basis);
    func_00327C80(quaternion, &basis);
    vector[0] = quaternion[0];
    vector[1] = quaternion[1];
    vector[2] = quaternion[2];
    vector[3] = quaternion[3];
    fileQueueSetRotation(record->queue, vector);
}

extern void fileQueueUpdate(FileQueue *queue);
extern void func_002D49B8(FileQueue *queue, u32 color);
extern u32 D_0040AE10[];

/* Advance active effect queues, honoring their delay and optional position step. */
void mnuUpdateEffectQueues(MnuEffectWork *work, s32 flags) {
    f32 position[4];
    MnuEffectList *list = work->lists;
    MnuEffectRecord *record;
    s32 listIndex;
    s32 recordIndex;

    for (listIndex = 0; listIndex < work->count; listIndex++, list++) {
        record = list->records;
        for (recordIndex = 0; recordIndex < list->count; recordIndex++, record++) {
            if (record->flags & 1) {
                if (listIndex == 1) {
                    func_002D49B8(record->queue, 0x40808080);
                }
                if (flags & 1) {
                    if (record->delay == 0) {
                        fileQueueUpdate(record->queue);
                    }
                } else if (record->delay > 0) {
                    record->delay--;
                } else {
                    if ((record->flags >> 9) & 1) {
                        fileReadVector40(record->queue, position);
                        position[0] += record->positionStep.x;
                        position[1] += record->positionStep.y;
                        position[2] += record->positionStep.z;
                        position[3] += record->positionStep.w;
                        fileQueueSetPosition(record->queue, position);
                    }
                    fileQueueUpdate(record->queue);
                    if (record->queue->unk84 == D_0040AE10[(record->flags >> 1) & 0xFF] ||
                        ((record->flags >> 11) & 1)) {
                        mnuClearNodeBroadcastFlag((u8 *)record);
                    }
                }
            }
        }
    }
}

void mnuPauseEffectQueueFrameAdvance(void) {
    fileSetRenderFlag(2);
}


u32 mnuResumeEffectQueueFrameAdvance(void) {
    return fileClearRenderFlag(2);
}


typedef struct MnuModelWork {
    u32 handle;
    s32 count;
    MnuNodeList *lists;
    u32 unk0C;
} MnuModelWork;

extern char D_0042DAE8[];

MnuModelWork *func_0031BFE0(s32 listCount, s32 *nodeCounts) {
    s32 listBytes = listCount * sizeof(MnuNodeList);
    s32 allocationSize = listBytes + sizeof(MnuModelWork);
    s32 i;
    u32 handle;
    MnuModelWork *work;
    MnuNodeList *list;
    u8 *records;

    for (i = 0; i < listCount; i++) allocationSize += nodeCounts[i] * sizeof(MnuModelNode);
    evtPrintDeveloperConsoleMessage(D_0042DAE8, allocationSize);
    handle = sdfAllocGeneralBlock(allocationSize);
    work = (MnuModelWork *)sdfMemoryGetBlockAddress(handle);
    memset(work, 0, allocationSize);
    work->handle = handle;
    work->count = listCount;
    work->lists = (MnuNodeList *)(work + 1);
    list = work->lists;
    records = (u8 *)list + listBytes;
    for (i = 0; i < listCount; i++, list++) {
        list->count = nodeCounts[i];
        list->nodes = list->count != 0 ? (MnuModelNode *)records : NULL;
        records += nodeCounts[i] * sizeof(MnuModelNode);
    }
    return work;
}

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

void func_0031C280(MnuNodeList *list, s32 resourceGroup, s32 resourceId, s32 entryFlags, f32 x, f32 y, f32 z) {
    MnuModelNode *node = list->nodes;
    s32 i;

    for (i = 0; i < list->count; i++, node++) {
        mnuCreateNodeModelEntry(node, resourceGroup, resourceId, entryFlags, x, y, z);
    }
}

/* Claims the first inactive node whose model pointer is already populated. */
u8 *mnuAcquireUnusedModelNode(u32 *group) {
    s32 index = 0;
    u8 *node = (u8 *)group[0];
    if ((s32)group[1] > 0) {
        do {
            MnuModelNode *entry = (MnuModelNode *)node;
            if ((entry->flags & 1) == 0) {
                MdlCtx *model = entry->model;
                if (model != 0) {
                    model->flags &= ~1U;
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

/* Override the model-entry scalar of every active node without changing its saved value. */
void mnuOverrideActiveNodeModelDepth(MnuNodeList *list, f32 z) {
    MnuModelNode *node = list->nodes;
    s32 i;

    for (i = 0; i < list->count; i++) {
        u32 active = node->flags & 1;

        if (active == 1) {
            node->model->first->frameStep = z;
        }
        node++;
    }
}

/* Restore the model-entry scalar saved when each active node was initialized. */
void mnuRestoreActiveNodeModelDepth(MnuNodeList *list) {
    MnuModelNode *node = list->nodes;
    s32 i;

    for (i = 0; i < list->count; i++) {
        u32 active = node->flags & 1;

        if (active == 1) {
            node->model->first->frameStep = node->savedModelValue;
        }
        node++;
    }
}

/* Create the resource-backed model; -1 omits flagged-entry setup and scalar saving. */
void mnuCreateNodeModelEntry(MnuModelNode *node, s32 resourceGroup, s32 resourceId, s32 entryFlags, f32 x, f32 y, f32 z) {
    MdlCtx *model = (MdlCtx *)func_00232198(resourceGroup, resourceId);
    node->model = model;
    if (entryFlags != -1) {
        model->first->frameStep = z;
        node->savedModelValue = z;
        mdlAddEntryFlaggedEx(model, 0, entryFlags, x, y);
    }
}

/* Release the node's active state and set bit 0 in its model's flag word. */
void mnuDeactivateModelNode(s32 nodeAddress) {
    MnuModelNode *node = (MnuModelNode *)nodeAddress;
    node->flags = 0;
    node->model->flags = node->model->flags | 1;
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
    MnuModelNode *modelNode = (MnuModelNode *)node;

    modelNode->primary[0] = x;
    modelNode->primary[1] = y;
    modelNode->primary[2] = z;
    modelNode->primary[3] = 0;
    VU0_LOAD_VF(vf10, modelNode->primary);
    mdlStorePrimaryVectorVU(modelNode->model);
}

/* Translate the primary (0x00) vector and load it into the model. */
void mnuTranslateNodePrimaryVector(u8 *node, f32 x, f32 y, f32 z) {
    MnuModelNode *modelNode = (MnuModelNode *)node;

    modelNode->primary[3] = 0;
    modelNode->primary[0] += x;
    modelNode->primary[1] += y;
    modelNode->primary[2] += z;
    VU0_LOAD_VF(vf10, modelNode->primary);
    mdlStorePrimaryVectorVU(modelNode->model);
}

/* Cache the source quaternion and use it to rebuild the model's rotation basis. */
void mnuRefreshNodeSecondaryVector(u8 *node) {
    f32 quaternion[4];

    func_00328160(quaternion);
    ((MnuModelNode *)node)->rotationQuaternion[0] = quaternion[0];
    ((MnuModelNode *)node)->rotationQuaternion[1] = quaternion[1];
    ((MnuModelNode *)node)->rotationQuaternion[2] = quaternion[2];
    ((MnuModelNode *)node)->rotationQuaternion[3] = quaternion[3];
    VU0_LOAD_VF(vf10, node + 0x10);
    mdlUpdateContextRotationBasisFromQuaternion((u32)((MnuModelNode *)node)->model);
}

void func_0031C688(MnuModelNode *node, f32 xAngle, f32 yAngle, f32 zAngle) {
    f32 vector[4];
    f32 left[4];
    f32 up[4];
    SdfMat4 basis;
    f32 quaternion[4];

    mnuSetBasisRow(vector, sdfViewTargetVector[0] - node->primary[0],
                   sdfViewTargetVector[1] - node->primary[1],
                   sdfViewTargetVector[2] - node->primary[2], 0.0f);
    mnuSetBasisRow(left, -1.0f, 0.0f, 0.0f, 0.0f);
    VU0_NORMALIZE_PACKED_VECTOR(vector);
    VU0_LOAD_VF(vf10, vector);
    VU0_LOAD_VF(vf11, left);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);

    memset(&basis, 0, sizeof(basis));
    mnuSetBasisRow(basis.m, left[0], left[1], left[2], 0.0f);
    mnuSetBasisRow(basis.m + 4, up[0], up[1], up[2], 0.0f);
    mnuSetBasisRow(basis.m + 8, vector[0], vector[1], vector[2], 0.0f);
    mnuSetBasisRow(basis.m + 12, 0.0f, 0.0f, 0.0f, 1.0f);
    func_00326BC8(&basis, xAngle);
    func_003270C8(&basis, yAngle);
    func_003275C8(&basis, zAngle);
    sdfMat4Transpose(&basis, &basis);
    func_00327C80(quaternion, &basis);
    vector[0] = quaternion[0];
    vector[1] = quaternion[1];
    vector[2] = quaternion[2];
    vector[3] = quaternion[3];
    VU0_LOAD_VF(vf10, vector);
    mdlUpdateContextRotationBasisFromQuaternion((u32)node->model);
}

/* Fill the tertiary (0x20) vector with one value and load it into the model. */
void mnuSetNodeScaleVector(u8 *node, f32 value) {
    MnuModelNode *modelNode = (MnuModelNode *)node;

    modelNode->tertiary[0] = value;
    modelNode->tertiary[1] = value;
    modelNode->tertiary[2] = value;
    modelNode->tertiary[3] = 0;
    VU0_LOAD_VF(vf10, modelNode->tertiary);
    mdlStoreTertiaryVectorVU(modelNode->model);
}

void mnuBroadcastNodeModelState(u8 *node) {
    mdlBroadcastMasked((u32)((MnuModelNode *)node)->model);
}


void mnuSetModelNodeBroadcastAlpha(void) {
}

/* Replace only the high byte of the model's broadcast word. */
void mnuSetNodeModelBroadcastByte(u8 *node, u8 highByte) {
    u32 broadcastLowBytes = mdlGetBroadcastValue((u32)((MnuModelNode *)node)->model) & 0xFFFFFF;

    mdlBroadcastMasked((u32)((MnuModelNode *)node)->model, broadcastLowBytes | ((u32)highByte << 24));
}

void mnuSetModelNodeVisibility(u8 *node, s8 selector) {
    MnuModelNode *entry = (MnuModelNode *)node;
    if (selector == 1) {
        entry->model->flags &= ~1U;
    } else {
        entry->model->flags |= 1U;
    }
}

INCLUDE_RODATA(const s32, "game/code_0031B188", D_0042DAE8);

INCLUDE_SDATA(const s32, "game/code_0031B188", D_00438950);


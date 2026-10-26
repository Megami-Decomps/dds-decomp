#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "common.h"
#include "eff_transform.h"

/* The slot-state constructor allocates and clears this complete 0xB4-byte owner.
 * Object resources, indexed slots and motion control share the same record. */
#define DDS3_OBJECT_RESOURCE_SLOT_COUNT 24

#define DDS3_OBJECT_RESOURCE_MODEL_CONTEXT 0
#define DDS3_OBJECT_RESOURCE_DEV_MOTION 1
#define DDS3_OBJECT_RESOURCE_RELEASED 3

struct sdfMotionCtrlr_t;
struct sdfModelRootNode_t;

typedef struct dds3ObjectBaseControlWorkData_t {
    u32 flags;
    struct WorldIndexNode *worldIndexNode;
    u32 resourceState;
    void *resourceHandle; /* Owner-held model context/device-motion root;
                           * borrowed views expire on release or rebuild. */
    void *slots[8];
    void *extData;
    struct sdfModelRootNode_t *devSlot;
    struct sdfMotionCtrlr_t *motion;
    s32 mode;
    f32 weight;
    u32 unk44;
    s32 resourceSlotCount;
    s32 resourceSlots[DDS3_OBJECT_RESOURCE_SLOT_COUNT];
    u32 resourceSlotFamily;
    s32 currentSlot;
} dds3ObjectBaseControlWorkData_t;

typedef char ObjBase_size_must_be_0xB4[(sizeof(dds3ObjectBaseControlWorkData_t) == 0xB4) ? 1 : -1];
typedef char ObjBase_resourceSlotFamily_offset_must_be_0xAC[
    ((u32)&((dds3ObjectBaseControlWorkData_t *)0)->resourceSlotFamily == 0xAC) ? 1 : -1];

dds3ObjectBaseControlWorkData_t *dds3CreateSlotResourceState(dds3Basic_t *owner);
void dds3ObjectBaseCreateMover(dds3Basic_t *object);
dds3ObjectBaseControlWorkData_t *dds3GetBaseControlWorkData(dds3Basic_t *object);
void *dds3ObjectBaseGetModelHandle(dds3Basic_t *object);
const char *dds3EventBasicGetResource(dds3Basic_t *object);
/* Forward a callback to the object's slot-1 handler when one is present; returns 1 if it did. */
u8 dds3ObjectBaseSetPadControl(dds3Basic_t *object,
                                   s32 (*update)(struct dds3PoseSet_t *, struct dds3Basic_t *));
dds3Basic_t *dds3ObjectBaseSetControlBasic(dds3Basic_t *object, dds3Basic_t *slotData);
dds3Basic_t *dds3ObjectBaseSetControlBasicType(dds3Basic_t *object, dds3Basic_t *slotData, s32 slotIndex);
dds3Basic_t *dds3ObjectBaseGetControlBasic(dds3Basic_t *object, s32 slotIndex);
void dds3ObjectBaseSetControlBasicSvs(dds3Basic_t *object, u32 value);
dds3Basic_t *dds3SearchSceneBasic(dds3Basic_t *world, u32 key, s32 kind);
dds3Basic_t *dds3SceneBasicSearchNameAndType(dds3Basic_t *world, s32 slotIndex, const char *name);
dds3Basic_t *dds3SceneBasicSearchName(dds3Basic_t *world, const char *name);
dds3Basic_t *dds3AddSceneBasicData(s32 kind);
/* Allocate and initialize the inner transform object for a world node. */
u8 dds3InitPoseSet(dds3Basic_t *node);
void dds3DeleteSceneBasicData(dds3Basic_t *node);
/* Load the named area resource and attach it to the object's world table. */
void dds3SceneBasicMakeScene(dds3Basic_t *object, const char *resourcePath);
/* Store the source object address used by a slot's later path replacement. */
void dds3MoverBasicSetPath(dds3Basic_t *object, dds3Basic_t *sourceObject);
dds3Basic_t *dds3GetActiveCameraSceneBasic(dds3Basic_t *world);
/* For a valid world, replace its camera and return the previous borrowed node (NULL if unset). */
dds3Basic_t *dds3ReplaceActiveCameraSceneBasic(dds3Basic_t *world, dds3Basic_t *camera);
dds3Basic_t *dds3GetActiveUnitSceneBasic(dds3Basic_t *world);
/* For a valid world, replace its player and return the previous borrowed node (NULL if unset). */
dds3Basic_t *dds3ReplaceActiveUnitSceneBasic(dds3Basic_t *world, dds3Basic_t *player);

/* Inner transform vector helpers; fetch results are returned in VU vf10. */
void dds3SetPosPoseSet(dds3Basic_t *object, u128 *vector);
void dds3SetRotPoseSet(dds3Basic_t *object, u128 *vector);
void dds3SetSclPoseSet(dds3Basic_t *object, u128 *vector);
void dds3GetPosPoseSet(dds3Basic_t *object);
void dds3GetRotPoseSet(dds3Basic_t *object);
void effObjFetchInnerScale(dds3Basic_t *object);
void dds3GetPosPoseSetMCR(dds3Basic_t *object, u128 *vector);
void dds3AddRotPoseSet(dds3Basic_t *object, u128 *vector);
void dds3GetSclPoseSetMCR(dds3Basic_t *object, u128 *vector);

/* Four-word object inner record (0x10); no direct C unit users yet. */
typedef struct {
    u8 pad[4];
    u32 unk4;
    u32 unk8;
    u32 unkC;
} ObjInner;

/* Doubly linked world index node (0x10); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct WorldIndexNode {
    s16 headIndex;
    s16 tailIndex;
    s16 cursorIndex;
    u16 entryCount;
    struct WorldIndexNode *next;
    struct WorldIndexNode *previous;
} WorldIndexNode;

typedef char WorldIndexNodeLayoutAssert[
    (sizeof(WorldIndexNode) == 0x10 &&
     (unsigned long)&((WorldIndexNode *)0)->headIndex == 0 &&
     (unsigned long)&((WorldIndexNode *)0)->tailIndex == 2 &&
     (unsigned long)&((WorldIndexNode *)0)->cursorIndex == 4 &&
     (unsigned long)&((WorldIndexNode *)0)->entryCount == 6 &&
     (unsigned long)&((WorldIndexNode *)0)->next == 8 &&
     (unsigned long)&((WorldIndexNode *)0)->previous == 0xC) ? 1 : -1];

WorldIndexNode *dds3GetBasicArrayHandle(s32 initialCount);
void dds3FreeBasicArrayHandle(WorldIndexNode *node);
WorldIndexNode *dds3ObjectBaseGetEventArray(dds3Basic_t *object);
u32 dds3AddBasicArray(dds3BasicArrayHandle_t *object, s32 count);
u16 dds3GetBasicArrayCount(const dds3BasicArrayHandle_t *object);
s32 dds3SetSearchNextBasicArray(dds3BasicArrayHandle_t *indexNode, const dds3Basic_t *targetNode);
/* Index-list values borrow base world nodes; the list does not own them. */
u32 dds3SetBasicArray(dds3BasicArrayHandle_t *object, dds3Basic_t *node);
dds3Basic_t *dds3GetBasicArray(const dds3BasicArrayHandle_t *object);
u32 dds3SetFirstBasicArray(dds3BasicArrayHandle_t *object);
u32 dds3SetNextBasicArray(dds3BasicArrayHandle_t *object);
/* Visit stored dds3Basic_t pointers in order. */
s32 dds3ForEachBasicArray(dds3BasicArrayHandle_t *object, s32 (*callback)(dds3Basic_t *));
WorldIndexNode *dds3SceneBasicGetList(dds3Basic_t *object, s32 kind);

void dds3AreaBasicRegistItem(dds3Basic_t *indexOwner, dds3Basic_t *node);
WorldIndexNode *dds3AreaBasicGetWorldIndexNode(dds3Basic_t *worldIndexOwner, s32 slotIndex);

/* Eight-byte entries link both allocated and free world value chains. */
typedef struct WorldValueEntry {
    s32 value;
    s16 nextIndex;
    s16 previousIndex;
} WorldValueEntry;

typedef char WorldValueEntry_size_must_be_0x08[(sizeof(WorldValueEntry) == 0x08) ? 1 : -1];

struct sdfMemHandle_t;

/* Complete kind-0 payload: node lists, selected objects and the value-entry pool. */
typedef struct {
    dds3Basic_t *firstNode;
    dds3Basic_t *lastNode;
    void *activeScene;  /* 0x08: selected primary world node */
    void *controlScene; /* 0x0C: selected secondary world node */
    struct sdfMemHandle_t *entryAllocation; /* 0x10: descriptor, distinct from its data */
    WorldValueEntry *entries;           /* 0x14: retained allocation address */
    u16 entryCapacity;                  /* 0x18: initial entry count */
    s16 freeHeadIndex;                  /* 0x1A: -1 when the pool is exhausted */
    s16 freeTailIndex;                  /* 0x1C */
    u16 freeEntryCount;                 /* 0x1E */
    WorldIndexNode *firstIndex;
    WorldIndexNode *lastIndex;
} WorldInfo;

typedef char WorldInfo_size_must_be_0x28[(sizeof(WorldInfo) == 0x28) ? 1 : -1];
typedef char WorldInfo_activeScene_at_0x08[((u32)&((WorldInfo *)0)->activeScene == 0x08) ? 1 : -1];
typedef char WorldInfo_controlScene_at_0x0C[((u32)&((WorldInfo *)0)->controlScene == 0x0C) ? 1 : -1];
typedef char WorldInfo_entries_at_0x14[((u32)&((WorldInfo *)0)->entries == 0x14) ? 1 : -1];
typedef char WorldInfo_firstIndex_at_0x20[((u32)&((WorldInfo *)0)->firstIndex == 0x20) ? 1 : -1];

/* Four-halfword world index key (0x8); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} IndexObj;

/* Scene object's state word at +0x14 (0x18); DDS1/2 basic/dds3SceneBasic.c. */
typedef struct {
    u8 pad0[0x14];
    u32 state;
} SceneObject;

/* Scene wrapper pointing at its object (0x1C); DDS1/2 basic/dds3SceneBasic.c. */
typedef struct {
    u8 pad0[0x18];
    SceneObject *object;
} Scene;

/* Rotation, position and scale copied by the world-transform helpers (0x28). */
typedef struct WorldTransformParams {
    f32 rotation[4];
    f32 position[3];
    f32 scale[3];
} WorldTransformParams;

/* Setup record consumed by dds3SetLightObjectResoruce (0x34). */
typedef struct WorldTransformSetup {
    u32 unk00;
    u32 flags;                      /* bit 0 -> 1, bit 1 -> 4 in the object's flags */
    u32 mode;
    WorldTransformParams transform;
} WorldTransformSetup;

/* Confirmed transform payload fields; no total allocation extent is claimed. */
typedef struct WorldTransformData {
    f32 position[3]; /* 0x00 */
    f32 positionW;   /* 0x0C */
    u8 opaque10[0x30];
    f32 scale[3];     /* 0x40 */
    f32 scaleW;       /* 0x4C */
    f32 rotation[4];  /* 0x50 */
    u32 mode;         /* 0x60 */
    u32 flags;        /* 0x64 */
} WorldTransformData;

void dds3SetLightObjectResoruce(dds3Basic_t *object, WorldTransformSetup *setup);
void dds3SetLightObjectPathResoruce(dds3Basic_t *object, const WorldTransformParams *params);

typedef char WorldTransformData_offsets_must_match_paired_loaders[
    ((u32)&((WorldTransformData *)0)->position == 0x00 &&
     (u32)&((WorldTransformData *)0)->positionW == 0x0C &&
     (u32)&((WorldTransformData *)0)->opaque10 == 0x10 &&
     (u32)&((WorldTransformData *)0)->scale == 0x40 &&
     (u32)&((WorldTransformData *)0)->scaleW == 0x4C &&
     (u32)&((WorldTransformData *)0)->rotation == 0x50 &&
     (u32)&((WorldTransformData *)0)->mode == 0x60 &&
     (u32)&((WorldTransformData *)0)->flags == 0x64) ? 1 : -1];

/* Kind-4 world-node data: cached look-at matrix and eye/up vectors (0x90). */
typedef struct CameraData {
    u32 matrix[4][4]; /* Raw VU matrix words; the SDK also exposes word [0][3]. */
    u128 localEyeOffset;
    u128 localUp;
    u128 worldEye;
    u128 worldUp;
    dds3ObjectBaseControlWorkData_t *handle;
    s32 eyeIsRelative;
    u32 fovUpdatePending; /* Bit 0 requests a field-of-view update. */
    f32 fieldOfView;      /* Radians. */
} CameraData;

typedef char CameraData_size_must_be_0x90[(sizeof(CameraData) == 0x90) ? 1 : -1];

dds3ObjectBaseControlWorkData_t *dds3GetCameraObjectBaseControlWorkData(dds3Basic_t *camera);


#endif /* DDS3OBJ_H */

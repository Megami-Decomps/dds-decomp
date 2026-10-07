#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "common.h"
#include "eff_transform.h"

/* The slot-state constructor allocates and clears this complete 0xB4-byte owner.
 * Object resources, indexed slots and motion control share the same record. */
#define DDS3_OBJECT_RESOURCE_SLOT_COUNT 24

struct Motion;

typedef struct ObjBase {
    u32 flags;
    u32 worldIndexNode;
    u32 resourceState;
    u32 resourceHandle;
    void *slots[8];
    void *extData;
    s32 devSlot;
    struct Motion *motion;
    s32 mode;
    f32 weight;
    u32 unk44;
    s32 resourceSlotCount;
    s32 resourceSlots[DDS3_OBJECT_RESOURCE_SLOT_COUNT];
    u32 unkAC;
    s32 currentSlot;
} ObjBase;

typedef char ObjBase_size_must_be_0xB4[(sizeof(ObjBase) == 0xB4) ? 1 : -1];

ObjBase *dds3CreateSlotResourceState(void *owner);
ObjBase *dds3GetObjectOwnedHandle();

/* Four-word object inner record (0x10); no direct C unit users yet. */
typedef struct {
    u8 pad[4];
    u32 unk4;
    u32 unk8;
    u32 unkC;
} ObjInner;

/* Object slot discriminator (0x10); DDS1 basic/dds3ObjectBase.c. */
typedef struct {
    u8 pad[0xF];
    u8 kind; /* Selects a slot in ObjBase. */
} ObjData;


/* Doubly linked world index node (0x10); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct NodeB {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct NodeB *next;
    struct NodeB *previous;
} NodeB;

/* Eight-byte entries link both allocated and free world value chains. */
typedef struct WorldValueEntry {
    s32 value;
    s16 nextIndex;
    s16 previousIndex;
} WorldValueEntry;

typedef char WorldValueEntry_size_must_be_0x08[(sizeof(WorldValueEntry) == 0x08) ? 1 : -1];

struct SdfMemBlock;

/* Complete kind-0 payload: node lists, selected objects and the value-entry pool. */
typedef struct {
    EffWorldNode *firstNode;
    EffWorldNode *lastNode;
    void *primaryObject;
    void *secondaryObject;
    struct SdfMemBlock *entryAllocation; /* 0x10: descriptor, distinct from its data */
    WorldValueEntry *entries;           /* 0x14: retained allocation address */
    u16 entryCapacity;                  /* 0x18: initial entry count */
    s16 freeHeadIndex;                  /* 0x1A: -1 when the pool is exhausted */
    s16 freeTailIndex;                  /* 0x1C */
    u16 freeEntryCount;                 /* 0x1E */
    NodeB *firstIndex;
    NodeB *lastIndex;
} WorldInfo;

typedef char WorldInfo_size_must_be_0x28[(sizeof(WorldInfo) == 0x28) ? 1 : -1];
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

/* Setup record consumed by dds3LoadWorldTransformSetup (0x34). */
typedef struct WorldTransformSetup {
    u32 unk00;
    u32 flags;                      /* bit 0 -> 1, bit 1 -> 4 in the object's flags */
    u32 mode;
    WorldTransformParams transform;
} WorldTransformSetup;


/* Kind-4 world-node data: cached look-at matrix and eye/up vectors (0x90). */
typedef struct CameraData {
    u32 matrix[4][4]; /* Raw VU matrix words; the SDK also exposes word [0][3]. */
    u128 localEyeOffset;
    u128 localUp;
    u128 worldEye;
    u128 worldUp;
    ObjBase *handle;
    s32 eyeIsRelative;
    u32 fovUpdatePending; /* Bit 0 requests a field-of-view update. */
    f32 fieldOfView;      /* Radians. */
} CameraData;

typedef char CameraData_size_must_be_0x90[(sizeof(CameraData) == 0x90) ? 1 : -1];

ObjBase *dds3GetCameraHandle(EffWorldNode *camera);


#endif /* DDS3OBJ_H */

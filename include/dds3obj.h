#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "common.h"

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

/* Shared world-node prefix through its list links (0x28). Key is compared by
 * world lookup; kind-0x11 field probes read the position payload at +0x18. */
typedef struct NodeA {
    u8 pad00[4];
    u32 key; /* +0x04 */
    u8 pad08[0x10];
    void *payload; /* +0x18: payload type depends on the node kind. */
    struct ObjectTransform *inner; /* +0x1C: separately allocated transform. */
    struct NodeA *next;
    struct NodeA *previous;
} NodeA;

/* Doubly linked world index node (0x10); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct NodeB {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct NodeB *next;
    struct NodeB *previous;
} NodeB;

/* World lookup entry (0x8); DDS1/2 basic/dds3WorldBasic.c via WorldInfo. */
typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} Entry;

/* World node lists, object slots and index range (0x28); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct {
    NodeA *firstNode;
    NodeA *lastNode;
    void *primaryObject;
    void *secondaryObject;
    u8 pad10[4];
    Entry *unk14;
    u8 pad18[2];
    u16 unk1A;
    s16 unk1C;
    u16 unk1E;
    NodeB *firstIndex;
    NodeB *lastIndex;
} WorldInfo;

/* World handle pointing at its index/list state (0x1C); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct {
    u8 pad[0x18];
    WorldInfo *info;
} World;

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

/* Separately allocated 0xD0-byte world-node transform. Its matrix, position,
 * quaternion and scale are consumed by the generic and camera VU0 routines. */
typedef struct ObjectTransform {
    u128 matrix[4];
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
    f32 smoothedPosition[4]; /* 0x70: lagged position used by the field camera. */
    u8 pad80[0x40];
    u32 flags;
    f32 radius;
    u32 unkC8;
    u8 padCC[4];
} ObjectTransform;

/* Kind-4 world-node data: cached look-at matrix and eye/up vectors (0x90). */
typedef struct CameraData {
    u32 matrix[4][4]; /* Raw VU matrix words; the SDK also exposes word [0][3]. */
    u128 localEyeOffset;
    u128 localUp;
    u128 worldEye;
    u128 worldUp;
    u32 handle;
    s32 eyeIsRelative;
    u32 fovUpdatePending; /* Bit 0 requests a field-of-view update. */
    f32 fieldOfView;      /* Radians. */
} CameraData;

/* Kind-4 specialization of the world node; its caption is shown by camera debug. */
typedef struct CameraObject {
    u8 unk0[4];
    s32 unk4;
    char *caption;
    u8 pad0C[0x0C];
    CameraData *data;
    ObjectTransform *inner;
    struct CameraObject *next;
    struct CameraObject *previous;
} CameraObject;

#endif /* DDS3OBJ_H */

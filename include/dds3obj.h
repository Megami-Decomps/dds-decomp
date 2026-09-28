#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "common.h"

/* Object flags, indexed slots and extension record. */
typedef struct {
    u32 flags;
    u32 unk4;
    u32 unk8;
    u32 unkC;
    void *slots[8];
    void *extData;
    u8 pad34[4];
    void *unk38;
} ObjBase;

typedef struct {
    u8 pad[4];
    u32 unk4;
    u32 unk8;
    u32 unkC;
} ObjInner;

typedef struct {
    u8 pad[0xF];
    u8 kind; /* Selects a slot in ObjBase. */
} ObjData;

/* World object lists, indices and their owner. */
typedef struct NodeA {
    u8 pad[0x20];
    struct NodeA *next;
    struct NodeA *previous;
} NodeA;

typedef struct NodeB {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    struct NodeB *next;
    struct NodeB *previous;
} NodeB;

typedef struct {
    s32 unk0;
    s16 unk4;
    s16 unk6;
} Entry;

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

typedef struct {
    u8 pad[0x18];
    WorldInfo *info;
} World;

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    u16 unk6;
} IndexObj;

/* Scene state reached through its object's +0x18 data pointer. */
typedef struct {
    u8 pad0[0x14];
    u32 state;
} SceneObject;

typedef struct {
    u8 pad0[0x18];
    SceneObject *object;
} Scene;

#endif /* DDS3OBJ_H */

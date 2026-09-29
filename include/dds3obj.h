#ifndef DDS3OBJ_H
#define DDS3OBJ_H

#include "common.h"

/* Base object flags, eight indexed slots and extension (0x3C); DDS1/2 basic/dds3ObjectBase.c. */
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

/* Doubly linked world object node (0x28); DDS1/2 basic/dds3WorldBasic.c. */
typedef struct NodeA {
    u8 pad[0x20];
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

#endif /* DDS3OBJ_H */

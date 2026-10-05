#ifndef EFF_TRANSFORM_H
#define EFF_TRANSFORM_H

#include "common.h"

typedef struct EffTransformNode EffTransformNode;

typedef struct {
    s32 (*create)(EffTransformNode *);
    void (*notify)(EffTransformNode *);
} EffTransformOwner;

/* 0xD0-byte transform node; inner vectors are transferred through VU0. */
struct EffTransformNode {
    u32 word0;
    u32 word4;
    u32 word8;
    u32 kindTag;
    EffTransformOwner *owner;
    u32 word14;
    u32 ownerData;
    EffTransformNode *inner;
    EffTransformNode *prev;
    EffTransformNode *next;
    u32 word28;
    u32 word2C;
    u32 word30;
    u32 color;
    u8 pad38[8];
    u128 vec40;
    u128 vec50;
    u128 vec60;
    u128 vec70;
    u128 vec80;
    u128 vec90;
    u128 vecA0;
    u8 vecB0[0x10];
    u32 flags;
    f32 scalar;
    u32 unkC8;
};

#endif

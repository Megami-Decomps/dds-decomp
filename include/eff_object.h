#ifndef EFF_OBJECT_H
#define EFF_OBJECT_H
#include "dds3obj.h"
struct EvtUnit;
/* Kind-5 constructors allocate and clear the complete 0x40-byte payload. */
typedef struct EffectObjectData {
    ObjBase *handle;
    u32 word04;
    struct EvtUnit *transitionWork; /* 0x08: object transition work */
    ObjBase *modelHolder; /* 0x0C: owned base retains the model resource. */
    s32 activeId;
    u32 followParameterIndex; /* 0x14: 16-byte follow-parameter row; all-ones selects the base record. */
    u32 word18;
    s32 pendingTargetKey; /* 0x1C: kind-0x11 world-node key used by the follow turn. */
    s32 angleReturnDelayFrames; /* 0x20: holdoff before returning the follow angle to zero. */
    u32 word24;
    f32 angle;
    f32 limitMin2C;
    f32 limitMax30;
    u32 word34;
    f32 limitMin38;
    f32 limitMax3C;
} EffectObjectData;
typedef char EffectObjectData_size_must_be_0x40[(sizeof(EffectObjectData) == 0x40) ? 1 : -1];
typedef char EffectObjectData_modelHolder_at_0x0C[((u32)&((EffectObjectData *)0)->modelHolder == 0x0C) ? 1 : -1];
typedef char EffectObjectData_tail_at_0x3C[((u32)&((EffectObjectData *)0)->limitMax3C == 0x3C) ? 1 : -1];

/* Kind-6 world nodes allocate this complete transform payload at 0x30 bytes. */
typedef struct EffectTransformData {
    ObjBase *resourceState;
    u32 roomNumber;
    u32 opacityMode;
    s32 activeId;
    f32 offset[4];
    f32 position[4];
} EffectTransformData;
typedef char EffectTransformData_size_must_be_0x30[(sizeof(EffectTransformData) == 0x30) ? 1 : -1];
typedef char EffectTransformData_roomNumber_at_0x04[((u32)&((EffectTransformData *)0)->roomNumber == 0x04) ? 1 : -1];
typedef char EffectTransformData_position_at_0x20[((u32)&((EffectTransformData *)0)->position == 0x20) ? 1 : -1];

ObjBase *dds3GetEffectObjectModelHolder(EffWorldNode *object);
ObjBase *effObjGetDataHandle(EffWorldNode *object);
#endif

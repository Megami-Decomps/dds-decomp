#include "common.h"

typedef struct ObjectSubstate {
    u8 pad0[0xC];
    u32 valueC;
    u8 pad10[0x74];
    s32 cameraMode;
} ObjectSubstate;

typedef struct ObjectWithSubstate {
    u8 pad0[0x18];
    ObjectSubstate *substate;
} ObjectWithSubstate;

typedef struct CameraSlotState {
    s32 unk0;   /* 0x0 */
    s32 unk4;   /* 0x4 */
} CameraSlotState;

typedef struct ActionObj {
    u8 unk0[4];             /* 0x0 */
    s32 unk4;               /* 0x4 */
    u8 unk8[0x10];          /* 0x8 */
    CameraSlotState *state; /* 0x18 */
    s32 unk1C;              /* 0x1C */
} ActionObj;

extern ActionObj *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();

extern void effObjSetInnerFirstVec();

extern void effObjSetInnerSecondVec();

extern void effObjInnerVecBackup();

extern void *dds3GetFirstWorldObjectNodeOfKind2(void);

extern void dds3ExchangeSlot();

extern void dds3InvokeSlot5Handler();

u32 dds3GetCameraMode(ObjectWithSubstate *object) {
    return object->substate->cameraMode;
}

void dds3SetCameraMode(ObjectWithSubstate *object, s32 value) {
    if (object->substate->cameraMode != value) {
        object->substate->cameraMode = value;
    }
}

u32 func_00113230(ObjectWithSubstate *object) {
    return object->substate->valueC;
}

ActionObj *dds3SpawnCameraSlotObj5(s32 a, void *firstVector, void *secondVector) {
    ActionObj *obj = dds3AppendWorldObjectNode(5);

    if (obj != NULL) {
        obj->unk4 = a;
        dds3EnsureSlotData(obj);
        effObjSetInnerFirstVec(obj, firstVector);
        effObjSetInnerSecondVec(obj, secondVector);
        effObjInnerVecBackup(obj->unk1C);
        obj->state->unk0 = -1;
        obj->state->unk4 = 0;
        dds3ExchangeSlot(obj, dds3GetFirstWorldObjectNodeOfKind2(), 5);
        dds3InvokeSlot5Handler(obj);
        return obj;
    }
}

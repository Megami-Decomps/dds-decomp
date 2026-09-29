#include "common.h"

typedef struct {
    u8 pad0[0xC];
    u32 valueC;
    u8 pad10[0x74];
    s32 mode;
} CameraData;

typedef struct {
    u8 pad0[0x18];
    CameraData *data;
} CameraObject;

u32 dds3GetCameraMode(CameraObject *camera) {
    return camera->data->mode;
}

void dds3SetCameraMode(CameraObject *camera, s32 mode) {
    if (camera->data->mode != mode) {
        camera->data->mode = mode;
    }
}

u32 func_00113008(CameraObject *camera) {
    return camera->data->valueC;
}

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

extern ActionObj *func_00110880();
extern void func_00112750();
extern void effObjSetInnerFirstVec();
extern void effObjSetInnerSecondVec();
extern void effObjInnerVecBackup();
extern s32 func_00111388();
extern void dds3ExchangeSlot();
extern void dds3InvokeSlot5Handler();

ActionObj *func_00113018(s32 a, void *firstVector, void *secondVector) {
    ActionObj *obj = func_00110880(5);

    if (obj != NULL) {
        obj->unk4 = a;
        func_00112750(obj);
        effObjSetInnerFirstVec(obj, firstVector);
        effObjSetInnerSecondVec(obj, secondVector);
        effObjInnerVecBackup(obj->unk1C);
        obj->state->unk0 = -1;
        obj->state->unk4 = 0;
        dds3ExchangeSlot(obj, func_00111388(), 5);
        dds3InvokeSlot5Handler(obj);
        return obj;
    }
}

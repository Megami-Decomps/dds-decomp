#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x40];
    u128 vec40;       /* 0x40 */
    u128 vec50;       /* 0x50 */
    u128 vec60;       /* 0x60 */
    u8 pad70[0x10];   /* 0x70 */
    u32 handle;       /* 0x80 */
    s32 unk84;        /* 0x84 */
    u32 flags;        /* 0x88 */
    f32 value;        /* 0x8C */
} CameraData;

typedef struct {
    u8 pad0[0x18];
    CameraData *data;
} CameraObject;

typedef struct EEF0Node EEF0Node;

typedef struct {
    u8 pad00[0x4];              /* 0x00 */
    void (*notify)(EEF0Node *); /* 0x04 called with the node being destroyed */
} EEF0Owner;

/* Transform node (0xD0 bytes). The links at 0x10/0x20/0x24 tie a node into
 * its owner's list; inner points at a child node whose vectors live in VU
 * registers between calls (loaded with lqc2, stored with sqc2).
 */
struct EEF0Node {
    u8 pad00[0x10];   /* 0x00 */
    EEF0Owner *owner; /* 0x10 */
    u8 pad14[0x8];    /* 0x14 */
    EEF0Node *inner;  /* 0x1C */
    EEF0Node *prev;   /* 0x20 */
    EEF0Node *next;   /* 0x24 */
    u8 pad28[0x18];   /* 0x28 */
    u128 vec40;       /* 0x40 */
    u128 vec50;       /* 0x50 */
    u128 vec60;       /* 0x60 */
    u8 pad70[0x10];   /* 0x70 */
    u128 vec80;       /* 0x80 copy of vec40 */
    u128 vec90;       /* 0x90 copy of vec50 */
    u128 vecA0;       /* 0xA0 copy of vec60 */
    u8 vecB0[0x10];   /* 0xB0 cleared on alloc */
    u32 flags;        /* 0xC0 bit0 set, bit1 cleared on vector write */
    f32 unkC4;        /* 0xC4 */
    u32 unkC8;        /* 0xC8 */
};

void dds3DestroyCameraData(CameraObject *camera) {
    CameraData *data;

    effObjFreeInner();
    data = camera->data;
    dds3DestroyObjectBase(data->handle);
    sdfReleaseChipBlock(data);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C20);

typedef struct ActionObj {
    u8 unk0[4];    /* 0x0 */
    s32 unk4;      /* 0x4 */
    u8 unk8[0x10]; /* 0x8 */
    CameraData *data; /* 0x18 */
    s32 unk1C;     /* 0x1C */
} ActionObj;

u32 func_00112D08(void) {
    return 1;
}

extern void effMiscQuaternionToMatrixVU(void);
extern void sdfVuBuildLookAtBasis(void *eye, void *target, void *up);

/* vu0 routine: rebuild the camera basis from the inner node's rotation (+0x50): the up vector, and the eye (+0x60) when mode 1 adds the inner node's position, then look-at. */
void func_00112D10(ActionObj *obj) {
    CameraData *data = obj->data;
    u8 *inner = (u8 *)obj->unk1C;

    VU0_LOAD_VF(vf10, inner + 0x50);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &data->vec50);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, data->pad70);
    if (data->unk84 == 1) {
        VU0_LOAD_VF(vf10, &data->vec40);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, inner + 0x40);
        VU0_ADD(vf10, vf10, vf11);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, &data->vec60);
    }
    sdfVuBuildLookAtBasis(&data->vec60, inner + 0x40, data->pad70);
    VU0_STORE_MATRIX(data);
}

u32 dds3GetCameraHandle(CameraObject *camera) {
    return camera->data->handle;
}

extern ActionObj *dds3AppendWorldObjectNode();

extern void dds3EnsureSlotData();
extern void effObjSetInnerFirstVec(ActionObj *obj, void *vec);
extern void effObjInnerVecBackup(s32 inner);
extern void func_00112D10(ActionObj *obj);

ActionObj *dds3CreateCameraObjectWithSlotData(s32 value) {
    ActionObj *obj = dds3AppendWorldObjectNode(4);

    obj->unk4 = value;
    dds3EnsureSlotData(obj);
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112E30);

ActionObj *func_00112F28(s32 value, void *innerVec, u128 *vec60, u128 *vec50) {
    ActionObj *obj = dds3CreateCameraObjectWithSlotData(value);
    CameraData *data = obj->data;

    data->value = 0.6283185f;
    data->unk84 = 0;
    effObjSetInnerFirstVec(obj, innerVec);
    effObjInnerVecBackup(obj->unk1C);
    PCP_COPY_VECTOR(&data->vec50, vec50);
    PCP_COPY_VECTOR(&data->vec60, vec60);
    func_00112D10(obj);
    return obj;
}

ActionObj *dds3CreateCameraObjectWithVectors(s32 slotValue, f32 value, void *innerVec, u128 *vec40, u128 *vec50, s32 flag84) {
    ActionObj *obj = dds3CreateCameraObjectWithSlotData(slotValue);
    CameraData *data = obj->data;

    data->value = value;
    data->unk84 = flag84;
    effObjSetInnerFirstVec(obj, innerVec);
    effObjInnerVecBackup(obj->unk1C);
    PCP_COPY_VECTOR(&data->vec50, vec50);
    PCP_COPY_VECTOR(&data->vec40, vec40);
    if (flag84 == 0) {
        PCP_COPY_VECTOR(&data->vec60, vec40);
    }
    func_00112D10(obj);
    return obj;
}

void dds3ReleaseCameraWorldNode(void) {
    dds3RemoveWorldObjectNode();
}

void dds3LoadObjectMatrixPointerIntoVu(void *obj) {
    VU0_LOAD_MATRIX(*(void **)((u8 *)obj + 0x18));
}

void dds3SetCameraVector(void *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)((u8 *)obj + 0x18) + 0x60, src);
}

void dds3LoadCameraVectorVU(EEF0Node *arg0) {
    u8 *p = *(u8 **)((u8 *)arg0 + 0x18) + 0x60;

    VU0_LOAD_VF_MEMORY(vf10, p);
}

void dds3SetCameraValue(CameraObject *camera, f32 value) {
    CameraData *state = camera->data;
    state->value = value;
    state->flags |= 1;
}

f32 dds3GetCameraValue(CameraObject *camera) {
    return camera->data->value;
}

extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: transform the data vectors by the inner quaternion matrix. */
s32 dds3TransformCameraVectorsByInnerRotation(ActionObj *obj, f32 *dst1, f32 *dst2) {
    CameraData *data = obj->data;
    CameraData *inner = (CameraData *)obj->unk1C;

    VU0_LOAD_VF(vf10, &inner->vec50);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf10, &data->vec50);
    VU0_APPLY_MATRIX(vf10, vf10);
    VU0_SET_W_ONE(vf10);
    VU0_STORE_VF(vf10, &data->pad70);
    if (data->unk84 == 1) {
        VU0_LOAD_VF(vf10, &data->vec40);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, &inner->vec40);
        VU0_ADD(vf10, vf10, vf11);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, &data->vec60);
    }
    dst1[0] = ((f32 *)&data->vec60)[0];
    dst1[1] = ((f32 *)&data->vec60)[1];
    dst1[2] = ((f32 *)&data->vec60)[2];
    dst1[3] = ((f32 *)&data->vec60)[3];
    dst2[0] = ((f32 *)&inner->vec40)[0];
    dst2[1] = ((f32 *)&inner->vec40)[1];
    dst2[2] = ((f32 *)&inner->vec40)[2];
    dst2[3] = ((f32 *)&inner->vec40)[3];
}

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412878);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412888);


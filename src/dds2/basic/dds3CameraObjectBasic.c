#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x80];
    u32 handle;
    u8 pad84[4];
    u32 flags;
    f32 value8C;
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
    func_00111A68(data->handle);
    sdfReleaseChipBlock(data);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C20);

u32 func_00112D08(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D10);

u32 dds3GetCameraHandle(CameraObject *camera) {
    return camera->data->handle;
}

typedef struct ActionObj {
    u8 unk0[4];    /* 0x0 */
    s32 unk4;      /* 0x4 */
    u8 unk8[0x14]; /* 0x8 */
    s32 unk1C;     /* 0x1C */
} ActionObj;

extern ActionObj *func_00110AA8();

extern void dds3EnsureSlotData();

ActionObj *func_00112DE8(s32 value) {
    ActionObj *obj = func_00110AA8(4);

    obj->unk4 = value;
    dds3EnsureSlotData(obj);
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112E30);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112F28);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112FC0);

void func_00113080(void) {
    func_00110B50();
}

void func_00113098(void *obj) {
    VU0_LOAD_MATRIX(*(void **)((u8 *)obj + 0x18));
}

void func_001130B8(void *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)((u8 *)obj + 0x18) + 0x60, src);
}

void dds3LoadCameraVectorVU(EEF0Node *arg0) {
    u8 *p = *(u8 **)((u8 *)arg0 + 0x18) + 0x60;

    VU0_LOAD_VF_MEMORY(vf10, p);
}

void dds3SetCameraValue(CameraObject *camera, f32 value) {
    CameraData *state = camera->data;
    state->value8C = value;
    state->flags |= 1;
}

f32 dds3GetCameraValue(CameraObject *camera) {
    return camera->data->value8C;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00113110);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412878);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_00412888);


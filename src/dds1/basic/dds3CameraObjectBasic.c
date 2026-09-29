#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad0[0x80];
    u32 handle;
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
    func_00111840(data->handle);
    func_002CFF98(data);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_001129F8);

u32 func_00112AE0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112AE8);

u32 dds3GetCameraHandle(CameraObject *camera) {
    return camera->data->handle;
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112BC0);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112C08);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D00);

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112D98);

void func_00112E58(void) {
    func_00110928();
}

void func_00112E70(void *obj) {
    VU0_LOAD_MATRIX(*(void **)((u8 *)obj + 0x18));
}

void func_00112E90(void *obj, void *src) {
    PCP_COPY_VECTOR(*(u8 **)((u8 *)obj + 0x18) + 0x60, src);
}

void func_00112EA8(EEF0Node *arg0) {
    u8 *p = *(u8 **)((u8 *)arg0 + 0x18) + 0x60;

    __asm__ volatile (
        ".set noreorder      \n"
        "lqc2 vf10, 0(%0)    \n"
        ".set reorder"
        :
        : "r" (p)
        : "memory"
    );
}

void func_00112EC0(u8 *obj, f32 value) {
    u8 *state = *(u8 **)(obj + 0x18);
    *(f32 *)(state + 0x8C) = value;
    *(u32 *)(state + 0x88) |= 1;
}

f32 func_00112ED8(u8 *obj) {
    return *(f32 *)(*(u8 **)(obj + 0x18) + 0x8C);
}

INCLUDE_ASM(const s32, "basic/dds3CameraObjectBasic", func_00112EE8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F6F8);

INCLUDE_RODATA(const s32, "basic/dds3CameraObjectBasic", D_0039F708);


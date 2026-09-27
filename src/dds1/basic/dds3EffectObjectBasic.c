#include "common.h"

typedef struct {
    void *unk0;  /* 0x0 object deref'd +0x10 by func_00111840, returned by func_00114A68 */
    u32 unk4;    /* 0x4 flag bits set/cleared by func_00115B88/func_00115BA0 */
    u8 pad8[4];  /* 0x8 */
    void *unkC;  /* 0xC bill object deref'd +0x2C/+0x58 by func_00151E60/func_00152200 */
} EffectData;

typedef struct {
    u8 pad[0x18];      /* 0x0 */
    EffectData *unk18; /* 0x18 */
} EffectObj;

void func_001143D8(void *arg);
void func_0010F5E8(void *arg);
void func_00111840(void *arg);
void func_002CFF98(void *arg);
/* Dispatchers take (bill handle, 16-byte vector, extra); the vector is
   loaded with lqc2 and the extra is forwarded to func_00114A78. */
void func_00114B18(void *arg0, void *vec, s32 arg2);
void func_00152200(void *arg);
void func_00114CE8(void *arg0, void *vec, s32 arg2);
void func_00114E90(void *arg0, void *vec, s32 arg2);
void func_00114FE0(void *arg0, void *vec, s32 arg2);
void func_001150F0(void);
void func_00115398(void);
void func_00115478(void);

extern void *func_0014FE28(void);

extern void *func_0014FE48(void);

extern void *func_0014FD20(void);

extern void *func_00151E08(s32 arg0, s32 arg1);

extern void *func_00151D88(s32 arg0, u32 arg1);

extern void *func_00151E60(void *arg);

void func_00114570(EffectObj *obj) {
    EffectData *data;

    data = obj->unk18;
    func_001143D8(data);
    func_0010F5E8(obj);
    func_00111840(data->unk0);
    func_002CFF98(obj->unk18);
    obj->unk18 = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001145C0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114988);

void *func_00114A68(EffectObj *obj) {
    return obj->unk18->unk0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114A78);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114B18);

void func_00114BF0(EffectObj *obj, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151E60(obj->unk18->unkC);
    func_00114B18(handle, vec, arg2);
}

void func_00114C38(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151D88(1, arg0);
    func_00114B18(handle, vec, arg2);
}

void func_00114C80(s32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151E08(1, arg0);
    func_00114B18(handle, vec, arg2);
}

void func_00114CC8(EffectObj *obj) {
    func_00152200(obj->unk18->unkC);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE8);

void func_00114DB8(EffectObj *obj, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151E60(obj->unk18->unkC);
    func_00114CE8(handle, vec, arg2);
}

void func_00114E00(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151D88(0, arg0);
    func_00114CE8(handle, vec, arg2);
}

void func_00114E48(s32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_00151E08(0, arg0);
    func_00114CE8(handle, vec, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114E90);

void func_00114F60(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FD20();
    func_00114E90(handle, vec, arg2);
}

void func_00114FA0(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FE48();
    func_00114E90(handle, vec, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114FE0);

void func_001150B0(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FE28();
    func_00114FE0(handle, vec, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F0);

void func_00115298(void) {
    func_001150F0();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001152B0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115318);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115478);

void func_00115840(void) {
    func_00115478();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115858);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158B8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158F0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115930);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115970);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001159E8);

void func_00115B88(EffectObj *obj, s32 flag) {
    obj->unk18->unk4 |= flag;
}

void func_00115BA0(EffectObj *obj, s32 flag) {
    obj->unk18->unk4 &= ~flag;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BB8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F7D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F800);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F828);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F878);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F890);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8C0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8E8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F900);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F918);

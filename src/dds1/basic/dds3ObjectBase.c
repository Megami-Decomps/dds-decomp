#include "common.h"

/* Object-base header returned by func_00112888. Flag bits at +0x0,
   indexed pointer slots at +0x10 exchanged by objExchangeSlot, and the
   sub-object at +0x30 dereferenced by objGetExtData. */
typedef struct {
    u32 unk0;       /* 0x0 flag bits */
    u32 unk4;       /* 0x4 read by objGetUnk04 */
    u32 unk8;       /* 0x8 mode switched by func_00111B40 */
    u32 unkC;       /* 0xC read by objGetUnk0C */
    void *unk10[8]; /* 0x10 slots indexed by objGetSlot */
    void *unk30;    /* 0x30 returned by objGetExtData */
    u8 pad34[4];    /* 0x34 */
    void *unk38;    /* 0x38 passed to func_002DB308 */
} ObjBase;

/* Inner object reached through +0x18 (see code_00111610 neighbors);
   func_00111B40 releases and clears unk8. */
typedef struct {
    u8 pad[4]; /* 0x0 */
    u32 unk4;  /* 0x4 */
    u32 unk8;  /* 0x8 */
    u32 unkC;  /* 0xC */
} ObjInner;

/* Value installed into a slot by objExchangeSlot; its +0xF byte selects
   the slot index via func_00111758. */
typedef struct {
    u8 pad[0xF]; /* 0x0 */
    u8 unkF;     /* 0xF */
} ObjData;

extern void *func_00111610(void *arg);
extern void *objGetSlot(void *arg0, s32 index);

extern ObjBase *func_00112888(void *obj);

s32 func_00111758(u8 arg);
void *objGetExtData(void *obj);
void *objSetSlotByKind(void *arg0, ObjData *arg1);
void *objExchangeSlot(void *arg0, void *arg1, s32 index);
void func_001111C8(void *arg0, void *arg1);
void func_00111698(void *arg0, void *arg1);
void func_001116A8(void *arg0, void *arg1);
void func_001116B8(void *arg0);
void func_002177D0(s32 arg0, s32 arg1);
void func_00222200(u32 arg0);
void func_002D7AC8(s32 arg0, s32 arg1, s32 arg2);
void func_002DB308(void *arg);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111840);

void func_00111908(void *obj, s32 flag) {
    ObjBase *base;

    base = func_00112888(obj);
    base->unk0 = base->unk0 | flag;
}

void func_00111938(void *obj, s32 flag) {
    ObjBase *base;

    base = func_00112888(obj);
    base->unk0 = base->unk0 & ~flag;
}

u8 func_00111970(void *obj, s32 flag) {
    ObjBase *base;

    base = func_00112888(obj);
    return (base->unk0 & flag) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001119A0);

void *objGetExtData(void *obj) {
    return func_00112888(obj)->unk30;
}

void *objSetSlotByKind(void *arg0, ObjData *arg1) {
    if (arg1 == NULL) {
        return NULL;
    }
    return objExchangeSlot(arg0, arg1, func_00111758(arg1->unkF));
}

void *objExchangeSlot(void *arg0, void *arg1, s32 index) {
    void *old;

    old = objGetSlot(arg0, index);
    func_00112888(arg0)->unk10[index] = arg1;
    return old;
}

void *objGetSlot(void *arg0, s32 index) {
    return func_00112888(arg0)->unk10[index];
}

u32 objGetUnk04(void *obj) {
    return func_00112888(obj)->unk4;
}

u32 objGetUnk0C(void *obj) {
    return func_00112888(obj)->unkC;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111B40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BD8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E30);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111F40);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112008);

s32 objInvokeSlot5Handler(void *arg0) {
    void *v;

    v = objGetSlot(arg0, 5);
    if (v == NULL) {
        return 0;
    }
    func_001111C8(v, arg0);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112100);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001122F0);

void func_00112750(void *arg0) {
    void *exists;
    void *val;

    exists = objGetSlot(arg0, 1);
    if (exists == NULL) {
        val = func_00111610(arg0);
        objSetSlotByKind(arg0, val);
        return;
    }
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001127A0);

s32 objInvokeSlot1Handler(void *arg0, void *arg1) {
    void *v;

    v = objGetSlot(arg0, 1);
    if (v == NULL) {
        return 0;
    }
    func_00111698(v, arg1);
    return 1;
}

void objRunSlot1Handlers(void *arg0, void *arg1) {
    void *v;

    v = objGetSlot(arg0, 1);
    func_001116A8(v, arg1);
    func_001116B8(arg0);
}

void objReleaseSlot1Data(void *arg0) {
    func_001116F8(objGetSlot(arg0, 1));
}

void objGetSlot1Data(void *arg0) {
    func_00111730(objGetSlot(arg0, 1));
}

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_003BA9C8);


#include "common.h"
#include "pcp_vu0.h"

extern u64 func_001579C8(void);

extern u64 func_001579E8(void);

extern u64 func_001578C0(void);

extern u64 billCreateFromResource(u64, u64);

extern u64 billCreateIndexed(u64, u64);

extern u64 func_00159A50(u32);

extern s32 func_00332D08(void *param, s32 id);

extern void effEventReleaseNode(void *node);

extern void *func_00197D68(void *bill, u32 id, void *vec);

extern void *func_00110AA8(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);

extern void *func_00343ED0(void *arg0, u32 *arg1, s32 arg2);

extern void *func_003297C8(void *arg);

typedef struct {
    void *objectHandle; /* 0x0 returned by effObjGetObjectHandle */
    u32 flags; /* 0x4 effect flag bits */
    s32 unk8;    /* 0x8 cleared ^6 by func_001158B8 */
    void *bill; /* 0xC bill object */
    u32 unk10;   /* 0x10 cleared by func_00115BB8 */
    u32 unk14;   /* 0x14 cleared by func_00115BB8 */
    void *unk18; /* 0x18 cleared by func_00115BB8 */
    u8 pad1C[4]; /* 0x1C */
    void *unk20; /* 0x20 set to the owner by func_00115BB8 */
    u16 unk24;   /* 0x24 */
    u16 unk26;   /* 0x26 kind copied from +0xF */
    void *unk28; /* 0x28 vector passed to func_00197D68 */
    void *unk2C; /* 0x2C node handle owned by func_00115580 */
} EffectData; /* 0x30 bytes */

typedef struct {
    u8 pad0[0x60];
    f32 parameter;
} EffectParameters;

typedef struct {
    u8 pad0[4];       /* 0x0 */
    u32 unk4;         /* 0x4 world counter copied by func_00114CE0 */
    u8 pad8[7];       /* 0x8 */
    u8 unkF;           /* 0xF kind checked ==7 by func_001158B8 */
    u8 pad10[8];       /* 0x10 */
    EffectData *data; /* 0x18 */
    EffectParameters *params; /* 0x1C vector base; scalar parameter at +0x60 */
} EffectObj;

EffectData *func_00115B20(EffectObj *obj);

/* Release the effect's dependent resources before clearing its data handle. */
void effObjReleaseObjectData(u32 object) {
    u32 *data;
    s32 objectAddress;

    objectAddress = (s32)object;
    data = *(u32 **)(objectAddress + 0x18);
    func_00114640(data);
    effObjFreeInner(object);
    func_00111A68(*data);
    func_00328E48(*(u32 *)(objectAddress + 0x18));
    *(u32 *)(objectAddress + 0x18) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114BF0);

u32 effObjGetObjectHandle(EffectObj *obj) {
    return (u32)obj->data->objectHandle;
}

EffectObj *func_00114CE0(u32 arg0, void *arg1, void *arg2) {
    EffectObj *obj;
    EffectData *data;

    obj = func_00110AA8(7);
    if (obj == NULL) {
        return NULL;
    }
    obj->unk4 = arg0;
    dds3EnsureSlotData(obj);
    effObjSetInnerFirstVec(obj, arg1);
    effObjSetInnerSecondVec(obj, arg2);
    effObjInnerVecBackup(obj->params);
    data = obj->data;
    data->flags = 0;
    data->unk8 = 0;
    data->bill = NULL;
    data->unk20 = NULL;
    data->unk24 = 0;
    data->unk26 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114D80);

/* Resolve the object's billboard and forward its two draw arguments. */
void func_00114E58(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114D80(bill, arg1, arg2);
}
/* Create an indexed billboard of kind one and dispatch it. */
void effObjCreateIndexedKindOne(u64 billId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateIndexed(1, billId);
    func_00114D80(bill, arg1, arg2);
}
/* Create a resource-backed billboard of kind one and dispatch it. */
void effObjCreateResourceKindOne(u64 resourceId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateFromResource(1, resourceId);
    func_00114D80(bill, arg1, arg2);
}

/* Select the billboard object's kind-one entry. */
void func_00114F30(EffectObj *obj) {
    billSetKind1Entry((u32)obj->data->bill);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114F50);

/* Resolve the object's billboard and dispatch through the second handler. */
void func_00115020(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114F50(bill, arg1, arg2);
}

/* Create an indexed billboard of kind zero for the second handler. */
void effObjCreateIndexedKindZero(u64 billId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateIndexed(0, billId);
    func_00114F50(bill, arg1, arg2);
}

/* Create a resource-backed billboard of kind zero for the second handler. */
void effObjCreateResourceKindZero(u64 resourceId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateFromResource(0, resourceId);
    func_00114F50(bill, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F8);

/* Dispatch a newly allocated handle from the first parameter source. */
void func_001151C8(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001578C0();
    func_001150F8(handle, arg1, arg2);
}

/* Dispatch a newly allocated handle from the second parameter source. */
void func_00115208(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001579E8();
    func_001150F8(handle, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115248);

/* Dispatch a newly allocated handle from the third parameter source. */
void func_00115318(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001579C8();
    func_00115248(handle, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115358);

void func_00115500(void) {
    func_00115358();
}

void *func_00115518(void *arg0, void *arg1, void *arg2) {
    u32 local;
    void *result;
    void *result2;

    local = 0;
    result = func_00343ED0(arg0, &local, 0);
    result2 = func_00115358(local, arg1, arg2);
    func_003297C8(result);
    return result2;
}

void func_00115580(EffectObj *obj, u32 arg1) {
    EffectData *data;

    data = obj->data;
    if (data->unk8 != 5) {
        return;
    }
    if (data->unk28 == NULL) {
        return;
    }
    if (data->bill == NULL) {
        return;
    }
    if (data->unk2C != NULL) {
        effEventReleaseNode(data->unk2C);
        data->unk2C = NULL;
    }
    data->unk2C = func_00197D68(data->bill, arg1 & 0xFFFF, data->unk28);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115600);

void func_001156C8(void) {
    func_00115600();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001156E0);

void func_00115AA8(void) {
    func_001156E0();
}

void *func_00115AC0(s32 kind, void *arg1) {
    u32 local;
    void *result;
    void *result2;

    local = 0;
    result = func_00343ED0(arg1, &local, 0);
    result2 = func_001156E0(kind, local);
    func_003297C8(result);
    return result2;
}

EffectData *func_00115B20(EffectObj *obj) {
    EffectData *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (obj->unkF != 7) {
        return data;
    }
    data = obj->data;
    if (data->unk8 != 6) {
        data = NULL;
    }
    return data;
}

s32 func_00115B58(EffectObj *obj) {
    if (func_00115B20(obj) == NULL) {
        return 0;
    }
    VU0_LOAD_VF(vf10, (u8 *)obj->params + 0x40);
    return 1;
}

/* Return the object's scalar parameter as an integer when data is present. */
s32 effObjGetIntParam(EffectObj *obj) {
    EffectParameters *parameters;

    if (func_00115B20(obj) == NULL) {
        return 0;
    }
    parameters = obj->params;
    return (s32)parameters->parameter;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BD8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C50);

void effObjSetFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags | flags;
}

void effObjClearFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags & ~flags;
}

s32 func_00115E20(EffectObj *arg0, EffectObj *arg1) {
    EffectData *data;
    u8 kind;

    kind = arg1->unkF;
    if (kind < 4 || (kind >= 10 && kind != 0x11)) {
        return 0;
    }
    data = arg0->data;
    data->unk18 = NULL;
    data->unk20 = arg1;
    data->unk24 = 0;
    data->flags |= 4;
    data->flags &= ~8;
    data->unk10 = 0;
    data->unk26 = arg1->unkF;
    data->unk14 = 0;
    return 1;
}

/* Billboard payload: state word and the SDF parameter block it was built from. */
typedef struct {
    u8 pad0[8];   /* 0x0 */
    u32 unk8;     /* 0x8 */
    void *unkC;   /* 0xC */
} BillPayload;

typedef struct {
    u8 pad0[0x18]; /* 0x0 */
    void *unk18;   /* 0x18 */
} BillParam;

void func_00115E88(EffectObj *obj) {
    EffectData *data;
    EffectObj *owner;
    BillPayload *bill;

    data = obj->data;
    if (data->flags & 8) {
        owner = data->unk20;
        if (owner->unkF == 5) {
            bill = owner->data->bill;
            if (bill->unk8 != 0) {
                return;
            }
            func_00332D08(((BillParam *)bill->unkC)->unk18, data->unk24);
        }
    }
}

s32 func_00115EE8(EffectObj *obj, EffectObj *arg1, s32 arg2) {
    EffectData *data;

    if (func_00115E20(obj, arg1) == 0) {
        return 1;
    }
    data = obj->data;
    data->unk24 = arg2;
    data->flags |= 8;
    return 1;
}

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412950);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412980);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129F8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A10);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A28);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A40);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A68);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A98);


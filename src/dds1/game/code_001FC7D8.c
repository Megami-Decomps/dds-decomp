#include "common.h"

extern s32 func_001A2FD8(s32, s32);
extern void effObjFetchInnerFirstVec(s32);
extern s32 func_002D9E98(s32, s32);
extern void func_0011E280(s32, f32, f32, f32, f32);

extern s32 func_001A17F0();
extern s8 D_003BB870;
extern s32 D_003BB87C;
extern s32 func_002011C8(s32, s32);
extern s32 func_002CFF68(s32);
extern void func_002CFF98(s32);

extern u32 D_003BB874;

extern void func_00202668(s32, s32);
extern void func_00202F90(s32, s32);
extern void func_00203248(s32, s32);
extern void func_00203098();
extern void func_00203A80(s32, s32);
extern void func_00203BA8(s32, s32);
extern void func_002041A0();
extern void func_002033C0();
extern void func_002034D0();
extern void func_002035E0();
extern void func_00204008();
extern void func_00204028();
extern void func_00203F98();
extern void func_00203CA8();
extern void func_00204048(s32, s32);
extern void func_00204080();
typedef struct EffChildCounters {
    u8 pad00[0x318];
    u8 firstCountdown;
    u8 secondCountdown;
} EffChildCounters;

typedef struct EffCounterOwner {
    u8 pad00[0x20];
    EffChildCounters *child;
} EffCounterOwner;


extern s32 (*D_00360D10[])(s32, u32);
extern s32 btlWaitEffectTask();
extern void func_001FDA78();
extern void func_001FE820();
extern s32 func_001ADB30();
extern void func_001ADB78(s32, s32);
extern void func_0019C590(s32, s32, s32, s32);
extern void func_003003F0(const char *fmt, ...);
extern s8 D_00324510[];
INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC7D8);

void func_001FC990(void) {
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FC998);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCAC0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCB50);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57E0);

INCLUDE_RODATA(const s32, "game/code_001FC7D8", D_003A57F0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCBA0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FCFB8);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD170);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD3C0);

typedef struct BtlEffOwner {
    u8 pad00[0x108];
    u64 ownerData;    /* 0x108: copied to the effect object */
    s32 flags;        /* 0x110: bit 9 controls the returned offset */
} BtlEffOwner;

/* Adds one to the base unless bit 9 of the owner's flags is set. */

s32 effOffsetIfOwnerFlagClear(BtlEffOwner *owner, s32 base) {
    return base + (((owner->flags >> 9) ^ 1U) & 1);
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD5C8);

void effDecrementFirstCountdown(EffCounterOwner *owner) {
    u8 remaining;
    EffChildCounters *child;

    child = owner->child;
    remaining = child->firstCountdown;
    if (remaining != 0) {
        child->firstCountdown = remaining + 0xff;
    }
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FD9B0);

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDA78);

void effDecrementSecondCountdown(EffCounterOwner *owner) {
    u8 remaining;
    EffChildCounters *child;

    child = owner->child;
    remaining = child->secondCountdown;
    if (remaining != 0) {
        child->secondCountdown = remaining + 0xff;
    }
}

typedef struct BtlObjLink {
    void *owner;
    s32 arg;
    s32 unk8;
} BtlObjLink;

typedef struct BtlEffObj {
    u8 kind;          /* 0x00 */
    u8 pad01[0xF];
    u8 unk10;         /* 0x10 */
    u8 pad11[0xF];
    s16 id;           /* 0x20 */
    u8 pad22[2];
    u16 flags;        /* 0x24 */
    u8 pad26[0x1A];
    u64 ownerData;    /* 0x40 */
    u8 pad48[4];
    void (*update)(); /* 0x4C */
    void (*destroy)(); /* 0x50 */
} BtlEffObj;

extern BtlEffObj *btlAllocTask(s32);
extern BtlObjLink *func_001D47D8(BtlEffObj *);

typedef struct BtlExtendedLink {
    u8 pad00[0x20];
    BtlEffOwner *owner; /* 0x20 */
    s32 state;          /* 0x24 */
    u8 parameter;       /* 0x28 */
} BtlExtendedLink;

BtlEffObj *btlCreateEffectCounterTask(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0x2C);
    BtlExtendedLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3B;
    obj->ownerData = owner->ownerData;
    obj->update = func_001FDA78;
    obj->destroy = effDecrementSecondCountdown;
    link = (BtlExtendedLink *)func_001D47D8(obj);
    link->parameter = arg;
    link->owner = owner;
    link->state = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FDF98);

extern void func_001FDF98();

BtlEffObj *btlCreateEffObjD(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x40;
    obj->ownerData = owner->ownerData;
    obj->update = func_001FDF98;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE118);

extern void func_001FE118();

BtlEffObj *func_001FE198(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x41;
    obj->ownerData = owner->ownerData;
    obj->update = func_001FE118;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE228);

extern void func_001FE228();

BtlEffObj *btlCreateEffObjA(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x42;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->update = func_001FE228;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

typedef struct BtlJyokyoOwner {
    u8 pad00[0x120];
    u16 flags;       /* 0x120 */
    u8 pad122[2];
    u16 unk124;      /* 0x124 */
} BtlJyokyoOwner;

typedef struct BtlJyokyoState {
    u8 pad00[0x498];
    s32 id;          /* 0x498 */
} BtlJyokyoState;

s32 btlJyokyoEffectUpdate(BtlObjLink *link) {
    BtlJyokyoState *state = (BtlJyokyoState *)func_001A17F0();
    BtlJyokyoOwner *owner = link->owner;

    if (link->unk8 == 0) {
        if (owner != NULL) {
            func_0019C590(state->id, 0, owner->unk124, (owner->flags & 0x20) ? 0xE : 0xF);
        }
        func_003003F0("JYOKYO ID : %d\n", state->id);
        func_001ADB78(state->id, link->arg);
    }
    if (func_001ADB30() == 0 || (u32)link->unk8 >= 0x2D) {
        return 1;
    }
    link->unk8++;
    return 0;
}

BtlEffObj *btlCreateEffObjB(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x3C;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->update = btlJyokyoEffectUpdate;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE500);

extern void func_001FE500();

BtlEffObj *btlCreateEffObjC(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x3D;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->update = func_001FE500;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

u32 func_001FE658(void) {
    return 1;
}

BtlEffObj *btlCreateEffectTask3E(BtlEffOwner *owner, u16 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3E;
    obj->ownerData = owner->ownerData;
    obj->update = func_001FE658;
    link = func_001D47D8(obj);
    link->owner = owner;
    *(u16 *)&link->arg = arg;
    link->unk8 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE6F0);

extern void func_001FE6F0();

BtlEffObj *func_001FE790(BtlEffOwner *owner, s32 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x3F;
    obj->ownerData = owner->ownerData;
    obj->update = func_001FE6F0;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = arg;
    link->unk8 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FE820);

BtlEffObj *btlCreateEffectWaitTask(BtlEffOwner *owner, u16 arg) {
    BtlEffObj *obj = btlAllocTask(0xC);
    BtlObjLink *link;

    obj->kind = 1;
    obj->id = 0x43;
    obj->flags |= 2;
    obj->unk10 = 0;
    if (owner != NULL) {
        obj->ownerData = owner->ownerData;
    }
    obj->update = func_001FE820;
    link = func_001D47D8(obj);
    link->owner = owner;
    *(u16 *)&link->arg = arg;
    link->unk8 = 0;
    return obj;
}

typedef struct BtlWaitTask {
    s32 value;
    u32 ticks;
} BtlWaitTask;

/* Waits for the task startup delay, then finishes when its two actor slots are clear. */

s32 btlWaitEffectTask(BtlWaitTask *task) {
    if (task->ticks == 0) {
        func_001AD230(task->value, 0);
        func_001BCB88(0, 8);
    }
    if (task->ticks >= 0x11) {
        if (func_001AD3F0() == 1) {
            if (D_00324510[0x21] < 0 || D_00324510[0x23] < 0) {
                func_001BCB88(1, 8);
                func_001AD428();
                return 1;
            }
        }
    } else {
        task->ticks += 1;
    }
    return 0;
}

BtlEffObj *btlCreateEffectTask44(BtlEffOwner *owner) {
    BtlEffObj *obj = btlAllocTask(8);
    BtlObjLink *link;

    obj->kind = 1;
    obj->unk10 = 0;
    obj->flags |= 2;
    obj->id = 0x44;
    obj->ownerData = owner->ownerData;
    obj->update = btlWaitEffectTask;
    link = func_001D47D8(obj);
    link->owner = owner;
    link->arg = 0;
    return obj;
}

u32 btlNextScaledRandom(u32 limit) {
    D_003BB874 = D_003BB874 * 0x41c64e6d + 0x3039;
    return (D_003BB874 >> 0x10) * (limit & 0xffff) >> 0x10;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEAA8);

s32 btlAllocAndCheck(s32 object) {
    s32 allocation = func_002CFF68(0x10);
    s32 actor = *(s32 *)(object + 0x18);

    D_003BB87C = allocation;
    *(s32 *)allocation = object;
    if (func_002011C8(actor, 0) != 0) {
        func_002CFF98(D_003BB87C);
        return 1;
    }
    func_002CFF98(D_003BB87C);
    return 0;
}

u32 func_001FEB78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xb;
    *(u32 *)(arg0 + 0x24) = 0xc2;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEB90);

void btlClearNodeFlags(void) {
    s32 node = *(s32 *)(func_001A17F0() + 0x224);

    if (node != NULL) {
        do {
            if (*(s32 *)(node + 0x18) != 0) {
                *(u8 *)(node + 0x146) = 0;
            }
            node = *(s32 *)(node + 0x16C);
        } while (node != NULL);
    }
    D_003BB870 = 0;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FEC68);

/* Top ten bits select the callback; the lower 22 bits are its argument. */
s32 btlDispatchPackedEffectAction(s32 context, u32 packedAction) {
    u32 type = packedAction >> 22;
    s32 result = 0;

    packedAction &= 0x3FFFFF;
    if (type != 0) {
        result = D_00360D10[type](context, packedAction) != 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001FC7D8", func_001FED20);

void func_001FEDE0(u8 *work, s8 flag) {
    s32 *slot;
    s32 i;

    for (i = 6, slot = (s32 *)(work + 0x164); i >= 0; i--, slot--) {
        *slot = slot[-1];
    }
    if (flag == 0) {
        *(s32 *)(work + 0x148) = *(s32 *)(work + 0x24);
    } else {
        *(s32 *)(work + 0x148) = *(s16 *)(*(u8 **)(work + 0x18) + 0x2B0);
    }
}

void btlCmdWithArgA(s32 arg0) {
    func_00202668(arg0, 0);
}

void btlCmdWithArgB(s32 arg0) {
    func_00202F90(arg0, 0);
}

void btlCmdWithArgC(s32 arg0) {
    func_00203248(arg0, 0);
}

void btlCmdSimpleA(void) {
    func_00203098();
}

void btlCmdWithArgD(s32 arg0) {
    func_00203A80(arg0, 0);
}

void btlCmdWithArgE(s32 arg0) {
    func_00203BA8(arg0, 0);
}

void btlCmdSimpleB(void) {
    func_002041A0();
}

void btlCmdSimpleC(void) {
    func_002033C0();
}

void btlCmdSimpleD(void) {
    func_002034D0();
}

void btlCmdSimpleE(void) {
    func_002035E0();
}

void btlCmdSimpleF(void) {
    func_00204008();
}

void btlCmdSimpleG(void) {
    func_00204028();
}

void btlCmdSimpleH(void) {
    func_00203F98();
}

void btlCmdSimpleI(void) {
    func_00203CA8();
}

void btlCmdWithArgF(s32 arg0) {
    func_00204048(arg0, 0);
}

void btlCmdSimpleJ(void) {
    func_00204080();
}

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB840);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB848);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB850);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB858);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB860);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB868);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB870);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB874);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB878);

INCLUDE_SDATA(const s32, "game/code_001FC7D8", D_003BB87C);


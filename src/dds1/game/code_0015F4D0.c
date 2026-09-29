#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

extern BillDispatch D_0034E658[];

extern BillDispatch D_0034E654[];

typedef struct BillWork {
    u8 pad00[0x64];
    u8 currentValue;
    u8 pad65[0x4B];
    u16 pendingCount;
    u16 queuedCount;
    u8 padB4[0xC];
    u8 stagedValue;
} BillWork;

typedef struct BillEntry {
    u8 pad00[0x10];
    u32 value;
} BillEntry;

typedef struct BillEntryOwner {
    u8 pad00[0x14];
    BillEntry *entries;
} BillEntryOwner;

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F4D0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F520);

void func_0015F578(BillObj *obj) {
    D_0034E658[*(u16 *)((u8 *)obj + 0xB0)].func();
}

void func_0015F5B0(BillObj *obj) {
    D_0034E654[*(u16 *)((u8 *)obj + 0xB0)].func();
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F5E8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F630);

u16 func_0015F670(BillWork *work) {
    return work->queuedCount;
}

void func_0015F678(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F688);

void func_0015F6E8(BillObj *effect, void *value) {
    effect->unk60 = value;
}

void effBillSetWorkValue(BillWork *work, u8 value) {
    if (work->pendingCount == 0) {
        work->stagedValue = value;
    }
    work->currentValue = value;
}

u8 func_0015F708(BillWork *work) {
    return work->currentValue;
}

void func_0015F710(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F718);

void effBillSetEntryValue(BillEntryOwner *owner, s32 index, u32 value) {
    owner->entries[index].value = value;
}

s32 func_0015F810(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F818);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F890);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F9C8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FB88);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FC48);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FD98);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FE20);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160210);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001602F8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160690);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001606C0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160800);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160858);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160888);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001608B8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160910);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160958);

INCLUDE_RODATA(const s32, "game/code_0015F4D0", D_003A0DB8);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB018);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB01C);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB020);


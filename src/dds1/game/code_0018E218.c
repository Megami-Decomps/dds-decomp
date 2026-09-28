#include "common.h"

/* Parameter blocks copied by the setters below. Sizes are exact: the 0x18
 * pair (D_00355930/D_00355F88), the 0x24 block (D_00356088), the 0x2C triple
 * (D_00355AF8/D_00355C70/D_003561C8) and the 0x30 quad
 * (D_00355880/D_00355908/D_003559A0/D_00355E48). */
typedef struct Work18 {
    u8 data[0x18];
} Work18;

typedef struct Work24 {
    u8 data[0x24];
} Work24;

typedef struct Work2C {
    u8 data[0x2C];
} Work2C;

typedef struct Work30 {
    u8 data[0x30];
} Work30;
/* Slot addressed by func_0018E660/effInitSlotTail with a 0x60 stride. Only the
 * tail is known: two words cleared and a float reset to 0.05f. */
typedef struct Slot60 {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    f32 unk5C;
} Slot60;

typedef struct SlotTab {
    Slot60 *slots;
} SlotTab;
/* Common prefix copied by the per-type setters (effCopyCh71Common and friends).
 * The trailing word is real: gcc emits lw/sw for it, so it cannot be part
 * of the byte blob. BD808/BD804/BD810 use the 0x2C form, BD80C the 0x24. */
typedef struct BDCommon2C {
    u8 data[0x28];
    u32 unk28;
} BDCommon2C;

typedef struct BDWork2C {
    BDCommon2C common;
    u32 unk2C;
} BDWork2C;

typedef struct BDCommon24 {
    u8 data[0x20];
    u32 unk20;
} BDCommon24;

typedef struct BDWork24 {
    BDCommon24 common;
    u32 unk24;
} BDWork24;

extern Work30 D_00355880;
extern Work30 D_00355908;
extern Work18 D_00355930;
extern Work30 D_003559A0;
extern Work2C D_00355AF8;
extern Work2C D_00355C70;
extern Work30 D_00355E48;
extern Work18 D_00355F88;
extern Work24 D_00356088;
extern Work2C D_003561C8;

extern BDWork24 *D_003BD80C;

extern s8 D_003BB075;

extern s8 D_003BB074;

extern s8 D_003BB073;

extern BDWork2C *D_003BD810;

extern s8 D_003BB076;

extern BDWork2C *D_003BD804;

extern s8 D_003BB072;

extern BDWork2C *D_003BD808;

extern s8 D_003BB071;

extern s8 D_003BB070;
extern u32 func_00151FC8(s32 arg);
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];

extern BDWork2C *func_00186C18(void *arg);
extern BDWork2C *func_00186F90(void *arg);
extern BDWork24 *func_00187FC0(void *arg);
extern BDWork2C *func_00187460(void *arg);
extern void func_00186498(Work30 *arg);
extern void func_00186CD0(BDWork2C *arg);
extern void func_00187098(BDWork2C *arg);
extern void func_00187598(BDWork2C *arg);
extern void func_00187988(Work30 *arg);
extern void func_00187C08(Work18 *arg);
extern void func_00188068(BDWork24 *arg);
INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E218);

void func_0018E2A8(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E2C0);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E408);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E548);

void effInitSlotTail(SlotTab *tab, s32 idx) {
    Slot60 *slot = &tab->slots[idx];

    slot->unk5C = 0.05f;
    slot->unk54 = slot->unk58 = 0;
}

s32 func_0018E660(SlotTab *table, s32 index) {
    return (s32)&table->slots[index];
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E678);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E740);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E810);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E938);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018EA68);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018EB08);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018ED80);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018EED0);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F2E0);

void func_0018F3A0(void) {
    D_003BB070 = 1;
}

void func_0018F3B0(void) {
    D_003BB070 = 0;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F3B8);

Work30 *effGetCh70Params(void) {
    return &D_00355880;
}

void func_0018F428(void) {
    D_003BB071 = 1;
}

void func_0018F438(void) {
    D_003BB071 = 0;
}

void effCopyCh71Common(BDCommon2C *src) {
    D_003BD808->common = *src;
}

BDWork2C *effGetCh71Work(void) {
    return D_003BD808;
}

void effSetCh71Id(u32 arg) {
    D_003BD808->unk2C = arg;
}

void effInitCh71Id(void) {
    D_003BD808->unk2C = func_00151FC8(2);
}

void func_0018F4E0(void) {
    D_003BB072 = 1;
}

void func_0018F4F0(void) {
    D_003BB072 = 0;
}

void effCopyCh72Common(BDCommon2C *src) {
    D_003BD804->common = *src;
}

BDWork2C *effGetCh72Work(void) {
    return D_003BD804;
}

void effSetCh72Id(u32 arg) {
    D_003BD804->unk2C = arg;
}

void effInitCh72Id(void) {
    D_003BD804->unk2C = func_00151FC8(2);
}

void func_0018F598(void) {
    D_003BB076 = 1;
}

void func_0018F5A8(void) {
    D_003BB076 = 0;
}

void effCopyCh76Common(BDCommon2C *src) {
    D_003BD810->common = *src;
}

BDWork2C *effGetCh76Work(void) {
    return D_003BD810;
}

void effSetCh76Id(u32 arg) {
    D_003BD810->unk2C = arg;
}

void effInitCh76Id(void) {
    D_003BD810->unk2C = func_00151FC8(3);
}

void func_0018F650(void) {
    D_003BB073 = 1;
}

void func_0018F660(void) {
    D_003BB073 = 0;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F668);

Work30 *effGetCh73Params(void) {
    return &D_00355908;
}

void func_0018F6D8(void) {
    D_003BB074 = 1;
}

void func_0018F6E8(void) {
    D_003BB074 = 0;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F6F0);

Work18 *effGetCh74Params(void) {
    return &D_00355930;
}

void func_0018F740(void) {
    D_003BB075 = 1;
}

void func_0018F750(void) {
    D_003BB075 = 0;
}

void effCopyCh75Common(BDCommon24 *src) {
    D_003BD80C->common = *src;
}

BDWork24 *effGetCh75Work(void) {
    return D_003BD80C;
}

void effSetCh75Id(u32 arg) {
    D_003BD80C->unk24 = arg;
}

void effInitCh75Id(void) {
    D_003BD80C->unk24 = func_00151FC8(0);
}

void effInitWorks(void) {
    D_003BD808 = func_00186C18(D_003558D8);
    D_003BD804 = func_00186F90(D_003558A8);
    D_003BD80C = func_00187FC0(D_00355948);
    D_003BD810 = func_00187460(D_00355970);
    *(s32 *)effGetCh76Work() = 4;
}

void effDispatchActive(void) {
    if (D_003BB070) {
        func_00186498(&D_00355880);
    }
    if (D_003BB071) {
        func_00186CD0(D_003BD808);
    }
    if (D_003BB072) {
        func_00187098(D_003BD804);
    }
    if (D_003BB076) {
        func_00187598(D_003BD810);
    }
    if (D_003BB073) {
        func_00187988(&D_00355908);
    }
    if (D_003BB074) {
        func_00187C08(&D_00355930);
    }
    if (D_003BB075) {
        func_00188068(D_003BD80C);
    }
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F8F8);

Work30 *effGetLoadDescA(void) {
    return &D_003559A0;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018F9C0);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FA20);

Work2C *effGetLoadDescB(void) {
    return &D_00355AF8;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FAE8);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FB50);

Work2C *effGetLoadDescC(void) {
    return &D_00355C70;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FC18);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FC80);

Work30 *effGetLoadDescD(void) {
    return &D_00355E48;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FD48);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FDA8);

Work18 *effGetLoadDescE(void) {
    return &D_00355F88;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FE70);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FEB0);

Work24 *effGetLoadDescF(void) {
    return &D_00356088;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FF78);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018FFD0);

Work2C *effGetLoadDescG(void) {
    return &D_003561C8;
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_00190098);

void func_00190100(void) {
    func_001606C0();
}

void func_00190118(void) {
    func_00160800();
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_00190130);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB068);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB06C);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB070);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB071);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB072);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB073);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB074);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB075);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB076);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB078);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB080);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB088);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB090);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB098);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0A0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0A8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0B0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0B8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0C0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0C8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0D0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0D8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0E0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0E8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0F0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0F8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB100);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB108);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB110);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB114);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB118);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB120);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB128);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB12C);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB130);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB138);


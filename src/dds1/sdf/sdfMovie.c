#include "common.h"

typedef struct MovSub {
    u8 pad0[0x4];
    u32 unk4;
    u8 pad8[0x4];
    u32 unkC;
    s32 unk10;
    u8 pad14[0x3C];
    s32 unk50;
    u8 pad54[0x8];
    s32 unk5C;
    u8 pad60[0x8];
    s32 unk68;
    s32 unk6C;
} MovSub;

typedef struct MovObj {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 pad3;
    u8 pad4[0xC];
    s32 unk10;
    u8 pad14[0x4];
    s32 unk18;
    MovSub *unk1C;
} MovObj;

/* Called with 3 args (func_002ED198) and 4 args (func_002ECF70); keep K&R. */
void func_002E6D48();

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ECF70);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED008);

void func_002ED198(MovObj *arg0) {
    MovSub *t;
    s32 n;

    t = arg0->unk1C;
    n = arg0->unk18;
    if (n == 0) {
        return;
    }
    if ((0x10000 - t->unk5C) < 0x4000 || ((t->unk6C - t->unk68) + 0x10000) < 0x4000) {
        arg0->unk1 = 5;
        return;
    }
    arg0->unk1 = 4;
    func_002E6D48(arg0->unk10, t->unk50, n <= 0x4000 ? n : 0x4000);
}

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED230);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED5C0);

INCLUDE_ASM(const s32, "sdf/sdfMovie", func_002ED740);

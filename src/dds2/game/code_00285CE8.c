#include "common.h"

extern void func_0010AE38();

extern void mdlFlagClear(s32 flag);

extern void mdlFlagSet(s32 flag);

extern s32 mdlFlagTest(s32 flag);

extern void func_002663D8(void);

extern void func_00290E48(s32 arg);

extern void func_00286BA8(void *record);

extern void func_00308808(s32, s32, s32, s32, s32, s32, s32);

/* Mantra record zeroed before each update call (0x1C4). */
typedef struct {
    u8 pad0[4];     /* 0x0 */
    u16 unk4;       /* 0x4 */
    u8 pad6[0x1BE]; /* 0x6 */
} MtrRecord;

/* Sprite grid geometry: cell counts at +0x1C/+0x1E. */
typedef struct {
    u8 pad0[0x1C]; /* 0x0 */
    s16 unk1C;     /* 0x1C */
    s16 unk1E;     /* 0x1E */
} MtrGrid;

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285CE8);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285D78);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285E98);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002860D8);

void func_00286270(s32 arg0, s32 arg1, s32 arg2, s32 arg3, MtrGrid *arg4, s32 arg5, s32 arg6) {
    func_00308808(arg0 << 4, arg1 << 3, arg2, arg4->unk1C << 4, arg4->unk1E << 3, arg3 | 0x80808000, arg6);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002862B0);

INCLUDE_RODATA(const s32, "game/code_00285CE8", D_00425DD8);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286350);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286410);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002864D0);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286530);

void func_002865A8(void) {
    s32 i;

    for (i = 0; i < 0x70; i++) {
        mdlFlagClear(i + 0x920);
    }
    for (i = 0; i < 6; i++) {
        mdlFlagClear(i + 0x9A0);
    }
    func_002663D8();
    func_0010AE38("*****************[mtrMantraEventBitReset()]*****************\n");
}

void func_00286618(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 2;
    func_00286BA8(&record);
    func_00290E48(0);
    if (mdlFlagTest(0x9A0)) {
        mdlFlagSet(0x9A1);
    }
}

void func_00286670(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 1;
    func_00286BA8(&record);
    func_00290E48(1);
    if (mdlFlagTest(0x9A1)) {
        mdlFlagSet(0x9A0);
    }
}

void func_002866C8(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 3;
    func_00286BA8(&record);
}

void func_00286700(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 7;
    func_00286BA8(&record);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286738);

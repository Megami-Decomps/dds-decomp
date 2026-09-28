#include "common.h"

typedef struct DevConsState {
    u8 pad0[4]; /* 0x0 */
    struct DevConsState *unk4; /* 0x4 */
    u16 unk8; /* 0x8 */
    u16 unkA; /* 0xA */
    s16 columns; /* 0xC */
    s16 rows; /* 0xE */
    u16 cursorColumn; /* 0x10 */
    s16 cursorRow; /* 0x12 */
    u8 unk14; /* 0x14 */
    u8 pad15; /* 0x15 */
    u8 unk16; /* 0x16 */
    u8 unk17; /* 0x17 */
    u8 pad18[4]; /* 0x18 */
    u8 *cells; /* 0x1C: two bytes per character cell */
} DevConsState;

typedef struct ConsBuf {
    u8 *unk0; /* 0x0 */
    u8 *unk4; /* 0x4 */
    u8 *unk8; /* 0x8 */
    void *unkC; /* 0xC */
    s32 unk10[2]; /* 0x10 */
    s32 unk18; /* 0x18 */
    u16 unk1C; /* 0x1C */
    u16 unk1E; /* 0x1E */
} ConsBuf;

void func_002E3F58(DevConsState *arg0, s32 arg1, s32 arg2);
void func_002E41B8(ConsBuf *arg0);
void func_002E4228(ConsBuf *arg0);
void func_002E42F8(DevConsState *arg0, ConsBuf *arg1);
void func_002E4428(DevConsState *arg0, ConsBuf *arg1);
void func_002E45D0(ConsBuf *arg0, void *arg1, s32 arg2);
s64 func_002D2468(s32 arg0, void *arg1);
void func_00305B08(void *arg0, const char *arg1, void *arg2);
s32 sceDmaSync(void *ch, s32 mode, s32 timeout);
void sceDmaSendN(void *ch, void *addr, s32 size);
s32 sceGsSyncPath(s32 mode, s32 timeout);
void *sceDmaGetChan(s32 id);
extern s32 D_003BDA34;
extern DevConsState *D_003BD3C0;
extern u32 D_00398660[];
extern u32 D_003987E0[];
extern void *memmove(void *dst, const void *src, u32 n);
extern void *memset(void *dst, s32 c, u32 n);

void sdfDevConsAdvanceRow(DevConsState *console) {
    if (console->cursorRow == console->rows - 1) {
        s32 rowBytes = console->columns * 2;
        s32 copyBytes = rowBytes * console->cursorRow;
        if (console->cursorRow > 0) {
            memmove(console->cells, console->cells + rowBytes, copyBytes);
        }
        memset(console->cells + copyBytes, 0, rowBytes);
    } else {
        console->cursorRow = (u16)console->cursorRow + 1;
    }
    console->cursorColumn = 0;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E3F58);

void func_002E40D0(DevConsState *arg0, s32 arg1, s32 arg2) {
    func_002E3F58(arg0, arg1, arg2);
}

s32 func_002E40E8(DevConsState *arg0, const char *fmt, ...) {
    char buf[0x200];
    __builtin_va_list ap;
    char *p;
    s32 i = 0;
    s32 c;
    __builtin_stdarg_start(ap, fmt);
    func_00305B08(buf, fmt, ap);
    c = buf[0];
    if (c != 0) {
        do {
            i++;
            func_002E3F58(arg0, c, arg0->unk16);
            p = buf + i;
            c = *p;
        } while (c != 0);
    }
    return i;
}

void func_002E4190(DevConsState *console, s16 column, s16 row) {
    console->cursorColumn = column;
    console->cursorRow = row;
}

void func_002E41A0(DevConsState *arg0, u8 arg1) {
    arg0->unk16 = arg1;
}

u8 func_002E41A8(DevConsState *arg0) {
    return arg0->unk14;
}

void func_002E41B0(DevConsState *arg0, u8 arg1) {
    arg0->unk14 = arg1;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E41B8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E4228);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E42F8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E4428);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E45D0);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_002E46B8);

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
    u8 charAttribute; /* 0x16: passed with each printed character */
    u8 unk17; /* 0x17 */
    u8 pad18[4]; /* 0x18 */
    u8 *cells; /* 0x1C: two bytes per character cell */
} DevConsState;

extern void *memmove(void *dst, const void *src, u32 n);

extern void *memset(void *dst, s32 c, u32 n);

void func_0033CE08(DevConsState *arg0, s32 arg1, s32 arg2);

void func_00360E78(void *arg0, const char *arg1, void *arg2);

void sdfAdvanceConsoleRow(DevConsState *console) {
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

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033CE08);

void func_0033CF80(DevConsState *console, s32 character, s32 attribute) {
    func_0033CE08(console, character, attribute);
}

s32 func_0033CF98(DevConsState *console, const char *fmt, ...) {
    char text[0x200];
    __builtin_va_list args;
    char *cursor;
    s32 written = 0;
    s32 character;
    __builtin_stdarg_start(args, fmt);
    func_00360E78(text, fmt, args);
    character = text[0];
    if (character != 0) {
        do {
            written++;
            func_0033CE08(console, character, console->charAttribute);
            cursor = text + written;
            character = *cursor;
        } while (character != 0);
    }
    return written;
}

void func_0033D040(DevConsState *console, s16 column, s16 row) {
    console->cursorColumn = column;
    console->cursorRow = row;
}

void func_0033D050(DevConsState *console, u8 attribute) {
    console->charAttribute = attribute;
}

u8 func_0033D058(DevConsState *console) {
    return console->unk14;
}

void func_0033D060(DevConsState *console, u8 value) {
    console->unk14 = value;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D068);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D0D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D1A8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D2D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D480);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D568);

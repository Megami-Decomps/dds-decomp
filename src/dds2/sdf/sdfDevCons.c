#include "common.h"
#include "ee_mmi.h"

typedef struct DevConsState {
    u8 pad0[4]; /* 0x0 */
    struct DevConsState *unk4; /* 0x4 */
    u16 unk8; /* 0x8 */
    u16 unkA; /* 0xA */
    s16 columns; /* 0xC */
    s16 rows; /* 0xE */
    u16 cursorColumn; /* 0x10 */
    s16 cursorRow; /* 0x12 */
    u8 controlByte; /* 0x14 */
    u8 pad15; /* 0x15 */
    u8 textAttribute; /* 0x16: passed with each printed character */
    u8 unk17; /* 0x17 */
    u8 pad18[4]; /* 0x18 */
    u8 *cells; /* 0x1C: two bytes per character cell */
} DevConsState;

extern void *memmove(void *dst, const void *src, u32 n);

extern void *memset(void *dst, s32 c, u32 n);

void func_0033CE08(DevConsState *arg0, s32 arg1, s32 arg2);

void func_00360E78(void *arg0, const char *arg1, void *arg2);

/* Scroll the two-byte cell grid at its last row, or advance the cursor. */
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

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033CE08);

/* Forward a character and explicit attribute to the console renderer. */
void sdfDevConsWriteCharacter(DevConsState *console, s32 character, s32 attribute) {
    func_0033CE08(console, character, attribute);
}

/* Format text into a fixed-size scratch buffer, then emit each character. */
s32 sdfDevConsPrintf(DevConsState *console, const char *fmt, ...) {
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
            func_0033CE08(console, character, console->textAttribute);
            cursor = text + written;
            character = *cursor;
        } while (character != 0);
    }
    return written;
}

/* Set the cell coordinates of the next character. */
void sdfDevConsSetCursor(DevConsState *console, s16 column, s16 row) {
    console->cursorColumn = column;
    console->cursorRow = row;
}

/* Select the attribute passed to each subsequently printed character. */
void sdfDevConsSetTextAttribute(DevConsState *console, u8 attribute) {
    console->textAttribute = attribute;
}

/* Read or write the console control byte at offset 0x14. */
u8 sdfDevConsGetControlByte(DevConsState *console) {
    return console->controlByte;
}

void sdfDevConsSetControlByte(DevConsState *console, u8 value) {
    console->controlByte = value;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D068);

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

void func_0033D068(ConsBuf *arg0);
s32 sceDmaSync(void *ch, s32 mode, s32 timeout);
void sceDmaSendN(void *ch, void *addr, s32 size);
s32 sceGsSyncPath(s32 mode, s32 timeout);

/* Submit the console's pending packet to the DMA channel when it holds anything, then flip to the other buffer and restart the list there. */
void func_0033D0D8(ConsBuf *buf) {
    s32 bytes;
    u32 *channel;
    u32 flip;

    func_0033D068(buf);
    bytes = buf->unk8 - buf->unk4;
    if ((bytes >> 3) != 0) {
        channel = buf->unkC;
        sceDmaSync(channel, 0, 0);
        *channel |= 0x40;
        EE_SYNC();
        sceDmaSendN(channel, (void *)((u32)buf->unk4 & 0x0FFFFFFF), bytes >> 4);
        flip = buf->unk1C ^ 1;
        buf->unk1C = flip;
        buf->unk8 = (u8 *)buf->unk10[flip];
        buf->unk0 = buf->unk8 + 0x10;
        buf->unk4 = buf->unk8;
        sceGsSyncPath(0, 0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D1A8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D2D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D480);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D568);

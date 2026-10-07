#include "common.h"
#include "ee_mmi.h"

#define SDF_DEVCONS_CELL_BYTES 2
#define SDF_DEVCONS_TEXT_BUFFER_BYTES 0x200
#define SDF_DEVCONS_NONEMPTY_SHIFT 3
#define SDF_DEVCONS_GIF_TAG_BYTES 0x10
#define SDF_DMA_QWORD_SHIFT 4
#define SDF_DMA_CHCR_TTE 0x40
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

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

s32 func_00360E78(char *destination, const char *format, void *args);

/* Scroll only at the last row; otherwise advance it. Always reset the column. */
void sdfDevConsAdvanceRow(DevConsState *console) {
    if (console->cursorRow == console->rows - 1) {
        s32 rowBytes = console->columns * SDF_DEVCONS_CELL_BYTES;
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

/* Return the emitted character count. The formatter receives no buffer limit. */
s32 sdfDevConsPrintf(DevConsState *console, const char *format, ...) {
    char textBuffer[SDF_DEVCONS_TEXT_BUFFER_BYTES];
    __builtin_va_list arguments;
    char *textCursor;
    s32 characterCount = 0;
    s32 character;
    __builtin_stdarg_start(arguments, format);
    func_00360E78(textBuffer, format, arguments);
    character = textBuffer[0];
    if (character != 0) {
        do {
            characterCount++;
            func_0033CE08(console, character, console->textAttribute);
            textCursor = textBuffer + characterCount;
            character = *textCursor;
        } while (character != 0);
    }
    return characterCount;
}

/* Set the next cell coordinates without clamping them to the grid. */
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

/* Store the control byte unchanged; its individual bits are not decoded here. */
void sdfDevConsSetControlByte(DevConsState *console, u8 controlByte) {
    console->controlByte = controlByte;
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D068);

typedef struct ConsBuf {
    u8 *writeCursor; /* 0x0: next glyph-data write */
    u8 *packetStart; /* 0x4: beginning of the DMA range */
    u8 *currentTag; /* 0x8: reserved, not-yet-finalized GIF tag */
    void *dmaChannel; /* 0xC */
    s32 bufferAddresses[2]; /* 0x10: CPU-side addresses, kept as signed words */
    s32 bufferBytes; /* 0x18: byte capacity of each buffer */
    u16 activeBufferIndex; /* 0x1C */
    u16 rowStep; /* 0x1E: added when the renderer advances to the next row */
} ConsBuf;

void func_0033D068(ConsBuf *arg0);
s32 sceDmaSync(void *ch, s32 mode, s32 timeout);
void sceDmaSendN(void *ch, void *addr, s32 size);
s32 sceGsSyncPath(s32 mode, s32 timeout);

/* Finalize the current GIF tag and submit completed packets, then rotate buffers.
 * Keep the eight-byte pending-data test distinct from the quadword DMA count.
 */
void sdfDevConsKickPacketDma(ConsBuf *packetBuffers) {
    s32 packetBytes;
    u32 *channelRegisters;
    u32 nextBufferIndex;

    func_0033D068(packetBuffers);
    packetBytes = packetBuffers->currentTag - packetBuffers->packetStart;
    if ((packetBytes >> SDF_DEVCONS_NONEMPTY_SHIFT) != 0) {
        channelRegisters = packetBuffers->dmaChannel;
        sceDmaSync(channelRegisters, 0, 0);
        *channelRegisters |= SDF_DMA_CHCR_TTE; /* CHCR bit 6: tag-transfer enable */
        EE_SYNC();
        sceDmaSendN(channelRegisters, (void *)((u32)packetBuffers->packetStart & SDF_DMA_ADDRESS_MASK), packetBytes >> SDF_DMA_QWORD_SHIFT);
        nextBufferIndex = packetBuffers->activeBufferIndex ^ 1;
        packetBuffers->activeBufferIndex = nextBufferIndex;
        packetBuffers->currentTag = (u8 *)packetBuffers->bufferAddresses[nextBufferIndex];
        packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
        packetBuffers->packetStart = packetBuffers->currentTag;
        sceGsSyncPath(0, 0);
    }
}

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D1A8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D2D8);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D480);

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D568);

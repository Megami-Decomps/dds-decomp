#include "common.h"
#include "sdf.h"
#include "sdf_dev_cons.h"
#include "ee_mmi.h"

#define SDF_DEVCONS_CELL_BYTES 2
#define SDF_DEVCONS_TEXT_BUFFER_BYTES 0x200
#define SDF_DEVCONS_NONEMPTY_SHIFT 3
#define SDF_DEVCONS_GIF_TAG_BYTES 0x10
#define SDF_DMA_QWORD_SHIFT 4
#define SDF_DMA_CHCR_TTE 0x40
#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

void sdfDevConsStoreCharacter(DevConsState *arg0, s32 arg1, s32 arg2);
void sdfDevConsFinalizeGlyphTag(ConsBuf *arg0);
void sdfDevConsKickPacketDma(ConsBuf *packetBuffers);
void sdfDevConsAppendBackgroundSprite(DevConsState *arg0, ConsBuf *arg1);
void sdfDevConsDrawGlyphGrid(DevConsState *arg0, ConsBuf *arg1);
void sdfDevConsInitPacketBuffers(ConsBuf *arg0, void *arg1, s32 arg2);
s32 sdfVFormatString(char *destination, const char *format, void *args);
s32 sceDmaSync(void *ch, s32 mode, s32 timeout);
void sceDmaSendN(void *ch, void *addr, s32 size);
s32 sceGsSyncPath(s32 mode, s32 timeout);
void *sceDmaGetChan(s32 id);
struct SdfTex;
extern struct SdfTex *D_003BDA34;
extern DevConsState *D_003BD3C0;
extern u32 D_00398660[];
extern u32 D_003987E0[];
extern void *memmove(void *dst, const void *src, u32 n);
extern void *memset(void *dst, s32 c, u32 n);

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

/* Store one character/attribute cell, interpreting cursor controls and row wrap. */
void sdfDevConsStoreCharacter(DevConsState *console, s32 character, s32 attribute) {
    s32 count;
    s32 remainder;
    s32 tabWidth;
    s32 offset;

    switch (character) {
    case 9:
        offset = (console->cursorRow * console->columns + console->cursorColumn) * SDF_DEVCONS_CELL_BYTES;
        tabWidth = console->tabWidth;
        remainder = console->cursorColumn % tabWidth;
        count = remainder ? remainder : tabWidth;
        do {
            console->cells[offset] = 0;
            console->cells[offset + 1] = attribute;
            offset += SDF_DEVCONS_CELL_BYTES;
            console->cursorColumn = (u16)console->cursorColumn + 1;
            if (console->cursorColumn == console->columns) {
                sdfDevConsAdvanceRow(console);
                break;
            }
        } while (--count != 0);
        break;
    case 10:
        sdfDevConsAdvanceRow(console);
        return;
    case 13:
        console->cursorColumn = 0;
        break;
    default:
        if (character == 0x20) {
            character = 0;
        } else if ((u32)character - 0x20U >= 0x60U) {
            character = 0x7F;
        }
        offset = (console->cursorRow * console->columns + console->cursorColumn) * SDF_DEVCONS_CELL_BYTES;
        console->cells[offset] = character;
        console->cells[offset + 1] = attribute;
        console->cursorColumn = (u16)console->cursorColumn + 1;
        if (console->cursorColumn == console->columns) {
            sdfDevConsAdvanceRow(console);
        }
        break;
    }
}

/* Forward a character and explicit attribute to the console renderer. */
void sdfDevConsWriteCharacter(DevConsState *console, s32 character, s32 attribute) {
    sdfDevConsStoreCharacter(console, character, attribute);
}

/* Return the emitted character count. The formatter receives no buffer limit. */
s32 sdfDevConsPrintf(DevConsState *console, const char *format, ...) {
    char textBuffer[SDF_DEVCONS_TEXT_BUFFER_BYTES];
    __builtin_va_list arguments;
    char *textCursor;
    s32 characterCount = 0;
    s32 character;
    __builtin_stdarg_start(arguments, format);
    sdfVFormatString(textBuffer, format, arguments);
    character = textBuffer[0];
    if (character != 0) {
        do {
            characterCount++;
            sdfDevConsStoreCharacter(console, character, console->textAttribute);
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

/* Close the ordered register-list tag in the console's uncached DMA buffer.
 * Five eight-byte register values form one sprite; pad an odd value count. */
void sdfDevConsFinalizeGlyphTag(ConsBuf *buffers) {
    u8 *cursor = buffers->writeCursor;
    vu64 *tag = (vu64 *)buffers->currentTag;
    s32 registerCount = ((cursor - buffers->currentTag) >> 3) - 2;

    if (registerCount != 0) {
        if (registerCount & 1) {
            cursor += 8;
        }
        registerCount /= 5;
        buffers->currentTag = cursor;
        buffers->writeCursor = buffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
        tag[0] = (s64)registerCount | 0x5400000000008000ULL;
        tag[1] = 0x53531;
    }
}

/* Finalize the current GIF tag and submit completed packets, then rotate buffers.
 * Keep the eight-byte pending-data test distinct from the quadword DMA count.
 */
void sdfDevConsKickPacketDma(ConsBuf *packetBuffers) {
    s32 packetBytes;
    u32 *channelRegisters;
    u32 nextBufferIndex;

    sdfDevConsFinalizeGlyphTag(packetBuffers);
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

/* Append the console background sprite, then reserve the next glyph tag.
 * Packed coordinates wrap as words before their zero-extended packet writes. */
void sdfDevConsAppendBackgroundSprite(DevConsState *console, ConsBuf *packetBuffers) {
    vu64 *packet;
    u32 origin;
    u32 extent;

    sdfDevConsFinalizeGlyphTag(packetBuffers);
    if (packetBuffers->bufferBytes < packetBuffers->currentTag - packetBuffers->packetStart + 0x80) {
        sdfDevConsKickPacketDma(packetBuffers);
    }
    packet = (vu64 *)packetBuffers->currentTag;
    origin = (u16)(console->unk8 - 0x30) | ((u32)console->unkA << 16);
    extent = (u16)(console->columns * 3 * 0x40 + 0x70) | ((u32)(console->rows * packetBuffers->rowStep) << 16);
    if (console->controlByte & 4) {
        origin += 0xFFD00000;
        extent += 0x700000;
    } else {
        origin += 0xFFE80000;
        extent += 0x380000;
    }
    packet[0] = 0x5400000000008001ULL;
    packet[1] = 0x5510;
    packet[2] = 0x146;
    packet[3] = 0x30000000;
    packet[4] = origin;
    packet[5] = origin + extent;
    packet[6] = 0x156;
    packetBuffers->currentTag = (u8 *)packet + 0x40;
    packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
}

/* Emit five register values per visible cell, advancing packed coordinates.
 * The native do-loops have no zero-dimension guard. */
void sdfDevConsDrawGlyphGrid(DevConsState *console, ConsBuf *buffers) {
    u8 *cells;
    vu64 *writeCursor;
    u8 *packetStart;
    s32 row;
    s32 column;
    s32 y;
    s32 cellStep;
    u64 top;
    s32 character;
    s32 attribute;
    u64 glyph;
    u64 color;

    if ((u8)(console->controlByte & 1)) {
        return;
    }
    buffers->rowStep = (console->controlByte & 4) ? 0xC0 : 0x60;
    if (console->controlByte & 2) {
        sdfDevConsAppendBackgroundSprite(console, buffers);
    }
    cellStep = ((u32)buffers->rowStep << 16) | 0xC0;
    row = 0;
    writeCursor = (vu64 *)buffers->writeCursor;
    packetStart = buffers->packetStart;
    cells = console->cells;
    y = console->unkA;
    do {
        top = (u32)console->unk8 | ((u32)y << 16);
        column = 0;
        do {
            character = cells[0];
            attribute = cells[1];
            cells += 2;
            if (character >= 0x20) {
                if (buffers->bufferBytes < (u8 *)writeCursor - packetStart + 0x50) {
                    buffers->writeCursor = (u8 *)writeCursor;
                    sdfDevConsKickPacketDma(buffers);
                    writeCursor = (vu64 *)buffers->writeCursor;
                    packetStart = buffers->packetStart;
                }
                glyph = D_00398660[character - 0x20];
                color = D_003987E0[attribute];
                writeCursor[0] = color;
                writeCursor[1] = glyph;
                writeCursor[2] = top;
                writeCursor[3] = glyph + 0xC000C0;
                writeCursor[4] = top + cellStep;
                writeCursor += 5;
            }
            column++;
            top += 0xC0;
        } while (column != console->columns);
        row++;
        y += buffers->rowStep;
    } while (row != console->rows);
    buffers->writeCursor = (u8 *)writeCursor;
}

/* Initialize both uncached packet buffers and complete the GS setup packet.
 * Reserve the first glyph tag only after the setup values have been written. */
void sdfDevConsInitPacketBuffers(ConsBuf *packetBuffers, void *buffer, s32 bufferBytes) {
    u32 ringAddress;
    u8 *ring;
    vu64 *packet;
    u64 textureState;
    s32 halfBufferBytes = bufferBytes >> 1;

    ringAddress = ((u32)buffer & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    ring = (u8 *)ringAddress;
    packetBuffers->bufferBytes = halfBufferBytes;
    packetBuffers->bufferAddresses[0] = ring;
    packetBuffers->bufferAddresses[1] = ring + halfBufferBytes;
    packetBuffers->packetStart = ring;
    packetBuffers->activeBufferIndex = 0;
    textureState = sdfTexGetPrimaryTextureState(D_003BDA34);
    packet = (vu64 *)ring;
    packet[0] = 0x10AB400000008005ULL;
    packet[1] = 0xE;
    packet[2] = 0x44;
    packet[3] = 0x42;
    packet[4] = 0x3000D;
    packet[5] = 0x47;
    packet[6] = 0;
    packet[7] = 8;
    packet[8] = 1;
    packet[9] = 0x14;
    packet[10] = textureState;
    packet[11] = 6;
    packetBuffers->currentTag = ring + 0x60;
    packetBuffers->writeCursor = packetBuffers->currentTag + SDF_DEVCONS_GIF_TAG_BYTES;
    packetBuffers->dmaChannel = sceDmaGetChan(2);
}

typedef struct DevConsStackPackets {
    u8 header[0x30];
    u8 data[0x1000]; /* packet storage handed to the console buffer setup */
    u8 trailer[0x10];
} DevConsStackPackets;

/* Render every console in the global chain into a stack packet buffer and submit it. */
void sdfDevConsRenderAll(void) {
    DevConsStackPackets storage;
    ConsBuf packetBuffers;
    DevConsState *console;

    if (D_003BD3C0 != NULL) {
        sdfDevConsInitPacketBuffers(&packetBuffers, storage.data, 0x1000);
        for (console = D_003BD3C0; console != NULL; console = console->unk4) {
            sdfDevConsDrawGlyphGrid(console, &packetBuffers);
        }
        sdfDevConsKickPacketDma(&packetBuffers);
    }
}

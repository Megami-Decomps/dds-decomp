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

typedef struct ConsBuf {
    u8 *writeCursor; /* 0x0: next glyph-data write */
    u8 *packetStart; /* 0x4: beginning of the DMA range */
    u8 *currentTag; /* 0x8: reserved, not-yet-finalized GIF tag */
    void *dmaChannel; /* 0xC */
    u8 *bufferAddresses[2]; /* 0x10: start of each alternating packet buffer */
    s32 bufferBytes; /* 0x18: byte capacity of each buffer */
    u16 activeBufferIndex; /* 0x1C */
    u16 rowStep; /* 0x1E: added when the renderer advances to the next row */
} ConsBuf;

struct SdfTex;
u64 sdfTexGetPrimaryTextureState(struct SdfTex *texture);
extern u32 D_00439194;

/* Close the ordered register-list tag in the console's uncached DMA buffer.
 * Five eight-byte register values form one sprite; pad an odd value count. */
void func_0033D068(ConsBuf *buffers) {
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

/* Append the console background sprite, then reserve the next glyph tag.
 * Packed coordinates wrap as words before their zero-extended packet writes. */
void func_0033D1A8(DevConsState *console, ConsBuf *packetBuffers) {
    vu64 *packet;
    u32 origin;
    u32 extent;

    func_0033D068(packetBuffers);
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

INCLUDE_ASM(const s32, "sdf/sdfDevCons", func_0033D2D8);

/* Initialize both uncached packet buffers and complete the GS setup packet.
 * Reserve the first glyph tag only after the setup values have been written. */
void func_0033D480(ConsBuf *packetBuffers, void *buffer, s32 bufferBytes) {
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
    textureState = sdfTexGetPrimaryTextureState((struct SdfTex *)D_00439194);
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

extern DevConsState *D_00438AB0;
void func_0033D480(ConsBuf *arg0, void *arg1, s32 arg2);
void func_0033D2D8(DevConsState *arg0, ConsBuf *arg1);

/* Render every console in the global chain into a stack packet buffer and submit it. */
void func_0033D568(void) {
    DevConsStackPackets storage;
    ConsBuf packetBuffers;
    DevConsState *console;

    if (D_00438AB0 != NULL) {
        func_0033D480(&packetBuffers, storage.data, 0x1000);
        for (console = D_00438AB0; console != NULL; console = console->unk4) {
            func_0033D2D8(console, &packetBuffers);
        }
        sdfDevConsKickPacketDma(&packetBuffers);
    }
}

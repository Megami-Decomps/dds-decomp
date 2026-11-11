#include "common.h"
#include "libdma.h"
#include "eeregs.h"
#include "sdfDevCons.h"
#include "libsdf.h"

#include "ps2asm.h"
typedef struct sdfDevConsCB_tag
{
    struct sdfDevConsCB_tag *prev;
    struct sdfDevConsCB_tag *next;
    u16 x;
    u16 y;
    s16 w;
    s16 h;
    s16 locX;
    s16 locY;
    u8 mode;
    u8 pad;
    u8 attr;
    u8 tabwidth;
    sdfMemHandle_t *bufferHandle;
    u8 *bufferPtr;
} sdfDevConsCB_tag;

typedef struct sdfDevConsDrawCB_t
{
    u8 *cur;
    u8 *start;
    u8 *tag;
    u32 *dmaCh;
    u8 *buf[2];
    s32 buffSize;
    u16 dblBuffId;
    u16 stepY;
} sdfDevConsDrawCB_t;

/* SMT3 DrawAll and the DDS1 twin reserve this same 0x1040-byte stack
 * allocation: only the central 0x1000 bytes are passed as DMA storage. */
typedef struct
{
    u8 opaquePrefix[0x30];
    u8 data[0x1000];
    u8 opaqueSuffix[0x10];
} sdfDevConsStackPackets_t;

/* The four print-control words are signed XYZ and packed RGBA when drawing. */
typedef struct
{
    s32 x;
    s32 y;
    s32 z;
    u32 attr;
} sdfDevPrintCB_t;

typedef struct
{
    u16 quadwordCount;
    u8 reserved02;
    u8 tagKind;
    u32 nextAddress;
    u32 vifNop;
    u32 vifDirect;
    u64 setupTag;
    u64 setupRegisters;
    u64 textureClamp;
    u64 textureClampRegister;
    u64 textureFilter;
    u64 textureFilterRegister;
    u64 textureState;
    u64 textureStateRegister;
    u64 glyphTag;
    u64 glyphRegisters;
} sdfDevConsPacketHeader_t;

typedef struct
{
    u64 color;
    u64 firstUv;
    u64 firstPosition;
    u64 secondUv;
    u64 secondPosition;
} sdfDevConsGlyph_t;

extern void sceDmaSendN(void *channel, void *address, s32 quadwords);
extern s32 sceGsSyncPath(s32 mode, s32 timeout);
extern void *sceDmaGetChan(s32 channel);
extern u64 sdfTexGetTex0Reg(sdfTexHandle_t *aHandle);
extern void *sdfGrOpenPacketBuff(void);
extern void sdfGrClosePacketBuff(saddr_t aBuffPtr);
void sdfDevConsDrawBG(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aCB);

extern void sdfDevConsDrawInit(sdfDevConsDrawCB_t *aCB, void *aPacketBuffer, s32 aBufferSize);
extern void sdfDevConsDraw(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aBuf);
extern void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *aCB);

extern void *memset(void *a, s32 b, size_t c);
extern void *memmove(void *a, const void *b, size_t c);
extern s32 vsprintf(char *buf, const char *fmt, void *args);

extern void *sdfChipAllocDirect(s32 aSize);
extern void sdfChipFreeDirect(void *aPtr);
extern sdfMemHandle_t *sdfMemAllocDirect(s32 aSize);
extern void sdfMemFreeDirect(sdfMemHandle_t *aHandle);
extern void *sdfMemLock(sdfMemHandle_t *aHandle);
extern sdfTexHandle_t *sdfTexSet(void *aTex);

extern sdfTexHandle_t *sdfDebugFontHandle;
extern s32 alreadyInitDevCons;
extern u8 sdfDebugFontTex[];
extern u32 sdfDevConsFontUvTable[];

extern sdfDevConsCB_tag *devConsTop;
extern u32 sdfDevConsColorTable[];

/* DDS defines both tables in this unit. */
/* Packed console glyph UVs for characters 0x20 through 0x7F. */
u32 sdfDevConsFontUvTable[96] __attribute__((aligned(8))) = {
    0x00000000, 0x000000C0, 0x00000180, 0x00000240, 0x00000300, 0x000003C0, 0x00000480, 0x00000540,
    0x00000600, 0x000006C0, 0x00C00000, 0x00C000C0, 0x00C00180, 0x00C00240, 0x00C00300, 0x00C003C0,
    0x00C00480, 0x00C00540, 0x00C00600, 0x00C006C0, 0x01800000, 0x018000C0, 0x01800180, 0x01800240,
    0x01800300, 0x018003C0, 0x01800480, 0x01800540, 0x01800600, 0x018006C0, 0x02400000, 0x024000C0,
    0x02400180, 0x02400240, 0x02400300, 0x024003C0, 0x02400480, 0x02400540, 0x02400600, 0x024006C0,
    0x03000000, 0x030000C0, 0x03000180, 0x03000240, 0x03000300, 0x030003C0, 0x03000480, 0x03000540,
    0x03000600, 0x030006C0, 0x03C00000, 0x03C000C0, 0x03C00180, 0x03C00240, 0x03C00300, 0x03C003C0,
    0x03C00480, 0x03C00540, 0x03C00600, 0x03C006C0, 0x04800000, 0x048000C0, 0x04800180, 0x04800240,
    0x04800300, 0x048003C0, 0x04800480, 0x04800540, 0x04800600, 0x048006C0, 0x05400000, 0x054000C0,
    0x05400180, 0x05400240, 0x05400300, 0x054003C0, 0x05400480, 0x05400540, 0x05400600, 0x054006C0,
    0x06000000, 0x060000C0, 0x06000180, 0x06000240, 0x06000300, 0x060003C0, 0x06000480, 0x06000540,
    0x06000600, 0x060006C0, 0x06C00000, 0x06C000C0, 0x06C00180, 0x06C00240, 0x06C00300, 0x06C003C0,
};

/* Mutable colors used by the device console and packet command setter. */
u32 sdfDevConsColorTable[16] = {
    0x80808080, 0x80800000, 0x80000080, 0x80800080,
    0x80008000, 0x80808000, 0x80008080, 0x80606060,
    0x80202020, 0x80400000, 0x80000040, 0x80400040,
    0x80004000, 0x80404000, 0x80004040, 0x00000000,
};

extern void *sdfDevVprintf(u32 *aCB, const char *aStr, void *LegacyParamArray);
void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr);

void sdfDevConsInit(void)
{
    if (!alreadyInitDevCons) {
        alreadyInitDevCons = 1;
        sdfDebugFontHandle = sdfTexSet(sdfDebugFontTex);
    }
}

sdfTexHandle_t *sdfDevConsGetFontHandle(void)
{
    sdfDevConsInit();
    return sdfDebugFontHandle;
}

s16 *sdfDevConsGetFontUvTable(void)
{
    return (s16 *)sdfDevConsFontUvTable;
}

void sdfDevConsLinkToTop(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsCB_tag *top;

    top = devConsTop;
    aHndl->prev = NULL;
    aHndl->next = top;
    if (top) {
        top->prev = aHndl;
    }
    devConsTop = aHndl;
}

void sdfDevConsUnlink(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsCB_tag *prev;
    sdfDevConsCB_tag *next;

    prev = aHndl->prev;
    next = aHndl->next;
    if (prev) {
        prev->next = next;
    } else {
        devConsTop = next;
    }
    if (next) {
        next->prev = prev;
    }
}

void sdfDevConsClose(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsUnlink(aHndl);
    sdfMemFreeDirect(aHndl->bufferHandle);
    sdfChipFreeDirect(aHndl);
}

void sdfDevConsClearInside(sdfDevConsCB_tag *aHndl)
{
    aHndl->locX = 0;
    aHndl->locY = 0;
    memset(aHndl->bufferPtr, 0, aHndl->w * aHndl->h * 2);
}

void sdfDevConsClear(sdfDevConsCB_tag *aHndl)
{
    sdfDevConsClearInside(aHndl);
}

sdfDevConsCB_tag *sdfDevConsOpen(s32 aX, s32 aY, s32 aW, s32 aH)
{
    sdfDevConsCB_tag *hndl;

    sdfDevConsInit();
    hndl = sdfChipAllocDirect(sizeof(sdfDevConsCB_tag));
    hndl->x = aX;
    hndl->y = aY;
    hndl->w = aW;
    hndl->h = aH;
    hndl->tabwidth = 8;
    hndl->mode = 0;
    hndl->attr = 0;
    hndl->bufferHandle = sdfMemAllocDirect(aW * aH * 2);
    hndl->bufferPtr = sdfMemLock(hndl->bufferHandle);
    sdfDevConsClearInside(hndl);
    sdfDevConsLinkToTop(hndl);
    return hndl;
}

void sdfDevConsNewLine(sdfDevConsCB_tag *aHndl)
{
    s32 lineSize;
    s32 size;

    if (aHndl->locY == aHndl->h - 1) {
        lineSize = aHndl->w * 2;
        size = lineSize * aHndl->locY;
        if (aHndl->locY > 0) {
            memmove(aHndl->bufferPtr, aHndl->bufferPtr + lineSize, size);
        }
        memset(aHndl->bufferPtr + size, 0, lineSize);
    } else {
        aHndl->locY++;
    }
    aHndl->locX = 0;
}

void sdfDevConsPutInside(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr)
{
    s32 cnt;
    s32 off;

    switch (aChar) {
    case '\t':
        off = (aHndl->locY * aHndl->w + aHndl->locX) * 2;
        cnt = aHndl->locX % aHndl->tabwidth;
        if (cnt == 0) {
            cnt = aHndl->tabwidth;
        }
        do {
            aHndl->bufferPtr[off] = 0;
            aHndl->bufferPtr[off + 1] = aAttr;
            off += 2;
            aHndl->locX++;
            if (aHndl->locX == aHndl->w) {
                sdfDevConsNewLine(aHndl);
                return;
            }
        } while (--cnt);
        return;
    case '\n':
        sdfDevConsNewLine(aHndl);
        return;
    case '\r':
        aHndl->locX = 0;
        return;
    }

    if (aChar == ' ') {
        aChar = 0;
    } else if (aChar < ' ' || aChar > 0x7F) {
        aChar = 0x7F;
    }
    off = (aHndl->locY * aHndl->w + aHndl->locX) * 2;
    aHndl->bufferPtr[off] = aChar;
    aHndl->bufferPtr[off + 1] = aAttr;
    aHndl->locX++;
    if (aHndl->locX == aHndl->w) {
        sdfDevConsNewLine(aHndl);
    }
}

void sdfDevConsPut(sdfDevConsCB_tag *aHndl, s32 aChar, s32 aAttr)
{
    sdfDevConsPutInside(aHndl, aChar, aAttr);
}

s32 sdfDevConsPrintf(sdfDevConsCB_tag *aHndl, const char *aFmt, ...)
{
    char buf[0x200];
    __builtin_va_list args;
    s32 count = 0;
    s32 ch;

    __builtin_stdarg_start(args, aFmt);
    vsprintf(buf, aFmt, args);
    ch = buf[0];
    if (ch != 0) {
        do {
            count++;
            sdfDevConsPutInside(aHndl, ch, aHndl->attr);
            ch = buf[count];
        } while (ch != 0);
    }
    return count;
}

void sdfDevConsLocate(sdfDevConsCB_tag *aHndl, s32 aX, s32 aY)
{
    aHndl->locX = aX;
    aHndl->locY = aY;
}

void sdfDevConsAttribute(sdfDevConsCB_tag *aHndl, s32 aAttr)
{
    aHndl->attr = aAttr;
}

s32 sdfDevConsGetMode(sdfDevConsCB_tag *aHndl)
{
    return aHndl->mode;
}

void sdfDevConsSetMode(sdfDevConsCB_tag *aHndl, s32 aMode)
{
    aHndl->mode = aMode;
}

void sdfDevConsTagClose(sdfDevConsDrawCB_t *aCB)
{
    u8 *cur = aCB->cur;
    volatile u64 *tag = (volatile u64 *)aCB->tag;
    s32 regCount = ((cur - aCB->tag) >> 3) - 2;

    if (regCount != 0) {
        if (regCount & 1) {
            cur += 8;
        }
        regCount /= 5;
        aCB->tag = cur;
        aCB->cur = cur + 0x10;
        tag[0] = (s64)regCount | 0x5400000000008000ULL;
        tag[1] = 0x53531;
    }
}

void sdfDevConsDrawFlush(sdfDevConsDrawCB_t *aCB) {
    s32 packetBytes;
    u32 *channelRegisters;
    u32 nextBufferIndex;

    sdfDevConsTagClose(aCB);
    packetBytes = aCB->tag - aCB->start;
    if ((packetBytes >> 3) != 0) {
        channelRegisters = aCB->dmaCh;
        sceDmaSync((sceDmaChan *)channelRegisters, 0, 0);
        *(volatile u32 *)channelRegisters = *(volatile u32 *)channelRegisters | 0x40; /* CHCR bit 6: tag-transfer enable */
        EE_SYNC_BARRIER();
        sceDmaSendN(channelRegisters, (void *)EE_PHYSICAL(EE_ADDR(aCB->start)), packetBytes >> 4);
        nextBufferIndex = aCB->dblBuffId ^ 1;
        aCB->dblBuffId = nextBufferIndex;
        aCB->tag = (u8 *)aCB->buf[nextBufferIndex];
        aCB->cur = aCB->tag + 0x10;
        aCB->start = aCB->tag;
        sceGsSyncPath(0, 0);
    }
}

void sdfDevConsDrawBG(sdfDevConsCB_tag *aHndl, sdfDevConsDrawCB_t *aCB) {
    vu64 *packet;
    u32 origin;
    u32 extent;

    sdfDevConsTagClose(aCB);
    if (aCB->buffSize < aCB->tag - aCB->start + 0x80) {
        sdfDevConsDrawFlush(aCB);
    }
    packet = (vu64 *)aCB->tag;
    origin = (u16)(aHndl->x - 0x30) | ((u32)aHndl->y << 16);
    extent = (u16)(aHndl->w * 3 * 0x40 + 0x70) | ((u32)(aHndl->h * aCB->stepY) << 16);
    if (aHndl->mode & 4) {
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
    aCB->tag = (u8 *)packet + 0x40;
    aCB->cur = aCB->tag + 0x10;
}

void sdfDevConsDraw(sdfDevConsCB_tag *console, sdfDevConsDrawCB_t *buffers) {
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

    if ((u8)(console->mode & 1)) {
        return;
    }
    buffers->stepY = (console->mode & 4) ? 0xC0 : 0x60;
    if (console->mode & 2) {
        sdfDevConsDrawBG(console, buffers);
    }
    cellStep = ((u32)buffers->stepY << 16) | 0xC0;
    row = 0;
    writeCursor = (vu64 *)buffers->cur;
    packetStart = buffers->start;
    cells = console->bufferPtr;
    y = console->y;
    do {
        top = (u32)console->x | ((u32)y << 16);
        column = 0;
        do {
            character = cells[0];
            attribute = cells[1];
            cells += 2;
            if (character >= 0x20) {
                if (buffers->buffSize < (u8 *)writeCursor - packetStart + 0x50) {
                    buffers->cur = (u8 *)writeCursor;
                    sdfDevConsDrawFlush(buffers);
                    writeCursor = (vu64 *)buffers->cur;
                    packetStart = buffers->start;
                }
                glyph = sdfDevConsFontUvTable[character - 0x20];
                color = sdfDevConsColorTable[attribute];
                writeCursor[0] = color;
                writeCursor[1] = glyph;
                writeCursor[2] = top;
                writeCursor[3] = glyph + 0xC000C0;
                writeCursor[4] = top + cellStep;
                writeCursor += 5;
            }
            column++;
            top += 0xC0;
        } while (column != console->w);
        row++;
        y += buffers->stepY;
    } while (row != console->h);
    buffers->cur = (u8 *)writeCursor;
}

void sdfDevConsDrawInit(sdfDevConsDrawCB_t *aCB, void *aPacketBuffer, s32 aBufferSize) {
    addr_t ringAddress;
    u8 *ring;
    vu64 *packet;
    u64 textureState;
    s32 halfBufferBytes = aBufferSize >> 1;

    ringAddress = EE_UNCACHED_RING(EE_PHYSICAL(EE_ADDR(aPacketBuffer)));
    ring = EE_ADDR_AS(u8 *, ringAddress);
    aCB->buffSize = halfBufferBytes;
    aCB->buf[0] = ring;
    aCB->buf[1] = ring + halfBufferBytes;
    aCB->start = ring;
    aCB->dblBuffId = 0;
    textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
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
    aCB->tag = ring + 0x60;
    aCB->cur = aCB->tag + 0x10;
    aCB->dmaCh = sceDmaGetChan(2);
}

void sdfDevConsDrawAll(void) {
    sdfDevConsStackPackets_t storage;
    sdfDevConsDrawCB_t packetBuffers;
    sdfDevConsCB_tag *console;

    if (devConsTop != NULL) {
        sdfDevConsDrawInit(&packetBuffers, storage.data, 0x1000);
        for (console = devConsTop; console != NULL; console = console->next) {
            sdfDevConsDraw(console, &packetBuffers);
        }
        sdfDevConsDrawFlush(&packetBuffers);
    }
}

void *sdfDevVprintf(u32 *aCB, const char *aStr, void *LegacyParamArray) {
    sdfDevPrintCB_t *input = (sdfDevPrintCB_t *)aCB;
    char text[0x200];
    s32 formattedCount;
    u8 *start;
    u8 *cursor;
    sdfDevConsPacketHeader_t *header;
    u8 *textCursor;
    s32 character;
    s32 glyphCount;
    u32 color;
    u64 position;
    s32 quadwordCount;

    sdfDevConsInit();
    formattedCount = vsprintf(text, aStr, LegacyParamArray);
    start = (u8 *)sdfGrOpenPacketBuff();
    if (formattedCount == 0) {
        QUAD_ZERO(*(u128 *)start);
        cursor = start + 0x10;
    } else {
        header = (sdfDevConsPacketHeader_t *)start;
        cursor = start + sizeof(*header);
        glyphCount = 0;
        /* This dual-use command carries x, y, depth and RGBA in its four words. */
        position = (u32)((u16)input->x | ((u32)(u16)input->y << 16)) |
                   ((u64)(u32)input->z << 32);
        color = input->attr;
        textCursor = (u8 *)text;
        while ((character = *textCursor++) != 0) {
            if (character >= 0x20) {
                sdfDevConsGlyph_t *glyph = (sdfDevConsGlyph_t *)cursor;
                u32 uv = sdfDevConsFontUvTable[character - 0x20];

                glyph->color = color;
                glyph->firstUv = uv;
                glyph->firstPosition = position;
                glyph->secondUv = (u32)(uv + 0x00C000C0);
                glyph->secondPosition = position + 0x006000C0;
                cursor += sizeof(*glyph);
                glyphCount++;
            }
            /* Control bytes consume the same horizontal advance as glyphs. */
            position += 0xC0;
        }
        if (glyphCount & 1) {
            *(u64 *)cursor = 0;
            cursor += sizeof(u64);
        }
        quadwordCount = ((s32)(cursor - start) >> 4) - 1;
        input->x = (u16)position;
        header->quadwordCount = quadwordCount;
        header->vifNop = 0;
        header->vifDirect = 0x50000000 | quadwordCount;
        header->setupTag = 0x10AB400000000003ULL;
        header->setupRegisters = 0xEEE;
        header->textureClamp = 0;
        header->textureClampRegister = 8;
        header->textureFilter = 1;
        header->textureFilterRegister = 0x14;
        header->textureState = sdfTexGetTex0Reg(sdfDebugFontHandle);
        header->textureStateRegister = 6;
        header->glyphTag = 0x5400000000008000ULL | glyphCount;
        header->glyphRegisters = 0x53531;
    }
    sdfGrClosePacketBuff(EE_ADDR_S(cursor));
    return start;
}

void *sdfDevPrintfEx(u32 *aCB, const char *aStr, ...)
{
    __builtin_va_list args;

    __builtin_stdarg_start(args, aStr);
    return sdfDevVprintf(aCB, aStr, args);
}

void *sdfDevPrintf(u32 aX, u32 aY, u32 aZ, s32 aAttr, const char *aStr, ...)
{
    u32 cb[4];
    __builtin_va_list args;

    sdfDevSetPrintCB(cb, aX, aY, aZ, aAttr);
    __builtin_stdarg_start(args, aStr);
    return sdfDevVprintf(cb, aStr, args);
}


void sdfPktSetCmd(u32 *aCB, s32 aMode)
{
    aCB[3] = sdfDevConsColorTable[aMode];
}

void sdfDevSetPrintCB(u32 *aCB, u32 aX, u32 aY, u32 aZ, s32 aAttr)
{
    aCB[0] = aX;
    aCB[1] = aY;
    aCB[2] = aZ;
    sdfPktSetCmd(aCB, aAttr);
}

INCLUDE_SDATA(const s32, "sdf/sdfDevCons", devConsTop);

INCLUDE_SDATA(const s32, "sdf/sdfDevCons", alreadyInitDevCons);


#include "common.h"

extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfAllocPacketAligned(s32);
extern void *func_0033A2D8(void *, s32, s32, s64, s32);
extern u64 *func_0033A2D0(void *);
extern void sdfAppendPacket();

extern s32 D_00435E6C;
extern s32 D_00435E70;
extern void func_0019B8B0(s32);
extern s32 func_0019FA08(s32, s32, s32, u32, s32, s32);
extern s32 func_0019E5D8();
extern void kwlnTaskCreate(const char *, s32, s32, s32, u32 (*)(void), void (*)(void), void *);
extern s32 scrCreateProcessTaskFromResource(s32, const char *, s32);
extern u32 itfDrawBackgroundAndGetTaskReadyMask(void);
extern void func_001A0438(void);

extern u32 D_0043658C;

extern u32 D_00438F24;

extern u64 func_0019CE78(u64, u64, u64, u64, u64);

extern u64 func_0019CE10(u64, u64, u64, u64, u64);

extern u32 D_0043654C;

extern u32 D_0043655C;

extern u32 D_00436560;

extern u32 D_00436590;

extern s64 kwlnTaskGetRegisteredState(u32);

/* Byte stream read by func_00196478/func_001964A0: base at +0x10, position at +0x18. */
typedef struct TextSub {
    u8 pad00[0x30];
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
} TextSub;

typedef struct TextStream {
    s32 x;           /* 0x0 */
    s32 y;           /* 0x4 */
    s32 z;           /* 0x8 */
    s8 unkC;         /* 0xC: set by opcode 0xF206 */
    s8 unkD;         /* 0xD: set by opcode 0xF202 */
    s8 unkE;         /* 0xE: set by opcode 0xF209 */
    s8 unkF;         /* 0xF: set by opcode 0xF207 */
    u8 *bytes;       /* 0x10: encoded input base */
    TextSub *sub;    /* 0x14 */
    s32 offset;      /* 0x18: current byte position */
    s8 unk1C;        /* 0x1C: set once an opcode has run */
    s8 unk1D;        /* 0x1D */
} TextStream;

s32 func_0019E848(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

extern u32 D_004528C0[];

/* Field block split by func_00198038. */
typedef struct MemBlock {
    s32 firstOffset; /* 0x0 */
    s32 secondDelta; /* 0x4 */
    u8 pad08[0x10]; /* 0x8 */
    s32 thirdDelta; /* 0x18 */
} MemBlock;

typedef struct MemOut {
    void *first; /* 0x0 */
    void *second; /* 0x4 */
    void *third; /* 0x8 */
} MemOut;

/* 8-byte node header; payload follows (func_00198248/itfEnqueueMemNode). */
typedef struct MemNode {
    u32 index;             /* 0x0 */
    struct MemNode *next;  /* 0x4 */
} MemNode;

void func_0019BE20(s32 id, const char *path);

extern u32 strlen(const char *str);

extern s32 func_0032C138(u32);

extern u64 func_00343ED0(const char *, u32 *, u64);

typedef struct TextPoolNode {
    struct TextPoolNode *previous;
    struct TextPoolNode *next;
    s32 index;
} TextPoolNode;

typedef struct TextPool {
    TextPoolNode *activeHead;
    TextPoolNode *activeTail;
    TextPoolNode *firstFree;
    TextPoolNode *lastFree;
} TextPool;

typedef struct TextVector {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} TextVector;

typedef struct TextStyleNode {
    u8 pad00[4];
    u32 x;
    u32 y;
    u8 pad0C[4];
    u32 color;
    u8 pad14[8];
    struct TextStyleNode *firstChild;
    u8 pad20[4];
    struct TextStyleNode *next;
    struct TextStyleNode *nextChild;
} TextStyleNode;

extern s32 func_00102930(void);

extern void func_00102908(void);

extern void func_0035B6E0(const char *);

extern char D_00414C50[]; /* "Camp process halted.\n", followed by padding no C emits */

extern u16 D_003B2F58[];

u32 itfReadEncodedTextLead(TextStream *stream) {
    s32 *position = &stream->offset;
    u8 *byte = stream->bytes + *position;
    u32 value = *byte;

    *position += 2;
    return (value + 0xFF) & 0xFF;
}

u32 itfReadEncodedCode(TextStream *stream) {
    u32 first;
    u32 second;

    first = (stream->bytes[stream->offset++] + 0xff) & 0xff;
    second = stream->bytes[stream->offset++];
    if (second == 0xff) {
        second = 0;
    } else {
        second = (second + 0xff) & 0xff;
    }
    return (second << 8) | first;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E1B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E5D8);

void func_0019E7C8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_0019E848(arg0, arg1, 0, 0, 0, 0, 0x80, arg2, arg3);
}

void func_0019E800(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    func_0019E848(arg0, arg1, 0, arg2 & 0xFF, arg3 & 0xFF, arg4 & 0xFF, arg5 & 0xFF, arg6, arg7);
}

s32 func_0019E848(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    TextStream args;
    args.x = arg0;
    args.y = arg1;
    args.z = arg2 << 4;
    args.unkC = arg3;
    args.unkD = arg4;
    args.unkE = arg5;
    args.unkF = arg6;
    args.bytes = (u8 *)arg7;
    args.sub = (TextSub *)arg8;
    args.offset = 0;
    args.unk1C = 1;
    args.unk1D = 1;
    return func_0019E5D8(&args);
}

extern s8 D_00436550;

s32 func_0019E8A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s8 saved = D_00436550;
    s32 result;
    D_00436550 = 0;
    result = func_0019E848(arg0, arg1, arg2, arg3 & 0xFF, arg4 & 0xFF, arg5 & 0xFF, arg6 & 0xFF, arg7, 0);
    D_00436550 = saved;
    return result;
}

u32 func_0019E8E0(u32 arg0) {
    return D_0043654C & arg0;
}

void func_0019E8F0(s32 arg0, s32 arg1) {
    D_004528C0[arg0] = arg1;
}

u32 func_0019E908(void) {
    return D_0043655C;
}

u32 func_0019E910(void) {
    return D_00436560;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E918);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EC00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EDC0);

void itfInitTextDrawArgs(u8 *arg0, TextSub *arg1) {
    TextStream args;
    args.x = 0;
    args.y = 0;
    args.z = 0;
    args.unkC = 0;
    args.unkD = 0;
    args.unkE = 0;
    args.unkF = 0;
    args.bytes = arg0;
    args.sub = arg1;
    args.offset = 0;
    args.unk1C = 1;
    args.unk1D = 1;
    func_0019EDC0(&args);
}

u64 func_0019EF38(u64 arg0, u64 arg1, s32 arg2, s32 alt, u64 measureFlag, u64 entryFlag, u64 colors, u64 source) {
    u64 glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frFontSetEntryFlag(kind, entryFlag);
    func_0019D1D0(1);
    func_0019D1E0(2);
    func_0019D1E0(0x10);
    glyph = func_0019CE10(source, kind, 0, 0, 0);
    func_0019D1D0(0x10);
    func_0019D1D0(2);
    func_0019D1E0(1);
    frFontSetFlagAndMeasureGlyphs(glyph, measureFlag);
    func_0019D100(glyph, arg0, arg1);
    func_0019D110(glyph, arg2 << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetChainFlag(glyph, 5);
    return glyph;
}

void mnuLoadStaffFonts(void) {
    func_0019BE20(4, "/font/staff1.fnt");
    func_0019BE20(5, "/font/staff2.fnt");
}

void mnuUnloadStaffFonts(void) {
    frFontFreeEntry(4);
    frFontFreeEntry(5);
}

/* DDS2 uses the larger 0xA10-entry glyph table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 value) {
    s32 adjusted = (value & 0xffff) + 0xffff7f80;
    s32 index = ((adjusted & 0xff00) >> 1) + (adjusted & 0x7f);

    if ((u32)index < 0xA10) {
        return D_003B2F58[index];
    }
    return 0xffff;
}

/* Preserve single-byte text and substitute 0x8080 for unmapped glyph pairs. */
void itfConvertText(u8 *output, const char *input) {
    s32 i;
    s32 length = strlen(input);

    for (i = 0; i < length; i++, output++) {
        if (input[i] >= 0) {
            output[0] = input[i];
        } else {
            u32 value = itfDecodeGlyph((u8)input[i + 1] | ((u8)input[i] << 8));
            if (value != 0xffff) {
                output[0] = value >> 8;
                output[1] = value;
            } else {
                output[0] = 0x80;
                output[1] = 0x80;
            }
            output++;
            i++;
        }
    }
}

/* Build a glyph with a fixed 16x18 cell, then attach it to its parent. */
void itfAttachGlyph16x18(u64 arg0, u64 arg1, s32 arg2, u64 colors,
                                    u64 glyphSource, u64 parent) {
    u64 glyph;

    glyph = func_0019CE10(glyphSource, 0, 0, 0, 0);
    func_0019D088(glyph, 0x10, 0x12);
    func_0019D100(glyph, arg0, arg1);
    func_0019D110(glyph, arg2 << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetFlagAndMeasureGlyphs(glyph, 0xfffffffffffffffc);
    frFontLinkGlyph(parent, glyph, 0);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F280);

extern s32 func_0019F280();

s32 func_0019F408(arg0, arg1, arg2, arg3, arg4, arg5)
    s32 arg0;
    s32 arg1;
    s32 arg2;
    s32 arg3;
    s32 arg4;
    s32 arg5;
{
    s32 handle = func_0019F280(arg0, arg1, arg2, arg3, arg4, 1, 0, arg5);
    frFontSetFlagAndMeasureGlyphs(handle, 3);
    return handle;
}

void func_0019F448(void) {
    func_0019F408();
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F460);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F5E8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F6C8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F798);

/* The sequel uses the negative flag variant on its alternate 12x16 glyph. */
void itfAttachGlyph12x16(u64 arg0, u64 arg1, s32 arg2, u64 colors,
                                    u64 glyphSource, u64 parent) {
    u64 glyph;

    glyph = func_0019CE78(glyphSource, 0, 0, 0, 0);
    func_0019D088(glyph, 0xc, 0x10);
    frFontSetFlagAndMeasureGlyphs(glyph, 0xfffffffffffffffd);
    func_0019D100(glyph, arg0, arg1);
    func_0019D110(glyph, arg2 << 4);
    frFontSetChildColors(glyph, colors);
    frFontLinkGlyph(parent, glyph, 0);
}

/* Count 0xFx0F separators in the packed text stream. */
s32 itfCountTextSeparators(char *text) {
    s32 separatorCount = 0;

    while (*text != 0) {
        if ((*(u8 *)text & 0xF0) == 0xF0) {
            if (text[1] == 0xF) {
                separatorCount++;
            }
            text++;
        }
        text++;
    }
    return separatorCount;
}

/* Copy one plain-text segment; other two-byte control codes pass through. */
void itfCopyTextSegment(char *src, char *dst, s32 segmentIndex) {
    s32 currentSegment = 0;

    while (*src != 0) {
        if ((*(u8 *)src & 0xF0) == 0xF0) {
            if (src[1] == 0xF) {
                currentSegment++;
            } else {
                dst[0] = *(u8 *)src;
                dst[1] = src[1];
                dst += 2;
            }
            src++;
        } else if (currentSegment == segmentIndex) {
            *dst = *(u8 *)src;
            dst++;
        }
        src++;
    }
    *dst = 0;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FA08);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FC38);

s32 func_0019FE00(s32 arg0, s32 arg1, s32 arg2, s8 arg3, u16 arg4, s32 arg5) {
    s32 result = 0;

    func_0019B8B0(0x13);
    switch (arg3) {
    case 0:
        result = func_0019FA08(arg0, arg1, arg2, arg4, D_00435E6C, arg5);
        break;
    case 1:
        result = func_0019FA08(arg0, arg1, arg2, arg4, D_00435E70, arg5);
        break;
    case 2:
        result = func_0019FA08(arg0, arg1, arg2, arg4, D_00435E6C, arg5);
        break;
    }
    func_0019B8B0(-1);
    return result;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FEF8);

u32 func_001A0038(void) {
    return 0;
}

void func_001A0040(void) {
}

void func_001A0048(void) {
}

s32 func_001A0050(s32 arg0, s32 arg1, s32 arg2) {
    return arg2;
}

extern s32 D_00438F20;
s32 func_001A0058(void) {
    return D_00438F20;
}

u32 func_001A0060(u32 arg0) {
    return arg0;
}

void itfSplitRelativeSegments(MemBlock *block, MemOut *segments) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    segments->first = (u8 *)block + firstOffset;
    segments->second = (u8 *)block + secondOffset;
    segments->third = (u8 *)block + thirdOffset;
}

u32 func_001A0098(u32 arg0) {
    return *(u32 *)(func_001A0060(arg0) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A00B8);

extern s32 func_003292A8(s32);
extern u8 *sdfResourceRetainAddress(s32);
extern void *memcpy(void *, const void *, u32);

/* The header before each payload forms a circular free-node list. */
u8 *itfCreateMemNodeRing(s32 payload, s32 count) {
    s32 handle = func_003292A8((payload + 8) * (count + 1) + 4);
    u8 *list = sdfResourceRetainAddress(handle);
    MemNode *node;
    MemNode *next;
    s32 i = 0;
    memcpy(list, &handle, 4);
    list += 4;
    node = (MemNode *)list;
    if (count > 0) {
        do {
            node->index = i;
            i++;
            next = (MemNode *)((u8 *)node + payload + 8);
            node->next = next;
            node = next;
        } while (i < count);
    }
    node->index = count;
    node->next = (MemNode *)list;
    return list;
}

void *itfDequeueMemNode(MemNode *queue) {
    MemNode *head = queue->next;

    if (head->index == 0) {
        return NULL;
    }
    queue->next = head->next;
    head->next = NULL;
    return head + 1;
}

s32 itfEnqueueMemNode(void *payload, MemNode *queue) {
    MemNode *node = (MemNode *)payload - 1;
    if (payload == NULL) {
        return 0;
    }
    if (node->next != NULL) {
        return 0;
    }
    node->next = queue->next;
    queue->next = node;
    return 1;
}

u32 itfReleasePayloadAllocation(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 - 4));
    return 1;
}

void itfLoadBackgroundSprite(void) {
    u32 resource;
    u64 buffer = func_00343ED0("/sprite/bg00.tmx", &resource, 0);

    D_00438F24 = func_0032C138(resource);
    func_003297C8(buffer);
}

void func_001A0338(void) {
    sdfTexReleaseReference(D_00438F24);
}

extern s32 func_00305C40();

typedef struct TextBackgroundSprite {
    u8 pad00[0xC];
    s16 width;
    s16 height;
} TextBackgroundSprite;

void itfDrawBackgroundSprite(void) {
    s32 origin[4];
    s32 color[4];
    s16 width;
    s16 height;
    TextBackgroundSprite *panel = (TextBackgroundSprite *)D_00438F24;
    if (panel != NULL) {
        width = panel->width;
        height = panel->height;
        origin[0] = 0;
        origin[1] = 0;
        origin[2] = width;
        origin[3] = height;
        color[0] = 0x80808080;
        color[1] = 0x80808080;
        color[2] = 0x80808080;
        color[3] = 0x80808080;
        func_00305C40(0, 0, 0, width * 0x10, height * 8, origin, color, 0, 0, 1, D_00438F24, 0x52);
    }
}

void func_001A03D8(void) {
    itfLoadBackgroundSprite();
    kwlnTaskCreate("test_font", 0x2B06, 0, 0, itfDrawBackgroundAndGetTaskReadyMask, func_001A0438, 0);
    D_00436590 = scrCreateProcessTaskFromResource(0x258, "host0:../../../dds3data/font/test.bf", 0);
}

void func_001A0438(void) {
    func_0019C490(D_0043658C);
    func_001A0338();
}

/* Return an all-bits-set ready mask only while the registered task is in state 3. */
u32 itfDrawBackgroundAndGetTaskReadyMask(void) {
    s64 taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = kwlnTaskGetRegisteredState(D_00436590);
    readyMask = 0xffffffff;
    if (taskState != 3) {
        readyMask = 0;
    }
    return readyMask;
}

void itfInitPool(TextPool *pool, TextPoolNode *nodes, s32 count, s32 stride) {
    s8 index = 0;
    TextPoolNode *previous = NULL;
    TextPoolNode *node = nodes;
    TextPoolNode *next;

    do {
        count--;
        next = (TextPoolNode *)((u8 *)node + stride);
        node->previous = previous;
        node->index = index;
        index++;
        node->next = next;
        previous = node;
        node = next;
    } while (count >= 2);
    node->previous = previous;
    node->index = index;
    node->next = NULL;
    pool->lastFree = node;
    pool->firstFree = nodes;
    pool->activeTail = NULL;
    pool->activeHead = NULL;
}

TextPoolNode *itfAcquirePoolNode(TextPool *pool) {
    TextPoolNode *node = pool->firstFree;
    TextPoolNode *next;

    if (node == NULL) {
        return NULL;
    }
    next = node->next;
    if (pool->activeHead != NULL) {
        node->previous = pool->activeTail;
        pool->activeTail->next = node;
    } else {
        node->previous = NULL;
        pool->activeHead = node;
    }
    node->next = NULL;
    pool->activeTail = node;
    if (next != NULL) {
        next->previous = NULL;
    } else {
        pool->lastFree = NULL;
    }
    pool->firstFree = next;
    return node;
}

void itfReleasePoolNode(TextPoolNode *node, TextPool *pool) {
    TextPoolNode *previous = node->previous;
    TextPoolNode *next = node->next;

    if (previous != NULL) {
        previous->next = next;
    } else {
        pool->activeHead = next;
    }
    if (next != NULL) {
        next->previous = previous;
    } else {
        pool->activeTail = previous;
    }
    node->next = NULL;
    {
        TextPoolNode *freeTail = pool->lastFree;
        node->previous = freeTail;
        if (freeTail != NULL) {
            freeTail->next = node;
        }
    }
    pool->lastFree = node;
    if (pool->firstFree == NULL) {
        pool->firstFree = node;
    }
}

void itfScaleVectors(TextVector *output, s32 scaleX, s32 scaleY, s32 scaleZ,
                   s32 w, const TextVector *input, s32 count) {
    while (count > 0) {
        s32 x = scaleX * input->x;
        s32 y = scaleY * input->y;
        s32 z = scaleZ * input->z;

        output->w = w;
        if (x > 0xff0000) x = 0xff0000;
        if (y > 0xff0000) y = 0xff0000;
        if (z > 0xff0000) z = 0xff0000;
        output->x = x >> 16;
        output->y = y >> 16;
        output->z = z >> 16;
        output++;
        input++;
        count--;
    }
}

void itfSetStyleColor(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = color;
        }
    }
}

void itfSetStyleColorBits(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = (child->color & ~0xff) | color;
        }
    }
}

void itfTranslateStyleEntries(TextStyleNode *entry, u32 xOffset, u32 yOffset) {
    for (; entry != NULL; entry = entry->next) {
        entry->x += xOffset;
        entry->y += yOffset;
    }
}

/* Keep the sprite texture handle while releasing the temporary file allocation. */
u64 itfLoadTextureFromAsset(const char *path) {
    u64 fileAllocation;
    u64 textureHandle;
    u32 assetInfo[4];

    fileAllocation = func_00343ED0(path, assetInfo, 0);
    textureHandle = func_0032C138(assetInfo[0]);
    func_003297C8(fileAllocation);
    return textureHandle;
}

typedef struct DrawVertex {
    s32 x;
    s32 y;
} DrawVertex;

typedef struct DrawColorRec {
    u32 word[4];
} DrawColorRec;

/* Pack each RGBA/XYZ pair into GS qwords; the 0x7000/0x7900 biases place
 * vertex coordinates in the GS screen-space origin. */
void itfDrawTriFlat3(DrawVertex *vertices, DrawColorRec *colors, s32 xOffset, s32 yOffset, u32 tail, void *command) {
    void *packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 3));
    func_0033A2D8(packet, 0x4B, 2, 0x51, 3);
    dst = func_0033A2D0(packet);
    for (i = 0; i < 3; i++) {
        dst[0] = (u64)colors->word[0] | ((u64)colors->word[1] << 32);
        dst[1] = (u64)colors->word[2] | ((u64)colors->word[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + xOffset + 0x7000) | ((u64)(vertices->y + yOffset + 0x7900) << 32);
        vertices++;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

void itfDrawQuadFlat4(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, void *command) {
    void *packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_0033A2D8(packet, 0x4D, 2, 0x51, 4);
    dst = func_0033A2D0(packet);
    for (i = 0; i < 4; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->word[0] | ((u64)color->word[1] << 32);
        dst[1] = (u64)color->word[2] | ((u64)color->word[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + 0x7000) | ((u64)(vertex->y + 0x7900) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A09C0);

void itfDrawQuadTextured4(DrawVertex *vertices, f32 *uvs, DrawColorRec *colors, u32 tail, s32 flag, void *command) {
    void *packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, 4));
    func_0033A2D8(packet, (flag << 9) | 0x5D, 3, 0x512, 4);
    dst = func_0033A2D0(packet);
    for (i = 0; i < 4; i++) {
        f32 *uvDst = (f32 *)dst;

        uvDst[0] = uvs[0];
        uvDst[1] = uvs[1];
        uvDst[2] = uvs[2];
        uvs += 4;
        dst += 2;
        dst[0] = (u64)colors->word[0] | ((u64)colors->word[1] << 32);
        dst[1] = (u64)colors->word[2] | ((u64)colors->word[3] << 32);
        colors++;
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertices->x + 0x7000) | ((u64)(vertices->y + 0x7900) << 32);
        vertices++;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0CA0);

void func_001A0E20(DrawVertex *vertices, DrawColorRec *uv, DrawColorRec *colors, u32 tail, s32 flag, void *command) {
    void *packet;
    u64 *dst;
    s32 x0, y0, x1, y1;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(5, 1));
    func_0033A2D8(packet, (flag << 9) | 0x15E, 5, 0x53531, 1);
    x0 = vertices[0].x + 0x7000;
    y0 = vertices[0].y + 0x7900;
    x1 = vertices[1].x + 0x7000;
    y1 = vertices[1].y + 0x7900;
    dst = func_0033A2D0(packet);
    dst[0] = (u64)colors->word[0] | ((u64)colors->word[1] << 32);
    dst[1] = (u64)colors->word[2] | ((u64)colors->word[3] << 32);
    dst[2] = (u64)uv->word[0] | ((u64)uv->word[1] << 32);
    dst[4] = (u64)(u32)x0 | ((u64)y0 << 32);
    dst[5] = (u64)tail;
    dst[6] = (u64)uv->word[2] | ((u64)uv->word[3] << 32);
    dst[8] = (u64)(u32)x1 | ((u64)y1 << 32);
    dst[9] = (u64)tail;
    sdfAppendPacket(command, packet);
}

void func_001A0F88(DrawVertex *vertices, DrawColorRec *colors, u32 tail, s32 flag, void *command) {
    void *packet;
    u64 *dst;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, 1));
    func_0033A2D8(packet, (flag << 9) | 0x46, 3, 0x551, 1);
    dst = func_0033A2D0(packet);
    dst[0] = (u64)colors->word[0] | ((u64)colors->word[1] << 32);
    dst[1] = (u64)colors->word[2] | ((u64)colors->word[3] << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[0].x + 0x7000) | ((u64)(vertices[0].y + 0x7900) << 32);
    dst += 2;
    dst[1] = (u64)tail;
    dst[0] = (u64)(u32)(vertices[1].x + 0x7000) | ((u64)(vertices[1].y + 0x7900) << 32);
    sdfAppendPacket(command, packet);
}

void itfEmitQuadListWide(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, void *command) {
    void *packet;
    u64 *dst;
    s32 half = count >> 1;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(4, half));
    func_0033A2D8(packet, 0x4C, 4, 0x5151, half);
    dst = func_0033A2D0(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->word[0] | ((u64)color->word[1] << 32);
        dst[1] = (u64)color->word[2] | ((u64)color->word[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + 0x7000) | ((u64)(vertex->y + 0x7900) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

void itfEmitQuadListA(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, void *command) {
    void *packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    func_0033A2D8(packet, 0x14A, 2, 0x51, count);
    dst = func_0033A2D0(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->word[0] | ((u64)color->word[1] << 32);
        dst[1] = (u64)color->word[2] | ((u64)color->word[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + 0x7000) | ((u64)(vertex->y + 0x7900) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

void itfEmitQuadListB(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, void *command) {
    void *packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    func_0033A2D8(packet, 0x49, 2, 0x51, count);
    dst = func_0033A2D0(packet);
    for (i = 0; i < count; i++) {
        DrawVertex *vertex = &vertices[*vertexIndex++];
        DrawColorRec *color = &colors[*colorIndex++];

        dst[0] = (u64)color->word[0] | ((u64)color->word[1] << 32);
        dst[1] = (u64)color->word[2] | ((u64)color->word[3] << 32);
        dst += 2;
        dst[1] = (u64)tail;
        dst[0] = (u64)(u32)(vertex->x + 0x7000) | ((u64)(vertex->y + 0x7900) << 32);
        dst += 2;
    }
    sdfAppendPacket(command, packet);
}

extern void *sdfAllocPacketAligned(s32);
extern u8 *func_0033A290(void *, s32);
extern void sdfAppendPacket();

void itfSendBlendPacket(void *list, s64 value, s32 flag) {
    void *packet = sdfAllocPacketAligned(0x30);
    u8 *command = func_0033A290(packet, 0x30);
    *(s64 *)(command + 0x20) = value;
    *(s64 *)(command + 0x28) = flag != 0 ? 0x48 : 0x47;
    sdfAppendPacket(list, packet);
}

extern void *sdfAllocPacketAligned(s32);
extern u8 *func_0033A290(void *, s32);
extern void sdfAppendPacket();
extern s64 D_003B4390[];

void itfSendTablePacket(void *list, s32 index, s32 flag) {
    void *packet = sdfAllocPacketAligned(0x30);
    u8 *command = func_0033A290(packet, 0x30);
    s64 value = D_003B4390[index];
    *(s64 *)(command + 0x20) = value;
    *(s64 *)(command + 0x28) = flag != 0 ? 0x43 : 0x42;
    sdfAppendPacket(list, packet, value);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1590);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1668);

u32 func_001A1810(void) {
    return 0;
}

u32 func_001A1818(void) {
    return 1;
}

void mnuReportCampProcessHalted(void) {
    if (func_00102930() != 5) {
        func_00102908();
    }
    func_0035B6E0(D_00414C50);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1858);

INCLUDE_RODATA(const s32, "game/code_0019E138", D_00414C50);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436580);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436584);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436588);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_0043658C);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436590);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436598);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_004365A0);


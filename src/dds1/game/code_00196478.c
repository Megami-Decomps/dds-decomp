#include "common.h"

extern s32 func_002D3288(u32);
extern s32 func_00102A40(void);
extern void func_00102A18(void);
extern void func_003003F0(const char *);
extern u64 func_002EB028(const char *, u32 *, u64);

extern u32 D_003BB190;
extern s64 kwlnTaskGetRegisteredState(u32);

extern u32 D_003BB18C;

extern u32 D_003BD81C;

extern u64 func_001951C8(u64, u64, u64, u64, u64);

extern u64 func_00195160(u64, u64, u64, u64, u64);

extern void frFontSetEntryFlag(s32 kind, u64 flag);

extern void func_00195520(u64 value);

extern void func_00195530(u64 value);

extern void frFontSetChainFlag(u64 glyph, u64 value);

extern s32 D_003BB168;

extern u32 D_003BB170;

extern u32 D_003BB16C;
extern u32 D_003BD818;
void func_00194190(s32 id, const char *path);
extern u16 D_00356620[];
extern u32 strlen(const char *str);

/* Byte stream read by itfReadEncodedTextLead/itfReadEncodedCode: base at +0x10, position at +0x18. */
typedef struct TextStream {
    u8 unk0[0x10]; /* 0x0 */
    u8 *bytes;       /* 0x10: encoded input base */
    u8 unk14[4];   /* 0x14 */
    s32 offset;      /* 0x18: current byte position */
} TextStream;

/* 8-byte node header; payload follows (itfDequeueMemNode/itfEnqueueMemNode). */
typedef struct MemNode {
    u32 index;             /* 0x0 */
    struct MemNode *next;  /* 0x4 */
} MemNode;

/* Field block split by itfSplitRelativeSegments. */
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

/* Value with u16 pair read by func_001971E0/func_00197200. */
typedef struct Unk6C84Val {
    u8 unk0[0x10]; /* 0x0 */
    u16 unk10;     /* 0x10 */
    u16 unk12;     /* 0x12 */
} Unk6C84Val;

/* 0x24-byte record pointing at the value. */
typedef struct Unk6C84Rec {
    Unk6C84Val *unk0; /* 0x0 */
    u8 unk4[0x20];    /* 0x4 */
} Unk6C84Rec;

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

typedef struct TextVector {
    s32 x;
    s32 y;
    s32 z;
    s32 w;
} TextVector;

typedef struct TextDrawArgs {
    s32 x;         /* 0x00 */
    s32 y;         /* 0x04 */
    s32 z;         /* 0x08 */
    u8 color[4];   /* 0x0C */
    s32 encodedText; /* 0x10: encoded input base */
    s32 sub;         /* 0x14: text subcontext */
    s32 offset;    /* 0x18: current byte position */
    u8 unk1C;      /* 0x1C */
    u8 unk1D;      /* 0x1D */
} TextDrawArgs;

extern u32 D_003BB15C;
extern u32 D_003D6E20[];
extern Unk6C84Rec D_003D6C84[];
extern s32 D_003BAA98;
extern s32 D_003BAA9C;
s32 func_00196B30(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, s32 encodedText, s32 sub);

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

INCLUDE_ASM(const s32, "game/code_00196478", func_001964F8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001968C0);

void func_00196AB0(s32 x, s32 y, s32 encodedText, s32 sub) {
    func_00196B30(x, y, 0, 0, 0, 0, 0x80, encodedText, sub);
}

void func_00196AE8(s32 x, s32 y, s32 channel0, s32 channel1, s32 channel2, s32 channel3, s32 encodedText, s32 sub) {
    func_00196B30(x, y, 0, channel0 & 0xFF, channel1 & 0xFF, channel2 & 0xFF, channel3 & 0xFF, encodedText, sub);
}

extern s32 func_001968C0(TextDrawArgs *);

s32 func_00196B30(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1, s32 channel2, s32 channel3, s32 encodedText, s32 sub) {
    TextDrawArgs args;

    args.x = x;
    args.y = y;
    args.z = depth << 4;
    args.color[0] = channel0;
    args.color[1] = channel1;
    args.color[2] = channel2;
    args.color[3] = channel3;
    args.encodedText = encodedText;
    args.sub = sub;
    args.offset = 0;
    args.unk1C = 1;
    args.unk1D = 1;
    return func_001968C0(&args);
}

void itfDrawColor(s32 x, s32 y, s32 depth, s32 channel0, s32 channel1,
                   s32 channel2, s32 channel3, s32 encodedText, s32 unusedSub) {
    func_00196B30(x, y, depth, channel0 & 0xff, channel1 & 0xff,
                  channel2 & 0xff, channel3 & 0xff, encodedText, 0);
}

u32 func_00196BB0(u32 mask) {
    return D_003BB15C & mask;
}

void func_00196BC0(s32 index, s32 value) {
    D_003D6E20[index] = value;
}

u32 func_00196BD8(void) {
    return D_003BB16C;
}

u32 func_00196BE0(void) {
    return D_003BB170;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196BE8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196ED0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197068);

void itfInitTextDrawArgs(s32 encodedText, s32 sub) {
    TextDrawArgs args;

    args.x = 0;
    args.y = 0;
    args.z = 0;
    args.color[0] = 0;
    args.color[1] = 0;
    args.color[2] = 0;
    args.color[3] = 0;
    args.encodedText = encodedText;
    args.sub = sub;
    args.offset = 0;
    args.unk1C = 1;
    args.unk1D = 1;
    func_00197068(&args);
}

u16 func_001971E0(s32 index) {
    return D_003D6C84[index].unk0->unk10;
}

u16 func_00197200(s32 index) {
    return D_003D6C84[index].unk0->unk12;
}

void func_00197220(s32 limit) {
    if (limit < 1) {
        limit = 0x14;
    }
    D_003BB168 = limit;
}

u64 func_00197238(u64 x, u64 y, s32 depth, s32 alt, u64 measureFlag, u64 entryFlag, u64 colors, u64 source) {
    u64 glyph;
    s32 kind = 4;

    if (alt) {
        kind = 5;
    }
    frFontSetEntryFlag(kind, entryFlag);
    func_00195520(1);
    func_00195530(2);
    func_00195530(0x10);
    glyph = func_00195160(source, kind, 0, 0, 0);
    func_00195520(0x10);
    func_00195520(2);
    func_00195530(1);
    frFontSetFlagAndMeasureGlyphs(glyph, measureFlag);
    func_00195450(glyph, x, y);
    func_00195460(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetChainFlag(glyph, 5);
    return glyph;
}

void mnuLoadStaffFonts(void) {
    func_00194190(4, "/font/staff1.fnt");
    func_00194190(5, "/font/staff2.fnt");
}

void mnuUnloadStaffFonts(void) {
    frFontFreeEntry(4);
    frFontFreeEntry(5);
}

/* Decode a two-byte glyph through the DDS1 lookup table; 0xFFFF is missing. */
u32 itfDecodeGlyph(u32 value) {
    s32 adjusted = (value & 0xffff) + 0xffff7f80;
    s32 index = ((adjusted & 0xff00) >> 1) + (adjusted & 0x7f);

    if ((u32)index < 0x9b0) {
        return D_00356620[index];
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
void itfAttachGlyph16x18(u64 x, u64 y, s32 depth, u64 colors,
                                    u64 glyphSource, u64 parent) {
    u64 glyph;

    glyph = func_00195160(glyphSource, 0, 0, 0, 0);
    func_001953D8(glyph, 0x10, 0x12);
    func_00195450(glyph, x, y);
    func_00195460(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontSetFlagAndMeasureGlyphs(glyph, 0xfffffffffffffffc);
    frFontLinkGlyph(parent, glyph, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197580);

s32 func_00197708(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
s32 arg1;
s32 arg2;
s32 arg3;
s32 arg4;
s32 arg5;
{
    s32 handle = func_00197580(arg0, arg1, arg2, arg3, arg4, 1, 0, arg5);

    frFontSetFlagAndMeasureGlyphs(handle, 3);
    return handle;
}

void func_00197748(void) {
    func_00197708();
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197760);

INCLUDE_ASM(const s32, "game/code_00196478", func_001978E8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001979C8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197A98);

/* Attach the alternate font glyph, with a 12x16 cell, to its parent. */
void itfAttachGlyph12x16(u64 x, u64 y, s32 depth, u64 colors,
                                    u64 glyphSource, u64 parent) {
    u64 glyph;

    glyph = func_001951C8(glyphSource, 0, 0, 0, 0);
    func_001953D8(glyph, 0xc, 0x10);
    frFontSetFlagAndMeasureGlyphs(glyph, 3);
    func_00195450(glyph, x, y);
    func_00195460(glyph, depth << 4);
    frFontSetChildColors(glyph, colors);
    frFontLinkGlyph(parent, glyph, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197C40);

s32 func_00197E08(s32 arg0, s32 arg1, s32 arg2, s8 arg3, u16 arg4, s32 arg5) {
    s32 result = 0;

    func_00197220(0x13);
    switch (arg3) {
    case 0:
        result = func_00197C40(arg0, arg1, arg2, arg4, D_003BAA98, arg5);
        break;
    case 1:
        result = func_00197C40(arg0, arg1, arg2, arg4, D_003BAA9C, arg5);
        break;
    }
    func_00197220(-1);
    return result;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197EC8);

u32 func_00198008(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

u32 func_00198020(u32 unused0, u32 unused1, u32 value) {
    return value;
}

u32 func_00198028(void) {
    return D_003BD818;
}

u32 func_00198030(u32 value) {
    return value;
}

void itfSplitRelativeSegments(MemBlock *block, MemOut *out) {
    s32 firstOffset = block->firstOffset;
    s32 secondOffset = firstOffset + block->secondDelta;
    s32 thirdOffset = secondOffset + block->thirdDelta;

    out->first = (u8 *)block + firstOffset;
    out->second = (u8 *)block + secondOffset;
    out->third = (u8 *)block + thirdOffset;
}

u32 func_00198068(u32 object) {
    return *(u32 *)(func_00198030(object) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198088);

/* The header before each payload forms a circular free-node list. */
u32 itfCreateMemNodeRing(s32 payloadBytes, s32 count) {
    u32 buffer;
    u8 *list;
    MemNode *cursor;
    MemNode *next;
    s32 i;

    buffer = func_002D03F8((payloadBytes + 8) * (count + 1) + 4);
    list = (u8 *)sdfResourceRetainAddress(buffer);
    i = 0;
    memcpy(list, &buffer, 4);
    list += 4;
    cursor = (MemNode *)list;
    for (; i < count; i++) {
        next = (MemNode *)((u8 *)cursor + payloadBytes + 8);
        cursor->index = i;
        cursor->next = next;
        cursor = next;
    }
    cursor->index = count;
    cursor->next = (MemNode *)list;
    return (u32)list;
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

u32 itfReleaseMemNodeBuffer(u8 *payload) {
    func_002D0918(*(u32 *)(payload - 4));
    return 1;
}

void itfLoadBackgroundSprite(void) {
    u32 resource;
    u64 buffer = func_002EB028("/sprite/bg00.tmx", &resource, 0);

    D_003BD81C = func_002D3288(resource);
    func_002D0918(buffer);
}

void func_00198308(void) {
    sdfTexReleaseReference(D_003BD81C);
}

typedef struct TextBackgroundSprite {
    u8 pad00[0xC];
    s16 width;
    s16 height;
} TextBackgroundSprite;

void itfDrawBackgroundSprite(void) {
    s32 origin[4];
    s32 color[4];
    TextBackgroundSprite *panel = (TextBackgroundSprite *)D_003BD81C;

    if (panel != NULL) {
        s32 x = panel->width;
        s32 y = panel->height;

        origin[0] = 0;
        origin[1] = 0;
        origin[2] = x;
        origin[3] = y;
        color[0] = 0x80808080;
        color[1] = 0x80808080;
        color[2] = 0x80808080;
        color[3] = 0x80808080;
        func_002BE4B8(0, 0, 0, x << 4, y << 3, origin, color, 0, 0, 1, (u8 *)panel, 0x52);
    }
}

extern s32 kwlnTaskCreate(s32, s32, s32, s32, s32, s32, s32);
extern s32 scrCreateProcessTaskFromResource(s32, char *, s32);
extern u32 itfDrawBackgroundAndGetTaskReadyMask(void);
extern void func_00198408(void);

void func_001983A8(void) {
    itfLoadBackgroundSprite();
    kwlnTaskCreate((s32)"test_font", 0x2B06, 0, 0, (s32)itfDrawBackgroundAndGetTaskReadyMask, (s32)func_00198408, 0);
    D_003BB190 = scrCreateProcessTaskFromResource(0x258, "host0:../../../dds3data/font/test.bf", 0);
}

void func_00198408(void) {
    func_00194800(D_003BB18C);
    func_00198308();
}

/* Return an all-bits-set ready mask only while the registered task is in state 3. */
u32 itfDrawBackgroundAndGetTaskReadyMask(void) {
    s64 taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = kwlnTaskGetRegisteredState(D_003BB190);
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

extern u64 sdfAllocPacketAligned(u32);
extern u32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void sdfConsInitPacketHeader(u64, s32, s32, s32, s32);
extern u64 *func_002E1420(u64);
extern void sdfAppendPacket(u64, u64);
extern u64 *func_002E13E0(u64, s32);
extern u64 D_00357998[];

typedef struct DrawVertex {
    s32 x;
    s32 y;
} DrawVertex;

typedef struct DrawColorRec {
    u32 word[4];
} DrawColorRec;

/* Keep the sprite texture handle while releasing the temporary file allocation. */
u64 itfLoadTextureFromAsset(const char *path) {
    u64 fileAllocation;
    u64 textureHandle;
    u32 assetInfo[4];

    fileAllocation = func_002EB028(path, assetInfo, 0);
    textureHandle = func_002D3288(assetInfo[0]);
    func_002D0918(fileAllocation);
    return textureHandle;
}

/* Pack each RGBA/XYZ pair into GS qwords; the 0x7000/0x7900 biases place
 * vertex coordinates in the GS screen-space origin. */
void itfDrawTriFlat3(DrawVertex *vertices, DrawColorRec *colors, s32 xOffset, s32 yOffset, u32 tail, u64 command) {
    u64 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 3));
    sdfConsInitPacketHeader(packet, 0x4B, 2, 0x51, 3);
    dst = func_002E1420(packet);
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

void itfDrawQuadFlat4(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, u32 tail, u64 command) {
    u64 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    sdfConsInitPacketHeader(packet, 0x4D, 2, 0x51, 4);
    dst = func_002E1420(packet);
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

INCLUDE_ASM(const s32, "game/code_00196478", func_00198990);

void itfDrawQuadTextured4(DrawVertex *vertices, f32 *uvs, DrawColorRec *colors, u32 tail, s32 flag, u64 command) {
    u64 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, 4));
    sdfConsInitPacketHeader(packet, (flag << 9) | 0x5D, 3, 0x512, 4);
    dst = func_002E1420(packet);
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

INCLUDE_ASM(const s32, "game/code_00196478", func_00198C70);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198DF0);

void func_00198F58(DrawVertex *vertices, DrawColorRec *colors, u32 tail, s32 flag, u64 command) {
    u64 packet;
    u64 *dst;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(3, 1));
    sdfConsInitPacketHeader(packet, (flag << 9) | 0x46, 3, 0x551, 1);
    dst = func_002E1420(packet);
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

void itfEmitQuadListWide(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, u64 command) {
    u64 packet;
    u64 *dst;
    s32 half = count >> 1;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(4, half));
    sdfConsInitPacketHeader(packet, 0x4C, 4, 0x5151, half);
    dst = func_002E1420(packet);
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

void itfEmitQuadListA(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, u64 command) {
    u64 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    sdfConsInitPacketHeader(packet, 0x14A, 2, 0x51, count);
    dst = func_002E1420(packet);
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

void itfEmitQuadListB(DrawVertex *vertices, DrawColorRec *colors, u8 *vertexIndex, u8 *colorIndex, s32 count, u32 tail, u64 command) {
    u64 packet;
    u64 *dst;
    s32 i;

    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, count));
    sdfConsInitPacketHeader(packet, 0x49, 2, 0x51, count);
    dst = func_002E1420(packet);
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

void itfSendBlendPacket(u64 command, u64 value, s32 flag) {
    u64 packet = sdfAllocPacketAligned(0x30);
    u64 *dst = func_002E13E0(packet, 0x30);

    dst[4] = value;
    dst[5] = flag ? 0x48 : 0x47;
    sdfAppendPacket(command, packet);
}

void itfSendTablePacket(u64 command, s32 index, s32 flag) {
    u64 packet = sdfAllocPacketAligned(0x30);
    u64 *dst = func_002E13E0(packet, 0x30);

    dst[4] = D_00357998[index];
    dst[5] = flag ? 0x43 : 0x42;
    sdfAppendPacket(command, packet);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00199560);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199638);

u32 func_001997E0(void) {
    return 0;
}

u32 func_001997E8(void) {
    return 1;
}

extern char D_003A13F0[]; /* "Camp process halted.\n", followed by padding no C emits */

void mnuReportCampProcessHalted(void) {
    if (func_00102A40() != 5) {
        func_00102A18();
    }
    func_003003F0(D_003A13F0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00199828);

INCLUDE_RODATA(const s32, "game/code_00196478", D_003A13F0);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB188);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB18C);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB190);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB198);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB1A0);


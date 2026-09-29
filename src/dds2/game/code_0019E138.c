#include "common.h"

extern u32 D_0043658C;

extern u32 D_00438F24;

extern u64 func_0019CE78(u64, u64, u64, u64, u64);

extern u64 func_0019CE10(u64, u64, u64, u64, u64);

extern u32 D_0043654C;

extern u32 D_0043655C;

extern u32 D_00436560;

extern u32 D_00436590;

extern s64 func_00101700(u32);

/* Byte stream read by func_00196478/func_001964A0: base at +0x10, position at +0x18. */
typedef struct TextStream {
    u8 unk0[0x10]; /* 0x0 */
    u8 *bytes;       /* 0x10: encoded input base */
    u8 unk14[4];   /* 0x14 */
    s32 offset;      /* 0x18: current byte position */
} TextStream;

s32 func_0019E848(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

extern u32 D_004528C0[];

/* Field block split by func_00198038. */
typedef struct MemBlock {
    s32 unk0; /* 0x0 */
    s32 unk4; /* 0x4 */
    u8 unk8[0x10]; /* 0x8 */
    s32 unk18; /* 0x18 */
} MemBlock;

typedef struct MemOut {
    void *unk0; /* 0x0 */
    void *unk4; /* 0x4 */
    void *unk8; /* 0x8 */
} MemOut;

/* 8-byte node header; payload follows (func_00198248/itfEnqueueMemNode). */
typedef struct MemNode {
    u32 unk0;              /* 0x0 */
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

u32 func_0019E138(TextStream *stream) {
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

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E848);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E8A0);

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

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EEE8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EF38);

void mnuLoadStaffFonts(void) {
    func_0019BE20(4, "/font/staff1.fnt");
    func_0019BE20(5, "/font/staff2.fnt");
}

void func_0019F078(void) {
    frFontFreeEntry(4);
    frFontFreeEntry(5);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F098);

void itfConvertText(u8 *output, const char *input) {
    s32 i;
    s32 length = strlen(input);

    for (i = 0; i < length; i++, output++) {
        if (input[i] >= 0) {
            output[0] = input[i];
        } else {
            u32 value = func_0019F098((u8)input[i + 1] | ((u8)input[i] << 8));
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

void func_0019F1B8(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_0019CE10(arg4, 0, 0, 0, 0);
    func_0019D088(temp_v0, 0x10, 0x12);
    func_0019D100(temp_v0, arg0, arg1);
    func_0019D110(temp_v0, arg2 << 4);
    func_0019D178(temp_v0, arg3);
    func_0019D058(temp_v0, 0xfffffffffffffffc);
    frFontLinkGlyph(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F280);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F408);

void func_0019F448(void) {
    func_0019F408();
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F460);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F5E8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F6C8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F798);

void func_0019F878(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_0019CE78(arg4, 0, 0, 0, 0);
    func_0019D088(temp_v0, 0xc, 0x10);
    func_0019D058(temp_v0, 0xfffffffffffffffd);
    func_0019D100(temp_v0, arg0, arg1);
    func_0019D110(temp_v0, arg2 << 4);
    func_0019D178(temp_v0, arg3);
    frFontLinkGlyph(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F940);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F990);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FA08);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FC38);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FEF8);

u32 func_001A0038(void) {
    return 0;
}

void func_001A0040(void) {
}

void func_001A0048(void) {
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0050);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0058);

u32 func_001A0060(u32 arg0) {
    return arg0;
}

void func_001A0068(MemBlock *arg0, MemOut *arg1) {
    s32 v0 = arg0->unk0;
    s32 v1 = v0 + arg0->unk4;
    s32 v2 = v1 + arg0->unk18;

    arg1->unk0 = (u8 *)arg0 + v0;
    arg1->unk4 = (u8 *)arg0 + v1;
    arg1->unk8 = (u8 *)arg0 + v2;
}

u32 func_001A0098(u32 arg0) {
    return *(u32 *)(func_001A0060(arg0) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A00B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A01D8);

void *func_001A0278(MemNode *queue) {
    MemNode *head = queue->next;

    if (head->unk0 == 0) {
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

u32 func_001A02D0(s32 arg0) {
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
    func_0032BB68(D_00438F24);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0350);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A03D8);

void func_001A0438(void) {
    func_0019C490(D_0043658C);
    func_001A0338();
}

u32 func_001A0458(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_001A0350();
    temp_v0 = func_00101700(D_00436590);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
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

u64 func_001A0710(const char *arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0760);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0888);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A09C0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0B60);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0CA0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0E20);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0F88);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A10B0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1200);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1348);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1490);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1508);

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

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436580);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436584);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436588);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_0043658C);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436590);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_00436598);

INCLUDE_SDATA(const s32, "game/code_0019E138", D_004365A0);

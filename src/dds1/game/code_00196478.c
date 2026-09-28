#include "common.h"

extern s32 func_002D3288(u32);
extern s32 func_00102A40(void);
extern void func_00102A18(void);
extern void func_003003F0(const char *);
extern u64 func_002EB028(const char *, u32 *, u64);

extern u32 D_003BB190;
extern s64 func_00101818(u32);

extern u32 D_003BB18C;

extern u32 D_003BD81C;

extern u64 func_001951C8(u64, u64, u64, u64, u64);

extern u64 func_00195160(u64, u64, u64, u64, u64);

extern s32 D_003BB168;

extern u32 D_003BB170;

extern u32 D_003BB16C;
extern u32 D_003BD818;
void func_00194190(s32 id, const char *path);
extern u16 D_00356620[];
extern u32 strlen(const char *str);

/* Byte stream read by func_00196478/readEncodedTextCode: base at +0x10, position at +0x18. */
typedef struct TextStream {
    u8 unk0[0x10]; /* 0x0 */
    u8 *unk10;     /* 0x10: base */
    u8 unk14[4];   /* 0x14 */
    s32 unk18;     /* 0x18: position */
} TextStream;

/* 8-byte node header; payload follows (func_00198248/func_00198270). */
typedef struct MemNode {
    u32 unk0;              /* 0x0 */
    struct MemNode *unk4;  /* 0x4 */
} MemNode;

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

extern u32 D_003BB15C;
extern u32 D_003D6E20[];
extern Unk6C84Rec D_003D6C84[];
s32 func_00196B30(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8);

u32 func_00196478(TextStream *stream) {
    s32 *ppos = &stream->unk18;
    u8 *p = stream->unk10 + *ppos;
    u32 b = *p;

    *ppos += 2;
    return (b + 0xFF) & 0xFF;
}

u32 readEncodedTextCode(TextStream *stream) {
    u32 first;
    u32 second;

    first = (stream->unk10[stream->unk18++] + 0xff) & 0xff;
    second = stream->unk10[stream->unk18++];
    if (second == 0xff) {
        second = 0;
    } else {
        second = (second + 0xff) & 0xff;
    }
    return (second << 8) | first;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001964F8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001968C0);

void func_00196AB0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00196B30(arg0, arg1, 0, 0, 0, 0, 0x80, arg2, arg3);
}

void func_00196AE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    func_00196B30(arg0, arg1, 0, arg2 & 0xFF, arg3 & 0xFF, arg4 & 0xFF, arg5 & 0xFF, arg6, arg7);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196B30);

void drawTextColor(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4,
                   s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    func_00196B30(arg0, arg1, arg2, arg3 & 0xff, arg4 & 0xff,
                  arg5 & 0xff, arg6 & 0xff, arg7, 0);
}

u32 func_00196BB0(u32 arg0) {
    return D_003BB15C & arg0;
}

void func_00196BC0(s32 arg0, s32 arg1) {
    D_003D6E20[arg0] = arg1;
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

INCLUDE_ASM(const s32, "game/code_00196478", func_00197190);

u16 func_001971E0(s32 arg0) {
    return D_003D6C84[arg0].unk0->unk10;
}

u16 func_00197200(s32 arg0) {
    return D_003D6C84[arg0].unk0->unk12;
}

void func_00197220(s32 arg0) {
    if (arg0 < 1) {
        arg0 = 0x14;
    }
    D_003BB168 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197238);

void loadStaffFonts(void) {
    func_00194190(4, "/font/staff1.fnt");
    func_00194190(5, "/font/staff2.fnt");
}

void func_00197378(void) {
    frFontFreeEntry(4);
    frFontFreeEntry(5);
}

u32 decodeFontGlyph(u32 value) {
    s32 adjusted = (value & 0xffff) + 0xffff7f80;
    s32 index = ((adjusted & 0xff00) >> 1) + (adjusted & 0x7f);

    if ((u32)index < 0x9b0) {
        return D_00356620[index];
    }
    return 0xffff;
}

void convertFontText(u8 *output, const char *input) {
    s32 i;
    s32 length = strlen(input);

    for (i = 0; i < length; i++, output++) {
        if (input[i] >= 0) {
            output[0] = input[i];
        } else {
            u32 value = decodeFontGlyph((u8)input[i + 1] | ((u8)input[i] << 8));
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

void func_001974B8(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_00195160(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0x10, 0x12);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    func_001953A8(temp_v0, 0xfffffffffffffffc);
    frFontLinkGlyph(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197580);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197708);

void func_00197748(void) {
    func_00197708();
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197760);

INCLUDE_ASM(const s32, "game/code_00196478", func_001978E8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001979C8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197A98);

void func_00197B78(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_001951C8(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0xc, 0x10);
    func_001953A8(temp_v0, 3);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    frFontLinkGlyph(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197C40);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197E08);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197EC8);

u32 func_00198008(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

u32 func_00198020(u32 arg0, u32 arg1, u32 arg2) {
    return arg2;
}

u32 func_00198028(void) {
    return D_003BD818;
}

u32 func_00198030(u32 arg0) {
    return arg0;
}

void func_00198038(MemBlock *arg0, MemOut *arg1) {
    s32 v0 = arg0->unk0;
    s32 v1 = v0 + arg0->unk4;
    s32 v2 = v1 + arg0->unk18;

    arg1->unk0 = (u8 *)arg0 + v0;
    arg1->unk4 = (u8 *)arg0 + v1;
    arg1->unk8 = (u8 *)arg0 + v2;
}

u32 func_00198068(u32 arg0) {
    return *(u32 *)(func_00198030(arg0) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198088);

INCLUDE_ASM(const s32, "game/code_00196478", func_001981A8);

void *func_00198248(MemNode *queue) {
    MemNode *head = queue->unk4;

    if (head->unk0 == 0) {
        return NULL;
    }
    queue->unk4 = head->unk4;
    head->unk4 = NULL;
    return head + 1;
}

s32 func_00198270(void *payload, MemNode *queue) {
    MemNode *node = (MemNode *)payload - 1;
    if (payload == NULL) {
        return 0;
    }
    if (node->unk4 != NULL) {
        return 0;
    }
    node->unk4 = queue->unk4;
    queue->unk4 = node;
    return 1;
}

u32 func_001982A0(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 - 4));
    return 1;
}

void loadBackgroundSprite(void) {
    u32 resource;
    u64 buffer = func_002EB028("/sprite/bg00.tmx", &resource, 0);

    D_003BD81C = func_002D3288(resource);
    func_002D0918(buffer);
}

void func_00198308(void) {
    func_002D2CB8(D_003BD81C);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198320);

INCLUDE_ASM(const s32, "game/code_00196478", func_001983A8);

void func_00198408(void) {
    func_00194800(D_003BB18C);
    func_00198308();
}

u32 func_00198428(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_00198320();
    temp_v0 = func_00101818(D_003BB190);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
}

void func_00198460(TextPool *pool, TextPoolNode *nodes, s32 count, s32 stride) {
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

TextPoolNode *acquireTextPoolNode(TextPool *pool) {
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

void releaseTextPoolNode(TextPoolNode *node, TextPool *pool) {
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

void scaleTextVectors(TextVector *output, s32 scaleX, s32 scaleY, s32 scaleZ,
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

void setTextStyleColor(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = color;
        }
    }
}

void setTextStyleColorBits(TextStyleNode *entry, u32 color) {
    for (; entry != NULL; entry = entry->next) {
        TextStyleNode *child;
        for (child = entry->firstChild; child != NULL; child = child->nextChild) {
            child->color = (child->color & ~0xff) | color;
        }
    }
}

void translateTextStyleEntries(TextStyleNode *entry, u32 xOffset, u32 yOffset) {
    for (; entry != NULL; entry = entry->next) {
        entry->x += xOffset;
        entry->y += yOffset;
    }
}

u64 func_001986E0(const char *arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002D3288(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198730);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198858);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198990);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198B30);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198C70);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198DF0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198F58);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199080);

INCLUDE_ASM(const s32, "game/code_00196478", func_001991D0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199318);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199460);

INCLUDE_ASM(const s32, "game/code_00196478", func_001994D8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199560);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199638);

u32 func_001997E0(void) {
    return 0;
}

u32 func_001997E8(void) {
    return 1;
}

void func_001997F0(void) {
    if (func_00102A40() != 5) {
        func_00102A18();
    }
    func_003003F0("Camp process halted.\n");
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00199828);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB188);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB18C);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB190);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB198);

INCLUDE_SDATA(const s32, "game/code_00196478", D_003BB1A0);


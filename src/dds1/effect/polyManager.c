#include "common.h"

/* Polygon node with an f32 scale pair and a resource handle at 0xDC.
   (Retail lwc1 at +0xDC belongs to the PolyQuad flavor below, whose
   +0xDC is a float, so the two layouts are distinct node types.) */
typedef struct {
    u8 pad[0xCC]; /* 0x0 */
    f32 unkCC;    /* 0xCC scaled by func_0015DA80/func_0015F2B0/func_0015EBF8 */
    f32 unkD0;    /* 0xD0 scaled by func_0015DA80/func_0015F2B0 */
    u8 padD4[8];  /* 0xD4 */
    u32 unkDC;    /* 0xDC handle released by func_0015D9E0/func_0015B918 */
} PolyNode;

/* Node of four f32s scaled together by func_0015E5A0. */
typedef struct {
    u8 pad[0xC8]; /* 0x0 */
    f32 unkC8;    /* 0xC8 */
    f32 unkCC;    /* 0xCC */
    f32 unkD0;    /* 0xD0 */
    u8 padD4[8];  /* 0xD4 */
    f32 unkDC;    /* 0xDC */
} PolyQuad;

/* Three further node flavors, each destructor releasing its own pair. */
typedef struct {
    u8 pad[0xE0]; /* 0x0 */
    u32 unkE0;    /* 0xE0 */
    u8 padE4[4];  /* 0xE4 */
    void *unkE8;  /* 0xE8 released by func_0015E888 */
} PolyNodeE0;

typedef struct {
    u8 pad[0xF0]; /* 0x0 */
    u32 unkF0;    /* 0xF0 */
    u8 padF4[4];  /* 0xF4 */
    void *unkF8;  /* 0xF8 released by func_0015E0D0 */
} PolyNodeF0;

typedef struct {
    u8 pad[0xF4]; /* 0x0 */
    u32 unkF4;    /* 0xF4 */
    u8 padF8[4];  /* 0xF8 */
    void *unkFC;  /* 0xFC released by func_0015EEC0 */
} PolyNodeF4;

typedef struct {
    s32 unk0;  /* 0x0 reset to 0xFFFFFF0 by func_0015F468 */
    s32 unk4;  /* 0x4 */
    s32 unk8;  /* 0x8 */
    s32 unkC;  /* 0xC */
    s32 unk10; /* 0x10 */
} PolyEntry; /* 0x14 bytes */

typedef struct {
    u8 pad[0x10];      /* 0x0 */
    u32 entryCount;     /* 0x10 */
    u32 unk14;         /* 0x14 */
    u8 pad18[0x50];    /* 0x18 */
    u32 unk68;         /* 0x68 */
    u32 unk6C;         /* 0x6C */
    u8 pad70[0x50];    /* 0x70 */
    u32 unkC0;         /* 0xC0 step subtracted by func_0015E100/func_0015E8B8 */
    u8 padC4[0x18];    /* 0xC4 */
    u32 unkDC;         /* 0xDC divisor read by func_0015EEF0 */
    u8 padE0[4];       /* 0xE0 */
    u32 *unkE4;        /* 0xE4 stepped by func_0015E8B8 */
    u8 padE8[0xC];     /* 0xE8 */
    u32 *unkF4;        /* 0xF4 stepped by func_0015E100 */
    PolyEntry *entries; /* 0xF8 */
} PolyList;

void func_0015B8B8(u32 arg);
void func_0015B918(u32 arg);
void func_0015DAA0(void);
void func_002CFF98(void *arg);
void func_002D0918(void *arg);

void func_0015D9E0(PolyNode *obj) {
    func_0015B8B8(obj->unkDC);
    func_002CFF98(obj);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DA10);

void func_0015DA80(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DAA0);

void func_0015DC48(PolyNode *obj) {
    func_0015DAA0();
    func_0015B918(obj->unkDC);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DC70);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DDC8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DE88);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DFA8);

void func_0015E0D0(PolyNodeF0 *obj) {
    func_0015B8B8(obj->unkF0);
    func_002D0918(obj->unkF8);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E100);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E148);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E238);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E3D8);

void func_0015E5A0(f32 scale, PolyQuad *obj) {
    obj->unkC8 = obj->unkC8 * scale;
    obj->unkDC = obj->unkDC * scale;
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E5D8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E760);

void func_0015E888(PolyNodeE0 *obj) {
    func_0015B8B8(obj->unkE0);
    func_002D0918(obj->unkE8);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E8B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E900);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E9A0);

void func_0015EBF8(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EC08);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015ED90);

void func_0015EEC0(PolyNodeF4 *obj) {
    func_0015B8B8(obj->unkF4);
    func_002D0918(obj->unkFC);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EEF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EF50);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F0C0);

void func_0015F2B0(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F2D0);

void func_0015F468(PolyList *obj) {
    u32 count;
    PolyEntry *entry;
    u32 i;

    count = obj->entryCount;
    i = 0;
    obj->unk14 = 0xFFFFFFF;
    obj->unk6C = 0;
    obj->unk68 = 0;
    entry = obj->entries;
    if (count != 0) {
        do {
            if (entry->unk0 != -0xFFFFFF) {
                entry->unk0 = 0xFFFFFF0;
            }
            i++;
            entry++;
        } while (i < count);
    }
}

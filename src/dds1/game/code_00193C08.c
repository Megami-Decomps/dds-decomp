#include "common.h"

/* Circular doubly-linked list node; prev/next at +0x18/+0x1C. */
typedef struct FntNode {
    u8 unk0[0x18];          /* 0x0 */
    struct FntNode *prev;   /* 0x18 */
    struct FntNode *next;   /* 0x1C */
} FntNode;

/* List header at D_003D68C0: entry count plus sentinel node link. */
typedef struct FntList {
    u8 unk0[0x18];  /* 0x0 */
    s32 count;      /* 0x18 */
    FntNode *head;  /* 0x1C */
} FntList;

/* 0x24-byte table entry in the D_003D6C80 font system (one per index & 0xFF). */
typedef struct FrFontEntry {
    void *unk0;  /* 0x0: buffer released by frFontFreeEntry */
    void *unk4;  /* 0x4: value record (u16 pair read via D_003D6C84 view) */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *unk10;   /* 0x10: flag bytes set by frFontSetEntryFlag */
    void *unk14; /* 0x14 */
    void *unk18; /* 0x18 */
    void *unk1C; /* 0x1C */
    u32 unk20;   /* 0x20 */
} FrFontEntry;

/* Font system at D_003D6C80: 9 entries followed by shared control words. */
typedef struct FrFontSysLocal {
    FrFontEntry entries[9]; /* 0x0 */
    s32 unk144;             /* 0x144 */
    s32 unk148;             /* 0x148 */
    s32 unk14C;             /* 0x14C */
    void *unk150;           /* 0x150: passed to func_001982A0 by frFontReleaseAll */
    void *unk154;           /* 0x154: passed to func_001982A0 by frFontReleaseAll */
    void *unk158;           /* 0x158: passed to func_002D1B90 by frFontReleaseAll */
    void *unk15C;           /* 0x15C: passed to func_002D1B90 by frFontReleaseAll */
    u8 unk160[0x34];        /* 0x160 */
    void *unk194;           /* 0x194: glyph slot */
    void *unk198;           /* 0x198: glyph slot */
} FrFontSysLocal;

extern FntList D_003D68C0;
extern FrFontSysLocal D_003D6C80;
extern u32 D_003565F8[];
extern void func_00194668(void);
extern void *func_00194840(void *arg0);
extern u32 func_001982A0(s32 arg0);
extern void func_00193B70(void);
extern void func_002D0918(void *arg0);
extern void func_002D1B90(void *arg0);

void frFontListInsert(FntNode *node) {
    FntNode *head = D_003D68C0.head;
    FntNode *next = head->next;

    node->prev = head;
    node->next = next;
    D_003D68C0.count += 1;
    head->next = node;
    next->prev = node;
}

void frFontSetEntryFlag(s32 index, s32 value) {
    FrFontEntry *entry = &D_003D6C80.entries[index & 0xFF];

    entry->unk10[0] = 1;
    entry->unk10[1] = value + 1;
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193C70);

s32 frFontBitLength(u32 value) {
    s32 count = 0;

    if (value == 0) {
        return 0;
    }
    do {
        value = value >> 1;
        count += 1;
    } while (value != 0);
    return (count + 0xFF) & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193CD8);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193D70);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00193FD0);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194038);

INCLUDE_ASM(const s32, "game/code_00193C08", func_001940B8);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194190);

INCLUDE_ASM(const s32, "game/code_00193C08", func_00194228);

void frFontReleaseAll(void) {
    func_00194668();
    func_00194840(D_003D6C80.unk194);
    func_00194840(D_003D6C80.unk198);
    func_001982A0(D_003D6C80.unk150);
    func_001982A0(D_003D6C80.unk154);
    func_00193B70();
    func_002D1B90(D_003D6C80.unk158);
    func_002D1B90(D_003D6C80.unk15C);
}

INCLUDE_ASM(const s32, "game/code_00193C08", func_001944A0);

void frFontFreeEntry(s32 arg0) {
    u32 idx = arg0 & 0xFF;
    FrFontEntry *entry;

    if (idx < 2) {
        return;
    }
    D_003565F8[idx] = 0;
    entry = &D_003D6C80.entries[idx];
    if (entry->unk0 != NULL) {
        func_002D0918(entry->unk0);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}

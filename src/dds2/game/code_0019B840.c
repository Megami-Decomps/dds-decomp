#include "common.h"

extern s32 D_00436558;

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

extern Unk6C84Rec D_00452724[];

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

extern FntList D_00452360;

/* 0x24-byte table entry in the D_003D6C80 font system (one per index & 0xFF). */
typedef struct FrFontEntry {
    void *unk0;  /* 0x0: buffer released by func_001945A8 */
    void *unk4;  /* 0x4: value record (u16 pair read via D_003D6C84 view) */
    u32 unk8;    /* 0x8 */
    u32 unkC;    /* 0xC */
    u8 *unk10;   /* 0x10: flag bytes set by func_00193C38 */
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
    void *unk150;           /* 0x150: passed to func_001982A0 by func_00194440 */
    void *unk154;           /* 0x154: passed to func_001982A0 by func_00194440 */
    void *unk158;           /* 0x158: passed to func_002D1B90 by func_00194440 */
    void *unk15C;           /* 0x15C: passed to func_002D1B90 by func_00194440 */
    u8 unk160[0x34];        /* 0x160 */
    void *unk194;           /* 0x194: glyph slot */
    void *unk198;           /* 0x198: glyph slot */
} FrFontSysLocal;

extern FrFontSysLocal D_00452720;

extern void func_0019C2F8(void);

extern void *func_0019C4D0(void *arg0);

extern u32 func_001A02D0(s32 arg0);

extern void func_0019B7A8(void);

extern void func_0032AA40(void *arg0);

extern u32 D_003B2F30[];

extern void func_003297C8(void *arg0);

void func_0019B840(FntNode *arg0) {
    FntNode *head = D_00452360.head;
    FntNode *next = head->next;

    arg0->prev = head;
    arg0->next = next;
    D_00452360.count += 1;
    head->next = arg0;
    next->prev = arg0;
}

u16 func_0019B870(s32 arg0) {
    return D_00452724[arg0].unk0->unk10;
}

u16 func_0019B890(s32 arg0) {
    return D_00452724[arg0].unk0->unk12;
}

void func_0019B8B0(s32 arg0) {
    if (arg0 < 1) {
        arg0 = 0x14;
    }
    D_00436558 = arg0;
}

void func_0019B8C8(s32 arg0, s32 arg1) {
    FrFontEntry *entry = &D_00452720.entries[arg0 & 0xFF];

    entry->unk10[0] = 1;
    entry->unk10[1] = arg1 + 1;
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B900);

s32 func_0019B928(u32 arg0) {
    s32 count = 0;

    if (arg0 == 0) {
        return 0;
    }
    do {
        arg0 = arg0 >> 1;
        count += 1;
    } while (arg0 != 0);
    return (count + 0xFF) & 0xFF;
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B968);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BA00);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BC60);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BCC8);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BD48);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BE20);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BEB8);

void func_0019C0D0(void) {
    func_0019C2F8();
    func_0019C4D0(D_00452720.unk194);
    func_0019C4D0(D_00452720.unk198);
    func_001A02D0(D_00452720.unk150);
    func_001A02D0(D_00452720.unk154);
    func_0019B7A8();
    func_0032AA40(D_00452720.unk158);
    func_0032AA40(D_00452720.unk15C);
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019C130);

void func_0019C238(s32 arg0) {
    u32 idx = arg0 & 0xFF;
    FrFontEntry *entry;

    if (idx < 2) {
        return;
    }
    D_003B2F30[idx] = 0;
    entry = &D_00452720.entries[idx];
    if (entry->unk0 != NULL) {
        func_003297C8(entry->unk0);
        entry->unk1C = NULL;
    }
    entry->unk18 = NULL;
}

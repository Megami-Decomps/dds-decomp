#include "common.h"

void func_00344120(int *param_1, int param_2, u8 *param_3, int param_4);

extern u32 D_004365E8;

/* One 0x14-byte slot per message window; the first field points at its state. */
typedef struct ItfMesSlot {
    void *mes;
    u8 unk4[0x10];
} ItfMesSlot;

extern ItfMesSlot D_0045296C[];

/* Globals behind D_003D6EA0: word at +0x4, bitfield at +0xC. */
typedef struct ItfMesGlobals {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4: read by func_0019B870 */
    u32 unk8; /* 0x8 */
    u16 unkC; /* 0xC: set/cleared by func_0019CC90/func_0019CCA8 */
    u16 unkE; /* 0xE */
} ItfMesGlobals;

extern ItfMesGlobals D_00452940;

/* 3 words zeroed by func_0019D0A0. */
typedef struct ItfMesZero {
    u32 unk0;
    u32 unk4;
    u32 unk8;
} ItfMesZero;

extern ItfMesZero D_003B4770;

/* Sized table indexed by func_0019D268: s16 count + u32 items. */
typedef struct ItfMesTable {
    u8 unk0[0x18]; /* 0x0 */
    s16 unk18;     /* 0x18: count */
    s16 unk1A;     /* 0x1A */
    u32 unk1C[1];  /* 0x1C: items */
} ItfMesTable;

/* 8-byte entry selected by func_0019D1D8/func_0019D1F0. */
typedef struct ItfMesEntry {
    u32 unk0;            /* 0x0: item list read by func_0019D5D0 */
    ItfMesTable *table;  /* 0x4: read by func_0019C920 */
} ItfMesEntry;

/* Record behind ItfMesState.sub; func_0019D240 reads word +0x18. */
typedef struct ItfMesSub {
    u8 unk0[0x18];      /* 0x0 */
    u32 unk18;          /* 0x18 */
    u8 unk1C[4];        /* 0x1C */
    ItfMesEntry unk20[1]; /* 0x20: indexed by func_0019D1D8/func_0019D1F0 */
} ItfMesSub;

typedef struct FrFontGlyph FrFontGlyph;

/* Block at ItfMesState +0x14. */
typedef struct ItfMesBlk14 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u32 unkC;            /* +0xC */
} ItfMesBlk14;

/* Block at ItfMesState +0x24. */
typedef struct ItfMesBlk24 {
    u32 unk0;            /* +0x0 */
    u8 unk4[4];          /* +0x4 */
    ItfMesTable *unk8;   /* +0x8 */
    FrFontGlyph *unkC;   /* +0xC */
    u32 unk10;           /* +0x10 */
} ItfMesBlk24;

/* Block at ItfMesState +0x40. */
typedef struct ItfMesBlk40 {
    u32 unk0;            /* +0x0 */
    u32 unk4;            /* +0x4 */
    FrFontGlyph *unk8;   /* +0x8 */
    u32 unkC;            /* +0xC */
    u16 unk10;           /* +0x10 */
    s16 unk12;           /* +0x12 */
    u16 unk14;           /* +0x14 */
    u16 unk16;           /* +0x16 */
} ItfMesBlk40;

/* Message-window state behind each ItfMesSlot. */
typedef struct ItfMesState {
    u32 flags;          /* 0x0: low half status, high half mask */
    ItfMesSub *sub;     /* 0x4 */
    u8 unk8[0xC];       /* 0x8 */
    ItfMesBlk14 blk14;    /* 0x14: passed to func_0019DDA8 */
    ItfMesBlk24 blk24;    /* 0x24: passed to func_0019DDD0 */
    u8 unk38;             /* 0x38 */
    u8 unk39;           /* 0x39: set by func_0019CB78 */
    u8 unk3A[2];        /* 0x3A */
    s16 unk3C;          /* 0x3C: read by func_0019C548 */
    s16 unk3E;          /* 0x3E: read by func_0019C528 */
    ItfMesBlk40 blk40;  /* 0x40 */
    u8 unk58[0x78];     /* 0x58 */
    u32 tableD0[1];  /* 0xD0: indexed by func_0019C568 (true length unknown) */
    u8 unkD4[0x108]; /* 0xD4 */
    u32 unk1DC;      /* 0x1DC: set by func_0019CB98 */
} ItfMesState;

ItfMesEntry *func_001A51F8(ItfMesState *mes, s32 index);

ItfMesEntry *func_001A5210(ItfMesSub *sub);

u32 func_001A5288(ItfMesTable *table, s32 index);

/* Item chained off a window node (+0x28); recolored by func_0019DA50. */
typedef struct ItfMesItem {
    u8 unk0[0x10];         /* 0x0 */
    u32 word10;            /* 0x10: low byte is the color */
    u8 unk14[2];           /* 0x14 */
    u8 flag16;             /* 0x16: tested by func_0019DB40 */
    u8 unk17[0x11];        /* 0x17 */
    struct ItfMesItem *next; /* 0x28 */
} ItfMesItem;

/* Window chain node walked by func_0019D920/DA50/DB40. */
typedef struct ItfMesNode {
    u8 unk0[4];        /* 0x0 */
    s32 unk4;          /* 0x4: adjusted by func_0019D8E8 */
    s32 unk8;          /* 0x8: adjusted by func_0019D8E8 */
    u8 unkC[8];        /* 0xC */
    s32 unk14;         /* 0x14: set by func_0019D920 */
    u8 unk18[4];       /* 0x18 */
    ItfMesItem *child; /* 0x1C */
    u8 unk20[4];       /* 0x20 */
    struct ItfMesNode *next; /* 0x24 */
} ItfMesNode;

/* Operands of func_0019D8B8: word at +0x8, divisor at +0x12. */
typedef struct ItfMesSpan {
    u8 unk0[8]; /* 0x0 */
    s32 unk8;   /* 0x8 */
    u8 unkC[6]; /* 0xC */
    s16 unk12;  /* 0x12 */
} ItfMesSpan;

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3370);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A33C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3410);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3458);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A34D0);

u32 func_001A3560(void) {
    return 1;
}

u32 func_001A3568(void) {
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3570);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A35F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3658);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A36C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3700);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3780);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A37D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3838);

u32 func_001A38A0(void) {
    return D_00452940.unk4;
}

void itfMesSetFlags(u32 arg0) {
    D_004365E8 = D_004365E8 | arg0;
}

void itfMesClearFlags(u32 arg0) {
    D_004365E8 = D_004365E8 & ~arg0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A38D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A39D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3A18);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3C28);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3CC8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3DA8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A3EE0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4008);

void func_001A4090(s32 arg0, u32 arg1) {
    *(u32 *)((s32)D_0045296C[arg0].mes + 0x4c) = arg1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A40B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4118);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A41A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4218);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A42A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4318);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A43A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4418);

u32 func_001A4488(s32 arg0) {
    return *(u32 *)D_0045296C[arg0].mes;
}

void func_001A44A8(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_0045296C[arg0].mes;
    *puVar1 = (u32)(u16)*puVar1 | (arg1 & 0xffff0000);
}

void func_001A44D8(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_0045296C[arg0].mes;
    *puVar1 = *puVar1 | (arg1 & 0xffff0000);
}

void func_001A4508(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)D_0045296C[arg0].mes;
    *puVar1 = *puVar1 & (~arg1 | 0xffff);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4538);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4558);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4578);

u32 func_001A4598(s32 arg0, s32 arg1) {
    return *(u32 *)(arg1 * 4 + (s32)D_0045296C[arg0].mes + 0xd0);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A45C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4858);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4888);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A48B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4940);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4988);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4A10);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4AD8);

void func_001A4B98(s32 arg0, u8 arg1) {
    *(u8 *)((s32)D_0045296C[arg0].mes + 0x39) = arg1;
}

void func_001A4BB8(s32 arg0, u32 arg1) {
    *(u32 *)((s32)D_0045296C[arg0].mes + 0x1dc) = arg1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4BD8);

void func_001A4CB0(u32 bits) {
    D_00452940.unkC |= bits;
}

void func_001A4CC8(u32 bits) {
    D_00452940.unkC &= ~bits;
}

u16 func_001A4CE8(void) {
    return D_00452940.unkC;
}

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CE0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414CF0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D00);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_00414D10);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A4CF8);

void func_001A50C0(void) {
    D_003B4770.unk0 = 0;
    D_003B4770.unk4 = 0;
    D_003B4770.unk8 = 0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A50D8);

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(u8 *arg0)
{
    u8 *base;
    u8 *fixups;
    s32 size;
    if (*(u8 *)(arg0 + 0x1C) == 0) {
        base = arg0 + 0x20;
        fixups = arg0 + *(s32 *)(arg0 + 0x10);
        size = *(s32 *)(arg0 + 0x14);
        func_00344120((int *)base, (int)base, fixups, size);
        *(u8 *)(arg0 + 0x1C) = 1;
    }
}

u32 itfMesIsMsgData(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if ((*(s32 *)(arg0 + 8) == 0x3047534d) || (*(s32 *)(arg0 + 8) == 0x3147534d)) {
        temp_v0 = 1;
    }
    return temp_v0;
}

ItfMesEntry *func_001A51F8(ItfMesState *mes, s32 index) {
    ItfMesEntry *entries = mes->sub->unk20;

    return &entries[index];
}

ItfMesEntry *func_001A5210(ItfMesSub *sub) {
    ItfMesEntry *entries = (ItfMesEntry *)((u8 *)sub + 0x20);

    return &entries[sub->unk18];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5228);

u32 func_001A5260(s32 arg0) {
    return *(u32 *)(*(s32 *)((s32)D_0045296C[arg0].mes + 4) + 0x18);
}

u32 func_001A5288(ItfMesTable *table, s32 index) {
    s32 count = table->unk18;

    if (index < 0 || index >= count) {
        return 0;
    }
    return table->unk1C[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A52B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5480);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A55B0);

u32 func_001A5600(s32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)func_001A5210(*(u32 *)(arg0 + 4));
    return *(u32 *)((u32)*(u16 *)(arg0 + 0x20) * 4 + *piVar1);
}

s32 itfMesCountZeroBits(s32 arg0, u32 arg1) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = 0;
    while (0 < arg0) {
        temp_v1 = arg1 & 1;
        arg1 = arg1 >> 1;
        arg0 = arg0 - 1;
        if (temp_v1 == 0) {
            temp_v0 = temp_v0 + 1;
        }
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5670);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5760);

ItfMesNode *func_001A5880(ItfMesNode *node) {
    while (node->next != NULL) {
        node = node->next;
    }
    return node;
}

void func_001A58B8(u8 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x20);
    *(u8 *)(arg1 + 0x15) = *arg0 >> 1;
    *(u8 *)(arg1 + 0x12) = *(u8 *)(temp_v0 + 0x15);
    *(u8 *)(arg1 + 0x13) = *(u8 *)(temp_v0 + 0x14);
    *(u8 *)(arg1 + 0x14) = *(u8 *)(temp_v0 + 0x16);
}

s32 func_001A58E8(ItfMesSpan *arg0, ItfMesSpan *arg1) {
    return ((arg1->unk8 - arg0->unk8) >> 3) / arg1->unk12 + 1;
}

void func_001A5918(ItfMesNode *node, s32 arg1, s32 arg2) {
    if (node == NULL) {
        return;
    }
    do {
        node->unk4 += arg1;
        node->unk8 += arg2;
        node = node->next;
    } while (node != NULL);
}

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_001A5950(u8 *arg0, int arg1)
{
    while (arg0 != ((void*)0)) {
        *(int *)(arg0 + 0x14) = arg1;
        arg0 = *(u8 **)(arg0 + 0x24);
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5988);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5A28);

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void func_001A5A80(int param_1,u32 param_2)
{
  int iVar1;
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    for (iVar1 = *(int *)(param_1 + 0x1c); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x28)) {
      *(u32 *)(iVar1 + 0x10) = *(u32 *)(iVar1 + 0x10) & 0xffffff00 | param_2;
    }
  }
  return;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5AD0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_001A5B20);

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_001A5B70(int param_1)
{
  for (; param_1 != 0; param_1 = *(int *)(param_1 + 0x24)) {
    if (*(u8 *)(*(int *)(param_1 + 0x1c) + 0x16) == '\0') {
      func_0019D038(param_1);
    }
  }
  return;
}
INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365E8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F0);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_004365F8);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436608);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436610);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436618);

INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436620);


INCLUDE_SDATA(const s32, "interface/itfMesManager", D_00436628);


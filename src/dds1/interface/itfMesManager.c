#include "common.h"

/* Record behind ItfMesState.sub; func_0019D240 reads word +0x18. */
typedef struct ItfMesSub {
    u8 unk0[0x18]; /* 0x0 */
    u32 unk18;     /* 0x18 */
} ItfMesSub;

/* Message-window state behind each ItfMesSlot. */
typedef struct ItfMesState {
    u32 flags;       /* 0x0: low half status, high half mask */
    ItfMesSub *sub;  /* 0x4 */
    u8 unk8[0x31];   /* 0x8 */
    u8 unk39;        /* 0x39: set by func_0019CB78 */
    u8 unk3A[0x12];  /* 0x3A */
    u32 unk4C;       /* 0x4C: set by func_0019C060 */
    u8 unk50[0x80];  /* 0x50 */
    u32 tableD0[1];  /* 0xD0: indexed by func_0019C568 (true length unknown) */
    u8 unkD4[0x108]; /* 0xD4 */
    u32 unk1DC;      /* 0x1DC: set by func_0019CB98 */
} ItfMesState;

/* One 0x14-byte slot per message window. */
typedef struct ItfMesSlot {
    ItfMesState *mes;
    u8 unk4[0x10]; /* 0x4 */
} ItfMesSlot;

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
    u8 unk0[0x14];     /* 0x0 */
    s32 unk14;         /* 0x14: set by func_0019D920 */
    u8 unk18[4];       /* 0x18 */
    ItfMesItem *child; /* 0x1C */
    u8 unk20[4];       /* 0x20 */
    struct ItfMesNode *next; /* 0x24 */
} ItfMesNode;

/* Relocatable message blob: magic + fixup table + payload. */
typedef struct ItfMesBin {
    u8 unk0[8];     /* 0x0 */
    u32 magic;      /* 0x8: "MSG0"/"MSG1" */
    u8 unkC[4];     /* 0xC */
    s32 fixupOff;   /* 0x10 */
    s32 fixupSize;  /* 0x14 */
    u8 unk18[4];    /* 0x18 */
    u8 relocated;   /* 0x1C */
    u8 unk1D[3];    /* 0x1D */
    u8 data[1];     /* 0x20: relocated base */
} ItfMesBin;

/* Single-use request block for func_0019D5D0: handle at +4, index at +0x20. */
typedef struct ItfMesIndex {
    u8 unk0[4];   /* 0x0 */
    u32 handle;   /* 0x4 */
    u8 unk8[0x18]; /* 0x8 */
    u16 index;    /* 0x20 */
} ItfMesIndex;

/* Row of the shade table read by func_0019D888. */
typedef struct ItfMesShade {
    u8 unk0[0x14]; /* 0x0 */
    u8 unk14;      /* 0x14 */
    u8 unk15;      /* 0x15 */
    u8 unk16;      /* 0x16 */
} ItfMesShade;

typedef struct ItfMesColorSrc {
    u8 val0;             /* 0x0 */
    u8 unk1[0x1F];       /* 0x1 */
    ItfMesShade *shade;  /* 0x20 */
} ItfMesColorSrc;

typedef struct ItfMesColorDst {
    u8 unk0[0x12]; /* 0x0 */
    u8 unk12;      /* 0x12 */
    u8 unk13;      /* 0x13 */
    u8 unk14;      /* 0x14 */
    u8 unk15;      /* 0x15 */
} ItfMesColorDst;

extern ItfMesSlot D_003D6ECC[];

extern u32 func_0019D1F0(u32);

extern u32 D_003BB1E8;

void func_002EB278(int *param_1, int param_2, u8 *param_3, int param_4);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B340);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B390);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B3E0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B428);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B4A0);

u32 func_0019B530(void) {
    return 1;
}

u32 func_0019B538(void) {
    return 1;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B540);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B5C0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B628);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B690);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B6D0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B750);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B7A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B808);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B870);

void itfMesSetFlags(u32 flags) {
    D_003BB1E8 |= flags;
}

void itfMesClearFlags(u32 flags) {
    D_003BB1E8 &= ~flags;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B8A8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019B9E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BBF8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BC98);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BD78);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BEB0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019BFD8);

void func_0019C060(s32 window, u32 value) {
    D_003D6ECC[window].mes->unk4C = value;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C080);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C0E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C178);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C1E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C278);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C2E8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C378);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C3E8);

u32 func_0019C458(s32 window) {
    return D_003D6ECC[window].mes->flags;
}

void func_0019C478(s32 window, u32 value) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags = (u32)(u16)mes->flags | (value & 0xffff0000);
}

void func_0019C4A8(s32 window, u32 value) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags |= value & 0xffff0000;
}

void func_0019C4D8(s32 window, u32 value) {
    ItfMesState *mes = D_003D6ECC[window].mes;

    mes->flags &= ~value | 0xffff;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C508);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C528);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C548);

u32 func_0019C568(s32 window, s32 index) {
    return D_003D6ECC[window].mes->tableD0[index];
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C590);

void func_0019C838(s32 window, u32 arg1, u32 arg2) {
    func_0019D460((u32)D_003D6ECC[window].mes, arg1, arg2, 0);
}

void func_0019C868(s32 window) {
    func_0019D460((u32)D_003D6ECC[window].mes);
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C898);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C920);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C968);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019C9F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CAB8);

void func_0019CB78(s32 window, u8 value) {
    D_003D6ECC[window].mes->unk39 = value;
}

void func_0019CB98(s32 window, u32 value) {
    D_003D6ECC[window].mes->unk1DC = value;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CBB8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CC90);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCA8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCC8);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1480);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A1490);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14A0);

INCLUDE_RODATA(const s32, "interface/itfMesManager", D_003A14B0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019CCD8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D0A0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D0B8);

/* Persona 4 func_00278d50 @ 00278D50 (src/itfMesManager.c), recompiled unchanged */
void itfMesRelocate(ItfMesBin *bin) {
    if (bin->relocated == 0) {
        func_002EB278((int *)bin->data, (int)bin->data,
                      (u8 *)bin + bin->fixupOff, bin->fixupSize);
        bin->relocated = 1;
    }
}

u32 itfMesIsMsgData(ItfMesBin *bin) {
    u32 valid;

    valid = 0;
    if (bin->magic == 0x3047534d || bin->magic == 0x3147534d) { /* "MSG0"/"MSG1" */
        valid = 1;
    }
    return valid;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1D8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D1F0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D208);

u32 func_0019D240(s32 window) {
    return D_003D6ECC[window].mes->sub->unk18;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D268);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D298);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D460);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D580);

u32 func_0019D5D0(ItfMesIndex *req) {
    u32 *table;

    table = *(u32 **)func_0019D1F0(req->handle);
    return table[req->index];
}

s32 itfMesCountZeroBits(s32 bits, u32 value) {
    s32 count;
    u32 bit;

    count = 0;
    while (bits > 0) {
        bit = value & 1;
        value >>= 1;
        bits--;
        if (bit == 0) {
            count++;
        }
    }
    return count;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D640);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D730);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D850);

void func_0019D888(ItfMesColorSrc *src, ItfMesColorDst *dst) {
    ItfMesShade *shade = src->shade;

    dst->unk15 = src->val0 >> 1;
    dst->unk12 = shade->unk15;
    dst->unk13 = shade->unk14;
    dst->unk14 = shade->unk16;
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D8B8);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D8E8);

/* Persona 4 func_0027a340 @ 0027A340 (src/itfMesManager.c), recompiled unchanged */
void func_0019D920(ItfMesNode *node, s32 value) {
    while (node != NULL) {
        node->unk14 = value;
        node = node->next;
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D958);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019D9F8);

/* Persona 4 func_0027a4d0 @ 0027A4D0 (src/itfMesManager.c), recompiled unchanged */
void func_0019DA50(ItfMesNode *node, u32 color) {
    ItfMesItem *item;

    for (; node != NULL; node = node->next) {
        for (item = node->child; item != NULL; item = item->next) {
            item->word10 = item->word10 & 0xffffff00 | color;
        }
    }
}

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019DAA0);

INCLUDE_ASM(const s32, "interface/itfMesManager", func_0019DAF0);

/* Persona 4 func_0027a580 @ 0027A580 (src/itfMesManager.c), recompiled unchanged */
void func_0019DB40(ItfMesNode *node) {
    for (; node != NULL; node = node->next) {
        if (node->child->flag16 == 0) {
            func_00195388(node);
        }
    }
}

#include "common.h"

extern void (*D_003BD2D4)(void);

extern u64 func_002CF530(u64);

extern u32 D_003BD2C8;

extern u32 func_002CD2A8(u16);

extern u32 func_002CE6B8(u16);

extern u32 func_002CD730(u32, u16);

extern s32 D_003BAA00;

/* Operand block used by the script VM helpers near func_002CD730 (layout inferred from field accesses). */
typedef struct ScrVmOperand {
    u8 pad_0x00[0x04]; // 0x00
    u16 h04;           // 0x04
    u8 pad_0x06[0x0E]; // 0x06
    u16 h14;           // 0x14
    u8 pad_0x16[0x3A]; // 0x16
    float f50;         // 0x50
    u8 unk54;          // 0x54
    s8 selectedIndex;  // 0x55: active operand chosen by scrSelectOperandIndex
    u8 pad_0x56[2];    // 0x56
    u32 flags[0x4C];   // 0x58: eight 4-bit flag slots per word
} ScrVmOperand;

extern u8 D_00394680[];

/* 24-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry24B {
    u8 v0;            // 0x00
    u8 pad_0x01[0x17]; // 0x01
} Entry24B; // 0x18

typedef struct Entry24W {
    u32 v0;            // 0x00
    u8 pad_0x04[0x14]; // 0x04
} Entry24W; // 0x18

/* 0xAARRGGBB color split into RGB and alpha fields. */
typedef struct RgbAlpha {
    u8 pad_0x00[0x18]; // 0x00
    u32 rgb18;         // 0x18
    u8 pad_0x1C[0x1C]; // 0x1C
    u32 alpha38;       // 0x38
    u32 x3C;           // 0x3C
    u8 pad_0x40[0x10]; // 0x40
    float f50;         // 0x50
} RgbAlpha; // 0x54

extern s32 CreateSema(void *);

extern Entry24B D_00393220[];

extern Entry24W D_00393234[];

/* 84-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry84W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x50]; // 0x04
} Entry84W; // 0x54

extern void func_002CD630(void *, s32);

extern Entry84W D_00391230[];

/* 28-byte table entries (full layout unknown; stride inferred from index math). */
typedef struct Entry28W {
    u32 v0;             // 0x00
    u8 pad_0x04[0x18]; // 0x04
} Entry28W; // 0x1C

typedef struct Entry28B {
    u8 v0;             // 0x00
    u8 pad_0x01[0x1B]; // 0x01
} Entry28B; // 0x1C

typedef struct Entry28H {
    u16 v0;            // 0x00
    u8 pad_0x02[0x1A]; // 0x02
} Entry28H; // 0x1C

/* Copy source for func_002CF3F8 (layout inferred from field accesses). */
typedef struct CfSrc {
    u8 pad_0x00[0x04]; // 0x00
    u32 x04;           // 0x04
    u8 pad_0x08[0x1C]; // 0x08
    u32 x24;           // 0x24
    u32 x28;           // 0x28
    u8 pad_0x2C[0x10]; // 0x2C
    float f3C;         // 0x3C
} CfSrc; // 0x40

extern Entry28W D_003907B8[];

extern Entry28B D_003907B4[];

extern Entry28H D_003907B6[];

extern Entry28B D_003907B5[];

extern Entry28W D_003907B0[];
extern s32 func_002FE950(const char *, const char *);
extern void func_002FE978(s32, const char *, s32, s32);
extern void func_002FE360(s32);
extern char D_003BD2B8[];
extern char D_003BD2C0[];
extern u32 func_00197C40(s32, s32, u32, u16, u32, u32);
extern void func_001954C8(u32, u32);
extern void func_001958A0(u32, s32, s32);
extern void func_00194920(u32);
extern u32 fileResolvePrimaryBuffer(void);
extern void func_0029CE50(u32);
extern s32 func_002CD548(s32, u16);
extern u16 D_003907BC[];
u32 func_002CD788(ScrVmOperand *);
s8 func_002CD7B8(ScrVmOperand *);
void func_002CC750(s32 left, s32 right) {
    s32 file = func_002FE950("debug.log", D_003BD2B8);
    if (file != 0) {
        func_002FE978(file, D_003BD2C0, left, right);
        func_002FE360(file);
    }
}

void func_002CC7D8(void) {
    memset(D_003BAA00 + 0x2ebb0, 0, 0x3000);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CC808);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CC9C0);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CCB80);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CCC18);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CCCC0);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CCDC8);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CCE60);

void func_002CD0C0(u32 arg0) {
    func_002CCE60(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD0D8);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD240);

u32 func_002CD2A8(u16 i) {
    return D_003907B8[i].v0;
}

void func_002CD2D0(u32 arg0, u16 arg1) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)func_002CD730(arg0, arg1);
    temp_v0 = func_002CD2A8(arg1);
    *puVar1 = temp_v0;
}

u32 func_002CD310(ScrVmOperand *work, s32 increment) {
    u32 *position;
    u32 limit;
    u32 result;
    s32 selectedIndex;
    if (func_002CD7B8(work) == 0) {
        return 0;
    }
    position = (u32 *)func_002CD788(work);
    selectedIndex = work->selectedIndex;
    *position += increment;
    limit = func_002CD2A8(selectedIndex & 0xffff);
    result = *position;
    if (limit < result) {
        *position = limit;
        result = limit;
    }
    return result;
}

void func_002CD398(u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 0xF;
    v >>= 4;
    *a = v;
    *b = lo << 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD3B8);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD428);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD548);

s32 scrCheckStateBits(ScrVmOperand *work) {
    u32 index = 0;
    do {
        u16 id = index;
        index++;
        if ((func_002CE6B8(id) & 1) == 0 &&
            !func_002CD548(work, id)) {
            return 0;
        }
    } while (index < 0x60);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD630);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD6B0);

s8 scrGetSelectedOperandIndex(ScrVmOperand *op) {
    return op->selectedIndex;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD730);

u32 func_002CD768(u32 arg0, u16 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_002CD730(arg0, arg1);
    return *puVar1;
}

u32 func_002CD788(ScrVmOperand *p) {
    return func_002CD730((u32)p, scrGetSelectedOperandIndex(p) & 0xFFFF);
}

s8 func_002CD7B8(ScrVmOperand *op) {
    return op->selectedIndex;
}

s8 scrSelectOperandIndex(ScrVmOperand *p, s32 v) {
    p->selectedIndex = v;
    func_002CD630(p, v & 0xFFFF);
    return p->selectedIndex;
}

u32 func_002CD7F0(u32 arg0, u32 arg1) {
    return arg1;
}

u32 func_002CD7F8(void) {
    return 0;
}

u32 func_002CD800(void) {
    return 1;
}

void scrDecodePackedFlagIndex(s32 unused, u32 v, u32 *a, u32 *b) {
    u32 lo;

    v &= 0xFFFF;
    lo = v & 7;
    v >>= 3;
    *a = v;
    *b = lo << 2;
}

s32 scrSetFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] |= 1U << shift;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD888);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CD8E8);

void scrSetSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] |= 4U << shift;
}

void scrClearSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    work->flags[word] &= ~(4U << shift);
}

void scrClearFlags(ScrVmOperand *work) {
    s32 index;
    for (index = 0; index < 0x260; index++) {
        scrClearSecondaryScriptFlag(work, index);
    }
}

u32 scrGetSecondaryScriptFlag(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    return work->flags[word] & (4U << shift);
}

u32 func_002CDA98(ScrVmOperand *work, u16 index) {
    u32 word, shift;
    u32 mask;
    scrDecodePackedFlagIndex((s32)work, index, &word, &shift);
    mask = work->flags[word];
    if (mask & (2U << shift)) {
        return 2;
    }
    return (mask & (1U << shift)) != 0;
}

s32 func_002CDB00(s32 arg0, s32 arg1) {
    u32 key = arg1 & 0xFFFF;
    u16 *p = (u16 *)(arg0 + 0x22);
    u32 i = 0;

    do {
        if (*p++ == key) {
            return 1;
        }
        i++;
    } while (i < 0x18);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CDB40);

s32 scrFindSlot(u8 *work, u16 key) {
    u32 index;
    u16 *entries = (u16 *)(work + 0x22);
    for (index = 0; index < 24; index++) {
        if (entries[index] == key) {
            return index;
        }
    }
    return -1;
}

u16 func_002CDC80(u8 *work, u32 index) {
    if (index >= 24) {
        return 0;
    }
    return *(u16 *)(work + 0x22 + index * 2);
}

u32 func_002CDCA0(u8 *work) {
    u16 *entries = (u16 *)(work + 0x22);
    u32 count = 0;
    u32 index;
    for (index = 0; index < 24; index++) {
        if (entries[index] != 0) {
            count++;
        }
    }
    return count;
}

u16 func_002CDCD8(s32 arg0, s32 arg1, u16 arg2) {
    u16 temp_v0;
    u16 *puVar2;

    puVar2 = (u16 *)(arg1 * 2 + arg0 + 0x22);
    temp_v0 = *puVar2;
    *puVar2 = arg2;
    return temp_v0;
}

s32 scrRemoveSlot(u8 *work, u16 key) {
    s32 index = scrFindSlot(work, key);
    if (index >= 0) {
        *(u16 *)(work + 0x22 + index * 2) = 0;
        return 1;
    }
    return 0;
}

u8 func_002CDD38(u16 i) {
    return D_003907B4[i].v0;
}

s32 func_002CDD60(u16 i) {
    return D_003907B6[i].v0;
}

u8 func_002CDD88(u16 i) {
    return D_003907B5[i].v0;
}

u32 func_002CDDB0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CDDB8);

u16 func_002CDE20(u16 scriptId, u32 entry) {
    if (entry >= 8) {
        return 0;
    }
    return D_003907BC[scriptId * 14 + entry];
}

u32 func_002CDE60(void) {
    return 0;
}

u32 func_002CDE68(ScrVmOperand *operand, u16 index) {
    u16 value = operand->h14;
    if (value < func_002CDD60(index)) {
        return 0;
    }
    return 1;
}

void func_002CDE98(u32 arg0, u32 arg1, u32 arg2) {
    memset(arg2, 0, 8);
}

u8 func_002CDEB8(s32 arg0, u32 arg1) {
    return (s64)*(s8 *)(arg0 + 0x55) == (arg1 & 0xffff);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CDED0);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CDFE8);

u32 func_002CE170(u32 arg0, u32 arg1, u32 arg2) {
    return func_002CDFE8(arg0, arg1, arg2, 0);
}

u32 func_002CE188(u8 *work) {
    u8 *slots = work + 0xc;
    u32 index;
    for (index = 0; index < 8; index++) {
        if (slots[index] == 0) {
            return index;
        }
    }
    return 8;
}

s32 func_002CE1C8(s32 state, u8 *operand) {
    s32 index = 0;
    s32 count = func_002CE188(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            if (func_002CD548(state, slots[index++]) == 0) {
                return 0;
            }
        } while (index < count);
    }
    return 1;
}

s32 func_002CE248(s32 state, u8 *operand) {
    u32 matched = 0;
    s32 index = 0;
    s32 count = func_002CE188(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            if (func_002CD548(state, slots[index++]) != 0) {
                matched++;
            }
        } while (index < count);
    }
    if (matched < *(u32 *)(operand + 8)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE2E8);

s32 func_002CE380(u8 *operand) {
    s32 index = 0;
    s32 count = func_002CE188(operand);
    if (count > 0) {
        u8 *slots = operand + 0xc;
        do {
            s32 present = 0;
            s32 selected = slots[index];
            s32 offset = 0;
            s32 remaining = 4;
            do {
                u8 *entry = (u8 *)D_003BAA00 + 0xa60 + offset;
                offset += 0x1a4;
                if ((*(u16 *)entry & 1) != 0) {
                    if (func_002CD548((s32)entry, (u16)selected) != 0) {
                        present = 1;
                    }
                }
                remaining--;
            } while (remaining >= 0);
            if (present == 0) {
                return 0;
            }
            index++;
        } while (index < count);
    }
    return 1;
}

u32 func_002CE468(ScrVmOperand *operand, u8 *value) {
    if (operand->h14 < value[4]) {
        return 0;
    }
    return 1;
}

u32 func_002CE480(u8 *operand) {
    if (*(u32 *)(D_003BAA00 + 0x3C) < *(u32 *)(operand + 8)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE498);

void func_002CE698(u32 arg0, u32 arg1, u16 arg2) {
    func_002CE498(arg0, arg1, arg2, 0);
}

u32 func_002CE6B8(u16 i) {
    return D_003907B0[i].v0;
}

u32 func_002CE6E0(u16 i) {
    return D_00391230[i].v0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE710);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE800);

u8 func_002CE8F0(s32 i) {
    return D_00393220[i].v0;
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE910);
u32 func_002CE968(s32 i) {
    return D_00393234[i].v0;
}

void func_002CE988(void) {
    s32 index = 0;
    do {
        index = func_002CE800(index);
        if (index >= 0) {
            u32 name = func_002CE968(index);
            if (name != 0) {
                mdlFlagSet(name);
            }
        }
    } while (index++ >= 0);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CE9E0);

void func_002CEA10(s32 x, s32 y, u32 first, u16 width, u32 second, s32 option) {
    u32 handle = func_00197C40(x, y, first, width, (u32)D_00394680, 0);
    func_001954C8(handle, second);
    func_001958A0(handle, 1, option);
    func_00194920(handle);
}

u8 *func_002CEA80(void) {
    return D_00394680;
}

void func_002CEA90(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(u32 *)(arg0 + 0x4c);
    puVar2 = *(u32 **)(arg0 + 8);
    if (temp_v0 != 0) {
        do {
            temp_v1 = temp_v1 + 1;
            *puVar2 = 0xffffffff;
            puVar2 = puVar2 + 2;
        } while (temp_v1 < temp_v0);
    }
    memset(*(u32 *)(arg0 + 0x10), 0, temp_v0 << 3);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEAE8);

void func_002CEC08(void) {
    func_0029CE50(fileResolvePrimaryBuffer());
}

void func_002CEC28(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CEC40);

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CF248);

float func_002CF390(ScrVmOperand *op) {
    return op->f50;
}

void func_002CF398(ScrVmOperand *op, float value) {
    op->f50 = value;
}

void func_002CF3A0(s32 arg0) {
    func_00296F58(arg0 + 0x14, arg0 + 0x38, 0, 0);
}

void func_002CF3C8(RgbAlpha *p, u32 color) {
    p->rgb18 = color & 0xFFFFFF;
    p->alpha38 = color >> 24;
}

u32 func_002CF3E8(s32 arg0) {
    return *(u32 *)(arg0 + 0x3c);
}

void func_002CF3F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_002CF3F8(RgbAlpha *dst, CfSrc *src) {
    dst->rgb18 = src->x04;
    dst->f50 = src->f3C;
    dst->alpha38 = src->x24;
    dst->x3C = src->x28;
}

void func_002CF420(void) {
    D_003BD2C8 = 1;
}

void func_002CF430(void) {
    D_003BD2C8 = 0;
}

void func_002CF438(void) {
}

s32 sdfCreateSemaphore(u32 initial, u32 option, u32 maximum) {
    struct {
        u32 attr;
        u32 option;
        u32 initial;
        u32 reserved[2];
        u32 maximum;
    } sema;

    sema.initial = initial;
    sema.option = option;
    sema.maximum = maximum;
    return CreateSema(&sema);
}

INCLUDE_ASM(const s32, "game/code_002CC750", func_002CF468);

void func_002CF4E0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_002CF530(arg1);
    func_002CF468(arg0, temp_v0, arg1, arg2);
}

INCLUDE_SDATA(const s32, "game/code_002CC750", D_003BD2B8);

INCLUDE_SDATA(const s32, "game/code_002CC750", D_003BD2C0);

INCLUDE_SDATA(const s32, "game/code_002CC750", D_003BD2C8);


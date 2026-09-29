#include "common.h"
#include "pcp_vu0.h"
/* libvu0 sceVu0UnitMatrix expansion: qmfc2 of vf0 (0,0,0,1), then MMI shuffles */
#define PCP_UNIT_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $5, $vf0\n\tpextuw $4, $0, $5\n\tpextuw $2, $0, $4\n\tpextuw $3, $4, $0\n\t" \
    "sq $2, 0(%0)\n\tsq $3, 0x10(%0)\n\tsq $4, 0x20(%0)\n\tsq $5, 0x30(%0)\n\t.set reorder" \
    : : "r" (dst) : "$2", "$3", "$4", "$5", "memory")
extern void *func_002DA730();
extern void func_002DA420(void *, f32);
extern void func_002DA3D8(void *, u32);
extern void func_002DA3C0(void *, u32);
extern void func_002DA3F0(void *, u32);
#include "eff.h"

typedef struct EffTemplatePacketList {
    u8 pad00[0x20];
    u32 packetCount;
    u8 pad24[0xD4];
    EffectBufferTail *buffer;
    u8 padFC[0x5C];
    f32 recordScale;
} EffTemplatePacketList;

extern EffectConfig D_0034DF54[];

s32 billCreateIndexed(s32 arg0, s32 arg1);

extern s32 D_003D6438[];

extern s32 D_003D6480[];

s32 sdfAllocPacketAligned(s32 arg0);

void sdfInitPacketList(s32 arg0);

s32 func_00151398(s32 arg0, s32 arg1);

void func_00152E40(s32 arg0, s32 arg1);

void func_00153128(s32 arg0, s32 arg1);

void func_00153740(s32 arg0);

void func_001539D0(s32 arg0);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_002CFEB8(s32 size);

s32 func_002D03F8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void func_001565E0(s32 arg0);

void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_0034DF54[index].unk00, 0);
    s32 *resource = *(s32 **)(D_003D6438[index] + 0x30);
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00151FC0(void) {
    return 0xf;
}

s32 func_00151FC8(s32 arg0) {
    return *(s32 *)(*(s32 *)(D_003D6438[arg0] + 0x30));
}

void effCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00151FF8(BillObj *effect, float scale) {
    effect->unk20 = scale;
}

void func_00152000(BillObj *effect, float x, float y) {
    effect->unk10 = x;
    effect->unk14 = y;
}

void func_00152010(BillObj *effect, u32 value) {
    effect->unk24 = value;
}

void effCopyPosition(BillObj *effect, const void *position) {
    if (effect->unk2C == 0) {
        memcpy((void *)((s32)effect->unk30 + 0xc), position, 16);
    }
}

void effBillSetMode(BillObj *effect, s32 mode) {
    s32 count;
    s32 remaining;
    s32 entry;
    mode = (s16)mode;
    switch (effect->unk2C) {
    case 0:
    case 3:
        effect->unk2E = mode;
        break;
    case 1:
        count = effect->entryCount;
        if (count > 0) {
            remaining = count;
            entry = (s32)effect->unk60 + 0xc;
            do {
                s32 node = *(s32 *)entry;
                u32 flags = *(u32 *)(node + 0x10) & ~6;
                *(u32 *)(node + 0x10) = flags;
                if (mode == 2) {
                    *(u32 *)(node + 0x10) = flags | 2;
                } else if (mode == 3) {
                    *(u32 *)(node + 0x10) = flags | 4;
                }
                entry += 0x14;
            } while (--remaining != 0);
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152100);

s32 func_00152170(BillObj *effect) {
    if (effect->unk2C == 0) {
        return *(s32 *)effect->unk30;
    }
    return 0;
}

u16 billGetKind(BillObj *effect) {
    return effect->unk2C;
}

void billSetVariantValue(BillObj *effect, s32 value) {
    s32 v = value & 0xffff;

    switch (effect->unk2C) {
    case 0:
        *(s16 *)((u8 *)effect->unk30 + 4) = v;
        break;
    case 1:
        effect->unk3C = v;
        break;
    }
}

u16 billGetVariantValue(BillObj *effect) {
    switch (effect->unk2C) {
    case 0:
        return *(u16 *)((u8 *)effect->unk30 + 4);
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

void billSetKind1Entry(s32 arg0, s32 arg1) {
    if ((*(u16 *)(arg0 + 0x2c) == 1) && (*(s32 *)(arg0 + 0x58) != arg1)) {
        func_001518D8(arg0, arg1);
    }
}

s32 billGetKindOneEntry(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x58);
    }
    return 0;
}

s32 func_00152260(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 4);
    }
    return 0;
}

void func_00152288(s32 arg0, u32 arg1) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        s32 n = *(s32 *)(arg0 + 0x5c);

        if (n > 0) {
            s32 p = *(s32 *)(arg0 + 0x60);
            s32 i = n;

            do {
                s32 q = *(s32 *)(p + 0xc);
                u32 r = arg1 % *(u32 *)(q + 0xc);
                i -= 1;
                *(s32 *)(p + 8) = 0;
                *(u32 *)(p + 4) = r;
                p += 0x14;
            } while (i != 0);
        }
    }
}

void func_001522E8(s32 arg0, u32 arg1) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        s32 n = *(s32 *)(arg0 + 0x5c);

        if (n > 0) {
            s32 p = *(s32 *)(arg0 + 0x60);
            s32 i = n;

            do {
                s32 q = *(s32 *)(p + 0xc);
                u32 r = arg1 % *(u32 *)(q + 0xc);
                i -= 1;
                *(s32 *)(p + 8) = 1;
                *(u32 *)(p + 4) = r;
                p += 0x14;
            } while (i != 0);
        }
    }
}

s32 billGetFirstEntryFramePeriod(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x60) + 0xc) + 0xc);
    }
    return 0;
}

u16 func_00152370(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(u16 *)(arg0 + 0x50);
    }
    return 0;
}

s32 func_00152390(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x54);
    }
    return 0;
}

void func_001523B0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        *(s32 *)(arg0 + 0x54) |= 0x1000000;
    }
}

void func_001523D8(s32 arg0, float arg1, float arg2) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        s32 tmp = *(s32 *)(arg0 + 0x30);
        *(float *)(tmp + 0x24) = arg1 * 0.5f;
        *(float *)(tmp + 0x28) = arg2 * 0.5f;
    }
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152408);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152560);

u8 *billCreateUnitObject(s32 arg0) {
    u8 *obj = func_002CFEB8(0x88);

    *(s32 *)(obj + 0x80) = billCreateIndexed(1, arg0);
    *(void **)(obj + 0x84) = func_002DA730();
    func_002DA420(*(void **)(obj + 0x84), 1.0f);
    PCP_UNIT_MATRIX(obj + 0x40);
    return obj;
}

u8 *billCloneUnitObject(u8 *src) {
    u8 *obj = func_002CFEB8(0x88);

    *(s32 *)(obj + 0x80) = func_00151E60(*(s32 *)(src + 0x80));
    *(void **)(obj + 0x84) = func_002DA730();
    func_002DA420(*(void **)(obj + 0x84), 1.0f);
    func_002DA3D8(*(void **)(obj + 0x84), 0x80808080);
    func_002DA3C0(*(void **)(obj + 0x84), 0x80808080);
    func_002DA3F0(*(void **)(obj + 0x84), 0x80808080);
    PCP_UNIT_MATRIX(obj + 0x40);
    PCP_UNIT_MATRIX(obj);
    return obj;
}

void effDestroy(u32 arg0) {
    sdfQueueAssetRelease(*(u32 *)(arg0 + 0x84));
    billDispatchByKind(*(u32 *)(arg0 + 0x80));
    func_002CFF98(arg0);
}

void func_00152758(s32 arg0, s128 *arg1) {
    effCopyVector(*(s128 **)(arg0 + 0x80), arg1);
}

void billSetChildScale2(s32 arg0, float arg1) {
    func_00152000(*(s32 *)(arg0 + 0x80), arg1, arg1);
}

void effCopyMatrixToNext(u8 *dst, void *src) {
    __asm__ volatile(
        "lqc2 $vf28, 0x0(%0)\n\t"
        "lqc2 $vf29, 0x10(%0)\n\t"
        "lqc2 $vf30, 0x20(%0)\n\t"
        "lqc2 $vf31, 0x30(%0)"
        : : "r"(src) : "memory");
    dst += 0x40;
    __asm__ volatile(
        ".set noreorder\n\t"
        "sqc2 $vf28, 0x0(%0)\n\t"
        "sqc2 $vf29, 0x10(%0)\n\t"
        "sqc2 $vf30, 0x20(%0)\n\t"
        "sqc2 $vf31, 0x30(%0)\n\t"
        ".set reorder"
        : : "r"(dst) : "memory");
}

void billSetChildValue(s32 arg0, u32 arg1) {
    func_00152010(*(s32 *)(arg0 + 0x80), arg1);
}

void func_001527D8(void *dst, void *src) {
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 $vf28, 0x0(%1)\n\t"
        "lqc2 $vf29, 0x10(%1)\n\t"
        "lqc2 $vf30, 0x20(%1)\n\t"
        "lqc2 $vf31, 0x30(%1)\n\t"
        "sqc2 $vf28, 0x0(%0)\n\t"
        "sqc2 $vf29, 0x10(%0)\n\t"
        "sqc2 $vf30, 0x20(%0)\n\t"
        "sqc2 $vf31, 0x30(%0)\n\t"
        ".set reorder"
        : : "r"(dst), "r"(src) : "memory");
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152800);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001528C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152E40);

void func_001530C0(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00152E40(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153128);

void func_00153618(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfInitPacketList(tmp);
    func_00153128(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

void func_00153680(void) {
    func_002E84A0(&D_003D6480);
}

void func_001536A0(void) {
}

EffectBufferTail *effAllocateBuffer(s32 count) {
    s32 bytes = count * sizeof(EffectBufferRecord);
    s32 allocation = func_002D03F8(bytes + sizeof(EffectBufferTail));
    EffectBufferRecord *record = sdfResourceRetainAddress(allocation);
    EffectBufferTail *tail = (EffectBufferTail *)((u8 *)record + bytes);

    tail->allocation = allocation;
    tail->records = record;
    if (count > 0) {
        s32 remaining = count;
        do {
            remaining--;
            record->unk20 = 0;
            record->unk24 = 0;
            record++;
        } while (remaining != 0);
    }
    return tail;
}

void func_00153728(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153740);

void effDestroyResources(effect)
    s32 effect;

{
    switch (*(u16 *)(effect + 0x30)) {
    case 1:
        func_00159CD8(*(s32 *)(effect + 0x38));
        break;
    case 2:
        func_0015B8B8(*(s32 *)(effect + 0x40));
        break;
    case 3:
        func_0015B8B8(*(s32 *)(effect + 0x44));
        break;
    case 4:
        func_00188480(*(s32 *)(effect + 0x44));
        break;
    }
    billDispatchByKind(*(s32 *)(effect + 0xf4));
    func_00153728(*(s32 *)(effect + 0xf8));
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001539D0);

void func_00153A40(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_00153A90(s32 arg0) {
    s32 obj = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, (void *)arg0, *(s32 *)(arg0 + 0xa0));
    memcpy((void *)(obj + 0x150), (void *)(arg0 + *(s32 *)(arg0 + 0xa0)), tailLen);
    func_00153740(obj);
    func_001539D0(obj);
    return obj;
}

void func_00153B10(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153B38);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153D98);

void func_00154048(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00154090(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 effCloneTemplate(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00154048(copy);
    return copy;
}

void func_00154160(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154188);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154430);

void func_00154698(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_001546E0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_00154730(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00154698(copy);
    return copy;
}

void func_001547B0(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001547D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154FD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155200);

void func_001552A0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 func_001552D8(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00155200(copy);
    return copy;
}

void func_00155358(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155380);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155618);

void func_00155830(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00155878(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

s32 func_001558B8(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00155830(copy);
    return copy;
}

void func_00155938(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155960);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155CC0);

void func_00155F30(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00155F78(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 func_00155FC8(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00155F30(copy);
    return copy;
}

void func_00156048(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156070);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156378);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001565E0);

void func_00156650(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
}

s32 billCloneTemplateSmall(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xA0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xA0)), tailLen);
    func_00153740(copy);
    func_001565E0(copy);
    return copy;
}

void func_00156708(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156730);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156920);

void func_00156B98(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00156BE0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_00156C30(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00156B98(copy);
    return copy;
}

void func_00156CB0(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156CD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156F30);

void effApplyTemplateScaleToRecords(EffTemplatePacketList *effect) {
    s32 i = 0;
    EffectBufferRecord *record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            *(f32 *)((u8 *)record + 0x34) = effect->recordScale;
            i++;
            record++;
        } while ((u32)i < effect->packetCount);
    }
}

void func_00157188(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

s32 func_001571D8(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    effApplyTemplateScaleToRecords(copy);
    return copy;
}

void func_00157258(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157280);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001575D0);

void func_00157970(void) {
    func_00157A58();
}

void func_00157988(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001579B0);

void func_00157A30(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157A58);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157B58);

void func_00157BE8(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00157C30(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_00157C80(s32 source) {
    s32 copy = (s32)func_002CFEB8(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_00153740(copy);
    func_00157BE8(copy);
    return copy;
}

void func_00157D00(u32 arg0) {
    effDestroyResources();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157D28);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001584B8);

void func_00158718(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00158760(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001587B0);

void func_00158848(u32 arg0) {
    func_002D0918(*(u32 *)(arg0 + 0x17c));
    effDestroyResources(arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158880);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158B10);

void func_00158D88(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 i;

    i = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            i = i + 1;
            record = record + 1;
        } while (i < effect->packetCount);
    }
}

void func_00158DD0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158E10);

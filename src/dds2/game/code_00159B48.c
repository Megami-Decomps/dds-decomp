#include "common.h"
#include "pcp_vu0.h"
extern void func_00333288(void *, u32);
extern void func_00333270(void *, u32);
extern void func_003332A0(void *, u32);
/* libvu0 sceVu0UnitMatrix expansion: qmfc2 of vf0 (0,0,0,1), then MMI shuffles */
#define PCP_UNIT_MATRIX(dst) __asm__ volatile ( \
    ".set noreorder\n\tqmfc2.ni $5, $vf0\n\tpextuw $4, $0, $5\n\tpextuw $2, $0, $4\n\tpextuw $3, $4, $0\n\t" \
    "sq $2, 0(%0)\n\tsq $3, 0x10(%0)\n\tsq $4, 0x20(%0)\n\tsq $5, 0x30(%0)\n\t.set reorder" \
    : : "r" (dst) : "$2", "$3", "$4", "$5", "memory")
extern void *func_003335E0();
extern void func_003332D0(void *, f32);
extern void func_001594C8();
extern s32 D_00451F20[];
extern void func_00341348();
#include "eff.h"

typedef struct EffTemplatePacketList {
    u8 pad00[0x20];
    u32 packetCount;
    u8 pad24[0xD4];
    EffectBufferTail *buffer;
} EffTemplatePacketList;

extern s32 D_00451EE0[];

s32 sdfAllocPacketAligned(s32 arg0);

void sdfResetPacketList(s32 arg0);

void func_0015AA30(s32 arg0, s32 arg1);

void func_0015AD18(s32 arg0, s32 arg1);

void func_0015B330(s32 arg0);

void func_0015B5C0(s32 arg0);

extern void *memset(void *s, s32 c, u32 n);

extern void *memcpy(void *dest, const void *src, u32 n);

extern void *func_00328D68(s32 size);

extern EffectConfig D_003AA884[];

s32 billCreateIndexed(s32 arg0, s32 arg1);

s32 func_003292A8(s32 size);

EffectBufferRecord *sdfResourceRetainAddress(s32 allocation);

void func_0015E1D0(s32 arg0);

void effRetainResource(s32 index) {
    s32 *effect = (s32 *)billCreateIndexed(D_003AA884[index].unk00, 0);
    s32 *resource = *(s32 **)(D_00451EE0[index] + 0x30);
    s32 references = resource[2];

    effect[12] = (s32)resource;
    resource[2] = references + 1;
}

u32 func_00159BB0(void) {
    return 0xf;
}

s32 func_00159BB8(s32 arg0) {
    return *(s32 *)(*(s32 *)(D_00451EE0[arg0] + 0x30));
}

void func_00159BD8(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_00159BE8(BillObj *effect, float scale) {
    effect->unk20 = scale;
}

void func_00159BF0(BillObj *effect, float x, float y) {
    effect->unk10 = x;
    effect->unk14 = y;
}

void func_00159C00(BillObj *effect, u32 value) {
    effect->unk24 = value;
}

INCLUDE_ASM(const s32, "game/code_00159B48", effCopyPosition);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159C40);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159CF0);

s32 func_00159D60(BillObj *effect) {
    if (effect->unk2C == 0) {
        return *(s32 *)effect->unk30;
    }
    return 0;
}

u16 func_00159D80(BillObj *effect) {
    return effect->unk2C;
}

void func_00159D88(BillObj *effect, s32 value) {
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

u16 func_00159DC0(BillObj *effect) {
    switch (effect->unk2C) {
    case 0:
        return *(u16 *)((u8 *)effect->unk30 + 4);
    case 1:
        return effect->unk3C;
    default:
        return 0;
    }
}

void func_00159DF0(BillObj *effect, u32 value) {
    if (effect->unk2C == 1 && effect->unk58 != value) {
        func_001594C8(effect, value);
    }
}

s32 func_00159E30(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x58);
    }
    return 0;
}

s32 func_00159E50(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 4);
    }
    return 0;
}

void func_00159E78(s32 arg0, u32 arg1) {
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

void func_00159ED8(s32 arg0, u32 arg1) {
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

s32 func_00159F38(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(*(s32 *)(*(s32 *)(arg0 + 0x60) + 0xc) + 0xc);
    }
    return 0;
}

u16 func_00159F60(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(u16 *)(arg0 + 0x50);
    }
    return 0;
}

s32 func_00159F80(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        return *(s32 *)(arg0 + 0x54);
    }
    return 0;
}

void func_00159FA0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 1) {
        *(s32 *)(arg0 + 0x54) |= 0x1000000;
    }
}

void func_00159FC8(s32 arg0, float arg1, float arg2) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        s32 tmp = *(s32 *)(arg0 + 0x30);
        *(float *)(tmp + 0x24) = arg1 * 0.5f;
        *(float *)(tmp + 0x28) = arg2 * 0.5f;
    }
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159FF8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A150);

u8 *func_0015A1B8(s32 arg0) {
    u8 *obj = func_00328D68(0x88);

    *(s32 *)(obj + 0x80) = billCreateIndexed(1, arg0);
    *(void **)(obj + 0x84) = func_003335E0();
    func_003332D0(*(void **)(obj + 0x84), 1.0f);
    PCP_UNIT_MATRIX(obj + 0x40);
    return obj;
}

u8 *func_0015A240(u8 *src) {
    u8 *obj = func_00328D68(0x88);

    *(s32 *)(obj + 0x80) = func_00159A50(*(s32 *)(src + 0x80));
    *(void **)(obj + 0x84) = func_003335E0();
    func_003332D0(*(void **)(obj + 0x84), 1.0f);
    func_00333288(*(void **)(obj + 0x84), 0x80808080);
    func_00333270(*(void **)(obj + 0x84), 0x80808080);
    func_003332A0(*(void **)(obj + 0x84), 0x80808080);
    PCP_UNIT_MATRIX(obj + 0x40);
    PCP_UNIT_MATRIX(obj);
    return obj;
}

void effDestroy(u32 arg0) {
    func_00333918(*(u32 *)((s32)arg0 + 0x84));
    billDispatchByKind(*(u32 *)((s32)arg0 + 0x80));
    func_00328E48(arg0);
}

void func_0015A348(s32 arg0) {
    func_00159BD8(*(u32 *)(arg0 + 0x80));
}

void func_0015A360(u8 *obj, f32 scale) {
    func_00159BF0(*(BillObj **)(obj + 0x80), scale, scale);
}

void func_0015A380(u8 *dst, void *src) {
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

void func_0015A3B0(u8 *obj, u32 value) {
    func_00159C00(*(BillObj **)(obj + 0x80), value);
}

void func_0015A3C8(void *dst, void *src) {
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

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

void func_0015ACB0(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfResetPacketList(tmp);
    func_0015AA30(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

void func_0015B208(s32 arg0, s32 arg1) {
    s32 tmp = sdfAllocPacketAligned(0x20);

    sdfResetPacketList(tmp);
    func_0015AD18(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

void func_0015B270(void) {
    func_00341348(D_00451F20);
}

void func_0015B290(void) {
}

EffectBufferTail *effAllocateBuffer(s32 count) {
    s32 bytes = count * sizeof(EffectBufferRecord);
    s32 allocation = func_003292A8(bytes + sizeof(EffectBufferTail));
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

void func_0015B318(u32 *arg0) {
    func_003297C8(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B330);

INCLUDE_ASM(const s32, "game/code_00159B48", effDestroyResources);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B5C0);

void func_0015B630(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_0015B680(s32 arg0) {
    s32 obj = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)obj, 0, 0x180);
    memcpy((void *)obj, (void *)arg0, *(s32 *)(arg0 + 0xa0));
    memcpy((void *)(obj + 0x150), (void *)(arg0 + *(s32 *)(arg0 + 0xa0)), tailLen);
    func_0015B330(obj);
    func_0015B5C0(obj);
    return obj;
}

void func_0015B700(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B728);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B988);

void func_0015BC38(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015BC80(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 effCloneTemplate(s32 source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015BC38(copy);
    return copy;
}

void func_0015BD50(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BD78);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C020);

void func_0015C288(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015C2D0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_0015C320(s32 source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015C288(copy);
    return copy;
}

void func_0015C3A0(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C3C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CBC8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CDF0);

void func_0015CE90(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 func_0015CEC8(s32 source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015CDF0(copy);
    return copy;
}

void func_0015CF48(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CF70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D208);

void func_0015D420(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015D468(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

s32 func_0015D4A8(s32 source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015D420(copy);
    return copy;
}

void func_0015D528(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D550);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D8B0);

void func_0015DB20(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015DB68(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

s32 func_0015DBB8(s32 source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015DB20(copy);
    return copy;
}

void func_0015DC38(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DC60);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015DF68);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E1D0);

void func_0015E240(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E278);

void func_0015E2F8(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E320);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E510);

void func_0015E788(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015E7D0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_0015E820(s32 source) {
    s32 copy = (s32)func_00328D68(0x180);
    s32 tailLen = 0x30;

    memset((void *)copy, 0, 0x180);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015E788(copy);
    return copy;
}

void func_0015E8A0(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E8C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EB20);

void func_0015ED30(s32 effect) {
    s32 i = 0;
    s32 node = *(s32 *)(*(s32 *)(effect + 0xf8) + 4);
    if (*(s32 *)(effect + 0x20) != 0) {
        do {
            *(u32 *)(node + 0x20) = 0xf0000001;
            *(float *)(node + 0x34) = *(float *)(effect + 0x158);
            i++;
            node += 0x40;
        } while ((u32)i < *(u32 *)(effect + 0x20));
    }
}

void func_0015ED78(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

s32 func_0015EDC8(s32 source) {
    s32 copy = (s32)func_00328D68(0x170);
    s32 tailLen = 0x20;

    memset((void *)copy, 0, 0x170);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015ED30(copy);
    return copy;
}

void func_0015EE48(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015EE70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F1C0);

void func_0015F560(void) {
    func_0015F648();
}

void func_0015F578(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F5A0);

void func_0015F620(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F648);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F748);

void func_0015F7D8(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_0015F820(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

s32 func_0015F870(s32 source) {
    s32 copy = (s32)func_00328D68(0x190);
    s32 tailLen = 0x40;

    memset((void *)copy, 0, 0x190);
    memcpy((void *)copy, (void *)source, *(s32 *)(source + 0xa0));
    memcpy((void *)(copy + 0x150), (void *)(source + *(s32 *)(source + 0xa0)), tailLen);
    func_0015B330(copy);
    func_0015F7D8(copy);
    return copy;
}

void func_0015F8F0(u32 arg0) {
    effDestroyResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

INCLUDE_ASM(const s32, "game/code_00159B48", func_001600A8);

void func_00160308(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_00160350(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_001603A0);

void func_00160438(u32 arg0) {
    func_003297C8(*(u32 *)((s32)arg0 + 0x17c));
    effDestroyResources(arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160470);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160700);

void func_00160978(EffTemplatePacketList *effect) {
    EffectBufferRecord *record;
    u32 index;

    index = 0;
    record = effect->buffer->records;
    if (effect->packetCount != 0) {
        do {
            record->unk20 = 0xf0000001;
            index = index + 1;
            record = record + 1;
        } while (index < effect->packetCount);
    }
}

void func_001609C0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160A00);

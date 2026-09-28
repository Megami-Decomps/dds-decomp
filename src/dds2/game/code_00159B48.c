#include "common.h"
#include "eff.h"

extern s32 D_00451EE0[];

s32 func_0032CE80(s32 arg0);

void func_0032CEC0(s32 arg0);

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

EffectBufferRecord *func_003298F8(s32 allocation);

void func_0015E1D0(s32 arg0);

void retainEffectResource(s32 index) {
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

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159BD8);

void func_00159BE8(s32 arg0, float arg1) {
    *(float *)(arg0 + 0x20) = arg1;
}

void func_00159BF0(s32 arg0, float arg1, float arg2) {
    *(float *)(arg0 + 0x10) = arg1;
    *(float *)(arg0 + 0x14) = arg2;
}

void func_00159C00(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00159B48", copyEffectPosition);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159C40);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159CF0);

s32 func_00159D60(s32 arg0) {
    if (*(u16 *)(arg0 + 0x2c) == 0) {
        return *(s32 *)(*(s32 *)(arg0 + 0x30));
    }
    return 0;
}

u16 func_00159D80(s32 arg0) {
    return *(u16 *)(arg0 + 0x2c);
}

void func_00159D88(s32 arg0, s32 arg1) {
    s32 v = arg1 & 0xffff;

    switch (*(u16 *)(arg0 + 0x2c)) {
    case 0:
        *(s16 *)(*(s32 *)(arg0 + 0x30) + 4) = v;
        break;
    case 1:
        *(s16 *)(arg0 + 0x3c) = v;
        break;
    }
}

u16 func_00159DC0(s32 arg0) {
    switch (*(u16 *)(arg0 + 0x2c)) {
    case 0:
        return *(u16 *)(*(s32 *)(arg0 + 0x30) + 4);
    case 1:
        return *(u16 *)(arg0 + 0x3c);
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00159DF0);

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

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A1B8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A240);

void effDestroy(u32 arg0) {
    func_00333918(*(u32 *)((s32)arg0 + 0x84));
    billDispatchByKind(*(u32 *)((s32)arg0 + 0x80));
    func_00328E48(arg0);
}

void func_0015A348(s32 arg0) {
    func_00159BD8(*(u32 *)(arg0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A360);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A380);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3C8);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A3F0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015A4B0);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AA30);

void func_0015ACB0(s32 arg0, s32 arg1) {
    s32 tmp = func_0032CE80(0x20);

    func_0032CEC0(tmp);
    func_0015AA30(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015AD18);

void func_0015B208(s32 arg0, s32 arg1) {
    s32 tmp = func_0032CE80(0x20);

    func_0032CEC0(tmp);
    func_0015AD18(tmp, arg1);
    ((void (*)(s32, s32))*(s32 *)(arg0 + 0x10))(arg0, tmp);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B270);

void func_0015B290(void) {
}

EffectBufferTail *allocateEffectBuffer(s32 count) {
    s32 bytes = count * sizeof(EffectBufferRecord);
    s32 allocation = func_003292A8(bytes + sizeof(EffectBufferTail));
    EffectBufferRecord *record = func_003298F8(allocation);
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

INCLUDE_ASM(const s32, "game/code_00159B48", destroyEffectResources);

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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B728);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015B988);

void func_0015BC38(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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

s32 cloneEffectTemplate(s32 source) {
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015BD78);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015C020);

void func_0015C288(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources();
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015CF70);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D208);

void func_0015D420(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D550);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015D8B0);

void func_0015DB20(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources();
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E320);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015E510);

void func_0015E788(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources();
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
    destroyEffectResources();
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F648);

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F748);

void func_0015F7D8(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources();
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_0015F918);

INCLUDE_ASM(const s32, "game/code_00159B48", func_001600A8);

void func_00160308(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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
    destroyEffectResources(arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160470);

INCLUDE_ASM(const s32, "game/code_00159B48", func_00160700);

void func_00160978(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
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

#include "common.h"

extern u32 func_00304938(u64);

extern u64 func_003298F8(u64);

extern u64 func_002C8108(void);

extern u32 func_00305148(u64, u64);

extern s32 func_002DC1D0(u32, u32);

extern u32 func_00331940(u32, u32);

extern u32 func_00328D68(u32);

extern s32 D_00437E74;

extern u32 D_00437E78;

extern u32 D_00437E50;

extern u32 D_00437E08;

extern u64 resolvePrimaryFileBuffer(void);

extern s64 func_001AA308(void);

extern s64 func_001AA318(void);

extern u64 func_002DC2D8(void);

extern s32 *D_00437E30;

extern u32 D_00437E38;

extern u32 D_00437E3C;

extern u32 func_00159A50(u32);

extern u32 func_002DDF48(u32);

extern u32 D_00437E70;

extern u32 D_00437E80;

extern s32 D_00437E84;

extern u32 D_00437E88;

extern u32 func_002DDCA0(u32, u32);

extern u64 func_002F69F0(u32, u32, u32);

extern s32 func_001AA6F8(void);

extern s32 D_004386C4;

extern s32 D_004386B4;

extern u8 D_00439075;

extern s32 D_004386F0;

extern s32 D_004386F4;

extern s32 D_0043875C;

extern s32 D_00438760;

extern s32 D_00438768;

extern u32 D_0043876C;

extern u32 D_00438770;

extern u32 D_00438778;

extern u32 D_004387A8;

extern u32 D_004387AC;

extern u32 D_004387B0;

extern u32 D_00438774;

extern void func_00232B40(void *);

extern void func_002DCAE8(s32 *);

extern void *func_002DDAA8(void *);

/* Reference-counted object header (layout inferred from field accesses). */
typedef struct RefObj {
    u8 pad_0x00[0x14]; // 0x00
    s32 cnt14;         // 0x14
    s32 unk18;         // 0x18
    s32 cnt1C;         // 0x1C
} RefObj; // 0x20

extern u32 D_00437E34;

/* 4x4 float matrix with 128-bit row access for VU0/DMA transfers. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u; // 0x00
} Matrix4; // 0x40

/* Function-pointer tables indexed by object fields (entry size inferred). */
typedef struct FnTbl28 {
    void (*fn)();
    u8 pad_0x04[0x18]; // 0x04
} FnTbl28; // 0x1C

extern FnTbl28 D_003E9964[];

typedef struct FnTbl24 {
    void (*fn)();
    u8 pad_0x04[0x14]; // 0x04
} FnTbl24; // 0x18

extern FnTbl24 D_003E9B90[];

extern u8 *D_003E9CA8[];

extern u32 D_00437E68;

extern FnTbl24 D_003E9D10[];

extern FnTbl28 D_003E9DEC[];

extern u32 D_00437E6C;

extern u32 func_00343ED0(const char *, u32 *, s32);

extern FnTbl28 D_003E9E74[];

extern u64 func_002F3D88();

extern u32 D_00437E7C;

extern FnTbl24 D_003E9F18[];

extern u32 D_004386B0;

extern u32 D_003FFA84[];

extern u8 D_0045C110[];

extern u8 D_00438758;

extern u8 D_00438759;

extern u8 D_0043875A;

extern u32 D_004386C0;

extern FnTbl28 D_003E9950[];

extern FnTbl24 D_003E9B80[];

extern FnTbl28 D_003E9DD8[];

extern FnTbl28 D_003E9E60[];

extern FnTbl24 D_003E9F08[];

extern u8 D_0045C1A0[];

extern u8 D_003FFA40[];

extern u8 D_0045C1E0[];

extern u8 *D_004386CC;

extern u8 D_0045C300[];

extern u8 D_00400150[];

extern u8 D_00400250[];

extern FnTbl24 D_003E9D00[];

extern FnTbl28 D_003EA02C[];

extern void *func_00232198(void *arg0, void *arg1);

extern void *func_00232EE8(void *arg);

extern void *func_00232EF8(void *arg);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC138);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC1D0);

void func_002DC260(s32 arg0) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x80) = 0;
    func_002322E8();
}

void *func_002DC280(void *arg0) {
    void *a;
    void *b;
    void *work;

    a = func_00232EE8(arg0);
    b = func_00232EF8(arg0);
    work = func_00232198(a, b);
    func_002DC138(work);
    return work;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC2D8);

void func_002DC370(void *p) {
    void *q = *(void **)((s32)p + 8);
    if (q != NULL) {
        func_00328E48(q);
    }
    if (*(s32 *)((s32)p + 4) != 0) {
        func_002DC260(*(s32 *)((s32)p + 4));
    }
    func_00328E48(p);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC3C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC4A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC4C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC540);

void func_002DC560(s32 arg0) {
    mdlBroadcastMasked(*(u32 *)(arg0 + 4));
}

void func_002DC578(s32 arg0, float scale) {
    float v[3];
    float t;

    t = *(float *)arg0 * scale;
    v[0] = v[1] = v[2] = t;
    __asm__ volatile ("lqc2 $vf10, 0(%0)" :: "r" (v));
    func_00232B40(*(void **)(arg0 + 4));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC5B0);

u64 func_002DC6E8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_002DC2D8();
    func_002DC5B0(temp_v0);
    temp_v1 = func_001AA308();
    if ((temp_v1 != 0) && (temp_v1 = func_001AA318(), temp_v1 == 0)) {
        func_002DCA88(temp_v0);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC788);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DC808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCA88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCB58);

void func_002DCBF0(void) {
    s32 *node = D_00437E30;
    s32 obj;
    s32 v;

    if (node != NULL) {
        do {
            obj = *node;
            v = *(s32 *)(obj + 0xC);
            if ((v & 2) != 0) {
                *(s32 *)(obj + 0xC) = v | 8;
            }
            node = *(s32 **)((s32)node + 8);
        } while (node != NULL);
    }
}

void func_002DCC30(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_00437E30;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) & 0xffffffef;
    }
}

void func_002DCC68(void) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = D_00437E30;
    while (piVar2 != (s32 *)0x0) {
        temp_v0 = *piVar2;
        piVar2 = (s32 *)piVar2[2];
        *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 0x10;
    }
}

void func_002DCCA0(void) {
    s32 *node = D_00437E30;
    s32 obj;
    s32 *next;
    s32 v;

    if (node == NULL) {
        return;
    }
    do {
        obj = *node;
        next = *(s32 **)((s32)node + 8);
        v = *(s32 *)(obj + 0xC) | 0xA;
        *(s32 *)(obj + 0xC) = v;
        func_002DCAE8(node);
        node = next;
    } while (node != NULL);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DCCE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD038);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD0C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD258);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD3C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DD448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA30);

void func_002DDA50(s32 arg0) {
    mdlBroadcastMasked(*(u32 *)(arg0 + 0xc0));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDA68);

void func_002DDA98(void) {
    D_00437E3C = 0;
    D_00437E38 = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDAA8);

u32 func_002DDCA0(u32 arg0, u32 arg1) {
    void *p;

    p = func_002DDAA8((void *)arg0);
    *(u32 *)((s32)p + 0x18) = arg1;
    return (u32)p;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDCC8);

RefObj *func_002DDD40(RefObj *obj) {
    obj->cnt14++;
    D_00437E34++;
    return obj;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDD60);

s32 func_002DDF08(void *work) {
    u32 count = *(u32 *)((u8 *)work + 4);
    u32 i = 0;
    s32 total = 0;

    if (count != 0) {
        u8 *entries = *(u8 **)((u8 *)work + 0x10);
        do {
            s32 value = *(s32 *)entries;
            entries += 0x10;
            i++;
            total++;
            total += value;
        } while (i < count);
    }
    return total;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DDF48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE090);

u32 func_002DE120(u32 arg0) {
    *(s32 *)((s32)arg0 + 0x1c) = *(s32 *)((s32)arg0 + 0x1c) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE138);

void func_002DE218(s32 arg0, u32 arg1, s32 arg2) {
    func_002DDD60(arg1, *(u32 *)(*(s32 *)(arg2 + 0xc) * 4 + *(s32 *)(arg0 + 0x14)));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE248);

void func_002DE288(void) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002DE248(temp_v0);
}

void func_002DE2A8(u32 arg0) {
    func_001057A8();
    func_00328E48(arg0);
}

void func_002DE2D0(s32 arg0) {
    func_002DE248(arg0 + 4);
}

void func_002DE2E8(u32 *arg0) {
    *arg0 = 0;
}

void func_002DE2F0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        kwlnFadeSetupFrames(arg0[1], arg0[2]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE338);

void func_002DE408(void) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002DE338(temp_v0);
}

void func_002DE428(void) {
    func_00328E48();
}

void func_002DE440(s32 arg0) {
    func_002DE338(arg0 + 0x18);
}

void func_002DE458(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DE460);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEB10);

void func_002DEB20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DEB28(s32 arg0) {
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

void func_002DEB80(void) {
    func_00316528();
}

void func_002DEB98(void) {
    func_00316648();
}

void func_002DEBB0(void) {
    func_00316668();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEBC8);

void func_002DEBE0(s32 *p) {
    func_002DEB28((s32)p);
    *p = 0;
}

void func_002DEC08(void) {
    if ((D_00437E08 & 2) == 0) {
        func_00316680();
        return;
    }
}

void func_002DEC38(void) {
    func_00316C88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEC50);

void func_002DEC78(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEC80);

void func_002DECD0(void) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002DEC80(temp_v0);
}

void func_002DECF0(u32 arg0) {
    func_0010A298();
    func_00328E48(arg0);
}

void func_002DED18(s32 arg0) {
    func_002DEC80(arg0 + 4);
}

void func_002DED30(u32 *arg0) {
    *arg0 = 0;
}

void func_002DED38(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    if (temp_v0 == 0) {
        func_00109D50(arg0[1], (s16)arg0[2], arg0[3], arg0[4]);
        temp_v0 = *arg0;
    }
    *arg0 = temp_v0 + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DED88);

void func_002DEF00(s32 arg0) {
    func_0018E850(arg0 + 0xc0);
}

void func_002DEF18(void) {
    func_0018E8F0();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DEF30);

void func_002DF138(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF148(s32 arg0) {
    func_0018EBC8(arg0 + 0xc0);
}

void func_002DF160(void) {
    func_0018ECB8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF178);

void func_002DF348(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

void func_002DF358(s32 arg0) {
    func_0018F098(arg0 + 0xc0);
}

void func_002DF370(void) {
    func_0018F1B8();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF388);

void func_002DF558(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x2c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF568);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF6C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DF978);

void func_002DFA80(s32 arg0) {
    func_0018FBF8(arg0 + 0xc0);
}

void func_002DFA98(void) {
    func_0018FC88();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFAB0);

void func_002DFC78(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x24) + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFC88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFDB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFE70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFED8);

void func_002DFF78(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFF80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002DFFE0);

void func_002DFFF0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

void func_002DFFF8(Matrix4 *mat, float value) {
    mat->u.m[1][2] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0000);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0130);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E01E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0250);

void func_002E02F0(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E02F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0358);

void func_002E0368(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0400);

u32 func_002E0460(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0478);

void func_002E0528(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 100);
    if (temp_v0 != 0) {
        billDispatchByKind(temp_v0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0560);

void func_002E0618(s32 arg0, s32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 100) != 0) {
        billDispatchByKind(*(s32 *)(arg0 + 100));
    }
    temp_v0 = func_00159A50(*(u32 *)(arg1 + 100));
    *(u32 *)(arg0 + 100) = temp_v0;
}

void func_002E0668(s32 arg0) {
    func_00159ED8(*(u32 *)(arg0 + 100), 0);
    *(u32 *)(arg0 + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0698);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E07A0);

void func_002E07B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E07C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E07C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0840);

void func_002E08D0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E0F30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1098);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1110);

void func_002E11A0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E11D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1908);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1A70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1AE8);

void func_002E1B80(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E1BB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E22C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2430);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E24A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2520);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E25B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2610);

void func_002E2670(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E26A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2C48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2E28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2F18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E2F78);

void func_002E2FD8(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3008);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E35F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3758);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E37D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3830);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E38C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E3920);

void func_002E3980(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E39B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4070);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E41D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4250);

void func_002E42F0(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4320);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4AC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4C30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4CA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4D00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4DF0);

void func_002E4E50(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E4E80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5540);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E56A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5800);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5860);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E58A8);

void func_002E5978(u8 *work) {
    D_003E9950[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E59C0);

void func_002E5A20(s32 arg0) {
    D_003E9964[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E5A58(u32 arg0) {
    func_002E59C0();
    func_002E5A20(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5A80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5A90);

void func_002E5AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E5AB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BC10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5AB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5C50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5D28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5DE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E5E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6390);

u32 func_002E63F0(s32 arg0) {
    return (&D_00437E50)[arg0];
}

void func_002E6408(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6418);

void func_002E64C0(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E64F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6758);

void func_002E68C8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x30) + 4);
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 8) + 4) = 0;
    func_002E7488(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E68F0);

void func_002E6B30(s32 arg0) {
    func_002E5D28(*(u32 *)(arg0 + 8));
    func_002E7420(*(u32 *)(arg0 + 4));
    func_003297C8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6B68);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6CB8);

void func_002E6E60(s32 arg0) {
    *(u32 *)(**(s32 **)(arg0 + 0x30) + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6E70);

void func_002E6F18(u32 arg0) {
    func_002E7698(*(u32 *)arg0);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E6F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7190);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7300);

void func_002E73F0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002E7300(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7420);

void func_002E7468(s32 arg0) {
    func_002E7300(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void func_002E7488(u8 *work) {
    D_003E9B80[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E74D0);

void func_002E7530(s32 arg0) {
    D_003E9B90[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002E7568(u32 arg0) {
    func_002E74D0();
    func_002E7530(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7590);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E75A0);

void func_002E75B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002E75C0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E75C8);

void func_002E7698(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E76C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7A88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7AE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7B18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7B90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7C00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7CB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7D08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7E70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7EE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E7F60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E81B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E82F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8350);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E83C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E84C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E85C0);

void func_002E86B8(s32 arg0, u32 arg1) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x48) != 0) {
        func_002DE090(*(s32 *)(arg0 + 0x48));
    }
    temp_v0 = func_002DDF48(arg1);
    *(u32 *)(arg0 + 0x48) = temp_v0;
}

void func_002E8708(s32 arg0) {
    if (*(s32 *)(arg0 + 0x4c) != 0) {
        clearFileRecordReferences(*(s32 *)(arg0 + 0x4c));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8738);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E8770);

void func_002E9668(u32 arg0) {
    func_002E8738();
    func_002E8770(arg0);
}

void func_002E9690(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x4c));
}

void func_002E96A8(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x4c));
}

void func_002E96C0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E96C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E96E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9840);

void func_002E9A68(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x3c));
    func_003297C8(*(u32 *)(arg0 + 0x48));
}

void func_002E9A98(s32 arg0) {
    *(u32 *)(arg0 + 0x24) = 0;
    *(u32 *)(arg0 + 0x28) = 3;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9AA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9B20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002E9E98);

void func_002EA0E8(u32 index) {
    u8 *entry = D_003E9CA8[index];
    ((void (*)(u8 *, u32))*(void **)(entry + 0x10))(entry, D_00437E68);
    D_00437E68 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA120);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA3B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA410);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA530);

void func_002EA5D0(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EA5D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAB78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAB88);

void func_002EABA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EABA8(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EABB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAC38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EAFE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB058);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB728);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB908);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EB968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBB88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBBF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBC58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBE28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EBF40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC300);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EC370);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECD10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECEF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ECF78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED360);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002ED3D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDB10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDCF0);

void func_002EDDE0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002EDCF0(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDE10);

void func_002EDE58(s32 arg0) {
    func_002EDCF0(*(u16 *)(arg0 + 0x2c), *(u32 *)(arg0 + 0x34));
}

void resetEffectDispatchCounter(u8 *work) {
    D_003E9D00[*(s32 *)(work + 0x2C)].fn(work);
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDEC0);

void func_002EDF20(s32 arg0) {
    D_003E9D10[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002EDF58(u32 arg0) {
    func_002EDEC0();
    func_002EDF20(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDF80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDF90);

void func_002EDFA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002EDFB0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EDFB8);

void func_002EE0A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE0D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE348);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE3C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE400);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE4A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE508);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE5A8);

void func_002EE608(s32 arg0) {
    if (*(s32 *)(arg0 + 0x34) != 0) {
        clearFileRecordReferences(*(s32 *)(arg0 + 0x34));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE638);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EE670);

void func_002EED48(u32 arg0) {
    func_002EE638();
    func_002EE670(arg0);
}

void func_002EED70(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x34));
}

void func_002EED88(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x34));
}

void func_002EEDA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEDA8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEDC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EEE88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF0D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF130);

void func_002EF190(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF1C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EF8C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFA28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFA78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFAE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFD30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFD90);

void func_002EFDF0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002EFE20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0530);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0698);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F06E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F09A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A08);

void func_002F0A68(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F17B8(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F0A98);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F10C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1228);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F12E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1380);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F13E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1428);

void func_002F14B8(u8 *work) {
    D_003E9DD8[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1500);

void func_002F1560(s32 arg0) {
    D_003E9DEC[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F1598(u32 arg0) {
    func_002F1500();
    func_002F1560(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15D0);

void func_002F15E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F15F0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F15F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1740);

void func_002F17B8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x18) == 0) {
        D_00437E74 = D_00437E74 - 1;
        if (D_00437E74 == 0) {
            func_002DDCC8(D_00437E78);
            D_00437E78 = 0;
        }
    }
    else {
        func_002DDCC8(*(s32 *)(arg0 + 0x18));
    }
    func_00333918(*(u32 *)(arg0 + 0x2c));
    func_003297C8(*(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1820);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1888);

void loadWindEffectTexture(void) {
    D_00437E6C = func_00343ED0("/effect/wind00.tmx", &D_00437E70, 0);
}

u32 func_002F1CC8(void) {
    return D_00437E70;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1CD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1D20);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1F70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F1FC0);

void func_002F2020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2050);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2760);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F28C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2950);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F29D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2A30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2A70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F2AE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3100);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3168);

void func_002F31D0(s32 arg0) {
    *(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x30) + 4) + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F31E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3258);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F36A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F36F0);

void func_002F3750(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x30);
    func_002F3F08(*(u32 *)(temp_v0 + 4));
    func_003297C8(*(u32 *)(temp_v0 + 8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3780);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F37F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3988);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3A40);

void func_002F3AD0(s32 arg0) {
    u64 temp_v0;

    temp_v0 = resolvePrimaryFileBuffer();
    func_002F3A40(*(u16 *)(arg0 + 0xc), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3B00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3B48);

void func_002F3BD8(u8 *work) {
    D_003E9E60[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3C20);

void func_002F3C80(s32 arg0) {
    D_003E9E74[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F3CB8(u32 arg0) {
    func_002F3C20();
    func_002F3C80(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3CE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3CF0);

void func_002F3D08(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F3D10(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

u32 func_002F3D18(void) {
    if (D_00437E84 == 0) {
        D_00437E88 = func_002DDCA0(D_00437E80, 0x200);
    }
    D_00437E84 = D_00437E84 + 1;
    return D_00437E88;
}

void func_002F3D58(void) {
    D_00437E84 = D_00437E84 - 1;
    if (D_00437E84 == 0) {
        func_002DDCC8(D_00437E88);
        D_00437E88 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3D88);

u64 func_002F3ED8(void) {
    u64 temp_v0;

    temp_v0 = func_002F3D88();
    func_002F3D18();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F08);

void func_002F3F40(u8 *work) {
    func_002F3D88(*(u32 *)work, *(u32 *)(work + 0x10));
    D_00437E84++;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F3F70);

void loadScalyEffectTexture(void) {
    D_00437E7C = func_00343ED0("/effect/scaly00.tmx", &D_00437E80, 0);
}

u32 func_002F45A0(void) {
    return D_00437E80;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F45A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F46D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F48E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F4960);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5168);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5358);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5488);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F54D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5520);

void func_002F55F0(u8 *work) {
    D_003E9F08[*(s32 *)(work + 0x2c)].fn();
    *(u32 *)(work + 0x28) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5638);

void func_002F5698(s32 arg0) {
    D_003E9F18[*(s32 *)(arg0 + 0x2C)].fn((void *)arg0);
}

void func_002F56D0(u32 arg0) {
    func_002F5638();
    func_002F5698(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F56F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5708);

void func_002F5720(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F5728(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5730);

void func_002F5820(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x18));
    func_003297C8(*(u32 *)(arg0 + 0x1c));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5850);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5AC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5BC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F5EF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6000);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6450);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F64D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F66F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F67D8);

void resetEffectObjectSlots(u8 *work) {
    u32 *objects = (u32 *)(*(u32 *)(work + 0x30) + 0x24);
    u32 i;
    for (i = 0; i < 5; i++) {
        u32 object = objects[i];
        if (object != 0) {
            *(u32 *)object = 0;
            *(u32 *)(object + 0x0C) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F69F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6A80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6BE8);

u64 func_002F6C30(s32 arg0) {
    u64 temp_v0;

    temp_v0 = func_002F69F0(*(u32 *)(arg0 + 0x38), **(u32 **)(arg0 + 0x30),
                                                (*(u32 **)(arg0 + 0x30))[1]);
    func_002F6A80(temp_v0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6C70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F6D00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7128);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7270);

void func_002F72D8(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        func_00200930(temp_v0 + 0x50, temp_v0 + 0x60, 0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7320);

INCLUDE_ASM(const s32, "game/code_002DC138", resetEffectSlots);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7580);

INCLUDE_ASM(const s32, "game/code_002DC138", setDormantEffectSlot);

INCLUDE_ASM(const s32, "game/code_002DC138", setActiveEffectSlot);

void func_002F7798(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    if ((*(u32 *)(temp_v0 + 0x218) & 0x6000000) == 0x6000000) {
        resetEffectSlots();
        return;
    }
}

void func_002F77D0(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    u32 *puVar3;
    u8 *puVar4;
    u32 temp_v2;

    temp_v2 = 0;
    func_001AA6F8();
    temp_v0 = *(s32 *)(arg0 + 0x28);
    puVar4 = (u8 *)(*(s32 *)(arg0 + 0x38) + 0x18);
    puVar3 = (u32 *)(*(s32 *)(arg0 + 0x38) + 0x10);
    do {
        temp_v1 = puVar3[-4];
        if (temp_v1 < *puVar3) {
            return;
        }
        if (temp_v0 == 0) {
            setActiveEffectSlot(temp_v2, *puVar4, puVar3[-2]);
            temp_v1 = puVar3[-4];
        }
        if ((temp_v1 != 0) && (temp_v0 == temp_v1 - *puVar3)) {
            setDormantEffectSlot(temp_v2, *puVar4, *puVar3);
        }
        temp_v2 = temp_v2 + 1;
        puVar4 = puVar4 + 1;
        puVar3 = puVar3 + 1;
    } while (temp_v2 < 2);
}

void func_002F7890(void) {
    func_002005B0(0xc);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F78A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7928);

void func_002F79E0(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7A00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7A48);

void func_002F7A90(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002EDE10(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7AC8);

void func_002F7C40(s32 arg0) {
    func_002EDF20(**(u32 **)(arg0 + 0x30));
}

void func_002F7C60(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7C80);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7CF0);

void func_002F7D38(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002E7D08(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7D70);

void func_002F7E88(s32 arg0) {
    func_002E8770(**(u32 **)(arg0 + 0x30));
}

void func_002F7EA8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00328D68(4);
    *puVar1 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F7F58);

void func_002F8008(u32 arg0) {
    if (*(s32 *)arg0 != 0) {
        func_002DC260(*(s32 *)arg0);
    }
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8040);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F81A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F81D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8270);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8328);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8378);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F84F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8590);

void func_002F85D8(void) {
    func_002F8590();
}

void func_002F85F0(s32 arg0) {
    func_002F8590(*(u32 *)(arg0 + 0x38));
}

void func_002F8608(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 4);
    if (temp_v0 != 0) {
        func_00333918(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002F8640(void) {
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8648);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8C38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8CF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F8FB8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9118);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9180);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F91D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9608);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F96D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9720);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9780);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9860);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F98C0);

void dispatchEnabledEffectCallback(u8 *work) {
    if (func_001AA308() != 0) {
        void (*callback)(void *) = D_003EA02C[*(s32 *)(work + 0x2C)].fn;
        if (callback != NULL) {
            callback(work);
        }
    }
}

void func_002F9988(u32 arg0) {
    func_002F98C0();
    dispatchEnabledEffectCallback(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99C0);

void func_002F99D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002F99E0(Matrix4 *mat, float value) {
    mat->u.m[2][0] = value;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F99E8);

INCLUDE_ASM(const s32, "game/code_002DC138", releaseEffectParticleResources);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9C38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9D58);

void func_002F9DE8(s32 arg0) {
    *(u32 *)(arg0 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002F9DF0);

void func_002FA388(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA390);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA4D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA550);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA5F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA778);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FA978);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAAE0);

void func_002FABA8(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    u32 temp_v2;

    if (*(s32 *)(arg0 + 0x60) != 0) {
        func_002DC260(*(s32 *)(arg0 + 0x60));
        *(u32 *)(arg0 + 0x60) = 0;
    }
    if (*(s32 *)(arg0 + 100) != 0) {
        func_00330978(*(s32 *)(arg0 + 100), 1, 1);
        *(u32 *)(arg0 + 100) = 0;
    }
    temp_v1 = func_002DC1D0(arg1, arg2);
    temp_v0 = *(s32 *)(temp_v1 + 0xc);
    *(s32 *)(arg0 + 0x60) = temp_v1;
    temp_v2 = func_00331940(*(u32 *)(temp_v0 + 0x14), *(u32 *)(temp_v0 + 0x18));
    *(u32 *)(arg0 + 100) = temp_v2;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAC38);

void func_002FAD30(s32 arg0) {
    if (*(s32 *)(arg0 + 0x70) != 0) {
        clearFileRecordReferences(*(s32 *)(arg0 + 0x70));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FAD98);

void func_002FB400(u32 arg0) {
    func_002FAD60();
    func_002FAD98(arg0);
}

void func_002FB428(s32 arg0) {
    func_002DC0C0(*(u32 *)(arg0 + 0x70));
}

void func_002FB440(s32 arg0) {
    func_002DC0F0(*(u32 *)(arg0 + 0x70));
}

void func_002FB458(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 4) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB460);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB480);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB5C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB790);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB840);

void func_002FB948(s32 arg0, s32 arg1) {
    *(u32 *)(arg0 + 0x478) = *(u32 *)(arg1 + 0x478);
    *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) = *(s16 *)(*(s32 *)(arg1 + 0x478) + 0x10) + 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB968);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FB998);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0B8);

void func_002FC0D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC0D8);

void initializeBattleEffectWork(void) {
    D_004386CC = D_003FFA40;
    D_0045C1A0[0] = 0;
    D_004386B0 = 0;
    D_00439075 = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C1E0) : "memory");
}

s32 updateBattleEffectWork(void) {
    if (D_004386CC != NULL) {
        if ((func_002FCA58(0) & 1) == 0) {
            func_002FC160();
            return 0;
        }
    }
    return 1;
}

void func_002FC158(void) {
}

void func_002FC160(void) {
    func_00300048();
    func_00303C50();
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC180);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC198);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC240);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC2B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC340);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC500);

void func_002FC5C8(u32 arg0) {
    if (D_004386B4 == 0) {
        func_002FC2B0();
        func_002D3710(arg0);
        return;
    }
}

void func_002FC610(u32 arg0) {
    if (D_004386B4 != 0) {
        func_002D4548(D_004386B4);
        D_004386B4 = 0;
    }
    if (D_004386C4 != 0) {
        fileJobDestroy(D_004386C4);
        D_004386C4 = 0;
    }
    fileJobDestroy(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC668);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC718);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC808);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC838);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC8F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FC9B8);

u32 func_002FCA40(void) {
    return D_003FFA84[0] + D_004386B0;
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BED0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BEF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF10);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF20);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF30);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF40);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF50);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF60);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BF90);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFA0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFC0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFD0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFE0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042BFF0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C000);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C010);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C020);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C040);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C050);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C060);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C070);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C080);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C090);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C0E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C100);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C120);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C140);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C160);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C200);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C220);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C240);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C260);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C280);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C2E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C300);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C320);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C340);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C360);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C380);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C3E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C400);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C420);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C440);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C460);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C480);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C4E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C500);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C520);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C548);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C568);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C588);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C5E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C608);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C628);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C648);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C668);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C688);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C6E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C708);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C728);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C748);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C768);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C788);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C7E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C808);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C828);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C848);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C868);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C888);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C8E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C908);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C928);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C948);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C968);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C988);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9A8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042C9E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CA88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAB0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CAD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB00);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CB88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CBE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CC88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CCE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CD98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDD8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CDF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE68);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE78);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE88);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CE98);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEA8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEB8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEC8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CED8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEE8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CEF8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF08);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF18);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF28);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF38);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF48);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF58);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF70);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042CF90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FCA58);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD748);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FD900);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDC90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDD18);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDD70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FDDD8);

u32 func_002FE208(void) {
    D_00439075 = 1;
    func_002FE230(0);
    return 0;
}

u32 func_002FE230(void) {
    if (*(s32 *)(D_004386F0 + 0x34) != 0) {
        D_004386F4 = *(s32 *)(D_004386F0 + 0x34);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE250);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE400);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE5B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE948);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE970);

INCLUDE_ASM(const s32, "game/code_002DC138", createPolyTrackTask);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE9C0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FE9E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA38);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA60);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEA88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEAB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEAD8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB00);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEB78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEBF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEC80);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D030);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D048);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FEDD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF080);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF0B8);

void func_002FF338(void) {
    D_00438758 = D_0045C110[0x88];
    D_00438759 = D_0045C110[0x89];
    D_0043875A = D_0045C110[0x8A];
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF360);

s32 func_002FF870(void) {
    D_0045C110[0x88] = 8;
    D_0045C110[0x89] = 0;
    D_0045C110[0x8a] = 0;
    __asm__ volatile(".set noreorder\n\tsqc2 $vf0, 0(%0)\n\t.set reorder" :: "r"(D_0045C300) : "memory");
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FF8A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFBA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFDA0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_002FFE48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300048);

void func_003000B8(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300100);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003001C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300238);

void func_003002A8(void) {
    if (D_00438768 != 0) {
        func_0020E368(D_00438768);
        D_00438768 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", queueEffectResource);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300328);

void func_003003E0(void) {
    if (D_00438760 != 0) {
        func_0020DF68(D_00438760);
        D_00438760 = 0;
    }
    if (D_0043875C != 0) {
        func_0020D8F0(D_0043875C);
        D_0043875C = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300428);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300548);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00300578);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003012B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301318);

void func_00301380(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0xb8));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003013A0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003013C8);

void func_00301428(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301448(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x38));
}

void func_00301468(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301488);

void func_003014E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_00301508(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301528);

void func_00301590(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015B0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

void func_003015D8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003015F8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x3c, temp_v0 + 0x60, *(u32 *)(temp_v0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301620);

void func_00301688(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x70));
}

void func_003016A8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0, temp_v0 + 0x24, *(u32 *)(temp_v0 + 0x34));
}

void func_003016C8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0xc);
    func_00300578(temp_v0 + 0x50, temp_v0 + 0x74, *(u32 *)(temp_v0 + 0x84));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003016F0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301750);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003017B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00301820);

void resetBattleEffectWork(void) {
    u8 *first = D_00400150;
    u8 *second = D_00400250;

    *(u32 *)(first + 0x10) |= 0x10;
    *(u32 *)(first + 0x0c) = 0;
    *(u32 *)(second + 0x10) |= 0x10;
    *(u32 *)(second + 0x0c) = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003018C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003021A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003021D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302208);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302238);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302268);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302298);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003022C8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003022F8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302328);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302358);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302388);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003023B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003023E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302418);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302448);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302478);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003024A8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003024D8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302508);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302538);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302568);

void func_00302598(void) {
    D_00438774 = 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003025A8);

void func_00302AD0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x3c, *(u32 *)(*(s32 *)(arg0 + 0xc) + 0x4c));
}

void func_00302AF0(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x3c) + 1);
}

void func_00302B18(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B40(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x70, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x8c) + 1);
}

void func_00302B68(s32 arg0) {
    func_003025A8(*(s32 *)(arg0 + 0xc) + 0x60, *(s32 *)(*(s32 *)(arg0 + 0xc) + 0x74) + 1);
}

void func_00302B90(s32 arg0) {
    func_003025A8(*(u32 **)(arg0 + 0xc) + 4, **(u32 **)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302BB0);

void func_00302C78(void) {
    func_00302BB0(0x4b);
}

void func_00302C90(void) {
    func_00302BB0(0x4b);
}

void func_00302CA8(void) {
    func_00302BB0(0xb);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302CC0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302D70);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302D90);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302DF0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E10);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E50);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302E70);

u32 func_00302E90(void) {
    u32 zero = 0;
    func_002D39C8(D_004386C0, &zero, 4, 4);
    return 0x400002;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302EC8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302EE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00302F48);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303130);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303478);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0B8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0C8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0D8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0E8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D0F8);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D108);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D118);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D128);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D140);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303660);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303A30);

void func_00303C50(void) {
    D_0043876C = 0;
    D_00438774 = 1;
    D_00438770 = 0;
    D_00438778 = 0;
    D_004387A8 = 0;
    D_004387AC = 0;
    D_004387B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303C78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303D00);

void func_00303D58(void) {
    func_00328E48();
}

u32 func_00303D70(u32 *arg0) {
    return *arg0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303D78);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303E30);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00303E88);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304030);

void func_003040B8(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_002C8108();
    temp_v1 = func_00305148(temp_v0, 0);
    *arg1 = temp_v1;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

void func_00304110(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_002C8108();
    temp_v1 = func_00305148(temp_v0, 1);
    *arg1 = temp_v1;
    func_002C7D00(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304158);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003041F0);

void func_00304258(u64 arg0, u32 *arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2;

    temp_v0 = func_002C8108();
    temp_v1 = func_003298F8(temp_v0);
    temp_v2 = func_00304938(temp_v1);
    *arg1 = temp_v2;
    func_003297C8(temp_v0);
    func_002C7D00(arg0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003042B8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304310);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304360);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003043E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304490);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304530);

u32 func_003045E8(u32 arg0) {
    func_00304490();
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304618);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003046D0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304768);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304938);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304998);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304A38);

u32 func_00304AB8(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0x14));
    return 1;
}

s32 func_00304AD8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    temp_v0 = *(s32 *)(temp_v1 + 0x9c);
    if (temp_v0 != 0) {
        temp_v1 = temp_v0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304AF8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304B18);

void func_00304CE0(s32 arg0, s32 arg1) {
    func_00304B18(arg0, arg1, *(s32 *)(arg0 + 0x18) + arg1 * 0xa0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304D08);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304D60);

void func_00304D80(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x18) + (s32)arg1 * 0xa0;
    memset(temp_v0, 0, 0xa0);
    func_00304D60(arg0, arg1, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304DE8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304EE0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304F40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00304FB0);

void func_00305068(u32 arg0) {
    func_00304FB0(arg0, 0);
}

u8 func_00305080(s32 arg0) {
    return **(s32 **)(arg0 + 0x24) != 0;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305090);

u32 func_00305110(u32 arg0) {
    func_003297C8(*(u32 *)arg0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305148);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305348);

u32 func_003054E8(u32 arg0) {
    s32 *piVar1;

    piVar1 = (s32 *)arg0;
    if (*piVar1 != 0) {
        func_003297C8(*piVar1);
    }
    if (piVar1[1] == 0) {
        func_00305068(arg0);
        func_003297C8(piVar1[8]);
    }
    func_003297C8(piVar1[3]);
    func_00304AB8(arg0);
    func_00328E48(arg0);
    return 1;
}

u32 func_00305558(u32 *arg0, u32 arg1, u32 arg2) {
    *arg0 = arg2;
    arg0[4] = arg1;
    if ((arg2 & 2) != 0) {
        func_00304AF8();
    }
    arg0[2] = arg0[2] + 1;
    return 1;
}

u32 func_003055A0(u32 arg0, s32 arg1, s32 arg2, u32 arg3) {
    func_00305558(arg0, *(s32 *)(arg1 + 8) + arg2 * 0x24, arg3);
    return 1;
}

u32 func_003055D8(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_003055E8);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305650);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003056B0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305848);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003058B0);

u32 func_00305950(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 0xa0 + *(s32 *)(arg0 + 0x18) + 0x9c) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305970);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003059E0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305A60);

u32 configureEffectWithDefaultSetting(u32 effect, u32 slot, u32 kind, u32 value, u32 flags, u32 color) {
    func_00305A60(effect, slot, kind, value, flags, 0, color);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305B28);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305BD0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305C40);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00305EB0);

INCLUDE_ASM(const s32, "game/code_002DC138", func_00306030);

INCLUDE_ASM(const s32, "game/code_002DC138", func_003064C0);

void func_00306500(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    func_00305BD0(arg6, arg7);
    func_003089B8(arg0, arg1, arg2, arg3, arg4, arg5, arg7);
    func_00308478(0x44, arg7);
}

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D170);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D180);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D190);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1A0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1B0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1C0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1D0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1E0);

INCLUDE_RODATA(const s32, "game/code_002DC138", D_0042D1F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E2C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E34);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E3C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E54);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E5C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E64);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E6C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E74);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E7C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E84);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E94);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437E98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437ED8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437EF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F00);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F08);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F10);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F18);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F20);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F28);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F30);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F38);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F40);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F48);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F50);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F58);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F60);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F68);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F70);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F78);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F80);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F88);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F90);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437F98);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FA8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FB8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FC8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FD8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FE8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00437FF8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438000);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438008);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438010);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438018);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438020);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438028);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438030);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438038);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438040);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438048);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438050);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438058);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438060);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438068);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438070);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438078);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438080);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438088);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438090);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438098);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004380F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438100);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438108);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438110);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438118);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438120);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438128);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438130);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438138);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438140);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438148);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438150);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438158);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438160);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438168);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438170);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438178);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438180);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438188);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438190);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438198);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004381F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438200);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438208);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438210);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438218);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438220);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438228);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438230);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438238);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438240);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438248);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438250);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438258);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438260);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438268);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438270);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438278);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438280);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438288);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438290);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438298);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004382F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438300);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438308);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438310);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438318);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438320);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438328);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438330);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438338);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438340);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438348);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438350);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438358);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438360);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438368);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438370);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438378);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438380);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438388);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438390);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438398);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004383F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438400);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438408);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438410);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438418);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438420);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438428);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438430);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438438);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438440);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438448);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438450);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438458);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438460);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438468);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438470);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438478);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438480);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438488);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438490);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438498);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004384F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438500);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438508);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438510);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438518);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438520);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438528);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438530);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438538);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438540);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438548);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438550);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438558);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438560);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438568);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438570);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438578);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438580);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438588);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438590);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438598);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004385F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438600);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438608);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438610);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438618);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438620);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438628);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438630);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438638);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438640);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438644);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438648);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438650);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438658);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438660);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438668);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438670);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438678);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438680);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438688);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438690);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438698);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386CC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004386F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438700);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438708);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438710);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438718);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438720);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438728);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438730);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438738);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438740);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438748);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438750);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438758);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438759);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875A);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875B);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043875C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438760);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438764);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438768);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043876C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438770);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438774);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438778);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_0043877C);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438780);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438788);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438790);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438798);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387A8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387AC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B4);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387B8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387BC);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387C8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387D8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387E8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F0);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_004387F8);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438800);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438808);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438810);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438818);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438820);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438828);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438830);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438838);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438840);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438848);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438850);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438858);

INCLUDE_SDATA(const s32, "game/code_002DC138", D_00438860);


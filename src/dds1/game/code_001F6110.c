#include "common.h"

extern void func_001D54C0(s32 actor);

extern u32 func_001DAE48(s32 actor);
extern s32 func_001DAE50(s32 actor, u32 index);
extern void func_001D5440(s32 actor);

extern s32 D_00360348[];
extern s32 D_0035FFE0[];
extern s32 D_003BB6B8;
extern s32 D_003BD854;
extern s32 func_002EB028(s32, u32 *, s32);
extern void btlCmdSimpleB(s32, u16);
extern void btlCmdSimpleA(s32, u16);
extern void btlCmdSimpleE(s32, u16);
extern void btlCmdSimpleD(s32, u16);
extern void btlCmdSimpleJ(s32, u16);
extern void func_001FF030(s32, u16);

extern s8 D_003BB870;

extern s8 D_003A5440[];

extern s8 D_003A5460[];

extern s8 D_003A5488[];

extern u8 D_003BB818[];

extern u8 D_003BD476;

extern void *func_002CFEB8(s32 size);

extern s32 sceDopen(char *);

extern s32 D_003BD868;

extern s8 D_003A54A8[];

extern void func_001FB0A8(s32, ...);

extern s8 D_003A54C8[];

extern s8 D_003A54F0[];

extern s8 D_003A5518[];

extern s8 D_003A5538[];

extern s8 D_003A5558[];

extern u32 D_003BB6C8;

extern void func_003014F0();

extern u8 D_003BB820[];

extern u64 func_001D9718(void);

typedef struct BtlUnit {
    u8 unk_00[0x70];
    f32 unk_70[4];
    f32 unk_80;
    u8 unk_84[4];
    f32 unk_88;
    u8 unk_8C[4];
    f32 unk_90[4];
    f32 unk_A0[4];
    f32 unk_B0;
    f32 unk_B4;
    u8 unk_B8[0x50];
    u64 unitId; /* 0x108: compared against the battle command's unit ID */
    u32 flags;
    u32 unk_114;
    u8 unk_118[8];
    u16 unk_120;
    u8 unk_122[2];
    u16 unk_124;
    u8 unk_126[0x1F6];
    u32 unk_31C;
    u32 unk_320;
    u8 unk_324[0x20];
    struct BtlUnit *next;
} BtlUnit;

typedef struct BtlActor {
    u8 unk_00[0x18];
    BtlUnit *unit;
} BtlActor;

typedef struct BtlList {
    u8 unk_00[0x20];
    s32 count;
} BtlList;

typedef struct BtlState {
    u8 unk_000[0x228];
    BtlUnit *units;
    u8 unk_22C[0x70];
    BtlList *list;
    u8 unk_2A0[4];
    s32 slot;
} BtlState;

typedef struct BtlCmdCtx {
    u8 unk_00[0x18];
    BtlUnit *unit;
    u8 unk_1C[4];
    s32 result;
    s32 arg;
    u8 unk_28[4];
    s32 unk_2C;
    s32 unk_30;
} BtlCmdCtx;

extern f32 btlUnitGetTopY(BtlUnit *);

typedef struct BtlVec3 {
    f32 x, y, z;
} BtlVec3;

extern f32 func_0010D4F0(s32);

extern s32 func_001A17F8(void *);
extern s32 func_001A1838(void *);

extern void func_001D6318(BtlUnit *, f32 *);
extern void func_002E7D98(void);
extern void effObjFetchInnerFirstVec(u32);

extern f32 func_001F79A0(f32 *, f32 *, f32 *);

typedef union BtlVec4 {
    f32 f[4];
    u128 q;
} BtlVec4;

extern u64 func_001D9780(void);

extern u64 btlCreateCommandSoundTask(u64, u64);

extern s32 D_003BB3D8;

extern u32 func_0020A3F0(void);

extern u32 battleGetEffectValue(void);

extern u32 battleGetEffectActive(void);

extern u32 func_002099A0(void);

extern u32 func_00208C68(void);

extern u64 func_001ACAE0(void);

extern u16 func_0010D428(u32);

extern s32 func_0010D6A8(void);

extern u32 D_003BD858;

extern u32 func_0029BF88(u32, u32);

extern s32 func_001A17F0(void);
extern s32 func_001FEC68(s32 context, s32 actor, u32 mask);
extern void func_0010D5F0();
extern void btlCmdSimpleC(s32, u16);
extern u8 *func_001D4748(s32);
extern void func_001F60E8(void);

u8 *btlCreateControlObject(void) {
    u8 *object;
    object = func_001D4748(0);
    object[0] = 1;
    *(void (**)(void))(object + 0x4c) = func_001F60E8;
    *(u16 *)(object + 0x20) = 0x60;
    *(s32 *)(object + 0x48) = 0;
    object[0x10] = 0;
    return object;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6158);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6300);

void btlUnitGetMuzzlePosVU(BtlUnit *unit) {
    f32 pos[4];
    func_001D6318(unit, pos);
    pos[2] += unit->unk_88;
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_70));
    func_002E7D98();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_90));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->unk_80));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


void btlUnitGetBodyPosVU(BtlUnit *unit) {
    f32 pos[4];
    func_001D6318(unit, pos);
    pos[2] += unit->unk_88;
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_70));
    func_002E7D98();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_A0));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->unk_80));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


void btlUnitGetEffectPosVU(BtlUnit *unit) {
    f32 pos[4];
    if (!(unit->flags & 2)) {
        btlUnitGetMuzzlePosVU(unit);
        return;
    }
    effObjFetchInnerFirstVec(unit->unk_31C);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_70));
    func_002E7D98();
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit->unk_90));
    __asm__ volatile(".set noreorder\n\tqmtc2.ni %0, vf2\n\t.set reorder" : : "r"(unit->unk_80));
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf10w\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "r"(pos));
}


f32 func_001F6618(s32 arg0) {
    f32 temp_f2;
    f32 temp_f1;

    temp_f2 = *(f32 *)(arg0 + 0xb4);
    temp_f1 = *(f32 *)(arg0 + 0xb0);
    if (temp_f1 < temp_f2) {
        return temp_f2 * *(f32 *)(arg0 + 0x80);
    }
    return temp_f1 * *(f32 *)(arg0 + 0x80);
}

f32 btlUnitGetTopY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    return unit->unk_B0 * unit->unk_80 * 0.5f - pos[1];
}


f32 btlUnitGetBottomY(BtlUnit *unit) {
    f32 pos[4];
    btlUnitGetMuzzlePosVU(unit);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
    return -pos[1] - unit->unk_B0 * unit->unk_80 * 0.5f;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F66D8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6970);

f32 btlGetMaxUnitTop(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 value = btlUnitGetTopY(unit);
            if (first) {
                best = value;
                first = 0;
            } else if (best < value) {
                best = value;
            }
        }
        unit = unit->next;
    }
    return best;
}



f32 btlGetMaxUnitReach(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            f32 value = unit->unk_B4 * unit->unk_80;
            if (first) {
                best = value;
                first = 0;
            } else if (best < value) {
                best = value;
            }
        }
        unit = unit->next;
    }
    return best;
}

f32 btlGetExtremeUnitY(u32 mask) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    f32 best = 0.0f;
    s32 first = 1;
    f32 pos[4];
    f32 value;
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask)) {
            btlUnitGetMuzzlePosVU(unit);
            __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(pos) : "memory");
            if (mask & 0x200) {
                value = pos[2] + unit->unk_B4 * unit->unk_80;
                if (first) {
                    best = value;
                    first = 0;
                } else if (best < value) {
                    best = value;
                }
            } else {
                value = pos[2] - unit->unk_B4 * unit->unk_80;
                if (first) {
                    best = value;
                    first = 0;
                } else if (value < best) {
                    best = value;
                }
            }
        }
        unit = unit->next;
    }
    return best;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F6E28);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F70C8);

BtlUnit *btlFindFarthestUnit(u32 mask, f32 *point) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    BtlUnit *farthest = NULL;
    f32 best = 0.0f;
    f32 dist;
    while (unit != NULL) {
        if (unit->flags & 1) {
            if (!(unit->flags & 0xC0)) {
                if (unit->flags & mask) {
                    btlUnitGetMuzzlePosVU(unit);
                    __asm__ volatile(
                        ".set noreorder\n\t"
                        "mfc1 $2, %2\n\t"
                        "qmtc2.ni $2, vf2\n\t"
                        "vaddx.y vf10, vf0, vf2x\n\t"
                        "lqc2 vf11, 0(%1)\n\t"
                        "vsub.xyzw vf10, vf10, vf11\n\t"
                        "vmul.xyz vf2, vf10, vf10\n\t"
                        "vaddy.x vf2, vf2, vf2y\n\t"
                        "vaddz.x vf2, vf2, vf2z\n\t"
                        "vsqrt Q, vf2x\n\t"
                        "vwaitq\n\t"
                        "cfc2.ni $2, $vi22\n\t"
                        "mtc1 $2, %0\n\t"
                        ".set reorder"
                        : "=f"(dist)
                        : "r"(point), "f"(point[1]));
                    if (best < dist) {
                        best = dist;
                        farthest = unit;
                    }
                }
            }
        }
        unit = unit->next;
    }
    return farthest;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F72C8);

void func_001F73E0(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        func_001D5440((s32)unit);
    }
}

void func_001F7428(void) {
    BtlUnit *unit;

    for (unit = ((BtlState *)func_001A17F0())->units; unit != NULL; unit = unit->next) {
        func_001D54C0((s32)unit);
    }
}

void func_001F7470(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                func_001D5440((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void func_001F74D0(s32 mask) {
    BtlUnit *unit;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            if (unit->flags & mask) {
                func_001D54C0((s32)unit);
            }
            unit = unit->next;
        } while (unit != NULL);
    }
}

void func_001F7530(s32 actor) {
    u32 i = 0;
    u32 count = func_001DAE48(actor);
    if (count != 0) {
        do {
            func_001D5440(func_001DAE50(actor, i));
            i++;
        } while (i < count);
    }
}


void func_001F7598(s32 actor) {
    u32 i = 0;
    u32 count = func_001DAE48(actor);
    if (count != 0) {
        do {
            func_001D54C0(func_001DAE50(actor, i));
            i++;
        } while (i < count);
    }
}


extern s32 func_001D5D58(s32);
extern void func_001D6280(s32, s32);
extern void func_001D6640(s32, s32);
extern void func_001D5990(s32);
extern void func_001D5578(s32, s32, s32, f32);

void func_001F7600(void) {
    s32 state = func_001A17F0();
    s32 actor = *(s32 *)(state + 0x228);
    while (actor != 0) {
        func_001D5440(actor);
        func_001D6280(actor, actor + 0x30);
        func_001D6640(actor, actor + 0x40);
        if ((func_001D5D58(actor) == 0 && *(s32 *)(actor + 0xf0) != 0) ||
            (*(u32 *)(actor + 0xe8) & 2) != 0) {
            func_001D5990(actor);
            *(s16 *)(actor + 0xf8) = 0;
            *(s16 *)(actor + 0xfa) = 0;
            func_001D5578(actor, *(s32 *)(actor + 0xfc), *(s32 *)(actor + 0x100),
                          *(f32 *)(actor + 0x104));
        }
        *(u32 *)(actor + 0x114) &= ~0x8000;
        actor = *(s32 *)(actor + 0x344);
    }
    {
        void (*callback)(void) = *(void (**)(void))(state + 0x5f0);
        if (callback != 0) {
            callback();
        }
    }
}

extern void evtSetUnitStatusFlags(u32);
extern void func_00221D98(u32, s32);

void btlRefreshUnitEffects(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    u32 handle;
    while (unit != NULL) {
        if (unit->flags & 2) {
            if (unit->unk_114 & 0x10) {
                handle = unit->unk_320;
                evtSetUnitStatusFlags(handle);
                __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(unit));
                func_00221D98(handle, 0);
            }
        }
        unit = unit->next;
    }
}


s32 func_001F7770(s32 mask) {
    BtlUnit *unit;
    s32 count = 0;
    s32 flags;

    unit = ((BtlState *)func_001A17F0())->units;
    if (unit != NULL) {
        do {
            flags = unit->flags;
            if (((flags & mask) != 0) && ((flags & 0x20) == 0)) {
                count += flags & 1;
            }
            unit = unit->next;
        } while (unit != NULL);
    }
    return count;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F77D8);


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7868);

/* Unit normal of the triangle (a, b, c); result in vf10 (VU register convention). */
void btlTriangleNormalVU(f32 *a, f32 *b, f32 *c) {
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%1)\n\t"
        "lqc2 vf11, 0(%0)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf12, vf10\n\t"
        "lqc2 vf11, 0(%2)\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "vsub.xyzw vf11, vf11, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "vmove.xyzw vf10, vf12\n\t"
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        ".set reorder"
        :
        : "r"(a), "r"(b), "r"(c)
        : "memory");
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001F79A0);

void btlPointOffPlaneVU(f32 *a, f32 *b, f32 *c, f32 *d) {
    f32 dist = func_001F79A0(a, b, c);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        ".set reorder"
        :
        : "r"(d), "r"(c));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "f"(dist), "r"(c));
}


void btlProjectOnPlaneVU(f32 *a, f32 *b, f32 *c) {
    f32 normal[4];
    f32 dot;
    btlTriangleNormalVU(a, b, c);
    __asm__ volatile(".set noreorder\n\tsqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(normal) : "memory");
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%1)\n\t"
        "lqc2 vf11, 0(%2)\n\t"
        "vsub.xyzw vf10, vf10, vf11\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        "lqc2 vf10, 0(%3)\n\t"
        "vmul.xyz vf2, vf10, vf11\n\t"
        "vaddy.x vf2, vf2, vf2y\n\t"
        "vaddz.x vf2, vf2, vf2z\n\t"
        "qmfc2.ni $2, vf2\n\t"
        "mtc1 $2, %0\n\t"
        ".set reorder"
        : "=f"(dot)
        : "r"(a), "r"(c), "r"(normal));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        :
        : "f"(dot), "r"(c));
}



s32 btlPointInBox(BtlVec3 *a, BtlVec3 *b, BtlVec3 *p) {
    if (((a->x >= p->x && p->x >= b->x) || (a->x <= p->x && p->x <= b->x))
        && ((a->y >= p->y && p->y >= b->y) || (a->y <= p->y && p->y <= b->y))
        && ((a->z >= p->z && p->z >= b->z) || (a->z <= p->z && p->z <= b->z))) {
        return 1;
    }
    return 0;
}


u32 btlBlendColor(u32 colorA, u32 colorB, f32 t) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;
    if (t >= 1.0f) {
        return colorB;
    }
    unit = 0x3C000000;
    color1[0] = colorB;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmove.xyzw vf11, vf10\n"
        ".set reorder"
        : : "r"(unit), "r"(color1) : "$2", "memory");
    color2[0] = colorA;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder"
        : : "f"(1.0f - t) : "$2");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "qmtc2.ni $3, vf2\n"
        "vmulx.xyzw vf11, vf11, vf2x\n"
        "vadd.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "f"(t) : "$3");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %1\n"
        "qmtc2.ni $3, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vftoi0.xyzw vf10, vf10\n"
        "qmfc2.ni %0, vf10\n"
        "ppach %0, $0, %0\n"
        "ppacb %0, $0, %0\n"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f) : "$3");
    blended[0] = packed;
    
    return packed;
}


/* Blend two RGBA float vectors (0..1 scale) by t and pack to 8-bit channels. */
u32 btlBlendColorVec(f32 *a, f32 *b, f32 t) {
    u32 packed;
    s32 blended[4];
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "lqc2 vf11, 0(%1)\n\t"
        ".set reorder"
        : : "r"(a), "r"(b));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $2, %0\n\t"
        "qmtc2.ni $2, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        ".set reorder"
        : : "f"(1.0f - t));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $3, %0\n\t"
        "qmtc2.ni $3, vf2\n\t"
        "vmulx.xyzw vf11, vf11, vf2x\n\t"
        "vadd.xyzw vf10, vf10, vf11\n\t"
        ".set reorder"
        : : "f"(t));
    __asm__ volatile(
        ".set noreorder\n\t"
        "mfc1 $3, %1\n\t"
        "qmtc2.ni $3, vf2\n\t"
        "vmulx.xyzw vf10, vf10, vf2x\n\t"
        "vftoi0.xyzw vf10, vf10\n\t"
        "qmfc2.ni %0, vf10\n\t"
        "ppach %0, $0, %0\n\t"
        "ppacb %0, $0, %0\n\t"
        ".set reorder"
        : "=r"(packed) : "f"(128.0f));
    blended[0] = packed;
    return blended[0];
}


void func_001F7CC8(s32 arg0, f32 arg1) {
    *(f32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7CD8);

void func_001F7D30(s32 arg0, f32 arg1) {
    f32 temp_f0;

    *(f32 *)(arg0 + 0xc) = 0.0f;
    *(f32 *)(arg0 + 0) = arg1;
    temp_f0 = *(f32 *)(arg0 + 0xc);
    *(f32 *)(arg0 + 4) = arg1;
    *(f32 *)(arg0 + 0x10) = temp_f0;
    if (arg1 == temp_f0) {
        return;
    }
    *(f32 *)(arg0 + 0x8) = 1.0f / (arg1 * arg1 * 0.25f);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7D80);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7DF8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F7F70);

void func_001F8280(void) {
    func_001FB0A8((s32)"btl:[%s]\n", D_003BB6B8);
    D_003BD854 = func_002EB028(D_003BB6B8, &D_003BD858, 0);
}

void func_001F82B8(void) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = func_001A17F0();
    temp_v1 = func_0029BF88(D_003BD858, 0x10000);
    *(u32 *)(temp_v0 + 0x4b4) = temp_v1;
}

void func_001F82F0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    effReleaseSharedReference(*(u32 *)(temp_v0 + 0x4b4));
    *(u32 *)(temp_v0 + 0x4b4) = 0;
}

u32 func_001F8328(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x20) = 1;
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

u32 func_001F8358(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x20) = 6;
    *(u32 *)(temp_v0 + 0x24) = 0xc2;
    return 1;
}

u32 func_001F8388(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 0xd;
    return 1;
}

u32 func_001F83B8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0x24) = 1;
    *(u32 *)(temp_v0 + 0x20) = 10;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001F83E8);

u32 func_001F84A8(void) {
    s32 context = func_0010D6A8();
    u16 first = func_0010D428(0);
    u32 second = func_0010D428(1);
    *(u32 *)(context + 0x20) = 2;
    *(u32 *)(context + 0x24) = first;
    *(u32 *)(context + 0x8c) = second;
    *(u32 *)(context + 0x38) = second;
    return 1;
}

u32 func_001F8508(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    *(u32 *)(temp_v0 + 0xc) = *(u32 *)(temp_v0 + 0xc) | 1;
    return 1;
}

u32 func_001F8538(void) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D6A8();
    temp_v0 = func_0010D428(0);
    *(u16 *)(*(s32 *)(temp_v1 + 0x18) + 0x122) = temp_v0;
    return 1;
}

u32 func_001F8578(void) {
    s32 temp_v0;
    s16 temp_v1;

    temp_v0 = func_001A17F0();
    func_0010D6A8();
    temp_v1 = func_0010D428(0);
    *(u8 *)(temp_v0 + 0x25e) = 4;
    *(s32 *)(temp_v0 + 0x280) = temp_v1;
    return 1;
}

u32 func_001F85C8(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F85F0(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleB(context, value);
    return 1;
}

u32 func_001F8630(void) {
    btlCmdWithArgB(func_0010D6A8());
    return 1;
}

u32 func_001F8658(void) {
    btlCmdWithArgC(func_0010D6A8());
    return 1;
}

u32 func_001F8680(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleA(context, value);
    return 1;
}

u32 func_001F86C0(void) {
    btlCmdWithArgE(func_0010D6A8());
    return 1;
}

u32 func_001F86E8(void) {
    btlCmdWithArgD(func_0010D6A8());
    return 1;
}

u32 func_001F8710(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleC(context, value);
    return 1;
}

u32 func_001F8750(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleD(context, value);
    return 1;
}

u32 func_001F8790(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleE(context, value);
    return 1;
}

u32 func_001F87D0(void) {
    btlCmdWithArgA(func_0010D6A8());
    return 1;
}

u32 func_001F87F8(void) {
    btlCmdSimpleF(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8828(void) {
    btlCmdSimpleG(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8858(void) {
    btlCmdSimpleH(func_0010D6A8(), 0);
    return 1;
}

u32 func_001F8888(void) {
    btlCmdWithArgF(func_0010D6A8());
    return 1;
}

u32 func_001F88B0(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    btlCmdSimpleJ(context, value);
    return 1;
}

u32 func_001F88F0(void) {
    btlCmdSimpleI(func_0010D6A8(), 0);
    return 1;
}

u32 btlNbScriptCheckActorFlag(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 1;
        *(s32 *)(context + 0x98) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~1;
    }
    return 1;
}

u32 func_001F89B0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x6000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 2;
        *(s32 *)(context + 0x9C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~2;
    }
    return 1;
}

u32 func_001F8A40(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 4;
        *(s32 *)(context + 0xA0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~4;
    }
    return 1;
}
u32 func_001F8AD0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 8;
        *(s32 *)(context + 0xA4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~8;
    }
    return 1;
}
u32 func_001F8B60(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x10;
        *(s32 *)(context + 0xA8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x10;
    }
    return 1;
}
u32 func_001F8BF0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x1C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x20;
        *(s32 *)(context + 0xAC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x20;
    }
    return 1;
}
u32 func_001F8C80(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x40;
        *(s32 *)(context + 0xB0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x40;
    }
    return 1;
}
u32 func_001F8D10(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x80;
        *(s32 *)(context + 0xB4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x80;
    }
    return 1;
}
u32 func_001F8DA0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x100;
        *(s32 *)(context + 0xB8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x100;
    }
    return 1;
}
u32 func_001F8E30(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x2C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x200;
        *(s32 *)(context + 0xBC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x200;
    }
    return 1;
}
u32 func_001F8EC0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x400;
        *(s32 *)(context + 0xC0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x400;
    }
    return 1;
}
u32 func_001F8F50(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x800;
        *(s32 *)(context + 0xC4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x800;
    }
    return 1;
}
u32 func_001F8FE0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x3800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x1000;
        *(s32 *)(context + 0xC8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x1000;
    }
    return 1;
}

u32 func_001F9070(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x4800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x2000;
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x2000;
    }
    return 1;
}

u32 func_001F90E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x5C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x2000000;
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x2000000;
    }
    return 1;
}

u32 func_001F9168(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x4C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x4000;
        *(s32 *)(context + 0xD0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x4000;
    }
    return 1;
}
u32 func_001F91F8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x5000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x4000;
        *(s32 *)(context + 0xD0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x4000;
    }
    return 1;
}

u32 func_001F9288(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x6c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F92D8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x9400000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9328(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x9c00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9378(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x8C00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x40000;
        *(s32 *)(context + 0xE0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x40000;
    }
    return 1;
}
u32 func_001F9410(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x9000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x80000;
        *(s32 *)(context + 0xE4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x80000;
    }
    return 1;
}

u32 func_001F94A8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10400000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9518(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10800000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9588(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x100000;
        *(s32 *)(context + 0xE8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x100000;
    }
    return 1;
}
u32 func_001F9620(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x200000;
        *(s32 *)(context + 0xEC) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x200000;
    }
    return 1;
}
u32 func_001F96B8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xA800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x400000;
        *(s32 *)(context + 0xF0) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x400000;
    }
    return 1;
}
u32 func_001F9750(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xAC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x800000;
        *(s32 *)(context + 0xF4) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x800000;
    }
    return 1;
}

u32 func_001F97E8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xB800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 1;
        *(s32 *)(context + 0x104) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~1;
    }
    return 1;
}
u32 func_001F9878(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xBC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 2;
        *(s32 *)(context + 0x108) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~2;
    }
    return 1;
}
u32 func_001F9908(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 4;
        *(s32 *)(context + 0x10C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~4;
    }
    return 1;
}
u32 func_001F9998(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 8;
        *(s32 *)(context + 0x110) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~8;
    }
    return 1;
}
u32 func_001F9A28(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xC800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x10;
        *(s32 *)(context + 0x114) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x10;
    }
    return 1;
}
u32 func_001F9AB8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xCC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x20;
        *(s32 *)(context + 0x118) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x20;
    }
    return 1;
}
u32 func_001F9B48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xD400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x400;
        *(s32 *)(context + 0x12C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x400;
    }
    return 1;
}
u32 func_001F9BD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xD800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x800;
        *(s32 *)(context + 0x130) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x800;
    }
    return 1;
}

u32 func_001F9C68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x10C00000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9CD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x11000000)) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9D48(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x9800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x40;
        *(s32 *)(context + 0x11C) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x40;
    }
    return 1;
}
u32 func_001F9DD8(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0xDC00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x1000;
        *(s32 *)(context + 0x134) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x1000;
    }
    return 1;
}

u32 func_001F9E68(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0xD000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x80;
        *(s32 *)(context + 0x120) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x80;
    }
    return 1;
}

u32 func_001F9EF0(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0xe800000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F40(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0xec00000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001F9F90(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0xB000000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x1000000;
        *(s32 *)(context + 0xF8) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x1000000;
    }
    return 1;
}

u32 func_001FA018(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), 0x0b400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x90) |= 0x04000000;
        *(s32 *)(context + 0x100) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x90) &= ~0x04000000;
    }
    return 1;
}

u32 func_001FA0A0(void) {
    if (func_002099E0() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA0E0(void) {
    if (battleHasEffectActor() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA120(void) {
    if (func_00207C18() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 btlCmdCheckHpPercent(void) {
    BtlUnit *unit = ((BtlState *)func_001A17F0())->units;
    s32 side = func_0010D428(0);
    s32 id = func_0010D428(1);
    s32 percent = func_0010D428(2);
    u32 mask = 0x200;
    if (side) {
        mask = 0x400;
    }
    while (unit != NULL) {
        if ((unit->flags & 1) && (unit->flags & mask) && !(unit->flags & 0x20) && unit->unitId == id) {
            u8 *stats = (u8 *)unit + 0x120;
            s32 current = func_001A17F8(stats);
            s32 maximum = func_001A1838(stats);
            if (!((u32)(maximum * percent) < (u32)(current * 100))) {
                func_0010D5F0(1);
                return 1;
            }
        }
        unit = unit->next;
    }
    func_0010D5F0(0);
    return 1;
}


u32 func_001FA270(void) {
    if (func_0020A418() != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA2B0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x0f800000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x4000;
        *(s32 *)(context + 0x13c) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x4000;
    }
    return 1;
}

u32 func_001FA340(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x0fc00000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x8000;
        *(s32 *)(context + 0x140) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x8000;
    }
    return 1;
}

u32 func_001FA3D0(void) {
    s32 context = func_0010D6A8();
    if (func_001FEC68(context, *(s32 *)(context + 0x18), func_0010D428(0) | 0x0f400000)) {
        func_0010D5F0(1);
        *(u32 *)(context + 0x94) |= 0x2000;
        *(s32 *)(context + 0x138) = func_0010D428(0);
    } else {
        func_0010D5F0(0);
        *(u32 *)(context + 0x94) &= ~0x2000;
    }
    return 1;
}

u32 func_001FA460(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    if (func_001FEC68(temp_v0, *(s32 *)(temp_v0 + 0x18), 0x10000000) != 0) {
        func_0010D5F0(1);
    } else {
        func_0010D5F0(0);
    }
    return 1;
}

u32 func_001FA4B0(void) {
    u64 temp_v0;

    temp_v0 = func_001ACAE0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA4D8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_0010D5F0(*(u32 *)(temp_v0 + 0x250));
    return 1;
}

u32 func_001FA500(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    func_0010D5F0(*(u16 *)(*(s32 *)(temp_v0 + 0x18) + 0x134));
    return 1;
}

u32 func_001FA530(void) {
    u32 temp_v0;

    temp_v0 = func_00208C68();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA558(void) {
    u32 temp_v0;

    temp_v0 = func_002099A0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA580(void) {
    u32 temp_v0;

    temp_v0 = battleGetEffectActive();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA5A8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    func_0010D5F0(*(u16 *)(temp_v0 + 0x25c));
    return 1;
}

u32 func_001FA5D0(void) {
    u32 temp_v0;

    temp_v0 = battleGetEffectValue();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA5F8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D6A8();
    func_0010D5F0(*(s8 *)(temp_v0 + 0x146));
    return 1;
}

u32 func_001FA620(void) {
    func_0010D5F0(D_003BB870);
    return 1;
}

u32 func_001FA648(void) {
    u32 temp_v0;

    temp_v0 = func_0020A3F0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_001FA670(void) {
    func_001FED20(func_0010D6A8());
    return 1;
}

u32 func_001FA698(void) {
    s32 context = func_0010D6A8();
    u16 value = func_0010D428(0);
    func_001FF030(context, value);
    return 1;
}

u32 func_001FA6D8(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = func_0010D6A8();
    temp_v0 = D_003BB3D8;
    *(u32 *)(temp_v1 + 0x20) = 0x10;
    *(u32 *)(temp_v1 + 0x24) = 0;
    *(u8 *)(temp_v0 + 0x54) = 1;
    return 1;
}

u32 func_001FA718(void) {
    func_001C44D0();
    return 1;
}

u32 func_001FA738(void) {
    func_001C4490();
    return 1;
}

u32 func_001FA758(void) {
    func_001F73E0();
    return 1;
}

u32 func_001FA778(void) {
    func_001F7428();
    func_001F7470(0x200);
    return 1;
}

u32 func_001FA7A0(void) {
    func_001F7428();
    func_001F7470(0x400);
    return 1;
}

extern s32 func_001DBAF0(s32, f32, f32, f32, f32, f32, f32, f32, f32);
extern s32 btlScheduleContextReset(void);

u32 btlCmdCameraMove(void) {
    f32 pos[3];
    f32 target[4];
    pos[0] = func_0010D4F0(0);
    pos[1] = func_0010D4F0(1);
    pos[2] = func_0010D4F0(2);
    target[0] = func_0010D4F0(3);
    target[1] = func_0010D4F0(4);
    target[2] = func_0010D4F0(5);
    target[3] = func_0010D4F0(6);
    startBattleTask(func_001D9718());
    startBattleTask(func_001D9780());
    startBattleTask(func_001DBAF0(0, pos[0], pos[1], pos[2], target[0], target[1], target[2], target[3], 40.0f));
    startBattleTask(btlScheduleContextReset());
    return 1;
}


u32 func_001FA898(void) {
    u64 temp_v0;

    temp_v0 = func_001D9718();
    startBattleTask(temp_v0);
    temp_v0 = func_001D9780();
    startBattleTask(temp_v0);
    temp_v0 = btlCreateCommandSoundTask(func_0010D6A8(), 0x11);
    startBattleTask(temp_v0);
    return 1;
}

u32 func_001FA8F0(void) {
    u64 temp_v0;

    temp_v0 = func_001D9718();
    startBattleTask(temp_v0);
    temp_v0 = func_001D9780();
    startBattleTask(temp_v0);
    temp_v0 = btlCreateCommandSoundTask(0, 3);
    startBattleTask(temp_v0);
    return 1;
}

extern f32 D_003D74E0[];
extern f32 D_003D7500[];

s32 func_001FA940(void) {
    D_003D74E0[0] = func_0010D4F0(0);
    D_003D74E0[1] = func_0010D4F0(1);
    D_003D74E0[2] = func_0010D4F0(2);
    D_003D7500[0] = func_0010D4F0(3);
    D_003D7500[1] = func_0010D4F0(4);
    D_003D7500[2] = func_0010D4F0(5);
    D_003D7500[3] = func_0010D4F0(6);
    return 1;
}

s32 func_001FA9C8(void) {
    D_003D74E0[4] = func_0010D4F0(0);
    D_003D74E0[5] = func_0010D4F0(1);
    D_003D74E0[6] = func_0010D4F0(2);
    D_003D7500[4] = func_0010D4F0(3);
    D_003D7500[5] = func_0010D4F0(4);
    D_003D7500[6] = func_0010D4F0(5);
    D_003D7500[7] = func_0010D4F0(6);
    return 1;
}

extern s32 func_001DBCB0(s32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

u32 btlCmdCameraMoveBlend(void) {
    f32 timeA = func_0010D4F0(0);
    f32 timeB = func_0010D4F0(1);
    startBattleTask(func_001D9718());
    startBattleTask(func_001D9780());
    startBattleTask(func_001DBCB0(0, D_003D74E0[0], D_003D74E0[1], D_003D74E0[2], D_003D7500[0], D_003D7500[1],
                                D_003D7500[2], D_003D7500[3], D_003D74E0[4], D_003D74E0[5], D_003D74E0[6],
                                D_003D7500[4], D_003D7500[5], D_003D7500[6], D_003D7500[7], timeA, timeB));
    startBattleTask(btlScheduleContextReset());
    return 1;
}


u32 func_001FAB38(void) {
    func_001FB0A8(D_003A5440);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FAB68(void) {
    func_001FB0A8(D_003A5460);
    func_0010D5F0(0x14);
    return 1;
}

u32 func_001FAB98(void) {
    func_0010D6A8();
    return 1;
}

u32 func_001FABB8(void) {
    func_001FB0A8(D_003A5488);
    func_0010D5F0(0);
    return 1;
}

u32 func_001FABE8(void) {
    func_001FB0A8(D_003A54A8);
    return 1;
}

u32 func_001FAC10(void) {
    func_001FB0A8(D_003A54C8);
    return 1;
}

u32 func_001FAC38(void) {
    func_001FB0A8(D_003A54F0);
    return 1;
}

u32 func_001FAC60(void) {
    func_001FB0A8(D_003A5518);
    return 1;
}

u32 func_001FAC88(void) {
    func_001FB0A8(D_003A5538);
    return 1;
}

u32 func_001FACB0(void) {
    func_001FB0A8(D_003A5558);
    return 1;
}

u32 func_001FACD8(void) {
    func_00204FE0();
    return 1;
}

extern s32 D_003BAAA8;

void btlBindActorSlot(BtlActor *actor, s32 arg1) {
    BtlState *state = (BtlState *)func_001A17F0();
    s32 slot = func_0010BC68(state->list->count - 1, D_003BAAA8, arg1);
    s32 handle;
    func_0010C028(slot, actor);
    handle = *(s32 *)((u8 *)func_00101A70(slot) + 0xCC);
    if (handle >= 0) {
        BtlUnit *unit = actor->unit;
        func_0019C590(handle, 0, unit->unk_124, (unit->unk_120 & 0x20) ? 1 : 2);
    }
    func_00101A80(state->list, slot);
    state->slot = slot;
}


void func_001FADA8(void) {
}

void func_001FADB0(void) {
}

void func_001FADB8(void) {
}

u32 func_001FADC0(void) {
    D_003BB6C8 = D_003BB6C8 | 0x2000000;
    return 0;
}

u32 func_001FADD8(u32 arg0) {
    return arg0;
}

void func_001FADE0(void) {
}

void func_001FADE8(void) {
}

void func_001FADF0(void) {
}

extern void func_00221FF8(s32, s32);

void func_001FADF8(void) {
    s32 state = func_001A17F0();
    if ((*(u32 *)(state + 0x1f4) & 0x40000000) == 0) {
        s32 actor = *(s32 *)(state + 0x228);
        while (actor != 0) {
            if ((*(u32 *)(actor + 0x110) & 2) != 0) {
                s32 effect = *(s32 *)(actor + 0x320);
                if (effect != 0) {
                    func_00221FF8(effect, 0);
                }
            }
            actor = *(s32 *)(actor + 0x344);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FAE78);

void func_001FB050(void) {
    D_00360348[4] &= ~0x80;
    D_0035FFE0[4] &= ~0x80;
}

void func_001FB080(void) {
}

void func_001FB088(void) {
}

void func_001FB090(void) {
}

void func_001FB098(void) {
}

void func_001FB0A0(void) {
}

void func_001FB0A8(s32 arg0, ...) {
}

void func_001FB0F0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, ...) {
}

void func_001FB130(void) {
}

void func_001FB138(void) {
}

void func_001FB140(void) {
}

void func_001FB148(void) {
}

void func_001FB150(void) {
}

void func_001FB158(void) {
}

void func_001FB160(void) {
}

void func_001FB168(void) {
}

void func_001FB170(void) {
}

void func_001FB178(void) {
}

void func_001FB180(void) {
}

void func_001FB188(void) {
}

void func_001FB190(void) {
}

void func_001FB198(void) {
}

void func_001FB1A0(void) {
}

void func_001FB1A8(void) {
}

void func_001FB1B0(void) {
}

void func_001FB1B8(void) {
}

void func_001FB1C0(void) {
}

void func_001FB1C8(void) {
}

void func_001FB1D0(void) {
}

void func_001FB1D8(void) {
}

void func_001FB1E0(void) {
}

void func_001FB1E8(void) {
}

void func_001FB1F0(void) {
}

u8 *btlFindEntryByCommand(u8 *list, s32 command) {
    s32 i;
    u8 *entry = *(u8 **)(list + 0x14);
    for (i = 0; i < *(s32 *)(list + 0x18); i++) {
        if (*(s32 *)(entry + 8) == command) {
            return entry;
        }
        entry += 0x18;
    }
    return entry;
}


s32 btlCountNonPartOpcodes(u8 *list) {
    u32 count;
    u8 *entry;
    u32 i;
    s32 found;
    if (list == NULL) {
        return 0;
    }
    count = *(u32 *)(list + 0x18);
    if (count == 0) {
        return 0;
    }
    entry = *(u8 **)(list + 0x14);
    found = 0;
    for (i = 0; i < count; i++) {
        switch (entry[4]) {
        case 0x14:
        case 0x15:
            break;
        default:
            found++;
            break;
        }
        entry += 0x18;
    }
    return found;
}

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5440);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5460);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5488);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54A8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54C8);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A54F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5518);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5538);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5558);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5580);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5590);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A55F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5600);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5610);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5620);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5630);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5640);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5650);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5660);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5670);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5680);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5690);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56A0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56B0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56C0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56D0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56E0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A56F0);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5700);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5710);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5720);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5730);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5740);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5750);

INCLUDE_RODATA(const s32, "game/code_001F6110", D_003A5760);

s32 func_001FB2A0(s32 arg0) {
    char buf[0x70];

    if (D_003BD476 != 0) {
        func_003014F0(buf, "pfs0:/%s", arg0);
        return sceDopen(buf);
    }
    D_003BD868 = 0;
    return 0;
}

void func_001FB2F0(void) {
    if (D_003BD476 == 0) {
        return;
    }
    func_003101B8();
}


extern char *D_00360380[];
extern s32 func_00310320(void);

typedef struct BtlReader {
    u32 flags;
    u8 unk_04[0x3C];
    char name[0x40];
} BtlReader;

s32 func_001FB320(s32 unused, BtlReader *reader) {
    if (D_003BD476 != 0) {
        return func_00310320();
    }
    if ((u32)D_003BD868 >= 8) {
        return 0;
    }
    strcpy(reader->name, D_00360380[D_003BD868]);
    reader->flags &= ~0x1000;
    D_003BD868++;
    return strlen(reader->name);
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB3C8);

void func_001FB870(s32 arg0) {
    s32 temp_v0;

    if (*(s32 *)(arg0 + 8) != 0) {
        temp_v0 = *(s32 *)(arg0 + 8);
        do {
            s32 temp_v1 = *(s32 *)(temp_v0 + 0x40);
            func_002CFF98(temp_v0);
            temp_v0 = temp_v1;
        } while (temp_v0 != 0);
    }
    func_002CFF98(*(s32 *)(arg0 + 4));
    func_002CFF98(arg0);
}

typedef struct BtlEntry {
    s32 category;
    s32 flags;
    s32 id;
    char name[0x30];
    struct BtlEntry *prev;
    struct BtlEntry *next;
} BtlEntry;

typedef struct BtlEntryList {
    s32 count;
    s32 unk_04;
    BtlEntry *head;
} BtlEntryList;

void btlAppendEntry(BtlEntryList *list, char *name, s32 category, s32 flags, s32 id) {
    BtlEntry *entry = func_002CFEB8(0x44);
    BtlEntry *tail;
    entry->category = category;
    entry->flags = flags;
    entry->id = id;
    strcpy(entry->name, name);
    if (list->head == NULL) {
        list->head = entry;
        entry->prev = NULL;
        entry->next = NULL;
    } else {
        tail = list->head;
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = entry;
        entry->prev = tail;
        entry->next = NULL;
    }
    list->count++;
}


INCLUDE_ASM(const s32, "game/code_001F6110", func_001FB9A8);

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FBA38);

void func_001FBEE8(s32 resource) {
    s32 handle = *(s32 *)(resource + 0x3c);
    if (handle != 0 && *(s32 *)(resource + 0x40) == 1) {
        func_002D2D00(handle);
    }
    func_002CFF98(resource);
}

void func_001FBF30(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_001FBF40(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_001FBF48(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

s32 func_001FBF50(u8 *resource, s32 output) {
    s32 index = *(s32 *)(*(s32 *)(resource + 0x44) + 4);
    if (index != 0) {
        func_003014F0(output, D_003BB818, index, *(s32 *)(resource + 0x34) + 0xc);
    } else {
        func_003014F0(output, D_003BB820, *(s32 *)(resource + 0x34) + 0xc);
    }
    return *(s32 *)(*(s32 *)(resource + 0x34));
}

s32 btlTrimResourceName(u8 *resource, char *output) {
    u32 length;
    u32 i;
    func_003014F0(output, D_003BB820, *(s32 *)(resource + 0x34) + 0xC);
    length = strlen(output);
    for (i = 0; i < length && output[i] != '.'; i++) {
    }
    if (length != i) {
        output[i] = 0;
    }
    return **(s32 **)(resource + 0x34);
}


u32 func_001FC078(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x34) + 4);
}

extern void func_002D2D00(s32);
extern void func_002D0918(s32);
void func_001FC100(u8 *, s32);

void func_001FC088(u8 *resource, s32 name) {
    u32 loaded;
    s32 handle = *(s32 *)(resource + 0x3c);
    s32 buffer;
    if (handle != 0 && *(s32 *)(resource + 0x40) == 1) {
        func_002D2D00(handle);
        *(s32 *)(resource + 0x3c) = 0;
    }
    buffer = func_002EB028(name, &loaded, 0);
    func_001FC100(resource, loaded);
    func_002D0918(buffer);
}

extern s32 func_002D3288(s32);

void func_001FC100(u8 *resource, s32 name) {
    s32 handle = *(s32 *)(resource + 0x3c);
    if (handle != 0 && *(s32 *)(resource + 0x40) == 1) {
        func_002D2D00(handle);
        *(s32 *)(resource + 0x3c) = 0;
    }
    *(s32 *)(resource + 0x3c) = func_002D3288(name);
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC160);

s32 func_001FC280(s32 arg0) {
    s32 temp_v0;

    temp_v0 = (s32)func_002CFEB8(0x38);
    *(s32 *)(temp_v0 + 0x14) = 9;
    *(s32 *)(temp_v0 + 0) = 8;
    *(s32 *)(temp_v0 + 4) = 8;
    *(s32 *)(temp_v0 + 8) = 0;
    *(s32 *)(temp_v0 + 0x10) = 0;
    *(s32 *)(temp_v0 + 0xc) = 0;
    *(s32 *)(temp_v0 + 0x18) = 0;
    strcpy(temp_v0 + 0x1c, arg0);
    return temp_v0;
}

void func_001FC2E8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_001F6110", func_001FC300);

void func_001FC720(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0) = arg1;
    *(s32 *)(arg0 + 4) = arg2;
}

u32 func_001FC730(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

void func_001FC738(char *arg0, char *arg1) {
    strcpy(arg0 + 0x21, arg1);
    *(s32 *)(arg0 + 0x10) = strlen(arg1);
}

void func_001FC778(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB818, arg0 + 0x21, arg0 + 0x1c);
}

void func_001FC7A8(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BB820, arg0 + 0x21);
}

void func_001FC7D0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6CC);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E4);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6E8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB6F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB700);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB708);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB710);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB718);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB720);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB724);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB728);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB730);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB738);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB740);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB748);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB750);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB758);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB760);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB768);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB76C);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB770);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB778);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB780);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB788);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB790);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB798);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7A8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7B8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7C8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7D8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7E0);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB7F8);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB800);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB808);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB810);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB818);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB820);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB828);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB830);

INCLUDE_SDATA(const s32, "game/code_001F6110", D_003BB838);


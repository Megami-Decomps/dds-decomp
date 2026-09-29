#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

typedef struct {
    u8 pad[0xA4];    /* 0x0 */
    u32 unkA4;       /* 0xA4 copied to unkFC by func_0015A608 */
    u8 padA8[0x54];  /* 0xA8 */
    void *unkFC;     /* 0xFC */
    u8 pad100[0x40]; /* 0x100 */
    u16 unk140;      /* 0x140 dispatch index for D_0034E250/D_0034E258/D_0034E2F0 */
    u16 unk142;      /* 0x142 read by func_0015A6E0, set to 1 by func_0015A608 */
    u8 pad144[0x30]; /* 0x144 */
    void *unk174;    /* 0x174 child released by func_00158F68 */
} ParObj;

extern void (*D_003AAC20[])();

extern BillDispatch D_003AAB88[];

void parReleaseObject(ParObj *obj) {
    s32 child;

    child = (s32)obj->unk174;
    if (child != 0) {
        func_003297C8(child);
    }
    effDestroyResources(obj);
    func_00328E48(obj);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00160B98);

INCLUDE_ASM(const s32, "effect/parManager", func_00160EF8);

INCLUDE_ASM(const s32, "effect/parManager", func_001612D8);

INCLUDE_ASM(const s32, "effect/parManager", func_001616A8);

INCLUDE_ASM(const s32, "effect/parManager", func_001617F8);

void func_001618C8(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "effect/parManager", func_001618E0);

INCLUDE_ASM(const s32, "effect/parManager", func_00161958);

INCLUDE_ASM(const s32, "effect/parManager", func_00161A10);

void parClearSlotFlag(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 4) + 4) = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161B38);

INCLUDE_ASM(const s32, "effect/parManager", func_00161D08);

u32 func_00161EE8(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
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
    color2[0] = colorB;
    __asm__ volatile (
        ".set noreorder\n"
        "lw $2, 0(%1)\n"
        "pextlb $2, $0, $2\n"
        "pextlh $2, $0, $2\n"
        "qmtc2.ni $2, vf10\n"
        "vitof0.xyzw vf10, vf10\n"
        "qmtc2.ni %0, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(unit), "r"(color2) : "$2", "memory");
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
    return blended[0];
}

INCLUDE_ASM(const s32, "effect/parManager", parCreateIndexed);

INCLUDE_ASM(const s32, "effect/parManager", parDispatchByKind);

INCLUDE_ASM(const s32, "effect/parManager", func_00161FE8);

INCLUDE_ASM(const s32, "effect/parManager", parCloneKind);

void parRestartKind(ParObj *obj) {
    D_003AAC20[obj->unk140]();
    obj->unkFC = (void *)obj->unkA4;
    obj->unk142 = 1;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00162248);

u16 parGetRestartFlag(ParObj *obj) {
    return obj->unk142;
}

void func_001622D8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

INCLUDE_ASM(const s32, "effect/parManager", func_001622E8);

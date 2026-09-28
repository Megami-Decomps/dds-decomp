#include "common.h"
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

void func_00160B58(ParObj *obj) {
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

INCLUDE_ASM(const s32, "effect/parManager", func_00161EE8);

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

u16 func_001622D0(ParObj *obj) {
    return obj->unk142;
}

INCLUDE_ASM(const s32, "effect/parManager", func_001622D8);

INCLUDE_ASM(const s32, "effect/parManager", func_001622E8);

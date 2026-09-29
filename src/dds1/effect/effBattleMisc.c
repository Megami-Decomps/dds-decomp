#include "common.h"

extern s32 func_00161860(void);

extern s32 func_00161858(void);

/* Flag word read by the two wrappers below. */
typedef struct {
    u8  pad_0x000[0x110]; /* 0x00 */
    u32 flags;            /* 0x110: selected by mask 0xE00 */
} EffBattleMiscCtx; /* 0x114 */

typedef struct {
    u8 unk00;
    u8 value; /* 0x01 */
} EffBattleMiscParam;

extern u32 D_0034E720[];

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161AA0);

extern u32 func_00161868(void);
extern u32 func_00161870(void);
extern void func_00161BA0();

void func_00161AE0(u32 unused, void *arg) {
    func_00161BA0(func_00161868(), arg);
}

void func_00161B10(u32 unused, void *arg) {
    func_00161BA0(func_00161870(), arg);
}

extern void func_001D63E8();

void func_00161B40(u32 unused, EffBattleMiscParam *param) {
    func_001D63E8(func_00161868(), param->value);
}

void func_00161B70(u32 unused, EffBattleMiscParam *param) {
    func_001D63E8(func_00161870(), param->value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161BA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161E48);

void func_00162028(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00161858();

    (void)unused;
    func_00161E48(ctx->flags & 0xE00, value);
}

void func_00162058(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00161860();

    (void)unused;
    func_00161E48(ctx->flags & 0xE00, value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162088);

void effBattleMiscApplyParamByte(u32 arg0, EffBattleMiscParam *param) {
    func_001D63E8(arg0, param->value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162270);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162338);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001623C0);

u32 effBattleMiscGetTableEntry(s32 arg0) {
    return D_0034E720[arg0];
}

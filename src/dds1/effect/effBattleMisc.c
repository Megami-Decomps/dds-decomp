#include "common.h"

extern s32 func_00161860(void);

extern s32 func_00161858(void);

/* Flag word read by the two wrappers below. */
typedef struct {
    u8  pad_0x000[0x110]; /* 0x00 */
    u32 unk110;           /* 0x110 */
} EffBattleMiscCtx; /* 0x114 */

extern u32 D_0034E720[];

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161AA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161AE0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B10);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B40);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161B70);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161BA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161E48);

void func_00162028(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00161858();

    (void)unused;
    func_00161E48(ctx->unk110 & 0xE00, value);
}

void func_00162058(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00161860();

    (void)unused;
    func_00161E48(ctx->unk110 & 0xE00, value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162088);

void func_00162258(u32 arg0, s32 arg1) {
    func_001D63E8(arg0, *(u8 *)(arg1 + 1));
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162270);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00162338);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001623C0);

u32 func_001624B8(s32 arg0) {
    return D_0034E720[arg0];
}

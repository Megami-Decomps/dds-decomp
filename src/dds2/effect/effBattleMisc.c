#include "common.h"
#include "pcp_vu0.h"

extern s32 func_00169440(void);

extern s32 func_00169438(void);

/* Flag word read by the two wrappers below. */
typedef struct {
    u8  pad_0x000[0x110]; /* 0x00 */
    u32 flags;            /* 0x110: selected by mask 0xE00 */
} EffBattleMiscCtx; /* 0x114 */

typedef struct {
    u8 unk00;
    u8 value; /* 0x01 */
} EffBattleMiscParam;

extern u32 D_003AB050[];

extern void (*D_003AF208[])();

/* Look up the position provider selected by param->unk00; it leaves the vector in vf10. */
void func_001696D0(void *owner, EffBattleMiscParam *param, u128 *out) {
    D_003AF208[param->unk00](owner, param);
    VU0_STORE_VF(vf10, out);
}

extern u32 func_00169448(void);
extern u32 func_00169450(void);
extern void func_001697D0();

void effBattleMiscCallByOwnerA(u32 unused, void *arg) {
    func_001697D0(func_00169448(), arg);
}

void func_00169740(u32 unused, void *arg) {
    func_001697D0(func_00169450(), arg);
}

extern void func_001E31F0();

void func_00169770(u32 unused, EffBattleMiscParam *param) {
    func_001E31F0(func_00169448(), param->value);
}

void func_001697A0(u32 unused, EffBattleMiscParam *param) {
    func_001E31F0(func_00169450(), param->value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_001697D0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169A78);

void func_00169C58(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00169438();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}

void func_00169C88(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)func_00169440();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}

typedef struct {
    u8 pad00[0x80];
    f32 f80;              /* 0x80 */
    u8 pad84[0x30];
    f32 fB4;              /* 0xB4 */
    u8 padB8[0x58];
    u32 flags110;         /* 0x110 */
    u32 flags114;         /* 0x114 */
} EffBattleMiscUnit;

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00169CB8);

void effBattleMiscApplyParamByte(u32 owner, EffBattleMiscParam *param) {
    func_001E31F0(owner, param->value);
}

extern f32 func_00208298(u32 mask, s32 a, s32 b);
extern void btlUnitGetEffectPosVU(void *unit);
extern void btlUnitGetMuzzlePosVU(void *unit);

f32 func_00169EA0(EffBattleMiscUnit *unit, EffBattleMiscParam *param) {
    EffBattleMiscUnit *other;
    f32 result = 0;

    switch (param->unk00) {
    case 0:
        result = unit->fB4 * unit->f80;
        break;
    case 1:
        other = (EffBattleMiscUnit *)func_00169438();
        result = func_00208298(other->flags110 & 0x600, 0, 0);
        break;
    case 2:
        other = (EffBattleMiscUnit *)func_00169440();
        result = func_00208298(other->flags110 & 0x600, 0, 0);
        break;
    case 3:
        result = func_00208298(0x600, 0, 0);
        break;
    case 6:
        other = (EffBattleMiscUnit *)func_00169448();
        result = other->fB4 * other->f80;
        break;
    case 7:
        other = (EffBattleMiscUnit *)func_00169450();
        result = other->fB4 * other->f80;
        break;
    }
    return result;
}

/* Unit vector from the queried position to the unit's effect or muzzle position. */
void func_00169F68(EffBattleMiscUnit *unit, EffBattleMiscParam *param, f32 *out) {
    f32 origin[4];

    func_001696D0(unit, param, (u128 *)origin);
    if (unit->flags114 & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, out);
}

typedef struct {
    u8 unk00;
    u8 kind;              /* 0x01 */
    u8 sub;               /* 0x02 */
} EffBattleMiscBasisParam;

extern u8 D_0037F680[];
extern u8 D_0037F690[];
extern u8 D_0037F6A0[];
extern f32 D_003AF1D8[];
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void func_003363D0(void);
extern void func_003364B8(f32 value);
extern void func_00336818(f32 value);
extern void func_00336B00(void);

/* Build a 4x4 basis matrix into `out`: look-at for kinds 4 / sub 9, else a table-driven rotation. */
void func_00169FF0(EffBattleMiscBasisParam *param, u128 *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (param->kind == 4 || param->sub == 9) {
        sdfVuBuildLookAtBasis(D_0037F680, D_0037F690, D_0037F6A0);
        func_003363D0();
        VU0_MOVE_VF(vf31, vf0);
    } else {
        if (param->kind == 5 || param->kind == 3 || param->sub == 8 || param->sub == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (param->sub < 9) {
            angle = D_003AF1D8[param->sub];
        }
        func_003364B8(tilt);
        func_00336818(angle);
        func_00336B00();
    }
    VU0_STORE_MATRIX(out);
}

u32 effBattleMiscGetTableEntry(s32 index) {
    return D_003AB050[index];
}

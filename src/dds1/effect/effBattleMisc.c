#include "common.h"
#include "pcp_vu0.h"

extern s32 effBTLFieldColorGetVariantSelector(void);

extern s32 effBTLFieldColorGetOriginalSelector(void);

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

extern void (*D_003528D8[])();

/* Look up the position provider selected by param->unk00; it leaves the vector in vf10. */
void effBattleMiscQueryPosition(void *owner, EffBattleMiscParam *param, u128 *out) {
    D_003528D8[param->unk00](owner, param);
    VU0_STORE_VF(vf10, out);
}

extern u32 effBTLFieldColorGetOverrideSelector(void);
extern u32 effBTLFieldColorGetFinalSelector(void);
extern void func_00161BA0();

void effBattleMiscCallByOwnerA(u32 unused, void *arg) {
    func_00161BA0(effBTLFieldColorGetOverrideSelector(), arg);
}

void func_00161B10(u32 unused, void *arg) {
    func_00161BA0(effBTLFieldColorGetFinalSelector(), arg);
}

extern void btlSetActorEffectParameterOrMuzzlePosition();

void func_00161B40(u32 unused, EffBattleMiscParam *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetOverrideSelector(), param->value);
}

void func_00161B70(u32 unused, EffBattleMiscParam *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetFinalSelector(), param->value);
}

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161BA0);

INCLUDE_ASM(const s32, "effect/effBattleMisc", func_00161E48);

void func_00162028(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)effBTLFieldColorGetOriginalSelector();

    (void)unused;
    func_00161E48(ctx->flags & 0xE00, value);
}

void func_00162058(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    func_00161E48(ctx->flags & 0xE00, value);
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

typedef struct {
    u8 unk00;
    u8 kind;              /* 0x01 */
    u8 sub;               /* 0x02 */
    u8 pad03;
    s32 length;           /* 0x04 zero selects the unit's own extent */
} EffBattleMiscTargetParam;

extern void *dds3GetWorldObject(void);
extern void *dds3GetWorldCameraObject(void *object);
extern void dds3LoadCameraVectorVU(void *object);
extern void effObjFetchInnerFirstVec(void *object);
extern f32 D_00352890[];
extern f32 D_003528A8[];
extern void func_002DD688(f32 angle);

/* Target offset for a unit part; the vector is returned in vf10. */
void effBattleMiscBuildUnitPartOffsetVU(EffBattleMiscUnit *unit, EffBattleMiscTargetParam *param) {
    f32 length = 750.0f;
    f32 height;
    f32 out[4];
    f32 dir[4];
    void *object;
    s32 lengthParam = param->length;
    u32 sub = param->sub;
    u32 kind = param->kind;

    if (lengthParam != 0) {
        length = (f32)lengthParam;
    }
    if (sub == 9 || kind == 4) {
        object = dds3GetWorldCameraObject(dds3GetWorldObject());
        dir[0] = dir[1] = dir[2] = 750.0f;
        dds3LoadCameraVectorVU(object);
        VU0_MOVE_VF(vf11, vf10);
        effObjFetchInnerFirstVec(object);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_LOAD_VF(vf11, dir);
        VU0_MUL(vf10, vf10, vf11);
        return;
    }
    if (kind == 5) {
        if (sub == 8 || sub == 10) {
            height = -1.0f;
            if (lengthParam != 0) {
                height = -length;
            }
        } else {
            height = -1.0f;
        }
    } else {
        height = D_00352890[kind] * 250.0f;
    }
    if (sub == 8 || sub == 10) {
        out[0] = 0;
        out[1] = height;
        out[2] = 0;
    } else {
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = (unit->flags110 & 0x200) ? 1.0f : -1.0f;
        func_002DD688(D_003528A8[sub]);
        VU0_LOAD_VF(vf10, dir);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = length * dir[0];
        out[1] = length * dir[1] + height;
        out[2] = length * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

void effBattleMiscApplyParamByte(u32 owner, EffBattleMiscParam *param) {
    btlSetActorEffectParameterOrMuzzlePosition(owner, param->value);
}

extern f32 func_001F6970(u32 mask, s32 a, s32 b);
extern void btlUnitGetEffectPosVU(void *unit);
extern void btlUnitGetMuzzlePosVU(void *unit);

f32 effBattleMiscQueryScalar(EffBattleMiscUnit *unit, EffBattleMiscParam *param) {
    EffBattleMiscUnit *other;
    f32 result = 0;

    switch (param->unk00) {
    case 0:
        result = unit->fB4 * unit->f80;
        break;
    case 1:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetOriginalSelector();
        result = func_001F6970(other->flags110 & 0x600, 0, 0);
        break;
    case 2:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetVariantSelector();
        result = func_001F6970(other->flags110 & 0x600, 0, 0);
        break;
    case 3:
        result = func_001F6970(0x600, 0, 0);
        break;
    case 6:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetOverrideSelector();
        result = other->fB4 * other->f80;
        break;
    case 7:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetFinalSelector();
        result = other->fB4 * other->f80;
        break;
    }
    return result;
}

/* Unit vector from the queried position to the unit's effect or muzzle position. */
void effBattleMiscDirectionTo(EffBattleMiscUnit *unit, EffBattleMiscParam *param, f32 *out) {
    f32 origin[4];

    effBattleMiscQueryPosition(unit, param, (u128 *)origin);
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

extern u8 D_00324680[];
extern u8 D_00324690[];
extern u8 D_003246A0[];
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void func_002DD608(f32 value);
extern void func_002DD968(f32 value);
extern void sdfMultiplyVuMatrixInPlace(void);

/* Build a 4x4 basis matrix into `out`: look-at for kinds 4 / sub 9, else a table-driven rotation. */
void effBattleMiscBuildBasis(EffBattleMiscBasisParam *param, u128 *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (param->kind == 4 || param->sub == 9) {
        sdfVuBuildLookAtBasis(D_00324680, D_00324690, D_003246A0);
        sdfInvertRigidVuTransform();
        VU0_MOVE_VF(vf31, vf0);
    } else {
        if (param->kind == 5 || param->kind == 3 || param->sub == 8 || param->sub == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (param->sub < 9) {
            angle = D_003528A8[param->sub];
        }
        func_002DD608(tilt);
        func_002DD968(angle);
        sdfMultiplyVuMatrixInPlace();
    }
    VU0_STORE_MATRIX(out);
}

u32 effBattleMiscGetTableEntry(s32 index) {
    return D_0034E720[index];
}

#include "common.h"
#include "pcp_vu0.h"

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOriginalSelector(void);

/* Flag word read by the two wrappers below. */
typedef struct {
    u8  pad_0x000[0x110]; /* 0x00 */
    u32 flags;            /* 0x110: selected by mask 0xE00 */
} EffBattleMiscCtx; /* 0x114 */

typedef struct {
    u8 querySelector; /* 0x00 */
    u8 value; /* 0x01 */
} EffBattleMiscParam;

extern u32 D_003AB050[];

extern void (*D_003AF208[])();
typedef struct {
    u8 pad00[0x70];
    f32 orientation[4];
    f32 f80;              /* 0x80 */
    u8 pad84[0x2C];
    f32 height;           /* 0xB0 */
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
extern f32 D_003AF1C0[];
extern f32 D_003AF1D8[];
extern void func_00336538(f32 angle);
extern void btlUnitGetEffectPosVU(void *unit);
extern void btlUnitGetMuzzlePosVU(void *unit);
extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *origin, void *direction, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void func_003364B8(f32 value);
extern void func_00336818(f32 value);
extern void sdfMultiplyVuMatrixInPlace(void);
extern u32 effFieldColorFlags;
extern void effMiscQuaternionToMatrixVU(void);
extern void sdfComposeVuMatrixFromRegisters(void);

/* Look up the position provider selected by param->querySelector; it leaves the vector in vf10. */
void effBattleMiscQueryPosition(void *owner, EffBattleMiscParam *param, u128 *out) {
    D_003AF208[param->querySelector](owner, param);
    VU0_STORE_VF(vf10, out);
}

extern u32 effBTLFieldColorGetOverrideSelector(void);
extern u32 effBTLFieldColorGetFinalSelector(void);
extern void effBattleComputeTargetPosition();
extern f32 func_00208298(u32 mask, s32 a, s32 b);
extern f32 btlGetMaxUnitTop(u32 mask);
extern f32 btlGetExtremeUnitY(u32 mask);

void effBattleMiscCallByOwnerA(u32 unused, void *arg) {
    effBattleComputeTargetPosition(effBTLFieldColorGetOverrideSelector(), arg);
}

void func_00169740(u32 unused, void *arg) {
    effBattleComputeTargetPosition(effBTLFieldColorGetFinalSelector(), arg);
}

extern void btlSetActorEffectParameterOrMuzzlePosition();

void effBattleMiscQueryOverrideAttachment(u32 unused, EffBattleMiscParam *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetOverrideSelector(), param->value);
}

void effBattleMiscQueryFinalAttachment(u32 unused, EffBattleMiscParam *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetFinalSelector(), param->value);
}

/* Query a unit-relative target position; camera-facing kinds use the view basis. */
void effBattleComputeTargetPosition(EffBattleMiscUnit *unit, EffBattleMiscTargetParam *param) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 length;
    f32 halfHeight;
    f32 height;
    u32 kind = param->kind;
    u32 sub = param->sub;

    if (effFieldColorFlags & 4) {
        if (unit->flags110 & 0x400) {
            if (kind == 5) {
                kind = 0;
            }
        }
    }
    if (param->length == 0) {
        length = unit->fB4 * unit->f80;
    } else {
        length = (f32)param->length;
    }
    halfHeight = unit->height * unit->f80 * 0.5f;
    if (unit->flags114 & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, pos);
    if (kind == 5) {
        if (sub == 8 || sub == 10) {
            height = -1.0f;
            if (param->length != 0) {
                height = -length;
            }
        } else {
            height = -1.0f;
        }
    } else {
        height = pos[1] - D_003AF1C0[kind] * halfHeight;
        if (sub == 8 || sub == 10) {
            if (param->length != 0) {
                height -= length;
            }
        }
    }
    if (sub == 9 || kind == 4) {
        dir[2] = halfHeight < length ? -length : -halfHeight;
        dir[0] = dir[1] = 0.0f;
        pos[1] = height;
        sdfVuBuildLookAtBasis(pos, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, pos);
        VU0_ADD(vf10, vf10, vf11);
    } else {
        if (sub == 8 || sub == 10) {
            out[0] = pos[0];
            out[1] = height;
            out[2] = pos[2];
        } else {
            dir[0] = 0;
            dir[2] = 1.0f;
            dir[1] = 0;
            VU0_LOAD_VF(vf10, unit->orientation);
            effMiscQuaternionToMatrixVU();
            func_00336818(D_003AF1D8[sub]);
            sdfComposeVuMatrixFromRegisters();
            VU0_LOAD_VF(vf10, dir);
            VU0_ROTATE_VEC(vf10, vf10);
            VU0_STORE_VF(vf10, dir);
            out[0] = pos[0] + length * dir[0];
            out[1] = height + length * dir[1];
            out[2] = pos[2] + length * dir[2];
        }
        VU0_LOAD_VF(vf10, out);
    }
}

void func_00169A78(u32 flags, u32 targetParam) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 vertical;
    f32 length;
    f32 extremeY;
    EffBattleMiscTargetParam *param = (EffBattleMiscTargetParam *)targetParam;
    u32 kind = param->kind;
    u32 sub = param->sub;

    func_00208298(flags, 0, 0);
    VU0_STORE_VF(vf10, pos);
    length = 500.0f;
    vertical = -btlGetMaxUnitTop(flags);
    extremeY = btlGetExtremeUnitY(flags);
    if (param->length != 0) {
        length = (f32)param->length;
    }

    switch (kind) {
    case 5:
        if (sub == 8 || sub == 10) {
            vertical = -1.0f;
            if (param->length != 0) {
                vertical = -length;
            }
        } else {
            vertical = -1.0f;
        }
        break;
    case 0:
        vertical = pos[1];
        break;
    case 1:
        vertical = pos[1];
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        vertical = 0.0f;
        break;
    default:
        vertical = 0.0f;
        break;
    }

    if (sub == 8) {
        out[0] = pos[0];
        out[1] = vertical;
        out[2] = extremeY;
    } else if (sub == 10) {
        out[0] = pos[0];
        out[1] = vertical;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        if (flags & 0x200) {
            dir[2] = 1.0f;
        } else {
            dir[2] = -1.0f;
        }
        func_00336538(D_003AF1D8[sub]);
        VU0_LOAD_VF(vf10, dir);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = pos[0] + length * dir[0];
        out[1] = vertical + length * dir[1];
        out[2] = extremeY + length * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

void func_00169C58(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)effBTLFieldColorGetOriginalSelector();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}

void func_00169C88(u32 unused, u32 value) {
    EffBattleMiscCtx *ctx = (EffBattleMiscCtx *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}


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
        height = D_003AF1C0[kind] * 250.0f;
    }
    if (sub == 8 || sub == 10) {
        out[0] = 0;
        out[1] = height;
        out[2] = 0;
    } else {
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = (unit->flags110 & 0x200) ? 1.0f : -1.0f;
        func_00336538(D_003AF1D8[sub]);
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

f32 effBattleMiscQueryScalar(EffBattleMiscUnit *unit, EffBattleMiscParam *param) {
    EffBattleMiscUnit *other;
    f32 result = 0;

    switch (param->querySelector) {
    case 0:
        result = unit->fB4 * unit->f80;
        break;
    case 1:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetOriginalSelector();
        result = func_00208298(other->flags110 & 0x600, 0, 0);
        break;
    case 2:
        other = (EffBattleMiscUnit *)effBTLFieldColorGetVariantSelector();
        result = func_00208298(other->flags110 & 0x600, 0, 0);
        break;
    case 3:
        result = func_00208298(0x600, 0, 0);
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


/* Build a 4x4 basis matrix into `out`: look-at for kinds 4 / sub 9, else a table-driven rotation. */
void effBattleMiscBuildBasis(EffBattleMiscBasisParam *param, u128 *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (param->kind == 4 || param->sub == 9) {
        sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
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
        sdfMultiplyVuMatrixInPlace();
    }
    VU0_STORE_MATRIX(out);
}

u32 effBattleMiscGetTableEntry(s32 index) {
    return D_003AB050[index];
}

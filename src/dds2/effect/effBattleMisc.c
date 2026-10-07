#include "common.h"
#include "pcp_vu0.h"
#include "btl.h"
#include "eff.h"

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOriginalSelector(void);


extern u32 D_003AB050[];

extern void (*D_003AF208[])();

extern void *dds3GetWorldObject(void);
struct EffWorldNode;
extern struct EffWorldNode *dds3GetWorldCameraObject(struct EffWorldNode *object);
extern void dds3LoadCameraVectorVU(struct EffWorldNode *object);
extern void effObjFetchInnerFirstVec(void *object);
extern f32 D_003AF1C0[];
extern f32 D_003AF1D8[];
extern void func_00336538(f32 angle);
extern void btlUnitGetEffectPosVU(void *unit);
extern void btlUnitGetMuzzlePosVU(void *unit);
extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *target, void *origin, void *up);
extern void sdfInvertRigidVuTransform(void);
extern void func_003364B8(f32 value);
extern void func_00336818(f32 value);
extern void sdfMultiplyVuMatrixInPlace(void);
extern u32 effFieldColorFlags;
extern void effMiscQuaternionToMatrixVU(void);
extern void sdfComposeVuMatrixFromRegisters(void);

/* The selected position provider leaves its vector in vf10. */
void effBattleMiscQueryPosition(void *owner, EffectVectorRequest *param, u128 *out) {
    D_003AF208[param->kind](owner, param);
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

void effBattleMiscQueryOverrideAttachment(u32 unused, EffectVectorRequest *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetOverrideSelector(), param->count);
}

void effBattleMiscQueryFinalAttachment(u32 unused, EffectVectorRequest *param) {
    btlSetActorEffectParameterOrMuzzlePosition(effBTLFieldColorGetFinalSelector(), param->count);
}

/* Query a unit-relative target position; camera-facing kinds use the view basis. */
void effBattleComputeTargetPosition(BtlUnit *unit, EffectVectorRequest *param) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 length;
    f32 halfHeight;
    f32 height;
    u32 kind = param->count;
    u32 sub = param->size;

    if (effFieldColorFlags & 4) {
        if (unit->flags & 0x400) {
            if (kind == 5) {
                kind = 0;
            }
        }
    }
    if (param->unk04 == 0) {
        length = unit->reach * unit->scale;
    } else {
        length = (f32)param->unk04;
    }
    halfHeight = unit->height * unit->scale * 0.5f;
    if (unit->stateFlags & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, pos);
    if (kind == 5) {
        if (sub == 8 || sub == 10) {
            height = -1.0f;
            if (param->unk04 != 0) {
                height = -length;
            }
        } else {
            height = -1.0f;
        }
    } else {
        height = pos[1] - D_003AF1C0[kind] * halfHeight;
        if (sub == 8 || sub == 10) {
            if (param->unk04 != 0) {
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

void func_00169A78(u32 flags, EffectVectorRequest *param) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 vertical;
    f32 length;
    f32 extremeY;
    u32 kind = param->count;
    u32 sub = param->size;

    func_00208298(flags, 0, 0);
    VU0_STORE_VF(vf10, pos);
    length = 500.0f;
    vertical = -btlGetMaxUnitTop(flags);
    extremeY = btlGetExtremeUnitY(flags);
    if (param->unk04 != 0) {
        length = (f32)param->unk04;
    }

    switch (kind) {
    case 5:
        if (sub == 8 || sub == 10) {
            vertical = -1.0f;
            if (param->unk04 != 0) {
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

void func_00169C58(u32 unused, EffectVectorRequest *value) {
    BtlUnit *ctx = (BtlUnit *)effBTLFieldColorGetOriginalSelector();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}

void func_00169C88(u32 unused, EffectVectorRequest *value) {
    BtlUnit *ctx = (BtlUnit *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    func_00169A78(ctx->flags & 0xE00, value);
}


/* Target offset for a unit part; the vector is returned in vf10. */
void effBattleMiscBuildUnitPartOffsetVU(BtlUnit *unit, EffectVectorRequest *param) {
    f32 length = 750.0f;
    f32 height;
    f32 out[4];
    f32 dir[4];
    struct EffWorldNode *camera;
    s32 lengthParam = param->unk04;
    u32 sub = param->size;
    u32 kind = param->count;

    if (lengthParam != 0) {
        length = (f32)lengthParam;
    }
    if (sub == 9 || kind == 4) {
        camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        dir[0] = dir[1] = dir[2] = 750.0f;
        dds3LoadCameraVectorVU(camera);
        VU0_MOVE_VF(vf11, vf10);
        effObjFetchInnerFirstVec(camera);
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
        dir[2] = (unit->flags & 0x200) ? 1.0f : -1.0f;
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

void effBattleMiscApplyParamByte(u32 owner, EffectVectorRequest *param) {
    btlSetActorEffectParameterOrMuzzlePosition(owner, param->count);
}

f32 effBattleMiscQueryScalar(BtlUnit *unit, EffectVectorRequest *param) {
    BtlUnit *other;
    f32 result = 0;

    switch (param->kind) {
    case 0:
        result = unit->reach * unit->scale;
        break;
    case 1:
        other = (BtlUnit *)effBTLFieldColorGetOriginalSelector();
        result = func_00208298(other->flags & 0x600, 0, 0);
        break;
    case 2:
        other = (BtlUnit *)effBTLFieldColorGetVariantSelector();
        result = func_00208298(other->flags & 0x600, 0, 0);
        break;
    case 3:
        result = func_00208298(0x600, 0, 0);
        break;
    case 6:
        other = (BtlUnit *)effBTLFieldColorGetOverrideSelector();
        result = other->reach * other->scale;
        break;
    case 7:
        other = (BtlUnit *)effBTLFieldColorGetFinalSelector();
        result = other->reach * other->scale;
        break;
    }
    return result;
}

/* Unit vector from the queried position to the unit's effect or muzzle position. */
void effBattleMiscDirectionTo(BtlUnit *unit, EffectVectorRequest *param, f32 *out) {
    f32 origin[4];

    effBattleMiscQueryPosition(unit, param, (u128 *)origin);
    if (unit->stateFlags & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, out);
}



/* Build a 4x4 basis matrix into `out`: look-at for kinds 4 / sub 9, else a table-driven rotation. */
void effBattleMiscBuildBasis(EffectVectorRequest *param, u128 *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (param->count == 4 || param->size == 9) {
        sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
        VU0_MOVE_VF(vf31, vf0);
    } else {
        if (param->count == 5 || param->count == 3 || param->size == 8 || param->size == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (param->size < 9) {
            angle = D_003AF1D8[param->size];
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

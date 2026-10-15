#include "common.h"
#include "sdf_vu_rotation_axes.h"
#include "dds3obj.h"
#include "btl_effect_position.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "btl.h"
#include "eff.h"
#include "eff_field_color.h"
#include "sdf_projection.h"

extern u32 effBTLFieldColorGetVariantSelector(void);

extern u32 effBTLFieldColorGetOriginalSelector(void);


extern u32 D_003AB050[];

extern void (*effBattlePositionCallbacks[])();

#include "dds3_world_primary.h"
struct EffWorldNode;
extern void dds3LoadCameraVectorVU(struct EffWorldNode *object);

extern f32 effBattleAnchorHeightFactors[];
extern f32 effBattleDirectionAngles[];
extern u8 sdfViewEyeVector[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
/* The selected position provider leaves its vector in vf10. */
void effBattleMiscQueryPosition(void *owner, EffectVectorRequest *request, u128 *out) {
    effBattlePositionCallbacks[request->kind](owner, request);
    VU0_STORE_VF(vf10, out);
}

extern u32 effBTLFieldColorGetOverrideSelector(void);
extern u32 effBTLFieldColorGetFinalSelector(void);
extern void effBattleComputeTargetPosition();
extern f32 btlMeasureEffectTargetGroupBounds(u32 mask, s32 a, s32 b);
extern f32 btlGetMaxUnitTop(u32 mask);
extern f32 btlGetExtremeUnitY(u32 mask);

void effBattleMiscCallByOwnerA(u32 unused, void *request) {
    effBattleComputeTargetPosition(effBTLFieldColorGetOverrideSelector(), request);
}

void effBattleMiscCallByFinalSelector(u32 unused, void *request) {
    effBattleComputeTargetPosition(effBTLFieldColorGetFinalSelector(), request);
}

void effBattleMiscQueryOverrideAttachment(u32 unused, EffectVectorRequest *request) {
    btlSetActorEffectParameterOrMuzzlePosition((BtlUnit *)effBTLFieldColorGetOverrideSelector(), request->count);
}

void effBattleMiscQueryFinalAttachment(u32 unused, EffectVectorRequest *request) {
    btlSetActorEffectParameterOrMuzzlePosition((BtlUnit *)effBTLFieldColorGetFinalSelector(), request->count);
}

/* Query a unit-relative target position; camera-facing kinds use the view basis. */
void effBattleComputeTargetPosition(BtlUnit *unit, EffectVectorRequest *request) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 length;
    f32 halfHeight;
    f32 height;
    u32 anchorKind = request->count;
    u32 directionMode = request->directionMode;

    if (effFieldColorFlags & 4) {
        if (unit->status.flags & 0x400) {
            if (anchorKind == 5) {
                anchorKind = 0;
            }
        }
    }
    if (request->extentOverride == 0) {
        length = unit->reach * unit->scale;
    } else {
        length = (f32)request->extentOverride;
    }
    halfHeight = unit->height * unit->scale * 0.5f;
    if (unit->status.stateFlags & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_STORE_VF(vf10, pos);
    if (anchorKind == 5) {
        if (directionMode == 8 || directionMode == 10) {
            height = -1.0f;
            if (request->extentOverride != 0) {
                height = -length;
            }
        } else {
            height = -1.0f;
        }
    } else {
        height = pos[1] - effBattleAnchorHeightFactors[anchorKind] * halfHeight;
        if (directionMode == 8 || directionMode == 10) {
            if (request->extentOverride != 0) {
                height -= length;
            }
        }
    }
    if (directionMode == 9 || anchorKind == 4) {
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
        if (directionMode == 8 || directionMode == 10) {
            out[0] = pos[0];
            out[1] = height;
            out[2] = pos[2];
        } else {
            dir[0] = 0;
            dir[2] = 1.0f;
            dir[1] = 0;
            VU0_LOAD_VF(vf10, unit->orientation);
            effMiscQuaternionToMatrixVU();
            sdfVuLoadSecondaryRotationMatrixY(effBattleDirectionAngles[directionMode]);
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

void effBattleMiscQueryMaskedTargetPosition(u32 actorFlagsMask, EffectVectorRequest *request) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 vertical;
    f32 length;
    f32 extremeY;
    u32 anchorKind = request->count;
    u32 directionMode = request->directionMode;

    btlMeasureEffectTargetGroupBounds(actorFlagsMask, 0, 0);
    VU0_STORE_VF(vf10, pos);
    length = 500.0f;
    vertical = -btlGetMaxUnitTop(actorFlagsMask);
    extremeY = btlGetExtremeUnitY(actorFlagsMask);
    if (request->extentOverride != 0) {
        length = (f32)request->extentOverride;
    }

    switch (anchorKind) {
    case 5:
        if (directionMode == 8 || directionMode == 10) {
            vertical = -1.0f;
            if (request->extentOverride != 0) {
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

    if (directionMode == 8) {
        out[0] = pos[0];
        out[1] = vertical;
        out[2] = extremeY;
    } else if (directionMode == 10) {
        out[0] = pos[0];
        out[1] = vertical;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        if (actorFlagsMask & 0x200) {
            dir[2] = 1.0f;
        } else {
            dir[2] = -1.0f;
        }
        sdfVuLoadRotationMatrixY(effBattleDirectionAngles[directionMode]);
        VU0_LOAD_VF(vf10, dir);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = pos[0] + length * dir[0];
        out[1] = vertical + length * dir[1];
        out[2] = extremeY + length * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

void effBattleMiscQueryOriginalTargetPosition(u32 unused, EffectVectorRequest *request) {
    BtlUnit *ctx = (BtlUnit *)effBTLFieldColorGetOriginalSelector();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}

void effBattleMiscQueryVariantTargetPosition(u32 unused, EffectVectorRequest *request) {
    BtlUnit *ctx = (BtlUnit *)effBTLFieldColorGetVariantSelector();

    (void)unused;
    effBattleMiscQueryMaskedTargetPosition(ctx->status.flags & 0xE00, request);
}


/* Target offset for a unit part; the vector is returned in vf10. */
void effBattleMiscBuildUnitPartOffsetVU(BtlUnit *unit, EffectVectorRequest *request) {
    f32 length = 750.0f;
    f32 height;
    f32 out[4];
    f32 dir[4];
    struct EffWorldNode *camera;
    s32 extentOverride = request->extentOverride;
    u32 directionMode = request->directionMode;
    u32 anchorKind = request->count;

    if (extentOverride != 0) {
        length = (f32)extentOverride;
    }
    if (directionMode == 9 || anchorKind == 4) {
        camera = dds3GetWorldCameraObject(dds3GetWorldObject());
        dir[0] = dir[1] = dir[2] = 750.0f;
        dds3LoadCameraVectorVU(camera);
        VU0_MOVE_VF(vf11, vf10);
        effObjFetchInnerPosition(camera);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_LOAD_VF(vf11, dir);
        VU0_MUL(vf10, vf10, vf11);
        return;
    }
    if (anchorKind == 5) {
        if (directionMode == 8 || directionMode == 10) {
            height = -1.0f;
            if (extentOverride != 0) {
                height = -length;
            }
        } else {
            height = -1.0f;
        }
    } else {
        height = effBattleAnchorHeightFactors[anchorKind] * 250.0f;
    }
    if (directionMode == 8 || directionMode == 10) {
        out[0] = 0;
        out[1] = height;
        out[2] = 0;
    } else {
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = (unit->status.flags & 0x200) ? 1.0f : -1.0f;
        sdfVuLoadRotationMatrixY(effBattleDirectionAngles[directionMode]);
        VU0_LOAD_VF(vf10, dir);
        VU0_ROTATE_VEC(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = length * dir[0];
        out[1] = length * dir[1] + height;
        out[2] = length * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

void effBattleMiscApplyParamByte(u32 owner, EffectVectorRequest *request) {
    btlSetActorEffectParameterOrMuzzlePosition((BtlUnit *)owner, request->count);
}

f32 effBattleMiscQueryScalar(BtlUnit *unit, EffectVectorRequest *request) {
    BtlUnit *other;
    f32 result = 0;

    switch (request->kind) {
    case 0:
        result = unit->reach * unit->scale;
        break;
    case 1:
        other = (BtlUnit *)effBTLFieldColorGetOriginalSelector();
        result = btlMeasureEffectTargetGroupBounds(other->status.flags & 0x600, 0, 0);
        break;
    case 2:
        other = (BtlUnit *)effBTLFieldColorGetVariantSelector();
        result = btlMeasureEffectTargetGroupBounds(other->status.flags & 0x600, 0, 0);
        break;
    case 3:
        result = btlMeasureEffectTargetGroupBounds(0x600, 0, 0);
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
void effBattleMiscDirectionTo(BtlUnit *unit, EffectVectorRequest *request, f32 *out) {
    f32 origin[4];

    effBattleMiscQueryPosition(unit, request, (u128 *)origin);
    if (unit->status.stateFlags & 0x8000) {
        btlUnitGetEffectPosVU(unit);
    } else {
        btlUnitGetMuzzlePosVU(unit);
    }
    VU0_LOAD_VF(vf11, origin);
    VU0_SUB(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, out);
}



/* Build a 4x4 basis matrix into `out`: look-at for anchor kind 4 / direction mode 9, else a table-driven rotation. */
void effBattleMiscBuildBasis(EffectVectorRequest *request, u128 *out) {
    f32 angle = 0;
    f32 tilt = 0;

    if (request->count == 4 || request->directionMode == 9) {
        sdfVuBuildLookAtBasis(sdfViewEyeVector, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
        VU0_MOVE_VF(vf31, vf0);
    } else {
        if (request->count == 5 || request->count == 3 || request->directionMode == 8 || request->directionMode == 10) {
            tilt = 3.14159265f / 2.0f;
        }
        if (request->directionMode < 9) {
            angle = effBattleDirectionAngles[request->directionMode];
        }
        sdfVuLoadRotationMatrixX(tilt);
        sdfVuLoadSecondaryRotationMatrixY(angle);
        sdfMultiplyVuMatrixInPlace();
    }
    VU0_STORE_MATRIX(out);
}

u32 effBattleMiscGetTableEntry(s32 index) {
    return D_003AB050[index];
}

#include "common.h"
#include "btl_special_entry_object_class.h"
#include "sdf_vu0.h"
#include "sdf_chip.h"
#include "btl_sound.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff_param.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1
};



extern char D_00414478[];
#include "sdf_debug_api.h"
extern f32 D_003AF1A0[4];
extern f32 D_003AF190[4];




/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
void effBTLCutDelete(effBTLCutHandle_t *owner) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_ENTRY_CAPACITY; workIndex++) {
        if (owner->parameterWorks[workIndex] != 0) {
            dds3EffectPartDelete(owner->parameterWorks[workIndex]);
        }
    }
    effBTLCutListRelease(owner);
    owner->state->active--;
    printf(D_00414478, owner->kind, owner->state,
                  owner->state->active);
    sdfChipFreeDirect(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBTLCutGetCutNumber(effBTLCutHandle_t *effect) {
    return effect->kind;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBTLCutGetFrame(effBTLCutHandle_t *effect) {
    return effect->currentFrame;
}

/* Read one caller-provided word without interpreting it. */
u32 effBTLCutGetFormation(u32 *value) {
    return *value;
}

/* Store the callback; native code invokes it with this work at the source frame. */
void effBTLCutSetMotionChangeCallbackFunc(effBTLCutHandle_t *effect, BattleEffectCallback callback) {
    effect->sourceFrameCallback = callback;
}

/* Store the callback used when the current frame reaches triggerFrame. */
void effBTLCutSetCutEndCallbackFunc(effBTLCutHandle_t *effect, BattleEffectCallback callback) {
    effect->triggerFrameCallback = callback;
}

/* Read the frame used by the source-frame callback equality test. */
s32 effBTLCutGetMotionChangeFrame(effBTLCutHandle_t *effect) {
    return effect->list->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBTLCutSetCutEndFrame(effBTLCutHandle_t *effect, s32 triggerFrame) {
    effect->triggerFrame = triggerFrame;
    if (effect->kind == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        effect->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBTLCutGetCutEndFrame(effBTLCutHandle_t *effect) {
    return effect->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBTLCutSetDrawEndFrame(effBTLCutHandle_t *effect, u32 endFrame) {
    if (effect->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (endFrame < effect->colorRampEndFrame) {
            effect->colorRampEndFrame = endFrame;
        }
        return;
    }
    effect->colorRampEndFrame = endFrame;
}

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBTLCutGetDrawEndFrame(effBTLCutHandle_t *effect) {
    return effect->colorRampEndFrame;
}

/* Store the opaque caller word without assigning it a stronger semantic role. */
void effBTLCutSetUserData(effBTLCutHandle_t *effect, u32 value) {
    effect->unk120 = value;
}

/* Return the same opaque caller word. */
u32 effBTLCutGetUserData(effBTLCutHandle_t *effect) {
    return effect->unk120;
}

/* Blend toward the neutral tint over the final six frames of the ramp. */
u32 effBTLCutGetAllPartColor(u32 elapsed, u32 duration)
{
    f32 ratio;
    u32 color[4];

    if (duration == 0 || elapsed >= duration) {
        return 0x808080;
    }
    if (duration < 6) {
        ratio = (f32)(duration - elapsed) / (f32)duration;
    } else {
        ratio = 1.0f;
        if (elapsed > duration - 6) {
            ratio = (f32)(duration - elapsed) / 6.0f;
        }
    }
    SDF_VU0_LOADVECTOR(D_003AF1A0);
    SDF_VU0_LOADVECTOR_B(D_003AF190);
    VU0_SCALAR_OP(1.0f - ratio, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3(ratio, "vmulx.xyzw vf11, vf11, vf2x");
    SDF_VU0_ADDVECTOR();
    EE_MMI_RGBA_PACK(color[0]);
    return color[0];
}

#include "btl.h"
#include "eff.h"
#include "mdl.h"
#include "evt_unit.h"
#include "eff_battle_color_keyframe.h"
extern f32 D_004144A0[4]; extern f32 D_004144B0[4]; extern f32 D_004144C0[4];
extern BtlSpecialEntryObjectClass btlClassifySpecialEntryObject(BtlUnit *entry);
extern void effMiscQuatMultiplyVU(void); extern void effMiscQuaternionToMatrixVU(void);
extern void sdfComposeVuMatrixFromRegisters(void);
extern void effBTLPartGetPositionPtr(BtlUnit *owner, EffectVectorRequest *request, u128 *out);
extern void effBTLPartGetRotateMatrixPtr(EffectVectorRequest *request, u128 *out);
extern f32 effBattleMiscQueryScalar(const BtlUnit *unit, const EffectVectorRequest *request);
extern u32 effBTLFieldColorGetOverrideSelector(void); extern u32 effBTLFieldColorGetFinalSelector(void);
extern u32 effMagaCylinderCalcColor(u32, u32); extern u32 effBTLCutGetAllPartColor(u32, u32);
extern void dds3SetQuake(s32, s32); extern void mdlGetScale(dds3ModelHandle_t *);
extern void effBTLModelGetMarkMatrix2(BtlUnit *unit, s32 mode);
extern void effBattleApplyUnitRgbKeyframe(BtlUnit *unit, EffBattleUnitRgbCommand *command, s32 frame);
extern void dds3EffectPartSetPosition(effPartHandle_t *, void *); extern void dds3EffectPartSetRotateMatrix(effPartHandle_t *, void *);
extern void dds3EffectPartSetColor(effPartHandle_t *, u32); extern void dds3EffectPartSetDirection(effPartHandle_t *, void *);
extern void dds3EffectPartSetArea(effPartHandle_t *, f32); extern void dds3EffectPartDraw(effPartHandle_t *);

extern void effBattleApplyUnitColorDirection(BtlUnit *unit, EffBattleUnitColorCommand *command, u32 frame);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144A0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144B0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144C0);

void effBattleUpdate(effBTLCutHandle_t *work) {
    BtlUnit *unit;
    EffBattleEntryList *list;
    EffBattleEntry *entry;
    s32 entryIndex;
    s32 count;
    const u32 frame = work->currentFrame;
    u16 flags = work->flags;
    u32 color;
    f32 localVector[4];
    f32 baseMatrix[4][4];
    f32 overrideMatrix[4][4];
    f32 finalMatrix[4][4];
    f32 alternateMatrix[4][4];
    f32 oppositeMatrix[4][4];
    f32 basisMatrix[4][4];
    BtlUnit *overrideOwner;
    BtlUnit *finalOwner;

    list = work->list;
    unit = work->owner;

    if ((flags & 8) != 0) {
        if ((flags & 2) != 0) {
            effBattleApplyUnitColorDirection(unit, &list->colorCommand, frame);
            flags = work->flags;
        }
        if ((flags & 4) != 0) {
            effBattleApplyUnitRgbKeyframe(unit,
                &list->rgbCommand, frame);
        }

        if (list->shake != 0 && list->shakeFrame == frame) {
            if (list->shake < 0x1F) {
                dds3SetQuake(list->shakeFade, list->shake * 10);
            } else {
                printf("effBattle : shake over[%d]\n", list->shake);
                dds3SetQuake(list->shakeFade, 0x12C);
            }
        }

        SDF_VU0_LOADVECTOR(unit->orientation);
        SDF_VU0_LOADVECTOR_B(D_004144A0);
        effMiscQuatMultiplyVU();
        effMiscQuaternionToMatrixVU();
        VU0_STORE_MATRIX(baseMatrix);

        overrideOwner = (BtlUnit *)effBTLFieldColorGetOverrideSelector();
        if (overrideOwner != NULL) {
            SDF_VU0_LOADVECTOR(overrideOwner->orientation);
            SDF_VU0_LOADVECTOR_B(D_004144A0);
            effMiscQuatMultiplyVU();
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(overrideMatrix);
        } else {
            EE_MMI_UNIT_MATRIX(overrideMatrix);
        }

        finalOwner = (BtlUnit *)effBTLFieldColorGetFinalSelector();
        if (finalOwner != NULL) {
            SDF_VU0_LOADVECTOR(finalOwner->orientation);
            SDF_VU0_LOADVECTOR_B(D_004144A0);
            effMiscQuatMultiplyVU();
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(finalMatrix);
        } else {
            EE_MMI_UNIT_MATRIX(finalMatrix);
        }

        if ((unit->status.flags & 0x200) != 0) {
            SDF_VU0_LOADVECTOR(D_004144B0);
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(alternateMatrix);
            SDF_VU0_LOADVECTOR(D_004144C0);
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(oppositeMatrix);
        } else {
            SDF_VU0_LOADVECTOR(D_004144C0);
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(alternateMatrix);
            SDF_VU0_LOADVECTOR(D_004144B0);
            effMiscQuaternionToMatrixVU();
            VU0_STORE_MATRIX(oppositeMatrix);
        }

        if ((work->flags & 1) == 0) {
            color = effMagaCylinderCalcColor(
                effBTLCutGetAllPartColor(frame, work->colorRampEndFrame), work->color);
        } else {
            color = work->color;
        }

        entry = list->entries;
        count = list->count;
        for (entryIndex = 0; entryIndex < count; entryIndex++, entry++) {
            f32 scalar = 0.0f;
            f32 matrixOutput[4][4];

            if (entry->zeroKindCount >= frame) {
                continue;
            }
            if (entry->kind != 0) {
                u16 eligibilityFlags = unit->effectLink.flags;
                BtlSpecialEntryObjectClass special = btlClassifySpecialEntryObject(unit);
                u16 step = 0xFFFF;
                if (entry->kind == 3) {
                    if ((eligibilityFlags & 2) == 0) {
                        continue;
                    }
                } else {
                    if ((eligibilityFlags & 4) == 0) {
                        continue;
                    }
                    if (entry->kind == 1 && (eligibilityFlags & 1) != 0) {
                        continue;
                    }
                    if (entry->kind == 2 && (eligibilityFlags & 1) == 0) {
                        continue;
                    }
                    if (entry->kind == 4 && (u16)(special + step) < 2) {
                        continue;
                    }
                    if (entry->kind == 5 &&
                        (special == BTL_SPECIAL_ENTRY_CLASS_NONE ||
                         special == BTL_SPECIAL_ENTRY_CLASS_UNIT_ID_CASE)) {
                        continue;
                    }
                }
            }
            if (work->parameterWorks[entryIndex] == NULL) {
                continue;
            }

            if ((work->flags & 0x10) == 0) {
                EffectVectorRequest *request = &entry->aimParams;
                effBTLPartGetPositionPtr(unit, request, (u128 *)localVector);
                dds3EffectPartSetPosition(work->parameterWorks[entryIndex], localVector);
                switch (request->kind) {
                case 0:
                case 4:
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], baseMatrix);
                    break;
                case 6:
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], overrideMatrix);
                    break;
                case 7:
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], finalMatrix);
                    break;
                case 1:
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], alternateMatrix);
                    break;
                case 2:
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], oppositeMatrix);
                    break;
                case 3:
                    break;
                case 5:
                    effBTLModelGetMarkMatrix2(unit, entry->aimParams.count);
                    SDF_VU0_LOADMATRIX_B(baseMatrix);
                    sdfComposeVuMatrixFromRegisters();
                    VU0_STORE_MATRIX_UNCLOBBERED(matrixOutput);
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], matrixOutput);
                    break;
                case 8:
                    if ((unit->status.flags & 2) != 0) {
                        mdlGetScale(unit->ext->owner);
                        SDF_VU0_GETVECTOR_X(scalar);
                    } else {
                        scalar = 0.0f;
                    }
                    effBTLModelGetMarkMatrix2(unit, entry->aimParams.count);
                    if (scalar != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, 1.0f / scalar);
                        SDF_VU0_SCALE_XYZ();
                    }
                    VU0_STORE_MATRIX_UNCLOBBERED(matrixOutput);
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], matrixOutput);
                    break;

                case 9:
                    if ((overrideOwner->status.flags & 2) != 0) {
                        mdlGetScale(overrideOwner->ext->owner);
                        SDF_VU0_GETVECTOR_X(scalar);
                    } else {
                        scalar = 0.0f;
                    }
                    effBTLModelGetMarkMatrix2(overrideOwner, entry->aimParams.count);
                    if (scalar != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, 1.0f / scalar);
                        SDF_VU0_SCALE_XYZ();
                    }
                    VU0_STORE_MATRIX_UNCLOBBERED(matrixOutput);
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], matrixOutput);
                    break;

                case 10:
                    if ((finalOwner->status.flags & 2) != 0) {
                        mdlGetScale(finalOwner->ext->owner);
                        SDF_VU0_GETVECTOR_X(scalar);
                    } else {
                        scalar = 0.0f;
                    }
                    effBTLModelGetMarkMatrix2(finalOwner, entry->aimParams.count);
                    if (scalar != 0.0f) {
                        SDF_VU0_SETSCALE1VECTOR();
                        VU0_SCALE_VF(vf10, 1.0f / scalar);
                        SDF_VU0_SCALE_XYZ();
                    }
                    VU0_STORE_MATRIX_UNCLOBBERED(matrixOutput);
                    dds3EffectPartSetRotateMatrix(work->parameterWorks[entryIndex], matrixOutput);
                    break;
                default:
                    break;
                }
                effBTLPartGetRotateMatrixPtr(request, (u128 *)basisMatrix);
                dds3EffectPartSetDirection(work->parameterWorks[entryIndex], basisMatrix);
                scalar = effBattleMiscQueryScalar(unit, request);
                dds3EffectPartSetArea(work->parameterWorks[entryIndex], scalar);
                dds3EffectPartSetColor(work->parameterWorks[entryIndex], color);
            }
            dds3EffectPartDraw(work->parameterWorks[entryIndex]);
        }
    }

    if (list->callbackFrame == frame && work->sourceFrameCallback != 0) {
        ((void (*)(effBTLCutHandle_t *))work->sourceFrameCallback)(work);
    }
    if (work->triggerFrame == frame && work->triggerFrameCallback != 0) {
        ((void (*)(effBTLCutHandle_t *))work->triggerFrameCallback)(work);
    }
    work->currentFrame++;
}

void effBTLCutSetScale(effBTLCutHandle_t *effect, f32 scale) {
    s32 i;
    s32 count;

    if (scale != 1.0f) {
        count = effect->list->count;
        for (i = 0; i < count; i++) {
            dds3EffectPartSetScale(effect->parameterWorks[i], scale);
        }
    }
}

/* True when any entry is of kind 2 or 3. */
s32 effBattleHasActiveKind(effBTLCutHandle_t *owner) {
    EffBattleEntryList *list = owner->list;
    s32 count = list->count;
    EffBattleEntry *entry = list->entries;
    s32 i;

    for (i = 0; i < count; i++, entry++) {
        if (entry->kind == 3) {
            return 1;
        }
        if (entry->kind == 2) {
            return 1;
        }
    }
    return 0;
}

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void effBTLPartCreate(SoundMixer *destination, const SoundMixer *source) {
    const u8 *sourceBytes = (const u8 *)source;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        EffBattleEntry *entry = destination->banks[bankIndex].list.entries;
        u16 entryCountForBank = destination->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->parameterKind != EFF_BATTLE_PARAMETER_KIND_NO_WORK) {
                    if (entry->parameterKind == EFF_BATTLE_PARAMETER_KIND_COPY_SOURCE_ENTRY) {
                        u32 copySourceIndex = entry->value.copySourceIndex;
                        u32 sourceBankIndex = copySourceIndex / EFF_BATTLE_PARAMETER_ENTRY_CAPACITY;
                        u32 sourceEntryIndex = copySourceIndex % EFF_BATTLE_PARAMETER_ENTRY_CAPACITY;
                        EffBattleEntry *copySourceEntry =
                            &destination->banks[sourceBankIndex].list.entries[sourceEntryIndex];
                        effPartHandle_t *duplicatedWork = dds3EffectPartCopy(copySourceEntry->value.work);
                        EffBattleParameterKind sourceKind = copySourceEntry->parameterKind;

                        entry->value.work = duplicatedWork;
                        entry->parameterKind = sourceKind;
                    } else {
                        entry->value.work = dds3EffectPartCreate(
                            entry->parameterKind, sourceBytes + entry->value.mixerOffset);
                    }
                    if (entry->parameterKind == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
                        entry->zeroKindCount++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
    }
}

/* Release each initialized parameter-work entry in the mixer's two banks. */
void effBTLPartDelete(SoundMixer *mixer) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = mixer->banks[bankIndex].list.count;
        EffBattleEntry *entry = mixer->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->parameterKind != EFF_BATTLE_PARAMETER_KIND_NO_WORK) {
                    dds3EffectPartDelete(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}

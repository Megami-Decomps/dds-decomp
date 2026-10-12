#include "common.h"
#include "sdf_chip.h"
#include "btl_sound.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff_param.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 0x3C
};




extern char D_003A0DB8[];
extern void func_003003F0(const char *, ...);
extern f32 D_00352870[4];
extern f32 D_00352860[4];



/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
void effReleaseBattleVoiceOwner(BattleEffect *owner) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_WORK_CAPACITY; workIndex++) {
        if (owner->parameterWorks[workIndex] != 0) {
            effDispatchParameterDataAndFreeWork(owner->parameterWorks[workIndex]);
        }
    }
    sndUnlinkVoice(owner);
    owner->state->active--;
    func_003003F0(D_003A0DB8, owner->kind, owner->state,
                  owner->state->active);
    sdfReleaseChipBlock(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBattleGetMode(BattleEffect *work) {
    return work->kind;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBattleGetCurrentFrame(BattleEffect *work) {
    return work->currentFrame;
}

/* Read one caller-provided word without interpreting it. */
u32 effBattleReadInputWord(u32 *value) {
    return *value;
}

/* Store the callback; native code invokes it with this work at the source frame. */
void effBattleSetSourceFrameCallback(BattleEffect *work, BattleEffectCallback callback) {
    work->sourceFrameCallback = callback;
}

/* Store the callback used when the current frame reaches triggerFrame. */
void effBattleSetTriggerFrameCallback(BattleEffect *work, BattleEffectCallback callback) {
    work->triggerFrameCallback = callback;
}

/* Read the callback frame from the linked source. */
s32 effBattleGetLinkedSourceValue(BattleEffect *work) {
    return work->list->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBattleSetInputValue(BattleEffect *work, s32 triggerFrame) {
    work->triggerFrame = triggerFrame;
    if (work->kind == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        work->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBattleGetInputValue(BattleEffect *work) {
    return work->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBattleUpdateSelectedValue(BattleEffect *work, u32 endFrame) {
    if (work->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (endFrame < work->colorRampEndFrame) {
            work->colorRampEndFrame = endFrame;
        }
        return;
    }
    work->colorRampEndFrame = endFrame;
}

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBattleGetSelectedValue(BattleEffect *work) {
    return work->colorRampEndFrame;
}

/* Store the opaque caller word without assigning it a stronger semantic role. */
void effBattleStoreOpaqueWord(BattleEffect *work, u32 value) {
    work->unk120 = value;
}

/* Return the same opaque caller word. */
u32 effBattleReadOpaqueWord(BattleEffect *work) {
    return work->unk120;
}

/* Blend toward the neutral tint over the final six frames of the ramp. */
u32 effBattleCalcFadeTint(u32 elapsed, u32 duration)
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
    VU0_LOAD_VF(vf10, D_00352870);
    VU0_LOAD_VF(vf11, D_00352860);
    VU0_SCALAR_OP(1.0f - ratio, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3(ratio, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(color[0]);
    return color[0];
}

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DE0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0DF0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_003A0E00);

INCLUDE_ASM(const s32, "effect/effBattle", effBattleUpdate);

void func_00161588(BattleEffect *effect, f32 scale) {
    s32 i;
    s32 count;

    if (scale != 1.0f) {
        count = effect->list->count;
        for (i = 0; i < count; i++) {
            effParamWorkCallback1(effect->parameterWorks[i], scale);
        }
    }
}



/* True when any entry is of kind 2 or 3. */
s32 effBattleHasActiveKind(BattleEffect *owner) {
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
void effBattleRebuildClonedParameterBanks(SoundMixer *destination, SoundMixer *source) {
    u8 *sourceBytes = (u8 *)source;
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        EffBattleEntry *entry = destination->banks[bankIndex].list.entries;
        u16 entryCountForBank = destination->banks[bankIndex].list.count;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->paramKind != 0xFFFE) {
                    if (entry->paramKind == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 sourceBank = index / 0x3C;
                        u32 sourceEntryIndex = index % 0x3C;
                        EffBattleEntry *sourceEntry = &destination->banks[sourceBank].list.entries[sourceEntryIndex];
                        EffParamWork *duplicatedWork = effParamWorkDuplicate(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->paramKind;

                        entry->value.work = duplicatedWork;
                        entry->paramKind = sourceKind;
                    } else {
                        entry->value.work = effParamWorkCreate(entry->paramKind, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->paramKind == EFF_PARAM_WORK_KIND_EFFECT_NODE) {
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
void effBattleReleaseParameterBanks(SoundMixer *mixer) {
    s32 bankIndex;

    for (bankIndex = 0; bankIndex < 2; bankIndex++) {
        u16 entryCount = mixer->banks[bankIndex].list.count;
        EffBattleEntry *entry = mixer->banks[bankIndex].list.entries;

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->paramKind != 0xFFFE) {
                    effDispatchParameterDataAndFreeWork(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}

INCLUDE_SDATA(const s32, "effect/effBattle", effFieldColorFlags);


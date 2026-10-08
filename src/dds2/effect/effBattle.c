#include "common.h"
#include "btl_sound.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 0x3C
};



typedef struct EffParamWork EffParamWork;
extern char D_00414478[];
extern void effDispatchParameterDataAndFreeWork(EffParamWork *);
extern void func_0035B6E0(const char *fmt, ...);
extern void sdfReleaseChipBlock(void *);
extern f32 D_003AF1A0[4];
extern f32 D_003AF190[4];

extern EffParamWork *effParamWorkCreate(u16 kind, void *data);
extern EffParamWork *effParamWorkDuplicate(EffParamWork *work);


typedef struct EffBattleEntry {
    u8 pad00[0x14];
    u8 kind;     /* 0x14 */
    u8 pad15[3];
} EffBattleEntry; /* 0x18 */

typedef struct EffBattleEntryList {
    u8 pad00[0x48];
    u32 callbackFrame; /* 0x48 */
    u8 pad4C[0x20];
    u16 count;            /* 0x6C */
    u8 pad6E[0xA];
    EffBattleEntry entries[1]; /* 0x78 */
} EffBattleEntryList;


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
    func_0035B6E0(D_00414478, owner->kind, owner->state,
                  owner->state->active);
    sdfReleaseChipBlock(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBattleGetMode(BattleEffect *effect) {
    return effect->kind;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBattleGetCurrentFrame(BattleEffect *effect) {
    return effect->currentFrame;
}

/* Read one caller-provided word without interpreting it. */
u32 effBattleReadInputWord(u32 *value) {
    return *value;
}

/* Store a callback address; native code invokes it with this work at the source frame. */
void effBattleSetSourceFrameCallback(BattleEffect *effect, u32 callbackAddress) {
    effect->sourceFrameCallbackAddress = callbackAddress;
}

/* Store the callback address used when the current frame reaches triggerFrame. */
void effBattleSetTriggerFrameCallback(BattleEffect *effect, u32 callbackAddress) {
    effect->triggerFrameCallbackAddress = callbackAddress;
}

/* Read the frame used by the source-frame callback equality test. */
u32 effBattleGetLinkedSourceValue(BattleEffect *effect) {
    return effect->list->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBattleSetInputValue(BattleEffect *effect, s32 triggerFrame) {
    effect->triggerFrame = triggerFrame;
    if (effect->kind == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        effect->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBattleGetInputValue(BattleEffect *effect) {
    return effect->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBattleUpdateSelectedValue(BattleEffect *effect, u32 endFrame) {
    if (effect->kind == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
        if (endFrame < effect->colorRampEndFrame) {
            effect->colorRampEndFrame = endFrame;
        }
        return;
    }
    effect->colorRampEndFrame = endFrame;
}

/* Return the selected endpoint used by native color-ramp calculation. */
u32 effBattleGetSelectedValue(BattleEffect *effect) {
    return effect->colorRampEndFrame;
}

/* Store the opaque caller word without assigning it a stronger semantic role. */
void effBattleStoreOpaqueWord(BattleEffect *effect, u32 value) {
    effect->unk120 = value;
}

/* Return the same opaque caller word. */
u32 effBattleReadOpaqueWord(BattleEffect *effect) {
    return effect->unk120;
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
    VU0_LOAD_VF(vf10, D_003AF1A0);
    VU0_LOAD_VF(vf11, D_003AF190);
    VU0_SCALAR_OP(1.0f - ratio, "vmulx.xyzw vf10, vf10, vf2x");
    VU0_SCALAR_OP_R3(ratio, "vmulx.xyzw vf11, vf11, vf2x");
    VU0_ADD(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(color[0]);
    return color[0];
}

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144A0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144B0);

INCLUDE_RODATA(const s32, "effect/effBattle", D_004144C0);

INCLUDE_ASM(const s32, "effect/effBattle", func_00168978);

INCLUDE_ASM(const s32, "effect/effBattle", func_00169168);

typedef union EffBattleParameterValue {
    u32 sourceOffset;
    EffParamWork *work;
} EffBattleParameterValue;

typedef struct EffBattleParameterEntry {
    u16 kind;
    u8 pad02[2];
    EffBattleParameterValue value;
    u8 pad08[8];
    u32 zeroKindCount; /* 0x10 */
    u32 pad14;
} EffBattleParameterEntry; /* 0x18 */

typedef struct EffBattleParameterBankHeader {
    u8 pad00[0x74];
    u16 entryCount; /* 0x74 */
    u8 pad76[0xA];
} EffBattleParameterBankHeader;

typedef struct EffBattleParameterMixer {
    u64 banks[2][0xC3]; /* Two 0x618-byte banks; each entry uses three qwords. */
} EffBattleParameterMixer;



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
void effBattleRebuildClonedParameterBanks(void *destination, void *source) {
    u8 *destinationBytes = destination;
    u8 *sourceBytes = source;
    u16 *entryCount = &((EffBattleParameterBankHeader *)destination)->entryCount;
    u32 bankOffset = 0;
    s32 remainingBanks = 1;

    do {
        EffBattleParameterEntry *entry = (EffBattleParameterEntry *)(destinationBytes + bankOffset + 0x80);
        u16 entryCountForBank = *entryCount;

        if (entryCountForBank != 0) {
            s32 remainingEntries = entryCountForBank;

            do {
                if (entry->kind != 0xFFFE) {
                    if (entry->kind == 0xFFFF) {
                        u32 index = entry->value.sourceOffset;
                        u32 bankIndex = index / 0x3C;
                        u32 entryIndex = index % 0x3C;
                        EffBattleParameterEntry *sourceEntry = (EffBattleParameterEntry *)
                            &((EffBattleParameterMixer *)destination)
                                 ->banks[bankIndex][0x10 + entryIndex * 3];
                        EffParamWork *duplicatedWork = effParamWorkDuplicate(sourceEntry->value.work);
                        u16 sourceKind = sourceEntry->kind;

                        entry->value.work = duplicatedWork;
                        entry->kind = sourceKind;
                    } else {
                        entry->value.work = effParamWorkCreate(entry->kind, sourceBytes + entry->value.sourceOffset);
                    }
                    if (entry->kind == 0) {
                        entry->zeroKindCount++;
                    }
                }
                entry++;
                remainingEntries--;
            } while (remainingEntries != 0);
        }
        entryCount = (u16 *)((u8 *)entryCount + 0x618);
        bankOffset += 0x618;
        remainingBanks--;
    } while (remainingBanks >= 0);
}

/* Release each initialized parameter-work entry in the mixer's two banks. */
void effBattleReleaseParameterBanks(EffBattleParameterMixer *mixer) {
    u16 *countAddress = &((EffBattleParameterBankHeader *)mixer)->entryCount;
    s32 bankCountdown;
    s32 bankOffset;

    for (bankOffset = 0, bankCountdown = 1; bankCountdown >= 0;
         bankCountdown--,
         countAddress = (u16 *)((u8 *)countAddress + sizeof(mixer->banks[0])),
         bankOffset += sizeof(mixer->banks[0])) {
        u16 entryCount = *countAddress;
        EffBattleParameterEntry *entry = (EffBattleParameterEntry *)((u8 *)mixer + bankOffset + 0x80);

        if (entryCount != 0) {
            s32 remaining = entryCount;

            do {
                if (entry->kind != 0xFFFE) {
                    effDispatchParameterDataAndFreeWork(entry->value.work);
                }
                entry++;
                remaining--;
            } while (remaining != 0);
        }
    }
}

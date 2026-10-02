#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

enum {
    EFF_BATTLE_COPY_TRIGGER_FRAME = 0,
    EFF_BATTLE_MINIMUM_RAMP_END_FRAME = 1,
    EFF_BATTLE_PARAMETER_WORK_CAPACITY = 0x3C
};


typedef struct EffBattleVoiceState {
    u8 pad00[0xC3C];
    s32 activeCount;
} EffBattleVoiceState;

typedef struct EffBattleVoiceOwner {
    u32 unk00;
    EffBattleVoiceState *state;
    u8 pad08[0x14];
    u16 voiceKind;
    u8 pad1E[0xA];
    s32 parameterWorks[EFF_BATTLE_PARAMETER_WORK_CAPACITY]; /* EffParamWork addresses */
} EffBattleVoiceOwner;

extern char D_003A0DB8[];
extern void effDispatchParameterDataAndFreeWork(s32);
extern void func_003003F0(char *, u16, void *, s32);
extern void sndUnlinkVoice(void *);
extern void sdfReleaseChipBlock(void *);
extern f32 D_00352870[4];
extern f32 D_00352860[4];

typedef struct EffParamWork EffParamWork;
extern EffParamWork *effParamWorkCreate(u16 kind, void *data);
extern EffParamWork *effParamWorkDuplicate(EffParamWork *work);

/* Shared work area for the battle-effect helpers in this TU. */
typedef struct BattleEffect {
    u8  pad_0x00[0x10]; /* 0x00 */
    u32 currentFrame;   /* 0x10: incremented after each native update */
    u32 triggerFrame;   /* 0x14: invokes triggerFrameCallbackAddress on equality */
    u32 colorRampEndFrame; /* 0x18: endpoint passed to the native color-ramp helper */
    u16 frameSelectionMode; /* 0x1C: mode 1 keeps the smaller ramp endpoint */
    u8  pad_0x1E[0xFA]; /* 0x1E */
    u32 sourceFrameCallbackAddress; /* 0x118: called when currentFrame equals source +0x48 */
    u32 triggerFrameCallbackAddress; /* 0x11C: called when currentFrame equals triggerFrame */
    u32 unk120;         /* 0x120: caller-controlled word; interpretation not established */
} BattleEffect; /* 0x124 */

/* Release each nonzero parameter-work slot, unlink the owner, then drop its active count.
 * The diagnostic observes the decremented count before the owner is freed. */
void effReleaseBattleVoiceOwner(EffBattleVoiceOwner *owner) {
    s32 workIndex;

    for (workIndex = 0; workIndex < EFF_BATTLE_PARAMETER_WORK_CAPACITY; workIndex++) {
        if (owner->parameterWorks[workIndex] != 0) {
            effDispatchParameterDataAndFreeWork(owner->parameterWorks[workIndex]);
        }
    }
    sndUnlinkVoice(owner);
    owner->state->activeCount--;
    func_003003F0(D_003A0DB8, owner->voiceKind, owner->state,
                  owner->state->activeCount);
    sdfReleaseChipBlock(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBattleGetMode(BattleEffect *work) {
    return work->frameSelectionMode;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBattleGetCurrentFrame(BattleEffect *work) {
    return work->currentFrame;
}

/* Read one caller-provided word without interpreting it. */
u32 func_00160BA0(u32 *value) {
    return *value;
}

/* Store a callback address; native code invokes it with this work at the source frame. */
void effBattleSetSourceFrameCallback(BattleEffect *work, u32 callbackAddress) {
    work->sourceFrameCallbackAddress = callbackAddress;
}

/* Store the callback address used when the current frame reaches triggerFrame. */
void effBattleSetTriggerFrameCallback(BattleEffect *work, u32 callbackAddress) {
    work->triggerFrameCallbackAddress = callbackAddress;
}

/* Read the callback frame from the linked source at +0x24; keep the existing raw view. */
s32 func_00160BB8(u8 *effectBytes) {
    return *(s32 *)(*(u8 **)(effectBytes + 0x24) + 0x48);
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBattleSetInputValue(BattleEffect *work, s32 triggerFrame) {
    work->triggerFrame = triggerFrame;
    if (work->frameSelectionMode == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        work->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBattleGetInputValue(BattleEffect *work) {
    return work->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBattleUpdateSelectedValue(BattleEffect *work, u32 endFrame) {
    if (work->frameSelectionMode == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
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
void func_00160C20(BattleEffect *work, u32 value) {
    work->unk120 = value;
}

/* Return the same opaque caller word. */
u32 func_00160C28(BattleEffect *work) {
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

INCLUDE_ASM(const s32, "effect/effBattle", func_00160D88);

INCLUDE_ASM(const s32, "effect/effBattle", func_00161588);

typedef struct EffBattleEntry {
    u8 pad00[0x14];
    u8 kind;     /* 0x14 */
    u8 pad15[3];
} EffBattleEntry; /* 0x18 */

typedef struct EffBattleEntryList {
    u8 pad00[0x6C];
    u16 count;            /* 0x6C */
    u8 pad6E[0xA];
    EffBattleEntry entries[1]; /* 0x78 */
} EffBattleEntryList;

typedef struct EffBattleListOwner {
    u8 pad00[0x24];
    EffBattleEntryList *list; /* 0x24 */
} EffBattleListOwner;

/* True when any entry is of kind 2 or 3. */
s32 effBattleHasActiveKind(EffBattleListOwner *owner) {
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

/* Rebuild each cloned bank's parameter work, resolving shared-entry references
 * and preserving disabled descriptors. */
void func_00161650(void *destination, void *source) {
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

INCLUDE_ASM(const s32, "effect/effBattle", func_00161790);

INCLUDE_SDATA(const s32, "effect/effBattle", effFieldColorFlags);

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

extern char D_00414478[];
extern void effDispatchParameterDataAndFreeWork(s32);
extern void func_0035B6E0(const char *fmt, ...);
extern void sndUnlinkVoice(void *);
extern void sdfReleaseChipBlock(void *);
extern f32 D_003AF1A0[4];
extern f32 D_003AF190[4];

/* The source frame is compared with currentFrame by the native updater. */
typedef struct BattleEffectValueSource {
    u8 pad00[0x48];
    u32 callbackFrame;
} BattleEffectValueSource;

typedef struct BattleEffect {
    u8 pad0[0x10];
    u32 currentFrame;  /* 0x10: incremented after each native update */
    u32 triggerFrame;  /* 0x14: invokes triggerFrameCallbackAddress on equality */
    u32 colorRampEndFrame; /* 0x18: endpoint passed to the native color-ramp helper */
    u16 frameSelectionMode; /* 0x1C: mode 1 keeps the smaller ramp endpoint */
    u8 pad1E[6];
    BattleEffectValueSource *frameSource; /* 0x24 */
    u8 pad28[0xF0];
    u32 sourceFrameCallbackAddress; /* 0x118: called when currentFrame equals source +0x48 */
    u32 triggerFrameCallbackAddress; /* 0x11C: called when currentFrame equals triggerFrame */
    u32 value120;      /* 0x120: caller-controlled word; interpretation not established */
} BattleEffect;

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
    func_0035B6E0(D_00414478, owner->voiceKind, owner->state,
                  owner->state->activeCount);
    sdfReleaseChipBlock(owner);
}

/* Return the policy controlling trigger-frame copying and minimum ramp selection. */
u16 effBattleGetMode(BattleEffect *effect) {
    return effect->frameSelectionMode;
}

/* Read the frame counter that the native updater advances after callbacks. */
u32 effBattleGetCurrentFrame(BattleEffect *effect) {
    return effect->currentFrame;
}

/* Read one caller-provided word without interpreting it. */
u32 func_00168790(u32 *value) {
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
    return effect->frameSource->callbackFrame;
}

/* Set the trigger frame; mode 0 also copies its stored bits into the color-ramp endpoint. */
void effBattleSetInputValue(BattleEffect *effect, s32 triggerFrame) {
    effect->triggerFrame = triggerFrame;
    if (effect->frameSelectionMode == EFF_BATTLE_COPY_TRIGGER_FRAME) {
        effect->colorRampEndFrame = triggerFrame;
    }
}

/* Return the frame tested by the native trigger callback. */
u32 effBattleGetInputValue(BattleEffect *effect) {
    return effect->triggerFrame;
}

/* Select the ramp endpoint: mode 1 keeps the unsigned minimum; other modes replace it. */
void effBattleUpdateSelectedValue(BattleEffect *effect, u32 endFrame) {
    if (effect->frameSelectionMode == EFF_BATTLE_MINIMUM_RAMP_END_FRAME) {
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
void func_00168810(BattleEffect *effect, u32 value) {
    effect->value120 = value;
}

/* Return the same opaque caller word. */
u32 func_00168818(BattleEffect *effect) {
    return effect->value120;
}

/* Blend toward the neutral tint over the final six frames of the ramp. */
u32 func_00168820(u32 elapsed, u32 duration)
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

INCLUDE_ASM(const s32, "effect/effBattle", func_00169230);

INCLUDE_ASM(const s32, "effect/effBattle", func_00169370);

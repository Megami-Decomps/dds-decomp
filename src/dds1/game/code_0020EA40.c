#include "pcp_vu0.h"
#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "dat_state.h"
#include "ee_mmi.h"


extern void btlBossDebugPrintf(const char *format, ...);

extern s32 datActionAnimationRecords;

extern s32 btlGetRuntime(void);

extern s32 btlSetLinkedDefeatCameraPresetB();

extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern void btlClearRuntimeFlag2000(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

/* Action-animation records are distinct from the 0x38-byte command metadata. */
typedef struct BtlActionTableRow {
    u8 pad00[3];
    u8 enabled;
    u8 pad04[0x18];
    u16 flags;
    u8 pad1E[2];
} BtlActionTableRow;

extern void btlSelectRandomDefeatCamera(BtlLinkedCommand *);

extern void btlFlagAllUnitDefeatCandidatesTask(void);

void btlRaiseLinkedActionPose(BtlLinkedCommand *command);

/* Dispatch the command's animation-camera flags. Clearing +0x110 resets command
 * state; it does not clear a BtlUnit's active flags. Returns 1 when handled. */
s32 btlDispatchActionAnimation(BtlLinkedCommand *command) {
    u16 flags = ((BtlActionTableRow *)datActionAnimationRecords)[command->actionCode].flags;
    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            btlSelectRandomDefeatCamera(command);
        } else {
            btlSetEffectCameraKeys(command, -203.0f, -531.1f, -1259.0f,
                           0.124f, -0.07f, -0.021f, 0.981f,
                           -203.0f, -46.1f, -1259.0f, -0.144f,
                           -0.066f, -0.003f, 0.978f, 45.0f, 30.0f);
        }
        command->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        btlSetLinkedDefeatCameraPresetB(command, command, 0);
    } else if (flags & 8) {
        if (btlGetIndexListCount(command->task->indexWork.indices) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseLinkedActionPose(command);
            command->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSelectRandomDefeatCamera(command);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

s32 btlGetPhaseCommand(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    switch (battle->phase) {
    case 0: return 0x11c;
    case 1: return 0x11b;
    default: return -1;
    }
}

s32 btlGetAlternatePhaseCommand(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    switch (battle->phase) {
    case 0: return 0x11f;
    case 1: return 0x120;
    default: return -1;
    }
}

/* Only scene-listed special units use these alternate action codes. */
s32 btlRemapListedUnitAction(BtlUnit *unit, s32 action) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->flags & 0x400) == 0) {
        return action;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) +
                                      (u32)datBattleSceneRecords))->unitModes;
    while (i < 0xB && listedMode[i] != unit->mode) {
        i++;
    }
    if (i == 0xB) {
        return action;
    }
    switch (action) {
    case 2:
        return 0;
    case 11:
        return 1;
    case 13:
        return -1;
    default:
        return action;
    }
}

/* Whether the unit's mode appears in its battle scene's listed unit modes. */
s32 btlIsSceneUnitModeListed(BtlUnit *unit) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->flags & 0x400) == 0) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) +
                                      (u32)datBattleSceneRecords))->unitModes;
    for (; i < 0xB; i++) {
        if (listedMode[i] == unit->mode) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0020EA40", func_0020ED90);

extern char D_003A66C0[];

void btlRunCleanupAndLog(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    void (*cleanup)(void) = battle->cleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlBossDebugPrintf(D_003A66C0);
}

extern char D_003A66D8[];

void btlReleaseBossData(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    void (*cleanup)(void);
    if ((battle->battleFlags & 0x80000) == 0) {
        return;
    }
    cleanup = battle->bossCleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlRunCleanupAndLog();
    if (battle->effect != 0) {
        sdfReleaseChipOrRetainedResource(battle->effect);
        battle->effect = 0;
    }
    battle->battleFlags &= ~0x80000;
    btlBossDebugPrintf(D_003A66D8);
}

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66C0);

INCLUDE_RODATA(const s32, "game/code_0020EA40", D_003A66D8);


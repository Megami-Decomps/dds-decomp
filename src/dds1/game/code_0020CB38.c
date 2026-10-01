#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern u32 btlGetIndexListCount();

extern char D_003BB8A0[];

extern char D_003BB898[];

extern char D_003BB8A8[];

extern s32 datActionAnimationRecords;

extern void func_003014F0();

extern s32 btlGetRuntime(void);

extern void func_001D6300(void *, void *);

extern void btlSetUnitPosition(BtlUnit *, void *);



extern s32 func_0020B348();

extern s32 func_0020DC38();

extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);

extern s32 btlGetIndexListEntry(void *, u32);


extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s64 btlStartTask(void *);

extern void func_001DB698();

extern s8 D_003BB880[];

extern s8 D_003BB888[];

extern s8 D_003BB890[];


extern void btlFlagAllUnitDefeatCandidatesTask(void);

extern void btlSetEffectCameraKeys(void *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

extern char D_003A5FF0[];

extern char D_003A6018[];

extern void btlChooseRandomPresetCameraKeys();

extern void func_0020B190(u8 *, void *);

/* 0x20-byte action metadata entries referenced by a unit's action index. */
typedef struct BtlActionTableRow {
    u8 pad00[3];
    u8 enabled; /* 0x03: zero rejects the action */
    u8 pad04[0x18];
    u16 flags;  /* 0x1C: special animation selection bits */
    u8 pad1E[2];
} BtlActionTableRow;

extern void btlClearRuntimeFlag2000(void);

/* Dispatch linked-camera animation metadata; +0x110 resets command state, not
 * unit flags. Returns 1 when a camera path is handled, otherwise 0. */
s32 btlDispatchActionAnimationB(BtlLinkedCommand *command) {
    u16 flags = ((BtlActionTableRow *)datActionAnimationRecords)[command->actionCode].flags;
    if (flags & 0x4000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            btlChooseRandomPresetCameraKeys((u8 *)command);
        } else {
            btlSetEffectCameraKeys((void *)command, 59.2f, -700.6f, -1901.2f,
                           0.092f, 0.023f, -0.009f, 0.987f,
                           59.2f, -160.6f, -1901.2f, -0.106f,
                           0.025f, -0.014f, 0.985f, 45.0f, 30.0f);
        }
        command->state = 0;
    } else if (flags & 0x8000) {
        btlFlagAllUnitDefeatCandidatesTask();
        func_0020B348((void *)command, (void *)command, 0);
    } else if (flags & 8) {
        if (btlGetIndexListCount(command->task->unk_60) == 1) {
            void *target = (void *)btlGetIndexListEntry((void *)command->task->unk_60, 0);
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020B190((u8 *)command, target);
            command->state = 0;
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlChooseRandomPresetCameraKeys((u8 *)command);
        }
    } else {
        return 0;
    }
    btlClearRuntimeFlag2000();
    return 1;
}

void func_0020CCA8(void) {
    func_0020ADA8();
}

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020CCC0);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020D168);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020D2E0);

s32 btlAllowsSpeciesCondition(BtlUnit *unit, BtlUnit *other, s32 condition) {
    s32 kind;
    if ((unit->flags & 0x200) == 0) {
        return 1;
    }
    if (condition != 0) {
        return 0;
    }
    kind = other->mode;
    switch (kind) {
    case 0x13d:
    case 0x13e:
        return 0;
    default:
        return 1;
    }
}

void btlFormatBattleEffectResourceName(s32 record, s32 action, s32 pathBuffer) {
    if (action != 0xd5) {
        return;
    }
    func_003014F0(pathBuffer, "%s%03X_%02X.BED", D_003BB8A8, *(u16 *)(datActionAnimationRecords + 0x1aa4), *(s32 *)(record + 0x38) - 0x13d);
}

u32 func_0020D548(void) {
    return 7;
}

s32 btlGetEnabledEnemyActionResponse(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)datActionAnimationRecords)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x114) {
        return 11;
    }
    return -1;
}

s32 func_0020D598(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)datActionAnimationRecords)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x111) {
        return 11;
    }
    return -1;
}

s32 btlOffsetSpecialTargetPositionForAction(BtlLinkedCommand *command, s32 unused1, s32 unused2) {
    BtlTask *link = command->task;
    BtlUnit *other;
    f32 pos[4] __attribute__((aligned(16)));

    if (link == 0) {
        return 1;
    }
    other = link->unit;
    if ((other->flags & 0x400) == 0) {
        return 0;
    }
    if (other->mode != 0x111) {
        return 0;
    }
    if (command->actionCode == 0x175) {
        PCP_COPY_VECTOR(pos, other->position);
        pos[2] += 600.0f;
        btlSetUnitPosition(other, pos);
    }
    return 0;
}

extern s32 btlOffsetSpecialTargetPositionForAction();

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020D668);

s32 func_0020D690(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)datActionAnimationRecords)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x109) {
        return 11;
    }
    return -1;
}

s32 btlDeactivateOthersOnSpecialUnitDefeat(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    BtlUnit *head = unit;
    s32 result = -1;
    for (; unit != NULL; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x13C) {
                    if (unit->flags & 0x20) {
                        result = 1;
                        break;
                    }
                }
            }
        }
    }
    if (result != -1) {
        for (unit = head; unit != NULL; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit->flags & 2) {
                        if (!(unit->flags & 0xE0)) {
                            if (unit->mode != 0x13C) {
                                btlStartTask(btlCreateUnitFadeOutTask(unit, 6, 0xA));
                                unit->flags &= ~1;
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

s32 btlNormalizeActionForSkill(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0 || unit->mode != 0x13c) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

void btlRecenterUnitsOnLead(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    BtlUnit *unit;
    BtlUnit *lead = 0;
    f32 shift;
    f32 pos[4];

    for (unit = work->units; unit != 0; unit = unit->next) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->mode == 0x137) {
                    lead = unit;
                    break;
                }
            }
        }
    }
    if (lead != 0) {
        func_001D6300(lead, pos);
        shift = -pos[0];
        pos[0] = 0;
        PCP_COPY_VECTOR(lead->position, pos);
        btlSetUnitPosition(lead, pos);
        for (unit = work->units; unit != 0; unit = unit->next) {
            if (unit->flags & 1) {
                if (unit->flags & 0x400) {
                    if (unit != lead) {
                        func_001D6300(unit, pos);
                        pos[0] = pos[0] + shift;
                        PCP_COPY_VECTOR(unit->position, pos);
                        btlSetUnitPosition(unit, pos);
                    }
                }
            }
        }
    }
}

u32 btlMapActionToCode(s32 action) {
    u32 result;

    result = 0x7d;
    if (action != 0x143) {
        result = 0;
    }
    return result;
}

u8 func_0020D9A8(s32 action) {
    return action != 0xd3;
}

s32 btlNormalizeActionForStatus(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0 || unit->mode != 0x115) {
        return action;
    }
    switch (action) {
    case 2: return 0;
    case 13: return -1;
    default: return action;
    }
}

s32 func_0020D9F8(BtlUnit *unit, s32 action) {
    if ((unit->flags & 0x400) == 0) {
        return -1;
    }
    if (((BtlActionTableRow *)datActionAnimationRecords)[action].enabled == 0) {
        return -1;
    }
    if (unit->mode == 0x113) {
        return 11;
    }
    return -1;
}

/* Select one of two fixed defeat-camera key sets for this motion command. */
void btlSelectRandomDefeatCamera(BtlLinkedCommand *command) {
    btlFlagAllUnitDefeatCandidatesTask();
    switch (effMiscRandMod(0, 2)) {
    case 0:
        func_003003F0(D_003A5FF0);
        btlSetEffectCameraKeys(command, -307.2f, -72.7f, -1292.7f, -0.096f, -0.085f, -0.004f, 0.983f, 148.3f, -84.1f, -1407.1f,
                      -0.079f, 0.064f, -0.018f, 0.986f, 40.0f, 25.0f);
        break;
    case 1:
        func_003003F0(D_003A6018);
        btlSetEffectCameraKeys(command, -207.7f, -76.6f, -1176.0f, -0.108f, -0.099f, -0.002f, 0.98f, -282.2f, -106.6f, -1451.2f,
                      -0.071f, -0.1f, -0.006f, 0.984f, 40.0f, 25.0f);
        break;
    }
}

extern void btlBuildLinkedCommandCameraPair(void *, void *, void *, s32, s32, f32, f32, f32);

extern void func_001DB698(void *);

/* Frame the linked units, extend both camera distances by 500 and mirror origins.
 * The old coordinate50/coordinateE0 names described distance scalars, not height. */
void btlRaiseLinkedActionPose(BtlLinkedCommand *command) {
    BtlCamState *frontCamera = &command->frontCamera;
    BtlCamState *backCamera = &command->backCamera;
    btlBuildLinkedCommandCameraPair(command, frontCamera, backCamera, 0, 1, 0.25f, 0.0f, 0.5f);
    command->motionParameter = 30.0f;
    command->flags |= 0x41;
    command->frontCamera.distance += 500.0f;
    command->backCamera.distance += 500.0f;
    func_001DB698(frontCamera);
    func_001DB698(backCamera);
}

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020DC38);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020DE50);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020DE70);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020E058);

INCLUDE_ASM(const s32, "game/code_0020CB38", func_0020E170);

extern void btlSelectRandomDefeatCamera(BtlLinkedCommand *);

s32 btlHandleLinkedUnitDefeatAction(BtlLinkedCommand *command) {
    BtlTask *entry = command->task;
    BtlUnit *other;
    if (entry->unit->flags & 0x200) {
        if (btlGetIndexListCount((void *)entry->unk_60) == 1) {
            other = (BtlUnit *)btlGetIndexListEntry((void *)entry->unk_60, 0);
            if ((other->flags & 0x400) == 0) {
                return 0;
            }
            btlRaiseLinkedActionPose(command);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            func_0020DC38((void *)command, (void *)command, 0);
            command->state = 0;
        }
        return 1;
    }
    return 0;
}

s32 btlApplyActionDefeatCamera(BtlLinkedCommand *command) {
    u16 flags = ((BtlActionTableRow *)datActionAnimationRecords)[command->actionCode].flags;
    if (flags & 0x1000) {
        btlFlagAllUnitDefeatCandidatesTask();
        if (!(flags & 0x10)) {
            btlSelectRandomDefeatCamera(command);
        } else {
            btlSetEffectCameraKeys((void *)command, -203.0f, -531.1f, -1259.0f,
                           0.124f, -0.07f, -0.021f, 0.981f,
                           -203.0f, -46.1f, -1259.0f, -0.144f,
                           -0.066f, -0.003f, 0.978f, 45.0f, 30.0f);
        }
        return 1;
    }
    if (flags & 0x2000) {
        if (btlGetIndexListCount((void *)command->task->unk_60) == 1) {
            btlFlagAllUnitDefeatCandidatesTask();
            btlRaiseLinkedActionPose(command);
        } else {
            btlFlagAllUnitDefeatCandidatesTask();
            btlSelectRandomDefeatCamera(command);
        }
        return 1;
    }
    return 0;
}

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8A8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B4);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8B8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8C0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8C8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8D0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8D8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8E0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8E8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8F0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB8F8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB900);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB908);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB910);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB918);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB920);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB928);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB930);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB938);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB93E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB940);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB948);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB950);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB958);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB960);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB968);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB970);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB978);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB980);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB988);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB990);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB998);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9A0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9A8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9B0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9B8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9C0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9C8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9D0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9D8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9E0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9E8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9F0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BB9F8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA00);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA08);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA10);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA18);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA20);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA28);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA30);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA38);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA40);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA48);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA50);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA58);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA60);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA68);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA70);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA78);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA80);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA88);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA90);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBA98);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAA0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAA8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAB0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAB8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAC0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAC8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAD0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAD8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAE0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAE8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAF0);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBAF8);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB00);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB08);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0D);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB0F);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB10);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB14);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB18);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB1C);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB1E);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB20);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB24);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB28);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB2C);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB30);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB38);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB40);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB48);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB50);

INCLUDE_SDATA(const s32, "game/code_0020CB38", D_003BBB58);


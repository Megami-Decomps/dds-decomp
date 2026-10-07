#include "common.h"
#include "btl_state.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "evt_unit.h"
#include "mdl.h"
#include "dat_state.h"

extern void btlSetActorEffectParameterOrMuzzlePosition();

extern void btlInterpolateVectorStep();

extern void btlCopyMotionTransform();

extern void func_001E88A8();

extern s32 mdlFlagTest(s32);


extern s32 btlGetRuntime(void);

extern void btlBossDebugPrintf(const char *format, ...);

extern u32 btlGetEffectActive(void);

extern u32 btlGetEffectValue(void);

extern u32 btlGetEffectActor(void);

/* The model flag word is the first member of the extension's +0x8C info object.
 * Retail accesses unit+0x340 -> extension+0x8C -> flags+0; no second unit view. */
typedef struct BtlUnitInfo {
    union {
        u8 b0;
        u32 flags;
    };
    u8 pad1[0x14];
    s32 unk18;
    struct BtlUnitData *data;
} BtlUnitInfo;


extern void *btlCreateUnitFadeOutTask(void *, s32, s32);

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern s32 btlHasEffectActor(void);

extern s64 btlStartTask(void *);

extern s32 btlCreateCommandSoundUpdateTask();

extern s32 btlCreateSecondaryCommandSoundTask();

extern s32 btlCreateCommandSoundTask();

extern s32 btlCreateEffObjB();

extern u8 *fldCreateSceneGroupAction(BtlTask *, u32, s32);

extern char D_00436CF8[];

extern char D_0041B628[];

extern char D_0041B640[];
extern s32 btlHasMarkedEntry14(s32 actor);
extern void btlClearRuntimeFlag2000(void);
extern u32 effMiscRandMod(void *state, u32 modulus);
extern void btlSetEffectCameraKeys(s32 command, f32, f32, f32, f32, f32, f32, f32, f32,
    f32, f32, f32, f32, f32, f32, f32, f32);


void func_00227288(void) {
    btlUpdateLinkedEffectUnitTransforms();
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002272A0);

s32 btlIsEffectPhaseInRange(s32 unused, s32 value) {
    BattleLinkedEffectState *effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->active != 1) {
        return 0;
    }
    switch (value) {
    case 0x12:
    case 0x13:
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227528);

s32 btlRemapEffectActiveCombatantAction(BtlUnit *unit, s32 code) {
    u32 id;
    if (!(unit->flags & 1)) {
        return code;
    }
    if (!(unit->flags & 0x400)) {
        return code;
    }
    if (((BtlState *)btlGetRuntime())->effect->active != 1) {
        return code;
    }
    id = unit->partyRecord.unitId;
    if (id == 0x119) {
        return code;
    }
    switch (code) {
    case 1:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 1;
        }
        break;
    case 2:
        if (id == 0x12E) {
            return 0xC8;
        }
        if (id == 0x12F) {
            return 0x66;
        }
        break;
    default:
        if (id == 0x12E) {
            return code + 0xC8;
        }
        if (id == 0x12F) {
            return code + 0x64;
        }
        break;
    }
    return code;
}

BtlUnit *btlFindFlaggedSpecialSpeciesUnit(s32 category, s32 species) {
    BtlUnit *unit;
    if (category != 1) {
        return 0;
    }
    if (species != 0x119) {
        return 0;
    }
    unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        u32 flags = unit->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (flags & 2) {
                    if (unit->resourceIndex == 0x119) {
                        return unit;
                    }
                }
            }
        }
        unit = unit->nextActor;
    }
    return 0;
}

/* Collapse three special unit modes to one display code. */
s32 btlGetCanonicalCombatantKind(BtlUnit *unit) {
    switch (unit->partyRecord.unitId) {
    case 0x119:
    case 0x12E:
    case 0x12F:
        return 0x119;
    }
    return unit->combatantKind;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227820);

INCLUDE_ASM(const s32, "game/code_00227288", func_002279F0);

s32 btlGetEffectTaskActorMatchCode(BtlTask *task) {
    BattleLinkedEffectState *effect;
    if ((task->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    return effect->actor == (u32)task->unit ? 12 : -1;
}

s32 btlEffectTaskStartFinale(BtlTask *task) {
    BattleLinkedEffectState *effect;
    u8 *group;

    if ((task->flags & 8) == 0) {
        return -1;
    }
    effect = ((BtlState *)btlGetRuntime())->effect;
    if (effect->actor != (u32)task->unit) {
        return -1;
    }
    btlStartTask(btlCreateCommandSoundUpdateTask());
    btlStartTask(btlCreateSecondaryCommandSoundTask());
    btlStartTask(btlCreateCommandSoundTask(task, 9));
    btlStartTask(btlCreateEffObjB(task->unit, 0xB4));
    group = fldCreateSceneGroupAction(task, 0x64, 1);
    *(s32 *)(group + 0x28) = 0x16;
    btlStartTask(group);
    effect->phase = 1;
    return (task->unit->partyRecord.status & 0x480) ? 0x19 : 0x1B;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00227DA8);

s32 btlIsEffectActor(u32 actor) {
    BattleLinkedEffectState *state = ((BtlState *)btlGetRuntime())->effect;
    u32 active = state->actor;
    if (active != 0) {
        return active == actor;
    }
    return 0;
}

s32 btlGetSoleTargetKind(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    BtlUnit *target;
    BtlUnit *unit;
    BtlUnit *last;
    s32 count;
    if (btlHasEffectActor() == 0) {
        return -1;
    }
    target = (BtlUnit *)btlGetEffectActor();
    last = NULL;
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 0x200) {
                if (!(unit->flags & 0xE0)) {
                    if (!(unit->partyRecord.status & 0x800)) {
                        count++;
                        last = unit;
                    }
                }
            }
        }
    }
    if (count == 1 && (last == NULL || last == target)) {
        return 7;
    }
    return -1;
}

s32 btlHasDifferentActiveTarget(u32 target) {
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    return btlGetEffectActor() != target;
}

s32 btlSetLinkFlagOff(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->partyRecord.unitId == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            other->ext->owner->flags &= ~1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            other->ext->owner->flags &= ~1;
        } else {
            other->ext->owner->flags |= 1;
        }
        return 0;
    }
    return 1;
}

s32 btlSetLinkFlagOn(BtlUnit *requestedUnit) {
    BtlUnit *unit;
    BtlUnit *other;
    if (btlHasEffectActor() == 0) {
        return 1;
    }
    for (unit = ((BtlState *)btlGetRuntime())->units; unit != NULL; unit = unit->nextActor) {
        if (unit->flags & 1) {
            if (unit->flags & 0x400) {
                if (unit->partyRecord.unitId == 0x12F) {
                    break;
                }
            }
        }
    }
    if (unit != NULL) {
        other = (BtlUnit *)btlGetEffectActor();
        if (!(other->flags & 2)) {
            return 1;
        }
        if (unit == requestedUnit) {
            other->ext->owner->flags |= 1;
            return 1;
        }
        if (other != requestedUnit) {
            return 1;
        }
        if (unit->flags & 4) {
            other->ext->owner->flags &= ~1;
        } else {
            other->ext->owner->flags |= 1;
        }
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_002286D8);

s32 btlTryScheduleMarkedUnitTask(BtlUnit *unit) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleLinkedEffectState *effect;
    BtlUnit *other;
    if ((unit->flags & 0x400) == 0) {
        return 1;
    }
    effect = battle->effect;
    if (effect->active != 0) {
        return 1;
    }
    if (unit->resourceIndex == 0x119) {
        return 1;
    }
    other = battle->units;
    while (other != 0) {
        u32 flags = other->flags;
        if (flags & 1) {
            if (flags & 0x400) {
                if (other != unit) {
                    if (flags & 0xe0) {
                        return 1;
                    }
                }
            }
        }
        other = other->nextActor;
    }
    btlStartTask(btlCreateUnitFadeOutTask(unit, 8, 10));
    unit->flags &= ~0x100;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00228B08);

/* Initialize the linked command's target aim once in state 0x1E. Action 0x171
 * succeeds without setup; unsupported actions return 0, deferred setup returns 1. */
s32 btlUnitStartAimAtTarget(BtlLinkedCommand *command) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *target;
    u32 flags;
    f32 distance;

    if (command->actionCode == 0x171) {
        return 1;
    }
    if (command->actionCode != 0x189) {
        return 0;
    }
    for (target = battle->units; target != 0; target = target->nextActor) {
        flags = target->flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x400) != 0) {
                if ((flags & 2) != 0) {
                    if (target->partyRecord.unitId == 0x12F) {
                        break;
                    }
                }
            }
        }
    }
    if (target == 0) {
        return 1;
    }
    if (command->state != 0x1E || command->motionProgress != 0) {
        return 1;
    }
    btlCopyMotionTransform(&command->frontCamera, command);
    command->motionParameter = 10.0f;
    command->motionProgress = 1;
    command->state = 0;
    btlSetActorEffectParameterOrMuzzlePosition(target, 0);
    VU0_STORE_VF_UNCLOBBERED(vf10, command->backCamera.position);
    command->backCamera.position[1] += 150.0f;
    btlInterpolateVectorStep(&command->frontCamera);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, command->backCamera.position);
    VU0_SUB(vf10, vf10, vf11);
    VU0_LENGTH_VF10(distance);
    command->backCamera.distance = distance;
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, command->backCamera.direction);
    command->backCamera.distance += 45.0f;
    func_001E88A8(command->backCamera.position);
    return 1;
}

/* Recognize the two action codes accepted by the linked-command aim handler. */
s32 func_00228F20(BtlLinkedCommand *command) {
    switch (command->actionCode) {
    case 0x171:
        return 1;
    case 0x189:
        return 1;
    }
    return 0;
}

s32 func_00228F48(BtlLinkedCommand *command) {
    u32 choice;

    if (command->actionCode != 0x188) {
        return 0;
    }
    if (btlHasMarkedEntry14((s32)command) != 0) {
        if (command->motionProgress >= 0x19) {
            btlClearRuntimeFlag2000();
            if (command->motionProgress == 0x19) {
                choice = effMiscRandMod(0, 2);
                switch (choice) {
                case 0:
                    btlSetEffectCameraKeys((s32)command,
                        -438.9f, -787.7f, 536.2f, 0.091f,
                        -0.889f, -0.328f, 0.275f, -533.3f,
                        -901.0f, 631.6f, 0.098f, -0.883f,
                        -0.326f, 0.295f, 40.0f, 15.0f);
                    break;
                case 1:
                    btlSetEffectCameraKeys((s32)command,
                        359.4f, -728.1f, 534.7f, -0.086f,
                        -0.911f, -0.283f, -0.254f, 399.0f,
                        -801.4f, 632.3f, -0.084f, -0.913f,
                        -0.284f, -0.248f, 40.0f, 15.0f);
                    break;
                }
                command->state = 0;
            }
        }
        command->motionProgress++;
    }
    return 1;
}

u32 btlGetEffectActive(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleLinkedEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->active;
    }
    return 0;
}

s32 btlHasEffectActor(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleLinkedEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state == NULL) {
        return 0;
    }
    return state->actor != 0;
}

u32 btlGetEffectValue(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 battleId = battle->battleMode;
    BattleLinkedEffectState *state;
    if (battleId != 0x312) {
        return 0;
    }
    state = battle->effect;
    if (state != NULL) {
        return state->value;
    }
    return 0;
}

u32 btlGetEffectActor(void) {
    BattleLinkedEffectState *state = ((BtlState *)btlGetRuntime())->effect;
    return state->actor;
}

s32 btlIsSpecialEnemyEffectLinkSatisfied(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *unit = battle->units;
    BattleLinkedEffectState *effect = battle->effect;
    while (unit != 0) {
        if ((unit->flags & 0x400) &&
            unit->partyRecord.unitId == 0x12F) {
            break;
        }
        unit = unit->nextActor;
    }
    if (unit == 0) {
        return 1;
    }
    if (!(unit->flags & 0xe0)) {
        return 0;
    }
    return effect->linkedUnit == unit;
}

void btlArmEventResourceTrigger(void) {
    u8 *puVar1;
    BtlState *battle;

    battle = (BtlState *)btlGetRuntime();
    puVar1 = (u8 *)battle->effect;
    puVar1[1] = 1;
    *puVar1 = 0;
}

u32 func_00229278(void) {
    return 0xffffffff;
}

s32 btlConsumeReadyEventScriptResource(void) {
    s32 result = -1;
    s32 state;

    state = *(s32 *)(btlGetRuntime() + 0x718);
    if (*(s8 *)(state + 0) != 0) {
        if (*(s8 *)(state + 1) != 0) {
            *(u8 *)(state + 1) = 0;
            return btlFindScriptResource(D_00436CF8);
        }
        *(u8 *)(state + 0) = 0;
        return -1;
    }
    return result;
}

void func_002292D8(void) {
    u16 flags;
    u16 *entry;
    u32 markedCount;
    u32 index;
    u32 activeCount;
    u8 hasKindTwo;

    hasKindTwo = 0;
    markedCount = 0;
    activeCount = 0;
    entry = (u16 *)((u32)datGameState + 0xa60);
    index = 0;
    do {
        flags = *entry;
        if ((flags & 1) != 0) {
            if (entry[2] == 2) {
                if ((flags & 2) != 0) {
                    return;
                }
                hasKindTwo = 1;
            }
            activeCount = activeCount + 1;
            if ((flags & 2) != 0) {
                markedCount = markedCount + 1;
            }
        }
        index = index + 1;
        entry = entry + 0xe2;
    } while (index < 5);
    if (((activeCount < 4) && (markedCount < 3)) && (hasKindTwo)) {
        func_0011AEE0(2);
        return;
    }
}

s32 btlClearEffectNodeRuntimeFlagForActiveUnits(void) {
    BtlUnit *unit = ((BtlState *)btlGetRuntime())->units;
    while (unit != 0) {
        if (unit->flags & 1) {
            unit->stateFlags &= ~0x100000;
        }
        unit = unit->nextActor;
    }
    return -1;
}

void btlMarkBattleUnitEntryForActiveKind(BtlUnit *unit) {
    u32 flags = unit->flags;
    if ((flags & 0x200) && unit->partyRecord.unitId == 2) {
        unit->flags = flags | 0x1000;
        unit->stateFlags |= 0x100000;
        unit->partyRecord.flags |= 0x1000;
    }
}

void btlSetAlternateKindForEnabledSpecialUnit(BtlUnit *unit) {
    if ((unit->flags & 0x400) &&
        unit->partyRecord.unitId == 0x144 &&
        mdlFlagTest(0x841)) {
        unit->partyRecord.hp = 1;
    }
}

u32 func_00229470(void) {
    return 6;
}

void func_00229478(void) {
    func_0011AEE0(7);
}

void func_00229490(void) {
    func_0011AEE0(1);
    func_0011AEE0(4);
    func_0011AEE0(5);
}

void func_002294B8(void) {
    func_0011AEE0(8);
}

INCLUDE_ASM(const s32, "game/code_00227288", btlRemapListedUnitAction);

INCLUDE_ASM(const s32, "game/code_00227288", func_002295D8);

s32 btlIsSceneUnitModeListed(BtlUnit *unit) {
    BtlState *battle;
    u16 *listedMode;
    u32 i;

    if ((unit->flags & 0x400) == 0) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    i = 0;
    listedMode = ((DatBattleSceneRecord *)(battle->battleMode * (s32)sizeof(DatBattleSceneRecord) + (u32)datBattleSceneRecords))->unitModes;
    for (; i < 0xB; i++) {
        if (listedMode[i] == unit->partyRecord.unitId) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00227288", func_00229728);

void btlRunCleanupAndLog(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    void (*cleanup)(void) = *(void (**)(void))(battle + 0x604);
    if (cleanup != 0) {
        cleanup();
    }
    btlBossDebugPrintf(D_0041B628);
}

void btlReleaseBossData(void) {
    u8 *battle = (u8 *)btlGetRuntime();
    void (*cleanup)(void);
    if ((((BtlState *)battle)->battleFlags & 0x80000) == 0) {
        return;
    }
    cleanup = ((BtlState *)battle)->bossCleanup;
    if (cleanup != 0) {
        cleanup();
    }
    btlRunCleanupAndLog();
    if (((BtlState *)battle)->effect != 0) {
        sdfReleaseChipOrRetainedResource(((BtlState *)battle)->effect);
        ((BtlState *)battle)->effect = 0;
    }
    ((BtlState *)battle)->battleFlags &= ~0x80000;
    btlBossDebugPrintf(D_0041B640);
}

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B628);

INCLUDE_RODATA(const s32, "game/code_00227288", D_0041B640);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436CF8);

INCLUDE_SDATA(const s32, "game/code_00227288", btlPrimaryScriptResourceName);

INCLUDE_SDATA(const s32, "game/code_00227288", btlSecondaryScriptResourceName);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D8E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436D98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DD8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436DF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436E98);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EA8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EB8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EC8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436ED8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EE8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF0);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436EF8);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F00);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F08);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F10);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F18);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F20);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F28);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F30);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F38);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F40);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F48);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F50);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F58);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5D);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F5F);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F60);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F61);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F62);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F64);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F68);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F6E);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F70);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F74);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F78);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F7C);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F80);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F84);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F88);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F90);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F96);

INCLUDE_SDATA(const s32, "game/code_00227288", D_00436F98);


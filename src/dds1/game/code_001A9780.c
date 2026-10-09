#include "common.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "fr_font_measure.h"
#include "fr_font.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "pcp_vu0.h"
#include "btl_ui.h"
#include "sdf.h"
#include "btl.h"
#include "btl_state.h"
#include "eff.h"
#include "btl_action.h"
#include "kwln.h"
#include "dat_state.h"
#include "dat_command.h"
#include "kwln_task_lifecycle.h"

extern void func_001C2E90(EffectSlotSet *, s32, s32 *, s32 *, f32);

extern SceneSlotFadeWork *D_003BD83C;

extern ActorSlotOrder *D_003BD840[2];

extern void mdlFlagSet(s32 flag);

extern void dspCloseChannel(void);

extern s32 dspStartEntry(s32 entry);

extern void evtCreateMessageWindowIfMissing(void *text);

extern void btlBossDebugPrintf(const char *format, ...);

typedef struct SceneSlot {
    u8 a;
    u8 b;
    u8 id;
} SceneSlot;

typedef struct BattleItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BattleItemDrop;

typedef struct SceneFadingRecord {
    SceneSlot slot;
    u8 alpha;
    s32 target;
} SceneFadingRecord;

typedef struct BattleController {
    u8 pad_000[0x1F4];
    u32 flags;
    u8 pad_1F8[0x2C];
    BtlTask *taskHead; /* 0x224 */
    BtlUnit *actors;
    u8 pad_22C[0x20];
    u16 variant;
    u8 pad_24E[2];
    s32 step;
    u8 pad_254[0x1C];
    s32 adjustmentRecordIndex;
    s32 adjustmentGroupIndex;
    s32 adjustmentEntryIndex;
    s32 mode;
    u8 pad_280[0x1C];
    KwlnTask *taskParent;
    u8 pad_2A0[8];
    s32 sceneObjectTask;
    s32 spriteObject;
    s32 cleanupTask;
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2C0 */
    u8 pad2C4[4];
    s32 experienceEarned; /* 0x2C8 */
    s32 epEarned; /* 0x2CC */
    u8 pad2D0[4];
    SceneSlot slots[8];
    BtlTask *groupPrimary[20];
    BtlTask *groupSecondary[45];
    BtlTask *groupTertiary[15];
    u8 pad_42C[0x20];
    SceneFadingRecord fadingRecords[8]; /* 0x44C */
    u8 pad_48C[0x18];
    EffectSlotSet *resA; /* 0x4A4 */
    EffectSlotSet *resB;
    EffectSlotSet *resC;
    u8 pad_4B0[0x100];
    s32 (*sceneCallback)();
} BattleController;

extern s32 btlGetEffectActive(void);

extern void func_001BCB88(s8, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

typedef struct SndPad {
    u8 pad00[0x21];
    s8 confirm;
    s8 edge22;
    s8 edge23;
    u8 pad24[2];
    s8 prev;
    s8 next;
    u8 pad28[0x18]; /* The kernel snapshot contains four 16-byte input sets. */
} SndPad;

extern SndPad D_00324510;

extern s8 D_00324530[];

extern u32 D_00359A78[];

extern s32 mdlFlagTest(s32);

extern DatEnemyRecord *datEnemyRecords;

extern SceneAiWork *fldGetSceneScriptTaskUserData(void);

extern u32 func_001978E8(s32, s32, s32, u32, char *, s32);

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

extern u32 D_003BB3DC;

extern SceneKindTable *D_003BD834;

extern u32 D_003BD838;

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

extern const char *D_003BB3A8;

extern const char *btlCommandPanelTaskNameRef;

extern const char *D_003BB3B0;

extern const char *D_003BB3AC;

extern const char *D_003BB3BC;

extern const char *D_003BB3C4;

extern const char *btlAnalyzPanelTaskNameRef;

extern const char *btlMahenPanelTaskNameRef;

extern void func_00101A80(KwlnTask *, KwlnTask *);

extern s32 btlGetTrackedTaskHandle(s32);

extern BattleCmdPanel *btlCommandPanelWork;

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

extern void func_001B83D8(BtlTask *, s8, s8);

extern s32 btlGetRuntime(void);

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

extern char D_003BB450[];

extern s32 datRosterDetails;

extern s32 effMiscRandMod(s32, s32);

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);

extern s8 effSharedRandomState[];

extern void func_001C45F0(void);

extern void itfMesDestroyWindowIfPresent(s32);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 arg0, BtlIndexList *arg1);

extern s8 effSharedRandomState[];

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);

extern void btlUnitGetMuzzlePosVU(void *);

extern s32 evtRunContext(s32, s32, s32, s32, u16);

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *arg0, s32 arg1);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s8 btlGetActorIndexedSignedValue(BtlUnit *unit, s32 index);

s32 btlGetCombinedPartyCommandPower(DatPartyRecord *base, BtlUnit *first, BtlUnit *second, BtlUnit *third, s32 command);

s32 btlGetCommandFailureReason(BtlUnit *unit, s32 command);

/* The label resource index is the copied party record's unit ID. */ void *btlGetIndexedUiResource(BtlUnit *unit);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

s8 func_001A2AE8(s32 command);

s32 func_001A2DE8(BtlUnit *base, BtlUnit *first, BtlUnit *second, BtlUnit *third, s32 command);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001A9780);

extern const f32 D_003A1D28[9];
extern const char D_003A1D50[];
extern void func_0011CE48(s32 chance);

s32 func_001A99B0(void) {
    extern u32 btlRollAiBucket(void);
    f32 levelScale[9];
    BtlState *state;
    u32 partyLevel;
    u32 count;
    u32 enemyLevel;
    u32 i;
    s32 difference;
    s32 chance;
    f32 ratio;

    memcpy(levelScale, D_003A1D28, sizeof(levelScale));
    state = (BtlState *)btlGetRuntime();
    if (datBattleSceneRecords[state->battleMode].flags & 0x200) {
        return 1;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 0x100) {
        return 0;
    }
    partyLevel = 0;
    count = 0;
    /* The ordering bytes select the first three active members of five slots. */
    for (i = 0; i < 5 && count < 3; i++) {
        u8 slot = datGameState->partyOrder[i];
        if (datGameState->party[slot].flags & 1) {
            if (datGameState->party[slot].flags & 2) {
                count++;
                partyLevel += datGameState->party[slot].level;
            }
        }
    }
    if (count >= 2) {
        partyLevel /= count;
    }
    enemyLevel = 0;
    count = 0;
    for (i = 0; i < 9; i++) {
        u16 enemy = datBattleSceneRecords[state->battleMode].unitModes[i];
        if (enemy != 0 && i < 8) {
            count++;
            enemyLevel += datEnemyRecords[enemy].level;
        }
    }
    if (count >= 2) {
        enemyLevel /= count;
    }
    difference = partyLevel - enemyLevel;
    if (difference < 0) {
        difference = 0;
    } else if (difference >= 9) {
        difference = 8;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 0x2000) {
        chance = 75;
    } else if (datBattleSceneRecords[state->battleMode].flags & 0x1000) {
        chance = 50;
    } else {
        chance = 10;
    }
    chance = (s32)((f32)chance * levelScale[difference]);
    ratio = 1.0f;
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (datGameState->party[i].flags & 2) {
                if (btlCheckSpecialAbility(&datGameState->party[i], 0x22D)) {
                    ratio *= datAbilityParameters[0x22D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
                }
            }
        }
    }
    chance = (s32)((f32)chance * ratio);
    btlBossDebugPrintf(D_003A1D50, chance, difference, ratio);
    func_0011CE48(chance);
    return (s32)btlRollAiBucket() < chance;
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1D28);

const char D_003A1D50[] = "btl:surprise=%d%%[level=%d,ratio=%.2f]\n";

void btlClearUnitStatusMask(void) {
    BtlUnit *actor = ((BtlState *)btlGetRuntime())->units;
    for (; actor != NULL; actor = actor->next) {
        u32 flags = actor->status.flags;
        if ((flags & 1) != 0) {
            if ((flags & 0x200) != 0) {
                actor->status.flags = flags & ~0x1000;
                actor->partyRecord.flags &= ~0x1000;
            }
        }
    }
}

extern s32 evtRunContext(s32, s32, s32, s32, u16);

s32 func_001A9D38(BtlUnit *actor, s32 index) {
    DatPartyRecord *record;
    s32 chance;
    f32 ratio;

    if ((actor->partyRecord.status & 0x7FFF) != 0x800) {
        return 0;
    }
    if (btlHasEnemyRecordDefeatExemptionFlag(actor)) {
        return 0;
    }
    switch ((u32)btlGetActorIndexedSignedValue(actor, index)) {
    case 0:
    case 1:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    record = &actor->partyRecord;
    chance = evtRunContext(0x12, (s32)record, 0, index, 0);
    ratio = 1.0f;
    if (btlCheckSpecialAbility(record, 0x233)) {
        ratio = datAbilityParameters[0x233 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    chance = (s32)((f32)chance * ratio);
    btlBossDebugPrintf("btl:stone dead=%d%%[ratio=%.2f]\n", chance, ratio);
    return btlRollAiBucket() < chance;
}

extern char D_003A1DA0[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

s32 btlRollActorEligibilityWithAbilityOverride(BtlUnit *actor) {
    u8 *battle = (u8 *)btlGetRuntime();
    s32 (*predicate)(BtlUnit *) = *(s32 (**)(BtlUnit *))(battle + 0x65C);
    if (predicate != 0 && predicate(actor) == 0) return 0;
    if (actor->status.stateFlags & 0x2000) return 0;
    if ((actor->partyRecord.status & 0x7FFF) == 0x4000) return 0;
    if (btlCheckSpecialAbility(&actor->partyRecord, 0x231)) return 1;
    btlBossDebugPrintf(D_003A1DA0, 5, 1.0);
    return btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *object) {
    if ((object->status.flags & 0x400) == 0) {
        return 0;
    }
    return ((s32)datEnemyRecords[object->partyRecord.unitId].flags & 0x100) > 0;
}

extern s32 effMiscRand(void *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1DA0);

u32 btlGetHuntPenaltyFlags(BtlUnit *unit, BtlUnit *enemy) {
    DatEnemyRecord *record;
    s32 chance;
    u16 status;
    u16 flags;

    if (btlCheckSpecialAbility(&unit->partyRecord, 0x225) != 0) {
        return 0;
    }
    status = enemy->partyRecord.status & 0x7FFF;
    record = &datEnemyRecords[enemy->partyRecord.unitId];
    switch (status) {
    case 0x400:
        chance = record->huntPenaltyChance * 3;
        break;
    case 0x80:
        chance = record->huntPenaltyChance * 2 + 30;
        break;
    default:
        chance = record->huntPenaltyChance;
        break;
    }
    btlBossDebugPrintf("btl:hunt bad = %d%%\n", chance);
    if (chance == 0) {
        return 0;
    }
    if (btlRollAiBucket() >= chance) {
        return 0;
    }
    flags = record->huntPenaltyFlags;
    if (flags == 0x18) {
        return (effMiscRand(effSharedRandomState) & 1) ? 0x10 : 8;
    }
    return flags & 0x18;
}

extern s32 D_00358690[];

s32 btlRollPassiveAbilityAction(BtlUnit *actor) {
    u32 i;
    s32 result;
    s32 threshold;

    if (btlIsUnitDefeatTriggeredByValueDelta(actor, 0)) {
        return -1;
    }
    result = 0;
    for (i = 0; i < 3; i++) {
        if (btlCheckSpecialAbility(&actor->partyRecord, D_00358690[i * 2])) {
            threshold = (s32)(datAbilityParameters[D_00358690[i * 2] - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
            if (btlRollAiBucket() < threshold) {
                result = (D_00358690 + i * 2)[1];
                break;
            }
        }
    }
    return result != 0 ? result : -1;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AA130);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AA548);

u32 func_001AA848(void) {
    btlGetRuntime();
    return 0xffffffff;
}

void btlRestoreUnitMinimumValueAndClearStatus(BtlUnit *object, s32 status) {
    *(u16 *)(status + 0x26) &= ~3;
    object->status.flags = (u32)object->status.flags & (~0x20);
    *(u16 *)((u8 *)object + 0x12E) &= ~0x4080;
    if (*(u16 *)((u8 *)object + 0x126) == 0) {
        *(u16 *)((u8 *)object + 0x126) = 1;
    }
}

s32 func_001AA8B0(BtlTask *action) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlOperandGroup *group;
    BtlUnit *unit;
    u32 count;
    u32 i;
    u32 targetSideA;
    u32 targetSideB;
    u32 activeSideA;
    u32 activeSideB;

    if ((action->flags & 0x40) != 0) {
        return 0;
    }
    if (battle->requestArgument != 0) {
        return 0;
    }
    if ((battle->commandRestrictFlags & 1) != 0) {
        return 0;
    }

    targetSideA = 0;
    targetSideB = 0;
    count = btlGetIndexListCount(action->indexWork.indices);
    group = action->indexWork.groups;
    for (i = 0; i < count; i++, group++) {
        if (group->inactive != 0) {
            BtlUnit *target;
            if (group->reflected != 0) {
                target = action->unit;
            } else {
                target = btlGetIndexListEntry(action->indexWork.indices, i);
            }
            if ((target->status.flags & 0x200) != 0) {
                targetSideA++;
            } else if ((target->status.flags & 0x400) != 0) {
                targetSideB++;
            }
        }
    }

    activeSideA = 0;
    activeSideB = 0;
    for (unit = battle->units; unit != NULL; unit = unit->next) {
        s32 flags = unit->status.flags;
        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0xE0) != 0) {
            continue;
        }
        if ((flags & 0x200) != 0) {
            activeSideA++;
        } else if ((flags & 0x400) != 0) {
            activeSideB++;
        }
    }
    if (targetSideA >= activeSideA) {
        return 0;
    }
    if (targetSideB < activeSideB) {
        return 0;
    }
    return 1;
}

s32 btlIsBattleRecordEligible(u8 *actor, u8 *target, s32 recordIndex, s32 speciesIndex) {
    s32 (*callback)(u8 *, u8 *, s32) =
        *(s32 (**)(u8 *, u8 *, s32))(btlGetRuntime() + 0x680);
    u8 *resource;
    if (callback != 0 && callback(actor, target, speciesIndex) == 0) {
        return 0;
    }
    resource = (u8 *)btlGetSideIndexedActorStatusTable(*(s32 *)(actor + 0xC4), *(s32 *)(actor + 0xC8));
    if (*(s16 *)(resource + recordIndex * 20 + 0x2C) != 2) {
        return 0;
    }
    if (speciesIndex != 0 &&
        datCommandRecords[speciesIndex].targetType != 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorModeActionCodeAllowed(BtlUnit *actor) {
    if (actor->unkDC != 1) {
        return 1;
    }
    switch (actor->displaySpecies) {
    case 0x26:
    case 0x54:
    case 0x153:
        return 0;
    default:
        return 1;
    }
}

s32 btlHasHighPriorityState(void) {
    BattleSelectionWork *resource;
    s8 index;
    s32 count;
    if (btlCommandPanelWork->state == 2) {
        resource = btlLinkedSelectionTaskBuffer;
        index = resource->selectedRow;
        count = resource->rowFade[index - 1];
        if (count >= 0x80) {
            if (resource->positions[index - 1].x >= 11) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    BtlUnit *actor = *(BtlUnit **)(btlGetRuntime() + 0x228);
    s32 count = 0;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x321) == 0x301 &&
            (*(u16 *)((u8 *)actor + 0x12E) & 0x800) == 0) {
            count++;
        }
        actor = *(BtlUnit **)((u8 *)actor + 0x344);
    }
    return count;
}

typedef struct BattleCmdPanelSlotTable {
    u32 values[5];
} BattleCmdPanelSlotTable;

extern const BattleCmdPanelSlotTable D_003A1FA8;

extern const BattleCmdPanelSlotTable D_003A1FC0;

void btlInitializeCommandPanelSlotTables(void) {
    BattleCmdPanelSlotTable tableA = D_003A1FA8;
    BattleCmdPanelSlotTable tableB = D_003A1FC0;
    s32 i;

    btlCommandPanelWork = sdfAllocAndClearQuadwords(0xCC);
    btlCommandPanelWork->state = 1;
    btlCommandPanelWork->classIndex = 0;
    btlCommandPanelWork->labelEntry = btlGetActorIdForClass(btlCommandPanelWork->classIndex);
    btlGetActorClassPair(btlCommandPanelWork->classIndex,
                  &btlCommandPanelWork->classX,
                  &btlCommandPanelWork->classY);
    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].unk01 = 0;
        btlCommandPanelWork->slotsA[i].fadeValue = tableB.values[i];
        btlCommandPanelWork->slotsB[i].unk01 = 1;
        btlCommandPanelWork->slotsB[i].fadeValue = tableA.values[i];
    }
}

typedef struct ActorClassIds {
    s16 values[8];
} ActorClassIds;

extern const ActorClassIds btlActorIdsByClass;

s16 btlGetActorIdForClass(s8 classId) {
    ActorClassIds table = btlActorIdsByClass;
    return table.values[classId];
}

typedef struct ActorClassPairTable {
    u32 values[14];
} ActorClassPairTable;

extern const ActorClassPairTable btlActorClassPairs;

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    ActorClassPairTable pairs = btlActorClassPairs;
    *first = pairs.values[classId * 2];
    *second = pairs.values[classId * 2 + 1];
}

extern SndPad D_00324510;

extern u8 D_00359160[];

extern void func_001B83D8(BtlTask *, s8, s8);

extern void sndSetStationedSeVolume(u32);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1FA8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_001A9780", btlActorIdsByClass);

INCLUDE_RODATA(const s32, "game/code_001A9780", btlActorClassPairs);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AADF8);

typedef struct BattleCmdPanelGridTable {
    u8 values[30];
} BattleCmdPanelGridTable;

extern const BattleCmdPanelGridTable D_003A2050;

extern const BattleCmdPanelGridTable D_003A2070;

void btlPopulateCommandPanelGrid(void) {
    BattleCmdPanelGridTable table1 = D_003A2050;
    BattleCmdPanelGridTable table2 = D_003A2070;
    s32 i;

    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].hidden =
            table1.values[btlCommandPanelWork->classIndex * 5 + i];
        btlCommandPanelWork->slotsB[i].hidden =
            table2.values[btlCommandPanelWork->classIndex * 5 + i];
    }
}

/* Ramp both five-row banks by panel state; narrow each update before clamping. */
void btlUpdateCommandPanelRowFades(void) {
    s32 i;

    switch (btlCommandPanelWork->state) {
    case 1:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue += 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 2:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0x80 ? 0x80 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0xF0 ? 0xF0 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 3:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= 0x10;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    case 4:
        for (i = 0; i < 5; i++) {
            btlCommandPanelWork->slotsA[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsA[i].fadeValue = btlCommandPanelWork->slotsA[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsA[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsA[i].fadeValue;
            btlCommandPanelWork->slotsB[i].fadeValue -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->slotsB[i].fadeValue = btlCommandPanelWork->slotsB[i].fadeValue <= 0 ? 0 :
                btlCommandPanelWork->slotsB[i].fadeValue > 0x80 ? 0x80 : btlCommandPanelWork->slotsB[i].fadeValue;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AB558);

extern f32 sdfSinPoly(f32);

typedef struct BattleCornerFadeDefaults {
    s16 values[20];
} BattleCornerFadeDefaults;

typedef struct BattleCornerPhaseDefaults {
    u32 values[12];
} BattleCornerPhaseDefaults;

extern const BattleCornerFadeDefaults D_003A2100;

extern const BattleCornerPhaseDefaults D_003A2128;

void func_001AB810(void) {
    BattleCornerFadeDefaults limits = D_003A2100;
    BattleCornerPhaseDefaults phases = D_003A2128;
    s32 i;
    switch (btlCommandPanelWork->state) {
    case 1:
        btlCommandPanelWork->labelFade += 0x10;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade += 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] += 0x10;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] >= limits.values[i] ? limits.values[i] : btlCommandPanelWork->cornerFade[i];
            btlCommandPanelWork->cornerPhase[i] = phases.values[i];
        }
        if (btlCommandPanelWork->labelFade >= 0x80 && btlCommandPanelWork->slotsB[0].fadeValue >= 0xF0)
            btlCommandPanelWork->state = 2;
        break;
    case 2: {
        btlCommandPanelWork->labelFade -= 8;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0x80 ? 0x80 :
            btlCommandPanelWork->labelFade >= 0x100 ? 0xFF : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0x80 ? 0x80 :
            btlCommandPanelWork->classFade >= 0x100 ? 0xFF : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerPhase[i] = (btlCommandPanelWork->cornerPhase[i] + 8) % 360;
            btlCommandPanelWork->cornerFade[i] =
                (u32)((sdfSinPoly((f32)((btlCommandPanelWork->cornerPhase[i] + 90) % 360) /
                                  180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 128.0f);
        }
        break;
    }
    case 3:
        btlCommandPanelWork->labelFade -= 0x10;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= 0x10;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] -= 0x10;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] > 0x80 ? 0x80 : btlCommandPanelWork->cornerFade[i];
        }
        if (btlCommandPanelWork->labelFade <= 0) btlCommandPanelWork->state = 0;
        break;
    case 4:
        btlCommandPanelWork->labelFade -= btlTrackedTaskHandles->status.bytes.fadeStep;
        btlCommandPanelWork->labelFade = btlCommandPanelWork->labelFade <= 0 ? 0 :
            btlCommandPanelWork->labelFade > 0x80 ? 0x80 : btlCommandPanelWork->labelFade;
        btlCommandPanelWork->classFade -= btlTrackedTaskHandles->status.bytes.fadeStep;
        btlCommandPanelWork->classFade = btlCommandPanelWork->classFade <= 0 ? 0 :
            btlCommandPanelWork->classFade > 0x80 ? 0x80 : btlCommandPanelWork->classFade;
        for (i = 0; i < 4; i++) {
            btlCommandPanelWork->cornerFade[i] -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlCommandPanelWork->cornerFade[i] = btlCommandPanelWork->cornerFade[i] <= 0 ? 0 :
                btlCommandPanelWork->cornerFade[i] > 0x80 ? 0x80 : btlCommandPanelWork->cornerFade[i];
        }
        if (btlCommandPanelWork->labelFade <= 0) btlCommandPanelWork->state = 0;
        break;
    }
}

extern BtlResBlock *btlResourceBlock;

extern void func_002BF438(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

typedef struct BattlePanelColors {
    u32 values[4];
} BattlePanelColors;

extern const BattlePanelColors D_003A21B8;

typedef struct BattleClassLabelOffsets {
    s32 values[8][2];
} BattleClassLabelOffsets;

extern const BattlePanelColors D_003A2158;

extern const BattleClassLabelOffsets D_003A2168;

extern void func_001AB558(void);

void func_001ABDF8(void) {
    BattlePanelColors colors = D_003A2158;
    BattleClassLabelOffsets offsets = D_003A2168;
    if (btlCommandPanelWork->state < 5) {
        if (btlCommandPanelWork->state > 0) {
            colors.values[0] = btlCommandPanelWork->cornerFade[0] | 0x80808000;
            colors.values[1] = btlCommandPanelWork->cornerFade[1] | 0x80808000;
            colors.values[2] = btlCommandPanelWork->cornerFade[2] | 0x80808000;
            colors.values[3] = btlCommandPanelWork->cornerFade[3] | 0x80808000;
            func_002BF438(0x50, 0x968, 0, colors.values, 0, btlResourceBlock->resA, 0x1E, 0x53);
            func_001AB558();
            colors.values[0] = btlCommandPanelWork->classFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            func_002BF438((btlCommandPanelWork->classX + 5) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1A, 0x53);
            func_002BF438((btlCommandPanelWork->classX + 0x37) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1B, 0x53);
            colors.values[0] = btlCommandPanelWork->labelFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            {
                s32 x = btlCommandPanelWork->classX + 5;
                s32 y = btlCommandPanelWork->classY + 0x12D;
                func_002BF438((x + offsets.values[btlCommandPanelWork->classIndex][0]) << 4,
                             (y + offsets.values[btlCommandPanelWork->classIndex][1]) << 3,
                             0, colors.values, 0, btlResourceBlock->resA, (s16)btlCommandPanelWork->labelEntry, 0x53);
            }
        }
    }
}

void btlReleaseAndClearChipBlock(void) {
    sdfReleaseChipBlock(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AC080);

void func_001AC398(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            switch (record->active) {
            case 1: {
                s32 scale = (s32)record->scale;
                s32 x = record->x;
                s32 y = record->y;
                s32 width = btlResourceBlock->resA->workEntries[record->slot].sourceWidth;
                s32 height;
                s32 scaledWidth;
                s32 scaledHeight;
                record->restoredWidth = width;
                scale = scale / 2;
                height = btlResourceBlock->resA->workEntries[record->slot].sourceHeight;
                scaledWidth = width * scale;
                scaledHeight = height * scale;
                record->active = 2;
                record->restoredHeight = height;
                x -= (scaledWidth - width) / 2;
                y -= (scaledHeight - height) / 2;
                record->secondX = x;
                record->y = y;
                record->width = scaledWidth << 4;
                record->height = scaledHeight << 3;
                break;
            }
            case 2:
                record->width -= 0x800;
                record->height -= 0x800;
                if (record->width <= 0x1000) record->active = 0;
                break;
            }
            remaining--;
            record++;
        } while (remaining != 0);
    }
}

void func_001AC4A8(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
    BattlePanelColors colors = D_003A21B8;
    if (count > 0) {
        BattleMirroredSpriteRecord *record = records;
        s32 remaining = count;
        do {
            if (record->active != 0) {
                u32 color;
                btlResourceBlock->resA->workEntries[record->slot].geometry.bounds[2] = record->width;
                btlResourceBlock->resA->workEntries[record->slot].geometry.bounds[3] = record->height;
                color = record->alpha | 0x80808000;
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                func_002BF438(record->x << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                func_002BF438(record->secondX << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                btlResourceBlock->resA->workEntries[record->slot].geometry.bounds[2] = record->restoredWidth << 4;
                btlResourceBlock->resA->workEntries[record->slot].geometry.bounds[3] = record->restoredHeight << 3;
            }
            record++;
        } while (--remaining != 0);
    }
}

void btlInitializeActionRecordWithScale(BattleMirroredSpriteRecord *record, s16 slot,
                                      s32 x, s32 y, s32 index, f32 scale) {
    record->active = 1;
    record->alpha = index * 8 + 0x18;
    record->slot = slot;
    record->scale = scale;
    record->x = x;
    record->y = y;
    record->frame = 0;
}

extern u8 D_003BB3E4;

extern u8 btlResourceBlockLoaded;

extern char D_003A21C8[]; /* "/battle/panel/batle_01.spr" */

extern char D_003A21E8[]; /* "/battle/panel/batle_02.spr" */

extern char D_003A2208[]; /* "/battle/panel/battle_03.spr" */

void btlPanelResourcesLoad(void) {
    u8 params[16];
    s32 handle;
    BtlResBlock *block;
    if (D_003BB3E4 == 0) {
        handle = (u32)sdfAllocGeneralBlock(0x28);
        block = (BtlResBlock *)sdfResourceRetainAddress((struct SdfMemBlock *)(handle));
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->nameA = (s32)(u32)sdfReadNamedResource(D_003A21C8, params, 0);
        btlResourceBlock->nameB = (s32)(u32)sdfReadNamedResource(D_003A21E8, params, 0);
        btlResourceBlock->nameC = (s32)(u32)sdfReadNamedResource(D_003A2208, params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_003BB3E4 = 1;
}

void btlLoadResourceBlock(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    if (btlResourceBlockLoaded == 0) {
        btlResourceBlock->resA = effCreateResourceSlotSetFromAllocation(
            (struct SdfMemBlock *)(u32)btlResourceBlock->nameA, 0);
        btlResourceBlock->resB = effCreateResourceSlotSetFromAllocation(
            (struct SdfMemBlock *)(u32)btlResourceBlock->nameB, 0);
        btlResourceBlock->resC = effCreateResourceSlotSetFromAllocation(
            (struct SdfMemBlock *)(u32)btlResourceBlock->nameC, 0);
        work->resA = btlResourceBlock->resA;
        work->resB = btlResourceBlock->resB;
        btlResourceBlockLoaded = 1;
    }
}

void btlReleaseResourceBlock(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    if (btlResourceBlockLoaded != 0) {
        effDestroyResourceSlotSet(btlResourceBlock->resA);
        btlResourceBlock->resA = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resB);
        btlResourceBlock->resB = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resC);
        btlResourceBlock->resC = 0;
        work->resA = 0;
        work->resB = 0;
        work->resC = 0;
        btlResourceBlockLoaded = 0;
    }
}

extern void btlClearTaskActorSlots(void);

extern s32 func_001AD758(s32);

s32 btlResetSceneSlotFades(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    SceneSlotFadeWork *state;
    ActorSlotOrder **bank;
    s32 kind;
    s32 count;
    s32 i;

    memset(D_003BD83C, 0, sizeof(*D_003BD83C));
    memset(D_003BD840[0], 0, sizeof(*D_003BD840[0]));
    memset(D_003BD840[1], 0, sizeof(*D_003BD840[1]));
    btlTrackedTaskHandles->fadeKindsCached = 0;
    btlCountSceneSlots();
    if (battle->mode == 1) {
        kind = 0;
        count = btlTrackedTaskHandles->fadeKindACount;
        D_003BD83C->enabled[1] = 0;
    } else {
        kind = 1;
        count = btlTrackedTaskHandles->fadeKindBCount;
        D_003BD83C->enabled[0] = 0;
    }
    bank = &D_003BD840[kind];
    D_003BD83C->currentIndex = count;
    D_003BD83C->lastIndex = count;
    D_003BD83C->bank = kind;
    D_003BD83C->enabled[kind] = 1;
    D_003BD83C->phase[0][kind] = 1;
    D_003BD83C->timer = 0;
    (*bank)->state[0] = 1;
    state = D_003BD83C;
    state->unk18 = 0;
    state->completed = 0;
    for (i = 0; i < count; i++) {
        (*bank)->unk6C[i] = 30.0f;
        (*bank)->unk8C[i] = 130;
    }
    btlInitializeActorSlotOrder(kind, count);
    btlClearTaskActorSlots();
    return func_001AD758(kind);
}

/* Clear each task's eight opaque words, forward for variant 1 and backward otherwise. */
void btlClearTaskActorSlots(void) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlTask *node = context->taskHead;
    while (node != 0) {
        s32 i;
        BtlUnit *actor = node->unit;
        if (actor != 0) {
            if (context->variant == 1) {
                if (actor->status.flags & 0x200) {
                    u32 *entries;
                    i = 0;
                    entries = &node->actions[0];
                    for (; i < 8; i++) {
                        *entries++ = 0;
                    }
                }
            } else if (actor->status.flags & 0x400) {
                u32 *entries;
                i = 7;
                entries = &node->actions[7];
                for (; i >= 0; i--) {
                    *entries-- = 0;
                }
            }
        }
        node = node->next;
    }
}

typedef struct ActorOrder12 {
    s32 entries[12];
} ActorOrder12;

typedef struct ActorOrder6 {
    s32 entries[6];
} ActorOrder6;

extern const ActorOrder12 D_003A2228;

extern const ActorOrder6 D_003A2258;

void btlInitializeActorSlotOrder(s32 selector, s32 count) {
    ActorOrder12 primaryOrder = D_003A2228;
    ActorOrder6 secondaryOrder = D_003A2258;
    s32 *order = selector != 0 ? primaryOrder.entries : secondaryOrder.entries;
    ActorSlotOrder *destination;
    s32 i;

    i = 0;
    if (count > 0) {
        destination = D_003BD840[selector];
        do {
            destination->entries[i] = order[i];
            i++;
        } while (i < count);
    }
}

s32 btlCountSceneSlots(void) {
    BattleController *scene;
    s32 fadeCounts[4];

    scene = (BattleController *)btlGetRuntime();
    return fldCountSceneFadeKinds(scene, fadeCounts);
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001ACB08);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001ACC20);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 2;
    puVar1 = datGameState->battleFlags;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

s32 func_001ACD30(s32 id, s32 operation) {
    s32 selector;
    s32 offset;
    u32 word;
    u32 bit;

    selector = (u16)id;
    offset = selector - 0x1AB;
    selector = (s8)operation;
    if (offset != 0) {
        word = (u32)offset >> 5;
        bit = offset & 0x1F;
    } else {
        word = 0;
        bit = 0;
    }
    switch (selector) {
    case 0:
        datGameState->battleFlags[word] |= 1 << bit;
        break;
    case 1:
        datGameState->battleFlags[word] &= ~(1 << bit);
        break;
    default:
        return ((datGameState->battleFlags[word] & (1 << bit)) != 0);
    }
    return 1;
}

void func_001ACDF0(void) {
    s32 *temp_v0;
    s32 temp_v1;

    temp_v0 = D_00359A78;
    temp_v0 += 2;
    temp_v1 = 2;
    do {
        temp_v1 = temp_v1 - 1;
        *temp_v0 = 0;
        temp_v0 = temp_v0 - 1;
    } while (-1 < temp_v1);
}

s32 func_001ACE28(s32 id, s32 operation) {
    s32 selector;
    s32 offset;
    u32 word;
    u32 bit;

    id = (u16)id;
    offset = id - 0x1AB;
    selector = (s8)operation;
    if (offset != 0) {
        word = (u32)offset >> 5;
        bit = offset & 0x1F;
    } else {
        word = 0;
        bit = 0;
    }
    switch (selector) {
    case 0:
        D_00359A78[word] |= 1 << bit;
        break;
    case 1:
        D_00359A78[word] &= ~(1 << bit);
        break;
    default:
        return ((D_00359A78[word] & (1 << bit)) != 0);
    }
    return 1;
}

extern const char *D_003BB3A0;

s32 btlSetTaskPhase2(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_003BB3A0);
    if (task != 0) {
        *(s32 *)kwlnTaskGetUserValue(task) = 2;
        return 1;
    }
    return 0;
}

extern void btlUpdateActorSlotPresentationState(BtlUnit *, s8, s8);

void btlHighlightActorStatPanel(BtlUnit *unit, s8 side) {
    KwlnTask *task;
    BattleActorPanelWork *work;
    u32 index;

    if (unit->status.flags & 0x200) {
        index = unit->lookupId;
        task = kwlnTaskGetTaskByName(D_003BB3B0);
        if (task != NULL) {
            work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
            if (side == 0) {
                work->activeEntries[index].presentation.hpState = 0x10;
                work->activeEntries[index].presentation.hpHighlightLevel = 0x7F;
            } else {
                work->activeEntries[index].presentation.mpState = 0x10;
                work->activeEntries[index].presentation.mpHighlightLevel = 0x7F;
            }
            btlUpdateActorSlotPresentationState(unit, 0, 2);
        }
    }
}

typedef struct { u16 flag; u16 unk_02; } MesWindowState;

typedef struct MesWindowConfig {
    MesWindowState state[9];
} MesWindowConfig;

typedef struct MesWindowList {
    s8 state;
    u8 pad01[3];
    s32 window[9];
    u8 pad28[0x18];
    s32 x;
    s32 y;
    MesWindowConfig config;
} MesWindowList;

typedef struct ItfMesSub ItfMesSub;

extern ItfMesSub D_0032A668;

extern s32 itfMesCreateWindow(ItfMesSub *);

extern u32 btlIsNamedBattleTaskRegistered(void);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

extern s32 func_001ADE68(KwlnTask *);

extern void itfMesCloseAllWindows(KwlnTask *);

s32 btlCreateMahenPanelTask(const MesWindowConfig *config) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    MesWindowList *work;
    KwlnTask *task;
    s32 i;

    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    work = sdfAllocAndClearQuadwords(sizeof(*work));
    work->config = *config;
    work->state = 0;
    for (i = 0; i < 9; i++) {
        if (work->config.state[i].flag != 0) {
            work->window[i] = itfMesCreateWindow(&D_0032A668);
        }
    }
    work->x = 0x20;
    work->y = 0x20;
    task = kwlnTaskCreate(btlMahenPanelTaskNameRef, 0x2B0E, 1, 1,
                          func_001ADE68, itfMesCloseAllWindows, (u32)work);
    func_00101A80(battle->taskParent, task);
    btlSetTrackedTaskHandle(10, (s32)task);
    return 1;
}

u32 btlIsNamedBattleTaskRegistered(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(10);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

void btlRequestMahenPanelClose(void) {
    MesWindowList *work;
    KwlnTask *task;

    task = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
    if (task != 0) {
        work = (MesWindowList *)kwlnTaskGetUserValue(task);
        work->state = 2;
    }
}

typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    s32 entry;
    s32 duration;
    s32 frame;
    u8 pad10[0x18];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0x6F];
} BtlAnalysisPanelWork;

extern s32 func_001AE540(KwlnTask *);

extern void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *);

/* Create the analysis panel, replacing any panel task already registered. */
s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlAnalysisPanelWork *data;
    s32 task;

    if (btlHasRegisteredAnalysisPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(11), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->page = 0;
    data->x = -20;
    data->y = 133;
    data->duration = duration;
    data->entry = entry;
    data->frame = 0;
    task = (s32)kwlnTaskCreate(btlAnalyzPanelTaskNameRef, 0x2B0E, 1, 1, func_001AE540,
                          btlReleaseTaskAndRefreshCursorIfFlagged, (u32)data);
    func_00101A80(context->taskParent, (KwlnTask *)task);
    btlSetTrackedTaskHandle(11, task);
    context->flags &= ~0x100000;
    return 1;
}

u32 btlHasRegisteredAnalysisPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(0xb);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlGetRegisteredTaskValueOrDefault(void) {
    if (btlHasRegisteredAnalysisPanelTask() == 0) {
        return 0x80;
    }
    return *(s8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef));
}

u32 btlRequestAnalysisPanelClose(void) {
    BtlAnalysisPanelWork *work;
    KwlnTask *task;

    task = kwlnTaskGetTaskByName(btlAnalyzPanelTaskNameRef);
    if (task != 0) {
        work = (BtlAnalysisPanelWork *)kwlnTaskGetUserValue(task);
        work->state = 2;
    }
    return 1;
}

/* Registered guide-panel task: fade, four corner colors, and message selection. */
typedef struct BtlGuidePanelWork {
    s8 state;
    u8 pad01[7];
    u16 fadeLevel;             /* 0x08: clamped with signed-halfword tests */
    u8 pad0A[2];
    BattlePanelColors colors;  /* 0x0C: TL, TR, BR, BL */
    s32 unk1C;
    s32 y;
    s32 windowIndex;
    s32 entryIndex;
} BtlGuidePanelWork;

typedef char BtlGuidePanelWorkSizeCheck[sizeof(BtlGuidePanelWork) == 0x2C ? 1 : -1];

extern s32 func_001B09A8(KwlnTask *);

extern void btlReleaseMessageWindowTask(KwlnTask *);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlCreateGuidePanelTask(s32 windowIndex, s32 entryIndex) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlGuidePanelWork *data;
    s32 task;

    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->unk1C = 0x18;
    data->y = 0x60;
    data->windowIndex = windowIndex;
    data->entryIndex = entryIndex;
    task = (s32)kwlnTaskCreate(D_003BB3C4, 0x2B0E, 1, 1, func_001B09A8,
                          btlReleaseMessageWindowTask, (u32)data);
    func_00101A80(context->taskParent, (KwlnTask *)task);
    btlSetTrackedTaskHandle(9, task);
    return 1;
}

void btlRequestGuidePanelClose(void) {
    u8 *puVar1;
    KwlnTask *temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)kwlnTaskGetUserValue(temp_v0);
        *puVar1 = 2;
    }
}

u32 btlHasRegisteredGuidePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(9);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_003BB3C4);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct BtlPhaseTask {
    s32 phase;
} BtlPhaseTask;

void btlSetTaskPhase5(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        ((BtlPhaseTask *)kwlnTaskGetUserValue(task))->phase = 5;
        btlCommandPanelWork->state = 3;
    }
}

void btlSetTrackedTaskDisplayMode(s32 mode) {
    s32 task = btlGetTrackedTaskHandle(7);
    if (task != 0) {
        s32 *data = (s32 *)kwlnTaskGetUserValue((KwlnTask *)task);
        data[2] = mode;
        if (mode == 0) {
            data[1] = 1;
            data[5] = 0x80;
        } else {
            data[5] = 0xFF;
            data[1] = 4;
            data[6] = 0xFF;
        }
    }
}

void btlInvalidateSceneFadeCounts(s32 unused) {
    btlGetRuntime();
    kwlnTaskGetUserValue((KwlnTask *)btlGetTrackedTaskHandle(7));
    btlTrackedTaskHandles->fadeKindsCached = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(6);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_003BB3BC);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AD758);

u32 btlHasRegisteredSkillNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_003BB3AC);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct BtlPanelTransitionWork {
    KwlnTask *task;
    const u8 *text;
    s32 elapsed;
    s32 frames;
    s32 width;
    s32 unk14;
    s32 verticalShift;
    s8 state;
    u8 pad1D;
    s16 fade;
    s16 fadeLevels[4];
    BattleSelectionPosition initial[2];
    s8 phase;
    u8 pad39[3];
    BattleSelectionPosition current[2];
} BtlPanelTransitionWork;

extern s32 btlUpdateSkillNamePanelTask(KwlnTask *);

extern void btlFreeRegisteredTaskData(KwlnTask *);

s32 func_001AD970(const u8 *text) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    KwlnTask *task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    BtlPanelTransitionWork *work;
    u32 glyph;

    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    work = sdfAllocAndClearQuadwords(sizeof(*work));
    work->text = text;
    work->frames = 30;
    work->fade = 0;
    work->fadeLevels[2] = work->fadeLevels[0] = 0x40;
    work->fadeLevels[3] = work->fadeLevels[1] = 0x10;
    glyph = itfCreateConvertedTextGlyph(0x1000, 0x200, 0xFF0000, 0x80808080, text, 0);
    work->width = frFontMeasureLines((struct FrFontGlyph *)glyph);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)glyph);
    work->initial[0].x = work->width - work->width / 2 + 0x105;
    work->initial[0].y = 0x40;
    work->initial[1].x = 0x92 - work->width / 2;
    work->initial[1].y = 0x40;
    task = kwlnTaskCreate(D_003BB3AC, 0x2B0E, 1, 1,
                          btlUpdateSkillNamePanelTask, btlFreeRegisteredTaskData, (u32)work);
    func_00101A80(battle->taskParent, task);
    work->task = task;
    btlSetTrackedTaskHandle(1, (s32)task);
    return 1;
}

u32 btlHasRegisteredAphNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(0);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_003BB3A8);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct MsgQueueTaskData {
    s32 task;
    s32 id;
    s32 unk08;
    s32 counter;
    s32 value;
    s16 fade;
} MsgQueueTaskData;

extern const BattlePanelColors D_003A2A90;

extern s32 itfMesMeasureEntryItem(s32, s32, s32);

extern void itfMesBlk24MoveTo(s32, s32, s32);

extern s32 btlDrawTimedDialogTask(KwlnTask *);

extern void btlReleaseDialogTaskData(KwlnTask *);

s32 btlReplaceDialogTasksAndQueueMessage(s32 arg0, s32 arg1) {
    s32 context = btlGetRuntime();
    s32 task = btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)task, 0);
    }
    data = (MsgQueueTaskData *)sdfAllocAndClearQuadwords(0x40);
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    data->id = arg0;
    data->unk08 = arg1;
    data->value = 0x2D;
    task = (s32)kwlnTaskCreate(D_003BB3A8, 0x2B0E, 1, 1, btlDrawTimedDialogTask, btlReleaseDialogTaskData, (u32)data);
    func_00101A80((KwlnTask *)*(u32 *)(context + 0x29C), (KwlnTask *)task);
    data->task = task;
    btlSetTrackedTaskHandle(0, task);
    return 1;
}

/* Query one of the fourteen task slots retained by the command UI. */
s32 btlGetTrackedTaskHandle(s32 slotIndex) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    return *handle;
}

void btlSetTrackedTaskHandle(s32 slotIndex, s32 taskHandle) {
    s32 *handle;

    handle = &btlTrackedTaskHandles->handles[slotIndex];
    *handle = taskHandle;
}

void func_001ADCE8(BtlUnit *unit, s8 style, s8 phase, s32 unused, BattlePanelColors *colors) {
    BtlState *state = (BtlState *)btlGetRuntime();

    if ((unit->status.flags & 0x20) || (unit->partyRecord.status & 0x4000)) {
        colors->values[0] = 0x80282222;
        colors->values[1] = 0x802C242A;
        colors->values[2] = 0x802C242A;
        colors->values[3] = 0x802C243E;
        return;
    }
    if (phase < 3) {
        if (phase > 0) {
            switch (style) {
            case 0:
                colors->values[0] = 0x80808080;
                colors->values[1] = 0x80808080;
                colors->values[2] = 0x80808080;
                colors->values[3] = 0x80808080;
                break;
            case 1:
                colors->values[0] = 0x80808080;
                colors->values[1] = 0x806C9064;
                colors->values[2] = 0x806C9064;
                colors->values[3] = 0x80C0FF40;
                break;
            case 2:
            default:
                colors->values[0] = 0x80787878;
                colors->values[1] = 0x80645874;
                colors->values[2] = 0x80482880;
                colors->values[3] = 0x804C2894;
                break;
            }
            return;
        }
    }
    if (btlCountSceneSlots() <= 0 || state->mode != 1) {
        colors->values[0] = 0x80808080;
        colors->values[1] = 0x80808080;
        colors->values[2] = 0x80808080;
        colors->values[3] = 0x80808080;
    } else {
        colors->values[0] = 0x80505050;
        colors->values[1] = 0x80505050;
        colors->values[2] = 0x80505050;
        colors->values[3] = 0x80505050;
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001ADE68);

void itfMesCloseAllWindows(KwlnTask *handle) {
    MesWindowList *list;
    s32 i;
    btlGetRuntime();
    list = (MesWindowList *)kwlnTaskGetUserValue(handle);
    for (i = 0; i < 9; i++) {
        if (list->config.state[i].flag != 0) {
            itfMesCleanupWindow(list->window[i], 0);
            itfMesDestroyWindowIfPresent(list->window[i]);
        }
    }
    sdfReleaseChipBlock(list);
    btlSetTrackedTaskHandle(10, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2050);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2070);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2090);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A20A0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A20D0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2100);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2128);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2158);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2168);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A21A8);

const BattlePanelColors D_003A21B8 = {{0x80808080, 0x80808080, 0x80808080, 0x80808080}};

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

extern const u32 D_003A22C0[4];

extern const s32 D_003A22D0[2][3], D_003A22E8[2][3], D_003A2300[2][3], D_003A23D8[2][3];

extern const s32 D_003A2318[16][3];

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A21C8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A21E8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2208);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2228);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2258);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2270);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2298);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A22C0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A22D0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A22E8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2300);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2318);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A23D8);

void func_001AE250(s32 section, s32 delta) {
    /* Rows contain pixel X, pixel Y and sprite slot; section 3 is the sixteen-row grid. */
    u32 colors[4];
    s32 primary[2][3];
    s32 secondary[2][3];
    s32 lower[2][3];
    s32 grid[16][3];
    s32 footer[2][3];
    s32 (*selectedRows)[3];
    s32 count;
    s32 i, j;

    memcpy(colors, D_003A22C0, sizeof(colors));
    memcpy(primary, D_003A22D0, sizeof(primary));
    memcpy(secondary, D_003A22E8, sizeof(secondary));
    memcpy(lower, D_003A2300, sizeof(lower));
    memcpy(grid, D_003A2318, sizeof(grid));
    memcpy(footer, D_003A23D8, sizeof(footer));

    switch (section) {
    case 0:
        selectedRows = primary;
        count = 2;
        break;
    case 1:
        selectedRows = secondary;
        count = 2;
        break;
    case 2:
        selectedRows = lower;
        count = 2;
        break;
    case 3:
        selectedRows = grid;
        count = 16;
        break;
    case 4:
    default:
        selectedRows = footer;
        count = 2;
        break;
    }
    for (i = 0; i < count; i++) {
        s32 (*strip)[3] = &selectedRows[i];
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA,
                                               (*strip)[2], j, delta);
        }
        func_002BF438((*strip)[0] << 4, (*strip)[1] << 3, 0, colors, 0,
                     btlResourceBlock->resA, (*strip)[2], 0x53);
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AE540);

extern void btlInitCursorAndApplyAction(BtlLinkedCommand *, BtlCamState *);

void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *handle) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u8 *data = (u8 *)kwlnTaskGetUserValue(handle);
    sdfReleaseChipBlock(data);
    btlSetTrackedTaskHandle(11, 0);
    if (work->cameraCommand.task->unit->partyRecord.status & 0x80) {
        btlInitCursorAndApplyAction(&work->cameraCommand, &work->cameraCommand.camera);
    }
    work->battleFlags |= 0x100000;
}

typedef struct BattlePanelEdgeWork {
    u8 pad00[0x30];
    s8 phase;
    s8 edgePhase;
    u8 pad32[2];
    s32 timer;
    u8 pad38[8];
    f32 anchor[4];
    f32 corners[4][4];
    s16 alpha;
} BattlePanelEdgeWork;

extern f32 D_003A2450[4];

/* Advance one corner toward the panel boundary before moving to the next edge. */
s32 btlAdvancePanelCornerPhase(BtlLinkedCommand *command, BattlePanelEdgeWork *work) {
    f32 center[4];

    memcpy(center, D_003A2450, sizeof(center));

    switch (work->edgePhase) {
    case 0:
        work->corners[0][1] += 80.0f;
        work->corners[0][1] =
            work->corners[0][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[0][1] ? center[1] + 174.0f :
            work->corners[0][1];
        if (center[1] + 174.0f <= work->corners[0][1]) {
            work->edgePhase++;
        }
        break;
    case 1:
        work->corners[2][0] += 80.0f;
        work->corners[2][0] =
            work->corners[2][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[2][0] ? center[0] + 200.0f :
            work->corners[2][0];
        if (center[0] + 200.0f <= work->corners[2][0]) {
            work->edgePhase++;
        }
        break;
    case 2:
        work->corners[1][1] -= 80.0f;
        work->corners[1][1] =
            work->corners[1][1] <= center[1] - 174.0f ? center[1] - 174.0f :
            center[1] + 174.0f <= work->corners[1][1] ? center[1] + 174.0f :
            work->corners[1][1];
        if (work->corners[1][1] <= center[1] - 174.0f) {
            work->edgePhase++;
        }
        break;
    case 3:
        work->corners[3][0] -= 80.0f;
        work->corners[3][0] =
            work->corners[3][0] <= center[0] - 200.0f ? center[0] - 200.0f :
            center[0] + 200.0f <= work->corners[3][0] ? center[0] + 200.0f :
            work->corners[3][0];
        if (work->corners[3][0] <= center[0] - 200.0f) {
            work->edgePhase++;
        }
        break;
    default:
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2460);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AF058);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AF5D0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2490);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A24A0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A24D0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2500);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2530);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001AFF78);

typedef struct BtlPanelStrip {
    s32 x;
    s32 y;
    s32 texture;
} BtlPanelStrip;

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A25C0);

void btlDrawThreePanelSpriteStrips(s32 unused, s32 x, s32 y, s32 delta) {
    BtlPanelStrip strips[3] = { {0, 0, 0x40}, {5, 0, 0x41}, {0x6B, 0, 0x42} };
    u32 colors[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 i, j;
    BtlPanelStrip *strip;

    for (strip = strips, i = 0; i < 3; i++, strip++) {
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA, strip->texture, j, delta);
        }
        func_002BF438((x + strip->x) << 4, (y + strip->y) << 3,
                     0, colors, 0, btlResourceBlock->resA, strip->texture, 0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2608);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A27C8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B05D0);

u32 btlSetSlotLowByteClamped(EffectSlotSet *owner, s32 group, s32 slot, s32 delta) {
    u32 word = owner->workEntries[group].savedColors[slot];
    u32 limit;
    u32 value;
    if (delta > 0) {
        limit = value = word & 0xFF;
        if ((u32)delta < value) {
            value = delta;
        }
    } else {
        limit = word & 0xFF;
        value = 0;
    }
    delta = value;
    if (limit > 0x80) {
        if (delta >= 0x80) {
            delta = limit;
        }
    }
    return (word & ~0xFF) | delta;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B09A8);

void btlReleaseMessageWindowTask(KwlnTask *task) {
    BtlGuidePanelWork *work = (BtlGuidePanelWork *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(work->windowIndex, 0);
    sdfReleaseChipBlock(work);
    btlSetTrackedTaskHandle(9, 0);
}

u32 btlGetVitalTextColor(BtlUnit *object, s32 current, s32 total, s8 mode) {
    u32 color;

    if (mode == 1 && (object->status.flags & 0x20) != 0) {
        color = 0x4F4E3E40;
    } else if ((object->partyRecord.status & 0x4800) != 0) {
        color = 0x4F4E3E40;
    } else if (current * 2 >= total) {
        color = 0xA09DC380;
    } else if (current <= 0) {
        color = 0x4F4E3E40;
    } else {
        u32 nearColor = 0xC8747380;
        color = 0xD1BA7180;
        if (current * 4 < total) {
            color = nearColor;
        }
    }
    return ((color >> 24) | ((color & 0xFF00) << 8)) |
           ((color << 24) | ((color >> 8) & 0xFF00));
}

u8 btlHasRequiredActorStatusBits(BtlUnit *unit) {
    return (~btlUnitStatusPair(unit) & 0x201) == 0;
}

void func_001B0D70(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x2c) + 0x18);
    *(s16 *)(temp_v0 + 0x2ac) = arg1;
    *(s16 *)(temp_v0 + 0x2ae) = arg2;
    *(s16 *)(temp_v0 + 0x2b0) = arg3;
}

s32 btlCountEligibleLinkedActors(BtlState *battle) {
    BtlUnit *node = battle->units;
    s32 count = 0;
    for (; node != 0; node = node->next) {
        if ((btlUnitStatusPair(node) & 0x201) == 0x201) {
            if ((node->partyRecord.flags & 2) != 0) {
                count++;
            }
        }
    }
    return count;
}

extern const char *D_003BB3C0;

typedef struct BattleRegisteredPanelWork {
    s32 state;
    s32 mode;
    s32 variant;
    s32 x;
    s32 y;
    s32 spriteAlpha;
    s32 overlayAlpha;
    s32 backdropAlpha;
} BattleRegisteredPanelWork;

typedef char BattleRegisteredPanelWork_size_check[
    sizeof(BattleRegisteredPanelWork) == 0x20 ? 1 : -1];

typedef char BattleRegisteredPanelWork_backdrop_offset_check[
    (u32)&((BattleRegisteredPanelWork *)0)->backdropAlpha == 0x1C ? 1 : -1];

extern s32 func_001B0E68(KwlnTask *);

extern void btlReleaseRegisteredChildTaskWork(KwlnTask *);

void btlStartRegisteredChildTask(void) {
    s32 context = btlGetRuntime();
    u32 data = (u32)sdfAllocAndClearQuadwords(sizeof(BattleRegisteredPanelWork));
    u32 task = (u32)kwlnTaskCreate(D_003BB3C0, 0x2B0E, 1, 1, func_001B0E68, btlReleaseRegisteredChildTaskWork, data);
    func_00101A80((KwlnTask *)*(u32 *)(context + 0x29C), (KwlnTask *)task);
    btlSetTrackedTaskHandle(7, task);
}

void func_001B0E48(BattleRegisteredPanelWork *work) {
    work->mode = 1;
    work->x = 0x80;
    work->y = 0;
    work->state = 0;
}

extern const u32 D_003A2870[4];

s32 func_001B0E68(KwlnTask *task) {
    u32 colors[4];
    BattleRegisteredPanelWork *work;
    s32 sprite;
    s32 i;

    memcpy(colors, D_003A2870, sizeof(colors));
    if ((((BtlState *)btlGetRuntime())->battleFlags & 0x200) == 0 &&
        btlTrackedTaskHandles->status.bytes.blocked == 0) {
        return 0;
    }
    if (btlResourceBlock->resC == NULL) {
        return 0;
    }
    work = (BattleRegisteredPanelWork *)kwlnTaskGetUserValue(task);
    switch (work->mode) {
    case 1:
        work->spriteAlpha += 32;
        work->spriteAlpha = work->spriteAlpha <= 0 ? 0 :
            work->spriteAlpha >= 128 ? 128 : work->spriteAlpha;
        if (work->spriteAlpha >= 128) {
            work->mode++;
        }
        work->x -= 16;
        work->x = work->x <= 0 ? 0 : work->x >= 128 ? 128 : work->x;
        if (work->x <= 16) {
            work->backdropAlpha += 32;
            work->backdropAlpha = work->backdropAlpha <= 0 ? 0 :
                work->backdropAlpha >= 128 ? 128 : work->backdropAlpha;
        }
        break;
    case 2:
        work->x -= 16;
        work->x = work->x <= 0 ? 0 : work->x >= 128 ? 128 : work->x;
        if (work->x <= 16) {
            work->backdropAlpha += 32;
            work->backdropAlpha = work->backdropAlpha <= 0 ? 0 :
                work->backdropAlpha >= 128 ? 128 : work->backdropAlpha;
            work->overlayAlpha -= 8;
            work->overlayAlpha = work->overlayAlpha <= 64 ? 64 :
                work->overlayAlpha >= 255 ? 255 : work->overlayAlpha;
        }
        break;
    case 3:
        work->spriteAlpha -= 32;
        work->spriteAlpha = work->spriteAlpha <= 0 ? 0 :
            work->spriteAlpha >= 128 ? 128 : work->spriteAlpha;
        work->backdropAlpha -= 32;
        work->backdropAlpha = work->backdropAlpha <= 0 ? 0 :
            work->backdropAlpha >= 128 ? 128 : work->backdropAlpha;
        work->overlayAlpha -= 32;
        work->overlayAlpha = work->overlayAlpha <= 0 ? 0 :
            work->overlayAlpha >= 255 ? 255 : work->overlayAlpha;
        break;
    case 4:
        work->x -= 16;
        work->x = work->x <= 0 ? 0 : work->x >= 128 ? 128 : work->x;
        if (work->x <= 16) {
            work->backdropAlpha += 32;
            work->backdropAlpha = work->backdropAlpha <= 0 ? 0 :
                work->backdropAlpha >= 128 ? 128 : work->backdropAlpha;
        }
        work->spriteAlpha -= 8;
        work->spriteAlpha = work->spriteAlpha <= 128 ? 128 :
            work->spriteAlpha >= 255 ? 255 : work->spriteAlpha;
        work->overlayAlpha -= 8;
        work->overlayAlpha = work->overlayAlpha <= 64 ? 64 :
            work->overlayAlpha >= 255 ? 255 : work->overlayAlpha;
        if (work->spriteAlpha <= 128) {
            work->mode = 2;
        }
        break;
    }
    if (work->mode < 5) {
        if (work->mode > 0) {
            sprite = work->variant == 0 ? 0x27 : 0x26;
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x29, i, work->backdropAlpha);
            }
            func_002BF438(0x19C0, -0x50, 0, colors, 0, btlResourceBlock->resC, 0x29, 0x53);
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
            }
            func_002BF438(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x28, 0x53);
            if (sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
                }
                func_002BF438(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x3A, 0x53);
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x3D, i, work->backdropAlpha);
                }
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 90.0f;
                func_002BF438(0x1CB0, 0x70, 0, colors, 0, btlResourceBlock->resC, 0x3D, 0x53);
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 0.0f;
            }
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->spriteAlpha);
            }
            func_002BF438((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                         0, colors, 0, btlResourceBlock->resC, sprite, 0x53);
            if (work->mode == 4 || sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->overlayAlpha);
                }
                func_002BF438((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                             0, colors, 0, btlResourceBlock->resC, 0x3B, 0x53);
            }
        }
    }
    return 0;
}

void btlReleaseRegisteredChildTaskWork(KwlnTask *arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock((void *)temp_v0);
    btlSetTrackedTaskHandle(7, 0);
}

/* AD758 allocates 0x138 bytes. The strip renderer uses the first three rows;
 * the later panel updater handles the other five points and their fades. */
typedef struct BattlePhasePanelWork {
    s32 frames;
    s32 mode;
    s8 phase;
    u8 pad09[0xF];
    s32 waitCounter; /* 0x18: delay before the first slide */
    u8 pad1C[0x1C];
    BattleSelectionPosition current[8]; /* 0x38 */
    BattleSelectionPosition saved[8];   /* 0x78 */
    s32 fade[8][4];                    /* 0xB8 */
} BattlePhasePanelWork;

typedef char BattlePhasePanelWork_size_must_be_0x138[
    (sizeof(BattlePhasePanelWork) == 0x138) ? 1 : -1];

typedef char BattlePhasePanelWork_waitCounter_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->waitCounter == 0x18) ? 1 : -1];

typedef char BattlePhasePanelWork_current_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->current == 0x38) ? 1 : -1];

typedef char BattlePhasePanelWork_saved_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->saved == 0x78) ? 1 : -1];

typedef char BattlePhasePanelWork_fade_offset_check[
    ((u32)&((BattlePhasePanelWork *)0)->fade == 0xB8) ? 1 : -1];

typedef struct BattlePhaseSlotIds { s32 values[3]; } BattlePhaseSlotIds;

typedef struct BattlePhaseFadeLimits { s32 values[3][4]; } BattlePhaseFadeLimits;

typedef struct BattlePhaseXBounds { s32 values[3][2]; } BattlePhaseXBounds;

extern const BattlePhaseSlotIds D_003A2880;

extern const BattlePhaseFadeLimits D_003A2890;

extern const BattlePhaseXBounds D_003A28C0;

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A28C0);

void func_001B1518(BattlePhasePanelWork *work) {
    BattlePhaseSlotIds slots = D_003A2880;
    BattlePhaseFadeLimits limits = D_003A2890;
    BattlePhaseXBounds bounds = D_003A28C0;
    s32 i, j;
    switch (work->phase) {
    case 0:
        work->waitCounter++;
        work->waitCounter = work->waitCounter <= 0 ? 0 : work->waitCounter > 10 ? 10 : work->waitCounter;
        if (work->waitCounter >= 3) {
            for (i = 0; i < 3; i++) {
                work->current[i].x -= 10;
                work->current[i].x = work->current[i].x <= bounds.values[i][0] ? bounds.values[i][0] :
                    work->current[i].x < bounds.values[i][1] ? work->current[i].x : bounds.values[i][1];
                for (j = 0; j < 4; j++) {
                    work->fade[i][j] += 0x20;
                    work->fade[i][j] = work->fade[i][j] <= 0 ? 0 : work->fade[i][j] < limits.values[i][j] ? work->fade[i][j] : limits.values[i][j];
                }
            }
            if (work->fade[0][0] >= 0x80) work->phase++;
        }
        break;
    case 1:
        work->fade[0][0] = 0xFF;
        work->current[0].x -= 2;
        if (work->current[0].x <= 300) work->phase++;
        break;
    case 2:
        work->current[0].x -= 20;
        work->current[1].x -= 16;
        work->fade[0][0] -= 16;
        work->fade[0][0] = work->fade[0][0] <= 0 ? 0 : work->fade[0][0] > 0xFF ? 0xFF : work->fade[0][0];
        if (work->current[0].x <= 128) work->phase++;
        break;
    case 3:
        work->current[0].x -= 18;
        work->current[1].x -= 18;
        work->current[2].x -= 18;
        if (work->current[0].x <= 96) {
            work->phase++;
            work->fade[0][0] = 0;
            work->fade[0][2] = 0;
        }
        break;
    case 4:
        work->current[0].x -= 20;
        work->current[1].x -= 20;
        work->current[2].x -= 20;
        work->fade[0][1] -= 0x20;
        work->fade[0][1] = work->fade[0][1] <= 0 ? 0 : work->fade[0][1] > 0x80 ? 0x80 : work->fade[0][1];
        work->fade[0][3] -= 0x20;
        work->fade[0][3] = work->fade[0][3] <= 0 ? 0 : work->fade[0][3] > 0x80 ? 0x80 : work->fade[0][3];
        work->fade[1][2] = work->fade[0][3];
        work->fade[1][0] = work->fade[0][1];
        if (work->fade[0][1] <= 64) {
            work->fade[1][1] -= 0x20;
            work->fade[1][1] = work->fade[1][1] <= 0 ? 0 : work->fade[1][1] > 0x80 ? 0x80 : work->fade[1][1];
            work->fade[2][0] = work->fade[1][1];
            work->fade[2][2] = work->fade[1][3];
        }
        if (work->fade[2][0] <= 64) {
            work->fade[2][1] -= 0x20;
            work->fade[2][1] = work->fade[2][1] <= 0 ? 0 : work->fade[2][1] > 0x80 ? 0x80 : work->fade[2][1];
            work->fade[2][3] -= 0x20;
            work->fade[2][3] = work->fade[2][3] <= 0 ? 0 : work->fade[2][3] > 0x80 ? 0x80 : work->fade[2][3];
        }
        break;
    }
    if (work->phase >= 3) {
        for (i = 0; i < 3; i++) {
            s32 height = btlResourceBlock->resC->workEntries[slots.values[i]].sourceHeight;
            work->saved[i].y += 3;
            work->saved[i].y = work->saved[i].y <= 0 ? 0 : work->saved[i].y < height ? work->saved[i].y : height;
        }
    }
    work->fade[1][3] = 0;
    work->fade[2][0] = 0;
    work->fade[2][1] = 0;
    work->fade[2][2] = 0;
    work->fade[2][3] = 0;
}

extern void fldScaleSceneCoordinateRecord(EffectSlotSet *, s32);

extern void func_001B19F8(BattlePhasePanelWork *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B19F8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2938);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B1C88);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2968);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B2390);

s32 btlUpdatePhaseGatedTaskUntilTimeout(KwlnTask *task) {
    BattlePhasePanelWork *state = (BattlePhasePanelWork *)kwlnTaskGetUserValue(task);
    s32 phase = btlGetNamedTaskPairStatusOrUnavailable();
    if ((u8)(phase - 1) < 2) {
        return 0;
    }
    func_001B1518(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001B19F8(state);
    }
    func_001B1C88(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001B2390(state);
    }
    ++state->frames;
    return state->frames < 50 ? 0 : -1;
}

void btlReleasePsechgPanelWork(KwlnTask *arg0) {
    BattlePhasePanelWork *work;

    work = (BattlePhasePanelWork *)kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock(work);
    btlSetTrackedTaskHandle(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2998);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B2AC8);

void btlReleaseBattleScratchBlocks(void) {
    sdfReleaseChipBlock(D_003BD834);
    sdfReleaseChipBlock((void *)D_003BD838);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A29F0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B2D80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B3300);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B3DC8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B3FB0);

void btlReleaseCmsleffPanelWork(KwlnTask *arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock((void *)temp_v0);
    btlSetTrackedTaskHandle(5, 0);
}

extern const BattlePanelColors D_003A2A80;

extern void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *);

struct FrFontGlyph;

extern void frFontDrawGlyphWithSharedFlags(struct FrFontGlyph *, s8);

s32 btlUpdateSkillNamePanelTask(KwlnTask *task) {
    BattlePanelColors colors = D_003A2A80;
    BtlPanelTransitionWork *work;
    s16 *fade;
    s32 i;
    s32 width;
    u32 glyph;

    if (btlGetTrackedTaskHandle(1) == 0) {
        return 0;
    }
    work = (BtlPanelTransitionWork *)kwlnTaskGetUserValue(task);
    switch (work->state) {
    case 0:
        btlUpdatePanelTransitionGradients(work);
        work->fade += 0x20;
        work->fade = work->fade <= 0 ? 0 : work->fade > 0x80 ? 0x80 : work->fade;
        if (work->fade >= 0x80) {
            work->state = 1;
        }
        break;
    case 1:
        btlUpdatePanelTransitionGradients(work);
        if (work->elapsed++ >= work->frames) {
            work->state = 2;
        }
        break;
    case 2:
        for (fade = work->fadeLevels, i = 3; i >= 0; i--, fade++) {
            *fade -= 0x20;
            *fade = *fade <= 0 ? 0 : *fade > 0x80 ? 0x80 : *fade;
        }
        work->fade -= 0x20;
        work->fade = work->fade <= 0 ? 0 : work->fade > 0x80 ? 0x80 : work->fade;
        work->unk14++;
        work->unk14 = work->unk14 <= 0 ? 0 : work->unk14 > 8 ? 8 : work->unk14;
        work->verticalShift++;
        work->verticalShift = work->verticalShift <= 0 ? 0 : work->verticalShift > 8 ? 8 : work->verticalShift;
        if (work->fade <= 0) {
            return -1;
        }
        break;
    }
    width = work->width;
    glyph = itfCreateConvertedTextGlyph((0x100 - (width >> 1)) << 4, 0x220, 0xFF0000,
                                       work->fade | 0x80808000, work->text, 0);
    frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)glyph, 1);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)glyph);
    colors.values[0] = work->fadeLevels[0] | 0x80808000;
    colors.values[1] = work->fadeLevels[2] | 0x80808000;
    colors.values[2] = work->fadeLevels[1] | 0x80808000;
    colors.values[3] = work->fadeLevels[3] | 0x80808000;
    if (work->verticalShift > 0) {
        btlResourceBlock->resC->workEntries[0x15].geometry.bounds[3] =
            (btlResourceBlock->resC->workEntries[0x15].sourceHeight << 3) - (work->verticalShift << 4);
        btlResourceBlock->resC->workEntries[0x16].geometry.bounds[3] =
            (btlResourceBlock->resC->workEntries[0x16].sourceHeight << 3) - (work->verticalShift << 4);
        btlResourceBlock->resC->workEntries[0x17].geometry.bounds[3] =
            (btlResourceBlock->resC->workEntries[0x17].sourceHeight << 3) - (work->verticalShift << 4);
    }
    func_002BF438(work->initial[1].x << 4, (work->initial[1].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    colors.values[0] = work->fadeLevels[1] | 0x80808000;
    colors.values[1] = work->fadeLevels[0] | 0x80808000;
    colors.values[2] = work->fadeLevels[3] | 0x80808000;
    colors.values[3] = work->fadeLevels[2] | 0x80808000;
    func_002BF438(work->initial[0].x << 4, (work->initial[0].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    colors.values[0] = work->fadeLevels[2] | 0x80808000;
    colors.values[1] = work->fadeLevels[3] | 0x80808000;
    colors.values[2] = work->fadeLevels[1] | 0x80808000;
    colors.values[3] = work->fadeLevels[0] | 0x80808000;
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    func_002BF438((0x100 - width / 2) << 4, (work->initial[1].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    if (work->verticalShift > 0) {
        btlResourceBlock->resC->workEntries[0x15].geometry.bounds[3] =
            btlResourceBlock->resC->workEntries[0x15].sourceHeight << 3;
        btlResourceBlock->resC->workEntries[0x16].geometry.bounds[3] =
            btlResourceBlock->resC->workEntries[0x16].sourceHeight << 3;
        btlResourceBlock->resC->workEntries[0x17].geometry.bounds[3] =
            btlResourceBlock->resC->workEntries[0x17].sourceHeight << 3;
    }
    return 0;
}

extern void evtSetDrawSurfaceIndex(u32);

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);

extern void evtSubmitPrimaryAlphaBlendMode(s32);

extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *work) {
    s16 *fade;
    s32 i;

    for (i = 3, fade = work->fadeLevels; i >= 0; i--, fade++) {
        *fade += 0x20;
        *fade = *fade <= 0 ? 0 : *fade > 0x80 ? 0x80 : *fade;
    }
    switch (work->phase) {
    case 0:
        work->current[0].x = work->initial[0].x + 0x10;
        work->current[0].y = work->initial[0].y;
        work->current[1].x = work->initial[1].x - 0x10;
        work->current[1].y = work->initial[1].y;
        work->phase++;
        break;
    case 1:
        work->current[0].x -= 2;
        work->current[0].x =
            work->current[0].x <= work->width - work->width / 2 + 0x105 ? work->width - work->width / 2 + 0x105 :
            work->width - work->width / 2 + 0x115 <= work->current[0].x ? work->width - work->width / 2 + 0x115 :
            work->current[0].x;
        work->current[1].x += 2;
        work->current[1].x =
            work->current[1].x <= 0x82 - work->width / 2 ? 0x82 - work->width / 2 :
            0x92 - work->width / 2 <= work->current[1].x ? 0x92 - work->width / 2 :
            work->current[1].x;
        break;
    }
    if (work->fadeLevels[1] < 0x80) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        evtSubmitDefaultDepthGradientRect(0x100 - work->width / 2,
            work->current[0].y + 0x1A, work->current[0].x - 0x92, 2,
            (work->fadeLevels[1] << 23) | 0x808080, 0,
            (work->fadeLevels[1] << 23) | 0x808080, 0);
        evtSubmitDefaultDepthGradientRect(work->current[1].x,
            work->current[1].y + 2, work->width / 2 - work->current[1].x + 0x100, 2,
            0, (work->fadeLevels[1] << 23) | 0x808080,
            0, (work->fadeLevels[1] << 23) | 0x808080);
    }
}

void btlFreeRegisteredTaskData(KwlnTask *task) {
    btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(1, 0);
}

void btlToggleModelFlagOnInput(void) {
    if (D_00324530[13] < 0) {
        if (mdlFlagTest(0xC0E)) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

s32 btlDrawTimedDialogTask(KwlnTask *task) {
    BattlePanelColors colors = D_003A2A90;
    MsgQueueTaskData *data;
    s32 expired;
    s32 width;
    s32 half;
    s32 i;

    if (btlGetTrackedTaskHandle(0) == 0) {
        return 0;
    }
    data = (MsgQueueTaskData *)kwlnTaskGetUserValue(task);
    if (data->counter++ < data->value) {
        expired = 0;
    } else {
        expired = 1;
    }
    width = itfMesMeasureEntryItem(data->id, data->unk08, 0);
    data->fade = 0x80;
    for (i = 0; i < 4; i++) {
        colors.values[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x15, i, data->fade);
    }
    half = width / 32;
    func_002BF438(((width >> 4) - half + 0x105) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = (width >> 4) << 4;
    func_002BF438((0x100 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_002BF438((0x92 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    if (expired) {
        return -1;
    }
    itfMesBlk24MoveTo(data->id, (0x100 - (width >> 5)) << 4, 0x220);
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(0);
    itfMesStartEntry(data->id, data->unk08, 0);
    return 0;
}

void btlReleaseDialogTaskData(KwlnTask *task) {
    s32 *entry;
    btlGetRuntime();
    entry = (s32 *)kwlnTaskGetUserValue(task);
    itfMesCleanupWindow(entry[1], 0);
    sdfReleaseChipBlock(entry);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)sdfAllocAndClearQuadwords(0x40);
    D_003BB3DC = (u32)window;
    *(s32 *)(window + 0x10) = 0x14;
    *(s32 *)(window + 0x18) = 0x1800080;
    *(s32 *)(window + 0x1C) = 0x40800080;
    *(s32 *)(window + 0x20) = 0x40800080;
    *(s32 *)(window + 0x24) = 0x60808080;
    *(s32 *)(window + 0x38) = 0xBB;
    *(s32 *)(window + 0x3C) = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B4D58);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AF0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B4F10);

void btlReleaseRegisteredTaskBuffer(s64 unused) {
    btlGetRuntime();
    sdfReleaseChipBlock((void *)D_003BB3DC);
    D_003BB3DC = 0;
    btlSetTrackedTaskHandle(4, 0);
}

s32 sndAreSlotsEmpty(void) {
    u32 *slot = datGameState->battleFlags;
    s32 i;
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            return 0;
        }
    }
    return 1;
}

extern u8 D_00359160[];

s32 func_001B53E8(s32 arg0) {
    s32 count;
    s32 i;

    func_001ACDF0();
    count = func_001A3740(arg0, D_00359160);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (func_001ACD30(*(u16 *)(D_00359160 + 4 + i * 12), 2) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return count;
}

extern s32 btlGetTaskState6(void);

INCLUDE_ASM(const s32, "game/code_001A9780", btlGetTaskState6);

typedef struct FlagEntry {
    u32 unk0;
    u16 id;
    u8 pad6[6];
} FlagEntry;

extern u8 *func_001BD708(u8 *, u16 *);

s32 btlClearFlagEntries(void) {
    u16 count;
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    FlagEntry *entries;
    s32 i;
    if (task != 0) {
        entries = (FlagEntry *)func_001BD708((u8 *)kwlnTaskGetUserValue(task), &count);
        for (i = 0; i < count; i++) {
            func_001ACD30(entries[i].id, 0);
        }
        return 1;
    }
    return 0;
}

extern const char *D_003BB3D0;

extern const char *D_003BB3D4;

s32 btlDestroyTaskC(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_003BB3D0);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0xC), 0);
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2B20);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B55A8);

void btlFinishTrackedBattleTaskAndCloseWindow(KwlnTask *task) {
    s32 context = btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(12, 0);
    *(u32 *)(context + 0x1F4) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2B50);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B5970);

s32 btlDestroyTaskD(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_003BB3D4);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0xD), 0);
        }
        return 1;
    }
    return 0;
}

void btlReleaseWindowTask(KwlnTask *task) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(13, 0);
    battle->flags |= 0x100000;
    btlTrackedTaskHandles->status.bytes.blocked = 0;
    evtFinishMessageWindowAndNotify();
}

extern u8 btlSoundSlotDefaults[];

void btlInitializeSelectionWork(void) {
    u8 initial[0x20];
    BattleSelectionWork *allocated;
    u32 *source;
    s32 *destination;
    s16 *state;
    s32 i;
    memcpy(initial, btlSoundSlotDefaults, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    btlLinkedSelectionTaskBuffer = allocated;
    state = allocated->rowFade;
    /* Retail copies coordinate pairs with its cursor on each row's Y word. */
    destination = &allocated->positions[0].y;
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    btlSetTrackedTaskHandle(2, 1);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", btlSoundSlotDefaults);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B5CD8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B60E8);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfReleaseChipBlock(btlLinkedSelectionTaskBuffer);
    }
    btlSetTrackedTaskHandle(2, 0);
}

/* Initial screen positions for the three active actor panels. */
typedef struct BattleActorPanelInitialPositions {
    s32 entries[3][2];
} BattleActorPanelInitialPositions;

extern const BattleActorPanelInitialPositions D_003A2C10;
extern s32 func_001B6CF8(KwlnTask *);
extern void btlReleaseStwrPanelResource(KwlnTask *);

void func_001B6308(void) {
    BattleActorPanelInitialPositions positions = D_003A2C10;
    BtlState *battle;
    struct SdfMemBlock *allocation;
    BattleActorPanelWork *work;
    s32 slot;
    KwlnTask *task;

    battle = (BtlState *)btlGetRuntime();
    allocation = sdfAllocGeneralBlock(sizeof(*work));
    work = (BattleActorPanelWork *)sdfResourceRetainAddress(allocation);
    memset(work, 0, sizeof(*work));
    work->allocation = allocation;
    work->activeCount = btlCountEligibleLinkedActors(battle);
    for (slot = 0; slot < 3; slot++) {
        work->activeEntries[slot].presentation.hpBarPulse.alpha = 0x80;
        work->activeEntries[slot].presentation.mpBarPulse.alpha = 0x80;
        work->activeEntries[slot].position[0] = positions.entries[slot][0];
        work->activeEntries[slot].position[1] = positions.entries[slot][1];
        work->activeEntries[slot].basePosition[0] = positions.entries[slot][0];
        work->activeEntries[slot].basePosition[1] = positions.entries[slot][1];
        work->activeEntries[slot].presentation.hpPulseFrame = 0;
        work->activeEntries[slot].presentation.mpPulseFrame = 0;
        work->activeEntries[slot].presentation.unk08[0] = 0;
        work->activeEntries[slot].presentation.unk08[1] = 180;
        work->activeEntries[slot].presentation.highlightPhase[4] = 90;
        work->activeEntries[slot].presentation.highlightPhase[5] = 180;
        work->activeEntries[slot].presentation.highlightPhase[6] = 0;
        work->activeEntries[slot].presentation.highlightPhase[7] = 90;
        work->activeEntries[slot].presentation.highlightPhase[0] = 90;
        work->activeEntries[slot].presentation.highlightPhase[1] = 0;
        work->activeEntries[slot].presentation.highlightPhase[2] = 180;
        work->activeEntries[slot].presentation.highlightPhase[3] = 90;
        work->activeEntries[slot].presentation.hpState = 2;
        work->activeEntries[slot].presentation.mpState = 2;
    }
    task = kwlnTaskCreate(D_003BB3B0, 0x2B0E, 1, 1, func_001B6CF8,
                          btlReleaseStwrPanelResource, (u32)work);
    func_00101A80(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(3, (s32)task);
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B6498);

typedef struct BattleActorPanelPositions {
    BattleSelectionPosition entries[3];
} BattleActorPanelPositions;

extern const BattleActorPanelPositions D_003A2C28;

extern s32 func_001B6498(s8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C28);

void func_001B6850(BtlState *battle, BattleActorPanelWork *work) {
    BattleActorPanelPositions positions = D_003A2C28;
    BtlUnit *actor;
    KwlnTask *task;
    BattleSceneObject *scene;
    s32 eligibleCount;
    s32 ordinal;
    s32 slot;
    s32 limit;

    eligibleCount = btlCountEligibleLinkedActors(battle);
    actor = battle->units;
    task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    func_001B6498(0);
    if (task == NULL) {
        ordinal = 0;
        while (actor != NULL && ordinal < eligibleCount) {
                if (btlHasRequiredActorStatusBits(actor)) {
                    slot = actor->lookupId;
                    switch (btlTrackedTaskHandles->presentationState) {
                    case 1:
                        work->activeEntries[slot].presentation.fade -= btlTrackedTaskHandles->status.bytes.fadeStep;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        work->activeEntries[slot].presentation.presentationState = 4;
                        work->activeEntries[slot].presentation.pendingSceneState = 5;
                        work->activeEntries[slot].presentation.transitionState = 4;
                        break;
                    case 0:
                    case 2:
                        work->activeEntries[slot].presentation.fade += 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        break;
                    }
                    ordinal++;
                }
                actor = actor->next;
        }
    } else {
        ordinal = 0;
        scene = (BattleSceneObject *)kwlnTaskGetUserValue(task);
        while (actor != NULL && ordinal < eligibleCount) {
                if (btlHasRequiredActorStatusBits(actor)) {
                    slot = actor->lookupId;
                    switch (scene->state) {
                    case 1:
                        work->activeEntries[slot].presentation.fade += 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        break;
                    case 2:
                        work->activeEntries[slot].presentation.fade += 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        break;
                    case 3:
                        work->activeEntries[slot].presentation.fade += 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        break;
                    case 7:
                    case 9:
                        work->activeEntries[slot].presentation.fade += 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        work->activeEntries[slot].basePosition[0] += 2;
                        limit = positions.entries[slot].x;
                        work->activeEntries[slot].basePosition[0] = work->activeEntries[slot].basePosition[0] <= limit - 8 ? limit - 8 :
                            work->activeEntries[slot].basePosition[0] < limit ? work->activeEntries[slot].basePosition[0] : limit;
                        if (scene->state == 9) {
                            func_001B6498(1);
                        }
                        break;
                    case 11:
                        work->activeEntries[slot].presentation.fade -= btlTrackedTaskHandles->status.bytes.fadeStep;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        work->activeEntries[slot].presentation.presentationState = 4;
                        work->activeEntries[slot].presentation.pendingSceneState = 5;
                        work->activeEntries[slot].presentation.transitionState = 4;
                        break;
                    case 6:
                    case 8:
                        work->activeEntries[slot].presentation.fade -= 0x20;
                        work->activeEntries[slot].presentation.fade = work->activeEntries[slot].presentation.fade <= 0 ? 0 :
                            work->activeEntries[slot].presentation.fade > 0x80 ? 0x80 : work->activeEntries[slot].presentation.fade;
                        work->activeEntries[slot].presentation.presentationState = 4;
                        work->activeEntries[slot].presentation.pendingSceneState = 5;
                        if (work->activeEntries[slot].presentation.fade <= 0) {
                            work->activeEntries[slot].basePosition[0] = positions.entries[slot].x - 6;
                        }
                        break;
                    }
                    ordinal++;
                }
                actor = actor->next;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B6CF8);

void btlReleaseStwrPanelResource(KwlnTask *arg0) {
    u32 temp_v0;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(*(s32 *)temp_v0));
    btlSetTrackedTaskHandle(3, 0);
}

s32 btlAreLinkedSceneCountersAtThreshold(void) {
    BattleSelectionWork *base;
    s32 i;
    if (btlGetTrackedTaskHandle(8) == 0) {
        if (btlGetTrackedTaskHandle(2) != 0) {
            base = btlLinkedSelectionTaskBuffer;
            for (i = 0; i < 4; i++) {
                if (base->rowFade[i] < 0x80) {
                    return 0;
                }
                if (base->positions[i].x < 11) {
                    return 0;
                }
            }
            if (base->positions[1].y == base->positions[0].y + 23) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct BattleReservePositions {
    s32 entries[4][2]; /* x, y */
} BattleReservePositions;

extern const BattleReservePositions D_003A2C70;
extern const char *D_003BB3B4;
extern s32 func_001B7F50(KwlnTask *);
extern void btlReleaseTrackedTaskResource(void);

/* Build the reserve actor panels: place one row per inactive party member and start the panel task. */
void func_001B7238(void) {
    s32 i;
    BattleReservePositions positions = D_003A2C70;
    BattleController *controller = (BattleController *)btlGetRuntime();
    BattleActorPanelWork *work = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3B0));
    struct SdfMemBlock *allocation;
    void *memory;
    KwlnTask *task;

    work->reserveCount = 0;
    work->selectedReserveIndex = 0;
    memset(&work->reserveEntries[0], 0, sizeof(BattleActorPanelEntry));
    memset(&work->reserveEntries[1], 0, sizeof(BattleActorPanelEntry));
    memset(&work->reserveEntries[2], 0, sizeof(BattleActorPanelEntry));
    memset(&work->reserveEntries[3], 0, sizeof(BattleActorPanelEntry));
    for (i = 0; i < datGameState->partyCount; i++) {
        if (datGameState->party[i].flags & 2) {
            work->reserveCount++;
        }
    }
    work->reserveCount = datGameState->partyCount - work->reserveCount;
    work->activeCount = datGameState->partyCount - work->reserveCount;
    for (i = 0; i < work->reserveCount; i++) {
        work->reserveEntries[i].position[0] = positions.entries[i][0];
        work->reserveEntries[i].position[1] = positions.entries[i][1];
        work->reserveEntries[i].presentation.hpBarPulse.alpha = 0x80;
        work->reserveEntries[i].presentation.mpBarPulse.alpha = 0x80;
        work->reserveEntries[i].presentation.hpPulseFrame = 0;
        work->reserveEntries[i].presentation.mpPulseFrame = 0;
        work->reserveEntries[i].presentation.unk08[0] = 0;
        work->reserveEntries[i].presentation.unk08[1] = 0xB4;
        work->reserveEntries[i].presentation.highlightPhase[4] = 0x5A;
        work->reserveEntries[i].presentation.highlightPhase[5] = 0xB4;
        work->reserveEntries[i].presentation.highlightPhase[6] = 0;
        work->reserveEntries[i].presentation.highlightPhase[7] = 0x5A;
        work->reserveEntries[i].presentation.highlightPhase[0] = 0x5A;
        work->reserveEntries[i].presentation.highlightPhase[1] = 0;
        work->reserveEntries[i].presentation.highlightPhase[2] = 0xB4;
        work->reserveEntries[i].presentation.highlightPhase[3] = 0x5A;
        work->reserveEntries[i].presentation.hpState = 2;
        work->reserveEntries[i].presentation.mpState = 2;
    }
    allocation = sdfAllocGeneralBlock(0x348);
    memory = (void *)sdfResourceRetainAddress(allocation);
    memset(memory, 0, 0x348);
    work->reserveUnitAllocation = allocation;
    task = kwlnTaskCreate(D_003BB3B4, 0x2B0E, 1, 1, func_001B7F50, (TaskDestroy)btlReleaseTrackedTaskResource, (u32)memory);
    func_00101A80(controller->taskParent, task);
    btlSetTrackedTaskHandle(8, (s32)task);
    btlSlotBankPromoteStates(work);
    work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B74C8);

void btlSlotBankPromoteStates(BattleActorPanelWork *bank) {
    s32 i;
    for (i = 0; i < bank->reserveCount; i++) {
        BattleActorPanelPresentation *slot = &bank->reserveEntries[i].presentation;
        u8 state = slot->presentationState;
        if (state - 1U < 2U) {
            slot->presentationState = 4;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B7880);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B7C90);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B7F50);

void btlReleaseTrackedTaskResource(void) {
    KwlnTask *temp_v0;
    u32 temp_v1;

    temp_v0 = kwlnTaskGetTaskByName(D_003BB3B0);
    temp_v1 = kwlnTaskGetUserValue(temp_v0);
    sdfReleaseResourceAllocation((struct SdfMemBlock *)(u32)(*(s32 *)(temp_v1 + 0x1200)));
    btlSetTrackedTaskHandle(8, 0);
}

extern void func_001B8838(BattleActorPanelWork *, s8);

extern void btlUpdateActorSlotStates(u8 *, s8);

extern void func_001B8BB0(BtlTask *, BattleActorPanelWork *, s32);

void func_001B83D8(BtlTask *task, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *actor = battle->units;
    KwlnTask *panelTask;
    BattleActorPanelWork *panel;

    for (; actor != NULL; actor = actor->next) {
        if (btlHasRequiredActorStatusBits(actor)) {
            slot = actor->lookupId;
            if (task->unit->identity == actor->identity) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        panelTask = kwlnTaskGetTaskByName(D_003BB3B0);
        if (panelTask != NULL) {
            panel = (BattleActorPanelWork *)kwlnTaskGetUserValue(panelTask);
            func_001B8838(panel, mode);
            btlUpdateActorSlotStates((u8 *)panel, 0);
            panel->activeEntries[slot].presentation.presentationState = 2;
            panel->activeEntries[slot].presentation.presentationValue = value;
            if (mode == 0) {
                func_001B8BB0(task, panel, 0);
            } else if (mode == 2) {
                func_001B8BB0(task, panel, 1);
            }
        }
    }
}

typedef struct BtlSlotRow {
    u8 pad_00[0x10];
    u8 state;
    u8 value;
} BtlSlotRow;

void btlUpdateActorSlotPresentationState(BtlUnit *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    BtlSlotRow *slotEntry;
    s32 offset;
    for (; node != 0; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (object->identity == node->identity) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        KwlnTask *task = kwlnTaskGetTaskByName(D_003BB3B0);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                func_001B8838((BattleActorPanelWork *)entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (BtlSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

void func_001B8650(BtlUnit *object, s32 unused, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    KwlnTask *task;
    BattleActorPanelWork *work;

    for (; node != NULL; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (object->identity == node->identity) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        task = kwlnTaskGetTaskByName(D_003BB3B0);
        if (task != NULL) {
            work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
            work->activeEntries[slot].presentation.transitionState = 2;
            work->activeEntries[slot].presentation.secondaryPresentationValue = value;
            work->activeEntries[slot].presentation.trianglePhase[4] = 90;
            work->activeEntries[slot].presentation.trianglePhase[5] = 180;
            work->activeEntries[slot].presentation.trianglePhase[6] = 0;
            work->activeEntries[slot].presentation.trianglePhase[7] = 90;
            work->activeEntries[slot].presentation.trianglePhase[0] = 90;
            work->activeEntries[slot].presentation.trianglePhase[1] = 0;
            work->activeEntries[slot].presentation.trianglePhase[2] = 180;
            work->activeEntries[slot].presentation.trianglePhase[3] = 90;
        }
    }
}

void btlResetActorSlotPresentationValue(BtlUnit *object, BattleSceneObject *sceneObject) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    s32 offset;
    for (; node != 0; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = object->lookupId;
            if (object->identity == node->identity) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3B0));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

void func_001B8838(BattleActorPanelWork *work, s8 mode) {
    s32 slot;
    s32 remaining;

    slot = 0;
    remaining = 2;
    do {
        if ((u32)((u8)work->activeEntries[slot].presentation.presentationState - 1) < 2) {
            if (mode == 0) {
                work->activeEntries[slot].presentation.presentationState = 3;
            } else if (mode != 2) {
                work->activeEntries[slot].presentation.presentationState = 4;
            }
        }
        if (work->activeEntries[slot].presentation.hpState == 3) {
            work->activeEntries[slot].presentation.hpState = 0;
            work->activeEntries[slot].presentation.hpPulseFrame = 0;
            memset(&work->activeEntries[slot].presentation.hpBarPulse, 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[0], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[1], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[2], 0, sizeof(BattleStatPulse));
        }
        if (work->activeEntries[slot].presentation.mpState == 3) {
            work->activeEntries[slot].presentation.mpState = 0;
            work->activeEntries[slot].presentation.mpPulseFrame = 0;
            memset(&work->activeEntries[slot].presentation.mpBarPulse, 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[0], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[1], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[2], 0, sizeof(BattleStatPulse));
        }
        slot++;
    } while (--remaining >= 0);
}

void func_001B89B0(BattleActorPanelWork *work, s8 mode) {
    s32 slot;
    s32 remaining;

    slot = 0;
    remaining = 2;
    do {
        if ((u32)((u8)work->activeEntries[slot].presentation.presentationState - 1) < 2 &&
            work->activeEntries[slot].presentation.presentationValue != 0) {
            work->activeEntries[slot].presentation.presentationState = mode == 0 ? 3 : 4;
        }
        if (work->activeEntries[slot].presentation.hpState == 3) {
            work->activeEntries[slot].presentation.hpState = 0;
            work->activeEntries[slot].presentation.hpPulseFrame = 0;
            memset(&work->activeEntries[slot].presentation.hpBarPulse, 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[0], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[1], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.hpBarPulses[2], 0, sizeof(BattleStatPulse));
        }
        if (work->activeEntries[slot].presentation.mpState == 3) {
            work->activeEntries[slot].presentation.mpState = 0;
            work->activeEntries[slot].presentation.mpPulseFrame = 0;
            memset(&work->activeEntries[slot].presentation.mpBarPulse, 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[0], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[1], 0, sizeof(BattleStatPulse));
            memset(&work->activeEntries[slot].presentation.mpBarPulses[2], 0, sizeof(BattleStatPulse));
        }
        slot++;
    } while (--remaining >= 0);
}

void btlUpdateActorSlotStates(u8 *context, s8 mode) {
    u8 *entry = context + 0x80;
    s32 modeZeroState = 3;
    s32 modeNonzeroState = 4;
    s32 i = 2;
    do {
        s32 state = entry[0xC];
        if (state == 1 || state == 2) {
            entry[0xC] = mode == 0 ? modeZeroState : modeNonzeroState;
        }
        i--;
        entry += 0x290;
    } while (i >= 0);
}

typedef struct UiSlotEntry {
    u8 pad00[0x18];
    s8 state;
    u8 pad19[0x277];
} UiSlotEntry;

void btlAdvancePendingSceneSlotStates(u8 *scene) {
    UiSlotEntry *entry = (UiSlotEntry *)(scene + 0xE0);
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->state == 1) {
            entry->state = 5;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B8BB0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B8CB8);

extern const BattlePanelColors D_003A2D00;

void func_001B91F0(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_003A2D00;
    s32 x = work->activeEntries[slot].position[0];
    s32 y = work->activeEntries[slot].position[1];
    s32 state = work->activeEntries[slot].presentation.pendingSceneState;
    u32 color;
    s32 i;

    if (state > 0 && (state < 4 || state == 5)) {
        color = work->activeEntries[slot].presentation.unkF0 | 0x80808000;
        for (i = 3; i >= 0; i--) {
            colors.values[i] = color;
        }
        func_002BF438((x + work->activeEntries[slot].presentation.cursorOffset[0]) << 4,
                      (y + work->activeEntries[slot].presentation.cursorOffset[1]) << 3, 0,
                      colors.values, 0, btlResourceBlock->resA, work->activeEntries[slot].presentation.cursorOption,
                      0x53);
    }
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C70);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C90);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2CB0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2CC0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2CD0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2CE8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D00);

void func_001B9318(BtlUnit *unit, BattleActorPanelWork *work, s32 slot, s8 reserve) {
    s32 i;

    switch (reserve == 0 ? work->activeEntries[slot].presentation.presentationState :
                           work->reserveEntries[slot].presentation.presentationState) {
    case 1:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                work->activeEntries[slot].presentation.highlightPhase[i] =
                    (work->activeEntries[slot].presentation.highlightPhase[i] + 8) % 360;
                work->activeEntries[slot].presentation.highlightLevel[i] =
                    (sdfSinPoly(((work->activeEntries[slot].presentation.highlightPhase[i] + 90) % 360) /
                               180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f;
            } else {
                work->reserveEntries[slot].presentation.highlightPhase[i] =
                    (work->reserveEntries[slot].presentation.highlightPhase[i] + 8) % 360;
                work->reserveEntries[slot].presentation.highlightLevel[i] =
                    (sdfSinPoly(((work->reserveEntries[slot].presentation.highlightPhase[i] + 90) % 360) /
                               180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f;
            }
        }
        break;
    case 2:
        break;
    case 3:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                if (work->activeEntries[slot].presentation.highlightLevel[i] != 0) {
                    work->activeEntries[slot].presentation.highlightLevel[i]--;
                }
            } else if (work->reserveEntries[slot].presentation.highlightLevel[i] >= 32) {
                work->reserveEntries[slot].presentation.highlightLevel[i] -= 32;
            } else {
                work->reserveEntries[slot].presentation.highlightLevel[i] = 0;
            }
        }
        break;
    case 0:
    case 4:
        for (i = 0; i < 8; i++) {
            if (reserve == 0) {
                if (work->activeEntries[slot].presentation.highlightLevel[i] >= 32) {
                    work->activeEntries[slot].presentation.highlightLevel[i] -= 32;
                } else {
                    work->activeEntries[slot].presentation.highlightLevel[i] = 0;
                }
            } else if (work->reserveEntries[slot].presentation.highlightLevel[i] >= 32) {
                work->reserveEntries[slot].presentation.highlightLevel[i] -= 32;
            } else {
                work->reserveEntries[slot].presentation.highlightLevel[i] = 0;
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B96F8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B9A50);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B9E98);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D28);

void func_001BA198(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    s32 i;

    switch (work->activeEntries[slot].presentation.transitionState) {
    case 1:
    case 2: {
        for (i = 0; i < 8; i++) {
            work->activeEntries[slot].presentation.trianglePhase[i] =
                (work->activeEntries[slot].presentation.trianglePhase[i] + 8) % 360;
            work->activeEntries[slot].presentation.triangleAlpha[i] =
                (u32)((sdfSinPoly((f32)((work->activeEntries[slot].presentation.trianglePhase[i] + 90) % 360) /
                                  180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 16.0f);
        }
        break;
    }
    case 3:
        for (i = 0; i < 8; i++) {
            if (work->activeEntries[slot].presentation.triangleAlpha[i] != 0) {
                work->activeEntries[slot].presentation.triangleAlpha[i]--;
            }
        }
        break;
    case 0:
    case 4: {
        s32 index;
        for (i = 7, index = 0; i >= 0; i--, index++) {
            u8 alpha = work->activeEntries[slot].presentation.triangleAlpha[index];
            work->activeEntries[slot].presentation.triangleAlpha[index] =
                alpha < 32 ? 0 : alpha - 32;
        }
        break;
    }
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BA408);

void func_001BA660(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    s32 i;

    switch (work->activeEntries[slot].presentation.transitionState) {
    case 3:
        work->activeEntries[slot].presentation.secondaryGeometry[0]++;
        work->activeEntries[slot].presentation.secondaryGeometry[1]--;
        work->activeEntries[slot].presentation.secondaryGeometry[2]--;
        work->activeEntries[slot].presentation.secondaryGeometry[3]++;
    case 4:
        for (i = 0; i < 2; i++) {
            if (work->activeEntries[slot].presentation.secondaryFade[i] >= 16) {
                work->activeEntries[slot].presentation.secondaryFade[i] -= 16;
            } else {
                work->activeEntries[slot].presentation.secondaryFade[i] = 0;
            }
            if (work->activeEntries[slot].presentation.secondaryPulseLevel[i] >= 16) {
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] -= 16;
            } else {
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] = 0;
            }
        }
        if (work->activeEntries[slot].presentation.secondaryFade[0] < 9) {
            work->activeEntries[slot].presentation.transitionState = 0;
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            switch (work->activeEntries[slot].presentation.secondaryPulseDirection[i]) {
            case 0:
                work->activeEntries[slot].presentation.secondaryPulseTimer[i]++;
                work->activeEntries[slot].presentation.secondaryPulseTimer[i] =
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] <= 0 ? 0 :
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] > 10 ? 10 :
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i];
                if (work->activeEntries[slot].presentation.secondaryPulseTimer[i] > 0) {
                    work->activeEntries[slot].presentation.secondaryPulseDirection[i] = 1;
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] = 0;
                }
                break;
            case 1:
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] -= 2;
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] =
                    work->activeEntries[slot].presentation.secondaryPulseLevel[i] <= 0 ? 0 :
                    work->activeEntries[slot].presentation.secondaryPulseLevel[i] > 128 ? 128 :
                    work->activeEntries[slot].presentation.secondaryPulseLevel[i];
                if (work->activeEntries[slot].presentation.secondaryPulseLevel[i] <= 0) {
                    work->activeEntries[slot].presentation.secondaryPulseDirection[i] = 2;
                }
                break;
            case 2:
            default:
                work->activeEntries[slot].presentation.secondaryPulseTimer[i]++;
                work->activeEntries[slot].presentation.secondaryPulseTimer[i] =
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] <= 0 ? 0 :
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] > 12 ? 12 :
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i];
                if (work->activeEntries[slot].presentation.secondaryPulseTimer[i] > 0) {
                    work->activeEntries[slot].presentation.secondaryPulseDirection[i] = 0;
                    work->activeEntries[slot].presentation.secondaryPulseTimer[i] = 0;
                    /* Native keeps separate channel reset paths with equal levels. */
                    if (i == 0) {
                        work->activeEntries[slot].presentation.secondaryPulseLevel[i] = 48;
                    } else {
                        work->activeEntries[slot].presentation.secondaryPulseLevel[i] = 48;
                    }
                }
                break;
            }
        }
        break;
    case 2:
        work->activeEntries[slot].presentation.secondaryGeometry[0] = 68;
        work->activeEntries[slot].presentation.secondaryGeometry[1] = 38;
        work->activeEntries[slot].presentation.secondaryGeometry[2] = 5;
        work->activeEntries[slot].presentation.secondaryGeometry[3] = 64;
        for (i = 0; i < 2; i++) {
            work->activeEntries[slot].presentation.secondaryFade[i] += 16;
            work->activeEntries[slot].presentation.secondaryFade[i] =
                work->activeEntries[slot].presentation.secondaryFade[i] <= 0 ? 0 :
                work->activeEntries[slot].presentation.secondaryFade[i] >= 128 ? 128 :
                work->activeEntries[slot].presentation.secondaryFade[i];
            work->activeEntries[slot].presentation.secondaryPulseLevel[i] += 8;
            work->activeEntries[slot].presentation.secondaryPulseLevel[i] =
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] <= 0 ? 0 :
                work->activeEntries[slot].presentation.secondaryPulseLevel[i] > 48 ? 48 :
                work->activeEntries[slot].presentation.secondaryPulseLevel[i];
        }
        if (work->activeEntries[slot].presentation.secondaryFade[0] >= 128) {
            work->activeEntries[slot].presentation.transitionState = 1;
            work->activeEntries[slot].presentation.secondaryPulseDirection[0] = 0;
            work->activeEntries[slot].presentation.secondaryPulseDirection[1] = 0;
            work->activeEntries[slot].presentation.secondaryPulseTimer[0] = 0;
            work->activeEntries[slot].presentation.secondaryPulseTimer[1] = 0;
            work->activeEntries[slot].presentation.secondaryPulseOffsets[0][0] = 2;
            work->activeEntries[slot].presentation.secondaryPulseOffsets[1][0] = 3;
            work->activeEntries[slot].presentation.secondaryPulseOffsets[0][1] = 2;
            work->activeEntries[slot].presentation.secondaryPulseOffsets[1][1] = 3;
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BAB08);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BAE08);

extern const BattlePanelColors D_003A2D68;

void func_001BB118(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat) {
    BattlePanelColors colors = D_003A2D68;
    f32 value;
    f32 maximum;
    s32 xOffset;
    s32 yOffset;
    s32 sprite;
    s32 targetProgress;
    s32 remaining;
    s32 i;

    if (!(stat & 1)) {
        value = unit->partyRecord.hp;
        maximum = unit->partyRecord.maxHp;
        xOffset = 57;
        yOffset = 49;
        sprite = 3;
    } else {
        value = unit->partyRecord.mp;
        maximum = unit->partyRecord.maxMp;
        xOffset = 28;
        yOffset = 64;
        sprite = 4;
    }
    if (maximum == 0.0f) maximum = 1.0f;
    targetProgress = (s32)(value / maximum * 39.0f);
    if (pulse->active == 0) {
        pulse->progress++;
        pulse->progress = pulse->progress <= 0 ? 0 : pulse->progress >= targetProgress ? targetProgress : pulse->progress;
        remaining = 39 - pulse->progress;
        remaining = remaining <= 0 ? 0 : remaining >= alpha ? alpha : remaining;
        if (pulse->progress >= targetProgress) {
            pulse->active = 1;
            pulse->phase = 260;
        }
        pulse->alpha = alpha - remaining;
    } else {
        pulse->phase = (pulse->phase + 16) % 360;
        pulse->alpha = (s32)((alpha + 119) * ((sdfSinPoly((f32)((pulse->phase + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f) + 8.0f);
        if (pulse->phase >= 160 && pulse->phase <= 180) {
            pulse->active = 0;
            pulse->phase = 0;
            pulse->progress = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        colors.values[i] = (colors.values[i] & 0xFFFFFF00) | pulse->alpha;
    }
    if (!(unit->status.flags & 0x20) && value != 0.0f && !(unit->partyRecord.status & 0x4800)) {
        func_002BF438((x + xOffset + pulse->progress) << 4,
                     (y + yOffset + pulse->yOffset) << 3, 0, colors.values,
                     0, btlResourceBlock->resA, sprite, 0x53);
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BB440);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D68);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BB6C8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BB990);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BBE18);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BC540);

extern SdfPoolNode D_003255A8;

extern s32 sdfAllocPacketAligned(s32 size);

/* Draw a textured command-panel quad with independently colored corners. */
s32 btlDrawGouraudTexturedPanelQuad(s32 x0, s32 y0, s32 x1, s32 y1,
                  s32 x2, s32 y2, s32 x3, s32 y3,
                  s32 u, s32 v, s32 width, s32 height,
                  const s32 *colors, SdfTex *texture) {
    SdfListHead *list;
    s32 uFixed;
    s32 vFixed;
    s32 uRight;
    s32 vBottom;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket(list, texture, 0);
    uFixed = u * 0x10;
    vFixed = v * 0x10;
    uRight = uFixed + width * 0x10;
    vBottom = vFixed + height * 0x10;
    sdfQueueGouraudTexturedQuad(list, 0x40,
        x0 * 0x10 + 0x7000, y0 * 8 + 0x7900, uFixed, vFixed, colors[0],
        x1 * 0x10 + 0x7000, y1 * 8 + 0x7900, uRight, vFixed, colors[1],
        x2 * 0x10 + 0x7000, y2 * 8 + 0x7900, uFixed, vBottom, colors[2],
        x3 * 0x10 + 0x7000, y3 * 8 + 0x7900, uRight, vBottom, colors[3],
        0xFEFFD0, NULL);
    D_003255A8.append(&D_003255A8, list);
    return 1;
}

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    KwlnTask *first;
    KwlnTask *second;
    if (btlTrackedTaskHandles != 0) {
        first = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        second = kwlnTaskGetTaskByName(D_003BB3B0);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (btlTrackedTaskHandles->status.bytes.blocked == 0) {
            return btlTrackedTaskHandles->status.bytes.state;
        }
        return 0;
    }
    return -128;
}

void func_001BCB88(s8 mode, s32 duration) {
    KwlnTask *commandTask = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    KwlnTask *panelTask = kwlnTaskGetTaskByName(D_003BB3B0);
    KwlnTask *registeredTask = kwlnTaskGetTaskByName(D_003BB3C0);

    switch (mode) {
    case 0: {
        s32 step;
        if (commandTask != NULL) {
            ((BattleSceneObject *)kwlnTaskGetUserValue(commandTask))->state = 11;
            btlCommandPanelWork->state = 4;
        }
        if (panelTask != NULL) {
            btlTrackedTaskHandles->presentationState = 1;
        }
        if (registeredTask != NULL) {
            ((BattleRegisteredPanelWork *)kwlnTaskGetUserValue(registeredTask))->mode = 3;
        }
        {
            BattleTrackedTaskWork *tracked = btlTrackedTaskHandles;
            step = 0x80 / duration;
            tracked->threshold = duration;
            tracked->status.bytes.state = 1;
            tracked->counter = 0;
            tracked->status.bytes.fadeStep = step;
        }
        break;
    }
    case 1: {
        s32 step;
        if (commandTask != NULL) {
            ((BattleSceneObject *)kwlnTaskGetUserValue(commandTask))->state = mode;
            btlCommandPanelWork->state = mode;
        }
        if (panelTask != NULL) {
            btlTrackedTaskHandles->presentationState = 2;
        }
        if (registeredTask != NULL) {
            func_001B0E48((BattleRegisteredPanelWork *)kwlnTaskGetUserValue(registeredTask));
        }
        {
            BattleTrackedTaskWork *tracked = btlTrackedTaskHandles;
            step = 0x80 / duration;
            tracked->threshold = duration;
            tracked->status.bytes.state = 3;
            tracked->counter = 0;
            tracked->status.bytes.fadeStep = step;
        }
        break;
    }
    }
}

s32 btlUpdateCommandUiTransition(void) {
    BattleTrackedTaskWork *flow;
    KwlnTask *task;
    s32 counter;

    if (btlTrackedTaskHandles != 0) {
        task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        flow = btlTrackedTaskHandles;
        switch (flow->status.bytes.state) {
        case 1:
            flow->counter++;
            if (flow->counter >= flow->threshold) {
                flow->status.bytes.state = 2;
            }
            break;
        case 3:
            counter = flow->counter + 1;
            flow->counter = counter;
            if (counter < flow->threshold) {
                break;
            }
            counter = counter <= 0 ? 0 :
                (counter < flow->threshold ? counter : flow->threshold);
            flow->counter = counter;
            if (btlAreLinkedSceneCountersAtThreshold() != 0) {
                if (task != 0) {
                    func_001B83D8(((BattleSceneObject *)kwlnTaskGetUserValue(task))->owner, 2, 0);
                }
                btlTrackedTaskHandles->status.bytes.state = 0;
            }
            break;
        }
    }
    return 0;
}

void fldInitializeBattleSceneFlow(void) {
    s32 context = btlGetRuntime();
    btlNextScaledRandom(7);
    if ((*(u32 *)(context + 0x1F4) & 0x400) != 0) {
        if (*(u16 *)(context + 0x24C) == 1) {
            btlInvalidateSceneFadeCounts(0);
            btlClearNodeFlags();
        } else {
            btlInvalidateSceneFadeCounts(1);
            btlClearNodeFlags();
        }
    }
    btlToggleModelFlagOnInput();
    btlUpdateCommandUiTransition();
}

void btlDebugPrintf(const char *fmt, ...) {
}

void func_001BCE90(void) {
}

void func_001BCE98(void) {
}

void fldSubmitSceneObjectAtCoordinates(s32 x, s32 y, u32 color, char *text) {
    u32 glyph;

    itfSetTextDrawLimit(0x13);
    glyph = func_001978E8(x << 4, y << 3, 0, color, text, 0);
    frFontDrawGlyphChain(glyph, 1, 0x53);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)glyph);
    itfSetTextDrawLimit(-1);
}

extern u8 *D_003BAA8C;

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA8C + index * 17, 0);
    frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)handle);
    itfSetTextDrawLimit(-1);
}

extern u8 *D_003BAA84;

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA84 + index * 25, 0);
    frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)handle);
    itfSetTextDrawLimit(-1);
}

s32 btlIsSceneActorLimitSatisfied(s32 unused, u32 limit) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    if (datBattleSceneRecords[index].flags & 0x800) {
        return 1;
    }
    if (limit < (u32)btlCountFlaggedSceneActors()) {
        return 0;
    }
    return 1;
}

extern const s8 D_003BB460[];

extern const s8 D_003BB468[];

extern const s8 D_003BB470[];

/* Resolve the command class through the owning actor's status-selected table. */
s32 func_001BD0D0(BattleSceneObject *object, s8 mode) {
    s32 flags = object->owner->unit->status.flags;
    s8 normal[6];
    s8 alternate[6];
    s8 restricted[6];
    const s8 *classes;

    memcpy(normal, D_003BB460, sizeof(normal));
    memcpy(alternate, D_003BB468, sizeof(alternate));
    memcpy(restricted, D_003BB470, sizeof(restricted));
    if ((flags & 0x400) != 0) {
        classes = restricted;
    } else {
        classes = normal;
        if ((flags & 0x1000) != 0) {
            classes = alternate;
        }
    }
    return classes[mode];
}

extern u16 *func_001BD2C0(s32, s16 *, u16, u16, u16);

extern void btlBuildEligibleActorList(s32, s16 *);

extern u8 *func_001BD708(u8 *, u16 *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DD8);

u32 btlGetCommandOptionCount(s32 object, s8 mode, s8 unlimited) {
    s32 kind = func_001BD0D0((BattleSceneObject *)object, mode);
    s16 count;

    switch (kind) {
    case 0:
        func_001BD2C0(object, &count, 0, 2, 3);
        D_003BD834->value[0] = count + 1;
        break;
    case 4:
        func_001BD708((u8 *)object, (u16 *)&count);
        D_003BD834->value[4] = count;
        break;
    case 2:
        btlBuildEligibleActorList(object, &count);
        D_003BD834->value[2] = count;
        break;
    case 3:
        D_003BD834->value[3] = 3;
        break;
    case 6:
        D_003BD834->value[6] = 1;
        break;
    default:
        D_003BD834->value[kind] = 0;
        break;
    }
    if (unlimited == 0) {
        if (D_003BD834->value[kind] >= 4) {
            return 4;
        }
    }
    return D_003BD834->value[kind];
}

extern u16 D_00359960[12];

extern u16 D_00358B20[0x260];

u16 *func_001BD2C0(s32 address, s16 *outCount, u16 firstKind, u16 secondKind, u16 thirdKind) {
    BattleSceneObject *object = (BattleSceneObject *)address;
    s32 wanted;
    BtlUnit *unit;
    u16 *source;
    u16 *out;
    s32 gathered;
    s32 count;
    DatCommandSelector *selectors;
    DatCommandRecord *records;
    s32 i;

    if (!(btlUnitStatusPair(object->owner->unit) & 0x1400)) {
        wanted = firstKind ? firstKind : 5;
    } else {
        wanted = firstKind;
    }
    btlGetRuntime();
    unit = object->owner->unit;
    out = D_00359960;
    memset(out, 0, 0x18);
    selectors = datCommandSelectors;
    records = datCommandRecords;
    source = unit->partyRecord.effectData;
    gathered = 0;
    for (i = 0; i < 24; i++, source++) {
        u16 id = *source;
        s32 kind = selectors[id].kind;
        if (kind != wanted && kind != secondKind && kind != thirdKind) {
            continue;
        }
        if (wanted == 2 && id == 0xE0) {
            continue;
        }
        if (!(records[id].unk_01 & 2)) {
            continue;
        }
        if (btlUnitStatusPair(object->owner->unit) & 0x1400) {
            if (selectors[id].kind == 5) {
                continue;
            }
        } else if (selectors[id].kind != 5) {
            continue;
        }
        if (id != 0) {
            *out++ = id;
            gathered++;
        }
    }
    count = gathered;
    out = D_00358B20;
    memcpy(out, D_00359960, count * 2);
    i = 0;
    while (i < count) {
        if (out[i] >= 0x200) {
            s32 j;
            for (j = i; j < count; j++) {
                out[j] = out[j + 1];
            }
            count--;
        } else {
            i++;
        }
    }
    if (wanted == 0 || wanted == 5) {
        count++;
    }
    *outCount = count;
    return out;
}

extern u16 D_00359960[];

extern u16 D_00358B20[];

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BD4F0);

extern u8 D_00358FE0[];

void btlBuildEligibleActorList(s32 unused, s16 *count) {
    s32 context = btlGetRuntime();
    s32 index = *(s32 *)(context + 0x27C);
    s32 total = 0;
    if ((datBattleSceneRecords[index].flags & 0x800) == 0) {
        u8 *selected = datGameState->inventory.counts;
        DatItemSkillRecord *items = datItemSkillRecords;
        u8 *out = D_00358FE0;
        s32 i;
        for (i = 0; i < 0xC0; i++, items++, selected++) {
            if (*selected != 0 && (items->flags & 2)) {
                out[0] = i;
                out[1] = *selected;
                out += 2;
                total++;
            }
        }
    }
    *count = total;
}

extern u8 D_00359160[];

u8 *func_001BD708(u8 *object, u16 *value) {
    s32 result = func_001A3740(*(s32 *)(*(u8 **)(object + 0x2C) + 0x18), D_00359160);
    *value = result;
    return D_00359160;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BD750);

extern void btlDrawRetreatCommandLabel(s32);

extern void btlDrawItemCommandRows(BattleSceneObject *);

void fldDispatchSceneKindHandler(s32 arg0) {
    switch (func_001BD0D0((BattleSceneObject *)arg0, btlCommandPanelWork->classIndex)) {
    case 0:
        func_001BDF60(arg0, 0, 2, 3);
        return;
    case 4:
        func_001BEB58(arg0);
        return;
    case 2:
        btlDrawItemCommandRows((BattleSceneObject *)arg0);
        return;
    case 3:
        func_001BE590(arg0);
        return;
    case 6:
        btlDrawRetreatCommandLabel(arg0);
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BDF60);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BE590);

extern const BattlePanelColors D_003A2E90;

extern char D_003BB478[];

extern s32 func_003014F0(char *, const char *, ...);

void btlDrawItemCommandRows(BattleSceneObject *object) {
    char text[16];
    BattlePanelColors colors = D_003A2E90;
    s32 shown;
    s32 total;
    s32 index;
    s32 selected;
    s32 row = 0;
    s32 y = 0x14C;
    s32 color;
    s32 countY;

    shown = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
    total = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 1);
    index = object->selections[2].cursor;
    selected = object->selections[2].entry;
    for (; row < shown && index < total; row++, index++, y += 0x17) {
        color = selected == index ? 0x89FEFF80 : 0xA09DC380;
        color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[row];
        btlQueueIndexedTextWithinDrawLimit(0x1A, y, 0xFF0010, color, D_00358FE0[index * 2]);
        countY = y + 4;
        colors.values[0] = color;
        colors.values[1] = color;
        colors.values[2] = color;
        colors.values[3] = color;
        func_002BF438(0x900, countY << 3, 0, colors.values, 0,
                     btlResourceBlock->resA, 5, 0x53);
        func_003014F0(text, D_003BB478, D_00358FE0[index * 2 + 1]);
        fldSubmitSceneObjectAtCoordinates(0xA0, countY, color, text);
    }
}

void btlDrawRetreatCommandLabel(s32 unused) {
    u8 text[8];
    BattleController *battle;
    s32 color;
    s32 handle;

    memcpy(text, D_003BB450, sizeof(text));
    battle = (BattleController *)btlGetRuntime();
    if (datBattleSceneRecords[battle->mode].unk00 != 0) {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x504F6100;
    } else {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x89FEFF00;
    }
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color, text, 0);
    frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)handle);
    itfSetTextDrawLimit(-1);
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BEB58);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BF040);

extern void func_001B5CD8(void);

extern void func_001B60E8(void);

extern void func_001AADF8(BattleSceneObject *);

extern s32 func_001B4F10(KwlnTask *);

extern s32 func_001BD750(BattleSceneObject *);

extern void func_001B2D80(BattleSceneObject *, s32);

extern s32 btlHasHighPriorityState(void);

extern s32 btlAreLinkedSceneCountersAtThreshold(void);

static inline void btlUpdateSceneCommandSelection(BattleSceneObject *object) {
    fldDispatchSceneKindHandler((s32)object);
    func_001B2D80(object, func_001BD0D0(object, btlCommandPanelWork->classIndex));
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2EA0);

s32 func_001BF0F8(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleSceneObject *object = (BattleSceneObject *)kwlnTaskGetUserValue(task);
    BattleActorPanelWork *panel;
    u8 slot;

    if (!(battle->battleFlags & 0x200)) {
        return 0;
    }
    func_001B5CD8();
    func_001B60E8();
    func_001AADF8(object);
    func_001B4F10(task);
    switch (object->state) {
    case 5:
        return -1;
    case 1:
        func_001BD750(object);
        btlUpdateSceneCommandSelection(object);
        if (btlHasHighPriorityState() && object->state == 1) {
            object->state = 2;
        }
        break;
    case 2:
        func_001BD750(object);
    case 3:
    case 6:
    case 8:
    case 10:
    case 11:
        btlUpdateSceneCommandSelection(object);
        break;
    case 7:
        if (func_001BD750(object)) {
            btlUpdateSceneCommandSelection(object);
        } else {
            btlUpdateSceneCommandSelection(object);
            if (btlAreLinkedSceneCountersAtThreshold()) {
                object->state = 2;
            }
        }
        break;
    case 9:
        btlUpdateSceneCommandSelection(object);
        if (btlAreLinkedSceneCountersAtThreshold()) {
            object->commandData->phase = 9;
            panel = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3B0));
            object->commandData->unk18 = panel->partyRecordIndex;
            object->state = 3;
            ((SceneAiWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_003BB3A0)))->state = 3;
            slot = object->commandData->linkedUnit->lookupId;
            panel->activeEntries[slot].presentation.unkF0 = 0;
            panel->activeEntries[slot].presentation.pendingSceneState = 5;
            panel->activeEntries[slot].presentation.hpState = 3;
            panel->activeEntries[slot].presentation.hpLevel = datGameState->party[panel->partyRecordIndex].hp;
            panel->activeEntries[slot].presentation.hpTarget = panel->activeEntries[slot].presentation.hpLevel;
            panel->activeEntries[slot].presentation.mpState = 3;
            panel->activeEntries[slot].presentation.mpLevel = datGameState->party[panel->partyRecordIndex].mp;
            panel->activeEntries[slot].presentation.mpTarget = panel->activeEntries[slot].presentation.mpLevel;
            panel->activeEntries[slot].presentation.presentationState = 2;
            panel->activeEntries[slot].presentation.presentationValue = 0;
        }
        break;
    }
    return 0;
}

void fldClearBattleSceneObject(KwlnTask *arg0) {
    u32 temp_v0;
    BattleController *battle;

    temp_v0 = kwlnTaskGetUserValue(arg0);
    sdfReleaseChipBlock((void *)temp_v0);
    battle = (BattleController *)btlGetRuntime();
    battle->sceneObjectTask = 0;
    btlReleaseBattleScratchBlocks();
}

extern BattleSceneObject *fldGetSceneObjectTaskUserData(void);

extern s32 func_001BF0F8(KwlnTask *task);

extern s32 func_001B5970(KwlnTask *task);

extern s32 func_001B55A8(KwlnTask *task);

extern void func_001B2AC8(BattleSceneObject *object);

extern u8 D_00358828[];

void fldInitializeSceneObject(BattleSceneObject *object, BtlTask *owner) {
    memset(object, 0, sizeof(*object));
    object->state = 1;
    object->commandData = &owner->indexWork;
    object->owner = owner;
}

BattleSceneObject *fldGetSceneObjectTaskUserData(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task == NULL) {
        return NULL;
    }
    return (BattleSceneObject *)kwlnTaskGetUserValue(task);
}

s32 fldGetSceneObjectState(void) {
    KwlnTask *temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (temp_v0 == 0) {
        return 0;
    }
    return fldGetSceneObjectTaskUserData()->state;
}

void func_001BF4C0(BtlTask *task) {
    BattleController *scene;
    BattleSceneObject *object;
    u32 handle;

    if (kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef) == 0) {
        scene = (BattleController *)btlGetRuntime();
        object = sdfAllocAndClearQuadwords(sizeof(*object));
        fldInitializeSceneObject(object, task);
        handle = (u32)kwlnTaskCreate(btlCommandPanelTaskNameRef, 0x2B0E, 1, 1,
                               func_001BF0F8, fldClearBattleSceneObject, (u32)object);
        func_00101A80(scene->taskParent, (KwlnTask *)handle);
        scene->sceneObjectTask = handle;
        func_001B83D8(task, 0, 0);
        btlInitializeSelectionWork();
        btlInitializeCommandPanelSlotTables();
        btlCreateMessageWindow();
        func_001B2AC8(object);
        if ((task->unit->status.flags & 0x200) && !(scene->flags & 0x1000000)) {
            if (mdlFlagTest(0x81B) == 0) {
                mdlFlagSet(0x81B);
                btlTrackedTaskHandles->status.bytes.blocked = 1;
                scene->flags &= ~0x100000;
                handle = (u32)kwlnTaskCreate(D_003BB3D4, 0x2B0E, 1, 1,
                                       func_001B5970, btlReleaseWindowTask,
                                       (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101A80(scene->taskParent, (KwlnTask *)handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00358828);
                dspStartEntry(2);
                func_001BCB88(0, 8);
            } else if (btlGetTaskState6() != 0) {
                scene->flags &= ~0x100000;
                handle = (u32)kwlnTaskCreate(D_003BB3D0, 0x2B0E, 1, 1,
                                       func_001B55A8, btlFinishTrackedBattleTaskAndCloseWindow,
                                       (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101A80(scene->taskParent, (KwlnTask *)handle);
                btlSetTrackedTaskHandle(0xC, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00358828);
                func_001BCB88(0, 8);
            } else {
                scene->flags |= 0x100000;
            }
        }
    }
    object = fldGetSceneObjectTaskUserData();
    if (object->state == 3) {
        btlCommandPanelWork->state = 2;
        object->state = 1;
    } else if (object->state == 8) {
        object->state = 6;
        btlCommandPanelWork->state = 2;
    } else if (object->state != 11) {
        btlCommandPanelWork->state = 1;
        object->state = 1;
    }
    btlLinkedSelectionTaskBuffer->selectedRow =
        btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
}

void fldSetSceneObjectAndGroupStates(void) {
    BattleSceneObject *object = fldGetSceneObjectTaskUserData();
    if (object != 0) {
        BattleCmdPanel *panel = btlCommandPanelWork;
        object->state = 5;
        panel->state = 3;
    }
}

void fldGetSceneDirectionStepOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2] = {
        {{-7, 3}, {-6, 4}, {-5, 4}, {-4, 5}, {-3, 5}, {-2, 6}, {0, 0}, {0, 0}},
        {{0x22, 3}, {0x21, 4}, {0x20, 4}, {0x1F, 5}, {0x1E, 5}, {0x1D, 6}, {0, 0}, {0, 0}},
        {{0x11, 0x29}, {0x11, 0x28}, {0x11, 0x27}, {0x11, 0x26}, {0x11, 0x25}, {0x11, 0x24}, {0, 0}, {0, 0}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

void func_001BF8B0(SceneAiWork *work) {
    s32 i;
    switch (work->animationPhase) {
    case 0:
        for (i = 0; i < 3; i++) {
            work->rowFade[i] = 128;
            work->rowScale[i] = 80.0f;
            work->rowPhase[i] = 0;
            fldGetSceneDirectionStepOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, 0);
        }
        break;
    case 4:
        break;
    case 5:
        for (i = 0; i < 3; i++) {
            switch (work->rowPhase[i]) {
            case 0:
                work->rowStep[i]++;
                work->rowStep[i] = work->rowStep[i] <= 0 ? 0 : work->rowStep[i] > 5 ? 5 : work->rowStep[i];
                fldGetSceneDirectionStepOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, work->rowStep[i]);
                if (work->rowStep[i] >= 5) work->rowPhase[i]++;
                break;
            case 1:
                work->rowStep[i]--;
                work->rowStep[i] = work->rowStep[i] <= 0 ? 0 : work->rowStep[i] > 5 ? 5 : work->rowStep[i];
                if (work->rowStep[i] <= 0) work->rowPhase[i]++;
                break;
            case 2:
                break;
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BFAD0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2FB8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2FE8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001BFDE0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C0650);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3020);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3030);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C08B8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C0DF8);

void btlDrawCenteredPanelSegments(s32 width) {
    u32 color[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    func_002BF438(x * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    func_002BF438((0x100 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_002BF438((0x92 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x15, 0x53);
}

s32 fldStepSceneStateMachine(KwlnTask *handle) {
    BattleController *work = (BattleController *)btlGetRuntime();
    SceneAiWork *state;
    s32 mode;
    s32 i;
    s32 count;
    BtlIndexList *owner;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (SceneAiWork *)kwlnTaskGetUserValue(handle);
    switch (state->state) {
    case 1:
        state->state = 2;
        break;
    case 2:
        owner = state->listA;
        mode = 1;
        switch (work->mode) {
        case 0x108:
            if (btlGetEffectActive() == 1) {
                mode = 5;
            }
            break;
        case 0x10E:
            count = btlGetIndexListCount(owner);
            for (i = 0; i < count; i++) {
                if (((BtlUnit *)btlGetIndexListEntry(state->listA, i))->status.flags & 0x200) {
                    break;
                }
            }
            if (i >= count) {
                mode = 5;
            }
            break;
        }
        func_001C08B8(work, state, mode);
        func_001C0DF8(state);
        break;
    case 3:
    case 4:
    case 5:
        break;
    case 6:
        return -1;
    }
    return 0;
}

extern void func_001A30F8(s32 source, BtlIndexList *list, s32, s32, s32);

extern u32 func_001A3360(void *source, BtlIndexList *list, s32);

extern s32 func_001C0650(s32, SceneAiWork *);

SceneAiWork *func_001C13E8(BtlTask *source) {
    SceneAiWork *work;
    BattleSceneObject *object;
    BattleActorPanelWork *other;
    s32 entry;
    s32 count;
    u32 i;
    u32 listCount;
    BtlUnit *actor;
    BtlState *battle;

    battle = (BtlState *)btlGetRuntime();
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(0xA4);
    object = (BattleSceneObject *)kwlnTaskGetUserValue(
        kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (BattleActorPanelWork *)kwlnTaskGetUserValue(
            kwlnTaskGetTaskByName(D_003BB3B0));
        count = btlCountFlaggedSceneActors();
        if (count < 2 &&
            (datGameState->party[other->partyRecordIndex].status & 0x4800)) {
            func_001A30F8((s32)source, work->listA, 1, 4, -0x4801);
        } else {
            func_001A30F8((s32)source, work->listA, 1, 4, -1);
        }
        btlGetIndexListCount(work->listA);
        work->result = 0;
    } else if (source != 0) {
        work->result = func_001A3360((void *)source, work->listA, 0);
    }
    work->entry = 0;
    switch (work->result) {
    case 0:
        entry = btlFindEligibleTargetForMultiActorCommand((s32)source, work->listA);
        work->entry = entry;
        btlAppendIndexListEntry(work->listB,
                                btlGetIndexListEntry(work->listA, entry));
        break;
    case 1:
    case 2:
        btlCopyIndexList(work->listB, work->listA);
        break;
    }
    work->source = source;
    work->state = 1;
    if (battle->battleMode != 0x10E || work->result != 0) {
        return work;
    }
    listCount = btlGetIndexListCount(work->listA);
    i = 0;
nextActor:
    if (i >= listCount) {
        goto selectTarget;
    }
    actor = (BtlUnit *)btlGetIndexListEntry(work->listA, i++);
    if (!(actor->status.flags & 0x200)) {
        goto nextActor;
    }
    return work;
selectTarget:
    work->cursor.position.row = 2;
    work->cursor.position.column = 1;
    func_001C0650(0, work);
    btlClearIndexList(work->listB);
    btlAppendIndexListEntry(work->listB,
                            btlGetIndexListEntry(work->listA, work->entry));
    return work;
}

void fldReleaseSceneSpriteWork(SceneAiWork *work) {
    btlFreeIndexList(work->listB);
    btlFreeIndexList(work->listA);
    sdfReleaseChipBlock(work);
}

void fldReleaseSceneSprite(KwlnTask *arg0) {
    SceneAiWork *temp_v0;
    s32 temp_v1;

    temp_v0 = (SceneAiWork *)kwlnTaskGetUserValue(arg0);
    fldReleaseSceneSpriteWork(temp_v0);
    temp_v1 = btlGetRuntime();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

SceneAiWork *fldGetSceneScriptTaskUserData(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_003BB3A0);
    if (task == 0) {
        return 0;
    }
    return (SceneAiWork *)kwlnTaskGetUserValue(task);
}

s32 fldGetSceneScriptState(void) {
    return fldGetSceneScriptTaskUserData()->state;
}

BtlIndexList *fldGetSceneScriptValue(void) {
    return fldGetSceneScriptTaskUserData()->listB;
}

extern s32 fldStepSceneStateMachine(KwlnTask *);

extern SceneAiWork *func_001C13E8(BtlTask *);

void fldCreateSceneSpriteTask(s32 arg0) {
    BattleController *scene;
    s32 task;
    kwlnTaskGetTaskByName(D_003BB3A0);
    if (btlIsNamedBattleTaskRegistered() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0xA), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    scene = (BattleController *)btlGetRuntime();
    task = (s32)kwlnTaskCreate(D_003BB3A0, 0x2B0E, 1, 1, fldStepSceneStateMachine, fldReleaseSceneSprite,
                          (u32)func_001C13E8((BtlTask *)arg0));
    func_00101A80(scene->taskParent, (KwlnTask *)task);
    scene->spriteObject = task;
}

void fldMarkActiveSceneScriptState(void) {
    SceneAiWork *work;

    work = fldGetSceneScriptTaskUserData();
    if (work != 0) {
        work->state = 6;
    }
}

void func_001C1850(s32 index, s8 operation) {
    s32 bank = D_003BD83C->bank;
    s32 count;
    s32 i;
    if (D_003BD83C->enabled[bank] == 0) return;
    switch (operation) {
    case 0:
        if (D_003BD840[bank]->state[index] >= 3) return;
        D_003BD840[bank]->state[index] = 3;
        count = D_003BD83C->currentIndex;
        for (i = 0; i < count; i++) {
            if ((u8)(D_003BD840[bank]->state[i] - 4) < 3) {
                D_003BD840[bank]->state[i] = 6;
                D_003BD840[bank]->scalePercent[i][0] = 132.0f;
                D_003BD840[bank]->slotValues[i][0] = 16;
                D_003BD840[bank]->scalePercent[i][1] = 105.0f;
                D_003BD840[bank]->slotValues[i][1] = 24;
            }
        }
        break;
    case 1:
        D_003BD840[bank]->state[index] = 7;
        D_003BD83C->phase[index + 1][bank] = 4;
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A30D8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C1988);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A31A0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C2158);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C27B0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3220);

void func_001C2938(s32 bank, s32 index) {
    if (D_003BD840[bank]->secondaryState[index] == 8) {
        D_003BD840[bank]->colorAdjustments[index][0] -= 8;
        D_003BD840[bank]->colorAdjustments[index][0] = D_003BD840[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][0] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][0];
        D_003BD840[bank]->colorAdjustments[index][1] -= 8;
        D_003BD840[bank]->colorAdjustments[index][1] = D_003BD840[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][1] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][1];
        D_003BD840[bank]->colorAdjustments[index][2] -= 8;
        D_003BD840[bank]->colorAdjustments[index][2] = D_003BD840[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][2] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][2];
        D_003BD840[bank]->colorAdjustments[index][3] -= 8;
        D_003BD840[bank]->colorAdjustments[index][3] = D_003BD840[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][3] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][3];
        if (D_003BD840[bank]->colorAdjustments[index][0] <= 0) D_003BD840[bank]->secondaryState[index] = 0;
    }
    if (D_003BD840[bank]->secondaryState[index] == 7) {
        D_003BD840[bank]->colorAdjustments[index][0] -= 24;
        D_003BD840[bank]->colorAdjustments[index][0] = D_003BD840[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][0] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][0];
        if (D_003BD840[bank]->colorAdjustments[index][0] <= 0) D_003BD840[bank]->secondaryState[index]++;
    }
    if ((u8)(D_003BD840[bank]->secondaryState[index] - 6) < 2) {
        D_003BD840[bank]->colorAdjustments[index][1] -= 27;
        D_003BD840[bank]->colorAdjustments[index][1] = D_003BD840[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][1] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][1];
    }
    if ((u8)(D_003BD840[bank]->secondaryState[index] - 5) < 3) {
        D_003BD840[bank]->colorAdjustments[index][3] -= 29;
        D_003BD840[bank]->colorAdjustments[index][3] = D_003BD840[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][3] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][3];
    }
    if ((u8)(D_003BD840[bank]->secondaryState[index] - 4) < 4) {
        D_003BD840[bank]->colorAdjustments[index][2] -= 32;
        D_003BD840[bank]->colorAdjustments[index][2] = D_003BD840[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][2] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][2];
    }
    switch (D_003BD840[bank]->secondaryState[index]) {
    case 3:
        D_003BD840[bank]->colorAdjustments[index][2] += 64;
        D_003BD840[bank]->colorAdjustments[index][2] = D_003BD840[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][2] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][2];
        if (D_003BD840[bank]->colorAdjustments[index][2] >= 127) D_003BD840[bank]->secondaryState[index]++;
        break;
    case 4:
        D_003BD840[bank]->colorAdjustments[index][3] += 64;
        D_003BD840[bank]->colorAdjustments[index][3] = D_003BD840[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][3] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][3];
        if (D_003BD840[bank]->colorAdjustments[index][3] >= 127) D_003BD840[bank]->secondaryState[index]++;
        break;
    case 5:
        D_003BD840[bank]->colorAdjustments[index][1] += 64;
        D_003BD840[bank]->colorAdjustments[index][1] = D_003BD840[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][1] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][1];
        if (D_003BD840[bank]->colorAdjustments[index][1] >= 127) D_003BD840[bank]->secondaryState[index]++;
        break;
    case 6:
        D_003BD840[bank]->colorAdjustments[index][0] += 64;
        D_003BD840[bank]->colorAdjustments[index][0] = D_003BD840[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_003BD840[bank]->colorAdjustments[index][0] > 127 ? 127 : D_003BD840[bank]->colorAdjustments[index][0];
        if (D_003BD840[bank]->colorAdjustments[index][0] >= 127) D_003BD840[bank]->secondaryState[index]++;
        break;
    case 2:
        D_003BD840[bank]->secondaryState[index]++;
        break;
    case 1:
        D_003BD840[bank]->colorAdjustments[index][0] = 127;
        D_003BD840[bank]->colorAdjustments[index][1] = 127;
        D_003BD840[bank]->colorAdjustments[index][2] = 127;
        D_003BD840[bank]->colorAdjustments[index][3] = 127;
        D_003BD840[bank]->secondaryState[index]++;
        break;
    }
}

void fldScaleSceneCoordinateRecord(EffectSlotSet *work, s32 index) {
    work->workEntries[index].geometry.bounds[2] = work->workEntries[index].sourceWidth << 4;
    work->workEntries[index].geometry.bounds[3] = work->workEntries[index].sourceHeight << 3;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C2E90);

void fldSetSceneSlotRange(s32 index) {
    SceneSlotFadeWork *scene = D_003BD83C;
    if (scene->currentIndex < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            scene = D_003BD83C;
            scene->fade[i][scene->bank] = 0x80;
            scene->phase[i][scene->bank] = 3;
        }
        D_003BD83C->lastIndex = index;
    }
    D_003BD83C->currentIndex = index;
}

extern s32 btlGetNamedTaskPairStatusOrUnavailable(void);

void func_001C3040(void) {
    s32 bank = D_003BD83C->bank;
    s32 last = D_003BD83C->lastIndex;
    s32 status = btlGetNamedTaskPairStatusOrUnavailable();
    s32 i;
    if (D_003BD83C->enabled[bank] == 0) return;
    if (status != 0 && status != 3) return;
    for (i = 0; i <= last; i++) {
        switch (D_003BD83C->phase[i][bank]) {
        case 1:
            D_003BD83C->timer++;
            D_003BD83C->timer = D_003BD83C->timer <= 0 ? 0 : D_003BD83C->timer > 34 ? 34 : D_003BD83C->timer;
            if (D_003BD83C->timer >= 34) {
                D_003BD83C->fade[i][bank] += 64;
                D_003BD83C->fade[i][bank] = D_003BD83C->fade[i][bank] <= 0 ? 0 : D_003BD83C->fade[i][bank] > 255 ? 255 : D_003BD83C->fade[i][bank];
                if (D_003BD83C->fade[i][bank] >= 255) {
                    D_003BD83C->phase[i][bank]++;
                    D_003BD83C->phase[i + 1][bank] = 1;
                }
            }
            break;
        case 2:
            D_003BD83C->fade[i][bank] -= 16;
            D_003BD83C->fade[i][bank] = D_003BD83C->fade[i][bank] <= 128 ? 128 : D_003BD83C->fade[i][bank] > 255 ? 255 : D_003BD83C->fade[i][bank];
            if (D_003BD83C->fade[i][bank] <= 128) {
                D_003BD83C->phase[i][bank]++;
                if (i == last) D_003BD83C->completed = 1;
            }
            break;
        case 3:
            break;
        case 4:
            D_003BD83C->fade[i][bank] -= 64;
            D_003BD83C->fade[i][bank] = D_003BD83C->fade[i][bank] <= 0 ? 0 : D_003BD83C->fade[i][bank] > 128 ? 128 : D_003BD83C->fade[i][bank];
            break;
        }
    }
}

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3A0);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlCommandPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3A8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3AC);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3B0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3B4);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3B8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3BC);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3C0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3C4);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlMahenPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlAnalyzPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3D0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3D4);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlTrackedTaskHandles);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3DC);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlCommandPanelWork);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3E4);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlResourceBlockLoaded);

INCLUDE_SDATA(const s32, "game/code_001A9780", btlResourceBlock);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3F0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB3F8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB400);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB408);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB410);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB418);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB420);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB428);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB430);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB438);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB440);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB448);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB450);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB458);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB460);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB468);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB470);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB478);

void func_001C32B0(void) {
    u32 overlays[2] = {0x0000FF00, 0xFF000000};
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 bank;
    s32 last;
    s32 channel;
    s32 fade;

    bank = D_003BD83C->bank;
    if (D_003BD83C->enabled[bank] <= 0) {
        return;
    }
    last = D_003BD83C->currentIndex - 1;
    if (last >= 0) {
        /* Solid background strips use the live fade again after color lookup. */
        if (last == 1 || last == 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 16, channel,
                D_003BD83C->fade[2][bank]);
                if (D_003BD83C->fade[2][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 16, 0x53);
        }
        if (last >= 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_003BD83C->fade[3][bank]);
                if (D_003BD83C->fade[3][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(392 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 18, channel,
                D_003BD83C->fade[4][bank]);
                if (D_003BD83C->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 18, 0x53);
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 19, channel,
                D_003BD83C->fade[4][bank]);
                if (D_003BD83C->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(349 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 19, 0x53);
        }
        if (last >= 4) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 20, channel,
                D_003BD83C->fade[5][bank]);
                if (D_003BD83C->fade[5][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(298 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 20, 0x53);
        }
        if (last >= 7) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_003BD83C->fade[8][bank]);
                if (D_003BD83C->fade[8][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(206 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }

    }

    if (last >= 0) {
        /* Foreground connectors retain the selected corner fade across lookup. */
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_003BD83C->fade[1][bank];
            } else {
                fade = D_003BD83C->fade[0][bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 10, channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[bank];
            }
        }
        func_002BF438(440 << 4, 6 << 3, 0, colors, 0,
        btlResourceBlock->resC, 10, 0x53);
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_003BD83C->fade[4][bank];
                } else {
                    fade = D_003BD83C->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 11, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(395 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 11, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_003BD83C->fade[4][bank];
                } else {
                    fade = D_003BD83C->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 12, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(362 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 12, 0x53);
        }
        if (last == 5) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 13, channel,
                D_003BD83C->fade[6][bank]);
                if (D_003BD83C->fade[6][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(302 << 4, 46 << 3, 0, colors, 0,
            btlResourceBlock->resC, 13, 0x53);
        }
        if (last >= 6) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_003BD83C->fade[7][bank];
                } else {
                    fade = D_003BD83C->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 14, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(302 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 14, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_003BD83C->fade[7][bank];
                } else {
                    fade = D_003BD83C->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 15, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_002BF438(269 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 15, 0x53);
        }
    }
}

extern const s32 D_003A32F0[10][3];

void func_001C3B28(void) {
    u32 overlays[4] = {0x0000FF00, 0xFF000000, 0x8080FF00, 0xFF808000};
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 rows[10][3] = {
        {435, 28, 6}, {404, 28, 7}, {373, 28, 8}, {342, 28, 7},
        {311, 28, 8}, {280, 28, 7}, {249, 28, 8}, {218, 28, 7},
        {187, 28, 8}, {156, 28, 7}
    };
    s32 finalRows[10][3];
    s32 row;
    s32 last;
    s32 channel;
    s32 fade;

    memcpy(finalRows, D_003A32F0, sizeof(finalRows));

    if (D_003BD83C->enabled[D_003BD83C->bank] <= 0) {
        return;
    }
    last = D_003BD83C->currentIndex - 1;
    for (row = 0; row < D_003BD83C->currentIndex; row++) {
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_003BD83C->fade[row + 1][D_003BD83C->bank];
            } else {
                fade = D_003BD83C->fade[row][D_003BD83C->bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       rows[row][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_003BD83C->bank];
            }
        }
        func_002BF438(rows[row][0] << 4, rows[row][1] << 3, 0, colors,
                      0, btlResourceBlock->resC, rows[row][2], 0x53);
    }
    if (last >= 0) {
        for (channel = 0; channel < 4; channel++) {
            fade = D_003BD83C->fade[last + 1][D_003BD83C->bank];
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       finalRows[last][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_003BD83C->bank];
            }
        }
        func_002BF438(finalRows[last][0] << 4, finalRows[last][1] << 3,
                      0, colors, 0, btlResourceBlock->resC, finalRows[last][2], 0x53);
    }
}

void fldInitSceneFadeRecords(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    SceneSlot *slot = scene->slots;
    SceneFadingRecord *record = scene->fadingRecords;
    u32 i = 0;

    while (i < 8 && slot->a != 0) {
        memcpy(&record->slot, slot, sizeof(SceneSlot));
        record->alpha = 0x80;
        record->target = -1;
        i++;
        slot++;
        record++;
    }
    while (i < 8) {
        memset(record, 0, sizeof(*record));
        record->target = -1;
        i++;
        record++;
    }
}

s32 btlFindSceneSlotById(SceneSlot *entry) {
    BattleController *context = (BattleController *)btlGetRuntime();
    SceneSlot *slot = context->slots;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->id == entry->id) {
            return i;
        }
    }
    return -1;
}

SceneSlot *fldFindSceneSlotRecord(SceneSlot *entry) {
    BattleController *context = (BattleController *)btlGetRuntime();
    s32 index = btlFindSceneSlotById(entry);
    SceneSlot *record = 0;

    if (index != -1) {
        record = &context->slots[index];
    }
    return record;
}

s32 btlFadeStaleSceneSlots(void) {
    BattleController *context = (BattleController *)btlGetRuntime();
    SceneFadingRecord *scene = context->fadingRecords;
    SceneSlot *slot = context->slots;
    u32 i;
    s32 changed = 0;
    for (i = 0; i < 8; i++, slot++, scene++) {
        if (scene->slot.id != slot->id && fldFindSceneSlotRecord(&scene->slot) == 0) {
            if (scene->alpha != 0) {
                scene->alpha -= 8;
                changed = 1;
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BattleController *scene, s32 *outFadeCount) {
    SceneFadingRecord *record = scene->fadingRecords;
    SceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    BattleTrackedTaskWork *global;

    while (fadeCount < 8 && record[fadeCount].slot.a != 0) {
        if (record[fadeCount].slot.a == 1) {
            countA++;
        }
        if (record[fadeCount].slot.a == 2) {
            countB++;
        }
        fadeCount++;
    }
    global = btlTrackedTaskHandles;
    if (global->fadeKindsCached == 0) {
        if (countA != 0 || countB != 0) {
            global->fadeKindACount = countA;
            global->fadeKindBCount = countB;
            global->fadeKindsCached = 1;
        }
    }
    fadeCount--;
    slot = scene->slots;
    slotCount = 0;
    while (slotCount < 8 && slot->a != 0) {
        slotCount++;
        slot++;
    }
    *outFadeCount = fadeCount;
    return slotCount;
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C4278);

s32 fldSceneCleanupTask(KwlnTask *task) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (btlTrackedTaskHandles->presentationState == 1 && btlTrackedTaskHandles->status.bytes.blocked == 0) {
        return 0;
    }
    func_001C4278();
    return 0;
}

void fldResetSceneStatus(KwlnTask *task) {
    BattleController *scene;

    scene = (BattleController *)btlGetRuntime();
    scene->cleanupTask = 0;
}

void fldBeginSceneTransition(void) {
    BattleController *scene = (BattleController *)btlGetRuntime();
    func_001BCB88(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    btlGetRuntime();
    func_001BCB88(0, 8);
}

void func_001C44F8(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 arg0) {
    s32 temp_v0;

    temp_v0 = btlGetRuntime();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

extern const char *D_003BB488;

extern s32 fldSceneCleanupTask(KwlnTask *);

void fldCreateSceneCleanupTask(void) {
    KwlnTask *oldTask = kwlnTaskGetTaskByName(D_003BB488);
    BattleController *context;
    u32 task;
    if (oldTask == 0) {
        btlGetRuntime();
    }
    context = (BattleController *)btlGetRuntime();
    task = (u32)kwlnTaskCreate(D_003BB488, 0x2B0E, 1, 1, fldSceneCleanupTask, fldResetSceneStatus, 0);
    func_00101A80(context->taskParent, (KwlnTask *)task);
    context->cleanupTask = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    func_001B6308();
    func_001B6498(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001C45F0);

static inline void btlDestroyTrackedTaskIfPresent(s32 slot) {
    if (btlGetTrackedTaskHandle(slot) != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(slot), 0);
    }
}

void fldDestroySceneTasksAndBuffers(void) {
    BattleController *battle = (BattleController *)btlGetRuntime();

    if (battle->sceneObjectTask != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)battle->sceneObjectTask, 0);
        battle->sceneObjectTask = 0;
    }
    if (battle->spriteObject != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)battle->spriteObject, 0);
        battle->spriteObject = 0;
    }
    if (battle->cleanupTask != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)battle->cleanupTask, 0);
        battle->cleanupTask = 0;
    }
    if (btlHasRegisteredPsechgPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(6), 0);
    }
    btlDestroyTrackedTaskIfPresent(5);
    btlDestroyTrackedTaskIfPresent(1);
    btlDestroyTrackedTaskIfPresent(0);
    btlDestroyTrackedTaskIfPresent(8);
    btlDestroyTrackedTaskIfPresent(3);
    btlDestroyTrackedTaskIfPresent(9);
    btlDestroyTrackedTaskIfPresent(10);
    if (btlGetTrackedTaskHandle(2) != 0) {
        btlReleaseSelectionTaskBuffer();
    }
    if (btlGetTrackedTaskHandle(12) != 0) {
        btlDestroyTaskC();
    }
    if (btlGetTrackedTaskHandle(13) != 0) {
        btlDestroyTaskD();
    }
    if (btlCommandPanelWork != NULL) {
        btlReleaseAndClearChipBlock();
    }
    if (btlGetTrackedTaskHandle(4) != 0) {
        btlReleaseRegisteredTaskBuffer(0);
    }
    btlDestroyTrackedTaskIfPresent(7);
    btlReleaseResourceBlock();
    sdfReleaseChipBlock(D_003BD840[1]);
    sdfReleaseChipBlock(D_003BD840[0]);
    sdfReleaseChipBlock(D_003BD83C);
    sdfReleaseChipBlock(btlTrackedTaskHandles);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A32F0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB488);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB494);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB496);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB498);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4A0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4A8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4B0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4B8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4C0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4C8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4D0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4D8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4E0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4E8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4F0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB4F8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB500);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB508);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB510);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB518);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB520);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB528);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB530);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB538);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB540);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB548);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB550);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB558);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB560);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB568);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB570);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB578);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB580);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB588);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB590);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB598);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5A0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5A8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5B0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5B8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5C0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5C8);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5D0);

INCLUDE_SDATA(const s32, "game/code_001A9780", D_003BB5D8);


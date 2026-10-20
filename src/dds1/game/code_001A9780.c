#include "btl_selection_view.h"
#include "btl_analysis_panel_work.h"
#include "btl_selection_trail_work.h"
#include "common.h"
#include "dsp_message_window_api.h"
#include "fr_font_context.h"
/* Declared before first use (were implicit int). */
extern void btlClearNodeFlags(void);
extern s32 btlGetNamedTaskPairStatusOrUnavailable(void);
extern u32 btlNextScaledRandom(u32 limit);
extern u16 btlRefreshUnitMaximumHpAndClampCurrentHp(struct DatPartyRecord *entryAddress);
extern u16 btlRefreshUnitMaximumMpAndClampCurrentMp(struct DatPartyRecord *entryAddress);
extern void evtCopyEntryStringToActiveWindow(s32 slotIndex, const void *sourceText);
extern s32 fldGetSceneObjectState(void);
extern void itfMesCleanupWindow(s32 window, s32 releasePrimaryBlock);
extern s32 itfMesStartEntry(s32 window, s32 entryIndex, s32 itemIndex);
extern void * memset(void *, s32, u32);
extern u32 strlen(const char *s);
/* Original-style implicit int, kept: matched code depends on it: btlCountEligibleLinkedActors. */

extern void itfSetTextDrawLimit(s32 limit);
#include "btl_actor_status_table_api.h"
#include "dsp_channel.h"
#include "mdl_flag_api.h"
#include "btl_stat_effect_offsets.h"
#include "btl_task_state6_api.h"
#include "btl_effect_active_api.h"
#include "kwln_task_add_child.h"
#include "btl_special_ability.h"
#include "btl_effect_position.h"
#include "evt_message_window_api.h"
#include "dsp_entry_api.h"
#include "eff_random.h"
#include "sdf_trig.h"
#include "snd_pad.h"
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
#include "btl_phase_panel_work.h"
#include "btl_roster_pair.h"
#include "sdf.h"
#include "btl.h"
#include "btl_state.h"
#include "btl_scene_initializer.h"
#include "btl_boss_debug_api.h"
#include "eff.h"
#include "btl_action.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "dat_state.h"
#include "dat_command.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"

extern void btlDrawActorPanelHighlights(BtlUnit *, BattleActorPanelWork *, s32, s8);
extern void func_001BBE18(BtlUnit *, BattleActorPanelWork *, s32, s8, s16, s32, s32);



extern void func_001C2E90(EffectSlotSet *, s32, s32 *, s32 *, f32);

extern SceneSlotFadeWork *D_003BD83C;

extern ActorSlotOrder *D_003BD840[2];



extern void evtCreateMessageWindowIfMissing(void *text);






extern void btlSetCommandPanelFadeMode(s8, s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

extern SndPad D_00324510;

extern s8 D_00324530[];

extern u32 btlPendingLinkageSkillFlags[];


extern DatEnemyRecord *datEnemyRecords;

extern SceneAiWork *btlGetTargetSelectionWork(void);

extern u32 itfCreateFont2TextGlyph(s32, s32, s32, u32, char *, s32);

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

/* The command hint window is a 0x40-byte allocation in btlCreateMessageWindow. */
typedef struct BtlCommandHintWork {
    u8 pad00[0x10];
    s32 unk10;
    s16 fade;
    u8 pad16[2];
    u32 colors[4];
    u8 pad28[0x10];
    s32 x;
    s32 y;
} BtlCommandHintWork;
typedef char BtlCommandHintWorkSize[sizeof(BtlCommandHintWork) == 0x40 ? 1 : -1];

extern BtlCommandHintWork *D_003BB3DC;

extern SceneKindTable *D_003BD834;

extern u32 btlCommandOptionAnimationWork;

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

extern const char *D_003BB3A8;

extern const char *btlCommandPanelTaskNameRef;

extern const char *D_003BB3B0;

extern const char *D_003BB3AC;

extern const char *D_003BB3BC;

extern const char *D_003BB3C4;

extern const char *btlAnalyzPanelTaskNameRef;

extern const char *btlMahenPanelTaskNameRef;
extern s32 btlGetTrackedTaskHandle(s32);

extern BattleCmdPanel *btlCommandPanelWork;


extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

extern void btlSetActionActorPanelPresentation(BtlTask *, s8, s8);
extern char D_003BB450[];

extern u8 *datRosterDetails;

extern s32 sdfGetRandInt(s32, s32);


extern EffRandState effSharedRandomState[];

extern void btlAllocatePanelTaskWork(void);

extern void itfMesDestroyWindowIfPresent(s32);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 actionTask, BtlIndexList *targets);

extern EffRandState effSharedRandomState[];

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);


extern s32 evtRunContext(s32, s32, s32, s32, u16);

s32 btlApplyCommandAbilityMultiplier(DatPartyRecord *battler, s32 command);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s8 btlGetActorIndexedSignedValue(BtlUnit *unit, s32 index);

s32 btlGetCombinedPartyCommandPower(DatPartyRecord *base, BtlUnit *first, BtlUnit *second, BtlUnit *third, s32 command);

s32 btlGetCommandFailureReason(BtlUnit *unit, s32 command);

/* The label resource index is the copied party record's unit ID. */ void *btlGetIndexedUiResource(BtlUnit *unit);


s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *actor, s32 delta);

s8 btlGetLinkageSlotCost(s32 command);

s32 btlGetLinkedPartyCommandFailureReason(BtlUnit *base, BtlUnit *first, BtlUnit *second, BtlUnit *third, s32 command);

/* Select a follow-up encounter from the override, forced scene or adjustment rows. */
s32 btlSelectSerialEncounter(void) {
    extern void btlSerialBattleChanceHook(s32);
    extern s32 btlRollAiBucket(void);

    BtlState *state = (BtlState *)btlGetRuntime();
    u16 selectionIndex = (u16)state->adjustmentEntryIndex;
    s32 (*serialOverride)(void) = state->serialOverride;
    u16 encounterPack;
    u16 groupIndex;
    u16 forcedScene;
    BattleAdjustmentEntry *entries;
    s32 chance;
    u16 candidates[20];
    u16 candidateCount;
    u16 rowIndex;

    state->requestMode = 0;
    state->requestArgument = 0;
    state->nextAdjustmentEntryIndex = selectionIndex;
    encounterPack = (u16)state->encounterPack;
    groupIndex = (u16)state->adjustmentGroupIndex;

    if (serialOverride != NULL) {
        s32 result = serialOverride();

        if (result != -1) {
            state->requestArgument = result;
            state->requestMode = 3;
            btlBossDebugPrintf("btl:serial boss [%X]\n", result);
            return 1;
        }
        return 0;
    }

    if (datBattleSceneRecords[state->battleMode].serialScene != 0) {
        forcedScene = datBattleSceneRecords[state->battleMode].serialScene;
        state->requestMode = 2;
        state->requestArgument = forcedScene;
        btlBossDebugPrintf("btl:serial 100%% [%X]\n", forcedScene);
        return 1;
    }
    if (encounterPack == 0) {
        return 0;
    }

    entries = D_003BAA3C[encounterPack].groups[groupIndex].entries;
    chance = entries[selectionIndex].serialEncounterChanceAdjustment;
    if ((datGameState->world.fieldFlags & 2) != 0) {
        chance *= 5;
    }
    if (chance > 90) {
        chance = 90;
    }
    btlBossDebugPrintf("btl:serial ratio=%d\n", chance);
    btlSerialBattleChanceHook(chance);
    if ((s32)btlRollAiBucket() >= chance) {
        return 0;
    }
    btlBossDebugPrintf("btl:serial\n");

    {
        s32 battleMode;

        candidateCount = 0;
        battleMode = state->battleMode;
        for (rowIndex = 0; rowIndex < 20; rowIndex++) {
            if (state->battleMode != entries[rowIndex].sceneIndex &&
                entries[rowIndex].sceneIndex != 0 && entries[rowIndex].weight != 0) {
                candidates[candidateCount] = rowIndex;
                candidateCount++;
            }
        }

        state->requestMode = 1;
        if (candidateCount == 0) {
            state->requestArgument = battleMode;
            return 1;
        }

        {
            u16 randomIndex = (u16)sdfGetRandInt(0, candidateCount);
            u16 selectedIndex = candidates[randomIndex];

            state->nextAdjustmentEntryIndex = selectedIndex;
            state->requestArgument = entries[selectedIndex].sceneIndex;
        }
    }
    return 1;
}

extern const f32 D_003A1D28[9];
extern const char D_003A1D50[];
extern void btlSurpriseChanceHook(s32 chance);

/* Roll the level/ability-scaled surprise chance, including scene overrides. */
s32 btlRollSurpriseAttack(void) {
    extern s32 btlRollAiBucket(void);
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
    btlSurpriseChanceHook(chance);
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

extern s32 btlRollAiBucket(void);

s32 btlRollStoneDeathChance(BtlUnit *actor, s32 index) {
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
    return (s32)btlRollAiBucket() < chance;
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
    return (s32)btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *object) {
    if ((object->status.flags & 0x400) == 0) {
        return 0;
    }
    return ((s32)datEnemyRecords[object->partyRecord.unitId].flags & 0x100) > 0;
}

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
    if ((s32)btlRollAiBucket() >= chance) {
        return 0;
    }
    flags = record->huntPenaltyFlags;
    if (flags == 0x18) {
        return (sdfGetRand((EffRandState *)effSharedRandomState) & 1) ? 0x10 : 8;
    }
    return flags & 0x18;
}

extern s32 btlPassiveAbilityActions[];

s32 btlRollPassiveAbilityAction(BtlUnit *actor) {
    u32 i;
    s32 result;
    s32 threshold;

    if (btlIsUnitDefeatTriggeredByValueDelta(actor, 0)) {
        return -1;
    }
    result = 0;
    for (i = 0; i < 3; i++) {
        if (btlCheckSpecialAbility(&actor->partyRecord, btlPassiveAbilityActions[i * 2])) {
            threshold = (s32)(datAbilityParameters[btlPassiveAbilityActions[i * 2] - BTL_ABILITY_PARAMETER_FIRST_SKILL].value * 100.0f);
            if ((s32)btlRollAiBucket() < threshold) {
                result = (btlPassiveAbilityActions + i * 2)[1];
                break;
            }
        }
    }
    return result != 0 ? result : -1;
}

/* Roster detail table entries are 0x14 bytes; the leading halfword is the ranking value. */
typedef struct {s16 base; u8 data[0x12];} BattleRosterDetail;
void btlBuildCombinedActorUnit(BtlUnit *dst, BtlUnit *first, BtlUnit *second, BtlUnit *third)
{
  BtlUnit *sources[3];
  u32 i;
  u32 n;
  u32 count;
  BtlUnit *src;
  s32 currentScore;
  s32 incomingScore;
  sources[0] = first;
  sources[1] = second;
  sources[2] = third;
  memset(dst, 0, sizeof(BtlUnit));
  dst->status.flags = (first->status.flags & 0x600) | 1;
  if ((&dst->partyRecord) != (&first->partyRecord))
  {
    memcpy(&dst->partyRecord, &first->partyRecord, sizeof(DatPartyRecord));
  }
  dst->partyRecord.level = 0;
  dst->partyRecord.menuValue = 0xA1;
  dst->partyRecord.hp = 0;
  dst->partyRecord.mp = 0;
  dst->partyRecord.maxHp = 0;
  dst->partyRecord.maxMp = 0;
  for (i = 0; i < 5; i++)
  {
    dst->partyRecord.baseStats[i] = 0;
  }

  dst->partyRecord.status = 0;
  btlClearAllActorEntrySlots(dst);
  for (i = 0; i < 7; i++)
  {
    if (dst->entrySlots[i].code < first->entrySlots[i].code)
    {
      dst->entrySlots[i].code = first->entrySlots[i].code;
    }
  }

  btlClearActorEntrySlot(dst, 5);
  btlClearActorEntrySlot(dst, 6);
  count = 0;
  for (n = 0; n < 3; n++)
  {
    src = sources[n];
    if (src == ((void *) 0))
    {
      continue;
    }
    if (dst->partyRecord.level < src->partyRecord.level)
    {
      dst->partyRecord.level = src->partyRecord.level;
    }
    if (dst->partyRecord.hp < src->partyRecord.hp)
    {
      dst->partyRecord.hp = src->partyRecord.hp;
    }
    if (dst->partyRecord.mp < src->partyRecord.mp)
    {
      dst->partyRecord.mp = src->partyRecord.mp;
    }
    if (dst->partyRecord.maxHp < src->partyRecord.maxHp)
    {
      dst->partyRecord.maxHp = src->partyRecord.maxHp;
    }
    if (dst->partyRecord.maxMp < src->partyRecord.maxMp)
    {
      dst->partyRecord.maxMp = src->partyRecord.maxMp;
    }
    for (i = 0; i < 5; i++)
    {
      s8 stat = datGetStatWithStatusOverride(&src->partyRecord, i);
      if (dst->partyRecord.baseStats[i] < stat)
      {
        dst->partyRecord.baseStats[i] = stat;
      }
    }

    for (i = 0; i < 24; i++)
    {
      if (src->partyRecord.effectData[i] != 0)
      {
        if (src->partyRecord.effectData[i] < 0x260)
        {
          u16 effect = src->partyRecord.effectData[i];
          if (datCommandSelectors[effect].kind != 4)
          {
            dst->partyRecord.effectData[count] = effect;
            count++;
          }
        }
      }
    }

    dst->partyRecord.status |= src->partyRecord.status & 0x10;
    if (((btlUnitStatusPair(src) & 0x1200) == 0x200) && (src->partyRecord.menuValue != 0))
    {
      currentScore = ((BattleRosterDetail *) datRosterDetails)[dst->partyRecord.unitId].base;
      incomingScore = ((BattleRosterDetail *) datRosterDetails)[src->partyRecord.unitId].base;
      if (currentScore < incomingScore)
      {
        dst->partyRecord.unitId = src->partyRecord.unitId;
      }
      if (dst->status.flags & 0x1000)
      {
        dst->status.flags &= ~0x1000;
        dst->partyRecord.menuValue = src->partyRecord.menuValue;
      }
      else
      {
        currentScore = datItemSkillRecords[dst->partyRecord.menuValue].commandIndex;
        incomingScore = datItemSkillRecords[src->partyRecord.menuValue].commandIndex;
        if (datCommandRecords[currentScore].hpPower < datCommandRecords[incomingScore].hpPower)
        {
          dst->partyRecord.menuValue = src->partyRecord.menuValue;
        }
      }
    }
  }

  if (dst->partyRecord.maxHp < dst->partyRecord.hp)
  {
    dst->partyRecord.hp = dst->partyRecord.maxHp;
  }
  if (dst->partyRecord.maxMp < dst->partyRecord.mp)
  {
    dst->partyRecord.mp = dst->partyRecord.maxMp;
  }
  for (n = count; n < 24; n++)
  {
    dst->partyRecord.effectData[n] = 0;
  }

}

#include "btl_start_voice_row.h"

extern BattleStartVoiceRow btlStartVoiceRows[];
extern s32 btlRollAiBucket(void);
extern s32 btlReadCurrentUnitHp(DatPartyRecord *);
extern u32 func_001A5C40(BtlUnit *, BtlUnit *, s32, s32, s32, s32, s32);

extern char D_003A1DD8[]; /* "btl:start voice off[ESC_OFF]\n" */
extern char D_003A1DF8[]; /* "btl:start voice off[SERIAL]\n" */
extern char D_003A1E18[]; /* "btl:start voice off[RAND]\n" */
extern char D_003A1E38[]; /* "btl:start voice off[UNIT NON]\n" */
extern char D_003A1E58[]; /* "btl:start voice off[STATE=%X,ID=%X]\n" */
extern char D_003A1E80[]; /* "btl:start voice on[VOICE=%X,STATE=%X,ID=%X]\n" */

/* Choose a battle-start voice ID, or -1 when no permitted choice is available. */
s8 btlChooseStartVoice(void) {
    BtlState *state;
    BtlUnit *candidates[4];
    BtlUnit *unit;
    BtlUnit *chosen;
    s8 *voices;
    u32 hp;
    u32 damage;
    s32 count;
    s32 category;
    u32 i;
    s8 voice;

    state = (BtlState *)btlGetRuntime();
    if (datBattleSceneRecords[state->battleMode].voice != 0) {
        btlBossDebugPrintf(D_003A1DD8);
        return -1;
    }
    if ((state->battleFlags & 0x4000) || state->eventReady == 5) {
        btlBossDebugPrintf(D_003A1DF8);
        return -1;
    }
    if ((u32)btlRollAiBucket() >= 10) {
        btlBossDebugPrintf(D_003A1E18);
        return -1;
    }
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                s32 flagId = btlStartVoiceRows[unit->partyRecord.unitId].flagId;
                if (flagId < 0 || mdlFlagTest(flagId)) {
                    candidates[count++] = unit;
                }
            }
        }
    }
    if (count == 0) {
        btlBossDebugPrintf(D_003A1E38);
        return -1;
    }
    chosen = candidates[sdfGetRandInt(0, count)];
    category = 2;
    hp = btlReadCurrentUnitHp(&chosen->partyRecord);
    for (unit = state->units; unit != NULL; unit = unit->next) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                damage = func_001A5C40(unit, chosen, 0, 1, 1, 1, 0);
                if (category == 2) {
                    if (hp + hp * 10 / 100 < damage) {
                        category = 0;
                    } else if (damage * 2 < hp) {
                        category = 1;
                    }
                }
            }
        }
    }
    if (chosen->status.flags & 0x1000) {
        voices = btlStartVoiceRows[chosen->partyRecord.unitId].voice[1][category];
    } else {
        voices = btlStartVoiceRows[chosen->partyRecord.unitId].voice[0][category];
    }
    count = 0;
    for (i = 0; i < 3; i++) {
        if (voices[i] >= 0) {
            count++;
        }
    }
    if (count == 0) {
        btlBossDebugPrintf(D_003A1E58, category, chosen->partyRecord.unitId);
        return -1;
    }
    voice = voices[sdfGetRandInt(0, count)];
    btlBossDebugPrintf(D_003A1E80, voice, category, chosen->partyRecord.unitId);
    return voice;
}

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

s32 btlTestSideCountEligibility(BtlTask *action) {
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
        *(s32 (**)(u8 *, u8 *, s32))((u8 *)btlGetRuntime() + 0x680);
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
    if (actor->modelResourceKind != 1) {
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
            if (resource->positions[index - 1][0] >= 11) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    BtlUnit *actor = *(BtlUnit **)((u8 *)btlGetRuntime() + 0x228);
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

extern s16 btlGetActorIdForClass(s8 classId);

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second);

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

extern s32 btlBuildActorLinkageSkillList(BtlUnit *, BattleRosterTable *);

extern void btlSetActionActorPanelPresentation(BtlTask *, s8, s8);

extern void sndSetStationedSeVolume(u32);

/* Command panel steps defined below; the refresh calls them before their definitions. */
void btlPopulateCommandPanelGrid(void);
void btlUpdateCommandPanelRowFades(void);
void btlUpdateCommandPanelFades(void);
void btlDrawCommandPanelClassLabel(void);

void btlPopulateCommandPanelGrid(void);
void btlUpdateCommandPanelRowFades(void);
void btlUpdateCommandPanelFades(void);
void btlDrawCommandPanelClassLabel(void);

extern u32 btlGetCommandOptionCount(s32, s8, s8);

static inline s32 btlRefreshCommandRow(BattleSceneObject *object) {
    btlLinkedSelectionTaskBuffer->selectedRow =
        btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
    return btlLinkedSelectionTaskBuffer->selectedRow != 0;
}
extern s32 btlGetActorCommandClass(BattleSceneObject *, s8);

void btlReleaseAndClearChipBlock(void);
void btlResetActorSlotPresentationValue(BtlUnit *object, BattleSceneObject *sceneObject);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1DD8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1DF8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1E18);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1E38);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1E58);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1E80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1FA8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A1FC0);

INCLUDE_RODATA(const s32, "game/code_001A9780", btlActorIdsByClass);

INCLUDE_RODATA(const s32, "game/code_001A9780", btlActorClassPairs);

void func_001AADF8(BattleSceneObject *object) {
    s32 changed = 0;

    switch (fldGetSceneObjectState()) {
    case 1:
    case 2: {
        if (D_00324510.confirm >= 0 && D_00324510.primaryCancel >= 0) {
            if ((D_00324510.alternateNext & 2) != 0) {
                for (; changed == 0;) {
                    if (btlCommandPanelWork->classIndex != 5 || D_00324510.alternateNext < 0) {
                        btlCommandPanelWork->classIndex++;
                        btlCommandPanelWork->labelFade = 0xF0;
                        btlLinkedSelectionTaskBuffer->backdropFade = 0;
                        if (btlCommandPanelWork->classIndex >= 6) {
                            btlCommandPanelWork->classIndex = 0;
                        }
                        btlLinkedSelectionTaskBuffer->selectedRow =
                            btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
                        if (btlLinkedSelectionTaskBuffer->selectedRow != 0) {
                            sndSetStationedSeVolume(0);
                            changed = 1;
                            break;
                        }
                    }
                    btlLinkedSelectionTaskBuffer->selectedRow =
                        btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
                    if (btlLinkedSelectionTaskBuffer->selectedRow != 0) {
                        changed = 1;
                    }
                }
            } else if ((D_00324510.alternatePrev & 2) != 0) {
                for (; changed == 0;) {
                    if (btlCommandPanelWork->classIndex != 0 || D_00324510.alternatePrev < 0) {
                        btlCommandPanelWork->classIndex--;
                        btlCommandPanelWork->labelFade = 0xF0;
                        btlLinkedSelectionTaskBuffer->backdropFade = 0;
                        if (btlCommandPanelWork->classIndex < 0) {
                            btlCommandPanelWork->classIndex = 5;
                        }
                        if (btlRefreshCommandRow(object)) {
                            changed = 1;
                            sndSetStationedSeVolume(0);
                        }
                    }
                    if (btlRefreshCommandRow(object)) {
                        changed = 1;
                    }
                }
            }
        }
    } /* fall through */
    case 3:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        btlCommandPanelWork->labelEntry = btlGetActorIdForClass(btlCommandPanelWork->classIndex);
        btlGetActorClassPair(btlCommandPanelWork->classIndex,
                             &btlCommandPanelWork->classX, &btlCommandPanelWork->classY);
        btlPopulateCommandPanelGrid();
        btlUpdateCommandPanelRowFades();
        btlUpdateCommandPanelFades();
        btlDrawCommandPanelClassLabel();
        break;
    case 4:
        break;
    case 5:
        btlReleaseAndClearChipBlock();
        break;
    }

    if (changed != 0) {
        s32 kind = btlGetActorCommandClass(object, btlCommandPanelWork->classIndex);
        u16 entry = object->selections[kind].entry;

        if (kind == 4) {
            btlSetActionActorPanelPresentation(object->owner, 1, 0);
            if (D_00359160.entries[entry].first != 0) {
                btlResetActorSlotPresentationValue(D_00359160.entries[entry].first, object);
            }
            if (D_00359160.entries[entry].second != 0) {
                btlResetActorSlotPresentationValue(D_00359160.entries[entry].second, object);
            }
        } else {
            btlSetActionActorPanelPresentation(object->owner, 1, 0);
        }
    }
}

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

extern const s16 D_003BB3F0[];
extern const u32 D_003A2090[];
extern const s32 D_003A20A0[][2];
extern const s32 D_003A20D0[][2];
extern BtlResBlock *btlResourceBlock;
extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

/* Publish and draw visible command-slot positions in the two panel banks. */
void func_001AB558(void) {
    s16 sprites[2];
    u32 colors[4];
    s32 positionsA[6][2];
    s32 positionsB[6][2];
    s32 i;

    memcpy(sprites, D_003BB3F0, sizeof(sprites));
    memcpy(colors, D_003A2090, sizeof(colors));
    memcpy(positionsA, D_003A20A0, sizeof(positionsA));
    memcpy(positionsB, D_003A20D0, sizeof(positionsB));
    if (btlCommandPanelWork->state < 5) {
        if (btlCommandPanelWork->state > 0) {
            for (i = 0; i < 5; i++) {
                if (btlCommandPanelWork->slotsA[i].hidden == 0) {
                    colors[0] = btlCommandPanelWork->slotsA[i].fadeValue | 0x80808000;
                    colors[1] = colors[0];
                    colors[2] = colors[0];
                    colors[3] = colors[0];
                    btlCommandPanelWork->slotsA[i].position[0] = positionsA[i][0];
                    btlCommandPanelWork->slotsA[i].position[1] = positionsA[i][1];
                    effDrawSpriteWithCornerColors((btlCommandPanelWork->slotsA[i].position[0] + 5) << 4,
                                                 (btlCommandPanelWork->slotsA[i].position[1] + 0x12D) << 3,
                                                 0, colors, 0, btlResourceBlock->resA,
                                                 sprites[0], 0x53);
                }
                if (btlCommandPanelWork->slotsB[i].hidden == 0) {
                    colors[0] = btlCommandPanelWork->slotsB[i].fadeValue | 0x80808000;
                    colors[1] = colors[0];
                    colors[2] = colors[0];
                    colors[3] = colors[0];
                    btlCommandPanelWork->slotsB[i].position[0] = positionsB[i][0];
                    btlCommandPanelWork->slotsB[i].position[1] = positionsB[i][1];
                    effDrawSpriteWithCornerColors((btlCommandPanelWork->slotsB[i].position[0] + 5) << 4,
                                                 (btlCommandPanelWork->slotsB[i].position[1] + 0x12D) << 3,
                                                 0, colors, 0, btlResourceBlock->resA,
                                                 sprites[1], 0x53);
                }
            }
        }
    }
}


typedef struct BattleCornerFadeDefaults {
    s16 values[20];
} BattleCornerFadeDefaults;

typedef struct BattleCornerPhaseDefaults {
    u32 values[12];
} BattleCornerPhaseDefaults;

extern const BattleCornerFadeDefaults D_003A2100;

extern const BattleCornerPhaseDefaults D_003A2128;

/* Advance the global command panel fades and its pulsing corner opacity. */
void btlUpdateCommandPanelFades(void) {
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

extern void effDrawSpriteWithCornerColors(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

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

/* Draw the global command panel class sprites and label with their current fades. */
void btlDrawCommandPanelClassLabel(void) {
    BattlePanelColors colors = D_003A2158;
    BattleClassLabelOffsets offsets = D_003A2168;
    if (btlCommandPanelWork->state < 5) {
        if (btlCommandPanelWork->state > 0) {
            colors.values[0] = btlCommandPanelWork->cornerFade[0] | 0x80808000;
            colors.values[1] = btlCommandPanelWork->cornerFade[1] | 0x80808000;
            colors.values[2] = btlCommandPanelWork->cornerFade[2] | 0x80808000;
            colors.values[3] = btlCommandPanelWork->cornerFade[3] | 0x80808000;
            effDrawSpriteWithCornerColors(0x50, 0x968, 0, colors.values, 0, btlResourceBlock->resA, 0x1E, 0x53);
            func_001AB558();
            colors.values[0] = btlCommandPanelWork->classFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            effDrawSpriteWithCornerColors((btlCommandPanelWork->classX + 5) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1A, 0x53);
            effDrawSpriteWithCornerColors((btlCommandPanelWork->classX + 0x37) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1B, 0x53);
            colors.values[0] = btlCommandPanelWork->labelFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            {
                s32 x = btlCommandPanelWork->classX + 5;
                s32 y = btlCommandPanelWork->classY + 0x12D;
                effDrawSpriteWithCornerColors((x + offsets.values[btlCommandPanelWork->classIndex][0]) << 4,
                             (y + offsets.values[btlCommandPanelWork->classIndex][1]) << 3,
                             0, colors.values, 0, btlResourceBlock->resA, (s16)btlCommandPanelWork->labelEntry, 0x53);
            }
        }
    }
}

void btlReleaseAndClearChipBlock(void) {
    sdfChipFreeDirect(btlCommandPanelWork);
    btlCommandPanelWork = 0;
}

extern const BattlePanelColors D_003A21A8;

/* Draw each digit and its enlarged faded layers, then restore the current sprite bounds. */
void func_001AC080(s32 x, s32 y, u32 color, const char *digits, s32 scale) {
    BattlePanelColors colors = D_003A21A8;
    s32 length = strlen(digits);
    s32 opacity = color >> 24;
    s32 index;
    s32 remaining;
    s32 advance = 0;
    u32 green;
    u32 blue;
    s32 layerShift;
    s32 vertical;
    s32 thirdXOffset;
    s32 thirdY;

    index = 0;
    if (length <= 0) {
        return;
    }
    layerShift = scale >> 8;
    vertical = y - ((opacity - 0x80) >> 4);
    green = (color & 0xFF00) << 8;
    blue = (color >> 8) & 0xFF00;
    thirdXOffset = layerShift * 3;
    thirdY = vertical - 1;
    remaining = length;
    do {
        s32 character = (s8)digits[index];
        if (character < ':') {
            if (character >= '0') {
                u32 alpha = color & 0xFF000000;
                u32 red = color << 24;
                u32 channels = green | blue;
                s32 glyph = character - 0x11;
                s32 drawX = x + advance;
                u32 packed = (alpha >> 24) | red | channels;

                colors.values[0] = packed;
                colors.values[1] = packed;
                colors.values[2] = packed;
                colors.values[3] = packed;
                effDrawSpriteWithCornerColors(drawX << 4, y << 3, 0, colors.values, 0,
                             btlResourceBlock->resA, glyph, 0x53);
                if (color > 0x80 && scale > 0x100) {
                    packed = (((alpha >> 3) & 0xFF000000) >> 24) | red | channels;
                    {
                        s32 scaledWidth = btlResourceBlock->resA->workEntries[glyph].sourceWidth * scale;
                        s32 scaledHeight = btlResourceBlock->resA->workEntries[glyph].sourceHeight * scale;
                        scaledWidth *= 3;
                        scaledHeight *= 3;
                        btlResourceBlock->resA->workEntries[glyph].geometry.bounds[2] = (scaledWidth / 4) >> 4;
                        btlResourceBlock->resA->workEntries[glyph].geometry.bounds[3] = (scaledHeight / 4) >> 4;
                    }
                    colors.values[0] = packed;
                    colors.values[1] = packed;
                    colors.values[2] = packed;
                    colors.values[3] = packed;
                    effDrawSpriteWithCornerColors((drawX + layerShift * 4) << 4, vertical << 3,
                                 0, colors.values, 0, btlResourceBlock->resA, glyph, 0x53);
                    effDrawSpriteWithCornerColors((drawX + thirdXOffset) << 4, thirdY << 3,
                                 0, colors.values, 0, btlResourceBlock->resA, glyph, 0x53);
                    btlResourceBlock->resA->workEntries[glyph].geometry.bounds[2] =
                        btlResourceBlock->resA->workEntries[glyph].sourceWidth << 4;
                    btlResourceBlock->resA->workEntries[glyph].geometry.bounds[3] =
                        btlResourceBlock->resA->workEntries[glyph].sourceHeight << 3;
                }
                index++;
                advance += 9;
                continue;
            }
        }
        index++;
        advance += 9;
    } while (--remaining != 0);
}

/* Initialize scaled sprite bounds, then shrink active mirrored records until hidden. */
void btlUpdateShrinkingMirroredSprites(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
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

/* Draw both positions of each active record, restoring shared sprite bounds afterward. */
void btlDrawMirroredPanelSprites(s32 unused, BattleMirroredSpriteRecord *records, s32 count) {
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
                effDrawSpriteWithCornerColors(record->x << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                effDrawSpriteWithCornerColors(record->secondX << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
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
        handle = (u32)sdfMemAllocDirect(0x28);
        block = (BtlResBlock *)sdfMemLock((struct SdfMemBlock *)(handle));
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->resourceAllocationA = sdfReadNamedResource(D_003A21C8, params, 0);
        btlResourceBlock->resourceAllocationB = sdfReadNamedResource(D_003A21E8, params, 0);
        btlResourceBlock->resourceAllocationC = sdfReadNamedResource(D_003A2208, params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_003BB3E4 = 1;
}

void btlLoadResourceBlock(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (btlResourceBlockLoaded == 0) {
        btlResourceBlock->resA = effCreateResourceSlotSetFromAllocation(
            btlResourceBlock->resourceAllocationA, 0);
        btlResourceBlock->resB = effCreateResourceSlotSetFromAllocation(
            btlResourceBlock->resourceAllocationB, 0);
        btlResourceBlock->resC = effCreateResourceSlotSetFromAllocation(
            btlResourceBlock->resourceAllocationC, 0);
        work->resA = btlResourceBlock->resA;
        work->numberGlyphSlots = btlResourceBlock->resB;
        btlResourceBlockLoaded = 1;
    }
}

void btlReleaseResourceBlock(void) {
    BtlState *work = (BtlState *)btlGetRuntime();
    if (btlResourceBlockLoaded != 0) {
        effDestroyResourceSlotSet(btlResourceBlock->resA);
        btlResourceBlock->resA = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resB);
        btlResourceBlock->resB = 0;
        effDestroyResourceSlotSet(btlResourceBlock->resC);
        btlResourceBlock->resC = 0;
        work->resA = 0;
        work->numberGlyphSlots = 0;
        work->resC = 0;
        btlResourceBlockLoaded = 0;
    }
}

extern void btlClearTaskActorSlots(void);

extern s32 btlCreatePhaseChangePanelTask(s32);

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
        (*bank)->rotationAngleDegrees[i] = 30.0f;
        (*bank)->drawOffsetScalePercent[i] = 130;
    }
    btlInitializeActorSlotOrder(kind, count);
    btlClearTaskActorSlots();
    return btlCreatePhaseChangePanelTask(kind);
}

/* Clear each task's eight opaque words, forward for variant 1 and backward otherwise. */
void btlClearTaskActorSlots(void) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlTask *node = context->tasks;
    while (node != 0) {
        s32 i;
        BtlUnit *actor = node->unit;
        if (actor != 0) {
            if (context->mode == 1) {
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
    BtlState *scene;
    s32 fadeCounts[4];

    scene = (BtlState *)btlGetRuntime();
    return fldCountSceneFadeKinds(scene, fadeCounts);
}

extern u8 btlHasRequiredActorStatusBits(BtlUnit *);

extern s32 btlCountEligibleLinkedActors(BtlState *battle);

s32 btlSyncEligibleActorPanelHpMp(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 eligibleCount = btlCountEligibleLinkedActors(battle);
    BtlUnit *unit = battle->units;
    KwlnTask *task = dds3SearchProcessName(D_003BB3B0);
    BattleActorPanelWork *panel;
    s32 i;

    if (task == NULL) {
        return 0;
    }
    panel = (BattleActorPanelWork *)dds3GetProcessData(task);
    /* i is never advanced: the count only gates the walk on any eligible actor. */
    for (i = 0; unit != NULL && i < eligibleCount; unit = unit->next) {
        if (btlHasRequiredActorStatusBits(unit) != 0) {
            s32 slot = unit->lookupId;

            if (panel->activeEntries[slot].presentation.hpState == 3) {
                panel->activeEntries[slot].presentation.hpLevel = unit->partyRecord.hp;
                panel->activeEntries[slot].presentation.hpTarget = unit->partyRecord.hp;
            }
            if (panel->activeEntries[slot].presentation.mpState == 3) {
                panel->activeEntries[slot].presentation.mpLevel = unit->partyRecord.mp;
                panel->activeEntries[slot].presentation.mpTarget = unit->partyRecord.mp;
            }
        }
    }
    return 1;
}

extern u8 btlHasRequiredActorStatusBits(BtlUnit *);

s32 btlSetEligibleActorPanelTransitionState(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *task = dds3SearchProcessName(D_003BB3B0);
    BattleActorPanelWork *panel;
    s32 i;
    BtlUnit *unit;
    s32 eligibleCount;

    if (task == NULL) {
        return 0;
    }
    panel = (BattleActorPanelWork *)dds3GetProcessData(task);
    eligibleCount = btlCountEligibleLinkedActors(battle);
    unit = battle->units;
    /* i is never advanced: the count only gates the walk on any eligible actor. */
    for (i = 0; unit != NULL && i < eligibleCount; unit = unit->next) {
        if (btlHasRequiredActorStatusBits(unit) != 0) {
            panel->activeEntries[unit->lookupId].presentation.transitionState = 4;
        }
    }
    return 1;
}


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

/* Mark (0), clear (1), or query (other) the recorded bit for a linkage skill ID. */
s32 btlAccessRecordedLinkageSkillFlag(s32 id, s32 operation) {
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

/* Clear the three DDS1 scratch words used to deduplicate linkage notices. */
void btlClearPendingLinkageSkillFlags(void) {
    s32 *flagWord;
    s32 counter;

    flagWord = btlPendingLinkageSkillFlags;
    flagWord += 2;
    counter = 2;
    do {
        counter = counter - 1;
        *flagWord = 0;
        flagWord = flagWord - 1;
    } while (-1 < counter);
}

/* Mark (0), clear (1), or query (other) the scratch bit for a pending linkage notice. */
s32 btlAccessPendingLinkageSkillFlag(s32 id, s32 operation) {
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
        btlPendingLinkageSkillFlags[word] |= 1 << bit;
        break;
    case 1:
        btlPendingLinkageSkillFlags[word] &= ~(1 << bit);
        break;
    default:
        return ((btlPendingLinkageSkillFlags[word] & (1 << bit)) != 0);
    }
    return 1;
}

extern const char *D_003BB3A0;

s32 btlSetTaskPhase2(void) {
    KwlnTask *task = dds3SearchProcessName(D_003BB3A0);
    if (task != 0) {
        *(s32 *)dds3GetProcessData(task) = 2;
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
        task = dds3SearchProcessName(D_003BB3B0);
        if (task != NULL) {
            work = (BattleActorPanelWork *)dds3GetProcessData(task);
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

typedef struct { u16 flag; u8 value; u8 pad03; } MesWindowState;

typedef struct MesWindowConfig {
    MesWindowState state[9];
} MesWindowConfig;

typedef struct MesWindowList {
    s8 state;
    u8 pad01[3];
    s32 window[9];
    u8 pad28[4];
    s16 alpha; /* 001ADECC..001ADF54: signed halfword opacity and clamps. */
    u8 pad2E[2];
    BattlePanelColors colors; /* 001AE064..001AE14C: four corner colors. */
    s32 x;
    s32 y;
    MesWindowConfig config;
} MesWindowList;
typedef char MesWindowListSizeCheck[sizeof(MesWindowList) == 0x6C ? 1 : -1];
typedef char MesWindowListAlphaOffsetCheck[((u32)&((MesWindowList *)0)->alpha == 0x2C) ? 1 : -1];
typedef char MesWindowListColorsOffsetCheck[((u32)&((MesWindowList *)0)->colors == 0x30) ? 1 : -1];

typedef struct ItfMesSub ItfMesSub;

extern ItfMesSub D_0032A668;

extern s32 itfMesCreateWindow(ItfMesSub *);

extern u32 btlIsNamedBattleTaskRegistered(void);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

extern s32 btlUpdateValueMessageWindows(KwlnTask *);

extern void itfMesCloseAllWindows(KwlnTask *);

s32 btlCreateMahenPanelTask(const MesWindowConfig *config) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    MesWindowList *work;
    KwlnTask *task;
    s32 i;

    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
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
    task = dds3StartProcess(btlMahenPanelTaskNameRef, 0x2B0E, 1, 1,
                          btlUpdateValueMessageWindows, itfMesCloseAllWindows, (u32)work);
    dds3LinkProcess(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(10, (s32)task);
    return 1;
}

u32 btlIsNamedBattleTaskRegistered(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(10);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(btlMahenPanelTaskNameRef);
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

    task = dds3SearchProcessName(btlMahenPanelTaskNameRef);
    if (task != 0) {
        work = (MesWindowList *)dds3GetProcessData(task);
        work->state = 2;
    }
}


extern s32 btlUpdateAndDrawAnalysisPanel(KwlnTask *);

extern void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *);

/* Create the analysis panel, replacing any panel task already registered. */
extern u32 btlHasRegisteredAnalysisPanelTask(void);

s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlAnalysisPanelWork *data;
    s32 task;

    if (btlHasRegisteredAnalysisPanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(11), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->page = 0;
    data->x = -20;
    data->y = 133;
    data->duration = duration;
    data->entry = entry;
    data->frame = 0;
    task = (s32)dds3StartProcess(btlAnalyzPanelTaskNameRef, 0x2B0E, 1, 1, btlUpdateAndDrawAnalysisPanel,
                          btlReleaseTaskAndRefreshCursorIfFlagged, (u32)data);
    dds3LinkProcess(context->scriptOwner, (KwlnTask *)task);
    btlSetTrackedTaskHandle(11, task);
    context->battleFlags &= ~0x100000;
    return 1;
}

u32 btlHasRegisteredAnalysisPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(0xb);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(btlAnalyzPanelTaskNameRef);
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
    return *(s8 *)dds3GetProcessData(dds3SearchProcessName(btlAnalyzPanelTaskNameRef));
}

u32 btlRequestAnalysisPanelClose(void) {
    BtlAnalysisPanelWork *work;
    KwlnTask *task;

    task = dds3SearchProcessName(btlAnalyzPanelTaskNameRef);
    if (task != 0) {
        work = (BtlAnalysisPanelWork *)dds3GetProcessData(task);
        work->state = 2;
    }
    return 1;
}

/* Registered guide-panel task: fade, four corner colors, and message selection. */
typedef struct BtlGuidePanelWork {
    s8 state;
    u8 pad01[7];
    s16 fadeLevel;             /* 0x08 */
    u8 pad0A[2];
    BattlePanelColors colors;  /* 0x0C: TL, TR, BR, BL */
    s32 unk1C;
    s32 y;
    s32 windowIndex;
    s32 entryIndex;
} BtlGuidePanelWork;

typedef char BtlGuidePanelWorkSizeCheck[sizeof(BtlGuidePanelWork) == 0x2C ? 1 : -1];

extern s32 btlUpdateGuidePanelTask(KwlnTask *);

extern void btlReleaseMessageWindowTask(KwlnTask *);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlCreateGuidePanelTask(s32 windowIndex, s32 entryIndex) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlGuidePanelWork *data;
    s32 task;

    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    data = sdfAllocAndClearQuadwords(sizeof(*data));
    data->state = 0;
    data->unk1C = 0x18;
    data->y = 0x60;
    data->windowIndex = windowIndex;
    data->entryIndex = entryIndex;
    task = (s32)dds3StartProcess(D_003BB3C4, 0x2B0E, 1, 1, btlUpdateGuidePanelTask,
                          btlReleaseMessageWindowTask, (u32)data);
    dds3LinkProcess(context->scriptOwner, (KwlnTask *)task);
    btlSetTrackedTaskHandle(9, task);
    return 1;
}

void btlRequestGuidePanelClose(void) {
    u8 *puVar1;
    KwlnTask *temp_v0;

    temp_v0 = dds3SearchProcessName(D_003BB3C4);
    if (temp_v0 != 0) {
        puVar1 = (u8 *)dds3GetProcessData(temp_v0);
        *puVar1 = 2;
    }
}

u32 btlHasRegisteredGuidePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(9);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(D_003BB3C4);
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
    KwlnTask *task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    if (task != 0) {
        ((BtlPhaseTask *)dds3GetProcessData(task))->phase = 5;
        btlCommandPanelWork->state = 3;
    }
}

void btlSetTrackedTaskDisplayMode(s32 mode) {
    s32 task = btlGetTrackedTaskHandle(7);
    if (task != 0) {
        s32 *data = (s32 *)dds3GetProcessData((KwlnTask *)task);
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
    dds3GetProcessData((KwlnTask *)btlGetTrackedTaskHandle(7));
    btlTrackedTaskHandles->fadeKindsCached = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(6);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(D_003BB3BC);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct BtlPsechgPositions {
    BattleSelectionPosition points[5];
} BtlPsechgPositions;

extern const BtlPsechgPositions D_003A2270;
extern const BtlPsechgPositions D_003A2298;
extern s32 btlUpdatePhaseGatedTaskUntilTimeout(KwlnTask *);
extern void btlReleasePsechgPanelWork(KwlnTask *);
extern void btlSetTrackedTaskHandle(s32, s32);
/* Create the phase-gated PSECHG panel with one of two initial point sets. */
s32 btlCreatePhaseChangePanelTask(s32 variant) {
    BtlPsechgPositions firstPoints = D_003A2270;
    BtlPsechgPositions secondPoints = D_003A2298;
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *previous = (KwlnTask *)btlGetTrackedTaskHandle(6);
    BattlePhasePanelWork *work;
    BtlPsechgPositions *points;
    s32 *destination;
    s32 *source;
    KwlnTask *task;
    s32 i;

    if (btlHasRegisteredPsechgPanelTask() != 0) {
        dds3KillProcess(previous, 0);
    }
    if ((battle->battleFlags & 0x200) == 0) {
        battle->battleFlags |= 0x200;
    }
    btlTrackedTaskHandles->phaseGate = 0;
    work = sdfAllocAndClearQuadwords(sizeof(*work));
    if (variant == 0) {
        work->mode = 0;
        points = &firstPoints;
    } else {
        work->mode = 1;
        points = &secondPoints;
    }
    /* Copy the five coordinate pairs into the current rows. */
    destination = &work->current[0][1];
    source = (s32 *)points->points;
    for (i = 0; i < 5; i++) {
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    task = dds3StartProcess(D_003BB3BC, 0x2B0E, 1, 1, btlUpdatePhaseGatedTaskUntilTimeout,
                          btlReleasePsechgPanelWork, (u32)work);
    dds3LinkProcess(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(6, (s32)task);
    return 1;
}

u32 btlHasRegisteredSkillNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(D_003BB3AC);
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

/* Replace conflicting label tasks and create a skill-name transition; returns one. */
s32 btlCreateSkillNamePanel(const u8 *text) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    BtlPanelTransitionWork *work;
    struct FrFontGlyph *glyph;

    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess(task, 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    work = sdfAllocAndClearQuadwords(sizeof(*work));
    work->text = text;
    work->frames = 30;
    work->fade = 0;
    work->fadeLevels[2] = work->fadeLevels[0] = 0x40;
    work->fadeLevels[3] = work->fadeLevels[1] = 0x10;
    glyph = itfCreateConvertedTextGlyph(0x1000, 0x200, 0xFF0000, 0x80808080, text, 0);
    work->width = frFontMeasureLines(glyph);
    frDestroyFontRequestAfter(glyph);
    work->initial[0].x = work->width - work->width / 2 + 0x105;
    work->initial[0].y = 0x40;
    work->initial[1].x = 0x92 - work->width / 2;
    work->initial[1].y = 0x40;
    task = dds3StartProcess(D_003BB3AC, 0x2B0E, 1, 1,
                          btlUpdateSkillNamePanelTask, btlFreeRegisteredTaskData, (u32)work);
    dds3LinkProcess(battle->scriptOwner, task);
    work->task = task;
    btlSetTrackedTaskHandle(1, (s32)task);
    return 1;
}

u32 btlHasRegisteredAphNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(0);
    if (task != 0) {
        if (dds3ExistProcess(task) != 0) {
            task = dds3SearchProcessName(D_003BB3A8);
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

s32 btlReplaceDialogTasksAndQueueMessage(s32 window, s32 entryIndex) {
    s32 context = (s32)(u32)btlGetRuntime();
    s32 task = btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)task, 0);
    }
    data = (MsgQueueTaskData *)sdfAllocAndClearQuadwords(0x40);
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(10), 0);
    }
    data->id = window;
    data->unk08 = entryIndex;
    data->value = 0x2D;
    task = (s32)dds3StartProcess(D_003BB3A8, 0x2B0E, 1, 1, btlDrawTimedDialogTask, btlReleaseDialogTaskData, (u32)data);
    dds3LinkProcess((KwlnTask *)*(u32 *)(context + 0x29C), (KwlnTask *)task);
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

/* Fill four packed panel colors from actor state, style, phase and scene mode. */
void btlChooseActorPanelColors(BtlUnit *unit, s8 style, s8 phase, s32 unused, BattlePanelColors *colors) {
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

extern void itfMesSetTextSlotFromValue(s32 window, s32 slotIndex, s32 value, s32 selector);
extern void itfMesCopyStringToWindowTableSlot(s32, u32, const void *);
extern char D_003BB3F8[];
extern s32 sprintf(char *, const char *, ...);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, s32, s32, s32, s32);

/* Fade and draw the task's enabled label/value windows; return -1 after fade-out. */
s32 btlUpdateValueMessageWindows(KwlnTask *task) {
    MesWindowList *work;
    s32 i;
    s32 count;
    s32 lineOffset = 0;
    s32 value;
    s32 width;
    s32 halfAlpha;
    char text[16];

    btlGetRuntime();
    work = (MesWindowList *)dds3GetProcessData(task);
    switch (work->state) {
    case 0:
        work->alpha += 16;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 128 ? 128 : work->alpha;
        if (work->alpha >= 128) {
            work->state = 1;
        }
        break;
    case 1:
        break;
    case 2:
    default:
        work->alpha -= 16;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 128 ? 128 : work->alpha;
        if (work->alpha <= 0) {
            return -1;
        }
        break;
    }
    switch (work->state) {
    case 0:
    case 1:
    case 2:
        count = 0;
        if (work->state == 1) {
            for (i = 0; i < 9; i++) {
                if (work->config.state[i].flag != 0) {
                    value = work->config.state[i].value;
                    count++;
                    itfMesSetTextSlotFromValue(work->window[i], 0, work->config.state[i].flag, 12);
                    sprintf(text, D_003BB3F8, value);
                    itfMesCopyStringToWindowTableSlot(work->window[i], 1, text);
                    width = itfMesMeasureEntryItem(work->window[i], 0, 0);
                    itfMesBlk24MoveTo(work->window[i], (256 - (width >> 5)) << 4,
                                      (work->y + lineOffset) << 4);
                    lineOffset += 12;
                    itfMesStartEntry(work->window[i], 0, 0);
                }
            }
        }
        if (count != 0) {
            halfAlpha = work->alpha >> 1;
            work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
                (halfAlpha << 24) | 0xA0A0A0;
            evtSetDrawSurfaceIndex(0x53);
            evtSubmitPrimaryGsTest(1, 1, 128, 3, 0, 0, 1, 1);
            evtSubmitPrimaryAlphaBlendMode(1);
            evtSubmitDefaultDepthGradientRect(work->x, work->y - 2, 456, 2,
                work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
            halfAlpha = work->alpha >> 1;
            work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
                (halfAlpha << 24) | 0x101010;
            evtSubmitPrimaryAlphaBlendMode(1);
            evtSubmitDefaultDepthGradientRect(work->x, work->y, 456, count * 24 + 48,
                work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
            halfAlpha = work->alpha >> 1;
            work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
                (halfAlpha << 24) | 0xA0A0A0;
            evtSubmitPrimaryAlphaBlendMode(1);
            evtSubmitDefaultDepthGradientRect(work->x, work->y + count * 24 + 48, 456, 2,
                work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
            evtSubmitPrimaryAlphaBlendMode(1);
        }
        break;
    }
    return 0;
}

void itfMesCloseAllWindows(KwlnTask *handle) {
    MesWindowList *list;
    s32 i;
    btlGetRuntime();
    list = (MesWindowList *)dds3GetProcessData(handle);
    for (i = 0; i < 9; i++) {
        if (list->config.state[i].flag != 0) {
            itfMesCleanupWindow(list->window[i], 0);
            itfMesDestroyWindowIfPresent(list->window[i]);
        }
    }
    sdfChipFreeDirect(list);
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

void btlDrawPanelIconGroup(s32 section, s32 delta) {
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
        effDrawSpriteWithCornerColors((*strip)[0] << 4, (*strip)[1] << 3, 0, colors, 0,
                     btlResourceBlock->resA, (*strip)[2], 0x53);
    }
}

extern void func_001AF058(BtlLinkedCommand *, BtlAnalysisPanelWork *);
extern void func_001AF5D0(BtlLinkedCommand *, BtlAnalysisPanelWork *);
extern void btlDrawCompositePanelSprites(void *, s32, s32, u32);
extern void func_001AFF78(BtlAnalysisPanelWork *, s32, s32, s32, s32);
extern u16 *btlGatherEnemyCommands(BtlUnit *, s16 *);
extern s32 frFontDrawGlyphChain(struct FrFontGlyph *, s8, u32);
extern s32 frFontDrawGlyphWithSharedFlags(struct FrFontGlyph *, s8);

/* Shared text submission: choose the standard shared draw flags and enqueue. */
static inline void btlSubmitSharedTextGlyph(struct FrFontGlyph *glyph) {
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    frDestroyFontRequestAfter(glyph);
}
extern const u8 *D_003BAA74, *D_003BAA80, *D_003BAA88;
extern const char D_003BB400[], D_003BB408[], D_003BB410[];
extern u8 btlAnalysisTextScratch[];
extern s32 sprintf(char *, const char *, ...);
extern DatBattleSceneRecord *datBattleSceneRecords;
extern void btlDrawIndexedBattleEntryGlyphs(s32, s32, s32, s32, u16);
extern void btlDrawThreePanelSpriteStrips(s32, s32, s32, s32);

/* Advance and draw the seven analysis rows while the panel fades. */
s32 btlUpdateAndDrawAnalysisPanel(KwlnTask *task) {
    char text[128];
    s32 offsets[8] = {52, 95, 157, 132, 132, 53, 53, 48};
    BtlState *battle;
    BtlAnalysisPanelWork *work;
    s32 row;
    struct FrFontGlyph *glyph;

    battle = (BtlState *)btlGetRuntime();
    work = (BtlAnalysisPanelWork *)dds3GetProcessData(task);
    work->frame++;
    work->frame = work->frame > 0 ?
        (work->frame < 0x1001 ? work->frame : 0x1000) : 0;
    if (work->frame >= work->duration && work->duration != 0) work->state = 2;

    switch (work->state) {
    case 0:
        func_001AF058(&battle->cameraCommand, work);
        func_001AF5D0(&battle->cameraCommand, work);
        if (work->page == 7) work->state = 1;
        break;
    case 1:
        func_001AF058(&battle->cameraCommand, work);
        func_001AF5D0(&battle->cameraCommand, work);
        work->alpha += 32;
        work->alpha = work->alpha > 0 ?
            (work->alpha < 129 ? work->alpha : 128) : 0;
        break;
    case 2:
        work->alpha -= 32;
        work->alpha = work->alpha > 0 ?
            (work->alpha < 129 ? work->alpha : 128) : 0;
        if (work->alpha <= 0) return -1;
        break;
    }
    if (work->state < 3) {
        if (work->state > 0) {
            for (row = 0; row < 7; row++) {
                s32 xOffset = offsets[row];
                switch (row) {
                case 0: {
                    BtlUnit *actor;
                    btlDrawCompositePanelSprites(work, work->x, work->y, work->alpha);
                    if ((datBattleSceneRecords[battle->battleMode].flags & 1) != 0) {
                        btlDrawPanelIconGroup(0, work->alpha);
                        break;
                    }
                    actor = (BtlUnit *)work->entry;
                    if ((datEnemyRecords[actor->partyRecord.unitId].flags & 0x800) != 0 || datEnemyRecords[actor->partyRecord.unitId].usePanelIcons != 0) {
                        btlDrawPanelIconGroup(0, work->alpha);
                        break;
                    }
                    sprintf(text, D_003BB400, actor->partyRecord.level);
                    glyph = (struct FrFontGlyph *)itfCreateFont2TextGlyph((work->x + xOffset) << 4,
                                          (work->y - 2) << 3, 0xFF0000,
                                          0x80808000 | (s32)work->alpha, text, 0);
                    frFontDrawGlyphChain(glyph, 1, 0x53);
                    frDestroyFontRequestAfter(glyph);
                    break;
                }
                case 1: {
                    BtlUnit *actor;
                    actor = (BtlUnit *)work->entry;
                    glyph = itfCreateConvertedTextGlyph(
                        (work->x + xOffset - 6) << 4, (work->y - 8) << 3,
                        0xFF0000, 0x80808000 | (s32)work->alpha,
                        D_003BAA88 + datEnemyRecords[actor->partyRecord.unitId].pad04 * 7, 0);
                    btlSubmitSharedTextGlyph(glyph);
                    break;
                }
                case 2: {
                    BtlUnit *actor;
                    actor = (BtlUnit *)work->entry;
                    glyph = itfCreateConvertedTextGlyph(
                        (work->x + xOffset + 2) << 4, (work->y - 7) << 3,
                        0xFF0000, 0x80808000 | (s32)work->alpha,
                        D_003BAA74 + actor->partyRecord.unitId * 17, 0);
                    btlSubmitSharedTextGlyph(glyph);
                    break;
                }
                case 3: {
                    BtlUnit *actor;
                    if ((datBattleSceneRecords[battle->battleMode].flags & 1) != 0) {
                        btlDrawPanelIconGroup(1, work->alpha);
                        break;
                    }
                    actor = (BtlUnit *)work->entry;
                    if ((datEnemyRecords[actor->partyRecord.unitId].flags & 0x800) != 0 || datEnemyRecords[actor->partyRecord.unitId].usePanelIcons != 0) {
                        btlDrawPanelIconGroup(1, work->alpha);
                        break;
                    }
                    sprintf(text, D_003BB408, actor->partyRecord.hp,
                                  actor->partyRecord.maxHp);
                    func_001AFF78(work, 0, work->x, work->y, work->alpha);
                    glyph = (struct FrFontGlyph *)itfCreateFont2TextGlyph((work->x + xOffset) << 4,
                                          (work->y + 17) << 3, 0xFF0000,
                                          0x80808000 | (s32)work->alpha, text, 0);
                    frFontDrawGlyphChain(glyph, 1, 0x53);
                    frDestroyFontRequestAfter(glyph);
                    break;
                }
                case 4: {
                    BtlUnit *actor;
                    if ((datBattleSceneRecords[battle->battleMode].flags & 1) != 0) {
                        btlDrawPanelIconGroup(2, work->alpha);
                        break;
                    }
                    actor = (BtlUnit *)work->entry;
                    if ((datEnemyRecords[actor->partyRecord.unitId].flags & 0x800) != 0 || datEnemyRecords[actor->partyRecord.unitId].usePanelIcons != 0) {
                        btlDrawPanelIconGroup(2, work->alpha);
                        break;
                    }
                    sprintf(text, D_003BB408, actor->partyRecord.mp,
                                  actor->partyRecord.maxMp);
                    func_001AFF78(work, 1, work->x, work->y, work->alpha);
                    glyph = (struct FrFontGlyph *)itfCreateFont2TextGlyph((work->x + xOffset) << 4,
                                          (work->y + 41) << 3, 0xFF0000,
                                          0x80808000 | (s32)work->alpha, text, 0);
                    frFontDrawGlyphChain(glyph, 1, 0x53);
                    frDestroyFontRequestAfter(glyph);
                    break;
                }
                case 5: {
                    BtlUnit *actor;
                    s32 yOffset = 93;
                    s16 count;
                    s32 i;
                    u16 *entries;
                    if ((datBattleSceneRecords[battle->battleMode].flags & 1) != 0) {
                        btlDrawPanelIconGroup(3, work->alpha);
                        break;
                    }
                    actor = (BtlUnit *)work->entry;
                    if ((datEnemyRecords[actor->partyRecord.unitId].flags & 0x800) != 0 || datEnemyRecords[actor->partyRecord.unitId].usePanelIcons != 0) {
                        btlDrawPanelIconGroup(3, work->alpha);
                        break;
                    }
                    entries = btlGatherEnemyCommands(actor, &count);
                    for (i = 0; i < count; i++) {
                        btlDrawThreePanelSpriteStrips((s32)work, work->x + xOffset,
                                                       work->y + yOffset, work->alpha);
                        btlDrawIndexedBattleEntryGlyphs(work->x + xOffset,
                            work->y + yOffset, 0xFF0010,
                            0x80808000 | (s32)work->alpha, *entries++);
                        if (i == 3) {
                            yOffset = 93;
                            xOffset += 120;
                        } else yOffset += 22;
                    }
                    break;
                }
                case 6: {
                    BtlUnit *actor;
                    s32 yOffset = 206;
                    s32 position;
                    s32 character;
                    if ((datBattleSceneRecords[battle->battleMode].flags & 1) != 0) {
                        btlDrawPanelIconGroup(4, work->alpha);
                        break;
                    }
                    actor = (BtlUnit *)work->entry;
                    if ((datEnemyRecords[actor->partyRecord.unitId].flags & 0x800) != 0 || datEnemyRecords[actor->partyRecord.unitId].usePanelIcons != 0) {
                        btlDrawPanelIconGroup(4, work->alpha);
                        break;
                    }
                    position = 0;
                    memset(btlAnalysisTextScratch, 0, 256);
                    sprintf((char *)btlAnalysisTextScratch, D_003BB410,
                        D_003BAA80 + ((BtlUnit *)work->entry)->partyRecord.unitId * 189);
                    while ((character = btlAnalysisTextScratch[position]) != 0) {
                        s32 length = 0;
                        if (character != '_' && character != 0) {
                            const u8 *cursor = btlAnalysisTextScratch + position;
                            s32 nextCharacter;
                            do {
                                text[length++] = *cursor++;
                                position++;
                                nextCharacter = *cursor;
                            } while (nextCharacter != '_' && nextCharacter != 0);
                        }
                        text[length] = '\0';
                        position++;
                        {
                            /* Capture this line before advancing the next line origin. */
                            s32 lineX = work->x + xOffset;
                            s32 lineY = work->y + yOffset;
                            yOffset += 24;
                            glyph = itfCreateConvertedTextGlyph(
                                lineX << 4, lineY << 3,
                                0xFF0000, 0x80808000 | (s32)work->alpha, (const u8 *)text, 0);
                        }
                        btlSubmitSharedTextGlyph(glyph);
                    }
                    break;
                }
                }
            }
        }
    }
    return 0;
}

extern void btlInitCursorAndApplyAction(BtlLinkedCommand *, BtlCamState *);

void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *handle) {
    BtlState *work = (BtlState *)btlGetRuntime();
    u8 *data = (u8 *)dds3GetProcessData(handle);
    sdfChipFreeDirect(data);
    btlSetTrackedTaskHandle(11, 0);
    if (work->cameraCommand.task->unit->partyRecord.status & 0x80) {
        btlInitCursorAndApplyAction(&work->cameraCommand, &work->cameraCommand.camera);
    }
    work->battleFlags |= 0x100000;
}


extern f32 D_003A2450[4];

/* Advance one corner toward the panel boundary before moving to the next edge. */
s32 btlAdvancePanelCornerPhase(BtlLinkedCommand *command, BtlAnalysisPanelWork *work) {
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

extern f32 D_003A2460[4];
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2450);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2460);

void func_001AF058(BtlLinkedCommand *command, BtlAnalysisPanelWork *work) {
    BtlAnalysisPanelWork *panel = work;
    f32 center[4] __attribute__((aligned(16)));
    s32 horizontalExtent;
    s32 verticalExtent;

    memcpy(center, D_003A2460, sizeof(center));
    switch (work->page) {
    case 0:
        /* Audited original SDK pipeline: five stores precede all coordinate reads. */
        VU0_LOAD_VF(vf10, center);
        VU0_STORE_VF_UNCLOBBERED(vf10, panel->anchor);
        VU0_STORE_VF_UNCLOBBERED(vf10, panel->corners[3]);
        VU0_STORE_VF_UNCLOBBERED(vf10, panel->corners[0]);
        VU0_STORE_VF_UNCLOBBERED(vf10, panel->corners[1]);
        VU0_STORE_VF_UNCLOBBERED(vf10, panel->corners[2]);
        {
            f32 centerZ = panel->anchor[2] + 200.0f;
            f32 lastZ = panel->corners[3][2] + 200.0f;
            f32 firstZ = panel->corners[0][2] + 200.0f;
            f32 secondZ = panel->corners[1][2] + 200.0f;
            f32 thirdZ = panel->corners[2][2] + 200.0f;
            f32 centerX = panel->anchor[0] - 200.0f;
            f32 centerY = panel->anchor[1] - 174.0f;
            f32 firstX = panel->corners[0][0] - 200.0f;
            f32 firstY = panel->corners[0][1] - 174.0f;
            f32 secondX = panel->corners[1][0] + 200.0f;
            f32 secondY = panel->corners[1][1] + 174.0f;
            f32 thirdX = panel->corners[2][0] - 200.0f;
            f32 thirdY = panel->corners[2][1] + 174.0f;
            f32 lastX = panel->corners[3][0] + 200.0f;
            f32 lastY = panel->corners[3][1] - 174.0f;

            work->page = 1;
            panel->anchor[2] = centerZ;
            panel->corners[3][2] = lastZ;
            panel->corners[0][2] = firstZ;
            panel->corners[1][2] = secondZ;
            panel->corners[2][2] = thirdZ;
            panel->anchor[0] = centerX;
            panel->anchor[1] = centerY;
            panel->corners[0][0] = firstX;
            panel->corners[0][1] = firstY;
            panel->corners[1][0] = secondX;
            panel->corners[1][1] = secondY;
            panel->corners[2][0] = thirdX;
            panel->corners[2][1] = thirdY;
            panel->corners[3][0] = lastX;
            panel->corners[3][1] = lastY;
        }
        panel->timer = 0;
        break;
    case 1:
        panel->timer++;
        panel->timer = panel->timer <= 0 ? 0 :
            panel->timer >= 10 ? 10 : panel->timer;
        if (panel->timer >= 10) {
            work->page = 2;
        }
        break;
    case 2:
        btlAdvancePanelCornerPhase(command, panel);
        panel->edgeAlpha += 32;
        panel->edgeAlpha = panel->edgeAlpha <= 0 ? 0 :
            panel->edgeAlpha >= 128 ? 128 : panel->edgeAlpha;
        if (panel->edgeAlpha >= 128) {
            work->page = 3;
        }
        break;
    case 3:
        btlAdvancePanelCornerPhase(command, panel);
        panel->timer++;
        panel->timer = panel->timer <= 0 ? 0 :
            panel->timer >= 26 ? 26 : panel->timer;
        if (panel->timer >= 26) {
            work->page = 4;
            panel->timer = 180;
        }
        break;
    case 4:
    {
        f32 insetLeft, insetRight, insetTop, insetBottom;
        f32 insetLimit;
        panel->timer = (panel->timer + 24) % 360;
        horizontalExtent = (s32)(((sdfSinPoly((f32)((panel->timer + 90) % 360) /
            180.0f * 3.14159f) + 1.0f) * 0.5f) * 40.0f + 0.0f);
        verticalExtent = (s32)(((sdfSinPoly((f32)((panel->timer + 90) % 360) /
            180.0f * 3.14159f) + 1.0f) * 0.5f) * 34.0f + 0.0f);
        insetLeft = center[0] - 200.0f;
        insetRight = center[0] + 200.0f;
        insetTop = center[1] - 174.0f;
        insetBottom = center[1] + 174.0f;
        insetLimit = insetBottom - 32.0f;
        insetLeft += (f32)horizontalExtent;
        insetRight -= (f32)horizontalExtent;
        insetTop += (f32)verticalExtent;
        insetBottom -= (f32)verticalExtent;
        panel->anchor[0] = insetLeft;
        panel->anchor[1] = insetTop;
        panel->corners[3][0] = insetLeft;
        panel->corners[3][1] = insetTop;
        panel->corners[0][0] = insetLeft;
        panel->corners[0][1] = insetBottom;
        panel->corners[1][0] = insetRight;
        panel->corners[1][1] = insetTop;
        panel->corners[2][0] = insetRight;
        panel->corners[2][1] = insetBottom;
        if (insetBottom <= insetLimit) {
            panel->timer = 0;
            work->page = 5;
        }
        break;
    }
    case 5:
        panel->timer++;
        panel->timer = panel->timer <= 0 ? 0 :
            panel->timer >= 10 ? 10 : panel->timer;
        if (panel->timer >= 10) {
            work->page = 6;
            panel->timer = 180;
        }
        break;
    case 6:
    case 7:
    {
        f32 fraction;
        f32 outsetLeft, outsetRight, outsetTop, outsetBottom;
        fraction = command->exponentialRange.end;
        if (fraction >= 1.0f) {
            fraction = 1.0f;
        }
        horizontalExtent = (s32)(fraction * 66.0f);
        verticalExtent = (s32)(fraction * 58.0f);
        outsetLeft = center[0] - 200.0f - (f32)horizontalExtent;
        outsetRight = center[0] + 200.0f + (f32)horizontalExtent;
        outsetTop = center[1] - 174.0f - (f32)verticalExtent;
        outsetBottom = center[1] + 174.0f + (f32)verticalExtent;
        panel->anchor[0] = outsetLeft;
        panel->anchor[1] = outsetTop;
        panel->corners[3][0] = outsetLeft;
        panel->corners[3][1] = outsetTop;
        panel->corners[0][0] = outsetLeft;
        panel->corners[0][1] = outsetBottom;
        panel->corners[1][0] = outsetRight;
        panel->corners[1][1] = outsetTop;
        panel->corners[2][0] = outsetRight;
        panel->corners[2][1] = outsetBottom;
        if (work->page == 6) {
            if (outsetBottom >= 448.0f) {
                panel->timer = 0;
                work->page = 7;
            }
        } else {
            panel->edgeAlpha -= 32;
            panel->edgeAlpha = panel->edgeAlpha <= 0 ? 0 :
                panel->edgeAlpha >= 128 ? 128 : panel->edgeAlpha;
            if (panel->edgeAlpha <= 0) {
                work->page = 8;
            }
        }
        break;
    }
    }
}

extern void evtSubmitGradientTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitViewParamPacket(f32, f32, f32, f32, f32, f32, u32, u32);

/* Draw the analysis silhouette with a mirrored five-level color ramp.
 * Continues Deep Quasar, Iron Nebula, PiM, Cedar Spark and Luna. */
void func_001AF5D0(BtlLinkedCommand *unused, BtlAnalysisPanelWork *work) {
    u32 depths[5];
    /* Inferred bounds keep the independent literal-copy alias sets distinct. */
    s16 deltas[] = {0, 3, 3, 4, 4, 5, 5, 8};
    s16 levels0[] = {0, 0, 1, 1, 0, 1, 1, 1, 2, 2, 1, 2, 2, 2, 3, 3, 2, 3, 3, 3, 4, 4, 3, 4};
    s16 levels1[] = {0, 1, 0, 0, 1, 1, 1, 2, 1, 1, 2, 2, 2, 3, 2, 2, 3, 3, 3, 4, 3, 3, 4, 4};
    s16 levels2[] = {1, 0, 0, 1, 0, 1, 2, 1, 1, 2, 1, 2, 3, 2, 2, 3, 2, 3, 4, 3, 3, 4, 3, 4};
    s16 levels3[] = {0, 1, 1, 0, 1, 0, 1, 2, 2, 1, 2, 1, 2, 3, 3, 2, 3, 2, 3, 4, 4, 3, 4, 3};
    u32 *level = depths;
    s32 i;

    depths[4] = depths[0] = work->edgeAlpha >> 4;
    depths[3] = depths[1] = work->edgeAlpha >> 1;
    depths[2] = depths[1] + (depths[1] >> 1) + (depths[1] >> 2);
    for (i = 4; i >= 0; i--) {
        *level++ <<= 24;
    }
    depths[0] |= 0x50641C;
    depths[1] |= 0x50944C;
    depths[2] |= 0x60A45C;
    depths[3] |= 0x50944C;
    depths[4] |= 0x50641C;

    if (work->page < 8) {
        if (work->page >= 0) {
            evtSetDrawSurfaceIndex(0x53);
            evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
            evtSubmitViewParamPacket(246.0f, 224.0f, 16711680.0f,
                266.0f, 224.0f, 16711680.0f, depths[1], depths[1]);
            evtSubmitViewParamPacket(256.0f, 216.0f, 16711680.0f,
                256.0f, 232.0f, 16711680.0f, depths[1], depths[1]);

            for (i = 0; i < 4; i++) {
                if (work->anchor[1] < work->corners[0][1]) {
                    evtSubmitGradientTriangle(
                        work->anchor[0] + deltas[2 * i], work->anchor[1] + deltas[2 * i],
                        work->corners[0][0] + deltas[2 * i], work->corners[0][1] - deltas[2 * i],
                        work->anchor[0] + deltas[2 * i + 1], work->anchor[1] + deltas[2 * i + 1],
                        depths[levels0[6 * i]], depths[levels0[6 * i + 1]], depths[levels0[6 * i + 2]]);
                    evtSubmitGradientTriangle(
                        work->anchor[0] + deltas[2 * i + 1], work->anchor[1] + deltas[2 * i + 1],
                        work->corners[0][0] + deltas[2 * i], work->corners[0][1] - deltas[2 * i],
                        work->corners[0][0] + deltas[2 * i + 1], work->corners[0][1] - deltas[2 * i + 1],
                        depths[levels0[6 * i + 3]], depths[levels0[6 * i + 4]], depths[levels0[6 * i + 5]]);
                }
            }

            for (i = 0; i < 4; i++) {
                if (work->corners[3][0] < work->corners[1][0]) {
                    evtSubmitGradientTriangle(
                        work->corners[3][0] + deltas[2 * i], work->corners[3][1] + deltas[2 * i],
                        work->corners[3][0] + deltas[2 * i + 1], work->corners[3][1] + deltas[2 * i + 1],
                        work->corners[1][0] - deltas[2 * i], work->corners[1][1] + deltas[2 * i],
                        depths[levels1[6 * i]], depths[levels1[6 * i + 1]], depths[levels1[6 * i + 2]]);
                    evtSubmitGradientTriangle(
                        work->corners[1][0] - deltas[2 * i], work->corners[1][1] + deltas[2 * i],
                        work->corners[3][0] + deltas[2 * i + 1], work->corners[3][1] + deltas[2 * i + 1],
                        work->corners[1][0] - deltas[2 * i + 1], work->corners[1][1] + deltas[2 * i + 1],
                        depths[levels1[6 * i + 3]], depths[levels1[6 * i + 4]], depths[levels1[6 * i + 5]]);
                }
            }

            for (i = 0; i < 4; i++) {
                if (work->corners[0][0] < work->corners[2][0]) {
                    evtSubmitGradientTriangle(
                        work->corners[0][0] + deltas[2 * i + 1], work->corners[0][1] - deltas[2 * i + 1],
                        work->corners[0][0] + deltas[2 * i], work->corners[0][1] - deltas[2 * i],
                        work->corners[2][0] - deltas[2 * i], work->corners[2][1] - deltas[2 * i],
                        depths[levels2[6 * i]], depths[levels2[6 * i + 1]], depths[levels2[6 * i + 2]]);
                    evtSubmitGradientTriangle(
                        work->corners[0][0] + deltas[2 * i + 1], work->corners[0][1] - deltas[2 * i + 1],
                        work->corners[2][0] - deltas[2 * i], work->corners[2][1] - deltas[2 * i],
                        work->corners[2][0] - deltas[2 * i + 1], work->corners[2][1] - deltas[2 * i + 1],
                        depths[levels2[6 * i + 3]], depths[levels2[6 * i + 4]], depths[levels2[6 * i + 5]]);
                }
            }

            for (i = 0; i < 4; i++) {
                if (work->corners[1][1] < work->corners[2][1]) {
                    evtSubmitGradientTriangle(
                        work->corners[1][0] - deltas[2 * i], work->corners[1][1] + deltas[2 * i],
                        work->corners[1][0] - deltas[2 * i + 1], work->corners[1][1] + deltas[2 * i + 1],
                        work->corners[2][0] - deltas[2 * i + 1], work->corners[2][1] - deltas[2 * i + 1],
                        depths[levels3[6 * i]], depths[levels3[6 * i + 1]], depths[levels3[6 * i + 2]]);
                    evtSubmitGradientTriangle(
                        work->corners[1][0] - deltas[2 * i], work->corners[1][1] + deltas[2 * i],
                        work->corners[2][0] - deltas[2 * i + 1], work->corners[2][1] - deltas[2 * i + 1],
                        work->corners[2][0] - deltas[2 * i], work->corners[2][1] - deltas[2 * i],
                        depths[levels3[6 * i + 3]], depths[levels3[6 * i + 4]], depths[levels3[6 * i + 5]]);
                }
            }
        }
    }
}

extern const BattlePanelColors D_003A25C0;

/* Draw the analysis panel's two-piece HP/MP bar and restore each cropped slot. */
void func_001AFF78(BtlAnalysisPanelWork *work, s32 stat, s32 x, s32 y, s32 alpha) {
    s32 frames[][2][3] = {
        {{119, 30, 0x30}, {170, 30, 0x31}},
        {{119, 54, 0x36}, {170, 54, 0x37}}
    };
    s32 bars[][2][3] = {
        {{119, 30, 0x2E}, {170, 30, 0x2F}},
        {{119, 54, 0x34}, {170, 54, 0x35}}
    };
    /* Retail retains this default copy before all four corner overrides. */
    BattlePanelColors colors = D_003A25C0;
    s32 current;
    s32 maximum;
    s32 count;
    s32 i;
    s32 j;
    f32 ratio;
    f32 divisor;
    f32 span;
    f32 secondRatio;
    f32 pixelWidth;
    f32 spriteWidth;

    if (stat == 0) {
        current = ((BtlUnit *)work->entry)->partyRecord.hp;
        maximum = ((BtlUnit *)work->entry)->partyRecord.maxHp;
        divisor = (f32)maximum;
        ratio = (f32)current;
    } else {
        current = ((BtlUnit *)work->entry)->partyRecord.mp;
        maximum = ((BtlUnit *)work->entry)->partyRecord.maxMp;
        divisor = (f32)maximum;
        ratio = (f32)current;
    }
    ratio /= divisor;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 4; j++) {
            colors.values[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA,
                frames[stat][i][2], j, alpha);
        }
        effDrawSpriteWithCornerColors((x + frames[stat][i][0]) << 4,
            (y + frames[stat][i][1]) << 3, 0, colors.values, 0,
            btlResourceBlock->resA, frames[stat][i][2], 0x53);
    }
    if (ratio < 0.5f) {
        count = 1;
    } else {
        count = 2;
    }
    for (i = 0; i < count; i++) {
        for (j = 0; j < 4; j++) {
            colors.values[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA,
                bars[stat][i][2], j, alpha);
        }
        if (i == 0) {
            if (ratio < 0.5f) {
                span = (f32)(btlResourceBlock->resA->descriptions[bars[stat][i][2]].colors[2] -
                    btlResourceBlock->resA->descriptions[bars[stat][i][2]].colors[0]);
                btlResourceBlock->resA->workEntries[bars[stat][i][2]].parameters[2] =
                    (s32)(span * ratio * 2.0f - span);
                spriteWidth = (f32)btlResourceBlock->resA->workEntries[bars[stat][i][2]].sourceWidth;
                pixelWidth = spriteWidth * ratio * 2.0f;
                btlResourceBlock->resA->workEntries[bars[stat][i][2]].geometry.bounds[2] =
                    (s32)(pixelWidth * 16.0f);
            }
        } else {
            if (ratio < 1.0f) {
                secondRatio = ratio - 0.5f;
                span = (f32)(btlResourceBlock->resA->descriptions[bars[stat][i][2]].colors[2] -
                    btlResourceBlock->resA->descriptions[bars[stat][i][2]].colors[0]);
                btlResourceBlock->resA->workEntries[bars[stat][i][2]].parameters[0] =
                    (s32)-(span * secondRatio * 2.0f - span);
                spriteWidth = (f32)btlResourceBlock->resA->workEntries[bars[stat][i][2]].sourceWidth;
                pixelWidth = spriteWidth * secondRatio * 2.0f;
                btlResourceBlock->resA->workEntries[bars[stat][i][2]].geometry.bounds[2] =
                    (s32)(pixelWidth * 16.0f);
            }
        }
        effDrawSpriteWithCornerColors((x + bars[stat][i][0]) << 4,
            (y + bars[stat][i][1]) << 3, 0, colors.values, 0,
            btlResourceBlock->resA, bars[stat][i][2], 0x53);
        btlResourceBlock->resA->workEntries[bars[stat][i][2]].geometry.bounds[2] =
            btlResourceBlock->resA->workEntries[bars[stat][i][2]].sourceWidth << 4;
        btlResourceBlock->resA->workEntries[bars[stat][i][2]].parameters[0] = 0;
        btlResourceBlock->resA->workEntries[bars[stat][i][2]].parameters[2] = 0;
    }
}

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
        effDrawSpriteWithCornerColors((x + strip->x) << 4, (y + strip->y) << 3,
                     0, colors, 0, btlResourceBlock->resA, strip->texture, 0x53);
    }
}



/* Draw the fixed panel composition at x/y using the requested corner opacity. */
void btlDrawCompositePanelSprites(void *unused, s32 x, s32 y, u32 alpha) {
    s32 strips[37][3] = {
        { 0, -16, 0x5E },
        { 186, -16, 0x5F },
        { 0, -23, 0x64 },
        { 186, -23, 0x65 },
        { 0, 68, 0x66 },
        { 186, 68, 0x67 },
        { 0, 82, 0x60 },
        { 222, 82, 0x61 },
        { 0, 75, 0x68 },
        { 222, 75, 0x69 },
        { 0, 177, 0x6A },
        { 222, 177, 0x6B },
        { 0, 191, 0x62 },
        { 222, 191, 0x63 },
        { 0, 184, 0x6C },
        { 222, 184, 0x6D },
        { 0, 277, 0x6E },
        { 222, 277, 0x6F },
        { 20, -22, 0x3A },
        { 27, -22, 0x3B },
        { 292, -22, 0x3C },
        { 86, 82, 0x3D },
        { 93, 82, 0x3E },
        { 249, 82, 0x3F },
        { 77, 194, 0x58 },
        { 84, 194, 0x59 },
        { 249, 194, 0x5A },
        { 20, 276, 0x5B },
        { 27, 276, 0x5C },
        { 302, 276, 0x5D },
        { 37, -1, 0x2B },
        { 101, 19, 0x2C },
        { 169, 18, 0x2D },
        { 101, 43, 0x32 },
        { 169, 42, 0x33 },
        { 42, 76, 0x38 },
        { 42, 186, 0x39 },
    };
    u32 colors[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 i, j;
    s32 xOffset, yOffset;

    for (i = 0; i < 37; i++) {
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA,
                                               strips[i][2], j, alpha);
            xOffset = strips[i][0];
            yOffset = strips[i][1];
            switch (strips[i][2]) {
            case 0x3A:
            case 0x3D:
            case 0x58:
            case 0x5B:
                /* Retail 0x001B079C retains the identical corner 1/3 alpha-clear arms. */
                if (j == 1 || j == 3) {
                    colors[j] &= ~0xFF;
                } else {
                    colors[j] &= ~0xFF;
                }
                xOffset += (alpha >> 4) - 8;
                break;
            case 0x3C:
            case 0x3F:
            case 0x5A:
            case 0x5D:
                if (j == 0 || j == 2) {
                    colors[j] = (colors[j] & ~0xFF) | alpha;
                } else {
                    colors[j] &= ~0xFF;
                }
                xOffset += (alpha >> 4) - 8;
                btlResourceBlock->resA->workEntries[strips[i][2]].geometry.bounds[2] =
                    (btlResourceBlock->resA->workEntries[strips[i][2]].sourceWidth << 4) + 0x140;
                break;
            case 0x3B:
            case 0x3E:
            case 0x59:
            case 0x5C:
                colors[j] = (colors[j] & ~0xFF) | alpha;
                xOffset += (alpha >> 4) - 8;
                break;
            }
        }
        effDrawSpriteWithCornerColors((x + xOffset) << 4, (y + yOffset) << 3,
                     0, colors, 0, btlResourceBlock->resA, strips[i][2], 0x53);
        btlResourceBlock->resA->workEntries[strips[i][2]].geometry.bounds[2] =
            btlResourceBlock->resA->workEntries[strips[i][2]].sourceWidth << 4;
    }
}

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

/* Fade the guide panel in and out, show its single message window once fully faded in,
 * and draw the dimmed full-screen backdrop with its framed band; -1 after fade-out. */
s32 btlUpdateGuidePanelTask(KwlnTask *task) {
    BtlGuidePanelWork *work = (BtlGuidePanelWork *)dds3GetProcessData(task);
    s32 width;
    u32 alpha;

    switch (work->state) {
    case 0:
        work->fadeLevel += 12;
        work->fadeLevel = work->fadeLevel <= 0 ? 0 : work->fadeLevel > 0x80 ? 0x80 : work->fadeLevel;
        if (work->fadeLevel >= 0x80) {
            work->state = 1;
        }
        break;
    case 1:
        break;
    case 2:
    default:
        work->fadeLevel -= 12;
        work->fadeLevel = work->fadeLevel <= 0 ? 0 : work->fadeLevel > 0x80 ? 0x80 : work->fadeLevel;
        if (work->fadeLevel <= 0) {
            return -1;
        }
        break;
    }
    switch (work->state) {
    case 0:
    case 1:
    case 2:
        if (work->state == 1) {
            width = itfMesMeasureEntryItem(work->windowIndex, work->entryIndex, 0);
            itfMesBlk24MoveTo(work->windowIndex, (256 - (width >> 5)) << 4, (work->y + 0x10) << 3);
            itfMesStartEntry(work->windowIndex, work->entryIndex, 0);
        }
        alpha = ((work->fadeLevel >> 1) + 16) << 24;
        work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
            alpha | 0x080808;
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 128, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(0);
        evtSubmitDefaultDepthGradientRect(0, 0, 0x200, 0x1C0,
            work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
        evtSubmitPrimaryAlphaBlendMode(1);
        work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
            alpha | 0xA0A0A0;
        evtSubmitDefaultDepthGradientRect(0, work->y - 2, 0x200, 2,
            work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
        work->colors.values[0] = alpha | 0x181818;
        work->colors.values[1] = alpha | 0x101010;
        work->colors.values[2] = alpha | 0x141414;
        work->colors.values[3] = alpha | 0x0C0C0C;
        evtSubmitDefaultDepthGradientRect(0, work->y, 0x200, 0xF0,
            work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
        work->colors.values[3] = work->colors.values[2] = work->colors.values[1] = work->colors.values[0] =
            alpha | 0xA0A0A0;
        evtSubmitDefaultDepthGradientRect(0, work->y + 0xF0, 0x200, 2,
            work->colors.values[0], work->colors.values[1], work->colors.values[2], work->colors.values[3]);
        evtSubmitPrimaryAlphaBlendMode(0);
        break;
    }
    return 0;
}

void btlReleaseMessageWindowTask(KwlnTask *task) {
    BtlGuidePanelWork *work = (BtlGuidePanelWork *)dds3GetProcessData(task);
    itfMesCleanupWindow(work->windowIndex, 0);
    sdfChipFreeDirect(work);
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

/* Store the confirmed command-selection tuple on the owning actor. */
void btlSetActorCommandSelection(s32 object, s16 first, s16 second, s16 slot) {
    BtlUnit *unit = ((BattleSceneObject *)object)->owner->unit;

    unit->partyRecord.savedCommandPhase = first;
    unit->partyRecord.unk18E = second;
    unit->partyRecord.actionSlot = slot;
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

extern s32 btlUpdateRegisteredPanelTask(KwlnTask *);

extern void btlReleaseRegisteredChildTaskWork(KwlnTask *);

void btlStartRegisteredChildTask(void) {
    s32 context = (s32)(u32)btlGetRuntime();
    u32 data = (u32)sdfAllocAndClearQuadwords(sizeof(BattleRegisteredPanelWork));
    u32 task = (u32)dds3StartProcess(D_003BB3C0, 0x2B0E, 1, 1, btlUpdateRegisteredPanelTask, btlReleaseRegisteredChildTaskWork, data);
    dds3LinkProcess((KwlnTask *)*(u32 *)(context + 0x29C), (KwlnTask *)task);
    btlSetTrackedTaskHandle(7, task);
}

/* Reset panel entrance mode and its initial horizontal offset. */
void btlResetRegisteredPanelEntrance(BattleRegisteredPanelWork *work) {
    work->mode = 1;
    work->x = 0x80;
    work->y = 0;
    work->state = 0;
}

extern const u32 D_003A2870[4];

/* Advance the registered panel slide/fade animation and draw its layered sprites. */
s32 btlUpdateRegisteredPanelTask(KwlnTask *task) {
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
    work = (BattleRegisteredPanelWork *)dds3GetProcessData(task);
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
            effDrawSpriteWithCornerColors(0x19C0, -0x50, 0, colors, 0, btlResourceBlock->resC, 0x29, 0x53);
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
            }
            effDrawSpriteWithCornerColors(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x28, 0x53);
            if (sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
                }
                effDrawSpriteWithCornerColors(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x3A, 0x53);
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x3D, i, work->backdropAlpha);
                }
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 90.0f;
                effDrawSpriteWithCornerColors(0x1CB0, 0x70, 0, colors, 0, btlResourceBlock->resC, 0x3D, 0x53);
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 0.0f;
            }
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->spriteAlpha);
            }
            effDrawSpriteWithCornerColors((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                         0, colors, 0, btlResourceBlock->resC, sprite, 0x53);
            if (work->mode == 4 || sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->overlayAlpha);
                }
                effDrawSpriteWithCornerColors((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                             0, colors, 0, btlResourceBlock->resC, 0x3B, 0x53);
            }
        }
    }
    return 0;
}

void btlReleaseRegisteredChildTaskWork(KwlnTask *task) {
    u32 temp_v0;

    temp_v0 = dds3GetProcessData(task);
    sdfChipFreeDirect((void *)temp_v0);
    btlSetTrackedTaskHandle(7, 0);
}

typedef struct BattlePhaseSlotIds { s32 values[3]; } BattlePhaseSlotIds;

typedef struct BattlePhaseFadeLimits { s32 values[3][4]; } BattlePhaseFadeLimits;

typedef struct BattlePhaseXBounds { s32 values[3][2]; } BattlePhaseXBounds;

extern const BattlePhaseSlotIds D_003A2880;

extern const BattlePhaseFadeLimits D_003A2890;

extern const BattlePhaseXBounds D_003A28C0;

/* Advance phase-panel movement, cropping and corner fades in the supplied work. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2870);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2880);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2890);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A28C0);

void btlUpdatePhasePanelAnimation(BattlePhasePanelWork *work) {
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
                work->current[i][0] -= 10;
                work->current[i][0] = work->current[i][0] <= bounds.values[i][0] ? bounds.values[i][0] :
                    work->current[i][0] < bounds.values[i][1] ? work->current[i][0] : bounds.values[i][1];
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
        work->current[0][0] -= 2;
        if (work->current[0][0] <= 300) work->phase++;
        break;
    case 2:
        work->current[0][0] -= 20;
        work->current[1][0] -= 16;
        work->fade[0][0] -= 16;
        work->fade[0][0] = work->fade[0][0] <= 0 ? 0 : work->fade[0][0] > 0xFF ? 0xFF : work->fade[0][0];
        if (work->current[0][0] <= 128) work->phase++;
        break;
    case 3:
        work->current[0][0] -= 18;
        work->current[1][0] -= 18;
        work->current[2][0] -= 18;
        if (work->current[0][0] <= 96) {
            work->phase++;
            work->fade[0][0] = 0;
            work->fade[0][2] = 0;
        }
        break;
    case 4:
        work->current[0][0] -= 20;
        work->current[1][0] -= 20;
        work->current[2][0] -= 20;
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
            work->saved[i][1] += 3;
            work->saved[i][1] = work->saved[i][1] <= 0 ? 0 : work->saved[i][1] < height ? work->saved[i][1] : height;
        }
    }
    work->fade[1][3] = 0;
    work->fade[2][0] = 0;
    work->fade[2][1] = 0;
    work->fade[2][2] = 0;
    work->fade[2][3] = 0;
}

extern void btlScaleEffectSlotSourceBounds(EffectSlotSet *, s32);

extern void func_001B19F8(BattlePhasePanelWork *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A28F8);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B19F8);

extern const s32 D_003BB418[];
extern const s32 D_003BB420[];
extern const s32 D_003A2928[2][2];
extern const s32 D_003A2938[2][2];

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2918);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2928);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2938);

void func_001B1C88(BattlePhasePanelWork *work) {
    s32 modeOneSlots[2];
    s32 modeZeroSlots[2];
    s32 modeOneBounds[2][2];
    s32 modeZeroBounds[2][2];
    s32 *slots;
    s32 (*bounds)[2];
    s32 i, j;

    memcpy(modeOneSlots, D_003BB418, sizeof(modeOneSlots));
    memcpy(modeZeroSlots, D_003BB420, sizeof(modeZeroSlots));
    memcpy(modeOneBounds, D_003A2928, sizeof(modeOneBounds));
    memcpy(modeZeroBounds, D_003A2938, sizeof(modeZeroBounds));
    if (work->mode == 0) {
        bounds = modeZeroBounds;
        slots = modeZeroSlots;
    } else {
        bounds = modeOneBounds;
        slots = modeOneSlots;
    }
    switch (work->secondaryPhase) {
    case 0:
        work->counter++;
        work->counter = work->counter <= 0 ? 0 : work->counter > 10 ? 10 : work->counter;
        if (work->counter >= 3) {
            work->echoCount++;
            work->saved[3][0] = 0;
            work->saved[4][0] = 0;
            for (i = 3; i < 5; i++) {
                work->current[i][0] -= 10;
                work->current[i][0] = work->current[i][0] <= bounds[i - 3][0] ? bounds[i - 3][0] :
                    work->current[i][0] < bounds[i - 3][1] ? work->current[i][0] : bounds[i - 3][1];
                for (j = 0; j < 4; j++) {
                    work->fade[i][j] += 16;
                    work->fade[i][j] = work->fade[i][j] <= 0 ? 0 : work->fade[i][j] > 128 ? 128 : work->fade[i][j];
                }
            }
            if (work->fade[3][0] >= 128) {
                work->secondaryPhase++;
                work->echoCount = 0;
                work->counter = 0;
            }
        }
        break;
    case 1:
        for (i = 5; i < 7; i++) {
            work->flags[i - 5] = 1;
            for (j = 3; j >= 0; j--) work->fade[i][j] = 255;
        }
        work->counter++;
        work->counter = work->counter <= 0 ? 0 : work->counter > 10 ? 10 : work->counter;
        if (work->counter >= 4) work->secondaryPhase++;
        break;
    case 2:
        for (i = 5; i < 7; i++) {
            work->fade[i][2] -= 16;
            work->fade[i][2] = work->fade[i][2] <= 0 ? 0 : work->fade[i][2] > 255 ? 255 : work->fade[i][2];
            work->fade[i][3] -= 16;
            work->fade[i][3] = work->fade[i][3] <= 0 ? 0 : work->fade[i][3] > 255 ? 255 : work->fade[i][3];
            work->fade[i][1] -= 16;
            work->fade[i][1] = work->fade[i][1] <= 0 ? 0 : work->fade[i][1] > 255 ? 255 : work->fade[i][1];
            work->fade[i][0] -= 16;
            work->fade[i][0] = work->fade[i][0] <= 0 ? 0 : work->fade[i][0] > 255 ? 255 : work->fade[i][0];
        }
        if (work->fade[6][3] <= 0) {
            work->flags[1] = 0;
            work->flags[0] = 0;
            work->secondaryPhase++;
        }
        break;
    case 3:
        work->current[3][0] -= 28;
        work->saved[3][0] += 2;
        work->saved[3][0] = work->saved[3][0] <= 0 ? 0 : work->saved[3][0] > 32 ? 32 : work->saved[3][0];
        if (work->current[3][0] <= 384) {
            work->secondaryPhase++;
            work->angle = 0;
        }
        break;
    case 4:
        work->current[3][0] -= 28;
        work->saved[3][0] += 2;
        work->saved[3][0] = work->saved[3][0] <= 0 ? 0 : work->saved[3][0] > 32 ? 32 : work->saved[3][0];
        work->saved[4][0] += 2;
        work->saved[4][0] = work->saved[4][0] <= 0 ? 0 : work->saved[4][0] > 32 ? 32 : work->saved[4][0];
        work->angle = (work->angle + 8) % 360;
        work->current[4][0] = (s32)((sdfSinPoly((f32)((work->angle + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f * 392.0f + 0.0f);
        if (work->current[3][0] <= 256) work->secondaryPhase++;
        break;
    case 5:
        for (i = 3; i < 5; i++) {
            s32 height = btlResourceBlock->resC->workEntries[slots[i - 3]].sourceHeight;
            work->saved[i][1] -= 2;
            work->saved[i][1] = work->saved[i][1] <= -height ? -height :
                work->saved[i][1] > 0 ? 0 : work->saved[i][1];
        }
        work->current[3][0] -= 28;
        work->angle = (work->angle + 8) % 360;
        work->angle = work->angle <= 0 ? 0 : work->angle > 180 ? 180 : work->angle;
        work->current[4][0] = (s32)((sdfSinPoly((f32)((work->angle + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f * 392.0f + 0.0f);
        for (j = 0; j < 4; j++) {
            work->fade[3][j] -= 28;
            work->fade[3][j] = work->fade[3][j] <= 0 ? 0 : work->fade[3][j] > 128 ? 128 : work->fade[3][j];
            work->fade[4][j] -= 20;
            work->fade[4][j] = work->fade[4][j] <= 0 ? 0 : work->fade[4][j] > 128 ? 128 : work->fade[4][j];
        }
        break;
    }
}


/* Draw the phase-change panels and their trailing echoes.
 * The first pair retains four converted source bounds across its echo draws;
 * each draw still reads the current coordinates and texture reference. */
extern const s32 D_003A2968[4];
extern const s32 D_003A2978[4];
extern const s32 D_003A2988[4];
extern s32 btlDrawGouraudTexturedPanelQuad(s32, s32, s32, s32, s32, s32, s32, s32,
                                         s32, s32, s32, s32, const s32 *, SdfTex *);
void func_001B2390(BattlePhasePanelWork *work) {
    f32 bounds[4];
    s32 modeOneSlots[4];
    s32 modeZeroSlots[4];
    s32 colors[4];
    s32 panel;
    s32 channel;
    s32 echo;
    s32 *slots;
    s32 cropX;
    s32 cropY;

    memcpy(modeOneSlots, D_003A2968, sizeof(modeOneSlots));
    memcpy(modeZeroSlots, D_003A2978, sizeof(modeZeroSlots));
    memcpy(colors, D_003A2988, sizeof(colors));
    if (work->secondaryPhase >= 7) {
        return;
    }
    if (work->secondaryPhase < 0) {
        return;
    }
    slots = modeZeroSlots;
    if (work->mode != 0) {
        slots = modeOneSlots;
    }
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(1);
    for (panel = 0; panel < 2; panel++) {
        for (channel = 0; channel < 4; channel++) {
            u32 color = btlSetSlotLowByteClamped(btlResourceBlock->resC, slots[panel],
                                               channel, work->fade[panel + 3][channel]);
            {
                u32 alphaRed = ((color & 0xFF000000) >> 24) | (color << 24);
                u32 greenBlue = ((color & 0xFF00) << 8) | ((color & 0xFF0000) >> 8);
                colors[channel] = alphaRed | greenBlue;
            }
        }
        bounds[0] = btlResourceBlock->resC->descriptions[slots[panel]].colors[0];
        bounds[1] = btlResourceBlock->resC->descriptions[slots[panel]].colors[1];
        bounds[2] = btlResourceBlock->resC->descriptions[slots[panel]].colors[2];
        bounds[3] = btlResourceBlock->resC->descriptions[slots[panel]].colors[3];
        cropX = btlResourceBlock->resC->workEntries[slots[panel]].geometry.bounds[2] >> 4;
        cropY = btlResourceBlock->resC->workEntries[slots[panel]].geometry.bounds[3] >> 3;
        btlDrawGouraudTexturedPanelQuad(
            work->current[panel + 3][0], work->current[panel + 3][1],
            work->current[panel + 3][0] + cropX, work->current[panel + 3][1],
            work->current[panel + 3][0] + work->saved[panel + 3][0],
            work->current[panel + 3][1] + work->saved[panel + 3][1] + cropY,
            work->current[panel + 3][0] + work->saved[panel + 3][0] + cropX,
            work->current[panel + 3][1] + work->saved[panel + 3][1] + cropY,
            bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1],
            colors, btlResourceBlock->resC->textureReferences[
                    btlResourceBlock->resC->descriptions[slots[panel]].textureIndex]);
        if (work->echoCount > 0) {
            for (echo = 0; echo < work->echoCount; echo++) {
                s32 x;
                for (channel = 0; channel < 4; channel++) {
                    u32 color = btlSetSlotLowByteClamped(btlResourceBlock->resC, slots[panel],
                                                       channel, work->echoCount * 6);
                    {
                        u32 alphaRed = ((color & 0xFF000000) >> 24) | (color << 24);
                        u32 greenBlue = ((color & 0xFF00) << 8) | ((color & 0xFF0000) >> 8);
                        colors[channel] = alphaRed | greenBlue;
                    }
                }
                x = work->current[panel + 3][0] + (work->echoCount - echo) * 5;
                btlDrawGouraudTexturedPanelQuad(
                    x, work->current[panel + 3][1],
                    x + cropX, work->current[panel + 3][1],
                    x + work->saved[panel + 3][0],
                    work->current[panel + 3][1] + work->saved[panel + 3][1] + cropY,
                    x + work->saved[panel + 3][0] + cropX,
                    work->current[panel + 3][1] + work->saved[panel + 3][1] + cropY,
                    bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1],
                    colors, btlResourceBlock->resC->textureReferences[
                            btlResourceBlock->resC->descriptions[slots[panel]].textureIndex]);
            }
        }
    }
    for (panel = 2; panel < 4; panel++) {
        for (channel = 0; channel < 4; channel++) {
            u32 fade;
            u32 red;
            u32 green;
            u32 blue;
            u32 rgb;
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, slots[panel],
                                                       channel, work->fade[panel + 3][channel]);
            fade = work->fade[panel + 3][channel];
            red = (fade << 24) | 0x80000000;
            green = (fade << 16) | 0x00800000;
            blue = (fade << 8) | 0x00008000;
            rgb = (red | green) | blue;
            colors[channel] |= rgb;
        }
        if (work->flags[panel - 2] != 0) {
            bounds[0] = btlResourceBlock->resC->descriptions[slots[panel]].colors[0];
            bounds[1] = btlResourceBlock->resC->descriptions[slots[panel]].colors[1];
            bounds[2] = btlResourceBlock->resC->descriptions[slots[panel]].colors[2];
            bounds[3] = btlResourceBlock->resC->descriptions[slots[panel]].colors[3];
            cropX = btlResourceBlock->resC->workEntries[slots[panel]].geometry.bounds[2] >> 4;
            cropY = btlResourceBlock->resC->workEntries[slots[panel]].geometry.bounds[3] >> 3;
            btlDrawGouraudTexturedPanelQuad(
                work->current[panel + 1][0], work->current[panel + 1][1],
                work->current[panel + 1][0] + cropX, work->current[panel + 1][1],
                work->current[panel + 1][0] + work->saved[panel + 1][0],
                work->current[panel + 1][1] + work->saved[panel + 1][1] + cropY,
                work->current[panel + 1][0] + work->saved[panel + 1][0] + cropX,
                work->current[panel + 1][1] + work->saved[panel + 1][1] + cropY,
                bounds[0], bounds[1], bounds[2] - bounds[0], bounds[3] - bounds[1],
                colors, btlResourceBlock->resC->textureReferences[
                        btlResourceBlock->resC->descriptions[slots[panel]].textureIndex]);
        }
    }
}

/* The quad renderer owns this complete four-slot mode-zero bank. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2968);

const s32 D_003A2978[4] = {0x21, 0x22, 0x2C, 0x2D};


s32 btlUpdatePhaseGatedTaskUntilTimeout(KwlnTask *task) {
    BattlePhasePanelWork *state = (BattlePhasePanelWork *)dds3GetProcessData(task);
    s32 phase = btlGetNamedTaskPairStatusOrUnavailable();
    if ((u8)(phase - 1) < 2) {
        return 0;
    }
    btlUpdatePhasePanelAnimation(state);
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

void btlReleasePsechgPanelWork(KwlnTask *task) {
    BattlePhasePanelWork *work;

    work = (BattlePhasePanelWork *)dds3GetProcessData(task);
    sdfChipFreeDirect(work);
    btlSetTrackedTaskHandle(6, 0);
}

extern u16 *btlGatherActorCommandsByKind(s32, s16 *, u16, u16, u16);
extern s32 D_003A2998[2][4];

/* Command-option layout: two rows of position, extent, size and 12.4 fixed-point size. */
typedef struct BattleCommandLayoutWork {
    s32 state;
    s32 positions[2][4];
    s32 extents[2][2];
    s16 selectedPositions[4];
    s32 dimensions[2][4];
    s32 fixedDimensions[2][4];
    u8 pad7C[8];
} BattleCommandLayoutWork;

extern s32 fileTestSavedSlotFlags(u32);

extern BattleRosterTable *btlGetActorLinkageSkillList(BattleSceneObject *, u16 *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2988);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2998);

void func_001B2AC8(BattleSceneObject *object) {
    enum {
        LAYOUT_ROW = (s32)&((BattleCommandLayoutWork *)0)->fixedDimensions[0][3],
        LAYOUT_POSITION = (s32)&((BattleCommandLayoutWork *)0)->positions[0] - LAYOUT_ROW,
        LAYOUT_WIDTH = (s32)&((BattleCommandLayoutWork *)0)->dimensions[0][0] - LAYOUT_ROW,
        LAYOUT_HEIGHT = (s32)&((BattleCommandLayoutWork *)0)->dimensions[0][1] - LAYOUT_ROW,
        LAYOUT_INNER_WIDTH = (s32)&((BattleCommandLayoutWork *)0)->dimensions[0][2] - LAYOUT_ROW,
        LAYOUT_INNER_HEIGHT = (s32)&((BattleCommandLayoutWork *)0)->dimensions[0][3] - LAYOUT_ROW,
        LAYOUT_FIXED_WIDTH = (s32)&((BattleCommandLayoutWork *)0)->fixedDimensions[0][0] - LAYOUT_ROW,
        LAYOUT_FIXED_HEIGHT = (s32)&((BattleCommandLayoutWork *)0)->fixedDimensions[0][1] - LAYOUT_ROW,
        LAYOUT_FIXED_INNER_WIDTH = (s32)&((BattleCommandLayoutWork *)0)->fixedDimensions[0][2] - LAYOUT_ROW,
        LAYOUT_FIXED_INNER_HEIGHT = 0
    };
    s32 initialRows[2][4];
    s16 count;
    BattleCommandLayoutWork *work;
    BdWork *slot;
    s32 row;
    u8 *cursor;
    s32 extent;

    memcpy(initialRows, D_003A2998, sizeof(initialRows));
    D_003BD834 = sdfAllocAndClearQuadwords(0x38);
    work = sdfAllocAndClearQuadwords(sizeof(BattleCommandLayoutWork));
    extent = 180;
    btlCommandOptionAnimationWork = (u32)work;
    slot = &btlResourceBlock->resA->workEntries[0x10];
    /* One row pointer walks the three parallel row arrays, as retail does: indexed
     * named fields give each array its own induction register (their field offsets
     * differ modulo the 16-byte offset alignment, so loop cannot combine them). */
    cursor = (u8 *)work + LAYOUT_ROW;
    for (row = 0; row < 2; ++row, ++slot, cursor += sizeof(work->dimensions[0])) {
        ((s32 *)(cursor + LAYOUT_POSITION))[0] = initialRows[row][0];
        ((s32 *)(cursor + LAYOUT_POSITION))[1] = initialRows[row][1];
        ((s32 *)(cursor + LAYOUT_POSITION))[2] = initialRows[row][2];
        ((s32 *)(cursor + LAYOUT_POSITION))[3] = initialRows[row][3];
        work->extents[row][0] = extent;
        work->extents[row][1] = extent;
        *(s32 *)(cursor + LAYOUT_WIDTH) = slot->sourceWidth;
        *(s32 *)(cursor + LAYOUT_FIXED_WIDTH) = *(s32 *)(cursor + LAYOUT_WIDTH) << 4;
        *(s32 *)(cursor + LAYOUT_HEIGHT) = slot->sourceHeight;
        *(s32 *)(cursor + LAYOUT_FIXED_HEIGHT) = *(s32 *)(cursor + LAYOUT_HEIGHT) << 3;
        *(s32 *)(cursor + LAYOUT_INNER_WIDTH) = slot->sourceWidth;
        *(s32 *)(cursor + LAYOUT_FIXED_INNER_WIDTH) = *(s32 *)(cursor + LAYOUT_INNER_WIDTH) << 4;
        *(s32 *)(cursor + LAYOUT_INNER_HEIGHT) = slot->sourceHeight;
        *(s32 *)(cursor + LAYOUT_FIXED_INNER_HEIGHT) = *(s32 *)(cursor + LAYOUT_INNER_HEIGHT) << 3;
    }
    if (fileTestSavedSlotFlags(1) == 0) {
        return;
    }
    switch ((s16)object->owner->unit->partyRecord.savedCommandPhase) {
    case 0:
    case 1:
        object->selections[0].entry = 0;
        object->selections[0].cursor = 0;
        break;
    case 2:
        object->selections[0].entry = object->owner->unit->partyRecord.unk18E;
        btlGatherActorCommandsByKind((s32)object, &count, 0, 2, 3);
        if (count - 1 < object->selections[0].entry) {
            object->selections[0].entry = 0;
            object->selections[0].cursor = 0;
        } else if (object->selections[0].entry >= 3) {
            object->selections[0].cursor = object->selections[0].entry - 2;
        }
        break;
    case 3:
        object->selections[4].entry = object->owner->unit->partyRecord.unk18E;
        btlGetActorLinkageSkillList(object, (u16 *)&count);
        if (count - 1 < object->selections[4].entry) {
            object->selections[4].entry = 0;
            object->selections[4].cursor = 0;
        } else if (count == object->selections[4].entry) {
            if (object->selections[4].entry >= 3) {
                object->selections[4].cursor = object->selections[4].entry - 3;
            }
        } else if (object->selections[4].entry >= 3) {
            object->selections[4].cursor = object->selections[4].entry - 2;
        }
        break;
    case 10:
        object->selections[0].entry = object->owner->unit->partyRecord.unk18E;
        btlGatherActorCommandsByKind((s32)object, &count, 0, 2, 3);
        object->selections[0].entry = (u16)count;
        if ((u16)count >= 3) {
            object->selections[0].cursor = (u16)count - 3;
        } else {
            object->selections[0].cursor = 0;
        }
        break;
    }
}

void btlReleaseBattleScratchBlocks(void) {
    sdfChipFreeDirect(D_003BD834);
    sdfChipFreeDirect((void *)btlCommandOptionAnimationWork);
}

extern const BattlePanelColors D_003A29F0;
extern u32 btlGetCommandOptionCount(s32, s8, s8);
extern void btlResetActorSlotPresentationValue(BtlUnit *, BattleSceneObject *);
extern void btlCreateSelectionTrailTask(s32, s32, s32);
extern void func_001B3300(BattleSceneObject *, s32);
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void evtSubmitGradientTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A29F0);

void func_001B2D80(BattleSceneObject *object, s32 kind) {
    /* Retail copies the rodata colors at 001B2DBC..001B2DD8. */
    BattlePanelColors colors = D_003A29F0;
    s32 row = 0;
    s32 entry;
    s32 cursor;
    s32 state;
    s32 y;
    s32 optionCount;
    u32 opacity;
    u32 color;
    u32 shade;

    switch (object->state) {
    case 1:
    case 7:
    case 9:
        D_003BD834->alpha += 16;
        D_003BD834->alpha = D_003BD834->alpha <= 0 ? 0 : D_003BD834->alpha <= 128 ? D_003BD834->alpha : 128;
        entry = object->selections[kind].entry;
        cursor = object->selections[kind].cursor;
        row = entry - cursor;
        D_003BD834->phase = 0;
        D_003BD834->entry = entry;
        D_003BD834->cursor = cursor;
        D_003BD834->row = row;
        break;
    case 10:
        D_003BD834->alpha += 16;
        D_003BD834->alpha = D_003BD834->alpha <= 0 ? 0 : D_003BD834->alpha <= 128 ? D_003BD834->alpha : 128;
        break;
    case 2:
    case 3:
        D_003BD834->phase = (D_003BD834->phase + 16) % 360;
        D_003BD834->alpha = (s32)((sdfSinPoly((f32)((D_003BD834->phase + 90) % 360) / 180.0f * 3.14159f) + 1.0f) * 0.5f * 64.0f + 64.0f);
        entry = object->selections[kind].entry;
        cursor = object->selections[kind].cursor;
        row = entry - cursor;
        state = object->state;
        if (state == 2 && (optionCount = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 1)) >= 2) {
            if ((D_00324510.prev & 2) || (D_00324510.next & 2)) {
                /* Retail compares cached entry with the relative row at 001B2FE4. */
                if (D_003BD834->cursor != cursor || D_003BD834->entry != row || D_003BD834->row != row) {
                    D_003BD834->alpha = 128;
                    D_003BD834->phase = 0;
                    if (D_003BD834->entry != entry && D_003BD834->row != row) {
                        btlCreateSelectionTrailTask(entry, D_003BD834->row, row);
                    }
                }
                if (kind == 4) {
                    btlSetActionActorPanelPresentation(object->owner, 1, 0);
                    if (D_00359160.entries[entry].first != NULL) {
                        btlResetActorSlotPresentationValue(D_00359160.entries[entry].first, object);
                    }
                    if (D_00359160.entries[entry].second != NULL) {
                        btlResetActorSlotPresentationValue(D_00359160.entries[entry].second, object);
                    }
                }
            }
        } else if (state != 2 || object->state != state) {
            D_003BD834->alpha = 128;
            D_003BD834->phase = 0;
        }
        D_003BD834->entry = entry;
        D_003BD834->cursor = cursor;
        D_003BD834->row = row;
        break;
    case 11:
        D_003BD834->alpha -= btlTrackedTaskHandles->status.bytes.fadeStep;
        D_003BD834->alpha = D_003BD834->alpha <= 0 ? 0 : D_003BD834->alpha <= 128 ? D_003BD834->alpha : 128;
        entry = object->selections[kind].entry;
        cursor = object->selections[kind].cursor;
        row = entry - cursor;
        break;
    case 6:
    case 8:
        D_003BD834->alpha -= 32;
        D_003BD834->alpha = D_003BD834->alpha <= 0 ? 0 : D_003BD834->alpha <= 128 ? D_003BD834->alpha : 128;
        entry = object->selections[kind].entry;
        cursor = object->selections[kind].cursor;
        row = entry - cursor;
        break;
    }
    func_001B3300(object, kind);
    color = ((u8)D_003BD834->alpha) | 0x80808000;
    colors.values[0] = color;
    colors.values[1] = color;
    colors.values[2] = color;
    colors.values[3] = color;
    y = row * 23;
    effDrawSpriteWithCornerColors(0x90, (y + 0x14F) << 3, 0, colors.values, 0, btlResourceBlock->resA, 12, 0x53);
    opacity = (u8)D_003BD834->alpha;
    shade = (opacity >> 2) << 24;
    evtSetDrawSurfaceIndex(0x53);
    evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(1);
    evtSubmitGradientTriangle(0x1F, y + 0x14C, 0xB9, y + 0x14C, 0x11, y + 0x15A,
        shade | 0xFF2400U, shade | 0xA00200U, shade | 0xFF2200U);
    evtSubmitGradientTriangle(0x11, y + 0x15A, 0xB9, y + 0x14C, 0x15, y + 0x15E,
        shade | 0xFF2200U, shade | 0xA00200U, shade | 0xFF2000U);
    evtSubmitGradientTriangle(0x15, y + 0x15E, 0xB9, y + 0x14C, 0xBD, y + 0x150,
        shade | 0xFF2000U, shade | 0xA00200U, shade | 0xC00400U);
    evtSubmitGradientTriangle(0x15, y + 0x15E, 0xBD, y + 0x150, 0xAF, y + 0x15E,
        shade | 0xFF2000U, shade | 0xC00400U, shade | 0xE00800U);
    evtSubmitPrimaryAlphaBlendMode(0);
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A30);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B3300);


extern const char *D_003BB3B8;
extern s32 func_001B3FB0(KwlnTask *);
extern void btlReleaseCmsleffPanelWork(KwlnTask *);

/* Replace tracked slot five with a staggered trail from previousRow to targetRow. */
void btlCreateSelectionTrailTask(s32 unused, s32 previousRow, s32 targetRow) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *task = (KwlnTask *)btlGetTrackedTaskHandle(5);
    BattleSelectionTrailWork *work;
    s32 distance;

    if (task != NULL) {
        dds3KillProcess(task, 0);
    }
    work = sdfAllocAndClearQuadwords(sizeof(*work));
    if (previousRow < targetRow) {
        distance = (targetRow - previousRow) * 23;
        work->direction = 1;
        work->targetRow = targetRow;
    } else {
        distance = (previousRow - targetRow) * 23;
        work->targetRow = targetRow;
    }
    if (work->direction == 1) {
        work->positions[3] = previousRow * 23;
        work->positions[2] = work->positions[3] + distance / 4;
        work->positions[1] = work->positions[3] + distance / 2;
        work->positions[0] = work->positions[3] + (distance * 3) / 4;
    } else {
        work->positions[3] = previousRow * 23;
        work->positions[2] = work->positions[3] - distance / 4;
        work->positions[1] = work->positions[3] - distance / 2;
        work->positions[0] = work->positions[3] - (distance * 3) / 4;
    }
    work->fadeCountdowns[0] = 32;
    work->fadeCountdowns[1] = 24;
    work->fadeCountdowns[2] = 16;
    work->fadeCountdowns[3] = 8;
    task = dds3StartProcess(D_003BB3B8, 0x2B0E, 1, 1, func_001B3FB0,
                          btlReleaseCmsleffPanelWork, (u32)work);
    dds3LinkProcess(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(5, (s32)task);
}

extern const BattlePanelColors D_003A2A70;
s32 func_001B3FB0(KwlnTask *task) {
    BattlePanelColors colors = D_003A2A70;
    BattleSelectionTrailWork *work = (BattleSelectionTrailWork *)dds3GetProcessData(task);
    s32 i;
    s32 opacity;
    u32 shade;

    if (work->fadeCountdowns[0] <= 0 || D_00324510.confirm < 0 ||
        D_00324510.primaryCancel < 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (work->fadeCountdowns[i] > 0) {
            work->fadeCountdowns[i]--;
        }
        work->fadeCountdowns[i] = work->fadeCountdowns[i] <= 0 ? 0 :
            work->fadeCountdowns[i] > 0x80 ? 0x80 : work->fadeCountdowns[i];
        if (work->direction == 1) {
            work->positions[i]++;
            if (work->positions[i] >= work->targetRow * 23) {
                work->positions[i] = work->targetRow * 23;
            }
        } else {
            work->positions[i]--;
            if (work->positions[i] <= work->targetRow * 23) {
                work->positions[i] = work->targetRow * 23;
            }
        }
        colors.values[0] = 0x80808000U | (work->fadeCountdowns[i] & 0xFF);
        colors.values[1] = 0x80808000U | (work->fadeCountdowns[i] & 0xFF);
        colors.values[2] = 0x80808000U | (work->fadeCountdowns[i] & 0xFF);
        colors.values[3] = 0x80808000U | (work->fadeCountdowns[i] & 0xFF);
        effDrawSpriteWithCornerColors(0x90, (work->positions[i] + 0x14F) << 3, 0,
            colors.values, 0, btlResourceBlock->resA, 0xC, 0x53);
        opacity = work->fadeCountdowns[i] & 0xFF;
        shade = (opacity >> 1) << 24;
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        evtSubmitGradientTriangle(0x1F, work->positions[i] + 0x14C,
            0xB9, work->positions[i] + 0x14C, 0x11, work->positions[i] + 0x15A,
            shade | 0xFF2400U, shade | 0xA00200U, shade | 0xFF2200U);
        evtSubmitGradientTriangle(0x11, work->positions[i] + 0x15A,
            0xB9, work->positions[i] + 0x14C, 0x15, work->positions[i] + 0x15E,
            shade | 0xFF2200U, shade | 0xA00200U, shade | 0xFF2000U);
        evtSubmitGradientTriangle(0x15, work->positions[i] + 0x15E,
            0xB9, work->positions[i] + 0x14C, 0xBD, work->positions[i] + 0x150,
            shade | 0xFF2000U, shade | 0xA00200U, shade | 0xC00400U);
        evtSubmitGradientTriangle(0x15, work->positions[i] + 0x15E,
            0xBD, work->positions[i] + 0x150, 0xAF, work->positions[i] + 0x15E,
            shade | 0xFF2000U, shade | 0xC00400U, shade | 0xE00800U);
        evtSubmitPrimaryAlphaBlendMode(0);
    }

    return 0;
}

void btlReleaseCmsleffPanelWork(KwlnTask *task) {
    u32 temp_v0;

    temp_v0 = dds3GetProcessData(task);
    sdfChipFreeDirect((void *)temp_v0);
    btlSetTrackedTaskHandle(5, 0);
}

extern const BattlePanelColors D_003A2A80;

extern void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *);

struct FrFontGlyph;

extern s32 frFontDrawGlyphWithSharedFlags(struct FrFontGlyph *, s8);

s32 btlUpdateSkillNamePanelTask(KwlnTask *task) {
    BattlePanelColors colors = D_003A2A80;
    BtlPanelTransitionWork *work;
    s16 *fade;
    s32 i;
    s32 width;
    struct FrFontGlyph *glyph;

    if (btlGetTrackedTaskHandle(1) == 0) {
        return 0;
    }
    work = (BtlPanelTransitionWork *)dds3GetProcessData(task);
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
    btlSubmitSharedTextGlyph(glyph);
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
    effDrawSpriteWithCornerColors(work->initial[1].x << 4, (work->initial[1].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    colors.values[0] = work->fadeLevels[1] | 0x80808000;
    colors.values[1] = work->fadeLevels[0] | 0x80808000;
    colors.values[2] = work->fadeLevels[3] | 0x80808000;
    colors.values[3] = work->fadeLevels[2] | 0x80808000;
    effDrawSpriteWithCornerColors(work->initial[0].x << 4, (work->initial[0].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    colors.values[0] = work->fadeLevels[2] | 0x80808000;
    colors.values[1] = work->fadeLevels[3] | 0x80808000;
    colors.values[2] = work->fadeLevels[1] | 0x80808000;
    colors.values[3] = work->fadeLevels[0] | 0x80808000;
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    effDrawSpriteWithCornerColors((0x100 - width / 2) << 4, (work->initial[1].y + work->verticalShift) << 3,
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
    sdfChipFreeDirect((void *)dds3GetProcessData(task));
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
    data = (MsgQueueTaskData *)dds3GetProcessData(task);
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
    effDrawSpriteWithCornerColors(((width >> 4) - half + 0x105) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = (width >> 4) << 4;
    effDrawSpriteWithCornerColors((0x100 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    effDrawSpriteWithCornerColors((0x92 - half) << 4, 0x200, 0,
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
    entry = (s32 *)dds3GetProcessData(task);
    itfMesCleanupWindow(entry[1], 0);
    sdfChipFreeDirect(entry);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    BtlCommandHintWork *window;
    btlGetRuntime();
    window = sdfAllocAndClearQuadwords(0x40);
    D_003BB3DC = window;
    window->unk10 = 0x14;
    window->colors[0] = 0x1800080;
    window->colors[1] = 0x40800080;
    window->colors[2] = 0x40800080;
    window->colors[3] = 0x60808080;
    window->x = 0xBB;
    window->y = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B4D58);

extern const BattlePanelColors D_003A2AF0;
extern s32 fldGetSceneObjectState(void);
extern s32 btlGetActorCommandClass(BattleSceneObject *,s8);
extern u32 btlGetCommandOptionCount(s32,s8,s8);
extern u16 *btlGatherActorCommandsByKind(s32,s16 *,u16,u16,u16);
extern void btlBuildEligibleActorList(s32,s16 *);
extern BattleRosterTable *btlGetActorLinkageSkillList(BattleSceneObject *,u16 *);
extern void btlReleaseRegisteredTaskBuffer(KwlnTask *);
extern void func_001B4D58(s32,s32,u32,u32,u16,s16);
extern u8 D_00358FE0[];
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A70);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2A90);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AA0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AB0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AC0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AD0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AE0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2AF0);

s32 func_001B4F10(KwlnTask *task) {
    BattlePanelColors colors = D_003A2AF0;
    BattleSceneObject *object = NULL;
    KwlnTask *panelTask;
    BtlState *battle;
    s16 count;
    s16 optionCount;
    s16 cursor;
    s16 row;
    u32 state;
    s32 kind;
    s32 selector;
    s32 draw;
    s32 i;

    if (btlGetTrackedTaskHandle(4) == 0) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    if ((battle->battleFlags & 0x200) == 0) {
        return 0;
    }

    panelTask = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    if (panelTask != NULL) {
        object = (BattleSceneObject *)dds3GetProcessData(panelTask);
    }
    draw = 0;
    state = fldGetSceneObjectState();
    switch (state) {
    case 1:
    case 2:
    case 3:
    case 10:

    {
        BtlCommandHintWork *fadeWindow;
    fadeWindow = (BtlCommandHintWork *)D_003BB3DC;
    fadeWindow->fade += 0x20;
    fadeWindow->fade = fadeWindow->fade <= 0 ? 0 :
                   fadeWindow->fade > 0x80 ? 0x80 : fadeWindow->fade;

    }
    if (mdlFlagTest(0xC0E) == 0 && panelTask != NULL) {
    kind = btlGetActorCommandClass(object, btlCommandPanelWork->classIndex);
    optionCount = (s16)btlGetCommandOptionCount((s32)object,
                                         btlCommandPanelWork->classIndex, 1);
    row = object->selections[kind].entry;
    cursor = object->selections[kind].cursor;

    switch (kind) {
    case 0:
        if ((cursor == 0 && row == 0) || optionCount - 1 == row) {
            BtlCommandHintWork *hintWindow;
            if (row == 0) {
                BtlTask *owner = object->owner;
                selector = (owner->unit->status.flags & 0x1000) != 0 ? 1 : 2;
            } else {
                selector = 3;
            }
            hintWindow = (BtlCommandHintWork *)D_003BB3DC;
            func_001B4D58(hintWindow->x + 0x13, hintWindow->y - 6, 0xFF0000,
                          (s16)hintWindow->fade | 0x80808000, 0, selector);
        } else {
            BtlCommandHintWork *entryWindow;
            u16 textId;
            u16 *entries = btlGatherActorCommandsByKind((s32)object, &count, 0, 2, 3);
            textId = entries[row - 1];
            entryWindow = (BtlCommandHintWork *)D_003BB3DC;
            func_001B4D58(entryWindow->x + 0x13, entryWindow->y - 6, 0xFF0000,
                          (s16)entryWindow->fade | 0x80808000, textId, 0);
        }
        draw = 1;
        break;
    case 4: {
        BtlCommandHintWork *skillWindow;
        u16 textId;
        BattleRosterTable *entries = btlGetActorLinkageSkillList(object, (u16 *)&count);
        textId=entries->entries[row].skill;
        skillWindow = (BtlCommandHintWork *)D_003BB3DC;
        draw = 1;
        func_001B4D58(skillWindow->x + 0x13, skillWindow->y - 6, 0xFF0000,
                      (s16)skillWindow->fade | 0x80808000, textId, 0);
        break;
    }
    case 2: {
        BtlCommandHintWork *drawWindow;
        u16 textId;
        btlBuildEligibleActorList((s32)object,&count);
        textId=D_00358FE0[row*2];
        drawWindow=D_003BB3DC;
        draw=1;
        func_001B4D58(drawWindow->x+0x13,drawWindow->y-6,0xFF0000,(s16)drawWindow->fade|0x80808000,textId,9);
        break;
    }
    case 3: {
        BtlCommandHintWork *drawWindow;
        s32 textKind;
        if(row==0) {
            textKind=(object->owner->unit->status.flags&0x1000)!=0?4:5;
        } else {
            textKind=row==1?6:7;
        }
        drawWindow=D_003BB3DC;
        draw=1;
        func_001B4D58(drawWindow->x+0x13,drawWindow->y-6,0xFF0000,(s16)drawWindow->fade|0x80808000,0,textKind);
        break;
    }
    case 6: {
        BtlCommandHintWork *drawWindow = (BtlCommandHintWork *)D_003BB3DC;
        draw = 1;
        func_001B4D58(drawWindow->x + 0x13, drawWindow->y - 6, 0xFF0000,
                      (s16)drawWindow->fade | 0x80808000, 0, 8);
        break;
    }
    }

    if (draw != 0) {
    for (i = 0; i < 4; i++) {
        colors.values[i] = btlSetSlotLowByteClamped(
            btlResourceBlock->resA, 0x2A, i, (s16)D_003BB3DC->fade);
    }
    {
    BtlCommandHintWork *spriteWindow = (BtlCommandHintWork *)D_003BB3DC;
    effDrawSpriteWithCornerColors(spriteWindow->x << 4, spriteWindow->y << 3, 0, colors.values, 0,
                  btlResourceBlock->resA, 0x2A, 0x53);
    }
    }
    }
    break;
    case 5:
        btlReleaseRegisteredTaskBuffer(task);
        return -1;
    }
    return 0;
}

void btlReleaseRegisteredTaskBuffer(KwlnTask *unused) {
    btlGetRuntime();
    sdfChipFreeDirect(D_003BB3DC);
    D_003BB3DC = 0;
    btlSetTrackedTaskHandle(4, 0);
}

/* The notice callback permits its LINKAGE tutorial while all three flag words are clear. */
s32 btlIsLinkageTutorialEligible(void) {
    u32 *slot = datGameState->battleFlags;
    s32 i;
    for (i = 0; i < 3; i++) {
        if (slot[i] != 0) {
            return 0;
        }
    }
    return 1;
}


/* Clear working notice flags, then test for an unmarked eligible LINKAGE skill. */
u8 btlHasUnmarkedLinkageSkill(BtlUnit *unit) {
    s32 count;
    s32 i;

    btlClearPendingLinkageSkillFlags();
    count = btlBuildActorLinkageSkillList(unit, &D_00359160);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (btlAccessRecordedLinkageSkillFlag(D_00359160.entries[i].skill, 2) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return 0;
}

/* The byte predicate result keeps retail's JAL + epilogue at 001B54AC. */
s32 btlGetTaskState6(void) {
    KwlnTask *task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    BattleSceneObject *scene;
    s32 count;

    if (task == NULL) {
        return 0;
    }
    scene = (BattleSceneObject *)dds3GetProcessData(task);
    count = btlHasUnmarkedLinkageSkill(scene->owner->unit);
    return count;
}


extern BattleRosterTable *btlGetActorLinkageSkillList(BattleSceneObject *, u16 *);

/* Record every listed linkage skill; return whether the command-panel task exists. */
s32 btlRecordListedLinkageSkills(void) {
    u16 count;
    KwlnTask *task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    BattleRosterTable *table;
    s32 i;
    if (task != 0) {
        table = btlGetActorLinkageSkillList((BattleSceneObject *)dds3GetProcessData(task), &count);
        for (i = 0; i < count; i++) {
            btlAccessRecordedLinkageSkillFlag(table->entries[i].skill, 0);
        }
        return 1;
    }
    return 0;
}

extern const char *D_003BB3D0;

extern const char *D_003BB3D4;

s32 btlDestroyTaskC(void) {
    KwlnTask *task = dds3SearchProcessName(D_003BB3D0);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0xC), 0);
        }
        return 1;
    }
    return 0;
}

/* The shared dialog creator clears 24 bytes; native counter/alpha/cursor are +4/+8/+14. */
typedef struct BtlTutorialDialogWork {
    s8 phase;
    u8 pad01[3];
    s32 counter;
    u16 alpha;
    u8 pad0A[0xA];
    s32 itemIndex;
} BtlTutorialDialogWork;
typedef char BtlTutorialDialogWorkSize[(sizeof(BtlTutorialDialogWork) == 0x18) ? 1 : -1];

extern const BattlePanelColors D_003A2B20;
extern u8 *D_003BAA8C;

/* Advance new-LINKAGE notices; return -1 after closing and updating notice flags. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2B20);

s32 btlUpdateLinkageNoticeTask(KwlnTask *task) {
    BattlePanelColors colors = D_003A2B20;
    BtlTutorialDialogWork *work = (BtlTutorialDialogWork *)dds3GetProcessData(task);
    BattleRosterTable *list;
    u16 count;
    u16 scanIndex;
    u16 id;

    switch (work->phase) {
    case 0:
        work->alpha += 0x20;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 0x80 ? 0x80 : work->alpha;
        if (work->alpha >= 0x80) {
            work->counter = 0;
            work->phase = 1;
        }
        break;
    case 1:
    case 3:
    case 6:
        work->counter++;
        work->counter = work->counter <= 0 ? 0 : work->counter > 0x40 ? 0x40 : work->counter;
        if (work->counter >= 0x30) {
            work->counter = 0;
        }
        break;
    case 2: {
        KwlnTask *panelTask = dds3SearchProcessName(btlCommandPanelTaskNameRef);
        if (panelTask == NULL) {
            work->phase = 6;
            break;
        }
        list = btlGetActorLinkageSkillList((BattleSceneObject *)dds3GetProcessData(panelTask), &count);
        scanIndex = (u16)work->itemIndex;
        while (scanIndex < count) {
            id = list->entries[work->itemIndex].skill;
            if (btlAccessRecordedLinkageSkillFlag(id, 2) == 0 && btlAccessPendingLinkageSkillFlag(id, 2) == 0) {
                btlAccessPendingLinkageSkillFlag(id, 0);
                evtCopyEntryStringToActiveWindow(0, D_003BAA8C + id * 17);
                dspStartEntry(0);
                work->itemIndex++;
                sndSetStationedSeVolume(7);
                break;
            }
            work->itemIndex++;
            scanIndex++;
        }
        if (scanIndex >= count) {
            work->phase = 4;
        } else {
            work->phase++;
        }
        break;
    }
    case 5:
        if (btlIsLinkageTutorialEligible()) {
            dspStartEntry(1);
            work->phase++;
        }
        break;
    case 4:
    default:
        work->alpha -= 0x20;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 0x80 ? 0x80 : work->alpha;
        break;
    }

    if (work->phase < 7) {
        if (work->phase >= 0) {
            u32 color = work->alpha | 0x80808000;
            colors.values[0] = color;
            colors.values[1] = color;
            colors.values[2] = color;
            colors.values[3] = color;
            if (evtGetMessageWindowControlState() == 1) {
                dspRequestMessageWindowModeOne();
                goto return_zero;
            }
            if (work->phase == 3) {
                work->phase--;
            } else {
                work->phase++;
            }
            goto return_zero;
        }
    }
    dspCloseChannel();
    btlRecordListedLinkageSkills();
    btlSetCommandPanelFadeMode(1, 8);
    return -1;
    /* Retail branches the three arms to one `move v0,zero` after the -1 return (0x2EC/0x308/0x314 -> 0x348); `return 0` per arm lets jump2 cross-jump the phase `sb` stores into a label fall-in (cc1probe --jump, find_cross_jump minimum=1). */
return_zero:
    return 0;
}

void btlFinishTrackedBattleTaskAndCloseWindow(KwlnTask *task) {
    s32 context = (s32)(u32)btlGetRuntime();
    sdfChipFreeDirect((void *)dds3GetProcessData(task));
    btlSetTrackedTaskHandle(12, 0);
    *(u32 *)(context + 0x1F4) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

extern const BattlePanelColors D_003A2B50;

/* Advance the command-panel notice dialog: fade in, hold, look up the panel, fade out;
 * return -1 after closing the channel once the phase leaves 0-6. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2B50);

s32 btlUpdateCommandNoticeTask(KwlnTask *task) {
    BattlePanelColors colors = D_003A2B50;
    BtlTutorialDialogWork *work = (BtlTutorialDialogWork *)dds3GetProcessData(task);

    switch (work->phase) {
    case 0:
        work->alpha += 0x20;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 0x80 ? 0x80 : work->alpha;
        if (work->alpha >= 0x80) {
            work->counter = 0;
            work->phase = 1;
        }
        break;
    case 1:
    case 3:
    case 6:
        work->counter++;
        work->counter = work->counter <= 0 ? 0 : work->counter > 0x40 ? 0x40 : work->counter;
        if (work->counter >= 0x30) {
            work->counter = 0;
        }
        break;
    case 2:
        /* Trimmed from btlUpdateLinkageNoticeTask: both arms now set phase 6, but
         * retail keeps the panel lookup's NULL test until the arms are merged. */
        if (dds3SearchProcessName(btlCommandPanelTaskNameRef) == NULL) {
            work->phase = 6;
            break;
        }
        work->phase = 6;
        break;
    case 5:
        break;
    case 4:
    default:
        work->alpha -= 0x20;
        work->alpha = work->alpha <= 0 ? 0 : work->alpha > 0x80 ? 0x80 : work->alpha;
        break;
    }

    if (work->phase < 7) {
        if (work->phase >= 0) {
            u32 color = work->alpha | 0x80808000;
            colors.values[0] = color;
            colors.values[1] = color;
            colors.values[2] = color;
            colors.values[3] = color;
            if (evtGetMessageWindowControlState() != 1) {
                work->phase++;
                /* Retail jump: 0x001B5B14 `b` to the return-0 block 0x001B5B50 placed after the -1 arm.
                 * Plain return 0 puts that block next to the mode-one arm (7 of 122 words differ);
                 * `phase < 7 && phase >= 0` folds to one sltu (cc1probe --nohook asm). */
                goto return_zero;
            }
            dspRequestMessageWindowModeOne();
            return 0;
        }
    }
    dspCloseChannel();
    btlSetCommandPanelFadeMode(1, 8);
    return -1;
return_zero:
    return 0;
}

s32 btlDestroyTaskD(void) {
    KwlnTask *task = dds3SearchProcessName(D_003BB3D4);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0xD), 0);
        }
        return 1;
    }
    return 0;
}

void btlReleaseWindowTask(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    sdfChipFreeDirect((void *)dds3GetProcessData(task));
    btlSetTrackedTaskHandle(13, 0);
    battle->battleFlags |= 0x100000;
    btlTrackedTaskHandles->status.bytes.blocked = 0;
    evtFinishMessageWindowAndNotify();
}

extern u8 btlSoundSlotDefaults[];

void btlInitializeSelectionWork(void) {
    u8 initial[0x20];
    BattleSelectionWork *allocated;
    u32 *source;
    s16 *state;
    s32 i;
    memcpy(initial, btlSoundSlotDefaults, sizeof(initial));
    allocated = sdfAllocAndClearQuadwords(0x30);
    btlLinkedSelectionTaskBuffer = allocated;
    state = allocated->rowFade;
    source = (u32 *)initial;
    for (i = 3; i >= 0; i--) {
        /* Remaining count 3..0 selects the four rows in forward order. */
        s32 *destination = &allocated->positions[3 - i][1];

        *state = 0;
        state++;
        destination[-1] = source[0];
        destination[0] = source[1];
        source += 2;
    }
    btlSetTrackedTaskHandle(2, 1);
}

#define SELECTION_VIEW ((SelView *)btlLinkedSelectionTaskBuffer)

INCLUDE_RODATA(const s32, "game/code_001A9780", btlSoundSlotDefaults);

s32 func_001B5CD8(void) {
    s32 i;

    if (!(((BtlState *)btlGetRuntime())->battleFlags & 0x200)) {
        return 0;
    }
    switch (fldGetSceneObjectState()) {
    case 1:
    case 2:
    case 3:
    case 10:
        for (i = 0; i < 4; i++) {
            btlLinkedSelectionTaskBuffer->positions[i][0] += 0x12;
            btlLinkedSelectionTaskBuffer->positions[i][0] = btlLinkedSelectionTaskBuffer->positions[i][0] <= -200 ? -200 :
                btlLinkedSelectionTaskBuffer->positions[i][0] > 11 ? 11 : btlLinkedSelectionTaskBuffer->positions[i][0];
            if (btlLinkedSelectionTaskBuffer->positions[i][0] >= 11) {
                btlLinkedSelectionTaskBuffer->rowFade[i] += 8;
            } else {
                btlLinkedSelectionTaskBuffer->rowFade[i] += 4;
            }
            btlLinkedSelectionTaskBuffer->rowFade[i] = btlLinkedSelectionTaskBuffer->rowFade[i] <= 0 ? 0 :
                btlLinkedSelectionTaskBuffer->rowFade[i] > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->rowFade[i];
        }
        btlLinkedSelectionTaskBuffer->backdropFade += 0x18;
        btlLinkedSelectionTaskBuffer->backdropFade = btlLinkedSelectionTaskBuffer->backdropFade <= 0 ? 0 :
            btlLinkedSelectionTaskBuffer->backdropFade > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->backdropFade;
        break;
    case 11:
        for (i = 0; i < 4; i++) {
            btlLinkedSelectionTaskBuffer->rowFade[i] -= btlTrackedTaskHandles->status.bytes.fadeStep;
            btlLinkedSelectionTaskBuffer->rowFade[i] = btlLinkedSelectionTaskBuffer->rowFade[i] <= 0 ? 0 :
                btlLinkedSelectionTaskBuffer->rowFade[i] > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->rowFade[i];
        }
        btlLinkedSelectionTaskBuffer->backdropFade -= btlTrackedTaskHandles->status.bytes.fadeStep;
        btlLinkedSelectionTaskBuffer->backdropFade = btlLinkedSelectionTaskBuffer->backdropFade <= 0 ? 0 :
            btlLinkedSelectionTaskBuffer->backdropFade > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->backdropFade;
        break;
    case 6:
    case 8:
        for (i = 0; i < 4; i++) {
            if (i != 1) {
                btlLinkedSelectionTaskBuffer->positions[i][0] -= i;
                btlLinkedSelectionTaskBuffer->rowFade[i] -= 0x18;
                btlLinkedSelectionTaskBuffer->positions[i][0] = btlLinkedSelectionTaskBuffer->positions[i][0] <= -24 ? -24 :
                    btlLinkedSelectionTaskBuffer->positions[i][0] > 11 ? 11 : btlLinkedSelectionTaskBuffer->positions[i][0];
                btlLinkedSelectionTaskBuffer->rowFade[i] = btlLinkedSelectionTaskBuffer->rowFade[i] <= 0 ? 0 :
                    btlLinkedSelectionTaskBuffer->rowFade[i] > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->rowFade[i];
            } else {
                btlLinkedSelectionTaskBuffer->positions[i][1] -= 6;
                btlLinkedSelectionTaskBuffer->positions[i][1] = btlLinkedSelectionTaskBuffer->positions[i][1] <= btlLinkedSelectionTaskBuffer->positions[0][1] ? btlLinkedSelectionTaskBuffer->positions[0][1] :
                    btlLinkedSelectionTaskBuffer->positions[i][1] >= btlLinkedSelectionTaskBuffer->positions[0][1] + 23 ? btlLinkedSelectionTaskBuffer->positions[0][1] + 23 : btlLinkedSelectionTaskBuffer->positions[i][1];
            }
        }
        btlLinkedSelectionTaskBuffer->backdropFade -= 0x18;
        btlLinkedSelectionTaskBuffer->backdropFade = btlLinkedSelectionTaskBuffer->backdropFade <= 0 ? 0 :
            btlLinkedSelectionTaskBuffer->backdropFade > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->backdropFade;
        break;
    case 7:
    case 9:
        for (i = 0; i < 4; i++) {
            if (i != 1) {
                btlLinkedSelectionTaskBuffer->positions[i][0] += (6 - i) * 2;
                btlLinkedSelectionTaskBuffer->positions[i][0] = btlLinkedSelectionTaskBuffer->positions[i][0] <= -24 ? -24 :
                    btlLinkedSelectionTaskBuffer->positions[i][0] > 11 ? 11 : btlLinkedSelectionTaskBuffer->positions[i][0];
                if (btlLinkedSelectionTaskBuffer->positions[i][0] >= 11) {
                    btlLinkedSelectionTaskBuffer->rowFade[i] += 0x10;
                } else {
                    btlLinkedSelectionTaskBuffer->rowFade[i] += 4;
                }
                btlLinkedSelectionTaskBuffer->rowFade[i] = btlLinkedSelectionTaskBuffer->rowFade[i] <= 0 ? 0 :
                    btlLinkedSelectionTaskBuffer->rowFade[i] > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->rowFade[i];
            } else {
                btlLinkedSelectionTaskBuffer->positions[i][1] += 6;
                btlLinkedSelectionTaskBuffer->positions[i][1] = btlLinkedSelectionTaskBuffer->positions[i][1] <= btlLinkedSelectionTaskBuffer->positions[0][1] ? btlLinkedSelectionTaskBuffer->positions[0][1] :
                    btlLinkedSelectionTaskBuffer->positions[i][1] >= btlLinkedSelectionTaskBuffer->positions[0][1] + 23 ? btlLinkedSelectionTaskBuffer->positions[0][1] + 23 : btlLinkedSelectionTaskBuffer->positions[i][1];
            }
        }
        btlLinkedSelectionTaskBuffer->backdropFade += 0x18;
        btlLinkedSelectionTaskBuffer->backdropFade = btlLinkedSelectionTaskBuffer->backdropFade <= 0 ? 0 :
            btlLinkedSelectionTaskBuffer->backdropFade > 0x80 ? 0x80 : btlLinkedSelectionTaskBuffer->backdropFade;
        break;
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2BD0);

INCLUDE_ASM(const s32, "game/code_001A9780", func_001B60E8);

void btlReleaseSelectionTaskBuffer(void) {
    if (btlGetTrackedTaskHandle(2) != 0) {
        sdfChipFreeDirect(btlLinkedSelectionTaskBuffer);
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

void btlInitializeStaffActorPanels(void) {
    BattleActorPanelInitialPositions positions = D_003A2C10;
    BtlState *battle;
    struct SdfMemBlock *allocation;
    BattleActorPanelWork *work;
    s32 slot;
    KwlnTask *task;

    battle = (BtlState *)btlGetRuntime();
    allocation = sdfMemAllocDirect(sizeof(*work));
    work = (BattleActorPanelWork *)sdfMemLock(allocation);
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
    task = dds3StartProcess(D_003BB3B0, 0x2B0E, 1, 1, func_001B6CF8,
                          btlReleaseStwrPanelResource, (u32)work);
    dds3LinkProcess(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(3, (s32)task);
}

s32 func_001B6498(s8 mode) {
    KwlnTask *task = dds3SearchProcessName(D_003BB3B0);
    BtlState *battle;
    BattleActorPanelWork *work;
    BtlUnit *actor;
    s32 eligibleCount;
    s32 state;
    u32 slot;
    s32 i;

    if (task == NULL) {
        return 0;
    }
    battle = (BtlState *)btlGetRuntime();
    work = (BattleActorPanelWork *)dds3GetProcessData(task);
    eligibleCount = btlCountEligibleLinkedActors(battle);
    actor = battle->units;

    if (mode == 0) {
        BattleCmdPanel *panel;
        state = mdlFlagTest(0xC0E) != 0 ? 1 : 2;
        panel = btlCommandPanelWork;
        if (panel == NULL || panel->state == 0) {
            state = 1;
        } else if (panel->state == 3) {
            state = 1;
        }
    } else {
        state = (s32)mode + 2;
    }

    /* i is never advanced: the count only gates the walk on any eligible actor. */
    switch (state) {
    case 1:
        for (i = 0; actor != NULL && i < eligibleCount; actor = actor->next) {
            if (btlHasRequiredActorStatusBits(actor)) {
                slot = actor->lookupId;
                work->activeEntries[slot].basePosition[1] += 6;
                work->activeEntries[slot].basePosition[1] = work->activeEntries[slot].basePosition[1] <= 0x143 ? 0x143 :
                    work->activeEntries[slot].basePosition[1] > 0x159 ? 0x159 : work->activeEntries[slot].basePosition[1];
                work->activeEntries[slot].position[1] += 1;
                work->activeEntries[slot].position[1] = work->activeEntries[slot].position[1] <= 0x143 ? 0x143 :
                    work->activeEntries[slot].position[1] > 0x159 ? 0x159 : work->activeEntries[slot].position[1];
            }
        }
        break;
    case 2:
        for (i = 0; actor != NULL && i < eligibleCount; actor = actor->next) {
            if (btlHasRequiredActorStatusBits(actor)) {
                slot = actor->lookupId;
                work->activeEntries[slot].basePosition[1] -= 6;
                work->activeEntries[slot].basePosition[1] = work->activeEntries[slot].basePosition[1] <= 0x143 ? 0x143 :
                    work->activeEntries[slot].basePosition[1] > 0x159 ? 0x159 : work->activeEntries[slot].basePosition[1];
                work->activeEntries[slot].position[1] -= 1;
                work->activeEntries[slot].position[1] = work->activeEntries[slot].position[1] <= 0x143 ? 0x143 :
                    work->activeEntries[slot].position[1] > 0x159 ? 0x159 : work->activeEntries[slot].position[1];
            }
        }
        break;
    case 3:
        for (i = 0; actor != NULL && i < eligibleCount; actor = actor->next) {
            if (btlHasRequiredActorStatusBits(actor)) {
                slot = actor->lookupId;
                work->activeEntries[slot].basePosition[1] = 0x159;
                work->activeEntries[slot].position[1] = 0x159;
            }
        }
        break;
    case 4:
        for (i = 0; actor != NULL && i < eligibleCount; actor = actor->next) {
            if (btlHasRequiredActorStatusBits(actor)) {
                slot = actor->lookupId;
                work->activeEntries[slot].basePosition[1] = 0x143;
                work->activeEntries[slot].position[1] = 0x143;
            }
        }
        break;
    }
    return 1;
}

typedef struct BattleActorPanelPositions {
    BattleSelectionPosition entries[3];
} BattleActorPanelPositions;

extern const BattleActorPanelPositions D_003A2C28;

extern s32 func_001B6498(s8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C10);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2C28);

void btlUpdateActivePanelEntryFades(BtlState *battle, BattleActorPanelWork *work) {
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
    task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
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
        scene = (BattleSceneObject *)dds3GetProcessData(task);
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

extern void btlDrawActorPanelCursor(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlDrawActorPanelPulse(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlDrawActorPanelSecondaryPulse(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlDrawActorPanelTransitionTriangles(void *, BattleActorPanelWork *, s32);
extern void btlTrackActorVitalChanges(BtlUnit *, BattleActorPanelWork *, s32, s8);
extern void btlUpdateActorPanelSecondaryPulse(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlUpdateActorVitalChangeEffects(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlWriteTriangleHighlightPulse(BtlUnit *, BattleActorPanelWork *, s32);
extern void func_001B8CB8(s32, BattleActorPanelWork *, s32);
extern void func_001B9A50(BtlUnit *, BattleActorPanelWork *, s32);
extern void func_001BB440(BtlUnit *, BattleActorPanelWork *, s32);
extern void btlUpdateActorPanelHighlights(BtlUnit *, BattleActorPanelWork *, s32, s8);
extern void btlUpdateActorVitalPulseTrail(BtlUnit *, BattleActorPanelWork *, s16, s32, s8);
extern void btlUpdateAndDrawVitalBarPulse(BtlUnit *, BattleStatPulse *, s32, s32, s16, s32, s32);
extern void func_001AC080(s32, s32, u32, const char *, s32);
extern const char D_003BB400[];
extern s32 sprintf(char *, const char *, ...);

s32 func_001B6CF8(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleActorPanelWork *work;
    BtlUnit *unit;
    char text[32];
    s32 i;
    s32 count;
    s32 slot;
    s16 baseFade;
    s32 highlight;
    s32 emphasis;
    u32 color;

    if ((battle->battleFlags & 0x200) == 0) {
        return 0;
    }
    i = 0;
    work = (BattleActorPanelWork *)dds3GetProcessData(task);
    count = btlCountEligibleLinkedActors(battle);
    unit = battle->units;
    btlUpdateActivePanelEntryFades(battle, work);
    for (; unit != NULL && i < count; unit = unit->next) {
        if (btlHasRequiredActorStatusBits(unit)) {
            slot = unit->lookupId;
            func_001BB440(unit, work, slot);
            work->activeEntries[slot].position[0] =
                work->activeEntries[slot].basePosition[0] + work->activeEntries[slot].presentation.offset[0];
            work->activeEntries[slot].position[1] =
                work->activeEntries[slot].basePosition[1] + work->activeEntries[slot].presentation.offset[1];
            btlUpdateActorPanelHighlights(unit, work, slot, 0);
            btlDrawActorPanelHighlights(unit, work, slot, 0);
            btlWriteTriangleHighlightPulse(unit, work, slot);
            btlDrawActorPanelTransitionTriangles(unit, work, slot);
            func_001BBE18(unit, work, slot, 0, work->activeEntries[slot].presentation.fade,
                          work->activeEntries[slot].position[0], work->activeEntries[slot].position[1]);
            btlUpdateActorVitalChangeEffects(unit, work, slot);
            btlTrackActorVitalChanges(unit, work, slot, 0);
            btlTrackActorVitalChanges(unit, work, slot, 1);
            if (work->activeEntries[slot].presentation.fade >= 0x80) {
                btlUpdateAndDrawVitalBarPulse(unit, &work->activeEntries[slot].presentation.hpBarPulse,
                                              work->activeEntries[slot].position[0],
                                              work->activeEntries[slot].position[1],
                                              work->activeEntries[slot].presentation.fade, slot, 0);
                btlUpdateActorVitalPulseTrail(unit, work, work->activeEntries[slot].presentation.fade, slot, 0);
                btlUpdateAndDrawVitalBarPulse(unit, &work->activeEntries[slot].presentation.mpBarPulse,
                                              work->activeEntries[slot].position[0],
                                              work->activeEntries[slot].position[1],
                                              work->activeEntries[slot].presentation.fade, slot, 1);
                btlUpdateActorVitalPulseTrail(unit, work, work->activeEntries[slot].presentation.fade, slot, 1);
            }
            func_001B9A50(unit, work, slot);
            btlDrawActorPanelPulse(unit, work, slot);
            btlUpdateActorPanelSecondaryPulse(unit, work, slot);
            btlDrawActorPanelSecondaryPulse(unit, work, slot);
            func_001B8CB8((s32)unit, work, slot);
            btlDrawActorPanelCursor(unit, work, slot);
            btlUpdateShrinkingMirroredSprites((s32)unit, work->activeEntries[slot].presentation.mirroredSprites, 4);
            btlDrawMirroredPanelSprites((s32)unit, work->activeEntries[slot].presentation.mirroredSprites, 4);

            baseFade = work->activeEntries[slot].presentation.fade;
            sprintf(text, D_003BB400, unit->partyRecord.hp);
            if (work->activeEntries[slot].presentation.fade < 0x80) {
                highlight = 0;
            } else {
                highlight = work->activeEntries[slot].presentation.hpHighlightLevel;
            }
            color = btlGetVitalTextColor(unit, unit->partyRecord.hp, unit->partyRecord.maxHp, 0);
            color = ((u32)(baseFade + highlight) << 24) | (color & 0x00FFFFFF);
            if ((u8)work->activeEntries[slot].presentation.hpState - 0x10U < 0x10U) {
                emphasis = work->activeEntries[slot].presentation.hpHighlightLevel + 0x100;
            } else {
                emphasis = 0x100;
            }
            func_001AC080(work->activeEntries[slot].position[0] + 0x50,
                          work->activeEntries[slot].position[1] + 0x26, color, text, emphasis);

            sprintf(text, D_003BB400, unit->partyRecord.mp);
            if (work->activeEntries[slot].presentation.fade < 0x80) {
                highlight = 0;
            } else {
                highlight = work->activeEntries[slot].presentation.mpHighlightLevel;
            }
            i++;
            color = btlGetVitalTextColor(unit, unit->partyRecord.mp, unit->partyRecord.maxMp, 1);
            color = ((u32)(baseFade + highlight) << 24) | (color & 0x00FFFFFF);
            func_001AC080(work->activeEntries[slot].position[0] + 0x48,
                          work->activeEntries[slot].position[1] + 0x3D, color, text, 0x100);
        }
    }
    return 0;
}

void btlReleaseStwrPanelResource(KwlnTask *task) {
    u32 temp_v0;

    temp_v0 = dds3GetProcessData(task);
    sdfMemFreeDirect((struct SdfMemBlock *)(u32)(*(s32 *)temp_v0));
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
                if (base->positions[i][0] < 11) {
                    return 0;
                }
            }
            if (base->positions[1][1] == base->positions[0][1] + 23) {
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
extern s32 btlUpdateAndDrawReserveActorPanels(KwlnTask *);
extern void btlReleaseTrackedTaskResource(void);

/* Build the reserve actor panels: place one row per inactive party member and start the panel task. */
void btlCreateReserveActorPanelTask(void) {
    s32 i;
    BattleReservePositions positions = D_003A2C70;
    BtlState *controller = (BtlState *)btlGetRuntime();
    BattleActorPanelWork *work = (BattleActorPanelWork *)dds3GetProcessData(dds3SearchProcessName(D_003BB3B0));
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
    allocation = sdfMemAllocDirect(0x348);
    memory = (void *)sdfMemLock(allocation);
    memset(memory, 0, 0x348);
    work->reserveUnitAllocation = allocation;
    task = dds3StartProcess(D_003BB3B4, 0x2B0E, 1, 1, btlUpdateAndDrawReserveActorPanels, (TaskDestroy)btlReleaseTrackedTaskResource, (u32)memory);
    dds3LinkProcess(controller->scriptOwner, task);
    btlSetTrackedTaskHandle(8, (s32)task);
    btlSlotBankPromoteStates(work);
    work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
}

extern const BattleReservePositions D_003A2C90;

/* Per-frame update of the reserve actor panels: handle the selection input and slide the rows in or out. */
s32 btlUpdateReservePanelSelection(KwlnTask *task, BtlState *battle) {
    BattleReservePositions positions = D_003A2C90;
    BattleActorPanelWork *work = (BattleActorPanelWork *)dds3GetProcessData(task);
    s32 count = work->reserveCount;
    KwlnTask *commandTask = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    BattleSceneObject *command;
    s32 i;

    if (commandTask != NULL) {
        command = (BattleSceneObject *)dds3GetProcessData(commandTask);
        if (command->state == 6) {
            if (D_00324510.confirm < 0) {
                work->partyRecordIndex = datGameState->partyOrder[work->selectedReserveIndex + work->activeCount];
                sndSetStationedSeVolume(8);
                command->state = 8;
                command->commandData->phase = 9;
            } else if (D_00324510.primaryCancel < 0) {
                btlSlotBankPromoteStates(work);
                command->state = 7;
                sndSetStationedSeVolume(0xA);
                btlSetActionActorPanelPresentation(command->owner, 0, 0);
            } else {
                if (D_00324510.alternateNext & 2) {
                    if (work->selectedReserveIndex != count - 1 || D_00324510.alternateNext < 0) {
                        work->selectedReserveIndex++;
                        if (work->selectedReserveIndex >= count) {
                            work->selectedReserveIndex = 0;
                        }
                        sndSetStationedSeVolume(0);
                        btlSlotBankPromoteStates(work);
                        work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
                    }
                } else if (D_00324510.alternatePrev & 2) {
                    if (work->selectedReserveIndex != 0 || D_00324510.alternatePrev < 0) {
                        work->selectedReserveIndex--;
                        if (work->selectedReserveIndex < 0) {
                            work->selectedReserveIndex = count - 1;
                        }
                        sndSetStationedSeVolume(0);
                        btlSlotBankPromoteStates(work);
                        work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
                    }
                }
            }
        }
        for (i = 0; i < count; i++) {
            switch ((u32)command->state) {
            case 6:
            case 8:
                work->reserveEntries[i].presentation.fade += 0x20;
                work->reserveEntries[i].presentation.fade =
                    work->reserveEntries[i].presentation.fade <= 0 ? 0 :
                    work->reserveEntries[i].presentation.fade > 0x80 ? 0x80 : work->reserveEntries[i].presentation.fade;
                work->reserveEntries[i].position[0]--;
                work->reserveEntries[i].position[0] =
                    work->reserveEntries[i].position[0] <= positions.entries[i][0] ? positions.entries[i][0] :
                    work->reserveEntries[i].position[0] >= positions.entries[i][0] + 8 ? positions.entries[i][0] + 8 :
                    work->reserveEntries[i].position[0];
                break;
            case 7:
            case 9:
                work->reserveEntries[i].presentation.fade -= 0x20;
                work->reserveEntries[i].presentation.fade =
                    work->reserveEntries[i].presentation.fade <= 0 ? 0 :
                    work->reserveEntries[i].presentation.fade > 0x80 ? 0x80 : work->reserveEntries[i].presentation.fade;
                btlSlotBankPromoteStates(work);
                break;
            }
        }
        if (command->state == 7 && work->reserveEntries[0].presentation.fade <= 0) {
            return 0;
        }
        if (command->state == 9 && work->reserveEntries[0].presentation.fade <= 0) {
            return 0;
        }
        if (command->state == 5) {
            return 0;
        }
    }
    return 1;
}

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

void btlAdvanceReservePanelPresentation(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    s32 i;

    switch (work->reserveEntries[slot].presentation.presentationState) {
    case 3:
        work->reserveEntries[slot].presentation.transitionGeometry[0]++;
        work->reserveEntries[slot].presentation.transitionGeometry[1]--;
        work->reserveEntries[slot].presentation.transitionGeometry[2]--;
        work->reserveEntries[slot].presentation.transitionGeometry[3]++;
    case 4:
        for (i = 0; i < 2; i++) {
            if (work->reserveEntries[slot].presentation.transitionFade[i] >= 16) {
                work->reserveEntries[slot].presentation.transitionFade[i] -= 16;
            } else {
                work->reserveEntries[slot].presentation.transitionFade[i] = 0;
            }
        }
        if (work->reserveEntries[slot].presentation.transitionFade[0] < 9) {
            work->reserveEntries[slot].presentation.presentationState = 0;
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            switch (work->reserveEntries[slot].presentation.pulseDirection[i]) {
            case 0:
                work->reserveEntries[slot].presentation.pulseLevel[i] += 10;
                work->reserveEntries[slot].presentation.pulseLevel[i] =
                    work->reserveEntries[slot].presentation.pulseLevel[i] <= 32 ? 32 :
                    work->reserveEntries[slot].presentation.pulseLevel[i] > 192 ? 192 :
                    work->reserveEntries[slot].presentation.pulseLevel[i];
                if (work->reserveEntries[slot].presentation.pulseLevel[i] >= 192) {
                    work->reserveEntries[slot].presentation.pulseDirection[i] = 1;
                    work->reserveEntries[slot].presentation.pulseLevel[i] = 192;
                }
                break;
            case 1:
            default:
                work->reserveEntries[slot].presentation.pulseLevel[i] -= 12;
                work->reserveEntries[slot].presentation.pulseLevel[i] =
                    work->reserveEntries[slot].presentation.pulseLevel[i] <= 32 ? 32 :
                    work->reserveEntries[slot].presentation.pulseLevel[i] > 192 ? 192 :
                    work->reserveEntries[slot].presentation.pulseLevel[i];
                if (work->reserveEntries[slot].presentation.pulseLevel[i] <= 32) {
                    work->reserveEntries[slot].presentation.pulseLevel[i] = 32;
                    work->reserveEntries[slot].presentation.pulseDirection[i] = 0;
                }
                break;
            }
        }
        break;
    case 2:
        work->reserveEntries[slot].presentation.transitionGeometry[0] = 68;
        work->reserveEntries[slot].presentation.transitionGeometry[1] = 38;
        work->reserveEntries[slot].presentation.transitionGeometry[2] = 5;
        work->reserveEntries[slot].presentation.transitionGeometry[3] = 64;
        for (i = 0; i < 2; i++) {
            work->reserveEntries[slot].presentation.transitionFade[i] += 16;
            work->reserveEntries[slot].presentation.transitionFade[i] =
                work->reserveEntries[slot].presentation.transitionFade[i] <= 0 ? 0 :
                work->reserveEntries[slot].presentation.transitionFade[i] >= 128 ? 128 :
                work->reserveEntries[slot].presentation.transitionFade[i];
            work->reserveEntries[slot].presentation.pulseLevel[i] += 12 - i * 2;
            work->reserveEntries[slot].presentation.pulseLevel[i] =
                work->reserveEntries[slot].presentation.pulseLevel[i] <= 0 ? 0 :
                work->reserveEntries[slot].presentation.pulseLevel[i] > 96 ? 96 :
                work->reserveEntries[slot].presentation.pulseLevel[i];
        }
        if (work->reserveEntries[slot].presentation.transitionFade[0] >= 128) {
            work->reserveEntries[slot].presentation.presentationState = 1;
            work->reserveEntries[slot].presentation.pulseDirection[0] = 0;
            work->reserveEntries[slot].presentation.pulseDirection[1] = 0;
            work->reserveEntries[slot].presentation.pulseTimer[0] = 0;
            work->reserveEntries[slot].presentation.pulseTimer[1] = 0;
            work->reserveEntries[slot].presentation.pulseOffsets[0][0] = 4;
            work->reserveEntries[slot].presentation.pulseOffsets[0][1] = 4;
            work->reserveEntries[slot].presentation.pulseOffsets[1][0] = 2;
            work->reserveEntries[slot].presentation.pulseOffsets[1][1] = 2;
        }
        break;
    }
}

extern const BattlePanelColors D_003A2CB0;

/* Draw a reserve row's paired pulse strips and their two offset echo layers. */
void btlDrawReservePanelPulse(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 index) {
    BattlePanelColors colors = D_003A2CB0;
    u32 baseColor;
    s32 level = 0;
    s32 xOffset = 0;
    s32 yOffset = 0;
    s32 layer;
    s32 x;
    s32 y;
    s8 presentationState;
    u32 color;

    baseColor = 0x80808000;
    presentationState = work->reserveEntries[index].presentation.presentationState;
    if (presentationState >= 4) return;
    if (presentationState <= 0) return;
    x = work->reserveEntries[index].position[0];
    y = work->reserveEntries[index].position[1];
    for (layer = 0; layer < 3; layer++) {
        if (layer > 0) {
            level = work->reserveEntries[index].presentation.pulseLevel[layer - 1];
            xOffset = work->reserveEntries[index].presentation.pulseOffsets[layer - 1][0];
            yOffset = work->reserveEntries[index].presentation.pulseOffsets[layer - 1][1];
            level = level <= 0 ? 0 :
                level < work->reserveEntries[index].presentation.transitionFade[0] ?
                level : work->reserveEntries[index].presentation.transitionFade[0];
            color = baseColor | level;
        } else {
            color = baseColor |
                work->reserveEntries[index].presentation.transitionFade[0];
        }
        colors.values[0] = color;
        colors.values[1] = color;
        colors.values[2] = color;
        colors.values[3] = color;
        effDrawSpriteWithCornerColors(
            (x + work->reserveEntries[index].presentation.transitionGeometry[0] + xOffset) << 4,
            (y + work->reserveEntries[index].presentation.transitionGeometry[1] - yOffset) << 3,
            0, colors.values, 0, btlResourceBlock->resA, 9, 0x53);

        if (layer > 0) {
            level = level <= 0 ? 0 :
                level < work->reserveEntries[index].presentation.transitionFade[1] ?
                level : work->reserveEntries[index].presentation.transitionFade[1];
            color = baseColor | level;
        } else {
            color = baseColor |
                work->reserveEntries[index].presentation.transitionFade[1];
        }
        colors.values[0] = color;
        colors.values[1] = color;
        colors.values[2] = color;
        colors.values[3] = color;
        effDrawSpriteWithCornerColors(
            (x + work->reserveEntries[index].presentation.transitionGeometry[2] - xOffset) << 4,
            (y + work->reserveEntries[index].presentation.transitionGeometry[3] + yOffset) << 3,
            0, colors.values, 0, btlResourceBlock->resA, 12, 0x53);
    }
}

extern void btlUpdateActorPanelHighlights(BtlUnit *, BattleActorPanelWork *, s32, s8);
extern void btlUpdateActorVitalPulseTrail(BtlUnit *, BattleActorPanelWork *, s16, s32, s8);
extern void btlUpdateAndDrawVitalBarPulse(BtlUnit *, BattleStatPulse *, s32, s32, s16, s32, s32);
extern s32 sprintf(char *, const char *, ...);
extern void btlDrawReservePanelPulse(BtlUnit *, BattleActorPanelWork *, s32);
extern void func_001AC080(s32, s32, u32, const char *, s32);
extern const BattlePanelColors D_003A2CC0;
extern const char D_003BB400[];

/* Refresh and draw reserve HP/MP panels; return -1 when their selection closes. */
s32 btlUpdateAndDrawReserveActorPanels(KwlnTask *task) {
    BtlUnit *unit;
    KwlnTask *panelTask;
    BattleActorPanelWork *work;
    char text[32];
    BattlePanelColors colors = D_003A2CC0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 reserveCount;
    s32 i;

    if ((battle->battleFlags & 0x200) == 0) {
        return 0;
    }
    unit = (BtlUnit *)dds3GetProcessData(task);
    panelTask = dds3SearchProcessName(D_003BB3B0);
    work = (BattleActorPanelWork *)dds3GetProcessData(panelTask);
    reserveCount = work->reserveCount;
    if (btlUpdateReservePanelSelection(panelTask, battle) == 0) {
        return -1;
    }
    i = 0;
    if (reserveCount > 0) {
        do {
            DatGameState *game = datGameState;
            s32 partyIndex = game->partyOrder[i + work->activeCount];
            s16 baseFade;
            u32 color;

            memcpy(&unit->partyRecord, &game->party[partyIndex],
                   sizeof(unit->partyRecord));
            btlRefreshUnitMaximumHpAndClampCurrentHp(&unit->partyRecord);
            btlRefreshUnitMaximumMpAndClampCurrentMp(&unit->partyRecord);
            btlUpdateActorPanelHighlights(unit, work, i, 1);
            btlDrawActorPanelHighlights(unit, work, i, 1);
            func_001BBE18(unit, work, i, 1, work->reserveEntries[i].presentation.fade,
                          work->reserveEntries[i].position[0], work->reserveEntries[i].position[1]);

            if (work->reserveEntries[i].presentation.fade >= 0x80) {
                btlUpdateAndDrawVitalBarPulse(unit, &work->reserveEntries[i].presentation.hpBarPulse, work->reserveEntries[i].position[0], work->reserveEntries[i].position[1],
                              work->reserveEntries[i].presentation.fade, i, 2);
                btlUpdateActorVitalPulseTrail(unit, work, work->reserveEntries[i].presentation.fade, i, 2);
                btlUpdateAndDrawVitalBarPulse(unit, &work->reserveEntries[i].presentation.mpBarPulse, work->reserveEntries[i].position[0], work->reserveEntries[i].position[1],
                              work->reserveEntries[i].presentation.fade, i, 3);
                btlUpdateActorVitalPulseTrail(unit, work, work->reserveEntries[i].presentation.fade, i, 3);
            }

            btlAdvanceReservePanelPresentation(unit, work, i);
            btlDrawReservePanelPulse(unit, work, i);

            if ((unit->partyRecord.flags & 0x1000) == 0) {
                u32 tint = 0x80808000 | (u32)(s32)work->reserveEntries[i].presentation.fade;
                colors.values[0] = tint;
                colors.values[1] = tint;
                colors.values[2] = tint;
                colors.values[3] = tint;
                effDrawSpriteWithCornerColors((work->reserveEntries[i].position[0] + 0x22) << 4, (work->reserveEntries[i].position[1] + 0x46) << 3,
                              0, colors.values, 0, btlResourceBlock->resA, 0xE, 0x53);
            }

            baseFade = work->reserveEntries[i].presentation.fade;
            sprintf(text, D_003BB400, unit->partyRecord.hp);
            color = btlGetVitalTextColor(unit, unit->partyRecord.hp, unit->partyRecord.maxHp, 0);
            color = ((u32)(baseFade + work->reserveEntries[i].presentation.hpHighlightLevel) << 24) |
                    (color & 0x00FFFFFF);
            func_001AC080(work->reserveEntries[i].position[0] + 0x50,
                          work->reserveEntries[i].position[1] + 0x26, color, text, 0x100);

            sprintf(text, D_003BB400, unit->partyRecord.mp);
            color = btlGetVitalTextColor(unit, unit->partyRecord.mp, unit->partyRecord.maxMp, 1);
            color = ((u32)(baseFade + work->reserveEntries[i].presentation.mpHighlightLevel) << 24) |
                    (color & 0x00FFFFFF);
            func_001AC080(work->reserveEntries[i].position[0] + 0x48,
                          work->reserveEntries[i].position[1] + 0x3D, color, text, 0x100);
            i++;
        } while (i < reserveCount);
    }
    return 0;
}

void btlReleaseTrackedTaskResource(void) {
    KwlnTask *temp_v0;
    u32 temp_v1;

    temp_v0 = dds3SearchProcessName(D_003BB3B0);
    temp_v1 = dds3GetProcessData(temp_v0);
    sdfMemFreeDirect((struct SdfMemBlock *)(u32)(*(s32 *)(temp_v1 + 0x1200)));
    btlSetTrackedTaskHandle(8, 0);
}

extern void btlRetireActorSlotPresentationChannels(BattleActorPanelWork *, s8);

extern void btlUpdateActorSlotStates(u8 *, s8);

extern void btlSyncActorPanelCommandGroups(BtlTask *, BattleActorPanelWork *, s8);

/* Set the action actor's panel state/value and apply mode-selected group updates. */
void btlSetActionActorPanelPresentation(BtlTask *task, s8 mode, s8 value) {
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
        panelTask = dds3SearchProcessName(D_003BB3B0);
        if (panelTask != NULL) {
            panel = (BattleActorPanelWork *)dds3GetProcessData(panelTask);
            btlRetireActorSlotPresentationChannels(panel, mode);
            btlUpdateActorSlotStates((u8 *)panel, 0);
            panel->activeEntries[slot].presentation.presentationState = 2;
            panel->activeEntries[slot].presentation.presentationValue = value;
            if (mode == 0) {
                btlSyncActorPanelCommandGroups(task, panel, 0);
            } else if (mode == 2) {
                btlSyncActorPanelCommandGroups(task, panel, 1);
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
        KwlnTask *task = dds3SearchProcessName(D_003BB3B0);
        if (task != 0) {
            entry = (u8 *)dds3GetProcessData(task);
            if (mode != 2) {
                btlRetireActorSlotPresentationChannels((BattleActorPanelWork *)entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (BtlSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

void btlSetActorSecondaryPresentation(BtlUnit *object, s32 unused, s8 value) {
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
        task = dds3SearchProcessName(D_003BB3B0);
        if (task != NULL) {
            work = (BattleActorPanelWork *)dds3GetProcessData(task);
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
        entry = (u8 *)dds3GetProcessData(dds3SearchProcessName(D_003BB3B0));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

void btlRetireActorSlotPresentationChannels(BattleActorPanelWork *work, s8 mode) {
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

void btlClearFinishedActorSlotChannels(BattleActorPanelWork *work, s8 mode) {
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

/* Map eligible actor rows to command groups, optionally retaining active states. */
void btlSyncActorPanelCommandGroups(BtlTask *unused, BattleActorPanelWork *work, s8 keepActive) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    s32 group;
    u8 slot;

    for (; node != NULL; node = node->next) {
        if (btlHasRequiredActorStatusBits(node) == 0) {
            continue;
        }
        group = 0;
        slot = node->lookupId;
        if (battle->groupPrimary[0] != NULL) {
            while (group < 20 && battle->groupPrimary[group] != NULL) {
                if (battle->groupPrimary[group]->unit->identity == node->identity) {
                    work->activeEntries[slot].presentation.pendingCursorSelection = group;
                    if (keepActive == 0 || work->activeEntries[slot].presentation.pendingSceneState != 1) {
                        work->activeEntries[slot].presentation.pendingSceneState = 3;
                    }
                    break;
                }
                group++;
            }
        }
    }
}

typedef struct BattleCursorIds {
    s16 values[3];
} BattleCursorIds;

typedef struct BattleCursorOffsets {
    s32 values[3][2];
} BattleCursorOffsets;

typedef struct BattleCursorEchoOffsets {
    s32 values[3][2];
} BattleCursorEchoOffsets;

extern const BattleCursorIds D_003BB458[];
extern const BattleCursorOffsets D_003A2CD0;
extern const BattleCursorEchoOffsets D_003A2CE8;

void func_001B8CB8(s32 unused, BattleActorPanelWork *work, s32 slot) {
    BattleCursorIds ids = D_003BB458[0];
    BattleCursorOffsets settle = D_003A2CD0;
    BattleCursorEchoOffsets echo = D_003A2CE8;
    s32 settled;

    switch (work->activeEntries[slot].presentation.pendingSceneState) {
    case 1:
        break;
    case 5:
        if (work->activeEntries[slot].presentation.unkF0 < 0x20) {
            work->activeEntries[slot].presentation.unkF0 = 0;
            return;
        }
        work->activeEntries[slot].presentation.unkF0 -= 0x20;
        if (work->activeEntries[slot].presentation.unkF0 < 0x20) {
            work->activeEntries[slot].presentation.unkF0 = 0;
        }
        return;
    case 3:
        if (work->activeEntries[slot].presentation.unkF0 < 0x20 ||
            (work->activeEntries[slot].presentation.unkF0 -= 0x20,
             work->activeEntries[slot].presentation.unkF0 < 0x20)) {
            work->activeEntries[slot].presentation.unkF0 = 0;
            work->activeEntries[slot].presentation.pendingSceneState = 2;
            work->activeEntries[slot].presentation.cursorSelection =
                work->activeEntries[slot].presentation.pendingCursorSelection;
            work->activeEntries[slot].presentation.cursorOffset[0] =
                settle.values[work->activeEntries[slot].presentation.cursorSelection][0];
            work->activeEntries[slot].presentation.cursorOffset[1] =
                settle.values[work->activeEntries[slot].presentation.cursorSelection][1];
            work->activeEntries[slot].presentation.cursorOption =
                ids.values[work->activeEntries[slot].presentation.cursorSelection];
            if (work->activeEntries[slot].presentation.cursorSelection != 0) {
                work->activeEntries[slot].presentation.cursorPhase = 0;
            } else {
                work->activeEntries[slot].presentation.cursorPhase = 180;
                work->activeEntries[slot].presentation.cursorEchoCount = 0;
            }
        }
        break;
    case 2:
        settled = 0;
        if (work->activeEntries[slot].presentation.cursorSelection != 0) {
            work->activeEntries[slot].presentation.cursorPhase++;
            work->activeEntries[slot].presentation.cursorPhase =
                work->activeEntries[slot].presentation.cursorPhase <= 0 ? 0 :
                work->activeEntries[slot].presentation.cursorPhase > 0xB ? 0xB :
                work->activeEntries[slot].presentation.cursorPhase;
            if (work->activeEntries[slot].presentation.cursorPhase >= 5) {
                work->activeEntries[slot].presentation.unkF0 += 6;
                work->activeEntries[slot].presentation.unkF0 =
                    work->activeEntries[slot].presentation.unkF0 <= 0 ? 0 :
                    (s8)work->activeEntries[slot].presentation.unkF0 < 0 ? 0x80 :
                    work->activeEntries[slot].presentation.unkF0;
                if ((s8)work->activeEntries[slot].presentation.unkF0 < 0) {
                    settled = 1;
                }
            }
        } else {
            work->activeEntries[slot].presentation.cursorPhase =
                (work->activeEntries[slot].presentation.cursorPhase + 0x10) % 360;
            work->activeEntries[slot].presentation.unkF0 =
                (sdfSinPoly((f32)((work->activeEntries[slot].presentation.cursorPhase + 90) % 360) /
                            180.0f * 3.14159f) + 1.0f) * 0.5f * 96.0f + 0.0f;
            work->activeEntries[slot].presentation.cursorEchoCount++;
            work->activeEntries[slot].presentation.cursorEchoCount =
                work->activeEntries[slot].presentation.cursorEchoCount <= 0 ? 0 :
                work->activeEntries[slot].presentation.cursorEchoCount >= 6 ? 5 :
                work->activeEntries[slot].presentation.cursorEchoCount;
            if (work->activeEntries[slot].presentation.cursorEchoCount < 5) {
                s32 x = work->activeEntries[slot].position[0];
                s32 y = work->activeEntries[slot].position[1];
                btlInitializeActionRecordWithScale(
                    &work->activeEntries[slot].presentation.mirroredSprites[
                        work->activeEntries[slot].presentation.cursorEchoCount - 1],
                    work->activeEntries[slot].presentation.cursorOption,
                    x + echo.values[0][0],
                    y + echo.values[0][1],
                    work->activeEntries[slot].presentation.cursorEchoCount - 1,
                    6.0f - work->activeEntries[slot].presentation.cursorEchoCount);
            }
            settled = work->activeEntries[slot].presentation.cursorEchoCount > 4;
        }
        if (settled == 1) {
            work->activeEntries[slot].presentation.cursorOffset[0] =
                echo.values[work->activeEntries[slot].presentation.cursorSelection][0];
            work->activeEntries[slot].presentation.cursorOffset[1] =
                echo.values[work->activeEntries[slot].presentation.cursorSelection][1];
            work->activeEntries[slot].presentation.pendingSceneState = 1;
            work->activeEntries[slot].presentation.cursorOption =
                ids.values[work->activeEntries[slot].presentation.cursorSelection];
            work->activeEntries[slot].presentation.unkF0 = 0x80;
        }
        break;
    }
}



extern const BattlePanelColors D_003A2D00;

/* Draw the active row's selected cursor sprite at its stored cursor offset. */
void btlDrawActorPanelCursor(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
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
        effDrawSpriteWithCornerColors((x + work->activeEntries[slot].presentation.cursorOffset[0]) << 4,
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

void btlUpdateActorPanelHighlights(BtlUnit *unit, BattleActorPanelWork *work, s32 slot, s8 reserve) {
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

extern void evtSubmitGradientTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw an active/reserve slot's eight-alpha highlights, then reset primary blending. */
void btlDrawActorPanelHighlights(BtlUnit *unit, BattleActorPanelWork *work, s32 index, s8 reserve) {
    s32 x;
    s32 y;
    u32 alpha[8];
    s32 top;
    s32 middle;
    s32 bottom;
    s32 baseColor;
    s8 state;

    if (reserve == 0) {
        baseColor = 0x40FF50;
        state = work->activeEntries[index].presentation.presentationState;
        switch (work->activeEntries[index].presentation.presentationValue) {
        case 1:
            break;
        case 0:
            baseColor = 0xFF6787;
            break;
        case 2:
        default:
            baseColor = 0x1B11A0;
            break;
        }
    } else {
        state = work->reserveEntries[index].presentation.presentationState;
        baseColor = 0xFF6787;
    }

    if (state != 0) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        if (reserve == 0) {
            x = work->activeEntries[index].position[0];
            y = work->activeEntries[index].position[1];
            alpha[0] = work->activeEntries[index].presentation.highlightLevel[0];
            alpha[1] = work->activeEntries[index].presentation.highlightLevel[1];
            alpha[2] = work->activeEntries[index].presentation.highlightLevel[2];
            alpha[3] = work->activeEntries[index].presentation.highlightLevel[3];
            alpha[4] = work->activeEntries[index].presentation.highlightLevel[4];
            alpha[5] = work->activeEntries[index].presentation.highlightLevel[5];
            alpha[6] = work->activeEntries[index].presentation.highlightLevel[6];
            alpha[7] = work->activeEntries[index].presentation.highlightLevel[7];
        } else {
            x = work->reserveEntries[index].position[0];
            y = work->reserveEntries[index].position[1];
            alpha[0] = work->reserveEntries[index].presentation.highlightLevel[0];
            alpha[1] = work->reserveEntries[index].presentation.highlightLevel[1];
            alpha[2] = work->reserveEntries[index].presentation.highlightLevel[2];
            alpha[3] = work->reserveEntries[index].presentation.highlightLevel[3];
            alpha[4] = work->reserveEntries[index].presentation.highlightLevel[4];
            alpha[5] = work->reserveEntries[index].presentation.highlightLevel[5];
            alpha[6] = work->reserveEntries[index].presentation.highlightLevel[6];
            alpha[7] = work->reserveEntries[index].presentation.highlightLevel[7];
        }
        top = y + 49;
        middle = y + 63;
        evtSubmitGradientTriangle(x + 54, top, x + 106, top, x + 93, middle,
                      baseColor | (alpha[0] << 24),
                      baseColor | (alpha[1] << 24),
                      baseColor | (alpha[3] << 24));
        evtSubmitGradientTriangle(x + 54, top, x + 93, middle, x + 40, middle,
                      baseColor | (alpha[0] << 24),
                      baseColor | (alpha[3] << 24),
                      baseColor | (alpha[2] << 24));
        bottom = y + 77;
        evtSubmitGradientTriangle(x + 40, middle, x + 93, middle, x + 80, bottom,
                      baseColor | (alpha[4] << 24),
                      baseColor | (alpha[5] << 24),
                      baseColor | (alpha[7] << 24));
        evtSubmitGradientTriangle(x + 40, middle, x + 80, bottom, x + 26, bottom,
                      baseColor | (alpha[4] << 24),
                      baseColor | (alpha[7] << 24),
                      baseColor | (alpha[6] << 24));
    }
    evtSubmitPrimaryAlphaBlendMode(0);
}

void func_001B9A50(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    s32 i;

    switch (work->activeEntries[slot].presentation.presentationState) {
    case 3:
        work->activeEntries[slot].presentation.transitionGeometry[0]++;
        work->activeEntries[slot].presentation.transitionGeometry[1]--;
        work->activeEntries[slot].presentation.transitionGeometry[2]--;
        work->activeEntries[slot].presentation.transitionGeometry[3]++;
    case 4:
        for (i = 0; i < 2; i++) {
            if (work->activeEntries[slot].presentation.transitionFade[i] >= 16) {
                work->activeEntries[slot].presentation.transitionFade[i] -= 16;
            } else {
                work->activeEntries[slot].presentation.transitionFade[i] = 0;
            }
            if (work->activeEntries[slot].presentation.pulseLevel[i] >= 16) {
                work->activeEntries[slot].presentation.pulseLevel[i] -= 16;
            } else {
                work->activeEntries[slot].presentation.pulseLevel[i] = 0;
            }
        }
        if (work->activeEntries[slot].presentation.transitionFade[0] < 9) {
            work->activeEntries[slot].presentation.presentationState = 0;
        }
        break;
    case 1:
        for (i = 0; i < 2; i++) {
            switch (work->activeEntries[slot].presentation.pulseDirection[i]) {
            case 0:
                work->activeEntries[slot].presentation.pulseLevel[i] += 10;
                work->activeEntries[slot].presentation.pulseLevel[i] =
                    work->activeEntries[slot].presentation.pulseLevel[i] <= 32 ? 32 :
                    work->activeEntries[slot].presentation.pulseLevel[i] > 192 ? 192 :
                    work->activeEntries[slot].presentation.pulseLevel[i];
                if (work->activeEntries[slot].presentation.pulseLevel[i] >= 192) {
                    work->activeEntries[slot].presentation.pulseDirection[i] = 1;
                    work->activeEntries[slot].presentation.pulseLevel[i] = 192;
                }
                break;
            case 1:
            default:
                work->activeEntries[slot].presentation.pulseLevel[i] -= 12;
                work->activeEntries[slot].presentation.pulseLevel[i] =
                    work->activeEntries[slot].presentation.pulseLevel[i] <= 32 ? 32 :
                    work->activeEntries[slot].presentation.pulseLevel[i] > 192 ? 192 :
                    work->activeEntries[slot].presentation.pulseLevel[i];
                if (work->activeEntries[slot].presentation.pulseLevel[i] <= 32) {
                    work->activeEntries[slot].presentation.pulseLevel[i] = 32;
                    work->activeEntries[slot].presentation.pulseDirection[i] = 0;
                }
                break;
            }
        }
        break;
    case 2:
        work->activeEntries[slot].presentation.transitionGeometry[0] = 68;
        work->activeEntries[slot].presentation.transitionGeometry[1] = 38;
        work->activeEntries[slot].presentation.transitionGeometry[2] = 5;
        work->activeEntries[slot].presentation.transitionGeometry[3] = 64;
        for (i = 0; i < 2; i++) {
            work->activeEntries[slot].presentation.transitionFade[i] += 16;
            work->activeEntries[slot].presentation.transitionFade[i] =
                work->activeEntries[slot].presentation.transitionFade[i] <= 0 ? 0 :
                work->activeEntries[slot].presentation.transitionFade[i] >= 128 ? 128 :
                work->activeEntries[slot].presentation.transitionFade[i];
            work->activeEntries[slot].presentation.pulseLevel[i] += 12 - i * 2;
            work->activeEntries[slot].presentation.pulseLevel[i] =
                work->activeEntries[slot].presentation.pulseLevel[i] <= 0 ? 0 :
                work->activeEntries[slot].presentation.pulseLevel[i] > 96 ? 96 :
                work->activeEntries[slot].presentation.pulseLevel[i];
        }
        if (work->activeEntries[slot].presentation.transitionFade[0] >= 128) {
            work->activeEntries[slot].presentation.presentationState = 1;
            work->activeEntries[slot].presentation.pulseDirection[0] = 0;
            work->activeEntries[slot].presentation.pulseDirection[1] = 0;
            work->activeEntries[slot].presentation.pulseTimer[0] = 0;
            work->activeEntries[slot].presentation.pulseTimer[1] = 0;
            work->activeEntries[slot].presentation.pulseOffsets[0][0] = 4;
            work->activeEntries[slot].presentation.pulseOffsets[0][1] = 4;
            work->activeEntries[slot].presentation.pulseOffsets[1][0] = 2;
            work->activeEntries[slot].presentation.pulseOffsets[1][1] = 2;
        }
        break;
    }
}

extern const BattlePanelColors D_003A2D28;

/* Draw active-row pulse strips and offset echo layers with presentation tint. */
void btlDrawActorPanelPulse(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_003A2D28;
    s32 x;
    s32 y;
    s32 offsetX = 0;
    s32 offsetY = 0;
    s16 alpha = 0;
    u32 baseColor;
    u32 color;
    s32 n;

    switch (work->activeEntries[slot].presentation.presentationValue) {
    case 0:
        baseColor = 0x80808000;
        break;
    case 1:
        baseColor = 0x50FF4000;
        break;
    case 2:
    default:
        baseColor = 0x60111B00;
        break;
    }
    if (work->activeEntries[slot].presentation.presentationState < 4) {
        if (work->activeEntries[slot].presentation.presentationState > 0) {
            x = work->activeEntries[slot].position[0];
            y = work->activeEntries[slot].position[1];
            for (n = 0; n < 3; n++) {
                if (n > 0) {
                    alpha = work->activeEntries[slot].presentation.pulseLevel[n - 1];
                    offsetX = work->activeEntries[slot].presentation.pulseOffsets[n - 1][0];
                    offsetY = work->activeEntries[slot].presentation.pulseOffsets[n - 1][1];
                    alpha = alpha <= 0 ? 0 :
                        alpha >= work->activeEntries[slot].presentation.transitionFade[0] ?
                        work->activeEntries[slot].presentation.transitionFade[0] : alpha;
                    color = baseColor | alpha;
                } else {
                    color = baseColor | work->activeEntries[slot].presentation.transitionFade[0];
                }
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                effDrawSpriteWithCornerColors((x + work->activeEntries[slot].presentation.transitionGeometry[0] + offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.transitionGeometry[1] - offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 9, 0x53);
                if (n > 0) {
                    alpha = alpha <= 0 ? 0 :
                        alpha >= work->activeEntries[slot].presentation.transitionFade[1] ?
                        work->activeEntries[slot].presentation.transitionFade[1] : alpha;
                    color = baseColor | alpha;
                } else {
                    color = baseColor | work->activeEntries[slot].presentation.transitionFade[1];
                }
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                effDrawSpriteWithCornerColors((x + work->activeEntries[slot].presentation.transitionGeometry[2] - offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.transitionGeometry[3] + offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 0xC, 0x53);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D28);

void btlWriteTriangleHighlightPulse(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
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

extern void evtSubmitGradientTriangle(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw four transition triangles with per-vertex alpha; restore alpha mode. */
void btlDrawActorPanelTransitionTriangles(void *task, BattleActorPanelWork *work, s32 index) {
    s32 x;
    s32 y;
    u32 alpha[8];
    s32 top;
    s32 middle;
    s32 bottom;
    s32 baseColor = 0x40FF50;

    if (work->activeEntries[index].presentation.transitionState != 0) {
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        x = work->activeEntries[index].position[0];
        y = work->activeEntries[index].position[1];
        alpha[0] = work->activeEntries[index].presentation.triangleAlpha[0];
        alpha[1] = work->activeEntries[index].presentation.triangleAlpha[1];
        alpha[2] = work->activeEntries[index].presentation.triangleAlpha[2];
        alpha[3] = work->activeEntries[index].presentation.triangleAlpha[3];
        alpha[4] = work->activeEntries[index].presentation.triangleAlpha[4];
        alpha[5] = work->activeEntries[index].presentation.triangleAlpha[5];
        alpha[6] = work->activeEntries[index].presentation.triangleAlpha[6];
        alpha[7] = work->activeEntries[index].presentation.triangleAlpha[7];
        top = y + 49;
        middle = y + 63;
        evtSubmitGradientTriangle(x + 54, top, x + 106, top, x + 93, middle,
                      (alpha[0] << 24) | baseColor,
                      (alpha[1] << 24) | baseColor,
                      (alpha[3] << 24) | baseColor);
        evtSubmitGradientTriangle(x + 54, top, x + 93, middle, x + 40, middle,
                      (alpha[0] << 24) | baseColor,
                      (alpha[3] << 24) | baseColor,
                      (alpha[2] << 24) | baseColor);
        bottom = y + 77;
        evtSubmitGradientTriangle(x + 40, middle, x + 93, middle, x + 80, bottom,
                      (alpha[4] << 24) | baseColor,
                      (alpha[5] << 24) | baseColor,
                      (alpha[7] << 24) | baseColor);
        evtSubmitGradientTriangle(x + 40, middle, x + 80, bottom, x + 26, bottom,
                      (alpha[4] << 24) | baseColor,
                      (alpha[7] << 24) | baseColor,
                      (alpha[6] << 24) | baseColor);
    }
    evtSubmitPrimaryAlphaBlendMode(0);
}


void btlUpdateActorPanelSecondaryPulse(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
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

extern const BattlePanelColors D_003A2D58;

/* Draw the active row's secondary pulse strips using their own fade and offsets. */
void btlDrawActorPanelSecondaryPulse(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_003A2D58;
    s32 x;
    s32 y;
    s32 offsetX = 0;
    s32 offsetY = 0;
    s16 alpha = 0;
    u32 baseColor;
    u32 color;
    s32 n;

    switch (work->activeEntries[slot].presentation.secondaryPresentationValue) {
    case 0:
        baseColor = 0x80808000;
        break;
    case 1:
        baseColor = 0x50FF4000;
        break;
    case 2:
    default:
        baseColor = 0x60111B00;
        break;
    }
    if (work->activeEntries[slot].presentation.transitionState < 4) {
        if (work->activeEntries[slot].presentation.transitionState > 0) {
            x = work->activeEntries[slot].position[0];
            y = work->activeEntries[slot].position[1];
            for (n = 0; n < 3; n++) {
                if (n > 0) {
                    alpha = work->activeEntries[slot].presentation.secondaryPulseLevel[n - 1];
                    offsetX = work->activeEntries[slot].presentation.secondaryPulseOffsets[n - 1][0];
                    offsetY = work->activeEntries[slot].presentation.secondaryPulseOffsets[n - 1][1];
                    alpha = alpha <= 0 ? 0 :
                        alpha >= work->activeEntries[slot].presentation.secondaryFade[0] ?
                        work->activeEntries[slot].presentation.secondaryFade[0] : alpha;
                    color = baseColor | alpha;
                } else {
                    color = baseColor | work->activeEntries[slot].presentation.secondaryFade[0];
                }
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                effDrawSpriteWithCornerColors((x + work->activeEntries[slot].presentation.secondaryGeometry[0] + offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.secondaryGeometry[1] - offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 9, 0x53);
                if (n > 0) {
                    alpha = alpha <= 0 ? 0 :
                        alpha >= work->activeEntries[slot].presentation.secondaryFade[1] ?
                        work->activeEntries[slot].presentation.secondaryFade[1] : alpha;
                    color = baseColor | alpha;
                } else {
                    color = baseColor | work->activeEntries[slot].presentation.secondaryFade[1];
                }
                colors.values[0] = color;
                colors.values[1] = color;
                colors.values[2] = color;
                colors.values[3] = color;
                effDrawSpriteWithCornerColors((x + work->activeEntries[slot].presentation.secondaryGeometry[2] - offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.secondaryGeometry[3] + offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 0xC, 0x53);
            }
        }
    }
}

extern void btlUpdateAndDrawVitalBarPulse(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat);

/* Advance the selected HP/MP trail and draw up to three delayed bar pulses. */
void btlUpdateActorVitalPulseTrail(BtlUnit *unit, BattleActorPanelWork *work, s16 alpha, s32 slot, s8 stat) {
    s8 *frame;
    s32 count;
    s32 n;

    switch (stat) {
    case 0:
        frame = &work->activeEntries[slot].presentation.hpPulseFrame;
        break;
    case 1:
        frame = &work->activeEntries[slot].presentation.mpPulseFrame;
        break;
    case 2:
        frame = &work->reserveEntries[slot].presentation.hpPulseFrame;
        break;
    default:
        frame = &work->reserveEntries[slot].presentation.mpPulseFrame;
        break;
    }
    *frame += 1;
    *frame = *frame <= 0 ? 0 : *frame >= 13 ? 12 : *frame;
    count = *frame / 4;
    if (count > 0) {
        for (n = 0; n < count && n < 3; n++) {
            switch (stat) {
            case 0:
                btlUpdateAndDrawVitalBarPulse(unit, &work->activeEntries[slot].presentation.hpBarPulses[n],
                              work->activeEntries[slot].position[0], work->activeEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 0);
                break;
            case 1:
                btlUpdateAndDrawVitalBarPulse(unit, &work->activeEntries[slot].presentation.mpBarPulses[n],
                              work->activeEntries[slot].position[0], work->activeEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 1);
                break;
            case 2:
                btlUpdateAndDrawVitalBarPulse(unit, &work->reserveEntries[slot].presentation.hpBarPulses[n],
                              work->reserveEntries[slot].position[0], work->reserveEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 2);
                break;
            default:
                btlUpdateAndDrawVitalBarPulse(unit, &work->reserveEntries[slot].presentation.mpBarPulses[n],
                              work->reserveEntries[slot].position[0], work->reserveEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, stat);
                break;
            }
        }
    }
}

extern const BattlePanelColors D_003A2D68;

/* Advance one HP/MP bar pulse and draw it when the unit status permits. */
void btlUpdateAndDrawVitalBarPulse(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat) {
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
        effDrawSpriteWithCornerColors((x + xOffset + pulse->progress) << 4,
                     (y + yOffset + pulse->yOffset) << 3, 0, colors.values,
                     0, btlResourceBlock->resA, sprite, 0x53);
    }
}

void func_001BB440(BtlUnit *unit, BattleActorPanelWork *work, s32 slot) {
    switch (work->activeEntries[slot].presentation.hpState) {
    case 0x11:
        work->activeEntries[slot].presentation.offset[0] = btlNextScaledRandom(8);
        work->activeEntries[slot].presentation.offset[0] =
            work->activeEntries[slot].presentation.offset[0] <= 0 ? 0 :
            work->activeEntries[slot].presentation.offset[0] > 8 ? 8 : work->activeEntries[slot].presentation.offset[0];
        work->activeEntries[slot].presentation.offset[1] = btlNextScaledRandom(8);
        work->activeEntries[slot].presentation.offset[1] =
            work->activeEntries[slot].presentation.offset[1] <= 0 ? 0 :
            work->activeEntries[slot].presentation.offset[1] > 8 ? 8 : work->activeEntries[slot].presentation.offset[1];
        work->activeEntries[slot].presentation.hpHighlightLevel -= 8;
        work->activeEntries[slot].presentation.hpHighlightLevel =
            work->activeEntries[slot].presentation.hpHighlightLevel <= 0 ? 0 :
            work->activeEntries[slot].presentation.hpHighlightLevel > 0x7F ? 0x7F :
            work->activeEntries[slot].presentation.hpHighlightLevel;
        if (++work->activeEntries[slot].presentation.hpEffectTimer >= 0x14) {
            work->activeEntries[slot].presentation.hpState = 0;
            work->activeEntries[slot].presentation.hpLevel = unit->partyRecord.hp;
            
            work->activeEntries[slot].presentation.offset[0] = work->activeEntries[slot].presentation.hpEffectTimer = work->activeEntries[slot].presentation.hpHighlightLevel = 0;
            work->activeEntries[slot].presentation.offset[1] = 0;
            btlClearFinishedActorSlotChannels(work, 0);
        }
        break;
    case 0x21:
        work->activeEntries[slot].presentation.hpState = 0;
        work->activeEntries[slot].presentation.hpLevel = unit->partyRecord.hp;
        work->activeEntries[slot].presentation.hpEffectTimer = work->activeEntries[slot].presentation.hpHighlightLevel = 0;
        work->activeEntries[slot].presentation.offset[0] = 0;
        work->activeEntries[slot].presentation.offset[1] = 0;
        break;
    }
    switch (work->activeEntries[slot].presentation.mpState) {
    default:
        return;
    case 0x11:
    case 0x21:
        if (++work->activeEntries[slot].presentation.mpEffectTimer >= 20) {
            work->activeEntries[slot].presentation.mpState = 0;
            work->activeEntries[slot].presentation.mpLevel = unit->partyRecord.mp;
            work->activeEntries[slot].presentation.mpHighlightLevel = 0;
            work->activeEntries[slot].presentation.mpEffectTimer = 0;
            btlClearFinishedActorSlotChannels(work, 0);
        }
        break;
    }
}

extern void func_001BC540(BtlUnit *, s32, s32, s32, s32, s32, s32, s16, s8, s8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D58);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2D68);

const BattleStatEffectOffsets D_003A2D78 = {{0x36, 0x31}, {0x1A, 0x40}};

/* Draw active HP/MP change effects while fading and clearing their effect states. */
void btlUpdateActorVitalChangeEffects(BtlUnit *unit, BattleActorPanelWork *work, s32 slot) {
    BattleStatEffectOffsets offsets = D_003A2D78;
    s32 alpha;
    s32 type;

    if (work->activeEntries[slot].presentation.hpEffectState == 0x11 ||
        work->activeEntries[slot].presentation.hpEffectState == 0x21) {
        type = work->activeEntries[slot].presentation.hpEffectState == 0x11 ? 1 : 2;
        alpha = work->activeEntries[slot].presentation.fade < 0x80 ?
            work->activeEntries[slot].presentation.fade : work->activeEntries[slot].presentation.hpEffectFade;
        func_001BC540(unit, unit->partyRecord.hp, work->activeEntries[slot].presentation.hpTarget,
                      unit->partyRecord.maxHp, 10,
                      work->activeEntries[slot].position[0] + offsets.hp.x,
                      work->activeEntries[slot].position[1] + offsets.hp.y, alpha, type, 0);
        work->activeEntries[slot].presentation.hpEffectFade -= 8;
        work->activeEntries[slot].presentation.hpEffectFade =
            work->activeEntries[slot].presentation.hpEffectFade <= 0 ? 0 :
            work->activeEntries[slot].presentation.hpEffectFade > 0xFF ? 0xFF :
            work->activeEntries[slot].presentation.hpEffectFade;
        if (work->activeEntries[slot].presentation.hpEffectFade <= 0) {
            if (work->activeEntries[slot].presentation.hpEffectState == 0x21) {
                btlUpdateActorSlotStates((u8 *)work, 0);
            }
            work->activeEntries[slot].presentation.hpEffectState = 0;
        }
    }
    if (work->activeEntries[slot].presentation.mpEffectState == 0x11 ||
        work->activeEntries[slot].presentation.mpEffectState == 0x21) {
        type = work->activeEntries[slot].presentation.mpEffectState == 0x11 ? 1 : 2;
        alpha = work->activeEntries[slot].presentation.fade < 0x80 ?
            work->activeEntries[slot].presentation.fade : work->activeEntries[slot].presentation.mpEffectFade;
        func_001BC540(unit, unit->partyRecord.mp, work->activeEntries[slot].presentation.mpTarget,
                      unit->partyRecord.maxMp, 11,
                      work->activeEntries[slot].position[0] + offsets.mp.x,
                      work->activeEntries[slot].position[1] + offsets.mp.y, alpha, type, 1);
        work->activeEntries[slot].presentation.mpEffectFade -= 8;
        work->activeEntries[slot].presentation.mpEffectFade =
            work->activeEntries[slot].presentation.mpEffectFade <= 0 ? 0 :
            work->activeEntries[slot].presentation.mpEffectFade > 0xFF ? 0xFF :
            work->activeEntries[slot].presentation.mpEffectFade;
        if (work->activeEntries[slot].presentation.mpEffectFade <= 0) {
            work->activeEntries[slot].presentation.mpEffectState = 0;
        }
    }
}

/* Compare cached HP/MP with current values and prepare change targets and pulse states. */
void btlTrackActorVitalChanges(BtlUnit *unit, BattleActorPanelWork *work, s32 slot, s8 kind) {
    switch (kind) {
    case 0:
        if (work->activeEntries[slot].presentation.hpState == 2) {
            work->activeEntries[slot].presentation.hpState = 0;
            work->activeEntries[slot].presentation.hpLevel = unit->partyRecord.hp;
        } else {
            if (work->activeEntries[slot].presentation.hpState == 0 || work->activeEntries[slot].presentation.hpState >= 0x10) {
                if (unit->partyRecord.hp < work->activeEntries[slot].presentation.hpLevel) {
                    if (work->activeEntries[slot].presentation.hpState == 0 || work->activeEntries[slot].presentation.hpState == 0x10) {
                        work->activeEntries[slot].presentation.hpEffectState = 0x11;
                        work->activeEntries[slot].presentation.hpEffectFade = 0xFF;
                        work->activeEntries[slot].presentation.hpPulseFrame = 0;
                        memset(&work->activeEntries[slot].presentation.hpBarPulse, 0, sizeof(BattleStatPulse));
                        memset(&work->activeEntries[slot].presentation.hpBarPulses[0], 0, sizeof(BattleStatPulse));
                        memset(&work->activeEntries[slot].presentation.hpBarPulses[1], 0, sizeof(BattleStatPulse));
                        memset(&work->activeEntries[slot].presentation.hpBarPulses[2], 0, sizeof(BattleStatPulse));
                        work->activeEntries[slot].presentation.hpEffectTimer = 0;
                        work->activeEntries[slot].presentation.offset[0] = 0;
                        work->activeEntries[slot].presentation.offset[1] = 0;
                    }
                    work->activeEntries[slot].presentation.hpTarget = work->activeEntries[slot].presentation.hpLevel;
                } else if (work->activeEntries[slot].presentation.hpLevel < unit->partyRecord.hp) {
                    if (work->activeEntries[slot].presentation.hpState == 0) {
                        work->activeEntries[slot].presentation.hpState = 0x20;
                        work->activeEntries[slot].presentation.hpHighlightLevel = 0x7F;
                        work->activeEntries[slot].presentation.hpEffectState = 0x21;
                        work->activeEntries[slot].presentation.hpEffectFade = 0xFF;
                    }
                    work->activeEntries[slot].presentation.hpTarget = work->activeEntries[slot].presentation.hpLevel;
                }
            }
            switch (work->activeEntries[slot].presentation.hpState) {
            case 0:
                work->activeEntries[slot].presentation.hpLevel = unit->partyRecord.hp;
                return;
            case 0x10:
            case 0x20:
                work->activeEntries[slot].presentation.hpPulseFrame = 0;
                memset(&work->activeEntries[slot].presentation.hpBarPulse, 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.hpBarPulses[0], 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.hpBarPulses[1], 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.hpBarPulses[2], 0, sizeof(BattleStatPulse));
                work->activeEntries[slot].presentation.hpEffectTimer = 0;
                work->activeEntries[slot].presentation.offset[0] = 0;
                work->activeEntries[slot].presentation.offset[1] = 0;
                work->activeEntries[slot].presentation.hpState++;
                break;
            }
        }
        break;
    case 1:
        if (work->activeEntries[slot].presentation.mpState == 2) {
            work->activeEntries[slot].presentation.mpState = 0;
            work->activeEntries[slot].presentation.mpLevel = unit->partyRecord.mp;
        } else {
            if (work->activeEntries[slot].presentation.mpState == 0) {
                if (unit->partyRecord.mp < work->activeEntries[slot].presentation.mpLevel) {
                    work->activeEntries[slot].presentation.mpState = 0x10;
                    work->activeEntries[slot].presentation.mpHighlightLevel = 0x7F;
                    work->activeEntries[slot].presentation.mpEffectState = 0x11;
                    work->activeEntries[slot].presentation.mpEffectFade = 0xFF;
                    work->activeEntries[slot].presentation.mpTarget = work->activeEntries[slot].presentation.mpLevel;
                } else if (work->activeEntries[slot].presentation.mpLevel < unit->partyRecord.mp) {
                    work->activeEntries[slot].presentation.mpState = 0x20;
                    work->activeEntries[slot].presentation.mpHighlightLevel = 0x7F;
                    work->activeEntries[slot].presentation.mpEffectState = 0x21;
                    work->activeEntries[slot].presentation.mpEffectFade = 0xFF;
                    work->activeEntries[slot].presentation.mpTarget = work->activeEntries[slot].presentation.mpLevel;
                }
            }
            switch (work->activeEntries[slot].presentation.mpState) {
            case 0:
                work->activeEntries[slot].presentation.mpLevel = unit->partyRecord.mp;
                break;
            case 0x10:
            case 0x20:
                work->activeEntries[slot].presentation.mpPulseFrame = 0;
                memset(&work->activeEntries[slot].presentation.mpBarPulse, 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.mpBarPulses[0], 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.mpBarPulses[1], 0, sizeof(BattleStatPulse));
                memset(&work->activeEntries[slot].presentation.mpBarPulses[2], 0, sizeof(BattleStatPulse));
                work->activeEntries[slot].presentation.mpEffectTimer = 0;
                work->activeEntries[slot].presentation.mpState++;
                break;
            }
        }
        break;
    }
}

typedef struct BattleHudOffsets { s32 values[5][2]; } BattleHudOffsets;
typedef struct BattleHudSprites { s32 values[5]; } BattleHudSprites;
extern const BattleHudSprites D_003A2DB0;
extern const BattlePanelColors D_003A2DC8;
extern SdfPoolNode D_003255A8;
extern void evtSetDrawSurfaceIndex(u32);
extern void evtSubmitPrimaryGsTest(s32,s32,s32,s32,s32,s32,s32,s32);
extern void btlDrawButtonIconFixed64(SdfPoolNode *,s32,s32,s32,s32,s32,s32,s32);
extern void btlDrawButtonIcon(SdfPoolNode *,s32,s32,s32,s32,s32,s32,s32);
void func_001BBE18(BtlUnit *unit, BattleActorPanelWork *work, s32 slot,
                   s8 bank, s16 alpha, s32 x, s32 y) {
    BattleHudOffsets offsets = {{{10, 43}, {51, 47}, {23, 62}, {54, 49}, {26, 64}}};
    BattleHudSprites sprites = D_003A2DB0;
    BattlePanelColors colors = D_003A2DC8;
    s32 level;
    u32 color;
    u32 i;

    colors.values[0] = alpha | 0x80808000;
    colors.values[1] = colors.values[0];
    colors.values[2] = colors.values[0];
    colors.values[3] = colors.values[0];
    for (i = 0; i < 1; i++) {
        effDrawSpriteWithCornerColors((x + offsets.values[i][0]) << 4,
            (y + offsets.values[i][1]) << 3, 0, colors.values, 0,
            btlResourceBlock->resA, sprites.values[i], 0x53);
    }
    if (bank == 0) {
        level = work->activeEntries[slot].presentation.fade;
        if (work->activeEntries[slot].presentation.transitionState == 1 ||
            work->activeEntries[slot].presentation.transitionState == 2) {
            work->activeEntries[slot].presentation.secondaryPresentationValue = 1;
            btlChooseActorPanelColors(unit, 1,
                work->activeEntries[slot].presentation.transitionState,
                work->activeEntries[slot].presentation.transitionFade[0], &colors);
        } else {
            btlChooseActorPanelColors(unit,
                work->activeEntries[slot].presentation.presentationValue,
                work->activeEntries[slot].presentation.presentationState,
                work->activeEntries[slot].presentation.transitionFade[0], &colors);
        }
        colors.values[0] = (level << 24) | (colors.values[0] & 0xFFFFFF);
        colors.values[1] = (level << 24) | (colors.values[1] & 0xFFFFFF);
        colors.values[2] = (level << 24) | (colors.values[2] & 0xFFFFFF);
        colors.values[3] = (level << 24) | (colors.values[3] & 0xFFFFFF);
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);
        btlDrawButtonIconFixed64(&D_003255A8,
            work->activeEntries[slot].position[0] + 9,
            work->activeEntries[slot].position[1] + 7,
            colors.values[0], colors.values[1], colors.values[2], colors.values[3],
            unit->partyRecord.unitId);
        if (work->activeEntries[slot].presentation.offset[0] != 0) {
            level = work->activeEntries[slot].presentation.fade >> 2;
            colors.values[0] = (level << 24) | (colors.values[0] & 0xFFFFFF);
            colors.values[1] = (level << 24) | (colors.values[1] & 0xFFFFFF);
            colors.values[2] = (level << 24) | (colors.values[2] & 0xFFFFFF);
            colors.values[3] = (level << 24) | (colors.values[3] & 0xFFFFFF);
            btlDrawButtonIconFixed64(&D_003255A8,
                work->activeEntries[slot].position[0] + work->activeEntries[slot].presentation.offset[0] + 9,
                work->activeEntries[slot].position[1] + work->activeEntries[slot].presentation.offset[1] + 7,
                colors.values[0], colors.values[1], colors.values[2], colors.values[3],
                unit->partyRecord.unitId);
            level = work->activeEntries[slot].presentation.fade >> 3;
            colors.values[0] = (level << 24) | (colors.values[0] & 0xFFFFFF);
            colors.values[1] = (level << 24) | (colors.values[1] & 0xFFFFFF);
            colors.values[2] = (level << 24) | (colors.values[2] & 0xFFFFFF);
            colors.values[3] = (level << 24) | (colors.values[3] & 0xFFFFFF);
            /* The negative echo uses the opposite axis's displacement. */
            btlDrawButtonIconFixed64(&D_003255A8,
                work->activeEntries[slot].position[0] - work->activeEntries[slot].presentation.offset[1] + 9,
                work->activeEntries[slot].position[1] - work->activeEntries[slot].presentation.offset[0] + 7,
                colors.values[0], colors.values[1], colors.values[2], colors.values[3],
                unit->partyRecord.unitId);
        }
        level = work->activeEntries[slot].presentation.fade;
        color = (level << 24) | 0x808080;
        if (unit->partyRecord.status & 0x7FFF) {
            btlDrawButtonIcon(&D_003255A8,
                work->activeEntries[slot].position[0] + 58,
                work->activeEntries[slot].position[1] + 22,
                color, color, color, color, unit->partyRecord.status);
        }
    }
    if (bank == 1) {
        u32 opacity;

        level = work->reserveEntries[slot].presentation.fade;
        btlChooseActorPanelColors(unit,
            work->reserveEntries[slot].presentation.presentationValue,
            work->reserveEntries[slot].presentation.presentationState,
            work->reserveEntries[slot].presentation.transitionFade[0], &colors);
        opacity = (u32)level << 24;
        colors.values[0] = opacity | (colors.values[0] & 0xFFFFFF);
        colors.values[1] = opacity | (colors.values[1] & 0xFFFFFF);
        colors.values[2] = opacity | (colors.values[2] & 0xFFFFFF);
        colors.values[3] = opacity | (colors.values[3] & 0xFFFFFF);
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 0, 0x80, 1, 0, 0, 1, 1);
        btlDrawButtonIconFixed64(&D_003255A8,
            work->reserveEntries[slot].position[0] + 9,
            work->reserveEntries[slot].position[1] + 7,
            colors.values[0], colors.values[1], colors.values[2], colors.values[3],
            unit->partyRecord.unitId);
        if (unit->partyRecord.status & 0x7FFF) {
            color = opacity | 0x808080;
            btlDrawButtonIcon(&D_003255A8,
                work->reserveEntries[slot].position[0] + 58,
                work->reserveEntries[slot].position[1] + 22,
                color, color, color, color, unit->partyRecord.status);
        }
    }
    colors.values[0] = alpha | 0x80808000;
    colors.values[1] = colors.values[0];
    colors.values[2] = colors.values[0];
    colors.values[3] = colors.values[0];
    for (i = 1; i < 3; i++) {
        effDrawSpriteWithCornerColors((x + offsets.values[i][0]) << 4,
            (y + offsets.values[i][1]) << 3, 0, colors.values, 0,
            btlResourceBlock->resA, sprites.values[i], 0x53);
    }
    func_001BC540(unit, unit->partyRecord.hp, unit->partyRecord.hp,
        unit->partyRecord.maxHp, sprites.values[3],
        x + offsets.values[3][0], y + offsets.values[3][1], alpha, 0, 0);
    func_001BC540(unit, unit->partyRecord.mp, unit->partyRecord.mp,
        unit->partyRecord.maxMp, sprites.values[4],
        x + offsets.values[4][0], y + offsets.values[4][1], alpha, 0, 1);
}

extern const BattlePanelColors D_003A2DD8;

/* Crop the HP/MP sprite to the selected amount, draw its change overlay, then
 * restore the resource-owned geometry and crop parameters for later users. */
void func_001BC540(BtlUnit *unit, s32 current, s32 displayed, s32 maximum,
                   s32 sprite, s32 x, s32 y, s16 alpha, s8 mode, s8 stat) {
    BattlePanelColors colors = D_003A2DD8;
    s32 flags = 0;
    s32 offset = 0;
    s32 displayedWidth = 0;
    s32 span;
    s32 currentWidth;
    f32 ratio;
    u32 color;
    s32 i;

    if (displayed == 0) {
        span = btlResourceBlock->resA->descriptions[sprite].colors[0] - btlResourceBlock->resA->descriptions[sprite].colors[2];
        btlResourceBlock->resA->workEntries[sprite].geometry.bounds[2] = 0;
        btlResourceBlock->resA->workEntries[sprite].parameters[2] = span;
    } else {
        ratio = (f32)displayed;
        ratio /= (f32)maximum;
        span = btlResourceBlock->resA->descriptions[sprite].colors[2] - btlResourceBlock->resA->descriptions[sprite].colors[0];
        displayedWidth = (s32)((f32)btlResourceBlock->resA->workEntries[sprite].sourceWidth * ratio);
        btlResourceBlock->resA->workEntries[sprite].geometry.bounds[2] = displayedWidth << 4;
        span = (s32)((f32)span * ratio) - span;
        btlResourceBlock->resA->workEntries[sprite].parameters[2] = span;
    }
    if (mode != 0 && current != displayed) {
        ratio = (f32)current;
        ratio /= (f32)maximum;
        span = btlResourceBlock->resA->descriptions[sprite].colors[2] - btlResourceBlock->resA->descriptions[sprite].colors[0];
        currentWidth = (s32)((f32)btlResourceBlock->resA->workEntries[sprite].sourceWidth * ratio);
        btlResourceBlock->resA->workEntries[sprite].parameters[0] = (s32)((f32)span * ratio);
        btlResourceBlock->resA->workEntries[sprite].geometry.bounds[2] = (displayedWidth - currentWidth) << 4;
        offset = currentWidth;
    }
    switch (mode) {
    case 0:
        if ((unit->status.flags & 0x20) || (unit->partyRecord.status & 0x4800)) {
            color = stat ? 0x20204000 : 0x30202000;
        } else {
            color = 0x80808000;
        }
        colors.values[3] = colors.values[2] = colors.values[1] = colors.values[0] = color | alpha;
        flags = 0;
        break;
    case 1:
        colors.values[3] = colors.values[2] = colors.values[1] = colors.values[0] = 0xFFFFFF00 | alpha;
        effDrawSpriteWithCornerColors((x + offset) << 4, y << 3, 0,
                                      colors.values, 0, btlResourceBlock->resA, sprite, 0x53);
        flags = 0x60;
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 1, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        break;
    case 2:
        colors.values[3] = colors.values[2] = colors.values[1] = colors.values[0] = 0xFFFFFF00 | alpha;
        flags = 0x60;
        evtSetDrawSurfaceIndex(0x53);
        evtSubmitPrimaryGsTest(1, 1, 0x80, 3, 0, 0, 1, 1);
        evtSubmitPrimaryAlphaBlendMode(1);
        break;
    }
    effDrawSpriteWithCornerColors((x + offset) << 4, y << 3, 0,
                                  colors.values, flags, btlResourceBlock->resA, sprite, 0x53);
    if (mode == 2) {
        for (i = 0; i < 4; i++) {
            colors.values[3] = colors.values[2] = colors.values[1] = colors.values[0] =
                0xFFFFFF00 | (alpha >> (i + 1));
            effDrawSpriteWithCornerColors((x + offset) << 4, y << 3, 0,
                                          colors.values, 0, btlResourceBlock->resA, sprite, 0x53);
        }
    }
    btlResourceBlock->resA->workEntries[sprite].geometry.bounds[2] = btlResourceBlock->resA->workEntries[sprite].sourceWidth << 4;
    btlResourceBlock->resA->workEntries[sprite].geometry.bounds[3] = btlResourceBlock->resA->workEntries[sprite].sourceHeight << 3;
    btlResourceBlock->resA->workEntries[sprite].parameters[2] =
        btlResourceBlock->resA->workEntries[sprite].parameters[0] = 0;
}

extern SdfPoolNode D_003255A8;

#include "sdf_packet_alloc.h"

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
        first = dds3SearchProcessName(btlCommandPanelTaskNameRef);
        second = dds3SearchProcessName(D_003BB3B0);
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

void btlSetCommandPanelFadeMode(s8 mode, s32 duration) {
    KwlnTask *commandTask = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    KwlnTask *panelTask = dds3SearchProcessName(D_003BB3B0);
    KwlnTask *registeredTask = dds3SearchProcessName(D_003BB3C0);

    switch (mode) {
    case 0: {
        s32 step;
        if (commandTask != NULL) {
            ((BattleSceneObject *)dds3GetProcessData(commandTask))->state = 11;
            btlCommandPanelWork->state = 4;
        }
        if (panelTask != NULL) {
            btlTrackedTaskHandles->presentationState = 1;
        }
        if (registeredTask != NULL) {
            ((BattleRegisteredPanelWork *)dds3GetProcessData(registeredTask))->mode = 3;
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
            ((BattleSceneObject *)dds3GetProcessData(commandTask))->state = mode;
            btlCommandPanelWork->state = mode;
        }
        if (panelTask != NULL) {
            btlTrackedTaskHandles->presentationState = 2;
        }
        if (registeredTask != NULL) {
            btlResetRegisteredPanelEntrance((BattleRegisteredPanelWork *)dds3GetProcessData(registeredTask));
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
        task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
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
                    btlSetActionActorPanelPresentation(((BattleSceneObject *)dds3GetProcessData(task))->owner, 2, 0);
                }
                btlTrackedTaskHandles->status.bytes.state = 0;
            }
            break;
        }
    }
    return 0;
}

void btlUpdateSceneFlow(void) {
    s32 context = (s32)(u32)btlGetRuntime();
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
    glyph = itfCreateFont2TextGlyph(x << 4, y << 3, 0, color, text, 0);
    frFontDrawGlyphChain(glyph, 1, 0x53);
    frDestroyFontRequestAfter((struct FrFontGlyph *)(u32)glyph);
    itfSetTextDrawLimit(-1);
}

extern u8 *D_003BAA8C;

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    struct FrFontGlyph *handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA8C + index * 17, 0);
    btlSubmitSharedTextGlyph(handle);
    itfSetTextDrawLimit(-1);
}

extern u8 *D_003BAA84;

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    struct FrFontGlyph *handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_003BAA84 + index * 25, 0);
    btlSubmitSharedTextGlyph(handle);
    itfSetTextDrawLimit(-1);
}

s32 btlIsSceneActorLimitSatisfied(s32 unused, u32 limit) {
    s32 context = (s32)(u32)btlGetRuntime();
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
s32 btlGetActorCommandClass(BattleSceneObject *object, s8 mode) {
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

extern u16 *btlGatherActorCommandsByKind(s32, s16 *, u16, u16, u16);

extern void btlBuildEligibleActorList(s32, s16 *);

extern BattleRosterTable *btlGetActorLinkageSkillList(BattleSceneObject *, u16 *);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DB0);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DC8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2DD8);

u32 btlGetCommandOptionCount(s32 object, s8 mode, s8 unlimited) {
    s32 kind = btlGetActorCommandClass((BattleSceneObject *)object, mode);
    s16 count;

    switch (kind) {
    case 0:
        btlGatherActorCommandsByKind(object, &count, 0, 2, 3);
        D_003BD834->value[0] = count + 1;
        break;
    case 4:
        btlGetActorLinkageSkillList((BattleSceneObject *)object, (u16 *)&count);
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

/* Gather kind-filtered actor commands; count includes the default slot for kinds 0/5. */
u16 *btlGatherActorCommandsByKind(s32 address, s16 *outCount, u16 firstKind, u16 secondKind, u16 thirdKind) {
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
        if (!(records[id].menuFlags & 2)) {
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

u16 *btlGatherEnemyCommands(BtlUnit *actor, s16 *outCount) {
    DatEnemyRecord *enemy;
    DatCommandRecord *records;
    u16 *out = D_00359960;
    s32 gathered;
    s32 count;
    s32 i;

    btlGetRuntime();
    enemy = datEnemyRecords;
    enemy += actor->partyRecord.unitId;
    memset(out, 0, 0x18);
    records = datCommandRecords;
    gathered = 0;
    for (i = 0; i < 8; i++) {
        u16 id = enemy->skills[i];
        if ((records[id].menuFlags & 2) != 0 && id != 0) {
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
            /* Retail 0x001BD5EC..0x001BD610 also copies the unused trailing slot. */
            for (j = i; j < count; j++) {
                out[j] = out[j + 1];
            }
            count--;
        } else {
            i++;
        }
    }
    *outCount = count;
    return out;
}

extern u8 D_00358FE0[];

void btlBuildEligibleActorList(s32 unused, s16 *count) {
    s32 context = (s32)(u32)btlGetRuntime();
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


/* Refresh and return the shared LINKAGE skill/member list for the owning actor. */
BattleRosterTable *btlGetActorLinkageSkillList(BattleSceneObject *object, u16 *value) {
    s32 result = btlBuildActorLinkageSkillList(object->owner->unit, &D_00359160);
    *value = result;
    return &D_00359160;
}

extern u16 btlReadUnitStatusMask(DatPartyRecord *);
#include "btl_command_affinity_api.h"
extern void btlCreateReserveActorPanelTask(void);
extern s32 btlIsSupportedCommandKind(BattleIndexWork *);
extern s32 func_001BF040(BtlUnit *, BattleRosterPair *);
extern s16 D_003BB494;
extern s16 D_003BB496;
extern s16 D_003BB498;

s32 func_001BD750(BattleSceneObject *object) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u16 count = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 1);
    s32 kind = btlGetActorCommandClass(object, btlCommandPanelWork->classIndex);
    s16 entry = object->selections[kind].entry;
    s16 cursor = object->selections[kind].cursor;
    BtlCommandPartnerOutput partners;
    u16 *commands;
    u16 *picked;

    if (D_00324510.confirm < 0) {
        switch (kind) {
        case 0:
            if (entry == 0 && cursor == 0) {
                object->commandData->skillId = 0;
                object->commandData->phase = 1;
                D_003BB494 = 1;
                D_003BB496 = 0;
                D_003BB498 = 0;
            } else {
                commands = btlGatherActorCommandsByKind((s32)object, (s16 *)&count, 0, 2, 3);
                if (entry == (u16)count) {
                    object->commandData->phase = 10;
                    btlSetActorCommandSelection((s32)object, 10, entry, 0);
                } else {
                    picked = &commands[entry - 1];
                    if (btlGetCommandFailureReason(object->owner->unit, *picked) != 0) {
                        sndSetStationedSeVolume(10);
                        break;
                    }
                    object->commandData->phase = 2;
                    object->commandData->skillId = *picked;
                    if (btlIsSupportedCommandKind(object->commandData) == 0) {
                        btlSetActorCommandSelection((s32)object, 2, entry, *picked);
                    } else {
                        D_003BB494 = 2;
                        D_003BB496 = entry;
                        D_003BB498 = *picked;
                    }
                }
            }
            object->state = 3;
            sndSetStationedSeVolume(8);
            return 1;
        case 4:
            if ((object->owner->unit->status.flags & 0x400) != 0) {
                if (btlHasRequiredCommandAffinityPartners(object->owner->unit, D_00359160.entries[entry].skill) == 0) {
                    break;
                }
                if (btlResolveCommandAffinityPartners(object->owner->unit, D_00359160.entries[entry].skill, &partners) == 0) {
                    break;
                }
                object->owner->indexWork.companionA = partners.firstPartner;
                object->commandData->skillId = partners.commandId;
                object->commandData->phase = 2;
                object->owner->indexWork.companionB = partners.secondPartner;
                object->owner->indexWork.skillId = partners.commandId;
                object->state = 3;
                sndSetStationedSeVolume(8);
                return 1;
            }
            if (func_001BF040(object->owner->unit, &D_00359160.entries[entry]) != 0) {
                sndSetStationedSeVolume(10);
                break;
            }
            object->commandData->phase = 2;
            object->commandData->skillId = D_00359160.entries[entry].skill;
            object->owner->indexWork.skillId = D_00359160.entries[entry].skill;
            object->owner->indexWork.companionA = D_00359160.entries[entry].first;
            object->state = 3;
            object->owner->indexWork.companionB = D_00359160.entries[entry].second;
            if ((battle->battleFlags & 0x1000000) != 0) {
                btlSetActorCommandSelection((s32)object, 3, entry, D_00359160.entries[entry].skill);
            } else {
                btlSetActorCommandSelection((s32)object, 0, 0, 0);
            }
            sndSetStationedSeVolume(8);
            return 1;
        case 2:
            object->commandData->phase = 4;
            object->commandData->reference = D_00358FE0[entry * 2];
            object->state = 3;
            btlSetActorCommandSelection((s32)object, 0, 0, 0);
            sndSetStationedSeVolume(8);
            return 1;
        case 3:
            switch (entry) {
            case 0:
                if ((btlReadUnitStatusMask(&object->owner->unit->partyRecord) & 0x1000) != 0) {
                    sndSetStationedSeVolume(10);
                    break;
                }
                object->commandData->phase = 5;
                object->state = 3;
                btlSetActorCommandSelection((s32)object, 0, 0, 0);
                sndSetStationedSeVolume(8);
                return 1;
            case 1: {
                s32 capacity = 0;
                s32 i;

                for (i = 0; i < datGameState->partyCount; i++) {
                    if ((datGameState->party[i].flags & 2) != 0) {
                        capacity++;
                    }
                }
                capacity = datGameState->partyCount - capacity;
                if ((datBattleSceneRecords[battle->battleMode].flags & 0x800) != 0) {
                    capacity = 0;
                }
                if (capacity == 0) {
                    sndSetStationedSeVolume(10);
                    break;
                }
                object->state = 6;
                sndSetStationedSeVolume(8);
                btlCreateReserveActorPanelTask();
                return 1;
            }
            case 2:
            default:
                if (btlIsSceneActorLimitSatisfied((s32)object->owner->unit, 1) != 0) {
                    sndSetStationedSeVolume(10);
                    break;
                }
                object->commandData->phase = 11;
                object->commandData->linkedUnit = object->owner->unit;
                object->state = 3;
                btlSetActorCommandSelection((s32)object, 0, 0, 0);
                sndSetStationedSeVolume(8);
                return 1;
            }
            break;
        case 6:
            if (datBattleSceneRecords[battle->battleMode].voice != 0) {
                sndSetStationedSeVolume(10);
                break;
            }
            object->commandData->phase = 6;
            object->state = 3;
            btlSetActorCommandSelection((s32)object, 0, 0, 0);
            sndSetStationedSeVolume(8);
            return 1;
        case 1:
        case 5:
        default:
            return 0;
        }
    } else if (D_00324510.primaryCancel < 0) {
        if ((u32)(object->state - 1) < 2) {
            if (kind == 4) {
                btlSetActionActorPanelPresentation(object->owner, 1, 0);
            }
            if (btlCommandPanelWork->classIndex == 0) {
                entry = 0;
                cursor = 0;
            }
            btlCommandPanelWork->classIndex = 0;
            object->selections[0].entry = 0;
            object->selections[0].cursor = 0;
            sndSetStationedSeVolume(10);
        }
    } else if ((D_00324510.prev & 2) != 0) {
        if (entry > 0) {
            if (entry == cursor + 1 && cursor > 0) {
                cursor--;
            }
            entry--;
            sndSetStationedSeVolume(0);
        } else if (D_00324510.prev < 0) {
            entry = (s16)(count - 1);
            cursor = entry - 3;
            if (cursor < 0) {
                cursor = 0;
            } else {
                sndSetStationedSeVolume(0);
            }
        }
    } else if ((D_00324510.next & 2) != 0) {
        if (entry < count - 1) {
            entry++;
            if (entry == cursor + 3 && cursor < count - 4) {
                cursor++;
            }
            if (count >= 2) {
                sndSetStationedSeVolume(0);
            }
        } else if (D_00324510.next < 0) {
            entry = 0;
            cursor = 0;
            if (count >= 2) {
                sndSetStationedSeVolume(0);
            }
        }
    } else if (D_00324510.buttons[0] < 0) {
        if (kind == 4) {
            btlSetActionActorPanelPresentation(object->owner, 1, 0);
        }
        object->commandData->phase = 10;
        count = btlGetCommandOptionCount((s32)object, 0, 1);
        btlSetActorCommandSelection((s32)object, 10, (s16)(count - 1), 0);
        object->state = 3;
        sndSetStationedSeVolume(8);
        return 1;
    }
    object->selections[kind].entry = entry;
    object->selections[kind].cursor = cursor;
    return 0;
}

extern void btlDrawRetreatCommandLabel(s32);

extern void btlDrawItemCommandRows(BattleSceneObject *);

extern void func_001BDF60(BattleSceneObject *, u16, u16, u16);

void func_001BE590(BattleSceneObject *object);

void func_001BEB58(BattleSceneObject *object);

void btlDrawCommandClassRows(s32 sceneContext) {
    switch (btlGetActorCommandClass((BattleSceneObject *)sceneContext, btlCommandPanelWork->classIndex)) {
    case 0:
        func_001BDF60((BattleSceneObject *)sceneContext, 0, 2, 3);
        return;
    case 4:
        func_001BEB58((BattleSceneObject *)sceneContext);
        return;
    case 2:
        btlDrawItemCommandRows((BattleSceneObject *)sceneContext);
        return;
    case 3:
        func_001BE590(sceneContext);
        return;
    case 6:
        btlDrawRetreatCommandLabel(sceneContext);
    }
}

extern char *const D_003A2E60[3];
extern const char D_003BB438[];
extern const BattlePanelColors D_003A2E50;

void func_001BDF60(BattleSceneObject *ctx, u16 kind, u16 page, u16 columns) {
    char text[16];
    BattlePanelColors colors = D_003A2E50;
    char *labels[3];
    char back[5];
    u16 count;
    s32 hasUp;
    s16 highlight;
    s16 top;
    s32 row;
    s32 y = 0x14C;
    u16 *list;
    u32 color;
    struct FrFontGlyph *glyph;
    s32 failure;
    s32 cost;
    u32 iconColor;

    memcpy(labels, D_003A2E60, sizeof(labels));
    memcpy(back, D_003BB438, sizeof(back));
    btlGetRuntime();
    hasUp = 0;
    highlight = 0;
    top = 0;
    switch (kind) {
    case 0:
        top = ctx->selections[0].cursor;
        highlight = ctx->selections[0].entry;
        break;
    case 2:
        top = ctx->selections[5].cursor;
        highlight = ctx->selections[5].entry;
        break;
    case 3:
        top = ctx->selections[7].cursor;
        highlight = ctx->selections[7].entry;
        break;
    }
    itfSetTextDrawLimit(0x13);
    if (top == 0) {
        if (kind == 0) {
            BtlUnit *unit = ctx->owner->unit;

            color = (highlight == 0) ? 0x89FEFF80 : 0xA09DC380;
            color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[0];
            if (btlUnitStatusPair(unit) & 0x1400) {
                glyph = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color, (u8 *)labels[kind], 0);
            } else {
                glyph = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color,
                                                    btlGetIndexedUiResource(unit), 0);
            }
            btlSubmitSharedTextGlyph(glyph);
            hasUp = 1;
            y = 0x163;
        } else if (kind == 2) {
            color = (highlight == 0) ? 0x89FEFF80 : 0xA09DC380;
            color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[0];
            hasUp = 1;
            btlDrawIndexedBattleEntryGlyphs(0x1A, 0x14C, 0xFF0010, color, 0xE0);
            y = 0x163;
        }
    }
    list = btlGatherActorCommandsByKind((s32)ctx, (s16 *)&count, kind, page, columns);
    switch (kind) {
    case 0:
        D_003BD834->value[0] = count + 1;
        break;
    case 2:
        D_003BD834->value[5] = count + 1;
        break;
    case 3:
        D_003BD834->value[7] = count;
        break;
    }
    if (hasUp == 0 && kind != 3) {
        top--;
        highlight--;
    }
    list += top;
    top += hasUp;
    itfSetTextDrawLimit(0x13);
    for (row = hasUp; row < 4 && top <= count; row++, y += 0x17, list++, top++) {
        if (kind != 0 && kind != 2 && top >= count) {
            break;
        }
        if (kind == 0 && top == count + hasUp - 1) {
            itfSetTextDrawLimit(0x13);
            color = (highlight == top) ? 0x89FEFF80 : 0xA09DC380;
            color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[row];
            glyph = itfCreateConvertedTextGlyph(0x1A0, y << 3, 0xFF0010, color, (u8 *)back, 0);
            btlSubmitSharedTextGlyph(glyph);
            break;
        }
        failure = btlGetCommandFailureReason(ctx->owner->unit, *list);
        if (failure == 0) {
            color = (highlight == top) ? 0x89FEFF80 : 0xA09DC380;
        } else {
            color = 0x504F6140;
        }
        color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[row];
        btlDrawIndexedBattleEntryGlyphs(0x1A, y, 0xFF0010, color, *list);
        cost = btlApplyCommandAbilityMultiplier(&ctx->owner->unit->partyRecord, *list);
        if (cost != 0) {
            sprintf(text, D_003BB400, cost);
            fldSubmitSceneObjectAtCoordinates(0x86, y + 4, color, text);
            iconColor = failure ? color : (btlLinkedSelectionTaskBuffer->rowFade[row] | 0x80808000);
            colors.values[0] = iconColor;
            colors.values[1] = iconColor;
            colors.values[2] = iconColor;
            colors.values[3] = iconColor;
            switch (datCommandRecords[*list].costMode) {
            case DAT_COMMAND_COST_MODE_HP:
                effDrawSpriteWithCornerColors(0xA70, (y + 7) << 3, 0, colors.values, 0, btlResourceBlock->resA, 7, 0x53);
                break;
            case DAT_COMMAND_COST_MODE_MP:
                /* Separate MP/default draw arms are present at001BE4B0/001BE4E8. */
                effDrawSpriteWithCornerColors(0xA70, (y + 7) << 3, 0, colors.values, 0, btlResourceBlock->resA, 8, 0x53);
                break;
            default:
                effDrawSpriteWithCornerColors(0xA70, (y + 7) << 3, 0, colors.values, 0, btlResourceBlock->resA, 8, 0x53);
                break;
            }
        }
    }
    itfSetTextDrawLimit(-1);
}

void func_001BE590(BattleSceneObject *object) {
    extern u16 btlReadUnitStatusMask(DatPartyRecord *);
    extern const u32 D_003A2E70[3];
    extern const u32 D_003A2E80[3];
    typedef struct CommandLabelBank { const u8 *text[3]; } CommandLabelBank;
    CommandLabelBank revertLabels;
    CommandLabelBank transformLabels;
    BtlState *battle;
    s16 cursor;
    s32 selected;
    s32 total;
    s32 shown;
    CommandLabelBank *labels;
    s32 row = 0;

    memcpy(&revertLabels, D_003A2E70, sizeof(revertLabels));
    memcpy(&transformLabels, D_003A2E80, sizeof(transformLabels));
    battle = (BtlState *)btlGetRuntime();
    cursor = (s16)object->selections[3].cursor;
    selected = (s16)object->selections[3].entry;
    shown = (s32)btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
    total = (s32)btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 1);
    labels = (btlUnitStatusPair(object->owner->unit) & 0x1400) != 0
                 ? &revertLabels : &transformLabels;
    if (row < shown && cursor < total) {
        do {
            u32 color;
            s32 x;
            s32 y;
            struct FrFontGlyph *glyph;
            BattleSelectionWork *selection;

            switch (row) {
            case 0:
                if ((btlReadUnitStatusMask(&object->owner->unit->partyRecord) & 0x1000) != 0) {
                    color = 0x504F6140;
                } else {
                    color = selected == cursor ? 0x89FEFF80 : 0xA09DC380;
                }
                break;
            case 1: {
                DatGameState *state = datGameState;
                s32 count = state->partyCount;
                /* Count active members, then convert to available capacity. */
                s16 capacity = 0;
                if (count > 0) {
                    DatPartyRecord *party = state->party;
                    s32 remainingCount = count;
                    do {
                        if ((party->flags & 2) != 0) {
                            capacity++;
                        }
                        party++;
                        remainingCount--;
                    } while (remainingCount != 0);
                }
                capacity = (s16)((u16)state->partyCount - capacity);
                if ((datBattleSceneRecords[battle->battleMode].flags & 0x800) != 0) {
                    capacity = 0;
                }
                color = capacity != 0
                          ? (selected == cursor ? 0x89FEFF80 : 0xA09DC380)
                          : 0x504F6140;
                break;
            }
            case 2: /* Withdraw is the third entry of both label banks. */
            default:
                color = btlIsSceneActorLimitSatisfied((s32)object->owner->unit, 1) == 0
                          ? (selected == cursor ? 0x89FEFF80 : 0xA09DC380)
                          : 0x504F6140;
                break;
            }
            selection = btlLinkedSelectionTaskBuffer;
            x = selection->positions[row][0];
            y = selection->positions[row][1];
            color = (color & ~0xFFU) | (u32)selection->rowFade[row];
            row++;
            itfSetTextDrawLimit(0x13);
            glyph = itfCreateConvertedTextGlyph((x + 15) << 4, (y + 3) << 3,
                        0xFF0010, color, labels->text[cursor], 0);
            btlSubmitSharedTextGlyph(glyph);
            itfSetTextDrawLimit(-1);
            cursor++;
        } while (row < shown && cursor < total);
    }
}

extern const BattlePanelColors D_003A2E90;

extern char D_003BB478[];

extern s32 sprintf(char *, const char *, ...);

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
        effDrawSpriteWithCornerColors(0x900, countY << 3, 0, colors.values, 0,
                     btlResourceBlock->resA, 5, 0x53);
        sprintf(text, D_003BB478, D_00358FE0[index * 2 + 1]);
        fldSubmitSceneObjectAtCoordinates(0xA0, countY, color, text);
    }
}


void btlDrawRetreatCommandLabel(s32 unused) {
    u8 text[8];
    BtlState *battle;
    s32 color;
    struct FrFontGlyph *handle;

    memcpy(text, D_003BB450, sizeof(text));
    battle = (BtlState *)btlGetRuntime();
    if (datBattleSceneRecords[battle->battleMode].voice != 0) {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x504F6100;
    } else {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x89FEFF00;
    }
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color, text, 0);
    btlSubmitSharedTextGlyph(handle);
    itfSetTextDrawLimit(-1);
}

extern const BattlePanelColors D_003A2EA0;

extern s32 scrTestGlobalSeenBit(u16);

static inline void btlSetCornerColors(BattlePanelColors *colors, u32 color) {
    colors->values[0] = color;
    colors->values[1] = color;
    colors->values[2] = color;
    colors->values[3] = color;
}

void func_001BEB58(BattleSceneObject *object) {
    char text[16];
    BattlePanelColors colors = D_003A2EA0;
    u16 count;
    s32 fadeCount;
    s32 i = 0;
    s32 rowCount;
    s32 total;
    s32 selected;
    s32 y;
    BtlState *scene;
    BattleRosterTable *list;
    s32 row;
    s32 j;
    s32 blocked;
    s32 power;
    u32 textColor;
    u32 glyphColor;
    u32 color;
    u32 cost;

    scene = (BtlState *)btlGetRuntime();
    y = 0x14C;
    rowCount = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 0);
    total = btlGetCommandOptionCount((s32)object, btlCommandPanelWork->classIndex, 1);
    row = object->selections[4].cursor;
    selected = object->selections[4].entry;
    list = btlGetActorLinkageSkillList(object, &count);

    for (; i < rowCount && row < total; i++, row++) {
        blocked = func_001BF040(object->owner->unit, &list->entries[row]);
        if (blocked == 0) {
            glyphColor = selected == row ? 0x89FEFF80 : 0xA09DC380;
        } else {
            glyphColor = 0x504F6140;
        }
        textColor = (glyphColor & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[i];
        glyphColor = textColor;
        if (scrTestGlobalSeenBit(list->entries[row].skill) == 0 && blocked == 0 && selected != row) {
            glyphColor = btlLinkedSelectionTaskBuffer->rowFade[i] | 0xD1BA7100;
        }
        btlDrawIndexedBattleEntryGlyphs(0x1A, y, 0xFF0010, glyphColor, list->entries[row].skill);

        btlSetCornerColors(&colors, btlLinkedSelectionTaskBuffer->backdropFade | 0x80808000);
        effDrawSpriteWithCornerColors(0x7C0, y << 3, 0, colors.values, 0, btlResourceBlock->resA, 0x29, 0x53);

        power = btlGetCombinedPartyCommandPower(&object->owner->unit->partyRecord, object->owner->unit,
                                                list->entries[row].first, list->entries[row].second,
                                                list->entries[row].skill);
        sprintf(text, D_003BB400, power);
        fldSubmitSceneObjectAtCoordinates(0x86, y + 2, textColor, text);

        cost = btlGetLinkageSlotCost(list->entries[row].skill);
        if (fldCountSceneFadeKinds(scene, &fadeCount) >= cost) {
            btlSetCornerColors(&colors, btlLinkedSelectionTaskBuffer->rowFade[i] | 0x80808000);
        } else {
            color = textColor;
            btlSetCornerColors(&colors, color);
        }
        for (j = 0; j < cost; j++) {
            effDrawSpriteWithCornerColors(0x8F0 + j * 0x90, (y + 0xF) << 3, 0, colors.values, 0, btlResourceBlock->resA, 6, 0x53);
        }

        if (blocked == 0) {
            btlSetCornerColors(&colors, btlLinkedSelectionTaskBuffer->rowFade[i] | 0x80808000);
        } else {
            color = textColor;
            btlSetCornerColors(&colors, color);
        }
        switch (datCommandRecords[list->entries[row].skill].costMode) {
        case 1:
            effDrawSpriteWithCornerColors(0xA70, (y + 5) << 3, 0, colors.values, 0, btlResourceBlock->resA, 7, 0x53);
            break;
        case 2:
            effDrawSpriteWithCornerColors(0xA70, (y + 5) << 3, 0, colors.values, 0, btlResourceBlock->resA, 8, 0x53);
            break;
        default:
            effDrawSpriteWithCornerColors(0xA70, (y + 5) << 3, 0, colors.values, 0, btlResourceBlock->resA, 8, 0x53);
            break;
        }
        y += 0x17;
    }
}

/* First non-zero command failure reason: the actor itself, then each linked
 * partner in turn. All three checks run before the first result is tested. */
s32 func_001BF040(BtlUnit *actor, BattleRosterPair *pair) {
    s32 result;
    s32 first = 0;
    s32 second = 0;

    result = btlGetLinkedPartyCommandFailureReason(actor, actor, pair->first, pair->second, pair->skill);
    if (pair->first != NULL) {
        first = btlGetLinkedPartyCommandFailureReason(pair->first, pair->first, actor, pair->second, pair->skill);
    }
    if (pair->second != NULL) {
        second = btlGetLinkedPartyCommandFailureReason(pair->second, pair->second, actor, pair->first, pair->skill);
    }
    if (result != 0) {
        return result;
    }
    if (first != 0) {
        return first;
    }
    if (second != 0) {
        return second;
    }
    return 0;
}

extern s32 func_001B5CD8(void);

extern s32 func_001B60E8(void);

extern void func_001AADF8(BattleSceneObject *);

extern s32 func_001B4F10(KwlnTask *);

extern s32 func_001BD750(BattleSceneObject *);

extern void func_001B2D80(BattleSceneObject *, s32);

extern s32 btlHasHighPriorityState(void);

extern s32 btlAreLinkedSceneCountersAtThreshold(void);

static inline void btlUpdateSceneCommandSelection(BattleSceneObject *object) {
    btlDrawCommandClassRows((s32)object);
    func_001B2D80(object, btlGetActorCommandClass(object, btlCommandPanelWork->classIndex));
}

/* Drive the command-panel state machine; state five terminates its task. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E50);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E60);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E70);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E80);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2E90);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A2EA0);

s32 btlUpdateBattleSceneCommands(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleSceneObject *object = (BattleSceneObject *)dds3GetProcessData(task);
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
            panel = (BattleActorPanelWork *)dds3GetProcessData(dds3SearchProcessName(D_003BB3B0));
            BTL_COMMAND_PARTY_RECORD_INDEX(object->commandData) = panel->partyRecordIndex;
            object->state = 3;
            ((SceneAiWork *)dds3GetProcessData(dds3SearchProcessName(D_003BB3A0)))->state = 3;
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

void btlReleaseCommandPanelTask(KwlnTask *task) {
    u32 temp_v0;
    BtlState *battle;

    temp_v0 = dds3GetProcessData(task);
    sdfChipFreeDirect((void *)temp_v0);
    battle = (BtlState *)btlGetRuntime();
    battle->sceneObject = 0;
    btlReleaseBattleScratchBlocks();
}

extern BattleSceneObject *btlGetCommandPanelTaskWork(void);

extern s32 btlUpdateBattleSceneCommands(KwlnTask *task);

extern s32 btlUpdateCommandNoticeTask(KwlnTask *task);

extern s32 btlUpdateLinkageNoticeTask(KwlnTask *task);

extern void func_001B2AC8(BattleSceneObject *object);

extern u8 btlLinkageTutorialMessages[];

void fldInitializeSceneObject(BattleSceneObject *object, BtlTask *owner) {
    memset(object, 0, sizeof(*object));
    object->state = 1;
    object->commandData = &owner->indexWork;
    object->owner = owner;
}

BattleSceneObject *btlGetCommandPanelTaskWork(void) {
    KwlnTask *task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    if (task == NULL) {
        return NULL;
    }
    return (BattleSceneObject *)dds3GetProcessData(task);
}

s32 fldGetSceneObjectState(void) {
    KwlnTask *temp_v0;

    temp_v0 = dds3SearchProcessName(btlCommandPanelTaskNameRef);
    if (temp_v0 == 0) {
        return 0;
    }
    return btlGetCommandPanelTaskWork()->state;
}

void btlOpenBattleCommandPanel(BtlTask *task) {
    BtlState *scene;
    BattleSceneObject *object;
    u32 handle;

    if (dds3SearchProcessName(btlCommandPanelTaskNameRef) == 0) {
        scene = (BtlState *)btlGetRuntime();
        object = sdfAllocAndClearQuadwords(sizeof(*object));
        fldInitializeSceneObject(object, task);
        handle = (u32)dds3StartProcess(btlCommandPanelTaskNameRef, 0x2B0E, 1, 1,
                               btlUpdateBattleSceneCommands, btlReleaseCommandPanelTask, (u32)object);
        dds3LinkProcess(scene->scriptOwner, (KwlnTask *)handle);
        scene->sceneObject = handle;
        btlSetActionActorPanelPresentation(task, 0, 0);
        btlInitializeSelectionWork();
        btlInitializeCommandPanelSlotTables();
        btlCreateMessageWindow();
        func_001B2AC8(object);
        if ((task->unit->status.flags & 0x200) && !(scene->battleFlags & 0x1000000)) {
            if (mdlFlagTest(0x81B) == 0) {
                mdlFlagSet(0x81B);
                btlTrackedTaskHandles->status.bytes.blocked = 1;
                scene->battleFlags &= ~0x100000;
                handle = (u32)dds3StartProcess(D_003BB3D4, 0x2B0E, 1, 1,
                                       btlUpdateCommandNoticeTask, btlReleaseWindowTask,
                                       (u32)sdfAllocAndClearQuadwords(0x18));
                dds3LinkProcess(scene->scriptOwner, (KwlnTask *)handle);
                btlSetTrackedTaskHandle(0xD, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(btlLinkageTutorialMessages);
                dspStartEntry(2);
                btlSetCommandPanelFadeMode(0, 8);
            } else if (btlGetTaskState6() != 0) {
                scene->battleFlags &= ~0x100000;
                handle = (u32)dds3StartProcess(D_003BB3D0, 0x2B0E, 1, 1,
                                       btlUpdateLinkageNoticeTask, btlFinishTrackedBattleTaskAndCloseWindow,
                                       (u32)sdfAllocAndClearQuadwords(0x18));
                dds3LinkProcess(scene->scriptOwner, (KwlnTask *)handle);
                btlSetTrackedTaskHandle(0xC, handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(btlLinkageTutorialMessages);
                btlSetCommandPanelFadeMode(0, 8);
            } else {
                scene->battleFlags |= 0x100000;
            }
        }
    }
    object = btlGetCommandPanelTaskWork();
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

void btlRequestCommandPanelClose(void) {
    BattleSceneObject *object = btlGetCommandPanelTaskWork();
    if (object != 0) {
        BattleCmdPanel *panel = btlCommandPanelWork;
        object->state = 5;
        panel->state = 3;
    }
}

void btlGetTargetRowAnimationOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2] = {
        {{-7, 3}, {-6, 4}, {-5, 4}, {-4, 5}, {-3, 5}, {-2, 6}, {0, 0}, {0, 0}},
        {{0x22, 3}, {0x21, 4}, {0x20, 4}, {0x1F, 5}, {0x1E, 5}, {0x1D, 6}, {0, 0}, {0, 0}},
        {{0x11, 0x29}, {0x11, 0x28}, {0x11, 0x27}, {0x11, 0x26}, {0x11, 0x25}, {0x11, 0x24}, {0, 0}, {0, 0}},
    };

    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

/* Initialize or advance the three target-row indicators; mode remains unused. */
void btlAdvanceTargetRowAnimation(SceneAiWork *work, s32 mode) {
    s32 i;
    switch (work->animationPhase) {
    case 0:
        for (i = 0; i < 3; i++) {
            work->rowFade[i] = 128;
            work->rowScale[i] = 80.0f;
            work->rowPhase[i] = 0;
            btlGetTargetRowAnimationOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, 0);
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
                btlGetTargetRowAnimationOffset(&work->rowPosition[i][0], &work->rowPosition[i][1], i, work->rowStep[i]);
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

void btlAdvanceTargetPanelAnimation(SceneAiWork *work, s32 mode) {
    switch (work->animationPhase) {
    case 0:
        work->panelScale[0] = 90.0f;
        work->panelScale[1] = 80.0f;
        work->panelScale[2] = 90.0f;
        work->panelScale[3] = 80.0f;
        work->panelFade[1] = work->panelFade[3] = 128;
        work->animationPhase = 4;
        work->animationCounter = 20;
        break;
    case 1:
        work->panelFade[1] += 128;
        work->panelFade[1] = work->panelFade[1] <= 0 ? 0 : work->panelFade[1] > 128 ? 128 : work->panelFade[1];
        if (work->panelFade[1] >= 128) {
            work->animationPhase++;
        }
        break;
    case 2:
        work->panelFade[0] += 64;
        work->panelFade[0] = work->panelFade[0] <= 0 ? 0 : work->panelFade[0] > 255 ? 255 : work->panelFade[0];
        work->panelFade[2] += 64;
        work->panelFade[2] = work->panelFade[2] <= 0 ? 0 : work->panelFade[2] > 255 ? 255 : work->panelFade[2];
        if (work->panelFade[2] >= 255) {
            work->animationPhase++;
        }
        break;
    case 3:
        work->panelScale[0] += 3.0f;
        if (work->panelScale[0] >= 110.0f) {
            work->panelScale[0] = 110.0f;
        }
        work->panelScale[2] += 3.0f;
        if (work->panelScale[2] >= 110.0f) {
            work->panelScale[2] = 110.0f;
        }
        work->panelFade[0] -= 24;
        work->panelFade[0] = work->panelFade[0] <= 0 ? 0 : work->panelFade[0] > 255 ? 255 : work->panelFade[0];
        work->panelFade[2] -= 32;
        work->panelFade[2] = work->panelFade[2] <= 0 ? 0 : work->panelFade[2] > 255 ? 255 : work->panelFade[2];
        if (work->panelFade[0] <= 0) {
            work->animationPhase++;
        }
        break;
    case 4:
        work->animationCounter++;
        work->animationCounter = work->animationCounter <= 0 ? 0 : work->animationCounter > 20 ? 20 : work->animationCounter;
        if (work->animationCounter > 0) {
            work->animationPhase++;
            work->panelFade[0] = 192;
            work->panelFade[2] = 192;
            work->panelScale[0] = 90.0f;
            work->panelScale[2] = 90.0f;
            work->rowPhase[0] = 0;
            work->rowPhase[1] = 0;
            work->rowPhase[2] = 0;
        }
        break;
    case 5:
        work->panelFade[0] -= 8;
        work->panelFade[0] = work->panelFade[0] <= 0 ? 0 : work->panelFade[0] > 192 ? 192 : work->panelFade[0];
        work->panelFade[2] -= 6;
        work->panelFade[2] = work->panelFade[2] <= 0 ? 0 : work->panelFade[2] > 192 ? 192 : work->panelFade[2];
        work->panelScale[0] -= 2.0f;
        if (work->panelScale[0] <= 60.0f) {
            work->panelScale[0] = 60.0f;
        }
        work->panelScale[2] -= 2.0f;
        if (work->panelScale[2] <= 60.0f) {
            work->panelScale[2] = 60.0f;
        }
        if (work->panelFade[2] <= 0) {
            work->animationPhase = 4;
            work->panelScale[0] = 90.0f;
            work->panelScale[2] = 90.0f;
            work->animationCounter = 0;
        }
        break;
    }
}

extern s32 btlSetActorEffectParameter();
extern s32 btlProjectForwardPositionToScreen(s32 *);
extern void btlUnitGetMuzzlePosVU(BtlUnit *);

void func_001BFDE0(BtlUnit *unit, SceneAiWork *work, u32 index) {
    s32 position[4];
    s32 origin[5];
    f32 muzzle[4];
    s32 panelRows[] = {0, 0, 25, 0, 0, 24, 7, 7, 27, 7, 7, 26};
    s32 rowSlots[] = {-13, 0, 46, 38, 0, 48, 16, 46, 56};
    s32 echoSlots[] = {47, 49, 57};
    u32 colors[] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 i;
    s32 j;
    s32 n;

    btlGetRuntime();
    if (btlSetActorEffectParameter(unit, 0) == 0) {
        btlUnitGetMuzzlePosVU(unit);
    } else if ((unit->status.flags & 0x8000000) != 0) {
        btlUnitGetMuzzlePosVU(unit);
        VU0_STORE_VF_UNCLOBBERED(vf10, muzzle);
        muzzle[1] = -24.0f;
        VU0_LOAD_VF(vf10, muzzle);
    }
    if (btlProjectForwardPositionToScreen(position) == 0) {
        return;
    }
    origin[3] = 0x35;
    origin[4] = 0x35;
    origin[0] = position[0] - 0x1A;
    origin[1] = position[1] - 0x1A;
    origin[2] = 0xFFFFFFF;
    if (work->animationPhase < 6) {
    if (work->animationPhase >= 0) {
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                if (i == 1) {
                    colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, panelRows[5], j, work->panelFade[1]);
                } else {
                    colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, panelRows[i * 3 + 2], j, work->panelFade[i]);
                }
            }
            btlResourceBlock->resC->workEntries[panelRows[i * 3 + 2]].geometry.angleDegrees = work->panelAngleDegrees[i];
            func_001C2E90(btlResourceBlock->resC, panelRows[i * 3 + 2], &ax, &ay, work->panelScale[i]);
            effDrawSpriteWithCornerColors((origin[0] + ax + panelRows[i * 3]) << 4,
                         (origin[1] + ay + panelRows[i * 3 + 1]) << 3, -1, colors, 0,
                         btlResourceBlock->resC, panelRows[i * 3 + 2], 0x53);
            btlResourceBlock->resC->workEntries[panelRows[i * 3 + 2]].geometry.angleDegrees = 0;
            btlScaleEffectSlotSourceBounds(btlResourceBlock->resC, panelRows[i * 3 + 2]);
        }
        for (i = 0; i < 3; i++) {
            for (j = 0; j < 4; j++) {
                colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, rowSlots[i * 3 + 2], j, work->rowFade[i]);
            }
            func_001C2E90(btlResourceBlock->resC, rowSlots[i * 3 + 2], &ax, &ay, work->rowScale[i]);
            effDrawSpriteWithCornerColors((origin[0] + work->rowPosition[i][0]) << 4,
                         (origin[1] + work->rowPosition[i][1]) << 3, -1, colors, 0,
                         btlResourceBlock->resC, rowSlots[i * 3 + 2], 0x53);
            btlScaleEffectSlotSourceBounds(btlResourceBlock->resC, rowSlots[i * 3 + 2]);
            if (work->rowStep[i] >= 0) {
                switch (work->rowPhase[i]) {
                case 0:
                    for (n = 0; n < work->rowStep[i]; n++) {
                        for (j = 0; j < 4; j++) {
                            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, echoSlots[i], j,
                                                                 work->rowFade[i] - ((6 - n) << 4));
                        }
                        btlGetTargetRowAnimationOffset(&ax, &ay, i, work->rowStep[i] - n);
                        func_001C2E90(btlResourceBlock->resC, echoSlots[i], &bx, &by, work->rowScale[i]);
                        effDrawSpriteWithCornerColors((origin[0] + ax + bx) << 4,
                                     (origin[1] + ay + by) << 3, -1, colors, 0,
                                     btlResourceBlock->resC, echoSlots[i], 0x53);
                        btlScaleEffectSlotSourceBounds(btlResourceBlock->resC, echoSlots[i]);
                    }
                    break;
                case 1:
                    for (n = 0; n < work->rowStep[i]; n++) {
                        for (j = 0; j < 4; j++) {
                            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, echoSlots[i], j,
                                                                 work->rowFade[i] - ((6 - n) << 4));
                        }
                        btlGetTargetRowAnimationOffset(&ax, &ay, i, 5 - n);
                        func_001C2E90(btlResourceBlock->resC, echoSlots[i], &bx, &by, work->rowScale[i]);
                        effDrawSpriteWithCornerColors((origin[0] + ax) << 4,
                                     (origin[1] + ay) << 3, -1, colors, 0,
                                     btlResourceBlock->resC, echoSlots[i], 0x53);
                        btlScaleEffectSlotSourceBounds(btlResourceBlock->resC, echoSlots[i]);
                    }
                    break;
                default:
                    for (j = 0; j < 4; j++) {
                        colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resC, echoSlots[i], j, work->rowFade[i] + 0x40);
                    }
                    btlGetTargetRowAnimationOffset(&ax, &ay, i, 5);
                    func_001C2E90(btlResourceBlock->resC, echoSlots[i], &bx, &by, work->rowScale[i]);
                    effDrawSpriteWithCornerColors((origin[0] + ax) << 4,
                                 (origin[1] + ay) << 3, -1, colors, 0,
                                 btlResourceBlock->resC, echoSlots[i], 0x53);
                    btlScaleEffectSlotSourceBounds(btlResourceBlock->resC, echoSlots[i]);
                    break;
                }
            }
        }
    }
    }
}

/* Step through the fixed 3x3 target-ID grid, skipping missing IDs; return whether the ID changed. */

s32 btlStepTargetGridSelection(s32 direction, SceneAiWork *work) {
    s32 ids[9] = {0x13E, 0xFFFF, 0x13D, 0x140, 0xFFFF, 0x13F, 0x141, 0x11B, 0x142};
    s32 count;
    s32 oldId;
    s32 targetId;
    s32 index;
    s32 missing = 1;
    BtlUnit *unit;

    count = (s32)btlGetIndexListCount(work->listA);
    oldId = ids[(s16)work->cursor.position.row * 3 + (s16)work->cursor.position.column];

    do {
        switch (direction) {
        case 1: {
            if (work->cursor.position.row == 2 && work->cursor.position.column == 1) {
                work->cursor.position.row = 0;
                work->cursor.position.column = 0;
                direction = 4;
            } else {
                work->cursor.position.row--;
                if ((s16)work->cursor.position.row < 0) work->cursor.position.row = 2;
            }
            break;
        }
        case 2: {
            if (work->cursor.position.row == 2 && work->cursor.position.column == 1) {
                work->cursor.position.row = 0;
                work->cursor.position.column = 2;
                direction = 3;
            } else {
                work->cursor.position.row++;
                if ((s16)work->cursor.position.row >= 3) work->cursor.position.row = 0;
            }
            break;
        }
        case 3:
            work->cursor.position.column--;
            if ((s16)work->cursor.position.column < 0) {
                work->cursor.position.column = 2;
                work->cursor.position.row--;
                if ((s16)work->cursor.position.row < 0) work->cursor.position.row = 2;
            }
            break;
        case 4:
            work->cursor.position.column++;
            if ((s16)work->cursor.position.column >= 3) {
                work->cursor.position.column = 0;
                work->cursor.position.row++;
                if ((s16)work->cursor.position.row >= 3) work->cursor.position.row = 0;
            }
            break;
        }

        targetId = ids[(s16)work->cursor.position.row * 3 + (s16)work->cursor.position.column];
        missing = 1;
        if (targetId != 0xFFFF) {
            index = 0;
            for (;;) {
                missing = 1;
                if (index >= count) goto retryScan;
                unit = btlGetIndexListEntry(work->listA, index);
                missing = unit->partyRecord.unitId != targetId;
                if (missing == 0) {
                    break;
                }
                index++;
            }
            {
                s32 changed = oldId != targetId;
                work->entry = index;
                return changed;
            }
        }
retryScan:
        ;
    } while (missing == 1);
    return 0;
}

extern s16 D_003BB494;
extern s16 D_003BB496;
extern s16 D_003BB498;

/* Process target confirm/cancel and mode-selected directional input. */
void btlUpdateTargetSelectionInput(BtlState *battle, SceneAiWork *work, s32 mode) {
    BattleSceneObject *object;
    KwlnTask *task;
    BtlUnit *unit;
    s32 oldEntry;
    s32 selection = 0;
    u32 count;

    if (D_00324510.confirm < 0) {
        task = dds3SearchProcessName(btlCommandPanelTaskNameRef);
        object = (BattleSceneObject *)dds3GetProcessData(task);
        if (object->state == 8) {
            work->state = 5;
            object->state = 9;
            object->commandData->linkedUnit =
                (BtlUnit *)btlGetIndexListEntry(work->listA, work->entry);
            btlSetActorCommandSelection((s32)object, 0, 0, 0);
            sndSetStationedSeVolume(9);
            return;
        }
        work->state = 3;
        if (btlGetActorCommandClass(object, btlCommandPanelWork->classIndex) == 0) {
            btlSetActorCommandSelection((s32)object, D_003BB494, D_003BB496, D_003BB498);
        }
        return;
    }

    if (D_00324510.primaryCancel < 0) {
        work->state = 4;
        sndSetStationedSeVolume(10);
        if (fldGetSceneObjectState() == 8) {
            return;
        }
        unit = (BtlUnit *)work->listA->entries[0];
        if ((unit->status.flags & 0x200) != 0) {
            task = dds3SearchProcessName(D_003BB3B0);
            if (task != NULL) {
                btlUpdateActorSlotStates((u8 *)dds3GetProcessData(task), 0);
            }
        }
        if (btlHasRegisteredAphNamePanelTask() == 0) {
            return;
        }
        task = (KwlnTask *)btlGetTrackedTaskHandle(0);
        dds3KillProcess(task, 0);
        return;
    }

    if (work->result != 0) {
        return;
    }

    oldEntry = work->entry;
    count = btlGetIndexListCount(work->listA);

    switch (mode) {
    case 0:
        if ((D_00324510.alternateNext & 2) != 0) selection = 1;
        else if ((D_00324510.alternatePrev & 2) != 0) selection = 2;
        break;
    case 1:
        if ((D_00324510.alternatePrev & 2) != 0) selection = 1;
        else if ((D_00324510.alternateNext & 2) != 0) selection = 2;
        break;
    case 2:
        if (((u8)D_00324510.prev & 2) != 0) selection = 1;
        else if (((u8)D_00324510.next & 2) != 0) selection = 2;
        break;
    case 3:
        if (((u8)D_00324510.next & 2) != 0) selection = 1;
        else if (((u8)D_00324510.prev & 2) != 0) selection = 2;
        break;
    case 4:
        if (battle->battleMode == 0x10E) {
            if (((u8)D_00324510.next & 2) != 0) selection = 1;
            else if (((u8)D_00324510.prev & 2) != 0) selection = 2;
            else if ((D_00324510.alternateNext & 2) != 0) selection = 3;
            else if ((D_00324510.alternatePrev & 2) != 0) selection = 4;
        } else {
            if (((u8)D_00324510.prev & 2) != 0 || (D_00324510.alternateNext & 2) != 0) {
                selection = 1;
            } else if (((u8)D_00324510.next & 2) != 0 || (D_00324510.alternatePrev & 2) != 0) {
                selection = 2;
            }
        }
        break;
    case 5:
        if (battle->battleMode == 0x10E) {
            if (((u8)D_00324510.prev & 2) != 0) selection = 1;
            else if (((u8)D_00324510.next & 2) != 0) selection = 2;
            else if ((D_00324510.alternatePrev & 2) != 0) selection = 3;
            else if ((D_00324510.alternateNext & 2) != 0) selection = 4;
        } else {
            if (((u8)D_00324510.next & 2) != 0 || (D_00324510.alternatePrev & 2) != 0) {
                selection = 1;
            } else if (((u8)D_00324510.prev & 2) != 0 || (D_00324510.alternateNext & 2) != 0) {
                selection = 2;
            }
        }
        break;
    }

    if (battle->battleMode == 0x10E && selection != 0 && mode >= 4) {
        if (btlStepTargetGridSelection(selection, work) != 0) {
            sndSetStationedSeVolume(1);
        }
        goto update_list;
    }

    if (selection == 1) {
        if (work->entry != 0) work->entry--;
        else work->entry = count - 1;
        if (count >= 2) sndSetStationedSeVolume(1);
    }
    if (selection == 2) {
        if ((u32)work->entry < count - 1) work->entry++;
        else work->entry = 0;
        if (count >= 2) sndSetStationedSeVolume(1);
    }

update_list:
    if (work->entry == oldEntry) {
        return;
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        task = (KwlnTask *)btlGetTrackedTaskHandle(0);
        dds3KillProcess(task, 0);
    }
    btlClearIndexList(work->listB);
    unit = (BtlUnit *)btlGetIndexListEntry(work->listA, work->entry);
    btlAppendIndexListEntry(work->listB, unit);
}


extern const u8 *D_003BAA70;
extern const u8 *D_003BAA74;
extern const u8 *D_003BAA88;
extern void btlAdvanceTargetPanelAnimation(SceneAiWork *work, s32 mode);
extern void func_001BFDE0(BtlUnit *unit, SceneAiWork *work, u32 index);
extern s32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit);
extern void btlDrawCenteredPanelSegments(s32 width);

/* Draw selected actor names and advance the battle scene's actor panels. */
void btlUpdateAndDrawTargetActorPanels(SceneAiWork *work) {
    BtlUnit *unit;
    struct FrFontGlyph *glyph;
    struct FrFontGlyph *categoryGlyph;
    s32 width;
    s32 categoryWidth;
    s32 totalWidth;
    u32 count;
    u32 i;
    s32 scene;
    KwlnTask *panelTask = NULL;

    count = btlGetIndexListCount(work->listB);
    if (count == 1 && btlHasRegisteredAphNamePanelTask() == 0 && work->state != 4) {
        unit = btlGetIndexListEntry(work->listB, 0);
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                glyph = itfCreateConvertedTextGlyph(0x640, 0x200, 0, 0xA09DC380,
                    D_003BAA70 + unit->partyRecord.unitId * 17, 0);
                width = frFontMeasureLines(glyph);
                btlDrawCenteredPanelSegments(width);
                frFontSetGlyphPosition(glyph, (0x100 - (width >> 1)) << 4, 0x220);
                frFontDrawGlyphChain(glyph, 1, 0x53);
                frDestroyFontRequestAfter(glyph);
            } else {
                glyph = itfCreateConvertedTextGlyph(0x640, 0x200, 0, 0xA09DC380,
                    D_003BAA74 + unit->partyRecord.unitId * 17, 0);
                width = frFontMeasureLines(glyph);
                categoryGlyph = itfCreateConvertedTextGlyph(0x640, 0x200, 0, 0xA09DC380,
                    D_003BAA88 + datEnemyRecords[unit->partyRecord.unitId].pad04 * 7, 0);
                categoryWidth = frFontMeasureLines(categoryGlyph);
                totalWidth = width + categoryWidth;
                btlDrawCenteredPanelSegments(totalWidth);
                frFontSetGlyphPosition(categoryGlyph, (0x100 - (totalWidth >> 1)) << 4, 0x220);
                frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)categoryGlyph, 1);
                frDestroyFontRequestAfter(categoryGlyph);
                if (categoryWidth == 0) {
                    s32 centeredNameX = -(width >> 1);
                    frFontSetGlyphPosition(glyph, (centeredNameX + 0x100) << 4, 0x220);
                } else {
                    frFontSetGlyphPosition(glyph, (categoryWidth - (width >> 1) + 0xF8) << 4, 0x220);
                }
                frFontDrawGlyphChain(glyph, 1, 0x53);
                frDestroyFontRequestAfter(glyph);
            }
        }
    }
    scene = fldGetSceneObjectState();
    if (work->state != 3 && scene != 8) {
        unit = btlGetIndexListEntry(work->listB, 0);
        if (unit->status.flags & 0x200) {
            panelTask = dds3SearchProcessName(D_003BB3B0);
            if (panelTask != NULL) {
                btlUpdateActorSlotStates((u8 *)dds3GetProcessData(panelTask), 0);
            }
        }
    }
    btlAdvanceTargetRowAnimation(work, 0);
    btlAdvanceTargetPanelAnimation(work, 0);
    for (i = 0; i < count; i++) {
        unit = btlGetIndexListEntry(work->listB, i);
        if (btlIsUnitInfoFlagOneEligible(unit) == 0) {
            func_001BFDE0(unit, work, i);
        }
        if ((u32)(work->state - 3) >= 2 && scene != 8 && scene != 9 &&
            (unit->status.flags & 0x200) && panelTask != NULL) {
            btlSetActorSecondaryPresentation(unit, 0, 1);
        }
    }
}

void btlDrawCenteredPanelSegments(s32 width) {
    u32 color[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 half = width / 2;
    s32 x = width - half + 0x105;
    effDrawSpriteWithCornerColors(x * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    effDrawSpriteWithCornerColors((0x100 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    effDrawSpriteWithCornerColors((0x92 - half) * 0x10, 0x200, 0, color, 0,
                  btlResourceBlock->resC, 0x15, 0x53);
}

s32 btlUpdateTargetSelectionTask(KwlnTask *handle) {
    BtlState *work = (BtlState *)btlGetRuntime();
    SceneAiWork *state;
    s32 mode;
    s32 i;
    s32 count;
    BtlIndexList *owner;
    if (work->battleFlags & 0x04000000) {
        return 0;
    }
    state = (SceneAiWork *)dds3GetProcessData(handle);
    switch (state->state) {
    case 1:
        state->state = 2;
        break;
    case 2:
        owner = state->listA;
        mode = 1;
        switch (work->battleMode) {
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
        btlUpdateTargetSelectionInput(work, state, mode);
        btlUpdateAndDrawTargetActorPanels(state);
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

extern s32 btlBuildEligibleTargetIndexList(void *source, BtlIndexList *list, s32, s32, s32);

extern u32 btlSelectCommandTargetPolicy(void *source, BtlIndexList *list, s32);

extern s32 btlStepTargetGridSelection(s32, SceneAiWork *);

SceneAiWork *btlCreateTargetSelectionWork(BtlTask *source) {
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
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(sizeof(SceneAiWork));
    object = (BattleSceneObject *)dds3GetProcessData(
        dds3SearchProcessName(btlCommandPanelTaskNameRef));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (BattleActorPanelWork *)dds3GetProcessData(
            dds3SearchProcessName(D_003BB3B0));
        count = btlCountFlaggedSceneActors();
        if (count < 2 &&
            (datGameState->party[other->partyRecordIndex].status & 0x4800)) {
            btlBuildEligibleTargetIndexList(source, work->listA, 1, 4, -0x4801);
        } else {
            btlBuildEligibleTargetIndexList(source, work->listA, 1, 4, -1);
        }
        btlGetIndexListCount(work->listA);
        work->result = 0;
    } else if (source != 0) {
        work->result = btlSelectCommandTargetPolicy((void *)source, work->listA, 0);
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
    btlStepTargetGridSelection(0, work);
    btlClearIndexList(work->listB);
    btlAppendIndexListEntry(work->listB,
                            btlGetIndexListEntry(work->listA, work->entry));
    return work;
}

void btlReleaseTargetSelectionWork(SceneAiWork *work) {
    btlFreeIndexList(work->listB);
    btlFreeIndexList(work->listA);
    sdfChipFreeDirect(work);
}

void btlReleaseTargetSelectionTask(KwlnTask *task) {
    SceneAiWork *temp_v0;
    s32 temp_v1;

    temp_v0 = (SceneAiWork *)dds3GetProcessData(task);
    btlReleaseTargetSelectionWork(temp_v0);
    temp_v1 = (s32)(u32)btlGetRuntime();
    *(u32 *)(temp_v1 + 0x2ac) = 0;
}

SceneAiWork *btlGetTargetSelectionWork(void) {
    KwlnTask *task = dds3SearchProcessName(D_003BB3A0);
    if (task == 0) {
        return 0;
    }
    return (SceneAiWork *)dds3GetProcessData(task);
}

s32 btlGetTargetSelectionState(void) {
    return btlGetTargetSelectionWork()->state;
}

BtlIndexList *btlGetSelectedTargetList(void) {
    return btlGetTargetSelectionWork()->listB;
}

extern s32 btlUpdateTargetSelectionTask(KwlnTask *);

extern SceneAiWork *btlCreateTargetSelectionWork(BtlTask *);

void btlCreateTargetSelectionTask(s32 sourceTask) {
    BtlState *scene;
    s32 task;
    dds3SearchProcessName(D_003BB3A0);
    if (btlIsNamedBattleTaskRegistered() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0xA), 0);
    }
    if (btlHasRegisteredGuidePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(9), 0);
    }
    if (btlHasRegisteredSkillNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(1), 0);
    }
    if (btlHasRegisteredAphNamePanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(0), 0);
    }
    scene = (BtlState *)btlGetRuntime();
    task = (s32)dds3StartProcess(D_003BB3A0, 0x2B0E, 1, 1, btlUpdateTargetSelectionTask, btlReleaseTargetSelectionTask,
                          (u32)btlCreateTargetSelectionWork((BtlTask *)sourceTask));
    dds3LinkProcess(scene->scriptOwner, (KwlnTask *)task);
    scene->spriteObject = task;
}

void btlRequestTargetSelectionClose(void) {
    SceneAiWork *work;

    work = btlGetTargetSelectionWork();
    if (work != 0) {
        work->state = 6;
    }
}

/* Request the selected slot-order transition in the enabled current bank. */
void btlRequestActorSlotOrderTransition(s32 index, s8 operation) {
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

typedef struct BtlActorSlotLimits { s32 values[12]; } BtlActorSlotLimits;
extern const BtlActorSlotLimits D_003A30A8;
extern const BtlActorSlotLimits D_003A30D8;
extern void btlUpdateActorSlotOrderColors(s32, s32);

/* Advance enabled slot-order states, scales, fades and secondary corner colors. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A30A8);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A30D8);

void btlUpdateActorSlotOrderAnimation(void) {
    BtlActorSlotLimits right = D_003A30A8;
    BtlActorSlotLimits left = D_003A30D8;
    s32 *limits;
    s32 status;
    s32 side;
    s32 count;
    s32 i;

    status = btlGetNamedTaskPairStatusOrUnavailable();
    side = D_003BD83C->bank;
    if (D_003BD83C->enabled[side] == 0) {
        return;
    }
    if (status != 0 && status != 3) {
        return;
    }
    count = D_003BD83C->currentIndex;
    for (i = 0; i < count; i++) {
        switch (D_003BD840[side]->state[i]) {
        case 1:
            D_003BD840[side]->timer++;
            D_003BD840[side]->timer = D_003BD840[side]->timer <= 0 ? 0 : D_003BD840[side]->timer >= 35 ? 34 : D_003BD840[side]->timer;
            if (D_003BD840[side]->timer >= 34) {
                limits = side == 0 ? left.values : right.values;
                if (--D_003BD840[side]->entries[i] < limits[0]) {
                    D_003BD840[side]->entries[i] = limits[2];
                }
                D_003BD840[side]->slotFade[i] += 0x20;
                D_003BD840[side]->slotFade[i] = D_003BD840[side]->slotFade[i] <= 0 ? 0 : D_003BD840[side]->slotFade[i] >= 0x81 ? 0x80 : D_003BD840[side]->slotFade[i];
                D_003BD840[side]->rotationAngleDegrees[i] -= 5.0f;
                if (D_003BD840[side]->rotationAngleDegrees[i] <= 0.0f) {
                    D_003BD840[side]->rotationAngleDegrees[i] = 0.0f;
                }
                D_003BD840[side]->drawOffsetScalePercent[i] = D_003BD840[side]->drawOffsetScalePercent[i] - 5.0f;
                if (D_003BD840[side]->drawOffsetScalePercent[i] <= 100.0f) {
                    D_003BD840[side]->drawOffsetScalePercent[i] = 100;
                }
                if (D_003BD840[side]->slotFade[i] >= 0x80) {
                    if (D_003BD840[side]->state[i + 1] < 3) {
                        D_003BD840[side]->state[i + 1] = 1;
                    }
                    D_003BD840[side]->state[i]++;
                    D_003BD840[side]->phaseFrames[i] = 0;
                    D_003BD840[side]->entries[i] = limits[i];
                    D_003BD840[side]->secondaryState[i] = 1;
                    D_003BD840[side]->colorAdjustments[i][0] = 0;
                    D_003BD840[side]->colorAdjustments[i][1] = 0;
                    D_003BD840[side]->colorAdjustments[i][2] = 0;
                    D_003BD840[side]->colorAdjustments[i][3] = 0;
                }
            }
            break;
        case 2:
            D_003BD840[side]->rotationAngleDegrees[i] -= 5.0f;
            if (D_003BD840[side]->rotationAngleDegrees[i] <= 0.0f) {
                D_003BD840[side]->rotationAngleDegrees[i] = 0.0f;
            }
            D_003BD840[side]->drawOffsetScalePercent[i] = D_003BD840[side]->drawOffsetScalePercent[i] - 5.0f;
            if (D_003BD840[side]->drawOffsetScalePercent[i] <= 100.0f) {
                D_003BD840[side]->drawOffsetScalePercent[i] = 100;
            }
            btlUpdateActorSlotOrderColors(side, i);
            break;
        case 3:
            D_003BD840[side]->scalePercent[i][0] = 120.0f;
            D_003BD840[side]->slotValues[i][0] = 0xC0;
            D_003BD840[side]->unk1B4[i][0] = 0;
            D_003BD840[side]->phaseFrames[i] = 0;
            D_003BD840[side]->slotFade[i] = 0x80;
            D_003BD840[side]->state[i]++;
            D_003BD840[side]->scalePercent[i][1] = 120.0f;
            D_003BD840[side]->slotValues[i][1] = 0x80;
            D_003BD840[side]->rotationAngleDegrees[i] = 0.0f;
            D_003BD840[side]->unk1B4[i][1] = 0;
            D_003BD840[side]->drawOffsetScalePercent[i] = 100;
            break;
        case 4:
            D_003BD840[side]->scalePercent[i][0] -= 1.0f;
            if (D_003BD840[side]->scalePercent[i][0] <= 90.0f) {
                D_003BD840[side]->scalePercent[i][0] = 90.0f;
            }
            D_003BD840[side]->phaseFrames[i]++;
            D_003BD840[side]->phaseFrames[i] = D_003BD840[side]->phaseFrames[i] <= 0 ? 0 : D_003BD840[side]->phaseFrames[i] >= 0xB ? 0xA : D_003BD840[side]->phaseFrames[i];
            if (D_003BD840[side]->phaseFrames[i] >= 9) {
                D_003BD840[side]->state[i]++;
                D_003BD840[side]->scalePercent[i][0] = 120.0f;
                D_003BD840[side]->slotValues[i][0] = 0x10;
                D_003BD840[side]->unk1B4[i][0] = 0;
            }
            break;
        case 5:
            D_003BD840[side]->scalePercent[i][1] -= 10.0f;
            if (D_003BD840[side]->scalePercent[i][1] <= 100.0f) {
                D_003BD840[side]->scalePercent[i][1] = 45.0f;
                D_003BD840[side]->slotValues[i][1] = 0x68;
                D_003BD840[side]->state[i]++;
            }
            break;
        case 6:
            D_003BD840[side]->scalePercent[i][0] -= 1.0f;
            if (D_003BD840[side]->scalePercent[i][0] <= 100.0f) {
                D_003BD840[side]->scalePercent[i][0] = 165.0f;
                D_003BD840[side]->slotValues[i][0] = 0x10;
            }
            D_003BD840[side]->slotValues[i][0] = (s32)(165.0f - D_003BD840[side]->scalePercent[i][0]) / 8 + 0x10;
            D_003BD840[side]->slotValues[i][0] = D_003BD840[side]->slotValues[i][0] <= 0 ? 0 : D_003BD840[side]->slotValues[i][0] >= 0x19 ? 0x18 : D_003BD840[side]->slotValues[i][0];
            D_003BD840[side]->scalePercent[i][1] += 10.0f;
            if (D_003BD840[side]->scalePercent[i][1] >= 100.0f) {
                D_003BD840[side]->slotValues[i][1] = 0x18;
            }
            if (D_003BD840[side]->scalePercent[i][1] >= 130.0f) {
                D_003BD840[side]->scalePercent[i][1] = 45.0f;
                D_003BD840[side]->slotValues[i][1] = 0x68;
            }
            break;
        case 7:
            D_003BD840[side]->slotFade[i] -= 0x10;
            D_003BD840[side]->slotFade[i] = D_003BD840[side]->slotFade[i] <= 0 ? 0 : D_003BD840[side]->slotFade[i] >= 0x81 ? 0x80 : D_003BD840[side]->slotFade[i];
            if (D_003BD840[side]->slotFade[i] <= 0) {
                D_003BD840[side]->state[i] = 0;
            }
            break;
        }
    }
}

typedef s32 SceneSlotCoordinate[3];

extern const BattlePanelColors D_003A3130;
extern const SceneSlotCoordinate D_003A3140[8];
extern const SceneSlotCoordinate D_003A31A0[8];
extern void btlDrawActorSlotOrderSecondarySprite(EffectSlotSet *, s32, s32, s32, s32, s32, s32);

/* Draw enabled bank-selected slot-order sprites with their current colors and scales. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3130);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3140);

INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A31A0);

void btlDrawActorSlotOrder(void) {
    u32 colors[4];
    SceneSlotCoordinate bankOneCoordinates[8];
    SceneSlotCoordinate bankZeroCoordinates[8];
    s32 slotCount;
    const SceneSlotCoordinate *coordinates;
    s32 bank;
    s32 index;
    s32 channel;
    s32 xOffset;
    s32 yOffset;

    memcpy(colors, D_003A3130.values, sizeof(colors));
    memcpy(bankOneCoordinates, D_003A3140, sizeof(bankOneCoordinates));
    memcpy(bankZeroCoordinates, D_003A31A0, sizeof(bankZeroCoordinates));

    xOffset = 0;
    yOffset = 0;
    bank = D_003BD83C->bank;
    if (D_003BD83C->enabled[bank] == 0) {
        return;
    }
    coordinates = bank == 0 ? bankZeroCoordinates : bankOneCoordinates;
    slotCount = D_003BD83C->currentIndex;

    for (index = 0; index < slotCount; index++) {
        switch (D_003BD840[bank]->state[index]) {
        case 1:
        case 2:
        case 3:
        case 7: {
            for (channel = 0; channel < 4; channel++) {
                s32 adjustment;
                u32 replicated;

                colors[channel] = btlSetSlotLowByteClamped(
                    btlResourceBlock->resC, coordinates[index][2], channel,
                    D_003BD840[bank]->slotFade[index] +
                    D_003BD840[bank]->colorAdjustments[index][channel]);
                adjustment = D_003BD840[bank]->colorAdjustments[index][channel];
                replicated = (u32)adjustment;

                {
                    u32 red = (replicated << 24) | 0x80000000;
                    u32 green = (replicated << 16) | 0x00800000;
                    u32 blue = (replicated << 8) | 0x00008000;

                    colors[channel] |= (red | green) | blue;
                }
            }

            btlResourceBlock->resC->workEntries[coordinates[index][2]]
                .geometry.angleDegrees = D_003BD840[bank]->rotationAngleDegrees[index];
            func_001C2E90(btlResourceBlock->resC, coordinates[index][2],
                          &xOffset, &yOffset,
                          (f32)D_003BD840[bank]->drawOffsetScalePercent[index]);
            effDrawSpriteWithCornerColors((coordinates[index][0] + xOffset) << 4,
                          (coordinates[index][1] + yOffset) << 3,
                          0, colors, 0, btlResourceBlock->resC,
                          coordinates[index][2], 0x53);
            btlResourceBlock->resC->workEntries[coordinates[index][2]]
                .geometry.angleDegrees = 0.0f;
            btlScaleEffectSlotSourceBounds(btlResourceBlock->resC,
                                         coordinates[index][2]);

            if (D_003BD840[bank]->rotationAngleDegrees[index] > 0.0f) {
                for (channel = 0; channel < 4; channel++) {
                    colors[channel] = btlSetSlotLowByteClamped(
                        btlResourceBlock->resC, coordinates[index][2],
                        channel,
                        D_003BD840[bank]->slotFade[index] >> 1);
                }
                effDrawSpriteWithCornerColors(coordinates[index][0] << 4,
                              coordinates[index][1] << 3,
                              0, colors, 0, btlResourceBlock->resC,
                              D_003BD840[bank]->entries[index], 0x53);
            }
            break;
        }
        case 4:
        case 5:
            btlDrawActorSlotOrderSecondarySprite(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 0);
            btlDrawActorSlotOrderSecondarySprite(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 1);
            break;
        case 6:
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(
                    btlResourceBlock->resC, coordinates[index][2],
                    channel, D_003BD840[bank]->slotFade[index]);
            }
            effDrawSpriteWithCornerColors(coordinates[index][0] << 4,
                          coordinates[index][1] << 3,
                          0, colors, 0, btlResourceBlock->resC,
                          coordinates[index][2], 0x53);
            btlDrawActorSlotOrderSecondarySprite(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 0);
            btlDrawActorSlotOrderSecondarySprite(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 1);
            break;
        }
    }
}

extern const u32 D_003A3220[4];

/* Draw one bank-selected secondary actor slot, rereading offsets after scaling. */
void btlDrawActorSlotOrderSecondarySprite(EffectSlotSet *work, s32 entry, s32 x, s32 y,
                   s32 bank, s32 row, s32 variant) {
    u32 colors[4];
    s32 channel;

    memcpy(colors, D_003A3220, sizeof(colors));
    for (channel = 0; channel < 4; channel++) {
        colors[channel] = btlSetSlotLowByteClamped(work, entry, channel,
                            D_003BD840[bank]->slotValues[row][variant]);
    }
    {
        ActorSlotOrder **selectedBank = &D_003BD840[bank];
        ActorSlotOrder *owner = *selectedBank;
        f32 scale = owner->scalePercent[row][variant];
        ActorSlotOrder *scaled;
        s32 drawX;
        s32 drawY;

        func_001C2E90(work, entry, &owner->drawOffsets[row][variant][0],
                      &owner->drawOffsets[row][variant][1], scale);
        scaled = *selectedBank;
        drawX = scaled->drawOffsets[row][variant][0];
        drawY = scaled->drawOffsets[row][variant][1];
        effDrawSpriteWithCornerColors((x + drawX) << 4, (y + drawY) << 3,
                      0, colors, 0, work, entry, 0x53);
    }
    btlScaleEffectSlotSourceBounds(work, entry);
}

/* Advance one slot-order secondary state and its four corner-color adjustments. */
INCLUDE_RODATA(const s32, "game/code_001A9780", D_003A3220);

void btlUpdateActorSlotOrderColors(s32 bank, s32 index) {
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

void btlScaleEffectSlotSourceBounds(EffectSlotSet *work, s32 index) {
    work->workEntries[index].geometry.bounds[2] = work->workEntries[index].sourceWidth << 4;
    work->workEntries[index].geometry.bounds[3] = work->workEntries[index].sourceHeight << 3;
}

void func_001C2E90(EffectSlotSet *work, s32 entry, s32 *offsetX, s32 *offsetY, f32 percent) {
    f32 scale = percent / 100.0f;
    s32 width = work->workEntries[entry].geometry.bounds[2];
    s32 height = work->workEntries[entry].geometry.bounds[3];
    s32 x;
    s32 y;

    if (scale >= 1.0f) {
        x = -((f32)(width >> 4) * (scale - 1.0f) * 0.5f);
    } else {
        x = (f32)(width >> 4) * (1.0f - scale) * 0.5f;
    }
    width = (f32)width * scale;
    if (scale >= 1.0f) {
        y = -((f32)(height >> 3) * (scale - 1.0f) * 0.5f);
    } else {
        y = (f32)(height >> 3) * (1.0f - scale) * 0.5f;
    }
    work->workEntries[entry].geometry.bounds[3] = (f32)height * scale;
    work->workEntries[entry].geometry.bounds[2] = width;
    *offsetX = x;
    *offsetY = y;
}

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

/* Step the current bank's slot fades when the task pair permits; slots share a timer. */
void btlUpdateBankedSlotFadeSequence(void) {
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

void btlDrawSceneSlotFades(void) {
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
            effDrawSpriteWithCornerColors(425 << 4, 21 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(392 << 4, 33 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 18, 0x53);
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 19, channel,
                D_003BD83C->fade[4][bank]);
                if (D_003BD83C->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            effDrawSpriteWithCornerColors(349 << 4, 21 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(298 << 4, 21 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(206 << 4, 33 << 3, 0, colors, 0,
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
        effDrawSpriteWithCornerColors(440 << 4, 6 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(395 << 4, 21 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(362 << 4, 21 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(302 << 4, 46 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(302 << 4, 48 << 3, 0, colors, 0,
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
            effDrawSpriteWithCornerColors(269 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 15, 0x53);
        }
    }
}

extern const s32 D_003A32F0[10][3];

void btlDrawSceneSlotGlyphGrid(void) {
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
        effDrawSpriteWithCornerColors(rows[row][0] << 4, rows[row][1] << 3, 0, colors,
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
        effDrawSpriteWithCornerColors(finalRows[last][0] << 4, finalRows[last][1] << 3,
                      0, colors, 0, btlResourceBlock->resC, finalRows[last][2], 0x53);
    }
}

void fldInitSceneFadeRecords(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    BtlSceneSlot *slot = scene->slots;
    BtlSceneFadingRecord *record = scene->fading;
    u32 i = 0;

    while (i < 8 && slot->group != 0) {
        memcpy(&record->slot, slot, sizeof(BtlSceneSlot));
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

s32 btlFindSceneSlotById(BtlSceneSlot *entry) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlSceneSlot *slot = context->slots;
    u32 i;
    for (i = 0; i < 8; i++, slot++) {
        if (slot->id == entry->id) {
            return i;
        }
    }
    return -1;
}

BtlSceneSlot *btlFindSceneSlotRecord(BtlSceneSlot *entry) {
    BtlState *context = (BtlState *)btlGetRuntime();
    s32 index = btlFindSceneSlotById(entry);
    BtlSceneSlot *record = 0;

    if (index != -1) {
        record = &context->slots[index];
    }
    return record;
}

s32 btlFadeStaleSceneSlots(s32 lastFadeIndex) {
    BtlState *context = (BtlState *)btlGetRuntime();
    BtlSceneFadingRecord *scene = context->fading;
    BtlSceneSlot *slot = context->slots;
    u32 i;
    s32 changed = 0;
    for (i = 0; i < 8; i++, slot++, scene++) {
        if (scene->slot.id != slot->id && btlFindSceneSlotRecord(&scene->slot) == 0) {
            if (scene->alpha != 0) {
                scene->alpha -= 8;
                changed = 1;
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BtlState *scene, s32 *outFadeCount) {
    BtlSceneFadingRecord *record = scene->fading;
    BtlSceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    BattleTrackedTaskWork *global;

    while (fadeCount < 8 && record[fadeCount].slot.group != 0) {
        if (record[fadeCount].slot.group == 1) {
            countA++;
        }
        if (record[fadeCount].slot.group == 2) {
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
    while (slotCount < 8 && slot->group != 0) {
        slotCount++;
        slot++;
    }
    *outFadeCount = fadeCount;
    return slotCount;
}

extern const char *D_003BB3B0;
void func_001C4278(void)
{
  BtlState *scene = (BtlState *) btlGetRuntime();
  BtlSceneFadingRecord *records;
  BtlSceneFadingRecord *fade;
  BtlSceneFadingRecord *record;
  BattleActorPanelWork *panel;
  s32 lastIndex = 0;
  s32 i;
  s32 count;
  u32 state;
  s32 fading;
  fldCountSceneFadeKinds(scene, &lastIndex);
  fading = btlFadeStaleSceneSlots(lastIndex);
  if (fading == 0)
  {
    s32 currentScene;

    fade = scene->fading;
    fldInitSceneFadeRecords();
    lastIndex = 0;
    if (fade[0].slot.group != 0)
    {
      do
      {
        lastIndex++;
      }
      while ((u32) lastIndex < 8 && fade[lastIndex].slot.group != 0);
    }
    /* Native 001C4304 snapshots scene kind before publishing the decremented
     * last index at 001C4314. Keep that read distinct from the count update. */
    currentScene = scene->currentScene;
    lastIndex--;
    if (currentScene == 7)
    {
      panel = (BattleActorPanelWork *) dds3GetProcessData(dds3SearchProcessName(D_003BB3B0));
      btlRetireActorSlotPresentationChannels(panel, 0);
      btlAdvancePendingSceneSlotStates((u8 *) panel);
      btlUpdateActorSlotStates((u8 *) panel, 0);
    }
  }
  records = scene->fading;
  fldSetSceneSlotRange(lastIndex + 1);
  btlUpdateBankedSlotFadeSequence();
  btlDrawSceneSlotFades();
  btlDrawSceneSlotGlyphGrid();
  count = lastIndex;
  for (i = 0; i <= count; i++)
  {
    record = &records[count - i];
    state = record->alpha;
    if (state == 128)
    {
      if (record->slot.remaining != 100)
      {
        btlRequestActorSlotOrderTransition(i, 0);
      }
    }
    if (record->alpha < 64)
    {
      btlRequestActorSlotOrderTransition(i, 1);
    }
  }
  btlUpdateActorSlotOrderAnimation();
  btlDrawActorSlotOrder();
}

s32 btlUpdateScenePanelTask(KwlnTask *task) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    if ((scene->battleFlags & 0x200) == 0) {
        return 0;
    }
    if (btlTrackedTaskHandles->presentationState == 1 && btlTrackedTaskHandles->status.bytes.blocked == 0) {
        return 0;
    }
    func_001C4278();
    return 0;
}

void btlClearScenePanelTaskHandle(KwlnTask *task) {
    BtlState *scene;

    scene = (BtlState *)btlGetRuntime();
    scene->cleanupTask = 0;
}

void btlBeginScenePanelFadeIn(void) {
    BtlState *scene = (BtlState *)btlGetRuntime();
    btlSetCommandPanelFadeMode(1, 8);
    scene->battleFlags |= 0x200;
}

void btlBeginScenePanelFadeOut(void) {
    btlGetRuntime();
    btlSetCommandPanelFadeMode(0, 8);
}

void btlLoadStartupPanelResources(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 index) {
    s32 temp_v0;

    temp_v0 = (s32)(u32)btlGetRuntime();
    return *(u32 *)(index * 4 + temp_v0 + 0x4b0);
}

void func_001C4540(void) {
}

extern const char *D_003BB488;

extern s32 btlUpdateScenePanelTask(KwlnTask *);

void fldCreateSceneCleanupTask(void) {
    KwlnTask *oldTask = dds3SearchProcessName(D_003BB488);
    BtlState *context;
    u32 task;
    if (oldTask == 0) {
        btlGetRuntime();
    }
    context = (BtlState *)btlGetRuntime();
    task = (u32)dds3StartProcess(D_003BB488, 0x2B0E, 1, 1, btlUpdateScenePanelTask, btlClearScenePanelTaskHandle, 0);
    dds3LinkProcess(context->scriptOwner, (KwlnTask *)task);
    context->cleanupTask = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    btlInitializeStaffActorPanels();
    func_001B6498(1);
    btlBeginScenePanelFadeIn();
}

void btlAllocatePanelTaskWork(void) {
    /* A one-pass loop: its notes stop sched2 from hoisting the slot-order
     * address above this call, where retail keeps it after the call. */
    do {
        btlGetRuntime();
    } while (0);
    btlTrackedTaskHandles = sdfAllocAndClearQuadwords(0x58);
    memset(btlTrackedTaskHandles, 0, 0x38);
    D_003BD83C = sdfAllocAndClearQuadwords(0xD8);
    D_003BD840[0] = sdfAllocAndClearQuadwords(0x274);
    D_003BD840[1] = sdfAllocAndClearQuadwords(0x274);
}

static inline void btlDestroyTrackedTaskIfPresent(s32 slot) {
    if (btlGetTrackedTaskHandle(slot) != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(slot), 0);
    }
}

void fldDestroySceneTasksAndBuffers(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();

    if (battle->sceneObject != 0) {
        dds3KillProcess((KwlnTask *)battle->sceneObject, 0);
        battle->sceneObject = 0;
    }
    if (battle->spriteObject != 0) {
        dds3KillProcess((KwlnTask *)battle->spriteObject, 0);
        battle->spriteObject = 0;
    }
    if (battle->cleanupTask != 0) {
        dds3KillProcess((KwlnTask *)battle->cleanupTask, 0);
        battle->cleanupTask = 0;
    }
    if (btlHasRegisteredPsechgPanelTask() != 0) {
        dds3KillProcess((KwlnTask *)btlGetTrackedTaskHandle(6), 0);
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
    sdfChipFreeDirect(D_003BD840[1]);
    sdfChipFreeDirect(D_003BD840[0]);
    sdfChipFreeDirect(D_003BD83C);
    sdfChipFreeDirect(btlTrackedTaskHandles);
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


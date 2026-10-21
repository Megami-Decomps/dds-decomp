#include "sdk_ps2.h"
/*
 * Translation unit boundary recovered at btlCalculateHpActionAmount (twin of
 * DDS1 code_001A5C40). btlCheckSpecialAbility and btlGetActorEntryMultiplier
 * belong to the preceding unit (code_001A8748); the retail calls to them keep
 * their possibly-throwing (non-nothrow) shape, so they must stay external
 * here. A same-unit definition makes reorg's branch-slot liveness scan see
 * through the call and flips the delay-slot annulment of the neighbouring
 * branches (bnezl vs bnez, bne vs bnel).
 */
#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "btl_actor_status_table_api.h"
#include "itf_debug_panel_work.h"
#include "itf_color_panel_row.h"
#include "btl_boss_data_api.h"
#include "btl_event_asset_api.h"
#include "btl_skill_max_api.h"
#include "kwln_task_add_child.h"
#include "btl_special_ability.h"
#include "dat_unit_has_skill_api.h"
#include "btl_command_partner_output.h"
#include "btl_enc_bg_entry.h"
#include "btl_animation_mode_entry.h"
#include "btl_effect_position.h"
#include "evt_solar.h"
#include "eff_random_mod.h"
#include "eff_random.h"
#include "sdf_trig.h"
#include "snd_pad.h"
#include "btl_item_drop.h"
#include "itf_mes_window.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "kwln_task_priority_query.h"
#include "kwln_task_create.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf.h"
#include "itf_panel_draw.h"
#include "itf_panel_api.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "btl_resolve_unit_value_api.h"
#include "btl_select_transform_parameters_api.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "mdl_resource_table.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"
#include "btl_target_result.h"

extern s32 btlGetCommandUseError(BtlUnit *, s32);

/* Named and indexed views of the same four signed 32-bit color channels. */
typedef union UiQuadColor {
    struct {
        s32 red;
        s32 green;
        s32 blue;
        s32 alpha;
    };
    s32 channels[4];
} UiQuadColor;

extern s32 dds3FindEntryIndex();

extern DatPartyRecord *btlGetIndexedPartyEntryRecord(s32);


extern const char *D_003B4CF8[];

extern u32 sndTestMessageResourceIndex;

extern SdfTex *sndTestMessageTexture;


extern u64 D_004366E8;

extern s32 btlRuntime;

extern u8 *D_00435DE0;

extern u8 *D_00435DF0;

extern u8 *D_00435DE4;

extern u8 *D_00435DF8;
extern s8 D_0037F530[];

extern u8 btlSharedActorTransformParameters[];

extern u8 *D_00435E00;

extern void *btlPartyUnitGunTypeNames[];

extern EncBgRow *D_00435E14;

extern s32 fldConsumeNextSceneRequest(s32 *outCode, s32 *outParameter);

typedef struct BattleController {
    u8 pad_000[0x214];
    s32 frame;
    u32 flags;
    u32 flags21C;
    u8 pad_220[0x28];
    ActionStateLink *taskHead; /* 0x248 */
    BtlUnit *actors;
    u8 pad_250[0x1E];
    u8 mode; /* 0x26E: mode 3 scales defeat experience in DDS2. */
    u8 pad26F;
    u16 variant; /* 0x270 */
    u8 pad_272[0x22];
    s32 adjustmentRecordIndex; /* 0x294 */
    s32 adjustmentGroupIndex;  /* 0x298 */
    s32 adjustmentEntryIndex;  /* 0x29C */
    u8 pad_2A0[0x24];
    KwlnTask *drawTask;
    u8 pad2C8[0x14];
    BattleItemDrop itemDrops[3];
    s32 moneyEarned; /* 0x2E8 */
    u32 rewardMacca; /* 0x2EC */
    s32 experienceEarned; /* 0x2F0 */
    s32 epEarned; /* 0x2F4 */
    s32 totalAp; /* 0x2F8: defeat EP accumulated for the result screen. */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;
typedef char BattleControllerTotalApOffsetCheck[
    ((u32)&((BattleController *)0)->totalAp == 0x2F8) ? 1 : -1];
typedef char BattleControllerDrawTaskOffsetCheck[
    ((u32)&((BattleController *)0)->drawTask == 0x2C4) ? 1 : -1];

extern s32 datCalculateCommandBaseValue(DatPartyRecord *, s32);

extern DatEnemyRecord *datEnemyRecords;

extern char D_00415158[];

extern char D_004150B0[]; /* "btl:hunt ep=%d[id=%X]\n" */

extern void btlBossDebugPrintf(const char *, ...);

extern EffRandState effSharedRandomState[];

extern void *btlCommandAttributeNames[];

extern f32 btlActiveGroupCountMultipliers[];



extern u16 mnuGetPartyEntryCurrentId(DatPartyRecord *);

extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern s32 evtCheckValueThreshold(s32 itemId, s32 minimumQuantity);


extern s32 sndUpdateTestMsgTask(KwlnTask *task);

extern SdfPoolNode D_003805A8;

extern u8 D_003B4D08[];

extern u8 D_003B4D18[];

extern u8 D_003B4D28[];

#include "sdf_packet_alloc.h"

extern void itfSendTablePacket(SdfListHead *list, s32 context, s32 mode);

extern void itfQueueTextureBoundQuadPacket(void *, void *, void *, s32, SdfTex *, s32, SdfListHead *);

/* Option IDs index the same signed-byte bank used by the named controls. */
extern SndPad D_0037F510;

extern SdfPoolNode kwlnDrawSurfaces[];

#define BTL_ENTRY_STATUS_MASK 0x7FFF

extern u32 datComputeSkillBoostedMaxHp(DatPartyRecord *);

extern u32 datComputeSkillBoostedMaxMp(DatPartyRecord *);

extern SdfPoolNode D_00380708;


extern s32 D_00435CBC;


extern void itfPrintTestMessageCallback();

extern s32 itfUpdateTestMessageResourceInput(KwlnTask *task);

extern u8 D_003B4A28[];

extern s32 scrCreateTaskForProcessId();

void sndCycleTestMessageResource(void);

/* Functions defined earlier in the previous translation unit. */
void *btlGetRuntime(void);
s8 btlGetActorIndexedSignedValue(BtlUnit *object, s32 index);
s32 btlCheckSpecialAbility(DatPartyRecord *record, s32 ability);
s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *partyRecord, u32 entryId);
f32 btlGetActorEntryMultiplier(BtlUnit *unit, u32 index, s8 includeCharge);
f32 btlGetElementAbilityMultiplier(BtlUnit *unit, s32 attr);
f32 btlGetLowHpAbilityMultiplier(BtlUnit *unit);
f32 btlGetClampedBattleTableValue(BtlUnit *source);
extern u8 *datRosterDetails;
extern s32 evtRunContext(s32, s32, s32, s32, u16);


s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record);
s32 btlGetAbilityAttributeMultiplierPercent(BtlUnit *actor, s32 attr);
extern s32 sdfCalculateCommandVitalDelta(s32, s32, DatPartyRecord *, u32, u8);
s32 btlCalculateHpActionAmount(BtlUnit *source, BtlUnit *target, s32 command,
                               u32 maxHits, s32 actionKind, s32 side,
                               u32 calculationFlags) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    SdfBattleParameters *parameters;
    f32 attack;
    f32 defense = 1.0f;
    f32 status;
    f32 automatic;
    f32 solar;
    f32 (*scaleHook)(BtlUnit *, BtlUnit *, u32, s32);
    s8 charge = !(datCommandRecords[command].flags & 1);
    u32 attribute;
    s32 base;
    s32 amount;
    s32 phase;
    s32 limit;

    switch (datCommandRecords[command].effectType) {
    case 0:
        calculationFlags &= 1;
        attack = calculationFlags == 0
            ? btlGetActorEntryMultiplier(source, 0, charge) : 1.0f;
        if (side == 1) {
            defense = btlGetActorEntryMultiplier(target, 3, charge);
        }
        status = 0.5f;
        switch (source->partyRecord.status & 0x7FFF) {
        case 1:
        case 0x80:
            break;
        case 0x1000:
            status = 0.1f;
            break;
        default:
            status = 1.0f;
            break;
        }
        break;
    case 1:
        if (side == 1 && datCommandRecords[command].hpType != 8 &&
            datCommandRecords[command].hpType != 10) {
            calculationFlags &= 1;
            attack = 1.0f;
            if (calculationFlags == 0) {
                attack = btlGetActorEntryMultiplier(source, 1, charge);
            }
            defense = btlGetActorEntryMultiplier(target, 3, charge);
        } else {
            defense = 1.0f;
            calculationFlags &= 1;
            attack = 1.0f;
        }
        status = 1.0f;
        break;
    case 2:
        return 0;
    default:
        defense = 0.0f;
        calculationFlags &= 1;
        attack = 0.0f;
        status = 0.0f;
        break;
    }

    if (side == 1 && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10 &&
        (target->partyRecord.status & 0x7FFF) == 0x100) {
        status *= 1.5f;
    }
    attribute = btlGetActorIndexedSignedValue(source, command);
    automatic = calculationFlags == 0
        ? btlGetElementAbilityMultiplier(source, attribute) : 1.0f;
    switch ((u32)command) {
    case 0:
    case 0x94:
    case 0x95:
    case 0x96:
        automatic *= btlGetLowHpAbilityMultiplier(source);
        break;
    }
    if (source->status.flags & 0x400) {
        if (btlGetEntryFlagsUnlessDisabled(&source->partyRecord) & 0x10000) {
            if (attribute < 2 ||
                (attribute == 7 && datCommandRecords[command].effectType == 0)) {
                automatic *= 3.0f;
            }
        }
    }
    if (actionKind == 2 && btlCheckSpecialAbility(&source->partyRecord, 0x229)) {
        defense *= datAbilityParameters[0x229 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (side == 1 && attribute != -1 &&
        datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        automatic *= (f32)btlGetAbilityAttributeMultiplierPercent(target, attribute) / 100.0f;
    }
    if (side == 0 && calculationFlags == 0 && btlCheckSpecialAbility(&source->partyRecord, 0x259)) {
        automatic *= datAbilityParameters[0x259 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (datCommandRecords[command].unk30 == 0x13 &&
        (target->partyRecord.status & 0x2000)) {
        status += status;
    }

    solar = 1.0f;
    if (side == 1 && attribute != -1 &&
        !(datBattleSceneRecords[battle->battleMode].flags & 1)) {
        switch (datCommandRecords[command].hpType) {
        case 1:
        case 6:
        case 12:
        case 14:
            parameters = datBattleParameters;
            phase = evtGetMirroredSolarPhase();
            solar = parameters->solarScale[
                (source->status.flags & 0x200) ? 0 : 1][phase];
            break;
        }
    }
    if (side == 1) {
        if (btlDoesEnabledStatusMatchCurrentId(&target->partyRecord, 0xDB) &&
            !(target->status.flags & 0x1000) && !(target->partyRecord.flags & 0x10) &&
            datCommandRecords[command].hpType != 8 &&
            datCommandRecords[command].hpType != 10) {
            automatic *= 0.7f;
        }
    }
    switch ((u32)command) {
    case 0:
    case 0x94:
    case 0x95:
    case 0x96:
        if ((source->partyRecord.status & 0x400) &&
            btlDoesEnabledStatusMatchCurrentId(&source->partyRecord, 0xE5)) {
            automatic += automatic;
        }
        break;
    }
    if (side == 1 && !calculationFlags &&
        btlDoesEnabledStatusMatchCurrentId(&source->partyRecord, 0xE7) &&
        datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        f32 varied = ((sdfGetRandFloat(effSharedRandomState) - 0.5f) * 2.0f) *
                     0.5f * automatic + automatic;
        automatic = varied;
    }
    if (side == 1 && !calculationFlags &&
        btlDoesEnabledStatusMatchCurrentId(&source->partyRecord, 0xE3) &&
        datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        automatic *= btlGetClampedBattleTableValue(source);
    }
    if (side == 0 && !calculationFlags && btlDoesEnabledStatusMatchCurrentId(&target->partyRecord, 0xE6)) {
        if (datCommandRecords[command].hpType != 11 ||
            datCommandRecords[command].hpPower != 100) {
            automatic += automatic;
        }
    }
    if (side == 1 && btlDoesEnabledStatusMatchCurrentId(&target->partyRecord, 0xEF) &&
        attribute == 7 && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        automatic *= 0.7f;
    }
    scaleHook = battle->commandAmountScaleHook;
    if (scaleHook != NULL) {
        automatic *= scaleHook(source, target, command, side);
    }
    if (datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10 && mdlFlagTest(0x80E) &&
        (source->status.flags & 0x400) && (target->status.flags & 0x200)) {
        attack *= datBattleParameters->unkB64;
    }
    if (side == 1 && !calculationFlags && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        if ((source->partyRecord.flags & 0x10) && (source->status.flags & 0x200)) {
            defense *= datBattleParameters->unkB90;
        }
        if ((target->partyRecord.flags & 0x10) && (target->status.flags & 0x200)) {
            defense *= datBattleParameters->unkB94;
        }
    }
    btlBossDebugPrintf("btl:hp[atk=%.2f,def=%.2f,bat=%.2f,auto=%.2f,sn=%.2f]\n",
                       attack, defense, status, automatic, solar);
    amount = sdfCalculateCommandVitalDelta(command, (s32)&source->partyRecord,
                                           &target->partyRecord, maxHits, 1);
    base = amount;
    amount = (s32)((f32)base * attack * defense * status * automatic * solar);
    if (!(target->status.flags & 0x10) && actionKind == 2 && amount != 0) {
        amount = (s32)((f32)amount *
                       (f32)evtRunContext(6, (s32)&source->partyRecord, 0, 0, 0) /
                       100.0f);
    }
    if ((target->status.flags & 0x10) && side == 1) {
        amount = (s32)((f32)amount + (f32)amount);
        btlBossDebugPrintf("btl:hp[back]\n");
    }
    limit = datCommandRecords[command].stat36;
    if (amount < -limit) {
        amount = -(limit * ((s32)sdfGetRandInt(0, 11) + 95)) / 100;
    } else if (amount > limit) {
        amount = limit * ((s32)sdfGetRandInt(0, 11) + 95) / 100;
    }
    if (amount == 0 && base != 0) {
        amount = base < 0 ? -1 : 1;
        btlBossDebugPrintf("btl:hp limit 1\n");
    }
    switch (datCommandRecords[command].hpType) {
    case 3:
    case 4:
    case 5:
    case 13:
        amount = base;
        break;
    }
    if (datCommandRecords[command].hpType == 12 && amount < 0 &&
        target->partyRecord.hp < -amount) {
        amount = -(s32)target->partyRecord.hp;
    }
    switch (side) {
    case 0:
        return amount > 0 ? amount : -1;
    case 1:
        return amount < 0 ? -amount : -1;
    default:
        return -1;
    }
}

s32 btlCalculateMpActionAmount(BtlUnit *source, BtlUnit *target, s32 command,
                               u32 maxHits, s32 actionKind, s32 side,
                               u32 calculationFlags) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    SdfBattleParameters *parameters;
    f32 attack;
    f32 defense = 1.0f;
    f32 status;
    f32 automatic;
    f32 solar;
    f32 (*scaleHook)(BtlUnit *, BtlUnit *, u32, s32);
    s8 charge = !(datCommandRecords[command].flags & 1);
    s32 attribute;
    s32 base;
    s32 amount;
    s32 phase;
    u32 flags;
    s32 limit;

    switch (datCommandRecords[command].effectType) {
    case 0:
        flags = calculationFlags & 1;
        attack = flags == 0
            ? btlGetActorEntryMultiplier(source, 0, charge) : 1.0f;
        if (side == 1) {
            defense = btlGetActorEntryMultiplier(target, 3, charge);
        }
        status = 0.5f;
        switch (source->partyRecord.status & 0x7FFF) {
        case 1:
        case 0x80:
            break;
        case 0x1000:
            status = 0.1f;
            break;
        default:
            status = 1.0f;
            break;
        }
        break;
    case 1:
        if (side == 1 && datCommandRecords[command].mpType != 8 &&
            datCommandRecords[command].mpType != 10) {
            flags = calculationFlags & 1;
            attack = 1.0f;
            if (flags == 0) {
                attack = btlGetActorEntryMultiplier(source, 1, charge);
            }
            defense = btlGetActorEntryMultiplier(target, 3, charge);
        } else {
            defense = 1.0f;
            flags = calculationFlags & 1;
            attack = 1.0f;
        }
        status = 1.0f;
        break;
    case 2:
        return 0;
    default:
        defense = 0.0f;
        flags = calculationFlags & 1;
        attack = 0.0f;
        status = 0.0f;
        break;
    }

    if (side == 1 && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10 &&
        (target->partyRecord.status & 0x7FFF) == 0x100) {
        status *= 1.5f;
    }
    attribute = btlGetActorIndexedSignedValue(source, command);
    automatic = flags == 0
        ? btlGetElementAbilityMultiplier(source, attribute) : 1.0f;
    switch ((u32)command) {
    case 0:
    case 0x94:
    case 0x95:
    case 0x96:
        automatic *= btlGetLowHpAbilityMultiplier(source);
        break;
    }

    if (actionKind == 2 && btlCheckSpecialAbility(&source->partyRecord, 0x229)) {
        defense *= datAbilityParameters[0x229 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (datCommandRecords[command].unk30 == 0x13 &&
        (target->partyRecord.status & 0x2000)) {
        status += status;
    }

    solar = 1.0f;
    if (side == 1 && attribute != -1) {
        switch (datCommandRecords[command].mpType) {
        case 1:
        case 6:
        case 12:
        case 14:
            parameters = datBattleParameters;
            phase = evtGetMirroredSolarPhase();
            solar = parameters->solarScale[
                (source->status.flags & 0x200) ? 0 : 1][phase];
            break;
        }
    }
    if (side == 1 && btlDoesEnabledStatusMatchCurrentId(&target->partyRecord, 0xDB) &&
        !(target->status.flags & 0x1000) && !(target->partyRecord.flags & 0x10) &&
        datCommandRecords[command].mpType != 8 &&
        datCommandRecords[command].mpType != 10) {
        automatic *= 0.7f;
    }
    if (side == 1 && !flags &&
        btlDoesEnabledStatusMatchCurrentId(&source->partyRecord, 0xE3) &&
        datCommandRecords[command].mpType != 8 &&
        datCommandRecords[command].mpType != 10) {
        automatic *= btlGetClampedBattleTableValue(source);
    }
    scaleHook = battle->commandAmountScaleHook;
    if (scaleHook != NULL) {
        automatic *= scaleHook(source, target, command, side);
    }
    if (datCommandRecords[command].mpType != 8 &&
        datCommandRecords[command].mpType != 10 && mdlFlagTest(0x80E) &&
        (source->status.flags & 0x400) && (target->status.flags & 0x200)) {
        attack *= datBattleParameters->unkB64;
    }
    if (side == 1 && !flags && datCommandRecords[command].hpType != 8 &&
        datCommandRecords[command].hpType != 10) {
        if ((source->partyRecord.flags & 0x10) && (source->status.flags & 0x200)) {
            defense *= datBattleParameters->unkB90;
        }
        if ((target->partyRecord.flags & 0x10) && (target->status.flags & 0x200)) {
            defense *= datBattleParameters->unkB94;
        }
    }
    btlBossDebugPrintf("btl:mp[atk=%.2f,def=%.2f,bat=%.2f,auto=%.2f,sn=%.2f]\n",
                       attack, defense, status, automatic, solar);
    amount = sdfCalculateCommandVitalDelta(command, (s32)&source->partyRecord,
                                           &target->partyRecord, maxHits, 2);
    base = amount;
    amount = (s32)((f32)base * attack * defense * status * automatic * solar);
    if (actionKind == 2 && amount != 0) {
        amount = (s32)((f32)amount *
                       (f32)evtRunContext(6, (s32)&source->partyRecord, 0, 0, 0) /
                       100.0f);
    }
    if ((target->status.flags & 0x10) && side == 1) {
        amount = (s32)((f32)amount + (f32)amount);
        btlBossDebugPrintf("btl:mp[back]\n");
    }
    limit = datCommandRecords[command].stat36;
    if (amount < -limit) {
        amount = -(limit * ((s32)sdfGetRandInt(0, 11) + 95)) / 100;
    } else if (amount > limit) {
        amount = limit * ((s32)sdfGetRandInt(0, 11) + 95) / 100;
    }
    if (amount == 0 && base != 0) {
        amount = base < 0 ? -1 : 1;
        btlBossDebugPrintf("btl:mp limit 1\n");
    }
    switch (datCommandRecords[command].mpType) {
    case 4:
    case 5:
    case 13:
        amount = base;
        break;
    }
    switch (side) {
    case 0:
        return amount > 0 ? amount : -1;
    case 1:
        if (target->partyRecord.mp < -amount) {
            amount = -(s32)target->partyRecord.mp;
        }
        return amount < 0 ? -amount : -1;
    default:
        return -1;
    }
}


s32 btlSumOtherTargetHitAmounts(u8 *action) {
    BtlTargetResult *result = *(BtlTargetResult **)(action + 0x88);
    u32 count = btlGetIndexListCount(*(BtlIndexList **)(action + 0x60));
    u32 i;
    u32 j;
    s32 total = 0;

    for (i = 0; i < count; i++, result++) {
        if (result->skipped != 0 || result->blocked != 0) {
            continue;
        }
        switch (result->kind) {
        case 2:
        case 4:
        case 0x10000:
        case 0x20000:
        case 0x40000:
            break;
        default:
            if (*(BtlUnit **)(action + 0x18) !=
                btlGetIndexListEntry(*(BtlIndexList **)(action + 0x60), i)) {
                for (j = 0; j < result->hitCount; j++) {
                    total += result->hits[j].amount;
                }
            }
            break;
        }
    }
    return total;
}

s32 btlTestActorStatusPredicate(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = *(s32 (**)(BtlUnit *))((u8 *)btlGetRuntime() + 0x698);
    if (hook != 0) {
        if (hook(unit) != 0) {
            return 1;
        }
    }
    return (unit->partyRecord.status & 0x2806) != 0;
}

s32 btlComputeStatusPenaltyFifth(BtlUnit *unit) {
    u16 status;
    s32 amount;

    status = unit->partyRecord.status & 0x7fff;
    amount = 0;
    if ((status == 0x80) || (status == 0x400)) {
        amount = (s32)-(u32)unit->partyRecord.maxHp / 5;
    }
    return amount;
}

extern char D_00415440[]; /* "btl:fear ratio[%d]\n" */

extern s32 btlRollAiBucket();

s32 btlRollFearChance(s32 unused, BtlUnit *unit, s32 flagsA, s32 flagsB) {
    s32 threshold;
    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    if (unit->status.stateFlags & 8) {
        return 0;
    }
    if (!(flagsA & 1)) {
        return 0;
    }
    if (unit->partyRecord.status & 1) {
        return 0;
    }
    threshold = 0x1E;
    if (!(flagsB & 2)) {
        threshold = !(flagsB & 4) ? 0 : 0x28;
    }
    btlBossDebugPrintf(D_00415440, threshold);
    return btlRollAiBucket() < threshold;
}

extern s32 btlGetActionRecordLookupValue(s32);

extern s32 fldCountSceneSlots(void);

/* Combine both contributions; groups of three or more suppress the 30% case. */
INCLUDE_RODATA(const s32, "game/code_001AF4A0", D_00415440);

s32 btlRollAllFearChance(s32 unused, BtlUnit *unit, u32 flags,
                  s32 unusedFlags, u8 useSelectedAction) {
    s32 actionThreshold;
    s32 statusThreshold;
    s32 threshold;
    u32 category;

    if (((BtlState *)btlGetRuntime())->unk220 & 0x80) {
        return 0;
    }
    actionThreshold = 0;
    statusThreshold = 0;
    if (useSelectedAction != 0) {
        if (unit->selectedEntryIndex == -1) {
            return 0;
        }
        category = btlGetActionRecordLookupValue(unit->selectedEntryIndex);
        switch (category) {
            case 0x20000:
            case 0x40000:
                actionThreshold = 40;
                break;
            case 0x10000:
                actionThreshold = 30;
                if (fldCountSceneSlots() >= 3) {
                    actionThreshold = 0;
                }
                break;
        }
    }
    if (flags & 0x60000) {
        statusThreshold = 40;
    } else if (flags & 0x10000) {
        statusThreshold = 30;
        if (fldCountSceneSlots() >= 3) {
            statusThreshold = 0;
        }
    }
    threshold = statusThreshold < actionThreshold ? actionThreshold : statusThreshold;
    btlBossDebugPrintf("btl:fear all ratio[%d]\n", threshold);
    return btlRollAiBucket() < threshold;
}

f32 func_001B0B20(void) {
    return 1.5f;
}


u8 btlSelectCommandHitCount(BtlUnit *unit, s32 command) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 result;
    s32 minimum;
    s32 maximum;

    if (controller->commandRangeOverride != NULL) {
        result = controller->commandRangeOverride(unit, command);
        if (result > 0) {
            return result;
        }
    }
    if (command == 0) {
        return 1;
    }
    if (datCommandSelectors[command].kind == 5 && (unit->status.flags & 0x200)) {
        minimum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMin;
        maximum = ((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].rangeMax;
    } else {
        minimum = datCommandRecords[command].rangeMin;
        maximum = datCommandRecords[command].rangeMax;
    }
    if (minimum < maximum) {
        result = minimum + sdfGetRandInt(0, maximum - minimum + 1);
    } else {
        result = minimum;
    }
    return result;
}

u8 btlGetActorDisplayByteWithDefault(BtlUnit *object, s32 index) {
    if (index == 0) {
        if ((object->status.flags & 0x400) != 0) {
            return datEnemyRecords[object->partyRecord.unitId].attackSpacing;
        }
        return 12;
    }
    return 12;
}

extern BtlActionAnimationRecord *datActionAnimationRecords;

extern s32 btlActionAnimationModeRows[];

/* Delay before an action: 9 for a 0x200-group unit in mode 6 with stat bit 0x10 clear on a
 * kind-5 or empty command, 1 for no command, else the animation's delay-table row. */
s32 btlGetActionAnimationDelay(BtlUnit *unit, s32 actionId) {
    s32 *delayTable;
    s32 *delay;

    if (actionId == 0 || datCommandSelectors[actionId].kind == 5) {
        if ((btlUnitStatusPair(unit) & 0x1200) == 0x200 &&
            (*(u64 *)&unit->partyRecord.flags & 0xFFFF00000010ULL) == 0x600000000ULL) {
            return 9;
        }
    }
    if (actionId == 0) {
        return 1;
    }
    delayTable = btlActionAnimationModeRows;
    delayTable++;
    delay = delayTable + datActionAnimationRecords[actionId].delayIndex * 2;
    btlBossDebugPrintf("btl:delay=%d\n", *delay);
    return *delay;
}

s32 btlMapActionCode(s32 unused, u32 id) {
    switch (id) {
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x185:
        return 0x35;
    case 0x186:
        return 0x1E;
    case 0x193:
        return 0x25;
    default:
        return datCommandSelectors[id].kind == 2 ? 0x2D : 0;
    }
}



/* Sort same-side targets for the selected animation, or perform thirteen swaps. */
void btlSortActorMuzzleDirections(BtlUnit *unit, BtlIndexList *targets, s32 actionId) {
    BtlUnit *pair[2];
    f32 muzzle[2][4];
    u32 randomIndices[2];
    u32 count;
    u32 mode;
    u32 i;
    u32 last;
    s32 sorted;

    count = btlGetIndexListCount(targets);
    if (count < 2) {
        return;
    }
    if ((unit->status.flags & 0x200) != 0 &&
        datCommandSelectors[actionId].kind == 5) {
        mode = unit->partyRecord.unitId == 6 ? 2 : 0;
    } else {
        u32 delayIndex = datActionAnimationRecords[actionId].delayIndex;
        mode = ((BtlAnimationModeRow *)btlActionAnimationModeRows)[delayIndex].mode;
    }
    switch (mode) {
    case 0:
        btlBossDebugPrintf("btl:anim=non\n");
        return;
    case 1:
        btlBossDebugPrintf("btl:anim=left\n");
        break;
    case 2:
        btlBossDebugPrintf("btl:anim=right\n");
        break;
    case 3:
        btlBossDebugPrintf("btl:anim=rand\n");
        break;
    }
    if (mode != 3) {
        last = count - 1;
        do {
            sorted = 1;
            for (i = 0; i < last; i++) {
                pair[0] = btlGetIndexListEntry(targets, i);
                pair[1] = btlGetIndexListEntry(targets, i + 1);
                btlUnitGetMuzzlePosVU(pair[0]);
                VU0_STORE_VF(vf10, muzzle[0]);
                btlUnitGetMuzzlePosVU(pair[1]);
                VU0_STORE_VF(vf10, muzzle[1]);
                if ((pair[0]->status.flags & 0x400) && (pair[1]->status.flags & 0x400)) {
                    if (mode == 1) {
                        if (muzzle[0][0] > muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] < muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                } else if ((pair[0]->status.flags & 0x200) && (pair[1]->status.flags & 0x200)) {
                    if (mode == 1) {
                        if (muzzle[0][0] < muzzle[1][0]) {
                            btlSwapIndexListEntries(targets, i, i + 1);
                            sorted = 0;
                        }
                    } else if (muzzle[0][0] > muzzle[1][0]) {
                        btlSwapIndexListEntries(targets, i, i + 1);
                        sorted = 0;
                    }
                }
            }
        } while (sorted == 0);
    } else {
        for (i = 0; i < 13; i++) {
            randomIndices[0] = sdfGetRandInt(0, count);
            randomIndices[1] = sdfGetRandInt(0, count);
            if (randomIndices[0] != randomIndices[1]) {
                btlSwapIndexListEntries(targets, randomIndices[0], randomIndices[1]);
            }
        }
    }
}

s32 btlShouldDistributeRandomTargetHits(BtlUnit *unit, s32 command) {
    BtlState *runtime = btlGetRuntime();
    /* 0x6D8: command-availability override (named in DDS1's BtlState). */
    s32 (*availability)(BtlUnit *, s32) = *(s32 (**)(BtlUnit *, s32))runtime->pad6D8;

    if (availability != NULL) {
        if (availability(unit, command) == 0) {
            return 0;
        }
    }
    if ((unit->status.flags & 0x200) && datCommandSelectors[command].kind == 5) {
        if (((EventRosterStat *)datRosterDetails)[unit->partyRecord.unitId].pad0B == 1) {
            return 1;
        }
    } else if (datCommandRecords[command].distributeRandomTargetHits == 1) {
        return 1;
    }
    return 0;
}

void btlDistributeRandomTargetHits(BtlUnit *unit, BtlIndexList *targets,
                   BtlTargetResult *results, s32 command) {
    u8 selected[13];
    BtlIndexList *copy;
    void *previous;
    void *entry;
    u32 targetCount;
    u32 maximumHits;
    u32 hitCount;
    u32 i;
    u32 index;
    s32 resultIndex;
    s32 allowConsecutiveHits;

    targetCount = btlGetIndexListCount(targets);
    allowConsecutiveHits = targetCount < 2;
    maximumHits = targetCount + sdfGetRandInt(0, 2);
    if (allowConsecutiveHits) {
        results[0].hitCount = maximumHits;
        return;
    }

    previous = NULL;
    copy = btlAllocateIndexList(ARRAY_COUNT(selected));
    i = 0;
    btlCopyIndexList(copy, targets);
    btlClearIndexList(targets);
    hitCount = btlSelectCommandHitCount(unit, command);
    memset(selected, 0, sizeof(selected));
    while (i < hitCount) {
        index = sdfGetRandInt(0, targetCount);
        entry = btlGetIndexListEntry(copy, index);
        if (!allowConsecutiveHits && entry == previous) {
            index = (index + sdfGetRandInt(0, targetCount - 1) + 1) % targetCount;
            entry = btlGetIndexListEntry(copy, index);
        }
        previous = entry;
        if (!selected[index]) {
            btlAppendIndexListEntry(targets, entry);
            resultIndex = btlFindListIndex(targets, entry);
            selected[index] = 1;
            results[resultIndex].hitCount = 1;
        } else {
            resultIndex = btlFindListIndex(targets, entry);
            if (results[resultIndex].hitCount < maximumHits) {
                results[resultIndex].hitCount++;
            }
        }
        i++;
    }
    btlFreeIndexList(copy);
}

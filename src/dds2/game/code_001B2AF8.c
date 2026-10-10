#include "fld_area_work.h"
#include "sdf_packet_list.h"
#include "common.h"
#include "btl_roster_pair.h"
#include "fr_font_measure.h"
#include "eff_resource_slots.h"
#include "sdf_chip.h"
#include "kwln.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "btl_scene_fade.h"
#include "btl_resource.h"
#include "eff.h"
#include "itf.h"
#include "itf_panel_draw.h"
#include "btl_state.h"
#include "btl_ui.h"
#include "sdf.h"
#include "sdf_projection.h"
#include "btl_action.h"
#include "scr.h"
#include "dat_state.h"
#include "mnu_result.h"
#include "dat_command.h"
#include "sdf_sif_command.h"
#include "kwln_task_lifecycle.h"
#include "sdf_packet_builders.h"
#include "sdf_texture_draw_packet.h"
#include "btl.h"
#include "btl_command.h"
#include "fr_font.h"

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

extern f32 D_00415F80[4];

extern s32 func_001ABB10(BtlUnit *, s32);

extern u32 D_004367F8;

extern const char *D_004367CC;

extern u32 sndTestMessageResourceIndex;

extern SdfTex *sndTestMessageTexture;

extern u64 D_004366E8;

extern s32 btlRuntime;

extern s32 btlGetRuntime(void);

extern BattleCmdPanel *btlCommandPanelWork;

extern const char *btlMahenPanelTaskNameRef;

extern void func_00101968(KwlnTask *parent, KwlnTask *child);

extern const char *btlAnalyzPanelTaskNameRef;

extern const char *D_004367E0;

extern const char *D_004367D8;

extern const char *D_004367C8;

extern const char *D_004367C4;

extern SceneKindTable *D_00438F4C;

extern u32 D_00438F50;

typedef struct BattleItemDrop {
    u16 id;
    u8 count;
    u8 pad03;
} BattleItemDrop;

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
    s32 rewardAp; /* 0x2F8 */
    u16 specialEnemyDefeats; /* 0x2FC: defeated enemy kinds 100 through 103. */
    u8 pad2FE[0x3DE];
    s32 (*commandRangeOverride)(BtlUnit *, s32);
} BattleController;

extern BattleTrackedTaskWork *btlTrackedTaskHandles;

extern s32 btlGetTrackedTaskHandle(s32);

extern u32 D_003B6928[];

extern DatEnemyRecord *datEnemyRecords;

extern void btlBossDebugPrintf(const char *, ...);

extern s8 effSharedRandomState[];

extern s32 btlCheckSpecialAbility(DatPartyRecord *, s32);

extern s32 mdlFlagTest(s32);

extern s32 btlDoesEnabledStatusMatchCurrentId(DatPartyRecord *, u32);

extern void mdlFlagSet(s32);

extern void mdlFlagClear(s32);

/* Native per-species AI record, shared layout with the action evaluator. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

typedef struct AiSpecies {
    u8 unk00;
    u8 pad01[0x3F];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *datEnemyAiRecords;

extern s32 func_001B32F8(s32, s32 *);

extern char D_00415840[]; /* "btl:endure=%d%%[ratio=%.2f]\n" */

extern u8 D_003B4EC8[];

extern char D_004159A0[];

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

/* Option IDs index the same signed-byte bank used by the named controls. */
typedef struct SndPad {
    u8 pad00[0x20];
    union {
        s8 buttons[0x20];
        struct {
            u8 pad20;
            s8 confirm;
            s8 unk22;
            s8 unk23;
            s8 unk24;
            s8 unk25;
            s8 prev;
            s8 next;
            u8 pad28[9];
            s8 unk31;
            s8 unk32;
            s8 cancel;
            s8 coarseDown;
            s8 coarseUp;
            s8 unk36;
            s8 unk37;
            s8 fineDown;
            u8 pad39;
            s8 fineUp;
            u8 pad3B[5];
        };
    };
} SndPad;

extern SndPad D_0037F510;

extern u8 D_00436630[5];

extern u8 D_00436638[5];

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);

extern char D_00436660[];

extern char D_00436668[];

extern char D_00436670[];

extern char D_00436678[];

extern char D_00436680[];

extern char D_00436688[];

extern f32 sdfSinPoly(f32);

extern char D_004366F0[];

extern SdfMemBlock *D_004366E0;

extern u32 func_001B5600(void);

extern s32 itfMesCreateWindow(ItfMesSub *);

extern char D_004366F8[];

extern void itfMesDestroyWindowIfPresent(s32);

extern s32 datGetStatWithStatusOverride(DatPartyRecord *, s32);

extern s32 evtGetMirroredSolarPhase(void);

extern s32 btlRollAiBucket(void);

extern u32 effMiscRandMod(void *state, u32 modulus);

extern s32 func_001B39E8(u32);

typedef struct BattleAdjustmentEntry {
    u16 sceneIndex;
    u16 weight;
    s8 value;
    u8 unk05;
} BattleAdjustmentEntry;

typedef struct BattleAdjustmentGroup {
    s32 interval;
    BattleAdjustmentEntry entries[20];
} BattleAdjustmentGroup;

typedef struct BattleAdjustmentRecord {
    u8 pad00[8];
    u32 conditions[3];
    u8 variantCodes[8];
    BattleAdjustmentGroup groups[3];
} BattleAdjustmentRecord;

extern BattleAdjustmentRecord *D_00435E0C;

extern u32 effMiscRandMod(void *, u32);

void btlClearActorEntrySlot(BtlUnit *unit, s32 index);

extern s32 btlRollAiBucket();

extern s32 fldCountSceneSlots(void);

extern s32 datRosterDetails;

extern s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *);

void btlClearAllActorEntrySlots(BtlUnit *unit);

s8 btlGetActorIndexedSignedValue(BtlUnit *object, s32 index);

s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *record);

s32 btlGetSideIndexedActorStatusTable(s32 arg0, s32 arg1);

s32 btlGetSlotValueAdjustedForSpecialAbility(BtlUnit *battler, s32 slot);

s32 btlIsUnitDefeatTriggeredByValueDelta(BtlUnit *unit, s32 delta);

/* Read current HP from a unit-entry address. */ s32 btlReadCurrentUnitHp(DatPartyRecord *entry);

/* Cache the skill-adjusted maximum and return current HP clamped to it. * The comparison uses the full-width result, not the u16 cache. */ u16 btlRefreshUnitMaximumHpAndClampCurrentHp(DatPartyRecord *entryAddress);

/* Cache the skill-adjusted maximum and return current MP clamped to it. * The comparison uses the full-width result, not the u16 cache. */ u16 btlRefreshUnitMaximumMpAndClampCurrentMp(DatPartyRecord *entryAddress);

/* Solar-phase bias table and special-ability scale inside the battle parameter block. */
typedef struct BattleEscapeParameters {
    u8 pad000[0xB50];
    s16 solarPhaseBias[12]; /* 0xB50 */
    f32 specialScale;       /* 0xB68 */
} BattleEscapeParameters;

const char D_004156B0[] = "btl:escape=%d%%[ratio=%.2f,sn=%d]\n";

s32 btlTryEscapeWithPhaseAdjustment(BtlUnit *actor) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *enemy;
    s32 noEligibleEnemy;
    s32 chance;
    f32 ratio;
    DatPartyRecord *record;
    s32 phase;

    if (datBattleSceneRecords[battle->battleMode].unk00 != 0) {
        return 0;
    }
    if (datBattleSceneRecords[battle->battleMode].flags & 0x20) {
        return 1;
    }
    if (battle->encounterKind == 3) {
        return 1;
    }
    noEligibleEnemy = 1;
    for (enemy = battle->units; enemy != NULL; enemy = enemy->nextActor) {
        if ((enemy->status.flags & 1) != 0) {
            if ((enemy->status.flags & 0x400) != 0) {
                if ((enemy->partyRecord.status & 0x2A0F) == 0) {
                    noEligibleEnemy = 0;
                    break;
                }
            }
        }
    }
    if (noEligibleEnemy) {
        return 1;
    }
    record = &actor->partyRecord;
    chance = evtRunContext(0x15, (s32)record, 0, 0, 0);
    ratio = 1.0f;
    if (btlCheckSpecialAbility(record, 0x250)) {
        ratio = datAbilityParameters[0x250 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    if (mdlFlagTest(0x80E)) {
        ratio *= ((BattleEscapeParameters *)datBattleParameters)->specialScale;
    }
    chance = chance * ratio;
    phase = evtGetMirroredSolarPhase();
    chance += ((BattleEscapeParameters *)datBattleParameters)->solarPhaseBias[phase];
    if (chance > 95) {
        chance = 95;
    } else if (chance < 40) {
        chance = 40;
    }
    btlBossDebugPrintf(D_004156B0, chance, ratio, ((BattleEscapeParameters *)datBattleParameters)->solarPhaseBias[phase]);
    return btlRollAiBucket() < chance;
}

s32 btlIsSelectedActorStatusAndRecordClear(BtlUnit *unit) {
    s32 work = btlGetRuntime();
    if (datBattleSceneRecords[*(s32 *)(work + 0x2A0)].unk00 != 0) {
        return 0;
    }
    if (unit->partyRecord.status & 0x2A0F) {
        return 0;
    }
    return datEnemyAiRecords[unit->partyRecord.unitId].unk00 == 0;
}

/* Return true when neither actor status nor its entry flags contain bit 0x40. */
s32 btlAreUnitStatusAndEntryFlagsClear(BtlUnit *actor) {
    if ((actor->partyRecord.status & 0x40) != 0) {
        return 0;
    }
    return (btlGetEntryFlagsUnlessDisabled(&actor->partyRecord) & 0x40) < 1;
}

extern s32 ptyMatchAffinityPermutation(s32 *actors, s32 affinity);

s32 btlFindCommandPartnersByAffinity(BtlUnit *unit, s32 command, BtlUnit **first, BtlUnit **second) {
    s32 statAddresses[3];
    u32 requiredCount = 0;
    BattleController *controller = (BattleController *)btlGetRuntime();
    s32 *requirementCursor;
    BtlUnit *candidate;
    BtlUnit *partner;
    u32 i;

    requirementCursor = datAffinityRecords[command - DAT_AFFINITY_FIRST_COMMAND].requirements;
    for (i = 0; i < 3; i++) {
        if (*requirementCursor++ != -1) {
            requiredCount++;
        }
    }
    if (requiredCount < 2) {
        return 0;
    }

    statAddresses[0] = (s32)&unit->partyRecord;
    for (candidate = controller->actors; candidate != NULL; candidate = candidate->nextActor) {
        u32 flags = candidate->status.flags;

        if ((flags & 1) == 0) {
            continue;
        }
        if ((flags & 0x400) == 0) {
            continue;
        }
        if ((candidate->partyRecord.status & 0x2A0E) != 0) {
            continue;
        }
        if (unit == candidate) {
            continue;
        }
        statAddresses[1] = (s32)&candidate->partyRecord;
        if (requiredCount == 3) {
            for (partner = controller->actors; partner != NULL; partner = partner->nextActor) {
                u32 partnerFlags = partner->status.flags;

                if ((partnerFlags & 1) == 0) {
                    continue;
                }
                if ((partnerFlags & 0x400) == 0) {
                    continue;
                }
                if ((partner->partyRecord.status & 0x2A0E) != 0) {
                    continue;
                }
                if (unit == partner || candidate == partner) {
                    continue;
                }
                statAddresses[2] = (s32)&partner->partyRecord;
                if (ptyMatchAffinityPermutation(statAddresses, command) == 0) {
                    continue;
                }
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = partner;
                }
                return 1;
            }
        } else {
            statAddresses[2] = 0;
            if (ptyMatchAffinityPermutation(statAddresses, command) != 0) {
                if (first != NULL) {
                    *first = candidate;
                }
                if (second != NULL) {
                    *second = NULL;
                }
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B2F50);

s32 btlHasAdjacentActorRecordStatus(BtlUnit *object) {
    u8 *status = datEnemyRecords[object->partyRecord.unitId].unk3E;
    u32 i;
    for (i = 0; i < 2; i++) {
        if (*status++ != 0) {
            return 1;
        }
    }
    return 0;
}

/* Return 1 when bit 26 is clear, preserving the original signed word shift. */
u32 btlIsActorHighStateFlagClear(BtlUnit *actor) {
    return (((s32)actor->status.flags >> 0x1a) ^ 1U) & 1;
}

s32 func_001B3200(BtlUnit *requester) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    u32 limit;
    u32 active;
    u32 spawnLimit;
    BtlUnit *unit;

    if (datBattleSceneRecords[battle->battleMode].maxActiveEnemies >= 6) {
        return 0;
    }
    limit = 5;
    if (datBattleSceneRecords[battle->battleMode].maxActiveEnemies != 0) {
        limit = datBattleSceneRecords[battle->battleMode].maxActiveEnemies;
    }
    if (battle->enemyLimitOverride != 0) {
        limit = battle->enemyLimitOverride(requester, &datBattleSceneRecords[battle->battleMode], limit);
    }
    active = 0;
    for (unit = battle->units; unit != 0; unit = unit->nextActor) {
        s32 flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x400) {
                active++;
            }
        }
    }
    if (active >= limit) {
        return 0;
    }
    if (datBattleSceneRecords[battle->battleMode].maxEnemySpawns != 0) {
        spawnLimit = datBattleSceneRecords[battle->battleMode].maxEnemySpawns;
        if (battle->unk27E >= spawnLimit) {
            return 0;
        }
    }
    return 1;
}


s32 func_001B32F8(s32 object, s32 *choices) {
    s32 count = 0;
    u32 i;
    u16 *entry = (u16 *)(object + 0x142);
    u16 value;

    for (i = 0; i < 0x18; i++) {
        value = entry[i];
        if (value < 0xA0) {
            continue;
        }
        if (value >= 0xA6) {
            if (value >= 0xB9) {
                continue;
            }
            if (value < 0xB5) {
                continue;
            }
        }
        if (func_001ABB10((BtlUnit *)object, value) != 0) {
            continue;
        }
        if (choices != NULL) {
            choices[count] = value;
        }
        count++;
    }
    return count;
}

u8 btlHasAvailableOption(u32 arg0) {
    s64 temp_v0;

    temp_v0 = func_001B32F8(arg0, 0);
    return temp_v0 != 0;
}

s32 btlChooseRandomAvailableOption(s32 object) {
    s32 choices[24];
    s32 count = func_001B32F8(object, choices);
    if (count != 0) {
        return choices[effMiscRandMod(0, count)];
    }
    return -1;
}

s32 btlChooseEligibleSkill(s32 object) {
    s32 choices[24];
    s32 count = 0;
    u32 i;
    u16 *ids = (u16 *)(object + 0x142);
    for (i = 0; i < 24; i++) {
        u32 id = *ids++;
        if (id != 0) {
            if (id < 0x2A0) {
                s32 category = datCommandSelectors[id].kind;
                if (category != 2) {
                    if (category != 4) {
                        if ((datCommandRecords[id].unk_01 & 2) != 0) {
                            if (id < 0xAB || (id >= 0xAD && id != 0xBF)) {
                                choices[count++] = id;
                            }
                        }
                    }
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    return choices[effMiscRandMod(0, count)];
}

f32 btlGetActionCategoryMultiplier(BtlUnit *unit, s32 unused, s32 index) {
    s32 mode = datCommandRecords[index].hpType;
    f32 rate;
    if (mode < 0xE) {
        rate = 1.0f;
        if (mode >= 0xC) {
            return rate;
        }
    }
    if (index == 0 && btlCheckSpecialAbility(&unit->partyRecord, 0x23D) != 0) {
        rate = datAbilityParameters[0x23D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    } else {
        rate = 0.0f;
    }
    return rate;
}

f32 btlGetActionCategoryGateAsFloat(s32 unused0, s32 unused1, s32 index) {
    s32 category = datCommandRecords[index].mpType;
    if (category < 14) {
        if (category >= 12) {
            return 1.0f;
        }
    }
    return 0.0f;
}

f32 btlGetActionRecordPercentAsFraction(s32 unused0, s32 unused1, s32 index) {
    return (f32)datCommandRecords[index].effectPercent / 100.0f;
}

s32 btlAdjustPointsForCombatFlags(s32 unused, u32 flags, u32 otherFlags, u32 value, s32 index) {
    DatCommandRecord *entry;
    u16 code;
    if (flags & 0x20000) {
        return 0x1194;
    }
    if (flags & 0x40000) {
        return 0x1194;
    }
    if (flags & 0x10000) {
        return value + 100;
    }
    if (flags & 2) {
        return value + 100;
    }
    if (flags & 4) {
        entry = (DatCommandRecord *)(index * 56 + (s32)datCommandRecords);
        code = entry->hpType;
        if (code != 8 && code != 10 && entry->effectType != 2 &&
            entry->unk30 != 4) {
            return value + 100;
        }
    }
    if (otherFlags & 4) {
        return value >> 1;
    }
    if (otherFlags & 2) {
        return value >> 1;
    }
    return value;
}

s32 btlGetCommandResultKindFromFlags(u32 flags, u32 otherFlags, s32 index) {
    DatCommandRecord *entry;
    u16 code;
    if (flags & 0x20000) {
        return 1;
    }
    if (flags & 0x40000) {
        return 1;
    }
    if (flags & 0x10000) {
        return 1;
    }
    if (flags & 2) {
        return 1;
    }
    if (flags & 4) {
        entry = (DatCommandRecord *)(index * 56 + (s32)datCommandRecords);
        code = entry->hpType;
        if (code != 8 && code != 10 && entry->effectType != 2 &&
            entry->unk30 != 4) {
            return 1;
        }
    }
    if (otherFlags & 4) {
        return 3;
    }
    if (otherFlags & 2) {
        return 2;
    }
    return 1;
}

s32 btlAverageMaximumValueForMask(s32 mask, s8 skipDown) {
    BtlUnit *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->partyRecord.flags & mask) {
                    count++;
                    total += unit->partyRecord.maxHp;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

void btlAverageFilteredMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 1);
}

void btlAverageAllMaximumForMask(u32 arg0) {
    btlAverageMaximumValueForMask(arg0, 0);
}

s32 btlAverageCurrentValueForMask(s32 mask, s8 skipDown) {
    BtlUnit *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->partyRecord.flags & mask) {
                    count++;
                    total += unit->partyRecord.hp;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

void btlAverageFilteredCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 1);
}

void btlAverageAllCurrentForMask(u32 arg0) {
    btlAverageCurrentValueForMask(arg0, 0);
}

s32 btlAverageMaskedActorStat(s32 mask, s8 skipDown) {
    BtlUnit *unit;
    s32 total = 0;
    s32 count = 0;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (skipDown == 0 || !(flags & 0x20)) {
                if (unit->partyRecord.flags & mask) {
                    count++;
                    total += unit->partyRecord.level;
                }
            }
        }
    }
    if (count > 0) {
        return total / count;
    }
    return 1;
}

s32 func_001B39E8(u32 mask) {
    return btlAverageMaskedActorStat(mask, 1);
}

void func_001B3A00(u32 arg0) {
    btlAverageMaskedActorStat(arg0, 0);
}

s32 btlSumOrAverageActorAttribute(u32 mask, s32 attribute, s8 skipDown) {
    s32 sum = 0;
    s32 count = 0;
    BtlUnit *unit = ((BattleController *)btlGetRuntime())->actors;
    for (; unit != 0; unit = unit->nextActor) {
        u32 flags = unit->status.flags;
        if ((flags & 1) != 0) {
            if (skipDown == 0 || (flags & 0x20) == 0) {
                if ((unit->partyRecord.flags & mask) != 0) {
                    s32 value = datGetStatWithStatusOverride(&unit->partyRecord, attribute);
                    count++;
                    sum += value;
                }
            }
        }
    }
    if (count >= 2) {
        sum /= count;
    }
    return sum;
}

void btlAverageFilteredActorAttribute(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 1);
}

void __udivdi3(u32 arg0, u32 arg1) {
    btlSumOrAverageActorAttribute(arg0, arg1, 0);
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B3B28);

extern s32 evtRunContext(s32, s32, s32, s32, u16);

/* Preserve the native forwarded chance word; the empty provider ignores it. */
extern void func_0011EBE8();

extern const char D_004156F8[];

s32 btlSelectPreemptiveOutcome(void) {
    BtlState *state = (BtlState *)btlGetRuntime();
    s32 chance;
    f32 ratio;
    u32 i;

    if (state->battleFlags & 0x4000) {
        if (state->requestMode == 2 || state->requestMode == 4) {
            if (datBattleSceneRecords[state->battleMode].flags & 2) {
                return 2;
            }
        }
        return 1;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 4) {
        return 1;
    }
    if (state->encounterKind != 3 &&
        (datBattleSceneRecords[state->battleMode].flags & 2)) {
        return 2;
    }
    if (state->specialEncounterBlocked) {
        return 2;
    }
    ratio = 1.0f;
    chance = evtRunContext(0x17, 0, 0, 0, 0);
    if (state->encounterKind == 3) {
        ratio = datBattleParameters->preemptiveModeScale;
    }
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (datGameState->party[i].flags & 2) {
                if (btlCheckSpecialAbility(&datGameState->party[i], 0x24E)) {
                    ratio *= datAbilityParameters[0x24E - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
                }
            }
        }
    }
    chance = (s32)((f32)chance * ratio);
    if (state->encounterKind != 3) {
        if (chance < 60) {
            chance = 60;
        } else if (chance > 80) {
            chance = 80;
        }
    }
    btlBossDebugPrintf(D_004156F8, chance, ratio);
    func_0011EBE8(chance);
    return btlRollAiBucket() < chance ? 1 : 2;
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004156F8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B3DD8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B4040);

extern const f32 D_004157A0[9];
extern const char D_004157C8[];
/* The empty native provider still receives the calculated chance word. */
extern void func_0011EBF8();

s32 btlRollSurpriseAttack(void) {
    f32 levelScale[9];
    BtlState *state;
    u32 partyLevel;
    u32 count;
    u32 enemyLevel;
    u32 i;
    s32 difference;
    s32 chance;
    f32 ratio;

    memcpy(levelScale, D_004157A0, sizeof(levelScale));
    state = (BtlState *)btlGetRuntime();
    if (mdlFlagTest(0x820)) {
        return 0;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 0x200) {
        return 1;
    }
    if (datBattleSceneRecords[state->battleMode].flags & 0x100) {
        return 0;
    }
    if (state->specialEncounterBlocked) {
        return 1;
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
        chance = 25;
    } else if (datBattleSceneRecords[state->battleMode].flags & 0x1000) {
        chance = 50;
    } else {
        chance = 5;
    }
    chance = (s32)((f32)chance * levelScale[difference]);
    ratio = 1.0f;
    for (i = 0; i < 5; i++) {
        if (datGameState->party[i].flags & 1) {
            if (datGameState->party[i].flags & 2) {
                if (btlCheckSpecialAbility(&datGameState->party[i], 0x24D)) {
                    ratio *= datAbilityParameters[0x24D - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
                }
            }
        }
    }
    chance = (s32)((f32)chance * ratio);
    btlBossDebugPrintf(D_004157C8, chance, difference, ratio);
    func_0011EBF8(chance);
    return btlRollAiBucket() < chance;
}

void btlClearUnitStatusMask(void) {
    BtlUnit *unit;
    u32 flags;
    for (unit = ((BattleController *)btlGetRuntime())->actors; unit != 0; unit = unit->nextActor) {
        flags = unit->status.flags;
        if (flags & 1) {
            if (flags & 0x200) {
                unit->status.flags = flags & ~0x1000;
                unit->partyRecord.flags &= 0xEFFF;
            }
        }
    }
}

s32 btlHasSpecialAbilityOrModelFlag(DatPartyRecord *record) {
    if (btlCheckSpecialAbility(record, 0x279)) {
        return 1;
    }
    return mdlFlagTest(0x820) != 0;
}

/* Roll the defeat chance for a petrified actor and the command's attack kind. */
INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004157A0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004157C8);

s32 func_001B4600(BtlUnit *unit, s32 command) {
    s32 kind;
    s32 chance;
    f32 ratio;
    DatPartyRecord *record;

    if ((unit->partyRecord.status & 0x7FFF) != 0x800) {
        return 0;
    }
    if (btlHasEnemyRecordDefeatExemptionFlag(unit) != 0) {
        return 0;
    }
    kind = btlGetActorIndexedSignedValue(unit, command);
    switch (kind) {
    case 2:
    case 3:
    case 4:
        return 0;
    case 7:
        if (datCommandRecords[command].effectType != 0) {
            return 0;
        }
        break;
    case 0:
    case 1:
    case 5:
    case 6:
        break;
    default:
        return 0;
    }
    record = &unit->partyRecord;
    chance = evtRunContext(0x13, (s32)record, 0, command, 0);
    ratio = 1.0f;
    if (btlCheckSpecialAbility(record, 0x253) != 0) {
        ratio = datAbilityParameters[0x253 - BTL_ABILITY_PARAMETER_FIRST_SKILL].value;
    }
    chance = (s32)((f32)chance * ratio);
    btlBossDebugPrintf("btl:stone dead=%d%%[ratio=%.2f]\n", chance, (f64)ratio);
    return btlRollAiBucket() < chance;
}

s32 btlRollActorEligibilityWithAbilityOverride(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->actorEligibilityOverride;
    if (hook != 0 && hook(unit) == 0) {
        return 0;
    }
    if (unit->status.stateFlags & 0x2000) {
        return 0;
    }
    if ((unit->partyRecord.status & 0x7FFF) == 0x4000) {
        return 0;
    }
    if (btlCheckSpecialAbility(&unit->partyRecord, 0x251) != 0) {
        return 1;
    }
    btlBossDebugPrintf(D_00415840, 5, 1.0);
    return btlRollAiBucket() < 5;
}

s32 btlHasEnemyRecordDefeatExemptionFlag(BtlUnit *object) {
    if ((object->status.flags & 0x400) == 0) {
        return 0;
    }
    return ((s32)datEnemyRecords[object->partyRecord.unitId].flags & 0x100) > 0;
}

extern s32 effMiscRand(void *);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415840);

u32 btlGetHuntPenaltyFlags(BtlUnit *unit, BtlUnit *enemy) {
    DatEnemyRecord *record;
    s32 chance;
    u16 status;
    u16 flags;

    if (btlCheckSpecialAbility(&unit->partyRecord, 0x245) != 0) {
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

typedef struct BtlAiActionEntry {
    s32 abilityId;
    s32 actionCode;
    u8 unk_08;
    u8 pad_09[3];
} BtlAiActionEntry;

extern BtlAiActionEntry D_003B5100[];

s32 func_001B4918(BtlUnit *unit, BtlUnit *target) {
    s32 result;
    s32 threshold;
    u32 i;

    if (btlIsUnitDefeatTriggeredByValueDelta(unit, 0) != 0) {
        return -1;
    }

    result = 0;
    for (i = 0; i < 5; i++) {
        if (btlCheckSpecialAbility(&unit->partyRecord, D_003B5100[i].abilityId) == 0) {
            continue;
        }
        if (D_003B5100[i].unk_08 != 0) {
            if ((unit->status.flags & 0x200) == 0) {
                continue;
            }
            if ((target->status.flags & 0x400) == 0) {
                continue;
            }
            if ((unit->status.flags & 0x1000) == 0) {
                if ((unit->partyRecord.flags & 0x10) == 0) {
                    continue;
                }
            }
            if ((unit->partyRecord.status & 0x40) != 0) {
                continue;
            }
            if ((btlGetEntryFlagsUnlessDisabled(&target->partyRecord) & 0x40) != 0) {
                continue;
            }
        }
        threshold = (s32)(datAbilityParameters[D_003B5100[i].abilityId -
                                             BTL_ABILITY_PARAMETER_FIRST_SKILL].value *
                          100.0f);
        if (btlRollAiBucket() < threshold) {
            result = D_003B5100[i].actionCode;
            break;
        }
    }
    return result != 0 ? result : -1;
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B4AA0);

typedef struct BattleStartVoiceRow {
    s32 flagId;       /* negative, or a model flag that must be set */
    s8 voice[2][3][3]; /* [alternate][state][choice]; -1 marks an empty choice */
    u8 pad16[2];
} BattleStartVoiceRow;

extern BattleStartVoiceRow D_003B5140[16];
extern s8 D_003B52C0[16];
extern char D_004158F8[], D_00415920[];
extern s32 btlReadCurrentUnitHp(DatPartyRecord *);
extern s32 func_001AF4A0(BtlUnit *, BtlUnit *, s32, s32, s32, s32, u32);

extern char D_00415878[]; /* "btl:start voice off[ESC_OFF]\n" */
extern char D_00415898[]; /* "btl:start voice off[SERIAL]\n" */
extern char D_004158B8[]; /* "btl:start voice off[RAND]\n" */
extern char D_004158D8[]; /* "btl:start voice off[UNIT NON]\n" */
extern char D_00415948[]; /* "btl:start voice off[STATE=%X,ID=%X]\n" */
extern char D_00415970[]; /* "btl:start voice on[VOICE=%X,STATE=%X,ID=%X]\n" */

s8 func_001B4EB8(void) {
    BtlState *state;
    BtlUnit *candidates[3];
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
    if (datBattleSceneRecords[state->battleMode].unk00 != 0) {
        btlBossDebugPrintf(D_00415878);
        return -1;
    }
    if ((state->battleFlags & 0x4000) || state->eventReady == 5) {
        btlBossDebugPrintf(D_00415898);
        return -1;
    }
    if (state->encounterKind != 3 && (u32)btlRollAiBucket() >= 10) {
        btlBossDebugPrintf(D_004158B8);
        return -1;
    }
    count = 0;
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x200) {
                s32 flagId = D_003B5140[unit->partyRecord.unitId].flagId;
                if (flagId < 0 || mdlFlagTest(flagId)) {
                    if ((unit->partyRecord.status & 0x7FFF) == 0) {
                        candidates[count++] = unit;
                    }
                }
            }
        }
    }
    if (count == 0) {
        btlBossDebugPrintf(D_004158D8);
        return -1;
    }
    chosen = candidates[effMiscRandMod(0, count)];
    if (chosen->partyRecord.unitId == 5 && mdlFlagTest(0x834)) {
        return -1;
    }
    if (state->encounterKind == 3) {
        if (!mdlFlagTest(0x81A)) {
            return -1;
        }
        if ((u32)btlRollAiBucket() >= 50) {
            btlBossDebugPrintf(D_004158F8);
            return -1;
        }
        voice = D_003B52C0[chosen->partyRecord.unitId];
        btlBossDebugPrintf(D_00415920, voice);
        return voice;
    }
    category = 2;
    hp = btlReadCurrentUnitHp(&chosen->partyRecord);
    for (unit = state->units; unit != NULL; unit = unit->nextActor) {
        if (unit->status.flags & 1) {
            if (unit->status.flags & 0x400) {
                damage = func_001AF4A0(unit, chosen, 0, 1, 1, 1, 0);
                /* Keep the first decisive category; later damage calls still run.
                 * The high threshold wins even when unsigned arithmetic wraps. */
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
        voices = D_003B5140[chosen->partyRecord.unitId].voice[1][category];
    } else {
        voices = D_003B5140[chosen->partyRecord.unitId].voice[0][category];
    }
    count = 0;
    for (i = 0; i < 3; i++) {
        if (voices[i] >= 0) {
            count++;
        }
    }
    if (count == 0) {
        btlBossDebugPrintf(D_00415948, category, chosen->partyRecord.unitId);
        return -1;
    }
    voice = voices[effMiscRandMod(0, count)];
    btlBossDebugPrintf(D_00415970, voice, category, chosen->partyRecord.unitId);
    return voice;
}


u32 func_001B5268(void) {
    btlGetRuntime();
    return 0xffffffff;
}

void btlRestoreUnitMinimumValueAndClearStatus(BtlUnit *object, BtlOperandEntry *resource) {
    object->status.flags &= ~0x20;
    object->partyRecord.status &= ~0x4080;
    resource->flags &= ~1;
    resource->flags &= ~2;
    if (object->partyRecord.hp == 0) {
        object->partyRecord.hp = 1;
    }
}

s32 btlTestSideCountCommandEligibility(ActionStateLink *action) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlOperandGroup *group;
    BtlUnit *unit;
    u32 count;
    u32 i;
    u32 targetSideA;
    u32 targetSideB;
    u32 activeSideA;
    u32 activeSideB;

    if ((action->pendingFlags & 0x40) != 0) {
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
    for (unit = battle->units; unit != NULL; unit = unit->nextActor) {
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

s32 btlCanUseActorCommandForModelEntry(s32 object, s32 other, s32 offset, s32 index) {
    s32 (*predicate)(s32, s32, s32);
    s32 table;
    predicate = *(s32 (**)(s32, s32, s32))(btlGetRuntime() + 0x6cc);
    if (predicate != 0 && !predicate(object, other, index)) {
        return 0;
    }
    if (index == 0x91) {
        return 0;
    }
    table = btlGetSideIndexedActorStatusTable(*(s32 *)(object + 0xc4), *(s32 *)(object + 0xc8));
    if (*(s16 *)(table + offset * 20 + 0x2c) != 2) {
        return 0;
    }
    if (index != 0 && datCommandRecords[index].targetType != 0) {
        return 0;
    }
    return 1;
}

s32 btlIsActorModeActionCodeAllowed(BtlUnit *object) {
    s32 value;
    if (object->unkDC != 1) {
        return 1;
    }
    value = object->combatantKind;
    if (value == 0x39 || value == 0x126) {
        return 0;
    }
    return 1;
}

typedef struct BtlWeightEntry {
    u16 id;
    u8 weight;
    u8 pad3;
} BtlWeightEntry;

typedef struct BtlWeightTable {
    BtlWeightEntry entries[8];
} BtlWeightTable;

extern BtlWeightTable *D_00435E40;

u32 btlPickWeightedEntry(u16 index) {
    u32 offset = index * sizeof(BtlWeightTable);
    BtlWeightEntry *counted = (BtlWeightEntry *)(offset + (u32)D_00435E40);
    BtlWeightEntry *entry;
    s32 total = 0;
    s32 sum;
    s32 roll;
    s32 i;
    for (i = 7; i >= 0; i--, counted++) {
        if (counted->id != 0) {
            total += counted->weight;
        }
    }
    roll = effMiscRandMod(0, total);
    sum = 0;
    entry = (BtlWeightEntry *)(offset + (u32)D_00435E40);
    for (i = 0; i < 8; i++, entry++) {
        if (entry->id != 0) {
            if (roll < sum + entry->weight) {
                return entry->id;
            }
            sum += entry->weight;
        }
    }
    return 0;
}

u32 func_001B5600(void) {
    u32 i;

    if (mdlFlagTest(0x818) != 0) {
        return 0;
    }
    for (i = 0; i < 0x12; i++) {
        u8 area = D_003B4EC8[i * 2];
        if (area == fldAreaState.area) {
            u8 zone = D_003B4EC8[i * 2 + 1];
            if (zone == fldAreaState.floor + 1) {
                btlBossDebugPrintf(D_004159A0, area, zone);
                return 1;
            }
        }
    }
    return 0;
}

extern char D_004159C0[]; /* "btl:auto attack[%X]\n" */

/* Pick one of the unit's auto-attack skills at random (0x96/0x95/0x94 for abilities 0x22C/0x22B/0x22A);
 * -1 when it has none. */
s32 func_001B5688(BtlUnit *unit) {
    s32 skills[3];
    u16 count = 0;
    s32 skill;

    if (btlCheckSpecialAbility(&unit->partyRecord, 0x22C)) {
        skills[count] = 0x96;
        count++;
    }
    if (btlCheckSpecialAbility(&unit->partyRecord, 0x22B)) {
        skills[count] = 0x95;
        count++;
    }
    if (btlCheckSpecialAbility(&unit->partyRecord, 0x22A)) {
        skills[count] = 0x94;
        count++;
    }
    if (count == 0) {
        return -1;
    }
    skill = skills[effMiscRandMod(0, count)];
    btlBossDebugPrintf(D_004159C0, skill);
    return skill;
}

s32 btlHasHighPriorityState(void) {
    BattleSelectionWork *table;
    s32 index;
    if (btlCommandPanelWork->state == 2) {
        table = btlLinkedSelectionTaskBuffer;
        index = table->selectedRow;
        if (table->rowFade[index - 1] >= 0x80) {
            if (table->positions[index - 1].x >= 0xB) {
                return 1;
            }
        }
    }
    return 0;
}

s32 btlCountFlaggedSceneActors(void) {
    BattleController *controller = (BattleController *)btlGetRuntime();
    BtlUnit *unit;
    s32 count = 0;
    for (unit = controller->actors; unit != 0; unit = unit->nextActor) {
        if ((btlUnitStatusPair(unit) & 0x321) == 0x301) {
            if (!(unit->partyRecord.status & 0x800)) {
                count++;
            }
        }
    }
    return count;
}

extern s16 btlGetActorIdForClass(s8);

extern void btlGetActorClassPair(s8, u32 *, u32 *);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415878);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415898);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004158B8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004158D8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004158F8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415920);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415948);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415970);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004159A0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004159C0);

void btlInitializeCommandPanelSlotTables(void) {
    u32 tableA[5] = {0x40, 0x30, 0x20, 0x10, 0};
    u32 tableB[5] = {0, 0x10, 0x20, 0x30, 0x40};
    s32 i;

    btlCommandPanelWork = sdfAllocAndClearQuadwords(0xCC);
    btlCommandPanelWork->state = 1;
    btlCommandPanelWork->classIndex = 0;
    btlCommandPanelWork->labelEntry =
        btlGetActorIdForClass(btlCommandPanelWork->classIndex);
    btlGetActorClassPair(btlCommandPanelWork->classIndex,
                         &btlCommandPanelWork->classX,
                         &btlCommandPanelWork->classY);
    for (i = 0; i < 5; i++) {
        btlCommandPanelWork->slotsA[i].unk01 = 0;
        btlCommandPanelWork->slotsA[i].fadeValue = tableB[i];
        btlCommandPanelWork->slotsB[i].unk01 = 1;
        btlCommandPanelWork->slotsB[i].fadeValue = tableA[i];
    }
}

s16 btlGetActorIdForClass(s8 classId) {
    s16 table[8] = {0x13, 0x17, 0x15, 0x16, 0x18, 0x14, 0x14, 0x14};
    return table[classId];
}

void btlGetActorClassPair(s8 classId, u32 *first, u32 *second) {
    u32 pairs[14] = {0x19, 4, 0x27, 4, 0x27, 4, 0x35, 4, 0x43, 4, 0x51, 4, 0x5F, 4};
    *first = pairs[classId * 2];
    *second = pairs[classId * 2 + 1];
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B5A20);

typedef struct BattleCmdPanelGridTable {
    u8 values[30];
} BattleCmdPanelGridTable;

extern const BattleCmdPanelGridTable D_00415B80;

extern const BattleCmdPanelGridTable D_00415BA0;

void btlPopulateCommandPanelGrid(void) {
    BattleCmdPanelGridTable table1 = D_00415B80;
    BattleCmdPanelGridTable table2 = D_00415BA0;
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B6180);

typedef struct BattleCornerFadeDefaults {
    s16 values[20];
} BattleCornerFadeDefaults;

typedef struct BattleCornerPhaseDefaults {
    u32 values[12];
} BattleCornerPhaseDefaults;

extern const BattleCornerFadeDefaults D_00415C30;

extern const BattleCornerPhaseDefaults D_00415C58;

void func_001B6438(void) {
    BattleCornerFadeDefaults limits = D_00415C30;
    BattleCornerPhaseDefaults phases = D_00415C58;
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

extern void func_00306C28(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);

typedef struct BattlePanelColors {
    u32 values[4];
} BattlePanelColors;

extern const BattlePanelColors D_00415CE8;

typedef struct BattleClassLabelOffsets {
    s32 values[8][2];
} BattleClassLabelOffsets;

extern const BattlePanelColors D_00415C88;

extern const BattleClassLabelOffsets D_00415C98;

extern void func_001B6180(void);

void func_001B6A20(void) {
    BattlePanelColors colors = D_00415C88;
    BattleClassLabelOffsets offsets = D_00415C98;
    if (btlCommandPanelWork->state < 5) {
        if (btlCommandPanelWork->state > 0) {
            colors.values[0] = btlCommandPanelWork->cornerFade[0] | 0x80808000;
            colors.values[1] = btlCommandPanelWork->cornerFade[1] | 0x80808000;
            colors.values[2] = btlCommandPanelWork->cornerFade[2] | 0x80808000;
            colors.values[3] = btlCommandPanelWork->cornerFade[3] | 0x80808000;
            func_00306C28(0x50, 0x968, 0, colors.values, 0, btlResourceBlock->resA, 0x1E, 0x53);
            func_001B6180();
            colors.values[0] = btlCommandPanelWork->classFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            func_00306C28((btlCommandPanelWork->classX + 5) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1A, 0x53);
            func_00306C28((btlCommandPanelWork->classX + 0x37) << 4,
                         (btlCommandPanelWork->classY + 0x12D) << 3,
                         0, colors.values, 0, btlResourceBlock->resA, 0x1B, 0x53);
            colors.values[0] = btlCommandPanelWork->labelFade | 0x80808000;
            colors.values[1] = colors.values[0];
            colors.values[2] = colors.values[0];
            colors.values[3] = colors.values[0];
            {
                s32 x = btlCommandPanelWork->classX + 5;
                s32 y = btlCommandPanelWork->classY + 0x12D;
                func_00306C28((x + offsets.values[btlCommandPanelWork->classIndex][0]) << 4,
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B6CA8);

void func_001B6FC0(BtlUnit *unused, BattleMirroredSpriteRecord *records, s32 count) {
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
                scale = scale >> 1;
                height = btlResourceBlock->resA->workEntries[record->slot].sourceHeight;
                scaledWidth = width * scale;
                scaledHeight = height * scale;
                record->active = 2;
                record->restoredHeight = height;
                x -= (scaledWidth - width) >> 1;
                y -= (scaledHeight - height) >> 1;
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

void func_001B70B8(BtlUnit *unused, BattleMirroredSpriteRecord *records, s32 count) {
    BattlePanelColors colors = D_00415CE8;
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
                func_00306C28(record->x << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
                func_00306C28(record->secondX << 4, record->y << 3, 0, colors.values, 0, btlResourceBlock->resA, record->slot, 0x53);
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

extern u8 D_00436800;

extern u8 btlResourceBlockLoaded;

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415B80);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415BA0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415BC0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415BD0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415C00);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415C30);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415C58);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415C88);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415C98);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415CD8);

const BattlePanelColors D_00415CE8 = {{0x80808080, 0x80808080, 0x80808080, 0x80808080}};

void btlPanelResourcesLoad(void) {
    u8 params[16];
    SdfMemBlock *handle;
    BtlResBlock *block;
    if (D_00436800 == 0) {
        handle = sdfAllocGeneralBlock(0x28);
        block = (BtlResBlock *)sdfResourceRetainAddress(handle);
        btlResourceBlock = block;
        block->unk0 = handle;
        block->resA = 0;
        block->resB = 0;
        block->unk1C = 0;
        btlResourceBlock->nameA = (s32)(u32)sdfReadNamedResource("/battle/panel/batle_01.spr", params, 0);
        btlResourceBlock->nameB = (s32)(u32)sdfReadNamedResource("/battle/panel/batle_02.spr", params, 0);
        btlResourceBlock->nameC = (s32)(u32)sdfReadNamedResource("/battle/panel/battle_03.spr", params, 0);
        btlResourceBlockLoaded = 0;
    }
    D_00436800 = 1;
}

typedef struct BtlWorkRes {
    u8 pad[0x4D8];
    EffectSlotSet *resA;
    EffectSlotSet *resB;
    EffectSlotSet *resC;
} BtlWorkRes;

void btlLoadResourceBlock(void) {
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
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
    BtlWorkRes *work = (BtlWorkRes *)btlGetRuntime();
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

extern SceneSlotFadeWork *D_00438F54;

extern ActorSlotOrder *D_00438F58[2];

extern void btlClearTaskActorSlots(void);

extern s32 func_001B8368(s32);

s32 btlResetSceneSlotFades(void) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    SceneSlotFadeWork *state;
    ActorSlotOrder **bank;
    s32 kind;
    s32 count;
    s32 i;

    memset(D_00438F54, 0, sizeof(*D_00438F54));
    memset(D_00438F58[0], 0, sizeof(*D_00438F58[0]));
    memset(D_00438F58[1], 0, sizeof(*D_00438F58[1]));
    btlTrackedTaskHandles->fadeKindsCached = 0;
    btlCountSceneSlots();
    if (battle->mode == 1) {
        kind = 0;
        count = btlTrackedTaskHandles->fadeKindACount;
        D_00438F54->enabled[1] = 0;
    } else {
        kind = 1;
        count = btlTrackedTaskHandles->fadeKindBCount;
        D_00438F54->enabled[0] = 0;
    }
    bank = &D_00438F58[kind];
    D_00438F54->currentIndex = count;
    D_00438F54->lastIndex = count;
    D_00438F54->bank = kind;
    D_00438F54->enabled[kind] = 1;
    D_00438F54->phase[0][kind] = 1;
    D_00438F54->timer = 0;
    (*bank)->state[0] = 1;
    state = D_00438F54;
    state->unk18 = 0;
    state->completed = 0;
    for (i = 0; i < count; i++) {
        (*bank)->unk6C[i] = 30.0f;
        (*bank)->unk8C[i] = 130;
    }
    btlInitializeActorSlotOrder(kind, count);
    btlClearTaskActorSlots();
    return func_001B8368(kind);
}

/* Clear each task's eight opaque words, forward for variant 1 and backward otherwise. */
void btlClearTaskActorSlots(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    ActionStateLink *node;
    BtlUnit *owner;
    BattleActionSlot *slot;
    BattleActionSlot *reverse;
    s32 i;
    for (node = work->taskHead; node != 0; node = node->next) {
        owner = node->unit;
        if (owner != 0) {
            if (work->variant == 1) {
                if (owner->status.flags & 0x200) {
                    for (i = 0, slot = &node->actions[0]; i < 8; i++) {
                        slot->word = 0;
                        slot++;
                    }
                }
            } else if (owner->status.flags & 0x400) {
                for (i = 7, reverse = &node->actions[7]; i >= 0; i--) {
                    reverse->word = 0;
                    reverse--;
                }
            }
        }
    }
}

typedef struct ActorOrder12 {
    s32 entries[12];
} ActorOrder12;

typedef struct ActorOrder6 {
    s32 entries[6];
} ActorOrder6;

extern ActorSlotOrder *D_00438F58[2];

extern const ActorOrder12 D_00415D58;

extern const ActorOrder6 D_00415D88;

void btlInitializeActorSlotOrder(s32 selector, s32 count) {
    ActorOrder12 primaryOrder = D_00415D58;
    ActorOrder6 secondaryOrder = D_00415D88;
    s32 *order = selector != 0 ? primaryOrder.entries : secondaryOrder.entries;
    ActorSlotOrder *destination;
    s32 i;

    i = 0;
    if (count > 0) {
        destination = D_00438F58[selector];
        do {
            destination->entries[i] = order[i];
            i++;
        } while (i < count);
    }
}

s32 btlCountSceneSlots(void) {
    struct BattleSceneWork *scene;
    s32 fadeCounts[4];

    scene = (struct BattleSceneWork *)btlGetRuntime();
    return fldCountSceneFadeKinds(scene, fadeCounts);
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B7718);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B7830);

void btlClearSharedBattleStateWords(void) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = 3;
    puVar1 = datGameState->battleFlags;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar1 = 0;
        puVar1 = puVar1 + 1;
    } while (-1 < temp_v0);
}

s32 func_001B7940(s32 id, s32 operation) {
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

void func_001B7A00(void) {
    s32 i;
    for (i = 3; i >= 0; i--) {
        D_003B6928[i] = 0;
    }
}

s32 func_001B7A38(s32 id, s32 operation) {
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
        D_003B6928[word] |= 1 << bit;
        break;
    case 1:
        D_003B6928[word] &= ~(1 << bit);
        break;
    default:
        return ((D_003B6928[word] & (1 << bit)) != 0);
    }
    return 1;
}

extern const char *D_004367B8;

s32 btlSetTaskPhase2(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_004367B8);
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
        task = kwlnTaskGetTaskByName(D_004367CC);
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

/* Mahen entries carry a halfword key and a displayed byte value. */
typedef struct MesWindowItem {
    u16 textId;
    u8 value;
    u8 pad03;
} MesWindowItem;

typedef struct MesWindowConfig {
    MesWindowItem active[9];
} MesWindowConfig;

typedef struct MesWindowSet {
    s8 state;
    u8 pad01[3];
    u32 handle[9];
    u8 pad28[4];
    s16 alpha;
    u8 pad2E[2];
    u32 colors[4];
    s32 x;
    s32 y;
    MesWindowConfig config;
} MesWindowSet;

extern ItfMesSub D_00385A08;

extern u32 btlIsNamedBattleTaskRegistered(void);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

extern s32 func_001B8A80(KwlnTask *);

extern void itfMesCloseAllWindows(KwlnTask *);

s32 btlCreateMahenPanelTask(const MesWindowConfig *config) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    MesWindowSet *work;
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
        if (work->config.active[i].textId != 0) {
            work->handle[i] = itfMesCreateWindow(&D_00385A08);
        }
    }
    work->x = 0x20;
    work->y = 0x20;
    task = kwlnTaskCreate(btlMahenPanelTaskNameRef, 0x2B0E, 1, 1,
                          func_001B8A80, itfMesCloseAllWindows, (u32)work);
    func_00101968(battle->drawTask, task);
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
    MesWindowSet *work;
    KwlnTask *task;

    task = kwlnTaskGetTaskByName(btlMahenPanelTaskNameRef);
    if (task != 0) {
        work = (MesWindowSet *)kwlnTaskGetUserValue(task);
        work->state = 2;
    }
}

typedef struct BtlAnalysisPanelWork {
    s8 state;
    u8 pad01[3];
    s32 unk04;
    s32 duration;
    s32 frame;
    u8 pad10[0x18];
    s32 x;
    s32 y;
    s8 page;
    u8 pad31[0x6F];
} BtlAnalysisPanelWork;

extern u32 btlHasRegisteredAnalysisPanelTask(void);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

extern s32 func_001B9158(KwlnTask *task);

extern void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *task);

s32 btlCreateAnalysisPanelTask(s32 entry, s32 duration) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlAnalysisPanelWork *data;
    KwlnTask *task;

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
    data->unk04 = entry;
    data->frame = 0;
    task = kwlnTaskCreate(btlAnalyzPanelTaskNameRef, 0x2B0E, 1, 1, func_001B9158,
                          btlReleaseTaskAndRefreshCursorIfFlagged, (u32)data);
    func_00101968(context->drawTask, task);
    btlSetTrackedTaskHandle(11, (s32)task);
    context->flags &= ~0x100000;
    return 1;
}

u32 btlHasRegisteredAnalysisPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(11);
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

extern s32 func_001BB5C0(KwlnTask *task);

extern void btlReleaseMessageWindowTask(KwlnTask *task);

extern u32 btlHasRegisteredGuidePanelTask(void);

extern u32 btlHasRegisteredSkillNamePanelTask(void);

extern u32 btlHasRegisteredAphNamePanelTask(void);

extern void btlSetTrackedTaskHandle(s32, s32);

s32 btlCreateGuidePanelTask(s32 windowIndex, s32 entryIndex) {
    BattleController *context = (BattleController *)btlGetRuntime();
    BtlGuidePanelWork *data;
    KwlnTask *task;

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
    task = kwlnTaskCreate(D_004367E0, 0x2B0E, 1, 1, func_001BB5C0,
                          btlReleaseMessageWindowTask, (u32)data);
    func_00101968(context->drawTask, task);
    btlSetTrackedTaskHandle(9, (s32)task);
    return 1;
}

void btlRequestGuidePanelClose(void) {
    u8 *puVar1;
    KwlnTask *temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_004367E0);
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
            task = kwlnTaskGetTaskByName(D_004367E0);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern const char *btlCommandPanelTaskNameRef;

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
    s32 *state;
    if (task != 0) {
        state = (s32 *)kwlnTaskGetUserValue((KwlnTask *)task);
        state[2] = mode;
        if (mode == 0) {
            state[1] = 1;
            state[5] = 0x80;
        } else {
            state[1] = 4;
            state[5] = 0xFF;
            state[6] = 0xFF;
        }
    }
}

void btlInvalidateSceneFadeCounts(s32 kind) {
    btlGetRuntime();
    kwlnTaskGetUserValue((KwlnTask *)btlGetTrackedTaskHandle(7));
    btlTrackedTaskHandles->fadeKindsCached = 0;
}

u32 btlHasRegisteredPsechgPanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(6);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_004367D8);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* B8368 allocates 0x138 bytes. The strip renderer uses the first three rows;
 * the later panel updater handles the other five points and their fades. */
typedef struct BattlePhasePanelWork {
    s32 frames;
    s32 mode;
    s8 phase;
    s8 secondaryPhase;
    u8 pad0A[2];
    s32 unk0C;
    u8 unk10[8];
    s32 waitCounter; /* 0x18: delay before the first slide */
    u8 pad1C[8];
    s32 counter;
    s32 angle;
    u8 pad2C[0xC];
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

typedef struct BtlPsechgPositions {
    BattleSelectionPosition points[5];
} BtlPsechgPositions;

extern const BtlPsechgPositions D_00415DA0;
extern const BtlPsechgPositions D_00415DC8;
extern s32 btlUpdatePhaseGatedTaskUntilTimeout(KwlnTask *);
extern void btlReleasePsechgPanelWork(KwlnTask *);
extern void btlSetTrackedTaskHandle(s32, s32);
extern void func_00101968(KwlnTask *, KwlnTask *);

/* Create the phase-gated PSECHG panel with one of two initial point sets. */
s32 func_001B8368(s32 variant) {
    BtlPsechgPositions firstPoints = D_00415DA0;
    BtlPsechgPositions secondPoints = D_00415DC8;
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *previous = (KwlnTask *)btlGetTrackedTaskHandle(6);
    BattlePhasePanelWork *work;
    BtlPsechgPositions *points;
    s32 *destination;
    s32 *source;
    KwlnTask *task;
    s32 i;

    if (btlHasRegisteredPsechgPanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(previous, 0);
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
    destination = &work->current[0].y;
    source = (s32 *)points->points;
    for (i = 0; i < 5; i++) {
        destination[-1] = source[0];
        destination[0] = source[1];
        destination += 2;
        source += 2;
    }
    task = kwlnTaskCreate(D_004367D8, 0x2B0E, 1, 1, btlUpdatePhaseGatedTaskUntilTimeout,
                          btlReleasePsechgPanelWork, (u32)work);
    func_00101968(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(6, (s32)task);
    return 1;
}


u32 btlHasRegisteredSkillNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_004367C8);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

extern void func_00101968(KwlnTask *, KwlnTask *);

extern s32 btlUpdateSkillNamePanelTask(KwlnTask *);

extern void btlFreeRegisteredTaskData(KwlnTask *);

extern struct FrFontGlyph *itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

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

s32 func_001B8580(const u8 *text) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    KwlnTask *task = (KwlnTask *)btlGetTrackedTaskHandle(1);
    BtlPanelTransitionWork *work;
    struct FrFontGlyph *glyph;

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
    work->width = frFontMeasureLines(glyph);
    frFontQueueGlyphForCurrentDrawBuffer(glyph);
    work->initial[0].x = work->width - work->width / 2 + 0x105;
    work->initial[0].y = 0x40;
    work->initial[1].x = 0x92 - work->width / 2;
    work->initial[1].y = 0x40;
    task = kwlnTaskCreate(D_004367C8, 0x2B0E, 1, 1,
                          btlUpdateSkillNamePanelTask, btlFreeRegisteredTaskData, (u32)work);
    func_00101968(battle->scriptOwner, task);
    work->task = task;
    btlSetTrackedTaskHandle(1, (s32)task);
    return 1;
}

u32 btlHasRegisteredAphNamePanelTask(void) {
    KwlnTask *task;

    task = (KwlnTask *)btlGetTrackedTaskHandle(0);
    if (task != 0) {
        if (kwlnTaskIsRegistered(task) != 0) {
            task = kwlnTaskGetTaskByName(D_004367C4);
            if (task != 0) {
                return 1;
            }
        }
    }
    return 0;
}

typedef struct MsgQueueTaskData {
    KwlnTask *task;
    s32 id;
    s32 unk08;
    s32 counter;
    s32 value;
    s16 fade;
} MsgQueueTaskData;

extern const BattlePanelColors D_004165C0;

extern s32 itfMesMeasureEntryItem(s32, s32, s32);

extern void itfMesBlk24MoveTo(s32, s32, s32);

extern s32 btlDrawTimedDialogTask(KwlnTask *task);

extern void btlReleaseDialogTaskData(KwlnTask *task);

extern s32 btlGetTrackedTaskHandle(s32);

s32 btlReplaceDialogTasksAndQueueMessage(s32 arg0, s32 arg1) {
    BattleController *context = (BattleController *)btlGetRuntime();
    KwlnTask *task = (KwlnTask *)btlGetTrackedTaskHandle(0);
    MsgQueueTaskData *data;

    if (btlHasRegisteredAphNamePanelTask() != 0) {
        kwlnTaskDestroyWithHierarchy(task, 0);
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
    task = kwlnTaskCreate(D_004367C4, 0x2B0E, 1, 1, btlDrawTimedDialogTask,
                          btlReleaseDialogTaskData, (u32)data);
    func_00101968(context->drawTask, task);
    data->task = task;
    btlSetTrackedTaskHandle(0, (s32)task);
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

void func_001B88F8(BtlUnit *unit, s8 style, s8 phase, s32 unused, BattlePanelColors *colors) {
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
        colors->values[0] = 0x80383232;
        colors->values[1] = 0x803C343A;
        colors->values[2] = 0x803C343A;
        colors->values[3] = 0x803C344E;
    }
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B8A80);

extern void itfMesCleanupWindow(s32, s32);

void itfMesCloseAllWindows(KwlnTask *handle) {
    MesWindowSet *set;
    s32 i;
    btlGetRuntime();
    set = (MesWindowSet *)kwlnTaskGetUserValue(handle);
    for (i = 0; i < 9; i++) {
        if (set->config.active[i].textId != 0) {
            itfMesCleanupWindow(set->handle[i], 0);
            itfMesDestroyWindowIfPresent(set->handle[i]);
        }
    }
    sdfReleaseChipBlock(set);
    btlSetTrackedTaskHandle(0xA, 0);
}

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

extern const u32 D_00415DF0[4];

extern const s32 D_00415E00[2][3], D_00415E18[2][3], D_00415E30[2][3], D_00415F08[2][3];

extern const s32 D_00415E48[16][3];

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415D58);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415D88);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415DA0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415DC8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415DF0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415E00);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415E18);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415E30);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415E48);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415F08);

void func_001B8E68(s32 section, s32 delta) {
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

    memcpy(colors, D_00415DF0, sizeof(colors));
    memcpy(primary, D_00415E00, sizeof(primary));
    memcpy(secondary, D_00415E18, sizeof(secondary));
    memcpy(lower, D_00415E30, sizeof(lower));
    memcpy(grid, D_00415E48, sizeof(grid));
    memcpy(footer, D_00415F08, sizeof(footer));

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
        func_00306C28((*strip)[0] << 4, (*strip)[1] << 3, 0, colors, 0,
                     btlResourceBlock->resA, (*strip)[2], 0x53);
    }
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B9158);

extern s32 btlInitCursorAndApplyAction(BtlLinkedCommand *, BtlCamState *);

void btlReleaseTaskAndRefreshCursorIfFlagged(KwlnTask *handle) {
    BtlState *work = (BtlState *)btlGetRuntime();
    ActionStateLink *actor;
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xB, 0);
    actor = work->cameraCommand.link;
    if (actor->unit->partyRecord.status & 0x80) {
        btlInitCursorAndApplyAction(&work->cameraCommand, &work->cameraCommand.camera);
    }
    work->battleFlags |= 0x100000;
}

/* Advance one corner toward the panel boundary before moving to the next edge. */
s32 btlAdvancePanelCornerPhase(BtlLinkedCommand *command, BattlePanelEdgeWork *work) {
    f32 center[4];

    memcpy(center, D_00415F80, sizeof(center));

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

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415F80);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415F90);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B9C70);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BA1E8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415FC0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00415FD0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416000);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416030);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416060);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BAB90);

typedef struct BtlPanelStrip {
    s32 x;
    s32 y;
    s32 texture;
} BtlPanelStrip;

extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004160F0);

void btlDrawThreePanelSpriteStrips(s32 unused, s32 x, s32 y, s32 delta) {
    BtlPanelStrip strips[3] = { {0, 0, 0x40}, {5, 0, 0x41}, {0x6B, 0, 0x42} };
    u32 colors[4] = { 0x80808080, 0x80808080, 0x80808080, 0x80808080 };
    s32 i, j;
    BtlPanelStrip *strip;

    for (strip = strips, i = 0; i < 3; i++, strip++) {
        for (j = 0; j < 4; j++) {
            colors[j] = btlSetSlotLowByteClamped(btlResourceBlock->resA, strip->texture, j, delta);
        }
        func_00306C28((x + strip->x) << 4, (y + strip->y) << 3,
                     0, colors, 0, btlResourceBlock->resA, strip->texture, 0x53);
    }
}



void func_001BB1E8(void *unused, s32 x, s32 y, u32 alpha) {
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
                /* Retail 0x001BB3B4 retains the identical corner 1/3 alpha-clear arms. */
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
        func_00306C28((x + xOffset) << 4, (y + yOffset) << 3,
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BB5C0);

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

void func_001BB988(BattleSceneObject *object, s32 first, s32 second, s32 slot) {
    BtlUnit *unit = object->owner->unit;
    unit->partyRecord.unk1AC = first;
    unit->partyRecord.unk1AE = second;
    unit->partyRecord.actionSlot = slot;
}

s32 btlCountEligibleLinkedActors(BtlState *battle) {
    BtlUnit *unit = battle->units;
    s32 count = 0;
    while (unit != 0) {
        if ((btlUnitStatusPair(unit) & 0x201) == 0x201 &&
            (unit->partyRecord.flags & 2) != 0) {
            count++;
        }
        unit = unit->nextActor;
    }
    return count;
}

extern const char *D_004367DC;

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

extern s32 func_001BBA80(KwlnTask *task);

extern void btlReleaseRegisteredChildTaskWork(KwlnTask *task);

void btlStartRegisteredChildTask(void) {
    BattleController *work = (BattleController *)btlGetRuntime();
    KwlnTask *task = kwlnTaskCreate(D_004367DC, 0x2B0E, 1, 1, func_001BBA80, btlReleaseRegisteredChildTaskWork, (u32)sdfAllocAndClearQuadwords(sizeof(BattleRegisteredPanelWork)));
    func_00101968(work->drawTask, task);
    btlSetTrackedTaskHandle(7, (s32)task);
}

void func_001BBA60(BattleRegisteredPanelWork *work) {
    work->mode = 1;
    work->x = 0x80;
    work->y = 0;
    work->state = 0;
}

extern const u32 D_004163A0[4];

s32 func_001BBA80(KwlnTask *task) {
    u32 colors[4];
    BattleRegisteredPanelWork *work;
    s32 sprite;
    s32 i;

    memcpy(colors, D_004163A0, sizeof(colors));
    if ((((BtlState *)btlGetRuntime())->battleFlags & 0x200) == 0 &&
        (btlTrackedTaskHandles->status.flags & 0x100) == 0) {
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
            func_00306C28(0x19C0, -0x50, 0, colors, 0, btlResourceBlock->resC, 0x29, 0x53);
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
            }
            func_00306C28(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x28, 0x53);
            if (sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x28, i, work->backdropAlpha);
                }
                func_00306C28(0x1AD0, 0x190, 0, colors, 0, btlResourceBlock->resC, 0x3A, 0x53);
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 0x3D, i, work->backdropAlpha);
                }
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 90.0f;
                func_00306C28(0x1CB0, 0x70, 0, colors, 0, btlResourceBlock->resC, 0x3D, 0x53);
                btlResourceBlock->resC->workEntries[0x3D].geometry.angleDegrees = 0.0f;
            }
            for (i = 0; i < 4; i++) {
                colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->spriteAlpha);
            }
            func_00306C28((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                         0, colors, 0, btlResourceBlock->resC, sprite, 0x53);
            if (work->mode == 4 || sprite == 0x26) {
                for (i = 0; i < 4; i++) {
                    colors[i] = btlSetSlotLowByteClamped(btlResourceBlock->resC, sprite, i, work->overlayAlpha);
                }
                func_00306C28((work->x + 0x1BB) << 4, (work->y + 0x36) << 3,
                             0, colors, 0, btlResourceBlock->resC, 0x3B, 0x53);
            }
        }
    }
    return 0;
}

void btlReleaseRegisteredChildTaskWork(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(7, 0);
}


typedef struct BattlePhaseSlotIds { s32 values[3]; } BattlePhaseSlotIds;

typedef struct BattlePhaseFadeLimits { s32 values[3][4]; } BattlePhaseFadeLimits;

typedef struct BattlePhaseXBounds { s32 values[3][2]; } BattlePhaseXBounds;

extern const BattlePhaseSlotIds D_004163B0;

extern const BattlePhaseFadeLimits D_004163C0;

extern const BattlePhaseXBounds D_004163F0;

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004163A0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004163B0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004163C0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004163F0);

void func_001BC138(BattlePhasePanelWork *work) {
    BattlePhaseSlotIds slots = D_004163B0;
    BattlePhaseFadeLimits limits = D_004163C0;
    BattlePhaseXBounds bounds = D_004163F0;
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

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416428);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BC618);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416448);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416458);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416468);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BC8A8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416498);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BCFB0);

extern s32 btlGetNamedTaskPairStatusOrUnavailable();

extern void func_001BC138();

extern void func_001BC618();

extern void func_001BC8A8();

extern void func_001BCFB0();

s32 btlUpdatePhaseGatedTaskUntilTimeout(KwlnTask *task) {
    BattlePhasePanelWork *state = (BattlePhasePanelWork *)kwlnTaskGetUserValue(task);
    s32 frame;
    if ((u32)((btlGetNamedTaskPairStatusOrUnavailable() - 1) & 0xFF) < 2U) {
        return 0;
    }
    func_001BC138(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001BC618(state);
    }
    func_001BC8A8(state);
    if (btlTrackedTaskHandles->phaseGate == 0) {
        func_001BCFB0(state);
    }
    frame = state->frames + 1;
    state->frames = frame;
    return frame < 0x32 ? 0 : -1;
}

void btlReleasePsechgPanelWork(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(6, 0);
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004164B8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004164C8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BD6E8);

void btlReleaseBattleScratchBlocks(void) {
    sdfReleaseChipBlock(D_00438F4C);
    sdfReleaseChipBlock((void *)D_00438F50);
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416520);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BD9A0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416560);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BDF20);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BE9E8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BEBD0);

void btlReleaseCmsleffPanelWork(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    btlSetTrackedTaskHandle(5, 0);
}

extern const BattlePanelColors D_004165B0;

extern void btlUpdatePanelTransitionGradients(BtlPanelTransitionWork *);

s32 btlUpdateSkillNamePanelTask(KwlnTask *task) {
    BattlePanelColors colors = D_004165B0;
    BtlPanelTransitionWork *work;
    s16 *fade;
    s32 i;
    s32 width;
    struct FrFontGlyph *glyph;

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
    frFontDrawGlyphWithSharedFlags(glyph, 1);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)glyph);
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
    func_00306C28(work->initial[1].x << 4, (work->initial[1].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x15, 0x53);
    colors.values[0] = work->fadeLevels[1] | 0x80808000;
    colors.values[1] = work->fadeLevels[0] | 0x80808000;
    colors.values[2] = work->fadeLevels[3] | 0x80808000;
    colors.values[3] = work->fadeLevels[2] | 0x80808000;
    func_00306C28(work->initial[0].x << 4, (work->initial[0].y + work->verticalShift) << 3,
                  0, colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    colors.values[0] = work->fadeLevels[2] | 0x80808000;
    colors.values[1] = work->fadeLevels[3] | 0x80808000;
    colors.values[2] = work->fadeLevels[1] | 0x80808000;
    colors.values[3] = work->fadeLevels[0] | 0x80808000;
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    func_00306C28((0x100 - width / 2) << 4, (work->initial[1].y + work->verticalShift) << 3,
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

void btlFreeRegisteredTaskData(KwlnTask *handle) {
    btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(1, 0);
}

extern s8 D_0037F53D[];

void btlToggleModelFlagOnInput(void) {
    if (D_0037F53D[0] < 0) {
        if (mdlFlagTest(0xC0E) != 0) {
            mdlFlagClear(0xC0E);
        } else {
            mdlFlagSet(0xC0E);
        }
    }
}

s32 btlDrawTimedDialogTask(KwlnTask *task) {
    BattlePanelColors colors = D_004165C0;
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
    func_00306C28(((width >> 4) - half + 0x105) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = (width >> 4) << 4;
    func_00306C28((0x100 - half) << 4, 0x200, 0,
                  colors.values, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_00306C28((0x92 - half) << 4, 0x200, 0,
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

void btlReleaseDialogTaskData(KwlnTask *handle) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)kwlnTaskGetUserValue(handle);
    itfMesCleanupWindow(*(s32 *)(window + 4), 0);
    sdfReleaseChipBlock(window);
    btlSetTrackedTaskHandle(0, 0);
}

void btlCreateMessageWindow(void) {
    u8 *window;
    btlGetRuntime();
    window = (u8 *)sdfAllocAndClearQuadwords(0x40);
    D_004367F8 = (u32)window;
    *(s32 *)(window + 0x10) = 0x14;
    *(s32 *)(window + 0x18) = 0x1800080;
    *(s32 *)(window + 0x1C) = 0x40800080;
    *(s32 *)(window + 0x20) = 0x40800080;
    *(s32 *)(window + 0x24) = 0x60808080;
    *(s32 *)(window + 0x38) = 0xBB;
    *(s32 *)(window + 0x3C) = 0x196;
    btlSetTrackedTaskHandle(4, 1);
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004165A0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004165B0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004165C0);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BF978);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004165E0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004165F0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416600);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416610);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416620);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BFB58);

void btlReleaseRegisteredTaskBuffer(s32 unused) {
    btlGetRuntime();
    sdfReleaseChipBlock((void *)D_004367F8);
    D_004367F8 = 0;
    btlSetTrackedTaskHandle(4, 0);
}

s32 func_001C0008(void) {
    s32 i;
    u32 *flags;
    if (mdlFlagTest(0xb8f)) {
        return 0;
    }
    flags = datGameState->battleFlags;
    for (i = 0; i < 4; i++) {
        if (flags[i]) {
            return 0;
        }
    }
    return 1;
}


s32 func_001C0080(s32 arg0) {
    s32 count;
    s32 i;

    func_001B7A00();
    count = func_001AC750(arg0, &D_003B5D10);
    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (func_001B7940(D_003B5D10.entries[i].skill, 2) == 0) {
                return 1;
            }
        }
        return 0;
    }
    return count;
}

extern s32 btlGetTaskState6(void);

INCLUDE_ASM(const s32, "game/code_001B2AF8", btlGetTaskState6);



extern s32 func_001B7940(s32, s32);

s32 btlClearFlagEntries(void) {
    u16 count;
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    BattleRosterTable *table;
    s32 i;
    if (task != 0) {
        table = fldGetCachedSceneActorNameAndId(kwlnTaskGetUserValue(task), &count);
        for (i = 0; i < count; i++) {
            func_001B7940(table->entries[i].skill, 0);
        }
        return 1;
    }
    return 0;
}

extern const char *D_004367EC;

extern s32 dspCloseChannel(void);

s32 btlDestroyTaskC(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_004367EC);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xC) != 0) {
            kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0xC), 0);
        }
        return 1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416650);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C0240);

extern s32 evtFinishMessageWindowAndNotify();

void btlFinishTrackedBattleTaskAndCloseWindow(KwlnTask *handle) {
    u8 *work = (u8 *)btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xC, 0);
    *(u32 *)(work + 0x218) |= 0x100000;
    evtFinishMessageWindowAndNotify();
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416680);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C0630);

/* The creator allocates and clears the complete 24-byte phase work. */
typedef struct BtlTutorialDialogWork {
    u8 phase;
    u8 reserved[0x17];
} BtlTutorialDialogWork;
typedef char BtlTutorialDialogWorkSize[(sizeof(BtlTutorialDialogWork) == 0x18) ? 1 : -1];
extern s32 evtGetMessageWindowControlState(void);
extern s32 fldGetActiveSceneGroupValue(void);
extern void func_0026C900(void);
extern BtlRuntimeTask *btlCreateCommandSoundTask(s32, s32);
extern u64 btlStartTask(void *);
extern void func_001E3108(void *, f32 *);
extern void btlCopyUnitRotationQuaternion(BtlUnit *, void *);
extern void func_001C7DB8(s8, s32);
extern s32 dspStartEntry(s32);
extern s32 dspCloseChannel(void);
extern BtlRuntimeTask *btlCreateFloatTask29(ActionStateLink *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern BtlRuntimeTask *btlCreateNotifyingCameraKeyframeTask(ActionStateLink *, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32, f32);

s32 func_001C0828(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlTutorialDialogWork *work = (BtlTutorialDialogWork *)kwlnTaskGetUserValue(task);
    s32 windowState = evtGetMessageWindowControlState();
    BtlUnit *unit;
    s32 count;
    BtlUnit *eligible[3];
    s32 handled;
    ActionStateLink *group;

    switch ((s8)work->phase) {
    case 0:
        if (windowState != 1) {
            dspStartEntry(5);
            work->phase++;
        }
        break;
    case 1:
        count = 0;
        for (unit = battle->units; unit != 0; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    count++;
                }
            }
        }
        if (count != 2) {
            if (count == 3) {
                btlStartTask(btlCreateFloatTask29(0,
                    0x1.594cccp+8f /* 345.3 */, -0x1.266666p+6f /* -73.6 */,
                    -0x1.4a9998p+7f /* -165.3 */, 0x1.604188p-5f /* 0.043 */,
                    -0x1.c51eb8p-1f /* -0.885 */, 0x1.d70a3cp-4f /* 0.115 */,
                    -0x1.b74bc6p-2f /* -0.429 */, 0x1.b41998p+8f /* 436.1 */,
                    -0x1.d33332p+4f /* -29.2 */, -0x1.beccccp+6f /* -111.7 */,
                    0x1.db22dp-6f /* 0.029 */, -0x1.c49ba4p-1f /* -0.8839999 */,
                    0x1.5c28f4p-4f /* 0.08499999 */, -0x1.c18936p-2f /* -0.439 */,
                    40.0f, 40.0f));
            }
        } else {
            btlStartTask(btlCreateFloatTask29(0,
                0x1.d0ccccp+7f /* 232.4 */, -0x1.0d6666p+7f /* -134.7 */,
                -0x1.cc6666p+7f /* -230.2 */, 0x1.89374ap-8f /* 0.006 */,
                0x1.d0e56p-1f /* 0.908 */, -0x1.cac082p-8f /* -0.007 */,
                0x1.958106p-2f /* 0.396 */, 0x1.319998p+8f /* 305.6 */,
                -0x1.3b9998p+6f /* -78.89999 */, -158.0f,
                -0x1.0624dcp-8f /* -0.004 */, 0x1.cf5c28p-1f /* 0.905 */,
                -0x1.ba5e34p-6f /* -0.027 */, 0x1.9db22cp-2f /* 0.404 */,
                40.0f, 40.0f));
        }
        work->phase++;
        break;
    case 2:
        if (windowState != 1) {
            dspStartEntry(6);
            work->phase++;
        }
        break;
    case 3:
        work->phase++;
        unit = battle->units;
        count = 0;
        memset(eligible, 0, sizeof(eligible));
        for (; unit != 0; unit = unit->nextActor) {
            if (unit->status.flags & 1) {
                if (unit->status.flags & 0x200) {
                    if (count < 3) {
                        eligible[count] = unit;
                        count++;
                    }
                }
            }
        }
        if (count != 0) {
            unit = eligible[effMiscRandMod(0, count)];
            if (unit != 0) {
                handled = 0;
                switch (unit->partyRecord.unitId) {
                case 1:
                    btlStartTask(btlCreateNotifyingCameraKeyframeTask(0,
                        0x1.7f3332p+5f /* 47.9 */, -0x1.bcccccp+5f /* -55.6 */,
                        0x1.143332p+7f /* 138.1 */, 0x1.26e978p-6f /* 0.018 */,
                        -0x1.ed0e56p-1f /* -0.963 */, 0x1.3f7cecp-3f /* 0.156 */,
                        -0x1.645a1cp-3f /* -0.174 */, 0x1.d4ccccp+5f /* 58.6 */,
                        -0x1.7a6666p+5f /* -47.3 */, 0x1.0f9998p+7f /* 135.8 */,
                        0x1.ba5e34p-6f /* 0.027 */, -0x1.e872bp-1f /* -0.954 */,
                        0x1.7ced9p-3f /* 0.186 */, -0x1.89374ap-3f /* -0.192 */,
                        40.0f, 30.0f));
                    handled = 1;
                    break;
                case 4:
                    btlStartTask(btlCreateNotifyingCameraKeyframeTask(0,
                        0x1.2a6666p+6f /* 74.6 */, -0x1.833332p+6f /* -96.8 */,
                        86.5f, 0x1.16872ap-5f /* 0.034 */,
                        -0x1.d99998p-1f /* -0.925 */, 0x1.20c49ap-3f /* 0.141 */,
                        -0x1.4ac082p-2f /* -0.323 */, 0x1.ccccccp+6f /* 115.2 */,
                        -0x1.34ccccp+5f /* -38.6 */, 0x1.513332p+6f /* 84.3 */,
                        0x1.645a1cp-4f /* 0.087 */, -0x1.bb645ap-1f /* -0.866 */,
                        0x1.a5e352p-3f /* 0.206 */, -0x1.b53f7cp-2f /* -0.427 */,
                        40.0f, 40.0f));
                    handled = 1;
                    break;
                case 5:
                    btlStartTask(btlCreateNotifyingCameraKeyframeTask(0,
                        0x1.36ccccp+6f /* 77.7 */, -0x1.bd9998p+6f /* -111.4 */,
                        0x1.d8ccccp+6f /* 118.2 */, 0x1.0e5604p-5f /* 0.033 */,
                        -0x1.dcac08p-1f /* -0.931 */, 0x1.26e978p-3f /* 0.144 */,
                        -0x1.322d0ep-2f /* -0.299 */, 0x1.21p+7f /* 144.5 */,
                        -16.5f, 0x1.8d6666p+7f /* 198.7 */,
                        0x1.581062p-5f /* 0.042 */, -0x1.d89374p-1f /* -0.923 */,
                        0x1.4dd2fp-3f /* 0.163 */, -0x1.3f7cecp-2f /* -0.312 */,
                        40.0f, 60.0f));
                    handled = 1;
                    break;
                case 6:
                    btlStartTask(btlCreateNotifyingCameraKeyframeTask(0,
                        -0x1.c86666p+6f /* -114.1 */, -160.0f,
                        0x1.a73332p+6f /* 105.8 */, 0x1.0624dcp-9f /* 0.002 */,
                        -0x1.c28f5cp-1f /* -0.88 */, -0x1.0624dcp-5f /* -0.032 */,
                        0x1.ccccccp-2f /* 0.45 */, -0x1.8cccccp+6f /* -99.2 */,
                        -0x1.c59998p+6f /* -113.4 */, 0x1.206666p+7f /* 144.2 */,
                        -0x1.6872bp-5f /* -0.044 */, -0x1.dd2f1ap-1f /* -0.932 */,
                        0x1.4fdf3ap-4f /* 0.08199999 */, 0x1.449ba4p-2f /* 0.317 */,
                        40.0f, 50.0f));
                    handled = 1;
                    break;
                case 2:
                case 3:
                case 7:
                case 8:
                    break;
                }
                if (handled) {
                    func_001E3108(unit, battle->cameraCommand.translation);
                    btlCopyUnitRotationQuaternion(unit, battle->cameraCommand.rotation);
                }
            }
        }
        break;
    case 4:
        if (windowState != 1) {
            dspStartEntry(7);
            work->phase++;
        }
        break;
    case 5:
        btlStartTask(btlCreateCommandSoundTask(0, 3));
        work->phase++;
        break;
    case 6:
        if (windowState != 1) {
            work->phase++;
        }
        break;
    default:
        group = (ActionStateLink *)fldGetActiveSceneGroupValue();
        if (group != 0 && (group->pendingFlags & 8)) {
            if (group->unit->status.flags & 0x200) {
                btlStartTask(btlCreateCommandSoundTask((s32)group, 9));
            }
        }
        dspCloseChannel();
        func_001C7DB8(1, 8);
        return -1;
    }
    if (windowState == 1) {
        func_0026C900();
    }
    return 0;
}


extern const char *D_004367F0;

s32 btlDestroyTaskD(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_004367F0);
    if (task != 0) {
        dspCloseChannel();
        if (btlGetTrackedTaskHandle(0xD) != 0) {
            kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(0xD), 0);
        }
        return 1;
    }
    return 0;
}

void btlReleaseDialogTaskAndMarkBattleState(KwlnTask *handle) {
    BattleController *battle = (BattleController *)btlGetRuntime();
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(handle));
    btlSetTrackedTaskHandle(0xD, 0);
    battle->flags |= 0x100000;
    btlTrackedTaskHandles->status.flags &= ~0x100;
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

INCLUDE_RODATA(const s32, "game/code_001B2AF8", btlSoundSlotDefaults);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C0EF0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416740);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C1300);

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

extern const BattleActorPanelInitialPositions D_00416780;
extern s32 func_001C1F10(KwlnTask *);
extern void btlReleaseStwrPanelResource(KwlnTask *);

void btlInitializeStaffActorPanels(void) {
    BattleActorPanelInitialPositions positions = D_00416780;
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
    task = kwlnTaskCreate(D_004367CC, 0x2B0E, 1, 1, func_001C1F10,
                          btlReleaseStwrPanelResource, (u32)work);
    func_00101968(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(3, (s32)task);
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C16B0);

typedef struct BattleActorPanelPositions {
    BattleSelectionPosition entries[3];
} BattleActorPanelPositions;

extern const BattleActorPanelPositions D_00416798;

extern s32 func_001C16B0(s8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416780);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416798);

void btlUpdateActorPanelPresentation(BtlState *battle, BattleActorPanelWork *work) {
    BattleActorPanelPositions positions = D_00416798;
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
    func_001C16B0(0);
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
            actor = actor->nextActor;
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
                        func_001C16B0(1);
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
            actor = actor->nextActor;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C1F10);

void btlReleaseStwrPanelResource(KwlnTask *task) {
    sdfReleaseResourceAllocation(*(struct SdfMemBlock **)kwlnTaskGetUserValue(task));
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

extern const BattleReservePositions D_004167E0;
extern const char *D_004367D0;
extern s32 func_001C3168(KwlnTask *);
void btlReleaseTrackedTaskResource(void);
void btlSlotBankPromoteStates(BattleActorPanelWork *bank);

/* Build the reserve actor panels: place one row per inactive party member and start the panel task. */
void func_001C2450(void) {
    s32 i;
    BattleReservePositions positions = D_004167E0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleActorPanelWork *work = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
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
    allocation = sdfAllocGeneralBlock(0x368);
    memory = (void *)sdfResourceRetainAddress(allocation);
    memset(memory, 0, 0x368);
    work->reserveUnitAllocation = allocation;
    task = kwlnTaskCreate(D_004367D0, 0x2B0E, 1, 1, func_001C3168, (TaskDestroy)btlReleaseTrackedTaskResource, (u32)memory);
    func_00101968(battle->scriptOwner, task);
    btlSetTrackedTaskHandle(8, (s32)task);
    btlSlotBankPromoteStates(work);
    work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
}

extern const BattleReservePositions D_00416800;
extern void func_001C35F0(ActionStateLink *, s8, s8);
extern void sndSetStationedSeVolume(u32);

/* Per-frame update of the reserve actor panels: handle the selection input and slide the rows in or out. */
s32 func_001C26E0(KwlnTask *task, BtlState *battle) {
    BattleReservePositions positions = D_00416800;
    BattleActorPanelWork *work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
    s32 count = work->reserveCount;
    KwlnTask *commandTask = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    BattleSceneObject *command;
    s32 i;

    if (commandTask != NULL) {
        command = (BattleSceneObject *)kwlnTaskGetUserValue(commandTask);
        if (command->state == 6) {
            if (D_0037F510.confirm < 0) {
                work->partyRecordIndex = datGameState->partyOrder[work->selectedReserveIndex + work->activeCount];
                sndSetStationedSeVolume(8);
                command->state = 8;
                command->commandData->phase = 9;
            } else if (D_0037F510.unk23 < 0) {
                btlSlotBankPromoteStates(work);
                command->state = 7;
                sndSetStationedSeVolume(0xA);
                func_001C35F0(command->owner, 0, 0);
            } else {
                if (D_0037F510.unk25 & 2) {
                    if (work->selectedReserveIndex != count - 1 || D_0037F510.unk25 < 0) {
                        work->selectedReserveIndex++;
                        if (work->selectedReserveIndex >= count) {
                            work->selectedReserveIndex = 0;
                        }
                        sndSetStationedSeVolume(0);
                        btlSlotBankPromoteStates(work);
                        work->reserveEntries[work->selectedReserveIndex].presentation.presentationState = 2;
                    }
                } else if (D_0037F510.unk24 & 2) {
                    if (work->selectedReserveIndex != 0 || D_0037F510.unk24 < 0) {
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C2A98);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C2EA8);

extern void btlUpdateActorPanelHighlights(BtlUnit *, BattleActorPanelWork *, s32, s8);
extern void func_001C6010(BtlUnit *, BattleActorPanelWork *, s16, s32, s8);
extern void func_001C6320(BtlUnit *, BattleStatPulse *, s32, s32, s16, s32, s32);
extern s32 func_0035C860(char *, const char *, ...);
extern void func_001C2EA8(BtlUnit *, BattleActorPanelWork *, s32);
extern void func_001B6CA8(s32, s32, u32, const char *, s32);
extern const BattlePanelColors D_00416830;
extern const char D_00436818[];

s32 func_001C3168(KwlnTask *task) {
    BtlUnit *unit;
    KwlnTask *panelTask;
    BattleActorPanelWork *work;
    char text[32];
    BattlePanelColors colors = D_00416830;
    BtlState *battle = (BtlState *)btlGetRuntime();
    s32 reserveCount;
    s32 i;

    if ((battle->battleFlags & 0x200) == 0) {
        return 0;
    }
    unit = (BtlUnit *)kwlnTaskGetUserValue(task);
    panelTask = kwlnTaskGetTaskByName(D_004367CC);
    work = (BattleActorPanelWork *)kwlnTaskGetUserValue(panelTask);
    reserveCount = work->reserveCount;
    if (func_001C26E0(panelTask, battle) == 0) {
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
            func_001C4900(unit, work, i, 1);
            func_001C7020(unit, work, i, 1, work->reserveEntries[i].presentation.fade,
                          work->reserveEntries[i].position[0], work->reserveEntries[i].position[1]);

            if (work->reserveEntries[i].presentation.fade >= 0x80) {
                func_001C6320(unit, &work->reserveEntries[i].presentation.hpBarPulse, work->reserveEntries[i].position[0], work->reserveEntries[i].position[1],
                              work->reserveEntries[i].presentation.fade, i, 2);
                func_001C6010(unit, work, work->reserveEntries[i].presentation.fade, i, 2);
                func_001C6320(unit, &work->reserveEntries[i].presentation.mpBarPulse, work->reserveEntries[i].position[0], work->reserveEntries[i].position[1],
                              work->reserveEntries[i].presentation.fade, i, 3);
                func_001C6010(unit, work, work->reserveEntries[i].presentation.fade, i, 3);
            }

            func_001C2A98(unit, work, i);
            func_001C2EA8(unit, work, i);

            if ((unit->partyRecord.flags & 0x1010) == 0) {
                u32 tint = 0x80808000 | (u32)(s32)work->reserveEntries[i].presentation.fade;
                colors.values[0] = tint;
                colors.values[1] = tint;
                colors.values[2] = tint;
                colors.values[3] = tint;
                func_00306C28((work->reserveEntries[i].position[0] + 0x22) << 4, (work->reserveEntries[i].position[1] + 0x46) << 3,
                              0, colors.values, 0, btlResourceBlock->resA, 0xE, 0x53);
            }

            baseFade = work->reserveEntries[i].presentation.fade;
            func_0035C860(text, D_00436818, unit->partyRecord.hp);
            color = btlGetVitalTextColor(unit, unit->partyRecord.hp, unit->partyRecord.maxHp, 0);
            color = ((u32)(baseFade + work->reserveEntries[i].presentation.hpHighlightLevel) << 24) |
                    (color & 0x00FFFFFF);
            func_001B6CA8(work->reserveEntries[i].position[0] + 0x50,
                          work->reserveEntries[i].position[1] + 0x26, color, text, 0x100);

            func_0035C860(text, D_00436818, unit->partyRecord.mp);
            color = btlGetVitalTextColor(unit, unit->partyRecord.mp, unit->partyRecord.maxMp, 1);
            color = ((u32)(baseFade + work->reserveEntries[i].presentation.mpHighlightLevel) << 24) |
                    (color & 0x00FFFFFF);
            func_001B6CA8(work->reserveEntries[i].position[0] + 0x48,
                          work->reserveEntries[i].position[1] + 0x3D, color, text, 0x100);
            i++;
        } while (i < reserveCount);
    }
    return 0;
}

void btlReleaseTrackedTaskResource(void) {
    sdfReleaseResourceAllocation(*(struct SdfMemBlock **)(kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC)) + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

typedef struct UiSlotEntry {
    u8 pad00[0x18];
    s8 state; /* 0x18 */
    u8 pad19[0x277];
} UiSlotEntry; /* 0x290 */

typedef struct UiSlotRow {
    u8 pad00[0x10];
    u8 state;
    u8 value;
} UiSlotRow;

typedef struct UiInputState {
    u8 pad00[0x3C];
    u32 flags; /* 0x3C */
} UiInputState;



extern const char *D_004367CC;



extern s32 btlGetRuntime(void);


extern u8 btlHasRequiredActorStatusBits(BtlUnit *node);


extern const char *btlCommandPanelTaskNameRef;
extern void btlClearClosingActorRowPulses(BattleActorPanelWork *, s8);
extern void btlUpdateActorSlotStates(u8 *, s8);
extern void func_001C3DB0(ActionStateLink *, BattleActorPanelWork *, s8);

void func_001C35F0(ActionStateLink *actor, s8 mode, s8 value) {
    s32 count;
    u8 slot;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    void *task;
    BattleActorPanelWork *work;

    if (battle->battleFlags & 0x8000) {
        if (kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef) != NULL) {
            return;
        }
    }
    count = 0;
    slot = 0;
    for (; node != NULL; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (actor->unit->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count >= 3) {
        return;
    }
    task = kwlnTaskGetTaskByName(D_004367CC);
    if (task == NULL) {
        return;
    }
    work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
    btlClearClosingActorRowPulses(work, mode);
    btlUpdateActorSlotStates((u8 *)work, 0);
    work->activeEntries[slot].presentation.presentationState = 2;
    work->activeEntries[slot].presentation.presentationValue = value;
    if (mode == 0) {
        func_001C3DB0(actor, work, 0);
    } else if (mode == 2) {
        func_001C3DB0(actor, work, 1);
    }
}
void btlUpdateActorSlotPresentationState(BtlUnit *object, s8 mode, s8 value) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    UiSlotRow *slotEntry;
    void *task;
    s32 offset;

    for (; node != 0; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (object->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        task = kwlnTaskGetTaskByName(D_004367CC);
        if (task != 0) {
            entry = (u8 *)kwlnTaskGetUserValue(task);
            if (mode != 2) {
                btlClearClosingActorRowPulses((BattleActorPanelWork *)entry, mode);
            }
            offset = slot * 0x290 + 0x10;
            slotEntry = (UiSlotRow *)(entry + offset);
            slotEntry->state = 2;
            slotEntry->value = value;
        }
    }
}

void btlSetActorSecondaryPresentation(BtlUnit *actor, s32 unused, s8 mode) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    s32 eligibleBefore = 0;
    u8 slot = 0;
    KwlnTask *task;
    BattleActorPanelWork *work;

    for (; node != NULL; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = node->lookupId;
            if (actor->owner == node->owner) {
                break;
            }
            eligibleBefore++;
        }
    }

    if (eligibleBefore >= 3) {
        return;
    }

    task = kwlnTaskGetTaskByName(D_004367CC);
    if (task == NULL) {
        return;
    }

    work = (BattleActorPanelWork *)kwlnTaskGetUserValue(task);
    work->activeEntries[slot].presentation.transitionState = 2;
    work->activeEntries[slot].presentation.secondaryPresentationValue = mode;
    work->activeEntries[slot].presentation.trianglePhase[4] = 90;
    work->activeEntries[slot].presentation.trianglePhase[5] = 180;
    work->activeEntries[slot].presentation.trianglePhase[6] = 0;
    work->activeEntries[slot].presentation.trianglePhase[7] = 90;
    work->activeEntries[slot].presentation.trianglePhase[0] = 90;
    work->activeEntries[slot].presentation.trianglePhase[1] = 0;
    work->activeEntries[slot].presentation.trianglePhase[2] = 180;
    work->activeEntries[slot].presentation.trianglePhase[3] = 90;
}

void btlResetActorSlotPresentationValue(BtlUnit *object, BattleSceneObject *sceneObject) {
    s32 count = 0;
    u8 slot = 0;
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    u8 *entry;
    s32 offset;
    for (; node != 0; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) != 0) {
            slot = object->lookupId;
            if (object->owner == node->owner) {
                break;
            }
            count++;
        }
    }
    if (count < 3) {
        entry = (u8 *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
        offset = slot * 0x290 + 0x10;
        entry += offset;
        *(u8 *)(entry + 0x10) = 2;
        *(u8 *)(entry + 0x11) = 0;
    }
}

void btlClearClosingActorRowPulses(BattleActorPanelWork *work, s8 mode) {
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

void btlAdvancePendingSceneSlotStates(u8 *scene) {
    UiSlotEntry *entry = (UiSlotEntry *)(scene + 0xE0);
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->state == 1) {
            entry->state = 5;
        }
    }
}

typedef struct UiSlotMatchRow {
    u8 pad00[0x18];
    u8 state;
    u8 pad19;
    u8 groupIndex; /* index of the matching primary group */
} UiSlotMatchRow;

void func_001C3DB0(ActionStateLink *unused, BattleActorPanelWork *work, s8 keepActive) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BtlUnit *node = battle->units;
    UiSlotMatchRow *row;
    s32 group;
    s32 offset;
    u8 slot;

    for (; node != NULL; node = node->nextActor) {
        if (btlHasRequiredActorStatusBits(node) == 0) {
            continue;
        }
        group = 0;
        slot = node->lookupId;
        if (battle->groupPrimary[0] != NULL) {
            while (group < 20 && battle->groupPrimary[group] != NULL) {
                if (battle->groupPrimary[group]->unit->owner == node->owner) {
                    offset = slot * 0x290 + 0xE0;
                    row = (UiSlotMatchRow *)((u8 *)work + offset);
                    row->groupIndex = group;
                    if (keepActive == 0 || (u32)(row->state - 1) >= 2) {
                        row->state = 3;
                    }
                    break;
                }
                group++;
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C3EC0);

extern BtlResBlock *btlResourceBlock;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern const BattlePanelColors D_00416870;

void func_001C43F8(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_00416870;
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
        func_00306C28((x + work->activeEntries[slot].presentation.cursorOffset[0]) << 4,
                      (y + work->activeEntries[slot].presentation.cursorOffset[1]) << 3, 0,
                      colors.values, 0, btlResourceBlock->resA, work->activeEntries[slot].presentation.cursorOption,
                      0x53);
    }
}

extern f32 sdfSinPoly(f32);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416830);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416840);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416858);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416870);

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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C4900);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C4C58);

extern const BattlePanelColors D_00416898;

void func_001C50A0(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_00416898;
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
                func_00306C28((x + work->activeEntries[slot].presentation.transitionGeometry[0] + offsetX) << 4,
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
                func_00306C28((x + work->activeEntries[slot].presentation.transitionGeometry[2] - offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.transitionGeometry[3] + offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 0xC, 0x53);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416898);

void func_001C53A0(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
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

void func_001C5610(void *task, BattleActorPanelWork *work, s32 index) {
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
        bottom = y + 77;
        evtSubmitGradientTriangle(x + 54, top, x + 106, top, x + 93, middle,
                      (alpha[0] << 24) | baseColor,
                      (alpha[1] << 24) | baseColor,
                      (alpha[3] << 24) | baseColor);
        evtSubmitGradientTriangle(x + 54, top, x + 93, middle, x + 40, middle,
                      (alpha[0] << 24) | baseColor,
                      (alpha[3] << 24) | baseColor,
                      (alpha[2] << 24) | baseColor);
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


void btlAdvanceActorSlotEchoAnimation(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
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


extern const BattlePanelColors D_004168C8;

void func_001C5D10(BtlUnit *unusedUnit, BattleActorPanelWork *work, s32 slot) {
    BattlePanelColors colors = D_004168C8;
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
                func_00306C28((x + work->activeEntries[slot].presentation.secondaryGeometry[0] + offsetX) << 4,
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
                func_00306C28((x + work->activeEntries[slot].presentation.secondaryGeometry[2] - offsetX) << 4,
                              (y + work->activeEntries[slot].presentation.secondaryGeometry[3] + offsetY) << 3, 0,
                              colors.values, 0, btlResourceBlock->resA, 0xC, 0x53);
            }
        }
    }
}

extern void func_001C6320(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat);

void func_001C6010(BtlUnit *unit, BattleActorPanelWork *work, s16 alpha, s32 slot, s8 stat) {
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
                func_001C6320(unit, &work->activeEntries[slot].presentation.hpBarPulses[n],
                              work->activeEntries[slot].position[0], work->activeEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 0);
                break;
            case 1:
                func_001C6320(unit, &work->activeEntries[slot].presentation.mpBarPulses[n],
                              work->activeEntries[slot].position[0], work->activeEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 1);
                break;
            case 2:
                func_001C6320(unit, &work->reserveEntries[slot].presentation.hpBarPulses[n],
                              work->reserveEntries[slot].position[0], work->reserveEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, 2);
                break;
            default:
                func_001C6320(unit, &work->reserveEntries[slot].presentation.mpBarPulses[n],
                              work->reserveEntries[slot].position[0], work->reserveEntries[slot].position[1],
                              alpha - (n + 1) * 0x20, slot, stat);
                break;
            }
        }
    }
}



extern const BattlePanelColors D_004168D8;

void func_001C6320(BtlUnit *unit, BattleStatPulse *pulse, s32 x, s32 y, s16 alpha, s32 unused, s32 stat) {
    BattlePanelColors colors = D_004168D8;
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
        func_00306C28((x + xOffset + pulse->progress) << 4,
                     (y + yOffset + pulse->yOffset) << 3, 0, colors.values,
                     0, btlResourceBlock->resA, sprite, 0x53);
    }
}


INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C6648);

typedef struct BattleStatEffectOffsets {
    s32 hp[2]; /* x, y */
    s32 mp[2]; /* x, y */
} BattleStatEffectOffsets;

extern void func_001C7760(BtlUnit *, s32, s32, s32, s32, s32, s32, s32, s32, s32);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004168C8);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004168D8);

const BattleStatEffectOffsets D_004168E8 = {{0x36, 0x31}, {0x1A, 0x40}};

void func_001C68D0(BtlUnit *unit, BattleActorPanelWork *work, s32 slot) {
    BattleStatEffectOffsets offsets = D_004168E8;
    s32 alpha;
    s32 type;

    if (work->activeEntries[slot].presentation.hpEffectState == 0x11 ||
        work->activeEntries[slot].presentation.hpEffectState == 0x21) {
        type = work->activeEntries[slot].presentation.hpEffectState == 0x11 ? 1 : 2;
        alpha = work->activeEntries[slot].presentation.fade < 0x80 ?
            work->activeEntries[slot].presentation.fade : work->activeEntries[slot].presentation.hpEffectFade;
        func_001C7760(unit, unit->partyRecord.hp, work->activeEntries[slot].presentation.hpTarget,
                      unit->partyRecord.maxHp, 10,
                      work->activeEntries[slot].position[0] + offsets.hp[0],
                      work->activeEntries[slot].position[1] + offsets.hp[1], alpha, type, 0);
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
        func_001C7760(unit, unit->partyRecord.mp, work->activeEntries[slot].presentation.mpTarget,
                      unit->partyRecord.maxMp, 11,
                      work->activeEntries[slot].position[0] + offsets.mp[0],
                      work->activeEntries[slot].position[1] + offsets.mp[1], alpha, type, 1);
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


void func_001C6B98(BtlUnit *unit, BattleActorPanelWork *work, s32 slot, s8 kind) {
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
                        work->activeEntries[slot].presentation.unk1C0 = 0;
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
                work->activeEntries[slot].presentation.unk1C0 = 0;
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
                work->activeEntries[slot].presentation.unk1C4 = 0;
                work->activeEntries[slot].presentation.mpState++;
                break;
            }
        }
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C7020);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C7760);


extern SdfPoolNode D_003805A8;
extern s32 sdfAllocPacketAligned(s32 size);
/* Draw a textured command-panel quad with independently colored corners. */
s32 btlDrawGouraudTexturedPanelQuad(s32 x0, s32 y0, s32 x1, s32 y1,
                  s32 x2, s32 y2, s32 x3, s32 y3,
                  s32 u, s32 v, s32 width, s32 height,
                  const s32 *colors, s32 texture) {
    SdfListHead *list;
    s32 uFixed;
    s32 vFixed;
    s32 uRight;
    s32 vBottom;

    list = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket(list, (SdfTex *)texture, 0);
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
    D_003805A8.append(&D_003805A8, list);
    return 1;
}

extern const char *btlCommandPanelTaskNameRef;

s32 btlGetNamedTaskPairStatusOrUnavailable(void) {
    void *first;
    void *second;

    if (btlTrackedTaskHandles != 0) {
        first = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
        second = kwlnTaskGetTaskByName(D_004367CC);
        if (first == 0 && second == 0) {
            return -128;
        }
        if (btlTrackedTaskHandles->status.flags & 0x100) {
            return 0;
        }
        return btlTrackedTaskHandles->status.bytes.state;
    }
    return -128;
}

extern const char *D_004367DC;
extern BattleCmdPanel *btlCommandPanelWork;
extern void func_001BBA60(BattleRegisteredPanelWork *);

void func_001C7DB8(s8 mode, s32 duration) {
    struct KwlnTask *commandTask = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    struct KwlnTask *panelTask = kwlnTaskGetTaskByName(D_004367CC);
    struct KwlnTask *registeredTask = kwlnTaskGetTaskByName(D_004367DC);

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
            func_001BBA60((BattleRegisteredPanelWork *)kwlnTaskGetUserValue(registeredTask));
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

extern s32 btlAreLinkedSceneCountersAtThreshold(void);

s32 btlUpdateCommandUiTransition(void) {
    BattleTrackedTaskWork *flow;
    void *task;
    s32 counter;

    if (btlTrackedTaskHandles != NULL) {
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
                if (task != NULL) {
                    func_001C35F0(((BattleSceneObject *)kwlnTaskGetUserValue(task))->owner, 2, 0);
                }
                btlTrackedTaskHandles->status.bytes.state = 0;
            }
            break;
        }
    }
    return 0;
}

extern BtlResBlock *btlResourceBlock;
extern void func_00306C28(s32, s32, s32, u32 *, s32, EffectSlotSet *, s32, s32);
extern void func_001CE418(EffectSlotSet *, s32, s32 *, s32 *, f32);

extern SceneSlotFadeWork *D_00438F54;
extern ActorSlotOrder *D_00438F58[2];

extern s32 btlGetRuntime(void);


extern u32 func_0019F5E8(s32, s32, s32, u32, char *, s32);
extern s32 frFontDrawGlyphWithSharedFlags(struct FrFontGlyph *, s8);




extern s32 btlGetTrackedTaskHandle(s32);




extern u32 btlHasRegisteredAphNamePanelTask(void);

extern s32 btlCreateAiWork(s32);

extern u32 fldGetSceneScriptTaskUserData(void);







extern BattleSceneObject *fldGetSceneObjectTaskUserData(void);




typedef struct SceneScriptState {
    u32 state;
    u8 pad04[0xC];
    u32 value10;
} SceneScriptState;



typedef struct BattleSceneWork {
    u8 pad00[0x218];
    u32 flags;
    u32 subFlags;
    u32 refreshFlags;
    u8 pad224[8];
    s32 currentScene;
    s32 queuedScene;
    s32 frame;
    s32 sceneState;
    u8 pad23C[4];
    s32 scriptState;          /* 0x240 */
    s32 scriptArg;            /* 0x244 */
    ActionStateLink *linkedNodes;
    BtlUnit *actors;
    u8 pad250[0x1E];
    u8 phaseFlag;
    u8 pad26F;
    u16 variant;
    u8 pad272[2];
    s32 step;
    u8 pad278[4];
    u8 unk27C;
    u8 pad27D[0x17];
    s32 effectLayer;
    u8 pad298[8];
    s32 mode;
    u8 pad2A4[8];
    s16 tileX;
    s16 tileY;
    s32 loadStep;             /* 0x2B0 */
    u8 pad2B4[0x10];
    KwlnTask *taskParent;
    u8 pad2C8[4];
    s32 pendingTask;          /* 0x2CC */
    KwlnTask *sceneObject;
    KwlnTask *spriteObject;
    KwlnTask *sceneStatus;
    u8 pad2DC[0x1C];
    s32 unk2F8;
    u8 pad2FC[2];
    BtlSceneSlot slots[8];
    u8 pad316[2];
    ActionStateLink *groupPrimary[20];    /* 0x318 */
    ActionStateLink *groupSecondary[45];  /* 0x368 */
    ActionStateLink *groupTertiary[15];   /* 0x41C */
    ActionStateLink *groupHandles[8];     /* 0x458 */
    u16 groupHandleCount;
    u8 pad47A[2];
    s32 activeGroupCount;
    BtlSceneFadingRecord fading[8];
    ActionStateLink *currentTask;
    u8 pad4C4[0x10];
    s32 scriptTarget;         /* 0x4D4 */
    u8 pad4D8[0xC];
    u32 values[64];
    s32 (*sceneCallback)();
} BattleSceneWork;

extern BattleCmdPanel *btlCommandPanelWork;

extern BattleSelectionWork *btlLinkedSelectionTaskBuffer;

extern u32 func_001C82D8(s32, s8);

extern s32 btlCountFlaggedSceneActors(void);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;


extern u8 D_003B5B10[];

extern s32 btlGetEntryFlagsUnlessDisabled(DatPartyRecord *);

extern u8 *D_00435E64;

extern u8 *D_00435E5C;







void fldInitializeBattleSceneFlow(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    btlNextScaledRandom(7);
    if ((scene->flags & 0x400) != 0) {
        if (scene->variant == 1) {
            btlInvalidateSceneFadeCounts(0);
            btlResetBattleHistoryCounters();
        } else {
            btlInvalidateSceneFadeCounts(1);
            btlResetBattleHistoryCounters();
        }
    }
    btlToggleModelFlagOnInput();
    btlUpdateCommandUiTransition();
}

void btlDebugPrintf(s32 tag, ...) {
}

void func_001C80C0(void) {
}

void func_001C80C8(void) {
}

/* Submit a scene object at fixed-point screen coordinates and retire its handle. */
void fldSubmitSceneObjectAtCoordinates(s32 x, s32 y, u32 color, char *text) {
    u32 glyph;

    itfSetTextDrawLimit(0x13);
    glyph = func_0019F5E8(x << 4, y << 3, 0, color, text, 0);
    frFontDrawGlyphChain(glyph, 1, 0x53);
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)(u32)glyph);
    itfSetTextDrawLimit(-1);
}

void btlDrawIndexedBattleEntryGlyphs(s32 x, s32 y, s32 z, s32 w, u16 index) {
    struct FrFontGlyph *handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_00435E64 + index * 17, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer(handle);
    itfSetTextDrawLimit(-1);
}

void btlQueueIndexedTextWithinDrawLimit(s32 x, s32 y, s32 z, s32 w, u16 index) {
    struct FrFontGlyph *handle;
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(x << 4, y << 3, z, w, D_00435E5C + index * 25, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer(handle);
    itfSetTextDrawLimit(-1);
}

s32 btlIsSceneActorCountWithinLimit(BtlUnit *unused, u32 limit) {
    btlGetRuntime();
    if (limit < btlCountFlaggedSceneActors()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C82D8);


extern SceneKindTable *D_00438F4C;

extern u16 *func_001C8518(s32, s16 *, u16, u16, u16);

extern void fldCollectAvailableRosterEntries(s32, s16 *);


/* Advance the per-kind scene counter table: refresh the slot for the current
 * scene kind, then return its value (clamped to 4 unless noClamp is set). */
INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416920);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416938);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416948);

u32 fldUpdateSceneKindCounter(s32 ctx, s8 kind, s8 noClamp) {
    /* This scratch is written as signed IDs or an unsigned roster count.
     * The scene counter interprets its first halfword as signed (retail 1C8470). */
    union {
        s16 ids[8];
        u16 count;
    } buffer;
    u32 type = func_001C82D8(ctx, kind);
    if (type != 4) {
        D_004367C0 = 0;
    }
    switch (type) {
    case 0:
        func_001C8518(ctx, buffer.ids, 0, 2, 3);
        D_00438F4C->value[0] = buffer.ids[0] + 1;
        break;
    case 4:
        fldGetCachedSceneActorNameAndId(ctx, &buffer.count);
        D_00438F4C->value[4] = buffer.ids[0];
        break;
    case 2:
        fldCollectAvailableRosterEntries(ctx, buffer.ids);
        D_00438F4C->value[2] = buffer.ids[0];
        break;
    case 3:
        D_00438F4C->value[3] = 3;
        break;
    case 6:
        D_00438F4C->value[6] = 1;
        break;
    default:
        D_00438F4C->value[type] = 0;
        break;
    }
    if (noClamp == 0) {
        if (D_00438F4C->value[type] >= 4) {
            return 4;
        }
    }
    return D_00438F4C->value[type];
}

extern u16 D_003B6810[12];
extern u16 D_003B55D0[0x260];

u16 *func_001C8518(s32 address, s16 *outCount, u16 firstKind, u16 secondKind, u16 thirdKind) {
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

    if (!(btlUnitStatusPair(object->owner->unit) & 0x1400) && !(object->owner->unit->partyRecord.flags & 0x10)) {
        wanted = firstKind ? firstKind : 5;
    } else {
        wanted = firstKind;
    }
    btlGetRuntime();
    unit = object->owner->unit;
    out = D_003B6810;
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
        if ((btlUnitStatusPair(object->owner->unit) & 0x1400) || (object->owner->unit->partyRecord.flags & 0x10)) {
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
    out = D_003B55D0;
    memcpy(out, D_003B6810, count * 2);
    i = 0;
    while (i < count) {
        if (out[i] >= 0x220) {
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C8768);

/* Pack available roster IDs and their values into consecutive byte pairs. */
void fldCollectAvailableRosterEntries(s32 unused, s16 *count) {
    u8 *roster;
    DatItemSkillRecord *availability;
    u8 *output;
    s32 found = 0;
    s32 i = 0;
    btlGetRuntime();
    roster = datGameState->inventory.counts;
    availability = datItemSkillRecords;
    output = D_003B5B10;
    do {
        if (*roster != 0 && (availability->flags & 2) != 0) {
            output[0] = i;
            found++;
            output[1] = *roster;
            output += 2;
        }
        i++;
        availability++;
        roster++;
    } while (i < 0x100);
    *count = found;
}

/* Cache the roster count while returning the shared paired-actor record. */
BattleRosterTable *fldGetCachedSceneActorNameAndId(s32 address, u16 *outCount) {
    BattleSceneObject *object = (BattleSceneObject *)address;
    s32 cachedCount = D_004367C0;
    if (cachedCount == 0) {
        cachedCount = func_001AC750((s32)object->owner->unit, &D_003B5D10);
        D_004367C0 = cachedCount;
    }
    *outCount = cachedCount;
    return &D_003B5D10;
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C8A80);

extern void func_001C92A0(s32, s32, s32, s32);

extern void func_001C9EA0(s32);

extern void func_001C9BE8(BattleSceneObject *);

extern void func_001C98E8(s32);

extern void btlDrawRetreatCommandLabel(s32);

void fldDispatchSceneKindHandler(s32 sceneContext) {
    switch (func_001C82D8(sceneContext, btlCommandPanelWork->classIndex)) {
    case 0:
        func_001C92A0(sceneContext, 0, 2, 3);
        return;
    case 4:
        func_001C9EA0(sceneContext);
        return;
    case 2:
        func_001C9BE8((BattleSceneObject *)sceneContext);
        return;
    case 3:
        func_001C98E8(sceneContext);
        return;
    case 6:
        btlDrawRetreatCommandLabel(sceneContext);
    }
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C98E8);

typedef struct ItemRowColors {
    u32 values[4];
} ItemRowColors;

extern const ItemRowColors D_00416A00;
extern char D_004368A0[];

void func_001C9BE8(BattleSceneObject *object) {
    char text[16];
    ItemRowColors colors = D_00416A00;
    s32 shown;
    s32 total;
    s32 index;
    s32 selected;
    s32 row = 0;
    s32 y = 0x14C;
    s32 color;
    s32 countY;

    shown = fldUpdateSceneKindCounter((s32)object, btlCommandPanelWork->classIndex, 0);
    total = fldUpdateSceneKindCounter((s32)object, btlCommandPanelWork->classIndex, 1);
    index = object->selections[2].cursor;
    selected = object->selections[2].entry;
    for (;;) {
        if (row >= shown || index >= total) {
            break;
        }
        color = selected == index ? 0x89FEFF80 : 0xA09DC380;
        color = (color & ~0xFF) | btlLinkedSelectionTaskBuffer->rowFade[row];
        btlQueueIndexedTextWithinDrawLimit(0x1A, y, 0xFF0010, color, D_003B5B10[index * 2]);
        countY = y + 4;
        colors.values[0] = color;
        colors.values[1] = color;
        colors.values[2] = color;
        colors.values[3] = color;
        func_00306C28(0x900, countY << 3, 0, colors.values, 0,
                     btlResourceBlock->resA, 5, 0x53);
        itfSetTextDrawLimit(0x13);
        func_0035C860(text, D_004368A0, D_003B5B10[index * 2 + 1]);
        fldSubmitSceneObjectAtCoordinates(0xA0, countY, color, text);
        itfSetTextDrawLimit(-1);
        row++;
        index++;
        y += 0x17;
    }
}

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436630);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436638);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", sndTestMessageResourceIndex);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", sndTestMessageTexture);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436648);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436650);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436658);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436660);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436668);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436670);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436678);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436680);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436688);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436690);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436698);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366A0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366A8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366B0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366B4);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366B8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366C0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366C8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366D0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366D8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366E0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlRuntime);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366E8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366F0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004366F8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436700);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436708);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436710);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436718);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436720);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436728);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436730);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436738);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436740);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436748);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436750);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436758);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436760);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436768);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436770);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436778);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436780);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436788);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436790);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436798);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367A0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367A8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367B0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367B8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlCommandPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367C0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367C4);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367C8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367CC);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367D0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367D4);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367D8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367DC);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367E0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlMahenPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlAnalyzPanelTaskNameRef);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367EC);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367F0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlTrackedTaskHandles);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004367F8);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlCommandPanelWork);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436800);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlResourceBlockLoaded);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", btlResourceBlock);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436808);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436810);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436818);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436820);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436828);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436830);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436838);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436840);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436848);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436850);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436858);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436860);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436868);

void btlDrawRetreatCommandLabel(s32 unused) {
    u8 text[8] = "Retreat";
    s32 color;
    struct FrFontGlyph *handle;
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();

    if (datBattleSceneRecords[scene->mode].unk00 != 0) {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x504F6100;
    } else {
        color = btlLinkedSelectionTaskBuffer->rowFade[0] | 0x89FEFF00;
    }
    itfSetTextDrawLimit(0x13);
    handle = itfCreateConvertedTextGlyph(0x1A0, 0xA60, 0xFF0010, color, text, 0);
    frFontDrawGlyphWithSharedFlags(handle, 1);
    frFontQueueGlyphForCurrentDrawBuffer(handle);
    itfSetTextDrawLimit(-1);
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C9EA0);

typedef struct SceneCheckArgs {
    u16 mode;
    u16 pad2;
    BtlUnit *first;
    BtlUnit *second;
} SceneCheckArgs;

extern s32 func_001ABDE8(BtlUnit *, BtlUnit *, BtlUnit *, BtlUnit *, s32);

extern s32 btlCheckCommandVitalCost(BtlUnit *, s32);

/* Check helper for a pair of scene objects: the caller's own result wins,
 * then the first object's, then the second's. */
s32 btlCheckScenePairResult(BtlUnit *self, SceneCheckArgs *args) {
    s32 base;
    s32 resultA = 0;
    s32 resultB = 0;
    base = func_001ABDE8(self, self, args->first, args->second, args->mode);
    if (args->first != 0) {
        resultA = func_001ABDE8(args->first, args->first, self, args->second, args->mode);
        if (resultA == 6 && base == 0) {
            if (btlCheckCommandVitalCost(args->first, args->mode) == 0) {
                resultA = 0;
            }
        }
    }
    if (args->second != 0) {
        resultB = func_001ABDE8(args->second, args->second, self, args->first, args->mode);
        if (resultB == 6 && base == 0) {
            if (btlCheckCommandVitalCost(args->second, args->mode) == 0) {
                resultB = 0;
            }
        }
    }
    if (base != 0) {
        return base;
    }
    if (resultA != 0) {
        return resultA;
    }
    return resultB;
}



extern void func_001C0EF0(void);
extern void func_001C1300(void);
extern void func_001B5A20(BattleSceneObject *);
/* Task-update result: retail returns 0 or -1, even when this caller discards it. */
extern s32 func_001BFB58(KwlnTask *);
extern s32 func_001C8A80(BattleSceneObject *);
extern void func_001BD9A0(BattleSceneObject *, s32);
extern s32 btlHasHighPriorityState(void);
extern s32 btlAreLinkedSceneCountersAtThreshold(void);

/* The same native command-selection sequence occurs in four switch paths. */
static inline void btlUpdateSceneCommandSelection(BattleSceneObject *object) {
    fldDispatchSceneKindHandler((s32)object);
    func_001BD9A0(object, func_001C82D8((s32)object, btlCommandPanelWork->classIndex));
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416A10);

s32 btlUpdateBattleSceneCommands(KwlnTask *task) {
    BtlState *battle = (BtlState *)btlGetRuntime();
    BattleSceneObject *object = (BattleSceneObject *)kwlnTaskGetUserValue(task);
    BattleActorPanelWork *panel;
    u8 slot;

    if (!(battle->battleFlags & 0x200)) {
        return 0;
    }
    func_001C0EF0();
    func_001C1300();
    func_001B5A20(object);
    func_001BFB58(task);
    switch (object->state) {
    case 5:
        return -1;
    case 1:
        func_001C8A80(object);
        btlUpdateSceneCommandSelection(object);
        if (btlHasHighPriorityState() && object->state == 1) {
            object->state = 2;
        }
        break;
    case 2:
        func_001C8A80(object);
    case 3:
    case 6:
    case 8:
    case 10:
    case 11:
        btlUpdateSceneCommandSelection(object);
        break;
    case 7:
        if (func_001C8A80(object)) {
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
            panel = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
            object->commandData->unk18 = panel->partyRecordIndex;
            object->state = 3;
            ((SceneScriptState *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367B8)))->state = 3;
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

extern void btlReleaseBattleScratchBlocks(void);

void fldClearBattleSceneObject(KwlnTask *task) {
    sdfReleaseChipBlock((void *)kwlnTaskGetUserValue(task));
    ((BattleSceneWork *)btlGetRuntime())->sceneObject = 0;
    btlReleaseBattleScratchBlocks();
}

void fldInitializeSceneObject(BattleSceneObject *object, ActionStateLink *owner) {
    memset(object, 0, sizeof(*object));
    object->state = 1;
    object->owner = owner;
    object->commandData = &owner->indexWork;
}

BattleSceneObject *fldGetSceneObjectTaskUserData(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task == NULL) {
        return NULL;
    }
    return (BattleSceneObject *)kwlnTaskGetUserValue(task);
}

s32 fldGetSceneObjectState(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef);
    if (task == NULL) {
        return 0;
    }
    return fldGetSceneObjectTaskUserData()->state;
}

s32 btlHasSpecialActiveSceneActor(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401) {
            u16 kind = actor->partyRecord.unitId;
            if (kind == 0x4C || kind == 0x3C) {
                return 1;
            }
        }
        actor = actor->nextActor;
    }
    return 0;
}

s32 func_001CA8D8(void) {
    u32 requiredFlags = 0x201;
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    s32 i;

    if (actor != 0) {
        do {
            if ((actor->status.flags & requiredFlags) == requiredFlags) {
                for (i = 0; i < 8; i++) {
                    if ((u16)(actor->partyRecord.effectData[i] - 0xE0) < 0x20) {
                        return actor->partyRecord.effectData[i];
                    }
                }
            }
            actor = actor->nextActor;
        } while (actor != 0);
    }
    return 0;
}

s32 btlHasSelectedActiveSceneActor(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401 &&
            (actor->partyRecord.status & 1) != 0) {
            return 1;
        }
        actor = actor->nextActor;
    }
    return 0;
}

s32 btlHaveActiveSceneActorEntriesCleared(void) {
    BtlUnit *actor = ((BattleSceneWork *)btlGetRuntime())->actors;
    while (actor != 0) {
        if ((btlUnitStatusPair(actor) & 0x421) == 0x401 &&
            btlGetEntryFlagsUnlessDisabled(&actor->partyRecord) != 0) {
            return 0;
        }
        actor = actor->nextActor;
    }
    return 1;
}

typedef struct SceneGlobalState {
    u8 pad00[0x38];
    u32 stage; /* 0x38: state gate */
    u32 flags; /* 0x3C: scene restrictions */
    u8 pad40[8];
    s8 fadeLatched;  /* 0x48: fade kind counts recorded */
    u8 pad49[3];
    s32 fadeKindA;
    s32 fadeKindB;
} SceneGlobalState;

extern s32 mdlFlagTest();

extern s32 btlGetTaskState6();

s32 fldSelectSceneMode(ActionStateLink *task) {
    s32 flag = ((BattleSceneWork *)btlGetRuntime())->phaseFlag == 3;
    if (mdlFlagTest(0x801) != 0) {
        return 1;
    }
    if (mdlFlagTest(0x81D) == 0 && mdlFlagTest(0x801) == 0 && !(((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x200)) {
        if (btlHasSpecialActiveSceneActor() != 0) {
            return 2;
        }
    }
    if (mdlFlagTest(0x81A) == 0) {
        if (flag != 0) {
            return 6;
        }
    }
    if (flag == 0) {
        if (btlGetTaskState6() != 0) {
            return 5;
        }
    }
    if (mdlFlagTest(0x805) == 0 && !(((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x200)) {
        if (func_001CA8D8() != 0 && btlHaveActiveSceneActorEntriesCleared() != 0) {
            return 3;
        }
    }
    if (mdlFlagTest(0x805) != 0) {
        if (mdlFlagTest(0x806) == 0) {
            if (btlHasSelectedActiveSceneActor() != 0) {
                return 4;
            }
        }
    }
    return 0;
}

extern void btlInitializeSelectionWork(void);
extern void btlInitializeCommandPanelSlotTables(void);
extern void btlCreateMessageWindow(void);
extern void func_001BD6E8(void *);
extern void btlBossDebugPrintf(const char *format, ...);
extern void mdlFlagClear(s32);
extern void mdlFlagSet(s32);
extern void btlSetTrackedTaskHandle(s32, s32);
extern s32 dspCloseChannel(void);
extern void evtCreateMessageWindowIfMissing(void *);
extern s32 dspStartEntry(s32);
extern void evtCopyEntryStringToActiveWindow(s32, const void *);
extern s32 func_0035C860(char *, const char *, ...);
extern void *memset(void *, s32, u32);
extern s32 func_001C0630(KwlnTask *);
extern s32 func_001C0828(KwlnTask *);
extern s32 func_001C0240(KwlnTask *);
extern void btlReleaseDialogTaskAndMarkBattleState(KwlnTask *);
extern void btlFinishTrackedBattleTaskAndCloseWindow(KwlnTask *);
extern u8 D_003B52D0[];
extern u8 D_00385228[];
extern char D_00436828[];

/* Open the battle command panel task and, on the first eligible ally turn, start the matching tutorial dialog. */
void func_001CAB60(ActionStateLink *task) {
    char text[32];
    BattleSceneWork *scene;
    BattleSceneObject *object;
    KwlnTask *handle;

    if (kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef) == 0) {
        D_004367C0 = 0;
        scene = (BattleSceneWork *)btlGetRuntime();
        object = sdfAllocAndClearQuadwords(0x30);
        fldInitializeSceneObject(object, task);
        handle = kwlnTaskCreate(btlCommandPanelTaskNameRef, 0x2B0E, 1, 1, btlUpdateBattleSceneCommands, fldClearBattleSceneObject,
                                (u32)object);
        func_00101968(scene->taskParent, handle);
        scene->sceneObject = handle;
        func_001C35F0(task, 0, 0);
        btlInitializeSelectionWork();
        btlInitializeCommandPanelSlotTables();
        btlCreateMessageWindow();
        func_001BD6E8(object);
        if ((task->unit->status.flags & 0x200) && !(scene->flags & 0x1000000)) {
            switch (fldSelectSceneMode(task)) {
            case 1:
                btlBossDebugPrintf("-----------------First Battle!!-------------------\n");
                mdlFlagClear(0x801);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_003B52D0);
                dspStartEntry(2);
                func_001C7DB8(0, 8);
                break;
            case 6:
                btlBossDebugPrintf("-----------------Tutorial Majin!!-------------------\n");
                mdlFlagSet(0x81A);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x300;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0828,
                                        btlReleaseDialogTaskAndMarkBattleState, (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(4);
                func_001C7DB8(0, 8);
                break;
            case 2:
                btlBossDebugPrintf("-----------------Tutorial Weak!!-------------------\n");
                mdlFlagSet(0x81D);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x300;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(1);
                func_001C7DB8(0, 8);
                break;
            case 3:
                btlBossDebugPrintf("-----------------Tutorial Hunt!!-------------------\n");
                mdlFlagSet(0x805);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                memset(text, 0, sizeof(text));
                func_0035C860(text, D_00436828, D_00435E64 + func_001CA8D8() * 17);
                evtCopyEntryStringToActiveWindow(0, text);
                dspStartEntry(2);
                func_001C7DB8(0, 8);
                break;
            case 4:
                btlBossDebugPrintf("-----------------Tutorial Fear!!-------------------\n");
                mdlFlagSet(0x806);
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x100;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367F0, 0x2B0E, 1, 1, func_001C0630,
                                        btlReleaseDialogTaskAndMarkBattleState, (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xD, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_00385228);
                dspStartEntry(3);
                func_001C7DB8(0, 8);
                break;
            case 5:
                btlBossDebugPrintf("-----------------New Linkage!!-------------------\n");
                ((SceneGlobalState *)btlTrackedTaskHandles)->flags |= 0x200;
                scene->flags &= ~0x100000;
                handle = kwlnTaskCreate(D_004367EC, 0x2B0E, 1, 1, func_001C0240,
                                        btlFinishTrackedBattleTaskAndCloseWindow,
                                        (u32)sdfAllocAndClearQuadwords(0x18));
                func_00101968(scene->taskParent, handle);
                btlSetTrackedTaskHandle(0xC, (s32)handle);
                dspCloseChannel();
                evtCreateMessageWindowIfMissing(D_003B52D0);
                func_001C7DB8(0, 8);
                break;
            default:
                scene->flags |= 0x100000;
                break;
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
    } else if (object->state != 0xB) {
        btlCommandPanelWork->state = 1;
        object->state = 1;
    }
    btlLinkedSelectionTaskBuffer->selectedRow = fldUpdateSceneKindCounter((s32)object, btlCommandPanelWork->classIndex, 0);
}

void fldSetSceneObjectAndGroupStates(void) {
    BattleSceneObject *object = fldGetSceneObjectTaskUserData();
    if (object != 0) {
        object->state = 5;
        btlCommandPanelWork->state = 3;
    }
}

extern s32 D_00416BB8[];

void fldGetSceneDirectionStepOffset(s32 *outX, s32 *outY, s32 dir, s32 step) {
    s32 offsets[3][8][2];

    memcpy(offsets, D_00416BB8, sizeof(offsets));
    *outX = offsets[dir][step][0];
    *outY = offsets[dir][step][1];
}

void func_001CB278(SceneAiWork *work, s32 mode) {
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

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416BB8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CB498);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416C98);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416CC8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CB7A8);

u32 func_001CC018(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416D00);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CC020);

extern const u8 *D_00435E48;
extern const u8 *D_00435E4C;
extern const u8 *D_00435E60;
extern void func_001CB498(SceneAiWork *work, s32 mode);
extern void func_001CB7A8(BtlUnit *unit, SceneAiWork *work, u32 index);
extern s32 btlIsUnitInfoFlagOneEligible(BtlUnit *unit);
extern void btlDrawCenteredPanelSegments(s32 width);
extern void btlUpdateActorSlotStates(u8 *, s8);
extern void btlSetActorSecondaryPresentation(BtlUnit *unit, s32 unused, s8 phase);
extern DatEnemyRecord *datEnemyRecords;

/* Draw selected actor names and advance the battle scene's actor panels. */
void func_001CC438(SceneAiWork *work) {
    BtlUnit *unit;
    u32 glyph;
    u32 categoryGlyph;
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
                    D_00435E48 + unit->partyRecord.unitId * 17, 0);
                width = frFontMeasureLines((struct FrFontGlyph *)glyph);
                btlDrawCenteredPanelSegments(width);
                frFontSetGlyphPosition((struct FrFontGlyph *)glyph, (0xFA - (width >> 1)) << 4, 0x220);
                frFontDrawGlyphChain(glyph, 1, 0x53);
                frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)glyph);
            } else {
                glyph = itfCreateConvertedTextGlyph(0x640, 0x200, 0, 0xA09DC380,
                    D_00435E4C + unit->partyRecord.unitId * 17, 0);
                width = frFontMeasureLines((struct FrFontGlyph *)glyph);
                categoryGlyph = itfCreateConvertedTextGlyph(0x640, 0x200, 0, 0xA09DC380,
                    D_00435E60 + datEnemyRecords[unit->partyRecord.unitId].pad04 * 7, 0);
                categoryWidth = frFontMeasureLines((struct FrFontGlyph *)categoryGlyph);
                totalWidth = width + categoryWidth;
                btlDrawCenteredPanelSegments(totalWidth);
                frFontSetGlyphPosition((struct FrFontGlyph *)categoryGlyph, (0xFA - (totalWidth >> 1)) << 4, 0x220);
                frFontDrawGlyphWithSharedFlags((struct FrFontGlyph *)categoryGlyph, 1);
                frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)categoryGlyph);
                if (categoryWidth == 0) {
                    s32 centeredNameX = -(width >> 1);
                    frFontSetGlyphPosition((struct FrFontGlyph *)glyph, (centeredNameX + 0xFA) << 4, 0x220);
                } else {
                    frFontSetGlyphPosition((struct FrFontGlyph *)glyph, (categoryWidth - (width >> 1) + 0xF2) << 4, 0x220);
                }
                frFontDrawGlyphChain(glyph, 1, 0x53);
                frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)glyph);
            }
        }
    }
    scene = fldGetSceneObjectState();
    if (work->state != 3 && scene != 8) {
        unit = btlGetIndexListEntry(work->listB, 0);
        if (unit->status.flags & 0x200) {
            panelTask = kwlnTaskGetTaskByName(D_004367CC);
            if (panelTask != NULL) {
                btlUpdateActorSlotStates((u8 *)kwlnTaskGetUserValue(panelTask), 0);
            }
        }
    }
    func_001CB278(work, 0);
    func_001CB498(work, 0);
    for (i = 0; i < count; i++) {
        unit = btlGetIndexListEntry(work->listB, i);
        if (btlIsUnitInfoFlagOneEligible(unit) == 0) {
            func_001CB7A8(unit, work, i);
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
    func_00306C28(x * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x17, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] = width << 4;
    func_00306C28((0x100 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x16, 0x53);
    btlResourceBlock->resC->workEntries[0x16].geometry.bounds[2] =
        btlResourceBlock->resC->workEntries[0x16].sourceWidth << 4;
    func_00306C28((0x92 - half) * 0x10, 0x200, 0, color, 0, btlResourceBlock->resC, 0x15, 0x53);
}

extern s32 btlGetEffectActive();

extern void func_001CC020();

extern void func_001CC438();

s32 fldStepSceneStateMachine(KwlnTask *handle) {
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    s32 *state;
    s32 mode;
    if (work->flags & 0x04000000) {
        return 0;
    }
    state = (s32 *)kwlnTaskGetUserValue(handle);
    switch (*state) {
    case 1:
        *state = 2;
        break;
    case 2:
        mode = 1;
        if (work->mode == 0x312) {
            mode = btlGetEffectActive() == 1 ? 5 : 1;
        }
        func_001CC020(work, state, mode);
        func_001CC438(state);
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









extern void func_001AC0F8(s32 source, BtlIndexList *list, s32, s32, s32);

extern s32 func_001AC360(s32 source, BtlIndexList *list, s32);

extern s32 btlFindEligibleTargetForMultiActorCommand(s32 source, BtlIndexList *list);

/* Creates the AI work object for `source`: allocates two index lists and
 * fills them according to the current scene object state. */
s32 btlCreateAiWork(s32 source) {
    SceneAiWork *work;
    BattleSceneObject *object;
    BattleActorPanelWork *other;
    s32 id;
    s32 count;
    btlGetRuntime();
    work = (SceneAiWork *)sdfAllocAndClearQuadwords(0xA4);
    object = (BattleSceneObject *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(btlCommandPanelTaskNameRef));
    work->listA = btlAllocateIndexList(0xD);
    work->listB = btlAllocateIndexList(0xD);
    if (object->state == 8) {
        other = (BattleActorPanelWork *)kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC));
        count = btlCountFlaggedSceneActors();
        if (count < 2 && (datGameState->party[other->partyRecordIndex].status & 0x4800)) {
            func_001AC0F8(source, work->listA, 1, 4, -0x4801);
        } else {
            func_001AC0F8(source, work->listA, 1, 4, -1);
        }
        btlGetIndexListCount(work->listA);
        work->result = 0;
    } else if (source != 0) {
        work->result = func_001AC360(source, work->listA, 0);
    }
    work->entry = 0;
    switch (work->result) {
    case 0:
        id = btlFindEligibleTargetForMultiActorCommand(source, work->listA);
        work->entry = id;
        btlAppendIndexListEntry(work->listB, btlGetIndexListEntry(work->listA, id));
        break;
    case 1:
    case 2:
        btlCopyIndexList(work->listB, work->listA);
        break;
    }
    work->source = (ActionStateLink *)source;
    work->state = 1;
    return (s32)work;
}

void fldReleaseSceneSpriteWork(SceneAiWork *work) {
    btlFreeIndexList(work->listB);
    btlFreeIndexList(work->listA);
    sdfReleaseChipBlock(work);
}

void fldReleaseSceneSprite(KwlnTask *arg) {
    fldReleaseSceneSpriteWork((SceneAiWork *)kwlnTaskGetUserValue(arg));
    ((BattleSceneWork *)btlGetRuntime())->spriteObject = 0;
}

u32 fldGetSceneScriptTaskUserData(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(D_004367B8);
    if (task == NULL) {
        return 0;
    }
    return kwlnTaskGetUserValue(task);
}

u32 fldGetSceneScriptState(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    return state->state;
}

u32 fldGetSceneScriptValue(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    return state->value10;
}

void fldCreateSceneSpriteTask(s32 sourceTask) {
    BattleSceneWork *scene;
    KwlnTask *task;
    kwlnTaskGetTaskByName(D_004367B8);
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
    scene = (BattleSceneWork *)btlGetRuntime();
    task = kwlnTaskCreate(D_004367B8, 0x2B0E, 1, 1, fldStepSceneStateMachine,
                          fldReleaseSceneSprite, btlCreateAiWork(sourceTask));
    func_00101968(scene->taskParent, task);
    scene->spriteObject = task;
}

void fldMarkActiveSceneScriptState(void) {
    SceneScriptState *state;

    state = (SceneScriptState *)fldGetSceneScriptTaskUserData();
    if (state != 0) {
        state->state = 6;
    }
}

void func_001CCD80(s32 index, s8 operation) {
    s32 bank = D_00438F54->bank;
    s32 count;
    s32 i;
    if (D_00438F54->enabled[bank] == 0) return;
    switch (operation) {
    case 0:
        if (D_00438F58[bank]->state[index] >= 3) return;
        D_00438F58[bank]->state[index] = 3;
        count = D_00438F54->currentIndex;
        for (i = 0; i < count; i++) {
            if ((u8)(D_00438F58[bank]->state[i] - 4) < 3) {
                D_00438F58[bank]->state[i] = 6;
                D_00438F58[bank]->scalePercent[i][0] = 132.0f;
                D_00438F58[bank]->slotValues[i][0] = 16;
                D_00438F58[bank]->scalePercent[i][1] = 105.0f;
                D_00438F58[bank]->slotValues[i][1] = 24;
            }
        }
        break;
    case 1:
        D_00438F58[bank]->state[index] = 7;
        D_00438F54->phase[index + 1][bank] = 4;
        break;
    }
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416D58);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416D88);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CCEB8);

typedef s32 SceneSlotCoordinate[3];

extern const u32 D_00416DE0[4];
extern const SceneSlotCoordinate D_00416DF0[8];
extern const SceneSlotCoordinate D_00416E50[8];
extern void func_001CDD38(EffectSlotSet *resource, s32 slot, s32 x, s32 y,
                          s32 bank, s32 index, s32 mode);
extern void fldScaleSceneCoordinateRecord(EffectSlotSet *resource, s32 slot);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416DE0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416DF0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416E50);

void func_001CD6E0(void) {
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

    memcpy(colors, D_00416DE0, sizeof(colors));
    memcpy(bankOneCoordinates, D_00416DF0, sizeof(bankOneCoordinates));
    memcpy(bankZeroCoordinates, D_00416E50, sizeof(bankZeroCoordinates));

    xOffset = 0;
    yOffset = 0;
    bank = D_00438F54->bank;
    if (D_00438F54->enabled[bank] == 0) {
        return;
    }
    coordinates = bank == 0 ? bankZeroCoordinates : bankOneCoordinates;
    slotCount = D_00438F54->currentIndex;

    for (index = 0; index < slotCount; index++) {
        switch (D_00438F58[bank]->state[index]) {
        case 1:
        case 2:
        case 3:
        case 7: {
            for (channel = 0; channel < 4; channel++) {
                s32 adjustment;
                u32 replicated;

                colors[channel] = btlSetSlotLowByteClamped(
                    btlResourceBlock->resC, coordinates[index][2], channel,
                    D_00438F58[bank]->slotFade[index] +
                    D_00438F58[bank]->colorAdjustments[index][channel]);
                adjustment = D_00438F58[bank]->colorAdjustments[index][channel];
                replicated = (u32)adjustment;

                {
                    u32 red = (replicated << 24) | 0x80000000;
                    u32 green = (replicated << 16) | 0x00800000;
                    u32 blue = (replicated << 8) | 0x00008000;

                    colors[channel] |= (red | green) | blue;
                }
            }

            btlResourceBlock->resC->workEntries[coordinates[index][2]]
                .geometry.angleDegrees = D_00438F58[bank]->unk6C[index];
            func_001CE418(btlResourceBlock->resC, coordinates[index][2],
                          &xOffset, &yOffset,
                          (f32)D_00438F58[bank]->unk8C[index]);
            func_00306C28((coordinates[index][0] + xOffset) << 4,
                          (coordinates[index][1] + yOffset) << 3,
                          0, colors, 0, btlResourceBlock->resC,
                          coordinates[index][2], 0x53);
            btlResourceBlock->resC->workEntries[coordinates[index][2]]
                .geometry.angleDegrees = 0.0f;
            fldScaleSceneCoordinateRecord(btlResourceBlock->resC,
                                         coordinates[index][2]);

            if (D_00438F58[bank]->unk6C[index] > 0.0f) {
                for (channel = 0; channel < 4; channel++) {
                    colors[channel] = btlSetSlotLowByteClamped(
                        btlResourceBlock->resC, coordinates[index][2],
                        channel,
                        D_00438F58[bank]->slotFade[index] >> 1);
                }
                func_00306C28(coordinates[index][0] << 4,
                              coordinates[index][1] << 3,
                              0, colors, 0, btlResourceBlock->resC,
                              D_00438F58[bank]->entries[index], 0x53);
            }
            break;
        }
        case 4:
        case 5:
            func_001CDD38(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 0);
            func_001CDD38(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 1);
            break;
        case 6:
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(
                    btlResourceBlock->resC, coordinates[index][2],
                    channel, D_00438F58[bank]->slotFade[index]);
            }
            func_00306C28(coordinates[index][0] << 4,
                          coordinates[index][1] << 3,
                          0, colors, 0, btlResourceBlock->resC,
                          coordinates[index][2], 0x53);
            func_001CDD38(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 0);
            func_001CDD38(btlResourceBlock->resC,
                          coordinates[index][2] + 0x32,
                          coordinates[index][0], coordinates[index][1],
                          bank, index, 1);
            break;
        }
    }
}


INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CDD38);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416ED0);

void func_001CDEC0(s32 bank, s32 index) {
    if (D_00438F58[bank]->secondaryState[index] == 8) {
        D_00438F58[bank]->colorAdjustments[index][0] -= 8;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        D_00438F58[bank]->colorAdjustments[index][1] -= 8;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
        D_00438F58[bank]->colorAdjustments[index][2] -= 8;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
        D_00438F58[bank]->colorAdjustments[index][3] -= 8;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
        if (D_00438F58[bank]->colorAdjustments[index][0] <= 0) D_00438F58[bank]->secondaryState[index] = 0;
    }
    if (D_00438F58[bank]->secondaryState[index] == 7) {
        D_00438F58[bank]->colorAdjustments[index][0] -= 24;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        if (D_00438F58[bank]->colorAdjustments[index][0] <= 0) D_00438F58[bank]->secondaryState[index]++;
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 6) < 2) {
        D_00438F58[bank]->colorAdjustments[index][1] -= 27;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 5) < 3) {
        D_00438F58[bank]->colorAdjustments[index][3] -= 29;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
    }
    if ((u8)(D_00438F58[bank]->secondaryState[index] - 4) < 4) {
        D_00438F58[bank]->colorAdjustments[index][2] -= 32;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
    }
    switch (D_00438F58[bank]->secondaryState[index]) {
    case 3:
        D_00438F58[bank]->colorAdjustments[index][2] += 64;
        D_00438F58[bank]->colorAdjustments[index][2] = D_00438F58[bank]->colorAdjustments[index][2] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][2] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][2];
        if (D_00438F58[bank]->colorAdjustments[index][2] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 4:
        D_00438F58[bank]->colorAdjustments[index][3] += 64;
        D_00438F58[bank]->colorAdjustments[index][3] = D_00438F58[bank]->colorAdjustments[index][3] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][3] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][3];
        if (D_00438F58[bank]->colorAdjustments[index][3] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 5:
        D_00438F58[bank]->colorAdjustments[index][1] += 64;
        D_00438F58[bank]->colorAdjustments[index][1] = D_00438F58[bank]->colorAdjustments[index][1] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][1] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][1];
        if (D_00438F58[bank]->colorAdjustments[index][1] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 6:
        D_00438F58[bank]->colorAdjustments[index][0] += 64;
        D_00438F58[bank]->colorAdjustments[index][0] = D_00438F58[bank]->colorAdjustments[index][0] <= 0 ? 0 : D_00438F58[bank]->colorAdjustments[index][0] > 127 ? 127 : D_00438F58[bank]->colorAdjustments[index][0];
        if (D_00438F58[bank]->colorAdjustments[index][0] >= 127) D_00438F58[bank]->secondaryState[index]++;
        break;
    case 2:
        D_00438F58[bank]->secondaryState[index]++;
        break;
    case 1:
        D_00438F58[bank]->colorAdjustments[index][0] = 127;
        D_00438F58[bank]->colorAdjustments[index][1] = 127;
        D_00438F58[bank]->colorAdjustments[index][2] = 127;
        D_00438F58[bank]->colorAdjustments[index][3] = 127;
        D_00438F58[bank]->secondaryState[index]++;
        break;
    }
}



void fldScaleSceneCoordinateRecord(EffectSlotSet *work, s32 index) {
    work->workEntries[index].geometry.bounds[2] = work->workEntries[index].sourceWidth << 4;
    work->workEntries[index].geometry.bounds[3] = work->workEntries[index].sourceHeight << 3;
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CE418);

void fldSetSceneSlotRange(s32 index) {
    SceneSlotFadeWork *scene = D_00438F54;
    if (scene->currentIndex < index) {
        s32 i;
        for (i = 0; i <= index; i++) {
            scene = D_00438F54;
            scene->fade[i][scene->bank] = 0x80;
            scene->phase[i][scene->bank] = 3;
        }
        D_00438F54->lastIndex = index;
    }
    D_00438F54->currentIndex = index;
}

extern s32 btlGetNamedTaskPairStatusOrUnavailable(void);

void func_001CE5C8(void) {
    s32 bank = D_00438F54->bank;
    s32 last = D_00438F54->lastIndex;
    s32 status = btlGetNamedTaskPairStatusOrUnavailable();
    s32 i;
    if (D_00438F54->enabled[bank] == 0) return;
    if (status != 0 && status != 3) return;
    for (i = 0; i <= last; i++) {
        switch (D_00438F54->phase[i][bank]) {
        case 1:
            D_00438F54->timer++;
            D_00438F54->timer = D_00438F54->timer <= 0 ? 0 : D_00438F54->timer > 34 ? 34 : D_00438F54->timer;
            if (D_00438F54->timer >= 34) {
                D_00438F54->fade[i][bank] += 64;
                D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 0 ? 0 : D_00438F54->fade[i][bank] > 255 ? 255 : D_00438F54->fade[i][bank];
                if (D_00438F54->fade[i][bank] >= 255) {
                    D_00438F54->phase[i][bank]++;
                    D_00438F54->phase[i + 1][bank] = 1;
                }
            }
            break;
        case 2:
            D_00438F54->fade[i][bank] -= 16;
            D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 128 ? 128 : D_00438F54->fade[i][bank] > 255 ? 255 : D_00438F54->fade[i][bank];
            if (D_00438F54->fade[i][bank] <= 128) {
                D_00438F54->phase[i][bank]++;
                if (i == last) D_00438F54->completed = 1;
            }
            break;
        case 3:
            break;
        case 4:
            D_00438F54->fade[i][bank] -= 64;
            D_00438F54->fade[i][bank] = D_00438F54->fade[i][bank] <= 0 ? 0 : D_00438F54->fade[i][bank] > 128 ? 128 : D_00438F54->fade[i][bank];
            break;
        }
    }
}


extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436878);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436880);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436888);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436890);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_00436898);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004368A0);

void btlDrawSceneSlotFades(void) {
    u32 overlays[2] = {0x0000FF00, 0xFF000000};
    u32 colors[4] = {0x80808080, 0x80808080, 0x80808080, 0x80808080};
    s32 bank;
    s32 last;
    s32 channel;
    s32 fade;

    bank = D_00438F54->bank;
    if (D_00438F54->enabled[bank] <= 0) {
        return;
    }
    last = D_00438F54->currentIndex - 1;
    if (last >= 0) {
        /* Solid background strips use the live fade again after color lookup. */
        if (last == 1 || last == 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 16, channel,
                D_00438F54->fade[2][bank]);
                if (D_00438F54->fade[2][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 16, 0x53);
        }
        if (last >= 2) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_00438F54->fade[3][bank]);
                if (D_00438F54->fade[3][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(392 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 18, channel,
                D_00438F54->fade[4][bank]);
                if (D_00438F54->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(425 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 18, 0x53);
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 19, channel,
                D_00438F54->fade[4][bank]);
                if (D_00438F54->fade[4][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(349 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 19, 0x53);
        }
        if (last >= 4) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 20, channel,
                D_00438F54->fade[5][bank]);
                if (D_00438F54->fade[5][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(298 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 20, 0x53);
        }
        if (last >= 7) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 17, channel,
                D_00438F54->fade[8][bank]);
                if (D_00438F54->fade[8][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(206 << 4, 33 << 3, 0, colors, 0,
            btlResourceBlock->resC, 17, 0x53);
        }

    }

    if (last >= 0) {
        /* Foreground connectors retain the selected corner fade across lookup. */
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_00438F54->fade[1][bank];
            } else {
                fade = D_00438F54->fade[0][bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 10, channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[bank];
            }
        }
        func_00306C28(440 << 4, 6 << 3, 0, colors, 0,
        btlResourceBlock->resC, 10, 0x53);
        if (last >= 3) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[4][bank];
                } else {
                    fade = D_00438F54->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 11, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(395 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 11, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[4][bank];
                } else {
                    fade = D_00438F54->fade[3][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 12, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(362 << 4, 21 << 3, 0, colors, 0,
            btlResourceBlock->resC, 12, 0x53);
        }
        if (last == 5) {
            for (channel = 0; channel < 4; channel++) {
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 13, channel,
                D_00438F54->fade[6][bank]);
                if (D_00438F54->fade[6][bank] > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(302 << 4, 46 << 3, 0, colors, 0,
            btlResourceBlock->resC, 13, 0x53);
        }
        if (last >= 6) {
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[7][bank];
                } else {
                    fade = D_00438F54->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 14, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(302 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 14, 0x53);
            for (channel = 0; channel < 4; channel++) {
                if (channel == 0 || channel == 2) {
                    fade = D_00438F54->fade[7][bank];
                } else {
                    fade = D_00438F54->fade[6][bank];
                }
                colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC, 15, channel, fade);
                if (fade > 128) {
                    colors[channel] |= overlays[bank];
                }
            }
            func_00306C28(269 << 4, 48 << 3, 0, colors, 0,
            btlResourceBlock->resC, 15, 0x53);
        }
    }
}


extern u32 btlSetSlotLowByteClamped(EffectSlotSet *, s32, s32, s32);
extern const s32 D_00416FA0[10][3];


void btlDrawSceneSlotFadeTiles(void) {
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

    memcpy(finalRows, D_00416FA0, sizeof(finalRows));

    if (D_00438F54->enabled[D_00438F54->bank] <= 0) {
        return;
    }
    last = D_00438F54->currentIndex - 1;
    for (row = 0; row < D_00438F54->currentIndex; row++) {
        for (channel = 0; channel < 4; channel++) {
            if (channel == 0 || channel == 2) {
                fade = D_00438F54->fade[row + 1][D_00438F54->bank];
            } else {
                fade = D_00438F54->fade[row][D_00438F54->bank];
            }
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       rows[row][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_00438F54->bank];
            }
        }
        func_00306C28(rows[row][0] << 4, rows[row][1] << 3, 0, colors,
                      0, btlResourceBlock->resC, rows[row][2], 0x53);
    }
    if (last >= 0) {
        for (channel = 0; channel < 4; channel++) {
            fade = D_00438F54->fade[last + 1][D_00438F54->bank];
            colors[channel] = btlSetSlotLowByteClamped(btlResourceBlock->resC,
                                                       finalRows[last][2], channel, fade);
            if (fade > 128) {
                colors[channel] |= overlays[D_00438F54->bank];
            }
        }
        func_00306C28(finalRows[last][0] << 4, finalRows[last][1] << 3,
                      0, colors, 0, btlResourceBlock->resC, finalRows[last][2], 0x53);
    }
}


void fldInitSceneFadeRecords(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    BtlSceneSlot *slot = scene->slots;
    BtlSceneFadingRecord *rec = scene->fading;
    u32 i = 0;
    while (i < 8 && slot->group != 0) {
        memcpy(&rec->slot, slot, sizeof(BtlSceneSlot));
        rec->alpha = 0x80;
        rec->target = -1;
        i++;
        slot++;
        rec++;
    }
    while (i < 8) {
        memset(rec, 0, sizeof(*rec));
        rec->target = -1;
        i++;
        rec++;
    }
}

s32 btlFindSceneSlotById(BtlSceneSlot *request) {
    BtlSceneSlot *slot = ((BattleSceneWork *)btlGetRuntime())->slots;
    u8 id = request->id;
    u32 i;
    for (i = 0; i < 8; i++) {
        if (slot->id == id) {
            return i;
        }
        slot++;
    }
    return -1;
}

u8 *fldFindSceneSlotRecord(s32 index) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    s32 slot = btlFindSceneSlotById((BtlSceneSlot *)index);
    u8 *entry = 0;
    if (slot != -1) {
        entry = (u8 *)&scene->slots[slot];
    }
    return entry;
}

s32 btlFadeStaleSceneSlots(s32 lastFadeIndex) {
    u32 i = 0;
    s32 changed = 0;
    BattleSceneWork *work = (BattleSceneWork *)btlGetRuntime();
    BtlSceneFadingRecord *record = work->fading;
    BtlSceneSlot *slot = work->slots;
    for (; i < 8; i++, slot++, record++) {
        if (record->slot.id != slot->id) {
            if (fldFindSceneSlotRecord((s32)record) == 0) {
                if (record->alpha != 0) {
                    record->alpha = record->alpha - 8;
                    changed = 1;
                }
            }
        }
    }
    return changed;
}

s32 fldCountSceneFadeKinds(BattleSceneWork *scene, s32 *outFadeCount) {
    SceneGlobalState *global;
    BtlSceneFadingRecord *rec = scene->fading;
    BtlSceneSlot *slot;
    u32 fadeCount = 0;
    s32 countA = 0;
    s32 countB = 0;
    u32 slotCount;
    while (fadeCount < 8 && rec[fadeCount].slot.group != 0) {
        if (rec[fadeCount].slot.group == 1) {
            countA++;
        }
        if (rec[fadeCount].slot.group == 2) {
            countB++;
        }
        fadeCount++;
    }
    global = (SceneGlobalState *)btlTrackedTaskHandles;
    if (global->fadeLatched == 0) {
        if (countA != 0 || countB != 0) {
            global->fadeKindA = countA;
            global->fadeKindB = countB;
            global->fadeLatched = 1;
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CF800);

s32 fldUpdateConditionalSceneCleanup(KwlnTask *task) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    if ((scene->flags & 0x200) == 0) {
        return 0;
    }
    if (((SceneGlobalState *)btlTrackedTaskHandles)->stage == 1 &&
        (((SceneGlobalState *)btlTrackedTaskHandles)->flags & 0x100) == 0) {
        return 0;
    }
    func_001CF800();
    return 0;
}

void fldResetSceneStatus(KwlnTask *task) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    scene->sceneStatus = 0;
}

void fldBeginSceneTransition(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();
    func_001C7DB8(1, 8);
    scene->flags |= 0x200;
}

void fldClearSceneTransition(void) {
    btlGetRuntime();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    btlPanelResourcesLoad();
}

u32 fldGetSceneIndexedValue(s32 index) {
    BattleSceneWork *scene;

    scene = (BattleSceneWork *)btlGetRuntime();
    return scene->values[index];
}

void func_001CFB90(void) {
}

extern char *D_004368B0;

extern void btlLoadResourceBlock(void);

extern void btlStartRegisteredChildTask(void);

extern void btlInitializeStaffActorPanels(void);


void fldCreateSceneCleanupTask(void) {
    BattleSceneWork *scene;
    KwlnTask *task;
    if (kwlnTaskGetTaskByName(D_004368B0) == 0) {
        btlGetRuntime();
    }
    scene = (BattleSceneWork *)btlGetRuntime();
    task = kwlnTaskCreate(D_004368B0, 0x2B0E, 1, 1, fldUpdateConditionalSceneCleanup, fldResetSceneStatus, 0);
    func_00101968(scene->taskParent, task);
    scene->sceneStatus = task;
    btlLoadResourceBlock();
    btlStartRegisteredChildTask();
    btlInitializeStaffActorPanels();
    func_001C16B0(1);
    fldBeginSceneTransition();
}

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001CFC40);

static inline void btlDestroyTrackedTaskIfPresent(s32 slot) {
    if (btlGetTrackedTaskHandle(slot) != 0) {
        kwlnTaskDestroyWithHierarchy((KwlnTask *)btlGetTrackedTaskHandle(slot), 0);
    }
}

void fldDestroySceneTasksAndBuffers(void) {
    BattleSceneWork *scene = (BattleSceneWork *)btlGetRuntime();

    if (scene->sceneObject != 0) {
        kwlnTaskDestroyWithHierarchy(scene->sceneObject, 0);
        scene->sceneObject = 0;
    }
    if (scene->spriteObject != 0) {
        kwlnTaskDestroyWithHierarchy(scene->spriteObject, 0);
        scene->spriteObject = 0;
    }
    if (scene->sceneStatus != 0) {
        kwlnTaskDestroyWithHierarchy(scene->sceneStatus, 0);
        scene->sceneStatus = 0;
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
    sdfReleaseChipBlock(D_00438F58[1]);
    sdfReleaseChipBlock(D_00438F58[0]);
    sdfReleaseChipBlock(D_00438F54);
    sdfReleaseChipBlock(btlTrackedTaskHandles);
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416FA0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004368B0);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004368BC);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004368BE);

INCLUDE_SDATA(const s32, "game/code_001B2AF8", D_004368C0);


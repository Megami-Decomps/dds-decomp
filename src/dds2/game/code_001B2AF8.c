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

s32 func_001B2AF8(BtlUnit *actor) {
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

s32 func_001B3BE0(void) {
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

s32 func_001B4210(void) {
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001B4EB8);

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

s32 func_001B52D8(ActionStateLink *action) {
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

void btlInvalidateSceneFadeCounts(void) {
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

extern u32 itfCreateConvertedTextGlyph(s32, s32, s32, u32, const u8 *, s32);

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
    frFontQueueGlyphForCurrentDrawBuffer((struct FrFontGlyph *)glyph);
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

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416138);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004162F8);

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001BB1E8);

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

void btlReleaseRegisteredTaskBuffer(void) {
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C0828);

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

void func_001C1520(void) {
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

void func_001C1A68(BtlState *battle, BattleActorPanelWork *work) {
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
s32 func_001C26E0(KwlnTask *task) {
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

INCLUDE_ASM(const s32, "game/code_001B2AF8", func_001C3168);

void btlReleaseTrackedTaskResource(void) {
    sdfReleaseResourceAllocation(*(struct SdfMemBlock **)(kwlnTaskGetUserValue(kwlnTaskGetTaskByName(D_004367CC)) + 0x1200));
    btlSetTrackedTaskHandle(8, 0);
}

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_004167E0);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416800);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416820);

INCLUDE_RODATA(const s32, "game/code_001B2AF8", D_00416830);

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


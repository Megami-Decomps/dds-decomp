#include "common.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "btl_action.h"
#include "btl_unit_tasks.h"
#include "btl_state.h"
#include "eff_transform.h"
#include "dat_state.h"
#include "dat_command.h"
#include "evt_unit.h"
#include "mdl.h"

#define BTL_AI_SLOT_COUNT 5

extern void func_00211EA8();

extern u32 btlPickWeightedAiSlot();

/* Native 0x10-byte AI selection scratch. Its producer retains the command
 * actor and species/mode; the conditional-action dispatcher uses +8 as a
 * table row and +0xC as its query selector. This is not the singleton battle work. */
typedef struct BtlAiScratchWork {
    ActionStateLink *actor;
    s32 speciesId;
    s32 rowIndex;
    s32 conditionKind;
} BtlAiScratchWork;

extern BtlAiScratchWork *btlActionScratchWork;

/* Per-species AI table (0x15C bytes each): five rows of five weighted slots. */
typedef struct AiSlot {
    u8 weight;
    u8 pad1;
    u16 actionId;
    u32 actionArg;
} AiSlot;

/* AICALC.TBL: each decision tier tests three packed predicates, then
 * selects a route in most-specific-first order; route 8 makes no selection. */
typedef struct AiDecisionRow {
    u32 predicates[3];
    u8 routes[8];
} AiDecisionRow;

typedef struct AiSpecies {
    u8 pad00[4];
    AiDecisionRow decisions[3];
    AiSlot slot[25];
    u8 pad108[0x54];
} AiSpecies;

extern AiSpecies *datEnemyAiRecords;

extern void *sdfAllocAndClearQuadwords(s32);

extern void sdfReleaseChipBlock(void *);

/* Pick a weighted slot in one species row, run its action and release the shared scratch allocation. */
void btlRunWeightedAiAction(ActionStateLink *task, s32 rowIndex) {
    u16 speciesId;
    s32 slotIndex;

    btlActionScratchWork = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    speciesId = task->unit->partyRecord.unitId;
    slotIndex = btlPickWeightedAiSlot((s32)task->unit, speciesId, rowIndex);
    func_00211EA8(task, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionId, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
}

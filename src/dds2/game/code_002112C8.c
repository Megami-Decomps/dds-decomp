#include "common.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "btl_command.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "btl_action.h"
#include "btl_ai_table.h"
#include "btl_ai_scratch.h"
#include "btl_unit_tasks.h"
#include "btl_state.h"
#include "eff_transform.h"
#include "dat_state.h"
#include "dat_command.h"
#include "evt_unit.h"
#include "mdl.h"

#define BTL_AI_SLOT_COUNT 5

extern s32 func_00211EA8();

extern u32 btlPickWeightedAiSlot();


extern BtlAiScratchWork *btlActionScratchWork;

extern AiSpecies *datEnemyAiRecords;



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

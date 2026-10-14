#include "common.h"
#include "btl_ai_slot.h"
#include "sdf_chip.h"
#include "btl_task_condition.h"
#include "eff_transform.h"
#include "btl_state.h"
#include "btl_command.h"
#include "btl_model_record.h"
#include "sdf_draw.h"
#include "evt_unit.h"
#include "mdl.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "btl_action.h"
#include "btl_ai_table.h"
#include "btl_ai_scratch.h"
#include "btl_unit_tasks.h"
#include "dat_state.h"
#include "dat_command.h"

#define BTL_AI_SLOT_COUNT 5

extern BtlAiScratchWork *btlActionScratchWork;

extern AiSpecies *datEnemyAiRecords;



extern s32 btlRunAiAction();


extern void (*btlAiActionHandlers[])(BtlTask *, u32, s32);

/* Pick a weighted slot in one species row, run its action and release the shared scratch allocation. */
void btlRunWeightedAiAction(BtlTask *task, s32 rowIndex) {
    u16 speciesId;
    s32 slotIndex;

    btlActionScratchWork = sdfAllocAndClearQuadwords(sizeof(BtlAiScratchWork));
    speciesId = task->unit->partyRecord.unitId;
    slotIndex = btlPickWeightedAiSlot(task->unit, speciesId, rowIndex);
    btlRunAiAction(task, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionId, datEnemyAiRecords[speciesId].slot[rowIndex * BTL_AI_SLOT_COUNT + slotIndex].actionArg);
    sdfReleaseChipBlock(btlActionScratchWork);
}

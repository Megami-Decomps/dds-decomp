#include "common.h"
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
#include "btl_unit_tasks.h"
#include "dat_state.h"
#include "dat_command.h"

#define BTL_AI_SLOT_COUNT 5

/* Command-actor state retained by AI queries; this is not a battle unit.
 * The first queued action is also the word tested by btlActionMatchesUnit. */
typedef union BtlActionSlot {
    s32 word;
    s16 actionId;
} BtlActionSlot;

typedef struct BtlActionTask {
    u8 pad00[0xC];
    s32 flags; /* 0x0C: AI context flags */
    u8 pad10[0x78];
    u16 aiCounter; /* 0x88: wraps as a halfword, then clamps to 0xFF */
    u8 pad8A[0xBC];
    s8 lowHpActionHold; /* 0x146: positive suppresses the low-HP action */
    u8 pad147;
    BtlActionSlot actions[8]; /* 0x148 */
    u8 pad168[4];
    struct BtlActionTask *next; /* 0x16C, same link as BtlTask */
} BtlActionTask;

/* Native 0x10-byte AI selection scratch. Its producer retains the command
 * actor and species/mode; the conditional-action dispatcher uses +8 as a
 * table row and +0xC as its query selector. This is not the singleton battle work. */
typedef struct BtlAiScratchWork {
    BtlActionTask *actor;
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

extern s32 btlRunAiAction();

extern u32 btlPickWeightedAiSlot();

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

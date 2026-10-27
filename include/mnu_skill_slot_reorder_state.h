#ifndef MNU_SKILL_SLOT_REORDER_STATE_H
#define MNU_SKILL_SLOT_REORDER_STATE_H

#include "common.h"

struct dds3ProcessID_t;

/* Getter returns 0 while empty and 1 after the first slot is saved. The source
 * index is -1 until then; confirming another slot commits the reorder. */
typedef u32 MnuSkillSlotReorderState;
enum {
    MNU_SKILL_SLOT_REORDER_AWAITING_SOURCE = 0, /* No saved slot index. */
    MNU_SKILL_SLOT_REORDER_SOURCE_SELECTED = 1, /* First slot is saved. */
};

void mnuClearSkillSlotReorderSelection(struct dds3ProcessID_t *task);
MnuSkillSlotReorderState mnuGetSkillSlotReorderState(
    struct dds3ProcessID_t *callback);

#endif /* MNU_SKILL_SLOT_REORDER_STATE_H */

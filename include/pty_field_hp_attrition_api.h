#ifndef PTY_FIELD_HP_ATTRITION_API_H
#define PTY_FIELD_HP_ATTRITION_API_H

#include "common.h"

/* Select the field-driven HP-loss pass over the frontline roster. Mode 1 also
 * requires the member's 0x80 status bit; modes 3 and 4 are DDS2 terrain cases. */
typedef s32 PtyFieldHpAttritionMode;
enum {
    PTY_FIELD_HP_ATTRITION_STATUS_AFFECTED_FRONTLINE = 1,
    PTY_FIELD_HP_ATTRITION_STANDARD_TERRAIN = 2,
    PTY_FIELD_HP_ATTRITION_SOUND_CLASS_20 = 3,
    PTY_FIELD_HP_ATTRITION_SOUND_CLASS_30 = 4,
};

void ptyApplyFieldHpAttrition(PtyFieldHpAttritionMode mode);

#endif /* PTY_FIELD_HP_ATTRITION_API_H */

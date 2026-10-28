#ifndef PTY_FIELD_HP_ATTRITION_API_H
#define PTY_FIELD_HP_ATTRITION_API_H

#include "common.h"

/* Select a field-driven HP-loss pass over the frontline roster. Each pass
 * leaves living units at 1 HP or above. */
typedef s32 PtyFieldHpAttritionMode;
enum {
    PTY_FIELD_HP_ATTRITION_FRONTLINE_STATUS_0X80_3_PERCENT = 1,
};

#if defined(VERSION_DDS1)
enum {
    PTY_FIELD_HP_ATTRITION_FRONTLINE_3_PERCENT = 2,
};
#elif defined(VERSION_DDS2)
enum {
    PTY_FIELD_HP_ATTRITION_FRONTLINE_10_PERCENT = 2,
    PTY_FIELD_HP_ATTRITION_FRONTLINE_50_PERCENT = 3,
    PTY_FIELD_HP_ATTRITION_FRONTLINE_30_PERCENT = 4,
};
#else
#error "pty_field_hp_attrition_api.h requires a DDS title version"
#endif

void ptyApplyFieldHpAttrition(PtyFieldHpAttritionMode mode);

#endif /* PTY_FIELD_HP_ATTRITION_API_H */

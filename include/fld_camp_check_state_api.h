#ifndef FLD_CAMP_CHECK_STATE_API_H
#define FLD_CAMP_CHECK_STATE_API_H

#include "common.h"

typedef u8 FldCampCheckState;
enum {
    FLD_CAMP_CHECK_STATE_ENCOUNTER_FLAG_SET = 0,
    FLD_CAMP_CHECK_STATE_MENU_ACTIVE = 1,
    FLD_CAMP_CHECK_STATE_HANDOFF_DELAY = 2,
    FLD_CAMP_CHECK_STATE_IDLE = 3
};

FldCampCheckState fldProcCampChk(void);

#endif /* FLD_CAMP_CHECK_STATE_API_H */

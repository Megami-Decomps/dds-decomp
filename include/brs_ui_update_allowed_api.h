#ifndef BRS_UI_UPDATE_ALLOWED_API_H
#define BRS_UI_UPDATE_ALLOWED_API_H

#include "mnu_result.h"

typedef s8 BrsUiUpdatePermission;
enum {
    BRS_UI_UPDATE_NOT_ALLOWED = 0,
    BRS_UI_UPDATE_ALLOWED = 1,
};

typedef s8 BrsUiUpdateBlockedState;
enum {
    BRS_UI_UPDATE_UNBLOCKED = 0,
    BRS_UI_UPDATE_BLOCKED = 1,
};

/* Check whether the result-screen work can update its UI this frame. */
BrsUiUpdatePermission brsTaskIsUiUpdateAllowed(BrsSkillPackageWork *work);
BrsUiUpdateBlockedState brsTaskIsUpdateBlocked(void);

#endif /* BRS_UI_UPDATE_ALLOWED_API_H */

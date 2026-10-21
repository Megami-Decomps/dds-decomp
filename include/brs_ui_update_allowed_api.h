#ifndef BRS_UI_UPDATE_ALLOWED_API_H
#define BRS_UI_UPDATE_ALLOWED_API_H

#include "mnu_result.h"

/* Check whether the result-screen work can update its UI this frame. */
s8 brsTaskIsUiUpdateAllowed(BrsSkillPackageWork *work);

#endif /* BRS_UI_UPDATE_ALLOWED_API_H */

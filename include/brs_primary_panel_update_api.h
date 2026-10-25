#ifndef BRS_PRIMARY_PANEL_UPDATE_API_H
#define BRS_PRIMARY_PANEL_UPDATE_API_H

#include "common.h"

/* Return the primary panel task's opacityReady state, or zero if no task exists. */
typedef s8 BrsPrimaryPanelReadyState;

BrsPrimaryPanelReadyState brsIsPrimaryPanelUpdateReady(void);

#endif /* BRS_PRIMARY_PANEL_UPDATE_API_H */

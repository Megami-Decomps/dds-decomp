#ifndef BRS_PRIMARY_PANEL_UPDATE_API_H
#define BRS_PRIMARY_PANEL_UPDATE_API_H

#include "common.h"

/* Return the primary panel task's opacityReady state, or zero if no task exists. */
typedef s8 BrsPrimaryPanelOpacityReadyState;
#define BRS_PRIMARY_PANEL_OPACITY_NOT_READY 0
#define BRS_PRIMARY_PANEL_OPACITY_READY 1

BrsPrimaryPanelOpacityReadyState brsIsPrimaryPanelUpdateReady(void);

#endif /* BRS_PRIMARY_PANEL_UPDATE_API_H */

#ifndef ITF_MES_PANEL_MODE_H
#define ITF_MES_PANEL_MODE_H

#include "common.h"
#include "itf_mes_window_index.h"

/* Signed selector: modes 0/1 use standard bounds; unlisted values also fall back. */
typedef s32 ItfMesPanelMode;
enum {
    ITF_MES_PANEL_MODE_STANDARD = 0,
    ITF_MES_PANEL_MODE_STANDARD_VARIANT = 1,
    ITF_MES_PANEL_MODE_WIDE = 2,
    ITF_MES_PANEL_MODE_NO_SPRITE_TRANSITION = 3
};

/* Alpha ceiling for the message-panel sprite; nonpositive values select 0x80. */
typedef s32 ItfMesPanelFadeLimit;

void itfMesMngChangeWindowType(ItfMesWindowIndex window, ItfMesPanelMode panelMode,
    ItfMesPanelFadeLimit fadeLimit);

#endif /* ITF_MES_PANEL_MODE_H */

#ifndef ITF_PANEL_API_H
#define ITF_PANEL_API_H

#include "common.h"
#include "sdf_packet_list.h"

struct UiSprite;

void itfPnlDrawTag(struct UiSprite *panel, struct SdfListHead *list);
void DrawTestAlphaPrim(struct SdfListHead *list);

void itfPnlSetPosition(struct UiSprite *panel, s32 left, s32 top,
    s32 right, s32 bottom, s32 renderValue);
void itfPnlSetPositionOffset(struct UiSprite *panel, s32 leftDelta,
    s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta);
void itfPnlSetColor(struct UiSprite *panel, s32 firstValue,
    s32 secondValue, s32 thirdValue, s32 fourthValue);

#endif /* ITF_PANEL_API_H */

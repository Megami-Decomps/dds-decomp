#ifndef ITF_PANEL_API_H
#define ITF_PANEL_API_H

#include "common.h"
#include "sdf_packet_list.h"

struct itfpanel_t;
struct sdfTexHandle_t;

/* Allocate the panel kind's payload and bind its shared UI texture handle. */
struct itfpanel_t *itfCreateUiSpriteByKind(s32 kind,
    struct sdfTexHandle_t *texture);

void itfPnlDrawTag(struct itfpanel_t *panel, struct sdfDrawTag_t *list);
void DrawTestAlphaPrim(struct sdfDrawTag_t *list);

void itfPnlSetPosition(struct itfpanel_t *panel, s32 left, s32 top,
    s32 right, s32 bottom, s32 renderValue);
void itfPnlSetPositionOffset(struct itfpanel_t *panel, s32 leftDelta,
    s32 topDelta, s32 rightDelta, s32 bottomDelta, s32 renderValueDelta);
void itfPnlSetColor(struct itfpanel_t *panel, s32 firstValue,
    s32 secondValue, s32 thirdValue, s32 fourthValue);

#endif /* ITF_PANEL_API_H */

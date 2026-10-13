#ifndef EVT_GRADIENT_RECT_H
#define EVT_GRADIENT_RECT_H

#include "common.h"

/* Submit a rectangle at GS depth with colors ordered top-left, top-right,
 * bottom-right, bottom-left. Both title providers consume this exact order. */
void evtSubmitGradientRectAtDepth(s32 x, s32 y, s32 width, s32 height, u32 depth,
                                  s32 topLeft, s32 topRight, s32 bottomRight, s32 bottomLeft);

#endif /* EVT_GRADIENT_RECT_H */

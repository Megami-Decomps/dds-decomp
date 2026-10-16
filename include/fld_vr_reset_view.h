#ifndef FLD_VR_RESET_VIEW_H
#define FLD_VR_RESET_VIEW_H

#include "common.h"

/* Reset view spanning the VR header and its first following render word. */
typedef struct FldVrResetView {
    s16 previewX;
    s16 previewY;
    u8 opaque04[4];
    u32 words[15];
} FldVrResetView;

typedef char FldVrResetViewSizeCheck[(sizeof(FldVrResetView) == 0x44) ? 1 : -1];
typedef char FldVrResetViewPreviewXOffsetCheck[
    ((u32)&((FldVrResetView *)0)->previewX == 0) ? 1 : -1];
typedef char FldVrResetViewPreviewYOffsetCheck[
    ((u32)&((FldVrResetView *)0)->previewY == 2) ? 1 : -1];
typedef char FldVrResetViewOpaqueOffsetCheck[
    ((u32)&((FldVrResetView *)0)->opaque04 == 4) ? 1 : -1];
typedef char FldVrResetViewWordsOffsetCheck[
    ((u32)&((FldVrResetView *)0)->words == 8) ? 1 : -1];

#endif

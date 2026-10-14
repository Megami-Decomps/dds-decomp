#ifndef FILE_SCROLL_ARROW_STATE_H
#define FILE_SCROLL_ARROW_STATE_H

#include "common.h"

/* Shared menu scroll-arrow animation state. The client updates the angle and
 * directional alpha words; the word at +8 remains opaque. */
typedef struct FileScrollArrowState {
    s32 angle;
    s32 upAlpha;
    s32 unk08;
    s32 downAlpha;
} FileScrollArrowState;

typedef char FileScrollArrowStateSizeCheck[
    (sizeof(FileScrollArrowState) == 0x10) ? 1 : -1];
typedef char FileScrollArrowStateAngleOffsetCheck[
    ((u32)&((FileScrollArrowState *)0)->angle == 0x00) ? 1 : -1];
typedef char FileScrollArrowStateUpAlphaOffsetCheck[
    ((u32)&((FileScrollArrowState *)0)->upAlpha == 0x04) ? 1 : -1];
typedef char FileScrollArrowStateUnknownOffsetCheck[
    ((u32)&((FileScrollArrowState *)0)->unk08 == 0x08) ? 1 : -1];
typedef char FileScrollArrowStateDownAlphaOffsetCheck[
    ((u32)&((FileScrollArrowState *)0)->downAlpha == 0x0C) ? 1 : -1];

#endif /* FILE_SCROLL_ARROW_STATE_H */

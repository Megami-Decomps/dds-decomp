#ifndef BTL_CAMERA_CURSOR_H
#define BTL_CAMERA_CURSOR_H

#include "common.h"

/* Cross-title view of the battle-camera cursor. Both title providers use the
 * script selectors, frame, busy flag, path endpoints, captured direction, and
 * distance. The title-specific state between these fields remains opaque here. */
typedef struct BtlCameraCursorView {
    u16 unk_00;
    s16 frame;              /* 0x02 */
    s16 mode;
    s16 index;
    union {
        u16 unk_08;
        s8 category;
    };
    s16 unk_0A;
    s16 unk_0C;
    u16 unk_0E;
    u8 pad10[4];
    s8 busy;                /* 0x14: nonzero suspends camera updates */
    u8 pad15[0xB];
    f32 pathEnd[4];         /* 0x20 */
    f32 pathStart[4];       /* 0x30 */
    u8 pad40[0x40];
    f32 direction[4];       /* 0x80 */
    f32 distance;           /* 0x90 */
    u8 pad94[0x9C];
} BtlCameraCursorView;

typedef char BtlCameraCursorViewSizeCheck[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];
typedef char BtlCameraCursorViewPathEndOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->pathEnd == 0x20) ? 1 : -1];
typedef char BtlCameraCursorViewDirectionOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->direction == 0x80) ? 1 : -1];

#endif /* BTL_CAMERA_CURSOR_H */

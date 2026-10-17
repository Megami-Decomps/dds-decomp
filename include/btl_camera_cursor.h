#ifndef BTL_CAMERA_CURSOR_H
#define BTL_CAMERA_CURSOR_H

#include "common.h"

/* Shared battle-camera script cursor and path state (0x130 bytes).
 * Both title providers use the same field offsets. */
typedef struct BtlCameraCursorView {
    u16 useTimedScript;     /* 0x00 */
    s16 frame;              /* 0x02 */
    s16 mode;
    s16 index;
    union {
        u16 unk_08;
        s8 category;
    };
    s16 scriptSetIndex;    /* 0x0A */
    s16 scriptIndex;       /* 0x0C */
    u16 unk_0E;
    u16 kind;              /* 0x10: path kind */
    s8 phase;
    s8 counter;
    s8 busy;                /* 0x14: nonzero suspends camera updates */
    s8 restoreAlternate;   /* 0x15: snapshot restore selector */
    u8 pad16[0xA];
    f32 pathEnd[4];         /* 0x20 */
    f32 pathStart[4];       /* 0x30 */
    f32 eyeFrom[4];         /* 0x40 */
    f32 eyeTo[4];           /* 0x50 */
    f32 focusFrom[4];       /* 0x60 */
    f32 focusTo[4];         /* 0x70 */
    f32 direction[4];       /* 0x80 */
    f32 distance;           /* 0x90 */
    u8 pad94[0xC];
    f32 fov;               /* 0xA0 */
    u8 padA4[0xC];
    f32 startFrame;        /* 0xB0 */
    u8 padB4[0xC];
    f32 duration;          /* 0xC0 */
    u8 padC4[0xC];
    u8 parameterD0;        /* 0xD0: unsigned serialized flag */
    u8 padD1[0xF];
    f32 parameterE0;       /* 0xE0 */
    u8 padE4[0xC];
    f32 blendRate;         /* 0xF0 */
    u8 padF4[0xC];
    f32 rangeStart;        /* 0x100 */
    u8 pad104[0xC];
    f32 rangeEnd;          /* 0x110 */
    s32 parameterIndex114;
    s32 parameterIndex118;
    s32 parameterIndex11C;
    s32 parameterIndex120;
    s32 parameterIndex124;
    s32 parameterIndex128;
    s32 parameterIndex12C;
} BtlCameraCursorView;

typedef char BtlCameraCursorViewSizeCheck[(sizeof(BtlCameraCursorView) == 0x130) ? 1 : -1];
typedef char BtlCameraCursorViewTimedScriptOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->useTimedScript == 0x00) ? 1 : -1];
typedef char BtlCameraCursorViewScriptSetIndexOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->scriptSetIndex == 0x0A) ? 1 : -1];
typedef char BtlCameraCursorViewScriptIndexOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->scriptIndex == 0x0C) ? 1 : -1];
typedef char BtlCameraCursorViewPathEndOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->pathEnd == 0x20) ? 1 : -1];
typedef char BtlCameraCursorViewDirectionOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->direction == 0x80) ? 1 : -1];
typedef char BtlCameraCursorViewKindOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->kind == 0x10) ? 1 : -1];
typedef char BtlCameraCursorViewEyeFromOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->eyeFrom == 0x40) ? 1 : -1];
typedef char BtlCameraCursorViewFocusFromOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->focusFrom == 0x60) ? 1 : -1];
typedef char BtlCameraCursorViewDistanceOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->distance == 0x90) ? 1 : -1];
typedef char BtlCameraCursorViewFovOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->fov == 0xA0) ? 1 : -1];
typedef char BtlCameraCursorViewStartFrameOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->startFrame == 0xB0) ? 1 : -1];
typedef char BtlCameraCursorViewDurationOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->duration == 0xC0) ? 1 : -1];
typedef char BtlCameraCursorViewParameterD0OffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->parameterD0 == 0xD0) ? 1 : -1];
typedef char BtlCameraCursorViewParameterE0OffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->parameterE0 == 0xE0) ? 1 : -1];
typedef char BtlCameraCursorViewBlendRateOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->blendRate == 0xF0) ? 1 : -1];
typedef char BtlCameraCursorViewRangeStartOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->rangeStart == 0x100) ? 1 : -1];
typedef char BtlCameraCursorViewRangeEndOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->rangeEnd == 0x110) ? 1 : -1];
typedef char BtlCameraCursorViewFirstParameterIndexOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->parameterIndex114 == 0x114) ? 1 : -1];
typedef char BtlCameraCursorViewLastParameterIndexOffsetCheck[
    ((u32)&((BtlCameraCursorView *)0)->parameterIndex12C == 0x12C) ? 1 : -1];

#endif /* BTL_CAMERA_CURSOR_H */

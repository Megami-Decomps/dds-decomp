#ifndef FILE_LOAD_MENU_STATE_H
#define FILE_LOAD_MENU_STATE_H

typedef struct FileLoadMenuState {
    u8 unk00[4];              /* 0x00: opaque */
    s32 transitionTickCount;  /* 0x04 */
    s32 inputRepeatTimer;     /* 0x08 */
    s32 standardRowAlpha;     /* 0x0C: clamped to 0..0x80 for ordinary rows */
    s8 slideMode;             /* 0x10: 0 idle, 1/2 slide direction */
    u8 pad11[3];              /* 0x11 */
    s32 rowVerticalOffset;    /* 0x14 */
    s32 leadingRowAlpha;      /* 0x18 */
    s32 trailingRowAlpha;     /* 0x1C */
} FileLoadMenuState;

typedef char FileLoadMenuState_size_must_be_0x20[
    (sizeof(FileLoadMenuState) == 0x20) ? 1 : -1];
typedef char FileLoadMenuState_transitionTickCount_offset_must_be_4[
    ((u32)&((FileLoadMenuState *)0)->transitionTickCount == 4) ? 1 : -1];
typedef char FileLoadMenuState_inputRepeatTimer_offset_must_be_8[
    ((u32)&((FileLoadMenuState *)0)->inputRepeatTimer == 8) ? 1 : -1];
typedef char FileLoadMenuState_standardRowAlpha_offset_must_be_C[
    ((u32)&((FileLoadMenuState *)0)->standardRowAlpha == 0xC) ? 1 : -1];
typedef char FileLoadMenuState_slideMode_offset_must_be_10[
    ((u32)&((FileLoadMenuState *)0)->slideMode == 0x10) ? 1 : -1];
typedef char FileLoadMenuState_rowVerticalOffset_offset_must_be_14[
    ((u32)&((FileLoadMenuState *)0)->rowVerticalOffset == 0x14) ? 1 : -1];
typedef char FileLoadMenuState_leadingRowAlpha_offset_must_be_18[
    ((u32)&((FileLoadMenuState *)0)->leadingRowAlpha == 0x18) ? 1 : -1];
typedef char FileLoadMenuState_trailingRowAlpha_offset_must_be_1C[
    ((u32)&((FileLoadMenuState *)0)->trailingRowAlpha == 0x1C) ? 1 : -1];

#endif

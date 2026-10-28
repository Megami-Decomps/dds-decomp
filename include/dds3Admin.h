#ifndef DDS3_ADMIN_H
#define DDS3_ADMIN_H

#include "common.h"

/* Shared 0x24-byte administration task state. The task creators in both titles
 * allocate this record and pass it to the mode-transition callbacks below. */
typedef struct AdminWork {
    u32 flags;                    /* +0x00 */
    u32 value;                    /* +0x04 */
    s8 activeMode;                /* +0x08; -1 when no mode is active */
    s8 requestedMode;             /* +0x09; -1 when no request is pending */
    u8 historyIndex;              /* +0x0A */
    u8 pad0B;
    s8 signedHistory[8];          /* +0x0C */
    u8 unsignedHistory[8];        /* +0x14 */
    void *ownedRequestData;        /* +0x1C: copied request payload, freed by the task. */
    u8 requestSizeLowByte;        /* +0x20 */
    u8 modeTransitionDelay;       /* +0x21 */
    u8 pad22[2];
} AdminWork;

typedef char AdminWork_size_must_be_0x24[(sizeof(AdminWork) == 0x24) ? 1 : -1];
typedef char AdminWork_activeMode_at_0x08[((u32)&((AdminWork *)0)->activeMode == 0x08) ? 1 : -1];
typedef char AdminWork_requestedMode_at_0x09[((u32)&((AdminWork *)0)->requestedMode == 0x09) ? 1 : -1];
typedef char AdminWork_historyIndex_at_0x0A[((u32)&((AdminWork *)0)->historyIndex == 0x0A) ? 1 : -1];
typedef char AdminWork_history_at_0x0C[((u32)&((AdminWork *)0)->signedHistory == 0x0C) ? 1 : -1];
typedef char AdminWork_unsignedHistory_at_0x14[((u32)&((AdminWork *)0)->unsignedHistory == 0x14) ? 1 : -1];
typedef char AdminWork_ownedRequestData_at_0x1C[((u32)&((AdminWork *)0)->ownedRequestData == 0x1C) ? 1 : -1];
typedef char AdminWork_requestSize_at_0x20[((u32)&((AdminWork *)0)->requestSizeLowByte == 0x20) ? 1 : -1];
typedef char AdminWork_transitionDelay_at_0x21[((u32)&((AdminWork *)0)->modeTransitionDelay == 0x21) ? 1 : -1];

/* The administrator's modes (activeMode, requestedMode): rows of
 * ddsAdminModeCallbacks, the same rows in both titles. Named are the rows
 * whose functions keep their original names (Call<Mode>, Exit<Mode>,
 * Check<Mode>), and the battle's, whose cleanup is CheckNewBattleTest. */
typedef enum Dds3AdminMode {
    DDS3_ADMIN_MODE_DEBUG = 0,
    DDS3_ADMIN_MODE_BOOT_MC = 1,
    DDS3_ADMIN_MODE_TITLE = 2,
    DDS3_ADMIN_MODE_LOAD = 3,
    DDS3_ADMIN_MODE_END_SAVE = 4,
    DDS3_ADMIN_MODE_FIELD = 5,
    DDS3_ADMIN_MODE_RECOVER = 8,
    DDS3_ADMIN_MODE_TERMINAL = 9,
    DDS3_ADMIN_MODE_RESULT2 = 10,
    DDS3_ADMIN_MODE_STAFF = 12,
    DDS3_ADMIN_MODE_CONFIG = 13,
    DDS3_ADMIN_MODE_BATTLE = 14,
    DDS3_ADMIN_MODE_TERMINAL_TEST = 21,
    DDS3_ADMIN_MODE_SOUND_TEST = 23,
    DDS3_ADMIN_MODE_BILLBOARD_TEST = 24,
    DDS3_ADMIN_MODE_FONT_TEST = 25
} Dds3AdminMode;

#endif /* DDS3_ADMIN_H */

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
    void *requestData;            /* +0x1C */
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
typedef char AdminWork_requestData_at_0x1C[((u32)&((AdminWork *)0)->requestData == 0x1C) ? 1 : -1];
typedef char AdminWork_requestSize_at_0x20[((u32)&((AdminWork *)0)->requestSizeLowByte == 0x20) ? 1 : -1];
typedef char AdminWork_transitionDelay_at_0x21[((u32)&((AdminWork *)0)->modeTransitionDelay == 0x21) ? 1 : -1];

#endif /* DDS3_ADMIN_H */

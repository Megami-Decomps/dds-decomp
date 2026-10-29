#ifndef DDS3_ADMIN_H
#define DDS3_ADMIN_H

#include "common.h"

/* Shared 0x24-byte administration task state. The task creators in both titles
 * allocate this record and pass it to the mode-transition callbacks below. */
typedef struct dds3AdminiData_t {
    u32 Stat;                     /* +0x00 */
    u32 RunTimer;                 /* +0x04 */
    s8 NowSequence;               /* +0x08; -1 when no mode is active */
    s8 NextSequence;              /* +0x09; -1 when no request is pending */
    u8 HistoryPointer;            /* +0x0A */
    u8 pad0B;
    s8 SequenceHistory[8];        /* +0x0C */
    u8 FlagHistory[8];            /* +0x14 */
    void *CallData;               /* +0x1C: copied request payload, freed by the task. */
    u8 CallDataSize;              /* +0x20 */
    u8 CallCount;                 /* +0x21 */
    u8 pad22[2];
} dds3AdminiData_t;

typedef char dds3AdminiData_t_size_must_be_0x24[(sizeof(dds3AdminiData_t) == 0x24) ? 1 : -1];
typedef char dds3AdminiData_t_activeMode_at_0x08[((u32)&((dds3AdminiData_t *)0)->NowSequence == 0x08) ? 1 : -1];
typedef char dds3AdminiData_t_requestedMode_at_0x09[((u32)&((dds3AdminiData_t *)0)->NextSequence == 0x09) ? 1 : -1];
typedef char dds3AdminiData_t_historyIndex_at_0x0A[((u32)&((dds3AdminiData_t *)0)->HistoryPointer == 0x0A) ? 1 : -1];
typedef char dds3AdminiData_t_history_at_0x0C[((u32)&((dds3AdminiData_t *)0)->SequenceHistory == 0x0C) ? 1 : -1];
typedef char dds3AdminiData_t_unsignedHistory_at_0x14[((u32)&((dds3AdminiData_t *)0)->FlagHistory == 0x14) ? 1 : -1];
typedef char dds3AdminiData_t_ownedRequestData_at_0x1C[((u32)&((dds3AdminiData_t *)0)->CallData == 0x1C) ? 1 : -1];
typedef char dds3AdminiData_t_requestSize_at_0x20[((u32)&((dds3AdminiData_t *)0)->CallDataSize == 0x20) ? 1 : -1];
typedef char dds3AdminiData_t_transitionDelay_at_0x21[((u32)&((dds3AdminiData_t *)0)->CallCount == 0x21) ? 1 : -1];

/* Native DDS administration rows. Shared handler identities use HD names
 * with DDS values; Nocturne's row numbers differ. Battle keeps its DDS name:
 * its CheckNewBattleTest callback alone does not establish the whole row. */
typedef enum Dds3AdminMode {
    DDS3_ADMINI_LIST_DEBUG = 0,
    DDS3_ADMINI_LIST_BOOTMC = 1,
    DDS3_ADMINI_LIST_TITLE = 2,
    DDS3_ADMINI_LIST_LOAD = 3,
    DDS3_ADMINI_LIST_ENDSAVE = 4,
    DDS3_ADMINI_LIST_FIELD = 5,
    DDS3_ADMINI_LIST_EVENT = 6,
    DDS3_ADMINI_LIST_CAMP = 7,
    DDS3_ADMINI_LIST_RECOVER = 8,
    DDS3_ADMINI_LIST_SAVE = 9,
    DDS3_ADMINI_LIST_RESULT2 = 10,
    DDS3_ADMINI_LIST_STAFF = 12,
    DDS3_ADMINI_LIST_CONFIG = 13,
    DDS3_ADMIN_MODE_BATTLE = 14,
    DDS3_ADMINI_LIST_SAVE_TEST = 21,
    DDS3_ADMINI_LIST_SOUND_TEST = 23,
    DDS3_ADMINI_LIST_BILLBOARD_TEST = 24,
    DDS3_ADMINI_LIST_FONT_TEST = 25
} Dds3AdminMode;

#endif /* DDS3_ADMIN_H */

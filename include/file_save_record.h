#ifndef FILE_SAVE_RECORD_H
#define FILE_SAVE_RECORD_H

#include "common.h"

/* The memory-card browser copies the first 0x30 bytes of a save into a
 * compact row. The first two trailing words remain opaque. The last is a
 * saved flag word: DDS1 stores MDL flags 0x843..0x851 in its low bits,
 * while DDS2 uses bit 31 for the hard-init selection. */
typedef struct FileSavePreviewRecord {
    char signature[3];
    s8 version;
    s8 mapGroup;
    s8 mapIndex;
    u8 pad06[2];
    s32 playTicks;
    s16 status;
    s16 newCycle;
    s8 party[8];
    s8 levels[8];
    u32 money;
    union {
        u32 stateWords[3];
        struct {
            u32 opaque24;
            u32 opaque28;
            u32 savedFlags;
        } saveState;
    };
} FileSavePreviewRecord;

typedef char FileSavePreviewRecordSizeCheck[
    sizeof(FileSavePreviewRecord) == 0x30 ? 1 : -1];
typedef char FileSavePreviewRecordStateWordsOffsetCheck[
    (u32)&((FileSavePreviewRecord *)0)->stateWords == 0x24 ? 1 : -1];
typedef char FileSavePreviewRecordSavedFlagsOffsetCheck[
    (u32)&((FileSavePreviewRecord *)0)->saveState.savedFlags == 0x2C ? 1 : -1];

#endif

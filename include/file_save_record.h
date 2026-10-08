#ifndef FILE_SAVE_RECORD_H
#define FILE_SAVE_RECORD_H

#include "common.h"

/* The memory-card browser copies the first 0x30 bytes of a save into a
 * compact row. The three trailing words are retained as neutral state data;
 * their individual meanings are not shared by every title's consumers. */
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
    u32 stateWords[3];
} FileSavePreviewRecord;

typedef char FileSavePreviewRecordSizeCheck[
    sizeof(FileSavePreviewRecord) == 0x30 ? 1 : -1];
typedef char FileSavePreviewRecordStateWordsOffsetCheck[
    (u32)&((FileSavePreviewRecord *)0)->stateWords == 0x24 ? 1 : -1];

#endif

#ifndef FILE_SAVE_RECORD_H
#define FILE_SAVE_RECORD_H

#include "common.h"
#include "dat_save_words.h"

/* The memory-card browser copies the first 0x30 bytes of a save into a
 * compact row. Its final three words retain the shared save-word group. The
 * first two remain opaque. The last is a saved flag word. DDS1 stores MDL flags
 * 0x843..0x851 in its low bits, while DDS2 uses bit 31 for the hard-init
 * selection. */
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
        DatSaveWords saveWords;
        struct {
            u32 opaque24;
            u32 opaque28;
            u32 savedFlags;
        } saveState;
    };
} FileSavePreviewRecord;

typedef char FileSavePreviewRecordSizeCheck[
    sizeof(FileSavePreviewRecord) == 0x30 ? 1 : -1];
typedef char FileSavePreviewRecordSaveWordsOffsetCheck[
    (u32)&((FileSavePreviewRecord *)0)->saveWords == 0x24 ? 1 : -1];
typedef char FileSavePreviewRecordSavedFlagsOffsetCheck[
    (u32)&((FileSavePreviewRecord *)0)->saveState.savedFlags == 0x2C ? 1 : -1];

#endif

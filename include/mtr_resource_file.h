#ifndef MTR_RESOURCE_FILE_H
#define MTR_RESOURCE_FILE_H

#include "common.h"

/* One row of mtrMantraResourceFiles, the files mtrAdvanceMantraResourceLoadState
 * loads: kind 1 a sprite set into the work's resourceSlots[argument], 2 the
 * shared mantra sprites, 3 a data file (the panel-position table), 4 sound
 * bank `argument`, 5 a message file (argument 0 or 1). A load pass takes the
 * rows whose mode matches its own. */
typedef struct MtrResourceFile {
    char name[48];
    u32 kind;
    s32 argument;
    u32 mode;
} MtrResourceFile;

typedef char MtrResourceFile_size[(sizeof(MtrResourceFile) == 0x3C) ? 1 : -1];

#endif /* MTR_RESOURCE_FILE_H */

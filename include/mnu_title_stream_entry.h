#ifndef MNU_TITLE_STREAM_ENTRY_H
#define MNU_TITLE_STREAM_ENTRY_H

#include "common.h"

/* Shared row shape used by the paired title-stream tables. Byte +1 is not
 * interpreted by the observed readers and remains opaque. */
typedef struct MnuTitleStreamEntry {
    u8 format;          /* 0x00: selects the stream frame format */
    u8 opaque_01;       /* 0x01: retained without a proven meaning */
    s16 parameter;      /* 0x02: copied to the stream repeat-frame state */
    char filename[12];  /* 0x04: passed as the stream name */
} MnuTitleStreamEntry;

typedef char MnuTitleStreamEntrySizeCheck[
    sizeof(MnuTitleStreamEntry) == 0x10 ? 1 : -1];

#endif

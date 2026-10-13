#ifndef MDL_PART_ENTRY_H
#define MDL_PART_ENTRY_H

#include "common.h"

/* One model-viewer part-list slot; the request buffer uses a 0x10-byte stride. */
typedef struct MdlPartEntry {
    u32 kind;       /* 0x00 */
    s32 state;      /* 0x04 */
    void *object;   /* 0x08 */
    u8 opaque0C[4]; /* 0x0C */
} MdlPartEntry;

typedef char MdlPartEntry_size_must_be_0x10[
    (sizeof(MdlPartEntry) == 0x10) ? 1 : -1];
typedef char MdlPartEntry_object_offset_must_be_0x08[
    ((u32)&((MdlPartEntry *)0)->object == 0x08) ? 1 : -1];

#endif

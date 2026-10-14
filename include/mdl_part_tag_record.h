#ifndef MDL_PART_TAG_RECORD_H
#define MDL_PART_TAG_RECORD_H

#include "common.h"

/* Metadata retained for unhandled model parts in the grouped packet walk. */
typedef struct MdlPartTagRecord {
    u32 tag;         /* 0x00 */
    u8 state;        /* 0x04 */
    u8 opaque05[3];  /* 0x05 */
} MdlPartTagRecord;

typedef char MdlPartTagRecord_size_must_be_0x08[
    (sizeof(MdlPartTagRecord) == 0x08) ? 1 : -1];
typedef char MdlPartTagRecord_state_at_0x04[
    ((u32)&((MdlPartTagRecord *)0)->state == 0x04) ? 1 : -1];

#endif /* MDL_PART_TAG_RECORD_H */

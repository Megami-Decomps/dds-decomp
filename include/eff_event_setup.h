#ifndef EFF_EVENT_SETUP_H
#define EFF_EVENT_SETUP_H

#include "common.h"

/* A setup record occupies 0x40 bytes in the paired static data blocks.
 * The final word and the callback-adjacent words remain semantically unknown. */
typedef struct EffEventSetupRecord {
    u8 unknown00[0x1C];
    void *source;
    void *destination;
    u32 copyBytes;
    u8 unknown28[4];
    s32 (*callback)(void *);
    u8 unknown30[8];
    s32 *callbackResult;
    u32 unknown3C;
} EffEventSetupRecord;

typedef char EffEventSetupRecord_size_must_be_0x40[
    (sizeof(EffEventSetupRecord) == 0x40) ? 1 : -1];

/* Native callers pass the record address as a word; these helpers return it unchanged. */
u32 func_0018CDD0(u32 value);
u32 func_00194A08(u32 value);

#endif

#ifndef FLD_H
#define FLD_H

#include "common.h"

/* Shared +0xA58 flag word; DDS1 and DDS2 diverge at later save offsets. */
typedef struct FldWorkFlags {
    u8 pad00[0xA58];
    u32 fieldFlags; /* 0xA58 */
} FldWorkFlags;

#endif /* FLD_H */

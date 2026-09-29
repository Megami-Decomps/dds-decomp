#ifndef FLD_H
#define FLD_H

#include "common.h"

/* Field-work flag prefix (0xA5C, flags at +0xA58); DDS1/2 game/code_0011D3A0/0011F208.c. */
typedef struct FldWorkFlags {
    u8 pad00[0xA58];
    u32 fieldFlags; /* 0xA58 */
} FldWorkFlags;

#endif /* FLD_H */

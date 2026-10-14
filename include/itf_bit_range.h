#ifndef ITF_BIT_RANGE_H
#define ITF_BIT_RANGE_H

#include "common.h"

/* Adjacent packed-resource offsets delimiting one bitstream entry. */
typedef struct ItfBitRange {
    u32 start;
    u32 end;
} ItfBitRange;

typedef char ItfBitRange_size[(sizeof(ItfBitRange) == 0x08) ? 1 : -1];
typedef char ItfBitRange_start_offset[((u32)&((ItfBitRange *)0)->start == 0x00) ? 1 : -1];
typedef char ItfBitRange_end_offset[((u32)&((ItfBitRange *)0)->end == 0x04) ? 1 : -1];

#endif

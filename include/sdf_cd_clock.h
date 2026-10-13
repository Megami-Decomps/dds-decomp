#ifndef SDF_CD_CLOCK_H
#define SDF_CD_CLOCK_H

#include "common.h"

/* Byte view returned by the SDF CD clock query. */
typedef struct SdfCdClock {
    u8 status;  /* 0x00 */
    u8 second;  /* 0x01, BCD */
    u8 minute;  /* 0x02, BCD */
    u8 hour;    /* 0x03, BCD */
    u8 pad04;   /* 0x04 */
    u8 day;     /* 0x05, BCD */
    u8 month;   /* 0x06, BCD */
    u8 year;    /* 0x07 */
} SdfCdClock;

typedef char SdfCdClock_size_must_be_8[
    (sizeof(SdfCdClock) == 8) ? 1 : -1];

#endif /* SDF_CD_CLOCK_H */

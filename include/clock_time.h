#ifndef CLOCK_TIME_H
#define CLOCK_TIME_H

#include "common.h"

/* Calendar value returned by the sound clock providers. */
typedef struct ClockTime {
    u8 pad00;  /* 0x00 */
    u8 sec;    /* 0x01 */
    u8 min;    /* 0x02 */
    u8 hour;   /* 0x03 */
    u8 day;    /* 0x04 */
    u8 month;  /* 0x05 */
    u16 year;  /* 0x06 */
} ClockTime;

typedef char ClockTime_size_must_be_8[(sizeof(ClockTime) == 8) ? 1 : -1];
typedef char ClockTime_year_must_be_at_6[
    ((u32)&((ClockTime *)0)->year == 6) ? 1 : -1];

#endif /* CLOCK_TIME_H */

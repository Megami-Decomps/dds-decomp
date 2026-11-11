#ifndef EFF_RECORD_BUCKET_H
#define EFF_RECORD_BUCKET_H

#include "eff.h"

/* Indexed timed-effect table entry: bucket name, step function and its records. */
typedef struct EffRecordBucket {
    const char *name;
    /* Negative results hold phase; nonnegative results are per-update deltas. */
    s32 (*step)(BdWork *, BdWork *, EffTimedState *);
    u32 count;
    u8 *records;
} EffRecordBucket;

typedef char EffRecordBucketSizeCheck[sizeof(EffRecordBucket) == 0x10 ? 1 : -1];

#endif

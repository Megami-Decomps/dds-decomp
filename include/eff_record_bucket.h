#ifndef EFF_RECORD_BUCKET_H
#define EFF_RECORD_BUCKET_H

#include "eff.h"

/* A tunable of a timed effect: name, value kind (effConvertParamValue) and its range. */
typedef struct EffTimedParamRecord {
    const char *name;
    u32 kind;
    f32 min;
    f32 max;
    f32 step;
    f32 initial;
} EffTimedParamRecord;

/* Indexed timed-effect table entry: bucket name, step function and its records. */
typedef struct EffRecordBucket {
    const char *name;
    /* Negative results hold phase; nonnegative results are per-update deltas. */
    s32 (*step)(BdWork *, BdWork *, EffTimedState *);
    u32 count;
    EffTimedParamRecord *records;
} EffRecordBucket;

typedef char EffTimedParamRecordSizeCheck[sizeof(EffTimedParamRecord) == 0x18 ? 1 : -1];
typedef char EffRecordBucketSizeCheck[sizeof(EffRecordBucket) == 0x10 ? 1 : -1];

#endif

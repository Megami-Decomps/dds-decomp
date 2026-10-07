#ifndef EFF_RECORD_BUCKET_H
#define EFF_RECORD_BUCKET_H

#include "eff.h"

/* Indexed timed-effect table entry; the first word's meaning is unknown. */
typedef struct EffRecordBucket {
    u32 unk_00;
    s32 (*step)(BdWork *, BdWork *, EffTimedState *);
    u32 count;
    u8 *records;
} EffRecordBucket;

typedef char EffRecordBucketSizeCheck[sizeof(EffRecordBucket) == 0x10 ? 1 : -1];

#endif

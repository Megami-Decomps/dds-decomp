#ifndef BTL_STAGE_TEST_TASK_H
#define BTL_STAGE_TEST_TASK_H

#include "common.h"

/* Stage-test cleanup view. Only the kind, allocation, resource handle, and
 * payload start are read by the paired cleanup callbacks; intervening bytes
 * stay opaque, and the allocation's total extent is not established here. */
typedef struct StageTestTaskWork {
    u8 pad00;
    u8 kind;             /* 0x01: kind 6 selects this owner */
    u8 pad02[6];
    void *allocation;    /* 0x08: chip block released by cleanup */
    s32 resource;        /* 0x0C: optional device-queue state */
    u8 pad10[0x20];
    u8 payload[1];       /* 0x30: packet-queue state passed to release */
} StageTestTaskWork;


#endif /* BTL_STAGE_TEST_TASK_H */

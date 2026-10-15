#ifndef SDF_DEV_PATH_PREFIX_H
#define SDF_DEV_PATH_PREFIX_H

#include "common.h"

/* Six-byte device prefix copied before the path suffix is appended. The
 * compiler's natural tail padding is not part of the accessed prefix. */
typedef struct DevPathPrefix {
    s32 head;
    u16 tail;
} DevPathPrefix;

typedef char DevPathPrefix_tail_must_be_at_4[
    ((u32)&((DevPathPrefix *)0)->tail == 4) ? 1 : -1];

#endif /* SDF_DEV_PATH_PREFIX_H */

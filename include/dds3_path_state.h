#ifndef DDS3_PATH_STATE_H
#define DDS3_PATH_STATE_H

#include "common.h"

/* Kind-6 world nodes keep this state in EffWorldNode.data. Only the word at
 * +0x10 is currently identified; the payload prefix remains opaque. */
typedef struct Dds3PathStatePayload {
    u8 opaque00[0x10];
    u32 value;
} Dds3PathStatePayload;

typedef char Dds3PathStatePayload_size_must_be_0x14[
    (sizeof(Dds3PathStatePayload) == 0x14) ? 1 : -1];
typedef char Dds3PathStatePayload_value_at_0x10[
    ((u32)&((Dds3PathStatePayload *)0)->value == 0x10) ? 1 : -1];

#endif

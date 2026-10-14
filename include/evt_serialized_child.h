#ifndef EVT_SERIALIZED_CHILD_H
#define EVT_SERIALIZED_CHILD_H

#include "common.h"

/* One child row emitted by the event-viewer track serializer. */
typedef struct EvtSerializedChild {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    u8 opaque08[4];
    s32 body[8];
} EvtSerializedChild;

typedef char EvtSerializedChild_size_check[
    (sizeof(EvtSerializedChild) == 0x2C) ? 1 : -1];
typedef char EvtSerializedChild_groupType_offset_check[
    ((u32)&((EvtSerializedChild *)0)->groupType == 0x00) ? 1 : -1];
typedef char EvtSerializedChild_start_offset_check[
    ((u32)&((EvtSerializedChild *)0)->start == 0x02) ? 1 : -1];
typedef char EvtSerializedChild_span_offset_check[
    ((u32)&((EvtSerializedChild *)0)->span == 0x04) ? 1 : -1];
typedef char EvtSerializedChild_value_offset_check[
    ((u32)&((EvtSerializedChild *)0)->value == 0x06) ? 1 : -1];
typedef char EvtSerializedChild_opaque_offset_check[
    ((u32)&((EvtSerializedChild *)0)->opaque08 == 0x08) ? 1 : -1];
typedef char EvtSerializedChild_body_offset_check[
    ((u32)&((EvtSerializedChild *)0)->body == 0x0C) ? 1 : -1];

#endif /* EVT_SERIALIZED_CHILD_H */

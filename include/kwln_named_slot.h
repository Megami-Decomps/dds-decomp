#ifndef KWLN_NAMED_SLOT_H
#define KWLN_NAMED_SLOT_H

#include "common.h"

/* Linked named-task slots shared by the DDS1 and DDS2 kernel windows. */
typedef struct KwlnNamedSlot {
    s32 kind;               /* 0x00 */
    char name[32];          /* 0x04 */
    void *task;             /* 0x24: task passed to scheduler APIs. */
    void *opaque28;         /* 0x28: cleared by unregister; contents unknown. */
    void *opaque2C;         /* 0x2C: cleared by unregister; contents unknown. */
    void *opaque30;         /* 0x30: cleared by unregister; contents unknown. */
    s32 opaque34;           /* 0x34: cleared by unregister; contents unknown. */
    void *opaque38;         /* 0x38: cleared by unregister; contents unknown. */
    s32 previous;           /* 0x3C: previous slot index. */
    s32 next;               /* 0x40: next slot index. */
} KwlnNamedSlot;

typedef char KwlnNamedSlot_size_must_be_44[(sizeof(KwlnNamedSlot) == 0x44) ? 1 : -1];
typedef char KwlnNamedSlot_task_offset_must_be_24[
    ((u32)&((KwlnNamedSlot *)0)->task == 0x24) ? 1 : -1];
typedef char KwlnNamedSlot_previous_offset_must_be_3C[
    ((u32)&((KwlnNamedSlot *)0)->previous == 0x3C) ? 1 : -1];
typedef char KwlnNamedSlot_next_offset_must_be_40[
    ((u32)&((KwlnNamedSlot *)0)->next == 0x40) ? 1 : -1];

#endif /* KWLN_NAMED_SLOT_H */

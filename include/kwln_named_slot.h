#ifndef KWLN_NAMED_SLOT_H
#define KWLN_NAMED_SLOT_H

#include "common.h"
#include "kwln.h"

/* Linked named-task slots shared by the DDS1 and DDS2 kernel windows. */
typedef struct fclNamedSlot_t {
    s32 kind;               /* 0x00 */
    char name[32];          /* 0x04 */
    dds3ProcessID_t *task;         /* 0x24: task passed to scheduler APIs. */
    dds3ProcessFunc_t update; /* 0x28 */
    dds3ProcessShutdownFunc_t destroy; /* 0x2C */
    void (*setState)(s32);  /* 0x30: state callback. */
    s32 enabled;            /* 0x34 */
    void *userValue;        /* 0x38 */
    s32 previous;           /* 0x3C: previous slot index. */
    s32 next;               /* 0x40: next slot index. */
} fclNamedSlot_t;

typedef char fclNamedSlot_size_must_be_44[(sizeof(fclNamedSlot_t) == 0x44) ? 1 : -1];
typedef char fclNamedSlot_task_offset_must_be_24[
    ((u32)&((fclNamedSlot_t *)0)->task == 0x24) ? 1 : -1];
typedef char fclNamedSlot_previous_offset_must_be_3C[
    ((u32)&((fclNamedSlot_t *)0)->previous == 0x3C) ? 1 : -1];
typedef char fclNamedSlot_next_offset_must_be_40[
    ((u32)&((fclNamedSlot_t *)0)->next == 0x40) ? 1 : -1];

#endif /* KWLN_NAMED_SLOT_H */

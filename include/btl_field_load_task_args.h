#ifndef BTL_FIELD_LOAD_TASK_ARGS_H
#define BTL_FIELD_LOAD_TASK_ARGS_H

#include "common.h"

struct FilePacRequest;

/* Complete 0x28-byte payload allocated by the paired field-load task creators. */
typedef struct BtlFieldLoadArgs {
    s32 stage;                         /* 0x00: area used to form the field path */
    s32 variant;                       /* 0x04: field variant used in the path */
    struct FilePacRequest *request; /* 0x08: pending PAC archive request */
    u8 pad0C[0xC];                     /* 0x0C: opaque state retained by the task */
    void *fieldF1;                     /* 0x18: loaded F1 resource */
    void *fieldF2;                     /* 0x1C: loaded F2 resource */
    void *fieldTB;                     /* 0x20: loaded texture-offset resource */
    s32 frame;                         /* 0x24: callback phase/frame */
} BtlFieldLoadArgs;

typedef char BtlFieldLoadArgs_size[(sizeof(BtlFieldLoadArgs) == 0x28) ? 1 : -1];
typedef char BtlFieldLoadArgs_stage_offset[((u32)&((BtlFieldLoadArgs *)0)->stage == 0x00) ? 1 : -1];
typedef char BtlFieldLoadArgs_variant_offset[((u32)&((BtlFieldLoadArgs *)0)->variant == 0x04) ? 1 : -1];
typedef char BtlFieldLoadArgs_request_offset[((u32)&((BtlFieldLoadArgs *)0)->request == 0x08) ? 1 : -1];
typedef char BtlFieldLoadArgs_pad_offset[((u32)&((BtlFieldLoadArgs *)0)->pad0C == 0x0C) ? 1 : -1];
typedef char BtlFieldLoadArgs_fieldF1_offset[((u32)&((BtlFieldLoadArgs *)0)->fieldF1 == 0x18) ? 1 : -1];
typedef char BtlFieldLoadArgs_fieldF2_offset[((u32)&((BtlFieldLoadArgs *)0)->fieldF2 == 0x1C) ? 1 : -1];
typedef char BtlFieldLoadArgs_fieldTB_offset[((u32)&((BtlFieldLoadArgs *)0)->fieldTB == 0x20) ? 1 : -1];
typedef char BtlFieldLoadArgs_frame_offset[((u32)&((BtlFieldLoadArgs *)0)->frame == 0x24) ? 1 : -1];

#endif /* BTL_FIELD_LOAD_TASK_ARGS_H */

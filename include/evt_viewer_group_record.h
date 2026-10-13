#ifndef EVT_VIEWER_GROUP_RECORD_H
#define EVT_VIEWER_GROUP_RECORD_H

#include "common.h"

typedef struct EvtViewerGroupRecord {
    u8 opaque00[8];
    s32 group;       /* 0x08 */
    s32 type;        /* 0x0C */
    u8 opaque10[0x10];
} EvtViewerGroupRecord;

typedef char EvtViewerGroupRecord_group_at_08[
    ((u32)&((EvtViewerGroupRecord *)0)->group == 0x08) ? 1 : -1];
typedef char EvtViewerGroupRecord_type_at_0C[
    ((u32)&((EvtViewerGroupRecord *)0)->type == 0x0C) ? 1 : -1];
typedef char EvtViewerGroupRecord_size_must_be_0x20[
    (sizeof(EvtViewerGroupRecord) == 0x20) ? 1 : -1];

#endif

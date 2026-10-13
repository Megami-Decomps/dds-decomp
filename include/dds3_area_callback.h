#ifndef DDS3_AREA_CALLBACK_H
#define DDS3_AREA_CALLBACK_H

#include "common.h"

typedef struct Dds3AreaCallbackRecordView {
    u8 opaque00[0x0C];
    s32 (*callback)(void);
} Dds3AreaCallbackRecordView;

typedef struct Dds3AreaObjectView {
    u8 opaque00[0x10];
    Dds3AreaCallbackRecordView *callbackRecord;
} Dds3AreaObjectView;

typedef char Dds3AreaCallbackOffsetCheck[((u32)&((Dds3AreaCallbackRecordView *)0)->callback == 0x0C) ? 1 : -1];
typedef char Dds3AreaRecordPointerOffsetCheck[((u32)&((Dds3AreaObjectView *)0)->callbackRecord == 0x10) ? 1 : -1];

s32 dds3InvokeAreaCallback(Dds3AreaObjectView *areaObject);

#endif

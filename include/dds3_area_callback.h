#ifndef DDS3_AREA_CALLBACK_H
#define DDS3_AREA_CALLBACK_H

#include "common.h"
#include "dds3_basic_array_visit.h"

typedef struct Dds3AreaObjectView Dds3AreaObjectView;

typedef struct Dds3AreaCallbackRecordView {
    u8 opaque00[0x0C];
    /* Exact zero stops the owning world's array traversal. */
    Dds3BasicArrayVisitResult (*callback)(Dds3AreaObjectView *areaObject);
} Dds3AreaCallbackRecordView;

struct Dds3AreaObjectView {
    u8 opaque00[0x10];
    Dds3AreaCallbackRecordView *callbackRecord;
};

typedef char Dds3AreaCallbackOffsetCheck[((u32)&((Dds3AreaCallbackRecordView *)0)->callback == 0x0C) ? 1 : -1];
typedef char Dds3AreaRecordPointerOffsetCheck[((u32)&((Dds3AreaObjectView *)0)->callbackRecord == 0x10) ? 1 : -1];

Dds3BasicArrayVisitResult CB_ReleaseObjectBasic(dds3Basic_t *areaObject);
Dds3BasicArrayVisitResult CB_BasicDrawProcuder(dds3Basic_t *areaObject);

#endif

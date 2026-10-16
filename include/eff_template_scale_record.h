#ifndef EFF_TEMPLATE_SCALE_RECORD_H
#define EFF_TEMPLATE_SCALE_RECORD_H

#include "common.h"

/* Template packet overlay: an initial tag at +0x20 and scale at +0x34. */
typedef struct EffTemplateScaleRecord {
    u8 opaque00[0x20];
    u32 initialTag;
    u8 opaque24[0x10];
    f32 scale;
    u8 opaque38[8];
} EffTemplateScaleRecord;

typedef char EffTemplateScaleRecordSizeCheck[(sizeof(EffTemplateScaleRecord) == 0x40) ? 1 : -1];
typedef char EffTemplateScaleRecordInitialTagOffsetCheck[
    ((u32)&((EffTemplateScaleRecord *)0)->initialTag == 0x20) ? 1 : -1];
typedef char EffTemplateScaleRecordScaleOffsetCheck[
    ((u32)&((EffTemplateScaleRecord *)0)->scale == 0x34) ? 1 : -1];

#endif

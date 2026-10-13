#ifndef DSP_NAME_H
#define DSP_NAME_H

#include "common.h"

/* Fixed-stride encoded mantra-name rows used by the display and script APIs. */
typedef struct DspMantraName {
    u8 encodedText[19];
} DspMantraName;

/* Unit-name tables use a 0x11-byte stride in message and menu clients. */
typedef struct DspUnitName {
    u8 encodedText[17];
} DspUnitName;

typedef char DspMantraName_size_must_be_19[(sizeof(DspMantraName) == 19) ? 1 : -1];
typedef char DspUnitName_size_must_be_17[(sizeof(DspUnitName) == 17) ? 1 : -1];

#endif

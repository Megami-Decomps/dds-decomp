#ifndef DSP_NAME_H
#define DSP_NAME_H

#include "common.h"

/* Fixed-stride encoded mantra-name rows used by the display and script APIs. */
typedef struct DspMantraName {
    u8 encodedText[19];
} DspMantraName;

typedef char DspMantraName_size_must_be_19[(sizeof(DspMantraName) == 19) ? 1 : -1];

#endif

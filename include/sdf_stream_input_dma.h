#ifndef SDF_STREAM_INPUT_DMA_H
#define SDF_STREAM_INPUT_DMA_H

#include "common.h"

/* One 16-byte tag in the IPU input DMA chain. */
typedef struct SdfStreamInputDmaTag {
    u64 control;
    u64 reserved;
} SdfStreamInputDmaTag;

typedef char SdfStreamInputDmaTag_size_must_be_0x10[
    (sizeof(SdfStreamInputDmaTag) == 0x10) ? 1 : -1];

#endif /* SDF_STREAM_INPUT_DMA_H */

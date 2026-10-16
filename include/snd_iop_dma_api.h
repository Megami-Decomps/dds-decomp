#ifndef SND_IOP_DMA_API_H
#define SND_IOP_DMA_API_H

#include "common.h"

void sndCopyToIopAndWait(s32 destination, const void *source, s32 size);

#endif /* SND_IOP_DMA_API_H */

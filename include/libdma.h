#ifndef LIBDMA_H
#define LIBDMA_H

#include "common.h"

typedef struct sceDmaChan {
    u32 chcr; u32 pad00[3];
    u32 madr; u32 pad10[3];
    u32 qwc;  u32 pad20[3];
    u32 tadr; u32 pad30[3];
    u32 asr0; u32 pad40[3];
    u32 asr1; u32 pad50[3];
    u32 pad60[8];
    u32 sadr;
} sceDmaChan;

#endif

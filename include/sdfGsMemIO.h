#ifndef SDF_GS_MEM_IO_H
#define SDF_GS_MEM_IO_H

#include "common.h"

typedef struct sdfStoreTransfer_t {
    s32 length;
    u32 address;
    void (*callback)(u32 parameter);
    u32 callbackParameter;
} sdfStoreTransfer_t;

#endif

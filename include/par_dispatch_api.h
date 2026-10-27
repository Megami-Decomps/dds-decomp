#ifndef PAR_DISPATCH_API_H
#define PAR_DISPATCH_API_H

#include "common.h"

struct ParObj;

/* Typed consumer view of one 0xC-byte particle dispatch row. The first
 * callback creates from template data or copies a live emitter. */
typedef struct ParDispatchApi {
    void *(*createOrCopy)(void *source);
    void (*update)(struct ParObj *object);
    void (*destroy)(void *object);
} ParDispatchApi; /* 0x0C */

typedef char ParDispatchApi_size_must_be_0x0C[
    (sizeof(ParDispatchApi) == 0x0C) ? 1 : -1];

extern ParDispatchApi dds3ParticleFuncTable[];

#endif

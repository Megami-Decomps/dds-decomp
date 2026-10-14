#ifndef EFF_MATRIX4_H
#define EFF_MATRIX4_H

#include "common.h"

/* Effect transform consumed as four float columns or four VU0/DMA rows. */
typedef struct Matrix4 {
    union {
        float m[4][4];
        s128 rows[4];
    } u;
} Matrix4;

typedef char Matrix4_size_must_be_0x40[
    sizeof(Matrix4) == 0x40 ? 1 : -1];

#endif

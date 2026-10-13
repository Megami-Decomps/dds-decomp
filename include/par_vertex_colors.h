#ifndef PAR_VERTEX_COLORS_H
#define PAR_VERTEX_COLORS_H

#include "common.h"

/* Packed color words written in the vertex order used by each cell geometry. */
typedef struct ParTriangleVertexColors {
    s32 edge0;  /* 0x00 */
    s32 middle; /* 0x04 */
    s32 edge1;  /* 0x08 */
} ParTriangleVertexColors;

typedef struct ParStripVertexColors {
    s32 edge0;   /* 0x00 */
    s32 middle0; /* 0x04 */
    s32 center0; /* 0x08 */
    s32 center1; /* 0x0C */
    s32 middle1; /* 0x10 */
    s32 edge1;   /* 0x14 */
} ParStripVertexColors;

typedef struct ParQuadVertexColors {
    s32 edge0;   /* 0x00 */
    s32 middle0; /* 0x04 */
    s32 middle1; /* 0x08 */
    s32 edge1;   /* 0x0C */
} ParQuadVertexColors;

typedef struct ParSymmetricVertexColors {
    s32 edge0;   /* 0x00 */
    s32 middle0; /* 0x04 */
    s32 center;  /* 0x08 */
    s32 middle1; /* 0x0C */
    s32 edge1;   /* 0x10 */
} ParSymmetricVertexColors;

typedef char ParTriangleVertexColors_size_must_be_0x0C[
    sizeof(ParTriangleVertexColors) == 0x0C ? 1 : -1];
typedef char ParStripVertexColors_size_must_be_0x18[
    sizeof(ParStripVertexColors) == 0x18 ? 1 : -1];
typedef char ParQuadVertexColors_size_must_be_0x10[
    sizeof(ParQuadVertexColors) == 0x10 ? 1 : -1];
typedef char ParSymmetricVertexColors_size_must_be_0x14[
    sizeof(ParSymmetricVertexColors) == 0x14 ? 1 : -1];

#endif /* PAR_VERTEX_COLORS_H */

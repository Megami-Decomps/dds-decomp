#ifndef EFF_OFFSET_TRIANGLE_H
#define EFF_OFFSET_TRIANGLE_H

#include "common.h"

/* Access view consumed by effAppendOffsetTriangle; opaque spans are retained
 * because this helper only reads or writes the listed fields. */
typedef struct EffOffsetTriangleView {
    u16 flags;                 /* 0x00: mask 0x2 enables normals; 0x4 enables colors */
    u8 opaque02[0x0A];         /* 0x02 */
    s32 count;                 /* 0x0C: output vertex index, advanced by three */
    u8 opaque10[0x34];         /* 0x10 */
    f32 (*positions)[4];       /* 0x44: four-float output vectors */
    u32 opaque48;              /* 0x48 */
    f32 (*normals)[4];         /* 0x4C: optional four-float output vectors */
    u32 *colors;               /* 0x50: optional output colors */
} EffOffsetTriangleView;

typedef char EffOffsetTriangleView_size_must_be_0x54[
    (sizeof(EffOffsetTriangleView) == 0x54) ? 1 : -1];

#endif

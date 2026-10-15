#ifndef DDS_POLY_RING_HEAD_H
#define DDS_POLY_RING_HEAD_H

#include "common.h"

/* Shared 0xC0 template prefix copied by the band, arc, and rotating-band
 * owners. Their parameter/resource tails are distinct and remain local. */
typedef struct PolyRingHead {
    f32 origin[4];
    u32 entryCount; /* 0x10: number of trailing per-entry records. */
    s32 duration;
    u32 templateSize; /* Byte length copied from the source template. */
    u8 pad1C[4];
    f32 matrix[16];
    u32 color;
    u8 loop;
    u8 pad65;
    u16 drawBucket; /* 0x66: selected particle draw bucket. */
    u32 startColorRampFrames;
    u32 endColorRampFrames;
    f32 matrixCopy[16]; /* Initialized from matrix when the owner is created. */
    u8 padB0[2];
    u16 active; /* Cleared when every entry finishes. */
    u8 padB4[0xC];
} PolyRingHead;

typedef char PolyRingHead_size[(sizeof(PolyRingHead) == 0xC0) ? 1 : -1];
typedef char PolyRingHead_entryCount_offset[((u32)&((PolyRingHead *)0)->entryCount == 0x10) ? 1 : -1];
typedef char PolyRingHead_matrix_offset[((u32)&((PolyRingHead *)0)->matrix == 0x20) ? 1 : -1];
typedef char PolyRingHead_color_offset[((u32)&((PolyRingHead *)0)->color == 0x60) ? 1 : -1];
typedef char PolyRingHead_matrixCopy_offset[((u32)&((PolyRingHead *)0)->matrixCopy == 0x70) ? 1 : -1];
typedef char PolyRingHead_drawBucket_offset[((u32)&((PolyRingHead *)0)->drawBucket == 0x66) ? 1 : -1];
typedef char PolyRingHead_active_offset[((u32)&((PolyRingHead *)0)->active == 0xB2) ? 1 : -1];

#endif

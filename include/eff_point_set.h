#ifndef EFF_POINT_SET_H
#define EFF_POINT_SET_H

#include "common.h"

struct SdfMemBlock;
struct SdfAsset;

/* Point-set node: `rows` 16-byte entries in `buffer`, then `tail`. */
typedef struct EffPointSet {
    u32 type;       // 0x00
    u32 color;      // 0x04
    s32 rows;       // 0x08
    u8 flag;        // 0x0C
    u8 pad_0D[3];
    u8 *buffer;     // 0x10
    u8 *tail;       // 0x14
    struct SdfAsset *handle; // 0x18
    struct SdfMemBlock *allocation; // 0x1C
} EffPointSet;

typedef char EffPointSetSizeCheck[sizeof(EffPointSet) == 0x20 ? 1 : -1];
typedef char EffPointSetHandleOffsetCheck[
    ((u32)&((EffPointSet *)0)->handle == 0x18) ? 1 : -1];
typedef char EffPointSetAllocationOffsetCheck[
    ((u32)&((EffPointSet *)0)->allocation == 0x1C) ? 1 : -1];

EffPointSet *effCreatePointSet5(s32 count);

#endif

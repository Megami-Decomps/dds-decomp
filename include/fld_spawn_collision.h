#ifndef FLD_SPAWN_COLLISION_H
#define FLD_SPAWN_COLLISION_H

#include "common.h"

typedef struct FldSpawnCollision {
    u32 kind;             /* 0x00 */
    u32 opaque04;
    u32 *header;          /* 0x08: passed to the geometry collision builder */
    u32 opaque0C;
    u8 opaque10[0x20];
} FldSpawnCollision;

typedef char FldSpawnCollision_kind_at_0[
    ((u32)&((FldSpawnCollision *)0)->kind == 0x00) ? 1 : -1];
typedef char FldSpawnCollision_header_at_8[
    ((u32)&((FldSpawnCollision *)0)->header == 0x08) ? 1 : -1];
typedef char FldSpawnCollision_size_must_be_0x30[
    (sizeof(FldSpawnCollision) == 0x30) ? 1 : -1];

#endif

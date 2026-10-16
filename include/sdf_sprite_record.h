#ifndef SDF_SPRITE_RECORD_H
#define SDF_SPRITE_RECORD_H

#include "common.h"

typedef struct SdfVec4 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} SdfVec4;

/* The packer writes the opaque header bytes, then relocates this vector. */
typedef struct SdfSpriteVectorRecord {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    s16 unk_14;
    u8 unk_16;
    u8 unk_17;
    SdfVec4 *vector;
} SdfSpriteVectorRecord;

typedef char SdfVec4SizeCheck[(sizeof(SdfVec4) == 0x10) ? 1 : -1];
typedef char SdfSpriteVectorRecordSizeCheck[(sizeof(SdfSpriteVectorRecord) == 0x1C) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk00OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_00 == 0x00) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk04OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_04 == 0x04) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk08OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_08 == 0x08) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk0COffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_0C == 0x0C) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk10OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_10 == 0x10) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk14OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_14 == 0x14) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk16OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_16 == 0x16) ? 1 : -1];
typedef char SdfSpriteVectorRecordUnk17OffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->unk_17 == 0x17) ? 1 : -1];
typedef char SdfSpriteVectorRecordVectorOffsetCheck[((u32)&((SdfSpriteVectorRecord *)0)->vector == 0x18) ? 1 : -1];

#endif

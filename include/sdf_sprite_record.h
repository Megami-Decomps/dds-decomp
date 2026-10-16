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

/* The packer writes this extended header and relocates its vector at +0x2C. */
typedef struct SdfSpriteExtendedRecord {
    u32 unk_00;
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    s16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    u32 unk_14;
    u32 unk_18;
    u32 unk_1C;
    u32 unk_20;
    u8 inlineData[8];
    SdfVec4 *vector;
} SdfSpriteExtendedRecord;

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
typedef char SdfSpriteExtendedRecordSizeCheck[(sizeof(SdfSpriteExtendedRecord) == 0x30) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk00OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_00 == 0x00) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk04OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_04 == 0x04) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk06OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_06 == 0x06) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk08OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_08 == 0x08) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk0AOffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_0A == 0x0A) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk0COffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_0C == 0x0C) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk0EOffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_0E == 0x0E) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk10OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_10 == 0x10) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk12OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_12 == 0x12) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk14OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_14 == 0x14) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk18OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_18 == 0x18) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk1COffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_1C == 0x1C) ? 1 : -1];
typedef char SdfSpriteExtendedRecordUnk20OffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->unk_20 == 0x20) ? 1 : -1];
typedef char SdfSpriteExtendedRecordInlineDataOffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->inlineData == 0x24) ? 1 : -1];
typedef char SdfSpriteExtendedRecordVectorOffsetCheck[((u32)&((SdfSpriteExtendedRecord *)0)->vector == 0x2C) ? 1 : -1];

#endif

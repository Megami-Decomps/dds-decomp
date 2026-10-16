#ifndef SDF_WORD_ARRAY_RECORD_H
#define SDF_WORD_ARRAY_RECORD_H

#include "common.h"

typedef struct SdfWordArrayInfo {
    u32 words[4];
} SdfWordArrayInfo;

typedef struct SdfWordArrayRecord {
    u32 unk_00;
    u32 unk_04;
    s16 unk_08;
    u16 unk_0A;
    u8 opaque0C[2];
    u16 itemCount;
    u32 *items;
    SdfWordArrayInfo *info;
} SdfWordArrayRecord;

typedef char SdfWordArrayRecordLayoutAssert[
    (sizeof(SdfWordArrayInfo) == 0x10 &&
     sizeof(SdfWordArrayRecord) == 0x18 &&
     (unsigned long)&((SdfWordArrayRecord *)0)->unk_00 == 0x00 &&
     (unsigned long)&((SdfWordArrayRecord *)0)->unk_04 == 0x04 &&
     (unsigned long)&((SdfWordArrayRecord *)0)->unk_08 == 0x08 &&
     (unsigned long)&((SdfWordArrayRecord *)0)->unk_0A == 0x0A &&
     (unsigned long)&((SdfWordArrayRecord *)0)->opaque0C == 0x0C &&
     (unsigned long)&((SdfWordArrayRecord *)0)->itemCount == 0x0E &&
     (unsigned long)&((SdfWordArrayRecord *)0)->items == 0x10 &&
     (unsigned long)&((SdfWordArrayRecord *)0)->info == 0x14) ? 1 : -1];

#endif /* SDF_WORD_ARRAY_RECORD_H */

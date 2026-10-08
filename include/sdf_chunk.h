#ifndef SDF_CHUNK_H
#define SDF_CHUNK_H

#include "common.h"

struct SdfModel;

/* Variable-size serialized chunk streams share only this eight-byte header.
 * The word at +8 belongs to each chunk's payload. */
typedef struct SdfChunkHeader {
    u32 id;   /* 0x00: zero terminates the chunk stream */
    u32 size; /* 0x04: byte extent to the next chunk */
} SdfChunkHeader;

typedef char SdfChunkHeader_size_must_be_8[(sizeof(SdfChunkHeader) == 8) ? 1 : -1];

/* MPOS has an eight-byte opaque prefix after the common header, then 0x40-byte
 * rows. Its enclosing chunk extent remains variable and is stored in size. */
typedef struct SdfMapPositionChunkPrefix {
    SdfChunkHeader header;
    u8 payload08[8];
} SdfMapPositionChunkPrefix;

typedef struct SdfMapPositionRecord {
    u32 nodeId;      /* 0x00: draw-node lookup key */
    s32 id;          /* 0x04: map-position record lookup key */
    u8 reserved08[8];
    u128 position;   /* 0x10 */
    u128 up;         /* 0x20 */
    u128 direction;  /* 0x30 */
} SdfMapPositionRecord;

typedef char SdfMapPositionChunkPrefix_size_must_be_0x10[
    (sizeof(SdfMapPositionChunkPrefix) == 0x10) ? 1 : -1];
typedef char SdfMapPositionRecord_size_must_be_0x40[
    (sizeof(SdfMapPositionRecord) == 0x40) ? 1 : -1];
typedef char SdfMapPositionRecord_id_at_4[
    ((u32)&((SdfMapPositionRecord *)0)->id == 4) ? 1 : -1];
typedef char SdfMapPositionRecord_position_at_10[
    ((u32)&((SdfMapPositionRecord *)0)->position == 0x10) ? 1 : -1];
typedef char SdfMapPositionRecord_up_at_20[
    ((u32)&((SdfMapPositionRecord *)0)->up == 0x20) ? 1 : -1];
typedef char SdfMapPositionRecord_direction_at_30[
    ((u32)&((SdfMapPositionRecord *)0)->direction == 0x30) ? 1 : -1];

SdfChunkHeader *sdfChunkFindById(SdfChunkHeader *chunk, s32 chunkId);
SdfChunkHeader *sdfChunkFindByTag(struct SdfModel *model, s32 tag);
u32 sdfCountMapPositionRecords(struct SdfModel *model);
SdfMapPositionRecord *sdfChunkFindRecordById(struct SdfModel *model, s32 recordId);
void sdfSetLookAtBasisFromRecord(struct SdfModel *model, SdfMapPositionRecord *record);
void sdfVuTransformMapRecordPosition(struct SdfModel *model, SdfMapPositionRecord *record);

#endif /* SDF_CHUNK_H */

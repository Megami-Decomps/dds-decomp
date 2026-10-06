#ifndef FLD_H
#define FLD_H

#include "common.h"

/* Field-work flag prefix (0xA5C, flags at +0xA58); DDS1/2 game/code_0011D3A0/0011F208.c. */
typedef struct FldWorkFlags {
    u8 pad00[0xA58];
    u32 fieldFlags; /* 0xA58 */
} FldWorkFlags;

/* Sequence command packet: the submission contract copies all 0xA0 bytes. */
typedef struct FieldSequenceRecord {
    u8 unk_00[0x30];
    u32 unk_30;
    u32 unk_34;
    u32 unk_38;
    u32 unk_3c;
    char name[16];
    s32 stage;
    s32 kind;
    s32 enabled;
    s32 mode;
    u16 code;
    u16 unk_62;
    s32 link;
    u8 unk_68[8];
    char detail[16];
    char note[16];
    u32 options; /* 0x90: record options */
    u8 pad94[0xC];
} FieldSequenceRecord;

/* Packed actor placement rows used by field searches, motion and sound dispatch.
 * Both games use 0x6C-byte rows, with different signed flags and sequence fields. */
typedef struct FldActorEntry {
    s8 kind; /* 0x00 */
    u8 pad01;
    s16 requiredFlag; /* 0x02: zero or a model-flag ID */
    s16 floor; /* 0x04: one-based floor */
    char name[0xC]; /* 0x06 */
    s16 motion; /* 0x12 */
    s16 secondaryMotion; /* 0x14 */
    s16 sound; /* 0x16 */
#ifdef VERSION_DDS1
    char motionName[0xC]; /* 0x18 */
    char otherName[0xC]; /* 0x24 */
    s8 variantMode; /* 0x30 */
    s8 flags31;
    s16 variant;
    s16 sequenceKind; /* 0x34: zero selects the default kind */
    u8 pad36[2];
    char sequenceName[0xC]; /* 0x38 */
    s8 linkKind; /* 0x44 */
    s8 rowIndex;
    char linkName[0xC]; /* 0x46 */
    s8 sequenceCode; /* 0x52 */
    s8 selectedRoom; /* 0x53: optional one-based room */
    s8 flags54;
    char taskName[0xF]; /* 0x55 */
    u8 flags64;
    s8 unk65;
    s8 unk66;
    s8 value67;
    s8 unk68;
    s8 unk69;
    s8 unk6A;
    s8 unk6B;
#endif
#ifdef VERSION_DDS2
    u8 motionName[0xC]; /* 0x18 */
    u8 otherName[0xC]; /* 0x24 */
    s8 variantMode; /* 0x30 */
    u8 flags31;
    s16 variant;
    s16 warpEntry; /* 0x34 */
    s16 warpEntry2;
    char warpName[0xC]; /* 0x38 */
    s8 linkKind; /* 0x44 */
    s8 unk45;
    char linkName[0xC]; /* 0x46 */
    s8 unk52;
    s8 unk53;
    s8 flags54;
    char pad55[0xF];
    u8 flags64;
    s8 unk65;
    s8 unk66;
    s8 value67;
    s8 unk68;
    s8 unk69;
    s8 unk6A;
    s8 unk6B;
#endif
} FldActorEntry;

/* Collision faces are 0x24-byte wire records in the field resource.
 * The room-record builders retain the face and stop-table addresses. */
typedef struct FldCollisionFace {
    u32 attributes;
    u8 moveFloor;
    u8 sound;
    u16 stop;
    u16 place;
    u8 automap[2];
    u32 vertexIndices[4];
    s16 encounterType;
    s16 encounter;
    s16 special[2];
} FldCollisionFace;

/* Both room-record builders construct this complete 0xE4-byte owner.
 * Zone classifiers use the bounding-plane prefix of the same record. */
typedef struct FldValueRecord {
    s16 mode;
    s16 count;
    f32 normal[4];
    f32 planeConstant;
    f32 plane[4][4]; /* 0x18 */
    f32 limit[4]; /* 0x58 */
    f32 bound[4]; /* 0x68: min0, min1, max0, max1 */
    f32 vertices[4][4]; /* 0x78 */
    f32 previousPosition[3]; /* 0xB8: saved actor world position */
    u32 unkC4;
    u16 unkC8;
    u16 unkCA;
    s32 id;
    s32 value; /* Record API word; some callers store an actor address. */
    FldCollisionFace *face;
    void *stopData;
    s32 sceneFlag;
    s32 unkE0;
} FldValueRecord;

typedef char FldCollisionFaceSizeCheck[(sizeof(FldCollisionFace) == 0x24) ? 1 : -1];
typedef char FldValueRecordSizeCheck[(sizeof(FldValueRecord) == 0xE4) ? 1 : -1];

/* fldFileResolver::func_001263F0 publishes file rows; func_00126A30 walks them
 * at stride 0x24. Both game resolvers walk counted names at stride 0x0C.
 * Name tables have variable extent; entries[1] is the native struct-hack head. */
typedef struct FldFileNameEntry {
    u32 word00;
    const char *name;
    u32 word08;
} FldFileNameEntry;

typedef struct FldFileNameTable {
    u32 count;
    FldFileNameEntry entries[1];
} FldFileNameTable;

typedef struct FldFileResource {
    u32 id;
    u32 word04;
    const char *name;
    u32 word0C;
    f32 *transform;
    u32 word14;
    FldFileNameTable *names;
    u32 word1C;
    void *data;
} FldFileResource;

typedef char FldFileNameEntrySizeCheck[(sizeof(FldFileNameEntry) == 0x0C) ? 1 : -1];
typedef char FldFileResourceSizeCheck[(sizeof(FldFileResource) == 0x24) ? 1 : -1];

#endif /* FLD_H */

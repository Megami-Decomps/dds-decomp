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
    u8 unk65;
    u8 unk66;
    s8 value67;
    u8 unk68;
    u8 unk69;
    u8 unk6A;
    u8 unk6B;
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

#endif /* FLD_H */

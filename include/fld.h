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

#endif /* FLD_H */

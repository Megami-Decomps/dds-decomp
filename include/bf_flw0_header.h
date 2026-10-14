#ifndef BF_FLW0_HEADER_H
#define BF_FLW0_HEADER_H

#include "scr.h"

/* FLW0 file header shared by the parser and the procedure-name lookup. */
typedef struct BfFlw0Header {
    u8 unk00[8];
    u32 magic;
    u8 unk0C[4];
    s32 sectionCount;
    s16 intCount;
    s16 floatCount;
    u8 unk18[8];
    ScrSection sections[1];
} BfFlw0Header;

typedef char BfFlw0Header_size_must_be_0x30[
    (sizeof(BfFlw0Header) == 0x30) ? 1 : -1];
typedef char BfFlw0Header_magic_offset_must_be_8[
    ((unsigned long)&((BfFlw0Header *)0)->magic == 0x08) ? 1 : -1];
typedef char BfFlw0Header_sectionCount_offset_must_be_0x10[
    ((unsigned long)&((BfFlw0Header *)0)->sectionCount == 0x10) ? 1 : -1];
typedef char BfFlw0Header_sections_offset_must_be_0x20[
    ((unsigned long)&((BfFlw0Header *)0)->sections == 0x20) ? 1 : -1];

#endif /* BF_FLW0_HEADER_H */
